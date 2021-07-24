from fpioa_manager import fm
from machine import UART, Timer
from utime import sleep_ms, sleep_us
import sensor
import lcd
from Maix import GPIO
import time


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
    if shot:
        #拍照
        img = sensor.snapshot()
        blobs = img.find_blobs([my_threshold], roi=roi)
        biggest_blob = find_biggest_blob(blobs)
        #找色块：找到了
        if biggest_blob is not None:
            #在图片上画方块，在IDE显示图片和提示信息
            #img.draw_rectangle(biggest_blob[0:4])
            #print(biggest_blob)
            #lcd.display(img)  # 这个语句会花费大量时间，导致帧率降到20以下
            #通过串口1发送色块中心坐标
            uart_A.write(bytearray([0xb3, 0xb3]))
            x = (biggest_blob.x() + biggest_blob.w() // 2).to_bytes(2, 'little')
            y = (biggest_blob.y() + biggest_blob.h() // 2).to_bytes(2, 'little')
            uart_A.write(x + y)
        else:
            print("None")

        # 打印帧率
        #print(clock.fps())
        #clock.tick()


############################################################# 主程序 #################################################################
# 设置找色块时感兴趣的图片范围。QVGA是320*240的，这里把感兴趣区域设置为正方形（靠近左侧，为了方便摄像头位置的校准）
sensor.reset(dual_buff=True)
################## 图像阈值 ###################
#sensor.set_pixformat(sensor.RGB565)
#my_threshold = (0, 18, 0, 122, 0, 122)
sensor.set_pixformat(sensor.GRAYSCALE)
my_threshold = (0, 18)
#############################################
################### 图像大小 ##################
#sensor.set_framesize(sensor.QQVGA)
#roi = (0, 0, 120, 120) # (x, y, w, h)
sensor.set_framesize(sensor.QVGA)
roi = (0, 0, 240, 240) # (x, y, w, h)
#############################################
sensor.run(1)
sensor.skip_frames()

# 初始化uart1
fm.register(15,fm.fpioa.UART1_TX)
fm.register(17,fm.fpioa.UART1_RX)
uart_A = UART(UART.UART1, 115200, 8, None, 1, timeout=1000, read_buf_len=4096)
uart_A.read(4096) # 初始化操作，防止第一次读uart缓冲区的时候丢掉开头的字符

# 初始化键盘和钟表
fm.register(16, fm.fpioa.GPIOHS16)
key = GPIO(GPIO.GPIOHS16, GPIO.IN)
clock = time.clock()  # 获得一个钟表对象

# 初始化定时器对象，并且启动
# 40hz
tim = Timer(0, 0, mode=Timer.MODE_PERIODIC, period=1000//40, unit=Timer.UNIT_MS, callback=on_timer0, arg=None, start=True, priority=1, div=0)

shot = True
while True:


    # 处理f4传来的指令
    uart_buf = uart_A.read(4096)
    if uart_buf is not None:
        try:
            f4_command = uart_buf.decode('ascii')
            if f4_command == "start":
                shot = True
            elif f4_command == "stop":
                shot = False
        except:
            pass

    # 处理按键信息（按下启动/停止拍照）
    if key.value() == 0:
        sleep_ms(200)
        shot = not shot

    ## 获取图像，进行处理，然后发送给f4
    #if shot:
        ##拍照
        #img = sensor.snapshot()
        #blobs = img.find_blobs([my_threshold], roi=roi)
        #biggest_blob = find_biggest_blob(blobs)
        ##找色块：找到了
        #if biggest_blob is not None:
            ##在图片上画方块，在IDE显示图片和提示信息
            ##img.draw_rectangle(biggest_blob[0:4])
            #print(biggest_blob)
            ##lcd.display(img)  # 这个语句会花费大量时间，导致帧率降到20以下
            ##通过串口1发送色块中心坐标
            #x = (biggest_blob.x() + biggest_blob.w() // 2).to_bytes(2, "little")
            #y = (biggest_blob.y() + biggest_blob.h() // 2).to_bytes(2, "little")
            #uart_A.write(x)
            #uart_A.write(y)
            #uart_A.write(b'\x0d\x0a')  # end of sentence
        #else:
            #print("None")

            ## 打印帧率
            #print(clock.fps())
            #clock.tick()

    sleep_ms(50)


