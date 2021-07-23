#include "control.h"

// 来自main.c
extern float kp, ki, kd;// pid参数
extern u32 ctrl_cycle; // 定时器定时周期，单位us
extern struct obj obj; // 视觉坐标

/* 此处写控制代码 */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		led_switch(LED0);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}























