
#include "key.h"
#include "delay.h"

void key_init(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //KEY0 对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE4
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP对应引脚PA0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
 
} 


uint8_t key0_state(void)
{
	
	return KEY0; //PE4
}


uint8_t key1_state(void)
{
	return KEY1; //PA0
}
