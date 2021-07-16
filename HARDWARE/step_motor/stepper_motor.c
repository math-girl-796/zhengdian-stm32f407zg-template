// ���Žӷ�
// IN4��PB6  d     
// IN3: PB7  c
// IN2: PB8  b
// IN1: PB9  a

#include "stepper_motor.h"

void Stepper_motorStop(void);//ֹͣ

//direction ��ѡ���ΪCW/CCW���ֱ��ʾ˳ʱ�����ʱ��
void OneStep4(u8 direction);
void OneStep8(u8 direction);

//TΪתһȦ��ʱ��   ע�⣺T��СΪ6000��6000ʱ���п��ܳ��ֿ������
void OneRound4(u8 direction, int T);

//TΪתһȦ��ʱ��
void OneRound8(u8 direction, int T);

//���������е�һ��
void LA_on(void);
void LB_on(void);
void LC_on(void);
void LD_on(void);
void LAB_on(void);
void LBC_on(void);
void LCD_on(void);
void LAD_on(void);

void LA_on(void)
{
	GPIO_SetBits(LA_GPIO_PORT, LA_GPIO_PIN);
	GPIO_ResetBits(LB_GPIO_PORT, LB_GPIO_PIN);
	GPIO_ResetBits(LC_GPIO_PORT, LC_GPIO_PIN);
	GPIO_ResetBits(LD_GPIO_PORT, LD_GPIO_PIN);
}

void LB_on(void)
{
	GPIO_ResetBits(LA_GPIO_PORT, LA_GPIO_PIN);
	GPIO_SetBits(LB_GPIO_PORT, LB_GPIO_PIN);
	GPIO_ResetBits(LC_GPIO_PORT, LC_GPIO_PIN);
	GPIO_ResetBits(LD_GPIO_PORT, LD_GPIO_PIN);
}

void LC_on(void)
{
	GPIO_ResetBits(LA_GPIO_PORT, LA_GPIO_PIN);
	GPIO_ResetBits(LB_GPIO_PORT, LB_GPIO_PIN);
	GPIO_SetBits(LC_GPIO_PORT, LC_GPIO_PIN);
	GPIO_ResetBits(LD_GPIO_PORT, LD_GPIO_PIN);
}

void LD_on(void)
{
	GPIO_ResetBits(LA_GPIO_PORT, LA_GPIO_PIN);
	GPIO_ResetBits(LB_GPIO_PORT, LB_GPIO_PIN);
	GPIO_ResetBits(LC_GPIO_PORT, LC_GPIO_PIN);
	GPIO_SetBits(LD_GPIO_PORT, LD_GPIO_PIN);
}

void LAB_on(void)
{
	GPIO_SetBits(LA_GPIO_PORT, LA_GPIO_PIN);
	GPIO_SetBits(LB_GPIO_PORT, LB_GPIO_PIN);
	GPIO_ResetBits(LC_GPIO_PORT, LC_GPIO_PIN);
	GPIO_ResetBits(LD_GPIO_PORT, LD_GPIO_PIN);
}

void LBC_on(void)
{
	GPIO_ResetBits(LA_GPIO_PORT, LA_GPIO_PIN);
	GPIO_SetBits(LB_GPIO_PORT, LB_GPIO_PIN);
	GPIO_SetBits(LC_GPIO_PORT, LC_GPIO_PIN);
	GPIO_ResetBits(LD_GPIO_PORT, LD_GPIO_PIN);
}

void LCD_on(void)
{
	GPIO_ResetBits(LA_GPIO_PORT, LA_GPIO_PIN);
	GPIO_ResetBits(LB_GPIO_PORT, LB_GPIO_PIN);
	GPIO_SetBits(LC_GPIO_PORT, LC_GPIO_PIN);
	GPIO_SetBits(LD_GPIO_PORT, LD_GPIO_PIN);
}

void LAD_on(void)
{
	GPIO_SetBits(LA_GPIO_PORT, LA_GPIO_PIN);
	GPIO_ResetBits(LB_GPIO_PORT, LB_GPIO_PIN);
	GPIO_ResetBits(LC_GPIO_PORT, LC_GPIO_PIN);
	GPIO_SetBits(LD_GPIO_PORT, LD_GPIO_PIN);
}


