#ifndef __KEY_H
#define __KEY_H

#include "sys.h"


#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)		//对应按键KEY0
#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)		//对应按键KEY1


//参数:无
//返回值:无
//功能：使能两个按键KEY0与KEY1
void key_init(void);

//参数:无
//返回值：	返回“1”，表示按下KEY0
//			否则，返回“0”
//功能:判断按键KEY0的工作状态
uint8_t key0_state(void);

//参数:无
//返回值：	返回“1”，表示按下KEY1
//			否则，返回“0”
//功能:判断按键KEY_UP的工作状态
uint8_t key1_state(void);

#endif
