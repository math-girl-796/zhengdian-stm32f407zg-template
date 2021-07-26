from fpioa_manager import fm
from machine import UART, Timer
from utime import sleep_ms, sleep_us
import sensor
import lcd
from Maix import GPIO
import time
import uos




############################################################# 函数定义 #################################################################
#定义函数，在一组色块中返回最大的一个
def find_biggest_blob(blobs):
    biggest_pixels = 0
    biggest_blob = None
    if blobs is None:
        return None
    for blobx in blobs:
        if blobx.pixels() > biggest_pixels:
            biggest_pixels = blobx.pixels()
            biggest_blob = blobx
    return biggest_blob

def on_timer0(timer):
    global fps
    #拍照
    img = sensor.snapshot()
    #根据指令反转照片的灰度值
    if negate:
        img = img.negate()
    #找色块
    blobs = img.find_blobs([my_threshold], roi=roi)
    biggest_blob = find_biggest_blob(blobs)
    if biggest_blob is not None:
        #在图片上画方块，在IDE显示图片和提示信息
        if shvs:
            img.draw_rectangle(biggest_blob[0:4]) # 画碰撞箱
            img.draw_string(2,2, ("fps:  %2.1f" %(fps)), color=str_color, scale=2) # 画帧率
            img.draw_string(2,22,("roi:  %d %d %d %d" %(roi[0], roi[1], roi[2], roi[3])), color=str_color, scale=2) # 画roi
            img.draw_string(2,42,("thres:%d %d" %(my_threshold[0], my_threshold[1])), color=str_color, scale=2) # 画roi
            img.draw_string(2,62,("negate: " + str(negate)), color=str_color, scale=2) # 画roi
            #print(biggest_blob)
            lcd.display(img)  # 这个语句会花费大量时间，导致帧率降到20以下
        #通过串口1发送色块中心坐标
        uart_A.write(bytearray([0xb3, 0xb3, 0x01])) # 帧头  指令帧
        x = (biggest_blob.x() + biggest_blob.w() // 2).to_bytes(2, 'little')
        y = (biggest_blob.y() + biggest_blob.h() // 2).to_bytes(2, 'little')
        uart_A.write(x + y)
    else:
        #print("None")
        pass

    # 打印帧率
    fps = clock.fps()
    #print(fps)
    clock.tick()

def my_lcd_init():
    lcd.init()
    lcd.rotation(2)

# 保存参数：roi，threshold，ovfps
def save_parametors():
    file_object = open('./k210_pm.txt', 'w')
    try:
        # 参数转为字符串，以空格隔开
        string = ' '.join([str(_) for _ in [roi[0], roi[1], roi[2], roi[3], my_threshold[0], my_threshold[1], ovfps, negate]])
        file_object.write(string)
        print("写入成功：", string)
    except:
        print("打开文件失败")
    finally:
        file_object.close()

def load_parametors():
    file_object = open('./k210_pm.txt', 'r')
    try:
        strs = file_object.read().split()
        print("读取到：", strs)
        roi = [int(_) for _ in strs[0:4]]
        my_threshold = [int(_) for _ in strs[4:6]]
        ovfps = int(strs[6])
        negate = bool(strs[7])
    except:
        print("打开文件失败")
    finally:
        file_object.close()

############################################################# 主程序 #################################################################
# 设置找色块时感兴趣的图片范围。QVGA是320*240的，这里把感兴趣区域设置为正方形（靠近左侧，为了方便摄像头位置的校准）
sensor.reset(dual_buff=True)
################## 图像阈值 ###################
#sensor.set_pixformat(sensor.RGB565)
#my_threshold = (0, 18, 0, 122, 0, 122)
sensor.set_pixformat(sensor.GRAYSCALE)
my_threshold = [0, 18]
#############################################
################### 图像大小 ##################
#sensor.set_framesize(sensor.QQVGA)
#roi = (0, 0, 120, 120) # (x, y, w, h)
sensor.set_framesize(sensor.QVGA)
#############################################
sensor.run(1)
sensor.skip_frames()


# 全局变量
roi = [0, 0, 320, 240] # (x, y, w, h)
shvs = False
fps = 0
ovfps = 10
key_flag = False
negate = False
str_color = (238, 252, 58) # img.draw_string 颜色

# 打印初始变量
print("当前目录:", uos.getcwd())  # 文件系统
print("/flash:", uos.listdir("/flash"))
print("/sd:", uos.listdir("/flash"))

# 初始化uart1
# uart1协议：[0xb3, 0xb3, cmd, data * 8]
# cmd: 0x01-数据，0x02-信息
fm.register(15,fm.fpioa.UART1_TX)
fm.register(17,fm.fpioa.UART1_RX)
uart_A = UART(UART.UART1, 115200, 8, None, 1, timeout=1000, read_buf_len=4096)
uart_A.read(4096) # 初始化操作，防止第一次读uart缓冲区的时候丢掉开头的字符

# 初始化led，用来标识定时器是否工作
io_led_green = 12
fm.register(io_led_green, fm.fpioa.GPIO0)
led_g=GPIO(GPIO.GPIO0, GPIO.OUT)
led_g.value(0)


# 初始化键盘和钟表
fm.register(16, fm.fpioa.GPIOHS16)
key = GPIO(GPIO.GPIOHS16, GPIO.IN)
clock = time.clock()  # 获得一个钟表对象

# 可能初始化lcd
if shvs == True:
    my_lcd_init()

# 初始化定时器对象，并且启动
# 40hz
tim = Timer(0, 0, mode=Timer.MODE_PERIODIC, period=1000//ovfps, unit=Timer.UNIT_MS, callback=on_timer0, arg=None, start=True, priority=1, div=0)




while True:


    # 处理f4传来的指令
    uart_buf = uart_A.read(4096)
    if uart_buf is not None:
        try:
            f4_command = uart_buf.decode('ascii')
            print("收到指令：", f4_command)
            if f4_command == "start":
                tim.restart()
                led_g.value(0)
            elif f4_command == "stop":
                tim.stop()
                led_g.value(1)
            elif f4_command == "shvs":
                my_lcd_init()
                shvs = True
            elif f4_command == "stpshvs":
                lcd.deinit()
                shvs = False
            elif f4_command.startswith("roi"):
                cmds = f4_command.split(',')
                try:
                    roi[0] = int(cmds[1])
                    roi[1] = int(cmds[2])
                    roi[2] = int(cmds[3])
                    roi[3] = int(cmds[4])
                    print(roi)
                except:
                    print("change roi error")
            elif f4_command.startswith("ovfps"):
                cmds = f4_command.split(',')
                print(cmds)
                try:
                    ovfps = int(cmds[1])
                    tim.period(1000//ovfps)
                    tim.restart()
                except:
                    print("change ovfps error")
                print(tim)
            elif f4_command == "svpm":
                save_parametors()
            elif f4_command == "ldpm":
                load_parametors()
            elif f4_command.startswith("thres"):
                cmds = f4_command.split(',')
                try:
                    my_threshold[0] = int(cmds[1])
                    my_threshold[1] = int(cmds[2])
                    print(my_threshold)
                except:
                    print("change threshold error")
            elif f4_command == "negate":
                negate = not negate
                print("negate:", negate)
            f4_command = ""

        except:
            pass

    # 处理按键信息（按下启动/停止拍照）

    if key.value() == 0:
        sleep_ms(200)
        if key_flag:
            tim.restart()
            led_g.value(0)
            print('key: timer restart')
        else:
            tim.stop()
            led_g.value(1)
            print('key: timer stop')
        key_flag = not key_flag


