#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
#include <math.h>
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

void TIM2_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init_us(u32 us);
void TIM2_Int_Stop(void);

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init_us(u32 us); // us ��65536������׼ȷ�ģ��ٸߵĻ��ᱻ�ض�һ��. ���6553500
void TIM3_Int_Stop(void);

/////// TIM4��usmartռ��


void TIM5_Int_Init(u16 arr,u16 psc);
void TIM5_Int_Init_us(u32 us);
void TIM5_Int_Stop(void);

#endif




