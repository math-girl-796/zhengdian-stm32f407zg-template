#ifndef  __STEPPER_MOTOR_H
#define  __STEPPER_MOTOR_H
#include "sys.h"
#include "stdio.h"
#include "sys.h"
#include "delay.h"

/* ���������ӵ�GPIO�˿�, �û�ֻ��Ҫ�޸�����Ĵ��뼴�ɸı���Ƶ�LED���� */
#define LA_GPIO_PORT    	GPIOC		                /* GPIO�˿� */
#define LA_GPIO_CLK 	    RCC_AHB1Periph_GPIOC		/* GPIO�˿�ʱ�� */
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

//void Stepper_motorStop(void);//ֹͣ

//direction ��ѡ���ΪCW/CCW���ֱ��ʾ˳ʱ�����ʱ��
//void OneStep4(u8 direction);
//void OneStep8(u8 direction);

//TΪתһȦ��ʱ��   ע�⣺T��СΪ6000��6000ʱ���п��ܳ��ֿ������
//void OneRound4(u8 direction, int T);

//TΪתһȦ��ʱ��
//void OneRound8(u8 direction, int T);


//��Ȼ�ƣ�������T=7000��Ϊ��������ʱ.

//angle��ת�Ƕȣ�  TΪתһȦ��ʱ�䣨��λ��ms��T��Ҫ����7000����
//����ʹ����������ʵ�֣��Ƕ�ԽС�����Խ��
//С�Ƕ�ʱ��ʹ��stepper_motor_ctrl_512����
void stepper_motor_ctrl(u8 direction, int angle, int T);

//���ڶ���������stepper_motor_ctrl��ͬ
//factorΪ512ʱתһȦ��factorÿ��һ����ת0.703125�Ƕ�
void stepper_motor_ctrl_512(u8 direction, int factor, int T);

#endif
