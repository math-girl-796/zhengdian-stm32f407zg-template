#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

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
