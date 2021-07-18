#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED0 0 //对应开发板上xx管jiao的led等
#define LED1 1 //


//参数：


//返回值：



//功能：
void led_init(void);


//参数：led：可选输入：LED0，LED1
void led_on(uint8_t led);


//功能：关闭对应的led
//参数：led：可选值：LED0，LED1
void led_off(uint8_t led);


//功能：使选择的LED灯状态切换
//参数：led：可选值：LED0，LED1
void led_switch(uint8_t led);


//功能：查看选择的LED灯的状态
//参数：led：可选值：LED0，LED1
//如果输入不合法：返回值不确定
uint8_t led_state(uint8_t led);



// 一些led闪烁模式，详见函数定义。并不是设置闪烁模式，而是通过delay实现。
void led_blink1(uint8_t LEDx);
void led_blink2(uint8_t LEDx);
void led_blink3(uint8_t LEDx);
void led_blink4(uint8_t LEDx);
void led_blink5(uint8_t LEDx);


#endif
