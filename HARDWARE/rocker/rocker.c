#include "rocker.h"

// ��ʼ��PA
void rocker_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	// ��ʼ���ܽţ�����ADC1ͨ��6
	ROCKER_X_GPIO_RCC_FUN(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROCKER_X_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	GPIO_Init(ROCKER_X_GPIO, &GPIO_InitStructure);	
	
	// ��ʼ���ܽţ�����ADC1ͨ��7
	ROCKER_Y_GPIO_RCC_FUN(ROCKER_Y_GPIO_RCC_ADDR, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROCKER_Y_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	GPIO_Init(ROCKER_Y_GPIO, &GPIO_InitStructure);	
	
	// ��ʼ���ܽţ�����ҡ�˰���
	ROCKER_SW_GPIO_RCC_FUN(ROCKER_SW_GPIO_RCC_ADDR, ENABLE);
	GPIO_InitStructure.GPIO_Pin = ROCKER_SW_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(ROCKER_SW_GPIO, &GPIO_InitStructure);	

	//  ʹ��ADC1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��

	ADC_Cmd(ADC1, ENABLE);//����ADת����	
}

// chx����ͨ��x
u16 get_adc(u8 ch)   
{
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}


// ��ȡҡ��x�������꣬�����Ϊ-2048�����ұ�Ϊ2047���м���0
int16_t rocker_x(void)
{
	uint32_t temp_val=0;
	u8 t;
	for(t=0;t<ADC_MEAN_TIME;t++)
	{
		temp_val+=get_adc(ROCKER_X_ADC_CHANNEL); // ��ȡģ���ź�
		delay_ms(2);
	}
	return temp_val/ADC_MEAN_TIME - 2048;
}


// ��ȡҡ��y�������꣬���ϱ�Ϊ-2048�����±�Ϊ2047���м���0
int16_t rocker_y(void)
{
	uint32_t temp_val=0;
	u8 t;
	for(t=0;t<ADC_MEAN_TIME;t++)
	{
		temp_val+=get_adc(ROCKER_Y_ADC_CHANNEL); // ��ȡģ���ź�
		delay_ms(2);
	}
	return temp_val/ADC_MEAN_TIME - 2048;
}


// ��ȡҡ�˰���״̬������Ϊ1���ɿ�Ϊ0
u8 rocker_sw(void)
{
	return !GPIO_ReadInputDataBit(ROCKER_SW_GPIO, ROCKER_SW_GPIO_PIN);
}
