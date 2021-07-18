#include "rocker.h"

// 初始化PA
void rocker_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	// 初始化管脚，用于ADC1通道6
	ROCKER_X_GPIO_RCC_FUN(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROCKER_X_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
	GPIO_Init(ROCKER_X_GPIO, &GPIO_InitStructure);	
	
	// 初始化管脚，用于ADC1通道7
	ROCKER_Y_GPIO_RCC_FUN(ROCKER_Y_GPIO_RCC_ADDR, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROCKER_Y_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
	GPIO_Init(ROCKER_Y_GPIO, &GPIO_InitStructure);	
	
	// 初始化管脚，用于摇杆按键
	ROCKER_SW_GPIO_RCC_FUN(ROCKER_SW_GPIO_RCC_ADDR, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROCKER_SW_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(ROCKER_SW_GPIO, &GPIO_InitStructure);	

	//  使能ADC1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//初始化

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化

	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
}

// chx代表通道x
u16 get_adc(u8 ch)   
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}


// 获取摇杆x方向坐标，最左边为-2048，最右边为2047，中间是0
int16_t rocker_x(void)
{
	uint32_t temp_val=0;
	u8 t;
	for(t=0;t<ADC_MEAN_TIME;t++)
	{
		temp_val+=get_adc(ROCKER_X_ADC_CHANNEL); // 获取模拟信号
		delay_ms(2);
	}
	return temp_val/ADC_MEAN_TIME - 2048;
}


// 获取摇杆y方向坐标，最上边为-2048，最下边为2047，中间是0
int16_t rocker_y(void)
{
	uint32_t temp_val=0;
	u8 t;
	for(t=0;t<ADC_MEAN_TIME;t++)
	{
		temp_val+=get_adc(ROCKER_Y_ADC_CHANNEL); // 获取模拟信号
		delay_ms(2);
	}
	return temp_val/ADC_MEAN_TIME - 2048;
}


// 获取摇杆按键状态，按下为1，松开为0
u8 rocker_sw(void)
{
	return !GPIO_ReadInputDataBit(ROCKER_SW_GPIO, ROCKER_SW_GPIO_PIN);
}
