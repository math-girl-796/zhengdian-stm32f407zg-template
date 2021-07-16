#ifndef  __STEPPER_MOTOR_H
#define  __STEPPER_MOTOR_H
#include "sys.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"

/* 定义电机连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LA_GPIO_PORT    	GPIOC		                /* GPIO端口 */
#define LA_GPIO_CLK 	    RCC_AHB1Periph_GPIOC		/* GPIO端口时钟 */
#define LA_GPIO_PIN			GPIO_Pin_0

#define LB_GPIO_PORT    	GPIOC
#define LB_GPIO_CLK 	    RCC_AHB1Periph_GPIOC
#define LB_GPIO_PIN		    GPIO_Pin_1

#define LC_GPIO_PORT    	GPIOC
#define LC_GPIO_CLK 	    RCC_AHB1Periph_GPIOC
#define LC_GPIO_PIN		    GPIO_Pin_2

#define LD_GPIO_PORT    	GPIOC
#define LD_GPIO_CLK 	    RCC_AHB1Periph_GPIOC
#define LD_GPIO_PIN		    GPIO_Pin_3

#define CW 1
#define CCW 0

void stepper_motor_init(void);

//void Stepper_motorStop(void);//停止

//direction 可选项均为CW/CCW，分别表示顺时针和逆时针
//void OneStep4(u8 direction);
//void OneStep8(u8 direction);

//T为转一圈的时间   注意：T最小为6000，6000时还有可能出现卡顿情况
//void OneRound4(u8 direction, int T);

//T为转一圈的时间
//void OneRound8(u8 direction, int T);


//段然云：建议以T=7000作为电机最低延时.

//angle旋转角度；  T为转一圈的时间（单位：ms；T需要大于7000）；
//由于使用整数除法实现，角度越小，误差越大
//小角度时请使用stepper_motor_ctrl_512函数
void stepper_motor_ctrl(u8 direction, int angle, int T);

//仅第二个参数与stepper_motor_ctrl不同
//factor为512时转一圈，factor每加一，多转0.703125角度
void stepper_motor_ctrl_512(u8 direction, int factor, int T);

#endif
