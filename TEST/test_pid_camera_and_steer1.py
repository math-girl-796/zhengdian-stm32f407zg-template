from fpioa_manager import fm
from machine import UART
from utime import sleep_ms
import sensor
import lcd

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.run(1)
sensor.skip_frames()
lcd.init(freq=15000000)

fm.register(15,fm.fpioa.UART1_TX)
fm.register(17,fm.fpioa.UART1_RX)
fm.register(9,fm.fpioa.UART2_TX)
fm.register(10,fm.fpioa.UART2_RX)
uart_A = UART(UART.UART1, 115200, 8, None, 1, timeout=1000, read_buf_len=4096)
uart_B = UART(UART.UART2, 115200, 8, None, 1, timeout=1000, read_buf_len=4096)

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


while True:
    #拍照
    img = sensor.snapshot()
    blobs = img.find_blobs([(0, 100, 32, 96, 23, 91)])
    biggest_blob = find_biggest_blob(blobs)
    #找色块：找到了
    if biggest_blob is not None:
        #在图片上画方块，在IDE显示图片和提示信息
        img.draw_rectangle(biggest_blob[0:4])
        print(biggest_blob)
        lcd.display(img)
        #通过串口1发送色块横坐标
        x = (biggest_blob.x() + biggest_blob.w() // 2).to_bytes(2, "little")
        eos = b'\x0d\x0a' # end of sentence
        uart_A.write(x)
        uart_A.write(eos)
    else:
        print("None")



    uart_buf = uart_B.read(4096)
    if uart_buf is not None and len(uart_buf) >= 4:
        newest_bytes = uart_buf[-4:-2]
        print(int.from_bytes(newest_bytes, "little"), uart_buf[-2:].decode('ascii')) # 读最新数据

    sleep_ms(20)
