#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//9g�����ѹ��5v
//���ⷢ������ѹ��5v
//���Ʒ��ȿ�ֱ����pwmģ�飺ռ�ձ�Ϊ1��Ϊȫ������

//TIM12_CH2��ѡ�ܽţ�PB15
#define TIM12_CH2_PWM_GPIOx				GPIOB
#define TIM12_CH2_PWM_GPIO_Pin			GPIO_Pin_15
#define TIM12_CH2_PWM_GPIO_Pin_Source	GPIO_PinSource15
#define TIM12_CH2_PWM_GPIO_RCC_FUN		RCC_AHB1PeriphClockCmd
#define TIM12_CH2_PWM_GPIO_RCC_ADDR		RCC_AHB1Periph_GPIOB
#define TIM12_SELF_RCC_FUN				RCC_APB1PeriphClockCmd
#define TIM12_SELF_RCC_ADDR				RCC_APB1Periph_TIM12
void TIM12_CH2_PWM_Init_us(u32 us);		
void TIM12_CH2_PWM_Set_Duration(float high_percentage);
void TIM12_CH2_PWM_Set_Compare(u32 cmp);
void TIM12_CH2_PWM_Init(u32 arr,u32 psc);
	
//TIM13_CH1��ѡ�ܽţ�PA6, PF8
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


//TIM14_CH1��ѡ�ܽţ�PF9��LED0����PA7
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
