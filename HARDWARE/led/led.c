
#include "led.h"

//ԭ����led.h�õ�λ�������������led.h���ᱻδ֪�ĺ�������
void led_init(void)
{
	/* ʹ�ܵ��� */
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOFʱ��

	//GPIOF9,F10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//LED0��LED1��ӦIO��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��GPIO

	
	/* �������ݣ�ʹ���ʼ����Ĭ��״̬Ϊ�ر� */
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10���øߣ�����
}


void led_on(uint8_t led)
{
	switch(led)
	{
		case LED0:
			GPIO_ResetBits(GPIOF,GPIO_Pin_9);
			break;
		case LED1:
			GPIO_ResetBits(GPIOF,GPIO_Pin_10);
			break;
		default:
			;
	}

}

void led_off(uint8_t led)
{
	switch(led)
	{
		case LED0:
			GPIO_SetBits(GPIOF,GPIO_Pin_9);
			break;
		case LED1:
			GPIO_SetBits(GPIOF,GPIO_Pin_10);
			break;
		default:
			;
	}
}


void led_switch(uint8_t led)
{
	if (led_state(led) == 1) led_off(led);
	else led_on(led);
}


uint8_t led_state(uint8_t led)
{
	switch(led)
	{
		case LED0:
			return !GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_9);
		case LED1:
			return !GPIO_ReadOutputDataBit(GPIOF,GPIO_Pin_10);
		default:
			return 2;
	}
}


//��ʱһ�ν϶̵�ʱ��
void led_delay()
{
	u32 i=0;
	for(;i<1000000;i++);
}
// һЩ��˸ģʽ���ڲ��Դ���ʱ���ⲿ����Ҫ�ټ���ʱ


// ���ȿ���
void led_blink1(uint8_t LEDx)
{
	led_on(LEDx);
	led_delay();
	led_off(LEDx);
	led_delay();
}


// ��������
void led_blink2(uint8_t LEDx)
{
	led_on(LEDx);
	led_delay();
	led_delay();
	led_off(LEDx);
	led_delay();
	led_delay();
}


// ��������
void led_blink3(uint8_t LEDx)
{
	static u8 speed = 1;
	if (speed == 1) 
	{
		led_on(LEDx);
		led_delay();
		led_off(LEDx);
		led_delay();
		speed = 2;
	}
	else
	{
		led_on(LEDx);
		led_delay();
		led_delay();
		led_off(LEDx);
		led_delay();
		led_delay();
		speed = 1;
	}
	

}


// �������
void led_blink4(uint8_t LEDx)
{
	static u8 speed = 1;
	if (speed == 1) 
	{
		led_on(LEDx);
		led_delay();
		led_off(LEDx);
		led_delay();
		speed = 2;
	}
	else if (speed == 2)
	{
		led_on(LEDx);
		led_delay();
		led_off(LEDx);
		led_delay();
		speed = 3;
	}
	else if (speed == 3)
	{
		led_on(LEDx);
		led_delay();
		led_delay();
		led_off(LEDx);
		led_delay();
		led_delay();
		speed = 4;
	}
	else
	{
		led_on(LEDx);
		led_delay();
		led_delay();
		led_off(LEDx);
		led_delay();
		led_delay();
		speed = 1;
	}
}


// ����������
void led_blink5(uint8_t LEDx)
{
	static u8 speed = 1;
	if (speed == 1) 
	{
		led_on(LEDx);
		led_delay();
		led_off(LEDx);
		led_delay();
		speed = 2;
	}
	else if (speed == 2)
	{
		led_on(LEDx);
		led_delay();
		led_off(LEDx);
		led_delay();
		speed = 3;
	}
	else if (speed == 3)
	{
		led_on(LEDx);
		led_delay();
		led_off(LEDx);
		led_delay();
		speed = 4;
	}
	else if (speed == 4)
	{
		led_on(LEDx);
		led_delay();
		led_delay();
		led_off(LEDx);
		led_delay();
		led_delay();
		speed = 5;
	}
	else if (speed == 5)
	{
		led_on(LEDx);
		led_delay();
		led_delay();
		led_off(LEDx);
		led_delay();
		led_delay();
		speed = 6;
	}
	else
	{
		led_on(LEDx);
		led_delay();
		led_delay();
		led_off(LEDx);
		led_delay();
		led_delay();
		speed = 1;
	}
}
