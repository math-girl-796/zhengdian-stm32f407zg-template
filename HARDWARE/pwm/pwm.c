#ifndef __PWM_H
#define __PWM_H

#include "pwm.h"
#include "led.h"
#include "usart.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ��PWM ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

static u32 TIM14_CH1_PWM_Cycle_us;

//��us��������PWM�Ĳ�������
void TIM14_CH1_PWM_Init_us(u32 us)
{
	TIM14_CH1_PWM_Cycle_us = us;
	TIM14_CH1_PWM_Init(us, 84);
}

//������ռ�ձȣ�һ�������иߵ�ƽ��ռ�ı���������Χ0-1��
void TIM14_CH1_PWM_Set_Duration(float high_percentage)
{
	TIM_SetCompare1(TIM14, TIM14_CH1_PWM_Cycle_us * high_percentage);
}

//cmpԽ�󣬸ߵ�ƽռһ�����ڵı���Խ��
void TIM14_CH1_PWM_Set_Compare(u32 cmp)
{
	TIM_SetCompare1(TIM14, cmp);
}

//TIM14 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
//��ʱ��ʱ��84,000,000hz
void TIM14_CH1_PWM_Init(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM14_SELF_RCC_FUN(TIM14_SELF_RCC_ADDR, ENABLE);  	//TIM14ʱ��ʹ��    
	TIM14_CH1_PWM_GPIO_RCC_FUN(TIM14_CH1_PWM_GPIO_RCC_ADDR, ENABLE); 	//ʹ��PORTFʱ��	
	
	GPIO_PinAFConfig(TIM14_CH1_PWM_GPIOx,TIM14_CH1_PWM_GPIO_Pin_Source,GPIO_AF_TIM14); //GPIOF9����Ϊ��ʱ��14
	
	GPIO_InitStructure.GPIO_Pin = TIM14_CH1_PWM_GPIO_Pin;           //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(TIM14_CH1_PWM_GPIOx,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//��ʼ����ʱ��14
	
	//��ʼ��TIM14 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM14, ENABLE);  //ʹ��TIM14
 
	TIM_SetCompare1(TIM14, 0);	//��ʼ��ռ�ձ�Ϊ0					  
}  



#endif
