
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
