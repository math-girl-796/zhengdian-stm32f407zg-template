#ifndef __KEY_H
#define __KEY_H

#include "sys.h"


#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)		//��Ӧ����KEY0
#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)		//��Ӧ����KEY1


//����:��
//����ֵ:��
//���ܣ�ʹ����������KEY0��KEY1
void key_init(void);

//����:��
//����ֵ��	���ء�1������ʾ����KEY0
//			���򣬷��ء�0��
//����:�жϰ���KEY0�Ĺ���״̬
uint8_t key0_state(void);

//����:��
//����ֵ��	���ء�1������ʾ����KEY1
//			���򣬷��ء�0��
//����:�жϰ���KEY_UP�Ĺ���״̬
uint8_t key1_state(void);

#endif