void stepper_motor_init(void)  //���ų�ʼ��PB6 7 8 9
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LA_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LB_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LC_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LD_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LA_GPIO_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(LA_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LB_GPIO_PIN ;
	GPIO_Init(LB_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LC_GPIO_PIN ;
	GPIO_Init(LC_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LD_GPIO_PIN ;
	GPIO_Init(LD_GPIO_PORT, &GPIO_InitStructure);
	
	Stepper_motorStop();
}


void OneStep4(u8 direction)  
{
	static uint8_t step = 0;
	if (direction == CW)
	{
		switch(step)
		{

		case 0:
			LA_on();
			break;
		case 1:
			LB_on();
			break;
		case 2:
			LC_on();
			break;
		case 3:
			LD_on();
			break;
		}
	}

	else if (direction == CCW)
	{
		switch(step)
		{
		case 0:
			LD_on();
			break;
		case 1:
			LC_on();
			break;
		case 2:
			LB_on();
			break;
		case 3:
			LA_on();
			break;
	}
}
	step = (step + 1) % 4;
}
	
void OneStep8(u8 direction)
{
	static uint8_t step = 0;
	if (direction == CW)
	{

		switch(step)
		{
		case 0:
			LA_on();
			break;
		case 1:
			LAB_on();
			break;
		case 2:
			LB_on();
			break;
		case 3:
			LBC_on();
			break;
		case 4:
			LC_on();
			break;
		case 5:
			LCD_on();
			break;
		case 6:
			LD_on();
			break;
		case 7:
			LAD_on();
			break;
		}
	}

	else if (direction == CCW)
	{
		switch(step)
		{
		case 0:
			LAD_on();
			break;
		case 1:
			LD_on();
			break;
		case 2:
			LCD_on();
			break;
		case 3:
			LC_on();
			break;
		case 4:
			LBC_on();
			break;
		case 5:
			LB_on();
			break;
		case 6:
			LAB_on();
			break;
		case 7:
			LA_on();
			break;
		}
	}
	step = (step + 1) % 8;
}
 
void Stepper_motorStop(void)
{
	GPIO_ResetBits(LA_GPIO_PORT, LA_GPIO_PIN);
	GPIO_ResetBits(LB_GPIO_PORT, LB_GPIO_PIN);
	GPIO_ResetBits(LC_GPIO_PORT, LC_GPIO_PIN);
	GPIO_ResetBits(LD_GPIO_PORT, LD_GPIO_PIN);
}


void OneRound4(u8 direction, int T)  //0Ϊ��ʱ�룬1Ϊ˳ʱ�룻TתһȦ����ʱ�䣨ms����ѭ��512��תһȦ //��һ�֣�ת��ת45��
{
	int i;
	u32 t;
	t=T*1000/(64*8);//t������һ���õ�ʱ��
	
	for(i = 0; i < 4; i++)
	{
		OneStep8(direction);
		delay_us(t/4);
	}

/*	if(direction == CW)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/4);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/4);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/4);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/4);
	}

	else if(direction == CCW)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/4);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/4);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/4);

		GPIO_SetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/4);
	}
*/
}


void OneRound8(u8 direction, int T)  //0Ϊ��1Ϊ��TתһȦ����ʱ�䣨ms����ѭ��512��תһȦ //��һ�֣�ת��ת45��
{
	int i;
	u32 t;
	t=T*1000/(64*8);//t������һ���õ�ʱ��

	for(i = 0; i < 8; i++)
	{
		OneStep8(direction);
		delay_us(t/8);
	}
	
/*	if(direction == CW)
	{
		LA_on();
		delay_us(t/8);

		LAB_on();
		delay_us(t/8);

		LB_on();
		delay_us(t/8);

		LBC_on();
		delay_us(t/8);

		LC_on();
		delay_us(t/8);

		LCD_on();
		delay_us(t/8);

		LD_on();
		delay_us(t/8);

		LAD_on();
		delay_us(t/8);
		}

		else if(direction == CCW)
		{
		GPIO_SetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/8);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/8);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/8);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/8);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/8);

		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/8);

		GPIO_SetBits(GPIOB, GPIO_Pin_6);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/8);

		GPIO_SetBits(GPIOB, GPIO_Pin_6);
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		delay_us(t/8);
	}
*/
}


void stepper_motor_ctrl(u8 direction, int angle, int T)
{
	int i;
	int round = angle / 360;
	int remain = angle % 360;
	
	int k = remain * 512 / 360;
	
	for(i=0;i<round;i++) // ����Ȧ��ת��
	{
		int j;
		for(j=0;j<512;j++)
		{
			OneRound8(direction,T);
		}
	}
	
	for(i=0;i<k;i++)     // תʣ�µĲ���һȦ��
	{
		 OneRound8(direction,T);
	}
}

void stepper_motor_ctrl_512(u8 direction, int factor, int T)
{
	int i;
	int round = factor / 512;
	int remain = factor % 512;
	
	
	for(i=0;i<round;i++) // ����Ȧ��ת��
	{
		int j;
		for(j=0;j<512;j++)
		{
			OneRound8(direction,T);
		}
	}
	
	for(i=0;i<remain;i++)     // תʣ�µĲ���һȦ��
	{
		 OneRound8(direction,T);
	}

}
