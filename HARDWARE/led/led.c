
#include "led.h"

//原来的led.h用的位带操作，这里的led.h还会被未知的函数引用
void led_init(void)
{
	/* 使能灯泡 */
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

	//GPIOF9,F10初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO

	
	/* 操作灯泡，使其初始化后默认状态为关闭 */
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);//GPIOF9,F10设置高，灯灭
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


//延时一段较短的时间
void led_delay()
{
	u32 i=0;
	for(;i<1000000;i++);
}
// 一些闪烁模式，内部自带延时，外部不需要再加延时


// 均匀快闪
void led_blink1(uint8_t LEDx)
{
	led_on(LEDx);
	led_delay();
	led_off(LEDx);
	led_delay();
}


// 均匀慢闪
void led_blink2(uint8_t LEDx)
{
	led_on(LEDx);
	led_delay();
	led_delay();
	led_off(LEDx);
	led_delay();
	led_delay();
}


// 快慢交替
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


// 快快慢慢
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


// 快快快慢慢慢
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
