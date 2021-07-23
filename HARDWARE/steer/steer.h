#ifndef __STEER_H
#define __STEER_H

#include "pwm.h"

void steer1_init(void);//请将TIM13管脚配置为PA6（在文件pwm.h中），以其为信号输出口。此函数初始化的信号可用于操作9g舵机
void steer1_set_angle(u8 angle); //0-180
void steer1_set_compare(u16); //500-2500（用PWM原生方法控制角度，500对应0度，2500对应180度）

void steer2_init(void);//请将TIM14管脚配置为PA7（在文件pwm.h中），以其为信号输出口。此函数初始化的信号可用于操作9g舵机
void steer2_set_angle(u8 angle); //0-180
void steer2_set_compare(u16); //500-2500（用PWM原生方法控制角度，500对应0度，2500对应180度）
















#endif

