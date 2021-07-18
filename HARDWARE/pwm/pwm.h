#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/6/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//TIM14_CH1可选管脚：PF9（LED0），PA7
#define TIM14_CH1_PWM_GPIOx				GPIOA
#define TIM14_CH1_PWM_GPIO_Pin			GPIO_Pin_7
#define TIM14_CH1_PWM_GPIO_Pin_Source	GPIO_PinSource7
#define TIM14_CH1_PWM_GPIO_RCC_FUN		RCC_AHB1PeriphClockCmd
#define TIM14_CH1_PWM_GPIO_RCC_ADDR		RCC_AHB1Periph_GPIOA
#define TIM14_SELF_RCC_FUN				RCC_APB1PeriphClockCmd
#define TIM14_SELF_RCC_ADDR				RCC_APB1Periph_TIM14
void TIM14_CH1_PWM_Init_us(u32 us);		
void TIM14_CH1_PWM_Set_Duration(float high_percentage);
void TIM14_CH1_PWM_Set_Compare(u32 cmp);
void TIM14_CH1_PWM_Init(u32 arr,u32 psc);
#endif
