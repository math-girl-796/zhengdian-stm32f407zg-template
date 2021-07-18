#ifndef __ROCKER_H
#define __ROCKER_H
#include "sys.h"
#include "delay.h"

// 取一次rocker_x坐标耗时：ADC_MEAN_TIME * ADC_MEAN_DELAY_MS + 3次转换耗时
// rocker_y同理
#define ADC_MEAN_TIME 3 // adc转换精度：取 ADC_MEAN_TIME 次转换的平均值
#define ADC_MEAN_DELAY_MS 2 // adc两次转换之间的延时

// 使用ADC1的两个通道
// 可选通道：PA0-PA7，PB0-PB1，PC0-PC5
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
