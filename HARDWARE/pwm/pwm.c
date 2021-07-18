#ifndef __PWM_H
#define __PWM_H

#include "pwm.h"
#include "led.h"
#include "usart.h"
 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//定时器PWM 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

static u32 TIM14_CH1_PWM_Cycle_us;

//以us长度设置PWM的波形周期
void TIM14_CH1_PWM_Init_us(u32 us)
{
	TIM14_CH1_PWM_Cycle_us = us;
	TIM14_CH1_PWM_Init(us, 84);
}

//参数：占空比（一个周期中高电平所占的比例）。范围0-1。
void TIM14_CH1_PWM_Set_Duration(float high_percentage)
{
	TIM_SetCompare1(TIM14, TIM14_CH1_PWM_Cycle_us * high_percentage);
}

//cmp越大，高电平占一个周期的比例越大
void TIM14_CH1_PWM_Set_Compare(u32 cmp)
{
	TIM_SetCompare1(TIM14, cmp);
}

//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
//定时器时钟84,000,000hz
void TIM14_CH1_PWM_Init(u32 arr,u32 psc)
{		 					 
	//此部分需手动修改IO口设置
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	TIM14_SELF_RCC_FUN(TIM14_SELF_RCC_ADDR, ENABLE);  	//TIM14时钟使能    
	TIM14_CH1_PWM_GPIO_RCC_FUN(TIM14_CH1_PWM_GPIO_RCC_ADDR, ENABLE); 	//使能PORTF时钟	
	
	GPIO_PinAFConfig(TIM14_CH1_PWM_GPIOx,TIM14_CH1_PWM_GPIO_Pin_Source,GPIO_AF_TIM14); //GPIOF9复用为定时器14
	
	GPIO_InitStructure.GPIO_Pin = TIM14_CH1_PWM_GPIO_Pin;           //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(TIM14_CH1_PWM_GPIOx,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);//初始化定时器14
	
	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
 
	TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM14, ENABLE);  //使能TIM14
 
	TIM_SetCompare1(TIM14, 0);	//初始化占空比为0					  
}  



#endif
