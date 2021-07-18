#ifndef __ROCKER_H
#define __ROCKER_H
#include "sys.h"
#include "delay.h"

// ȡһ��rocker_x�����ʱ��ADC_MEAN_TIME * ADC_MEAN_DELAY_MS + 3��ת����ʱ
// rocker_yͬ��
#define ADC_MEAN_TIME 3 // adcת�����ȣ�ȡ ADC_MEAN_TIME ��ת����ƽ��ֵ
#define ADC_MEAN_DELAY_MS 2 // adc����ת��֮�����ʱ

// ʹ��ADC1������ͨ��
// ��ѡͨ����PA0-PA7��PB0-PB1��PC0-PC5
#define ROCKER_X_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define ROCKER_X_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOA
#define ROCKER_X_GPIO			GPIOA
#define ROCKER_X_GPIO_PIN		GPIO_Pin_4
#define ROCKER_X_ADC_CHANNEL	4

#define ROCKER_Y_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define ROCKER_Y_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOA
#define ROCKER_Y_GPIO			GPIOA
#define ROCKER_Y_GPIO_PIN		GPIO_Pin_5
#define ROCKER_Y_ADC_CHANNEL	5

#define ROCKER_SW_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define ROCKER_SW_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOC
#define ROCKER_SW_GPIO			GPIOC
#define ROCKER_SW_GPIO_PIN		GPIO_Pin_0



void rocker_init(void);
int16_t rocker_x(void);
int16_t rocker_y(void);
u8 rocker_sw(void);


#endif
