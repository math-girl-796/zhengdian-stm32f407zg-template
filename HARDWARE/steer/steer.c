#include "steer.h"

void steer1_init(void)
{
	TIM13_CH1_PWM_Init_us(20000);
}


void steer1_set_angle(u8 angle)
{
	TIM13_CH1_PWM_Set_Compare(500 + 2000 * (int) angle / 180);
}

void steer1_set_compare(u16 cmp)
{
	TIM13_CH1_PWM_Set_Compare(cmp);
}

void steer2_init(void)
{
	TIM14_CH1_PWM_Init_us(20000);
}


void steer2_set_angle(u8 angle)
{
	TIM14_CH1_PWM_Set_Compare(500 + 2000 * (int) angle / 180);
}

void steer2_set_compare(u16 cmp)
{
	TIM14_CH1_PWM_Set_Compare(cmp);
}


