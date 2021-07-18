#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//9g舵机电压：5v
//激光发射器电压：5v
	
//TIM13_CH1可选管脚：PA6, PF8
#define TIM13_CH1_PWM_GPIOx				GPIOA
#define TIM13_CH1_PWM_GPIO_Pin			GPIO_Pin_6
#define TIM13_CH1_PWM_GPIO_Pin_Source	GPIO_PinSource6
#define TIM13_CH1_PWM_GPIO_RCC_FUN		RCC_AHB1PeriphClockCmd
#define TIM13_CH1_PWM_GPIO_RCC_ADDR		RCC_AHB1Periph_GPIOA
#define TIM13_SELF_RCC_FUN				RCC_APB1PeriphClockCmd
#define TIM13_SELF_RCC_ADDR				RCC_APB1Periph_TIM13
void TIM13_CH1_PWM_Init_us(u32 us);		
void TIM13_CH1_PWM_Set_Duration(float high_percentage);
void TIM13_CH1_PWM_Set_Compare(u32 cmp);
void TIM13_CH1_PWM_Init(u32 arr,u32 psc);


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
