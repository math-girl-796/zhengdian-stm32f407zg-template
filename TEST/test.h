#ifndef __TEST_H
#define __TEST_H

#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "led.h"
#include "key.h"
#include "stepper_motor.h"
#include "pwm.h"
#include "steer.h"
#include "utils.h"
#include "lcd.h"
#include "touch.h"
#include "rocker.h"
#include "usb_conf.h" 
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"
#include "usmart.h"
#include "sram.h"  
#include "keyboard.h"
#include "flash.h"
#include "timer.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

void test_uart1(void);
void test_uart2(void);
void test_uart3(void);
void test_uart4(void);
void test_uart5(void);
void test_uart6(void);
void test_led(void);
void test_key(void); //uart1���ӵ��ԣ�ͨ��XCOM�鿴��֤
void test_stepper_motor(void);
void test_pwm_TIM13_CH1(void);
void test_pwm_TIM14_CH1(void);
void test_pwm_and_uart1(void);
void test_steer1_and_uart1(void); // ���
void test_fun(void); //����
void test_pid_camera_and_steer1(void);
void test_parse_bytes_and_uart1(void);
void test_lcd_show(void);
void test_lcd_touch_screen(void);
void test_rocker(void);
///////////////////////////////
//������ʵ����Ҫ�������ļ��Ĵ���Ķ������ܹ��档����ֻ�������õļ��̲���
//void test_usb_slave(void);//��Ƭ������������
//void test_usb_host(void);//��Ƭ����ȡ�洢�豸����ļ�
void test_keyboard(void);
///////////////////////////////
void test_fatfs(void);
void test_flash(void);
void test_timer3(void);
void test_timer2(void);
void test_timer5(void);
void test_mpu6050(void);
void test_iic_24c02(void);
void test_time(void);
void test_steers(void);



















#endif


