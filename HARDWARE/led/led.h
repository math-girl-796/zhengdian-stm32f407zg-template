#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED0 0 //��Ӧ��������xx��jiao��led��
#define LED1 1 //


//������


//����ֵ��



//���ܣ�
void led_init(void);


//������led����ѡ���룺LED0��LED1
void led_on(uint8_t led);


//���ܣ��رն�Ӧ��led
//������led����ѡֵ��LED0��LED1
void led_off(uint8_t led);


//���ܣ�ʹѡ���LED��״̬�л�
//������led����ѡֵ��LED0��LED1
void led_switch(uint8_t led);


//���ܣ��鿴ѡ���LED�Ƶ�״̬
//������led����ѡֵ��LED0��LED1
//������벻�Ϸ�������ֵ��ȷ��
uint8_t led_state(uint8_t led);

#endif
