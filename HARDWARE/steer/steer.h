#ifndef __STEER_H
#define __STEER_H

#include "pwm.h"

void steer1_init(void);//�뽫TIM13�ܽ�����ΪPA6�����ļ�pwm.h�У�������Ϊ�ź�����ڡ��˺�����ʼ�����źſ����ڲ���9g���
void steer1_set_angle(u8 angle); //0-180
void steer1_set_compare(u16); //500-2500����PWMԭ���������ƽǶȣ�500��Ӧ0�ȣ�2500��Ӧ180�ȣ�

void steer2_init(void);//�뽫TIM14�ܽ�����ΪPA7�����ļ�pwm.h�У�������Ϊ�ź�����ڡ��˺�����ʼ�����źſ����ڲ���9g���
void steer2_set_angle(u8 angle); //0-180
void steer2_set_compare(u16); //500-2500����PWMԭ���������ƽǶȣ�500��Ӧ0�ȣ�2500��Ӧ180�ȣ�
















#endif

