// 引脚接法
// IN4：PB6  d     
// IN3: PB7  c
// IN2: PB8  b
// IN1: PB9  a

#include "stepper_motor.h"

void Stepper_motorStop(void);//停止

//direction 可选项均为CW/CCW，分别表示顺时针和逆时针
void OneStep4(u8 direction);
void OneStep8(u8 direction);

//T为转一圈的时间   注意：T最小为6000，6000时还有可能出现卡顿情况
void OneRound4(u8 direction, int T);

//T为转一圈的时间
void OneRound8(u8 direction, int T);

//开启四项中的一项
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


void stepper_motor_init(void)  //引脚初始化PB6 7 8 9
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LA_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LB_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LC_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(LD_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LA_GPIO_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
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


void OneRound4(u8 direction, int T)  //0为逆时针，1为顺时针；T转一圈所用时间（ms），循环512次转一圈 //走一轮，转子转45°
{
	int i;
	u32 t;
	t=T*1000/(64*8);//t代表走一轮用的时间
	
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


void OneRound8(u8 direction, int T)  //0为，1为；T转一圈所用时间（ms），循环512次转一圈 //走一轮，转子转45°
{
	int i;
	u32 t;
	t=T*1000/(64*8);//t代表走一轮用的时间

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
	
	for(i=0;i<round;i++) // 把整圈的转完
	{
		int j;
		for(j=0;j<512;j++)
		{
			OneRound8(direction,T);
		}
	}
	
	for(i=0;i<k;i++)     // 转剩下的不足一圈的
	{
		 OneRound8(direction,T);
	}
}

void stepper_motor_ctrl_512(u8 direction, int factor, int T)
{
	int i;
	int round = factor / 512;
	int remain = factor % 512;
	
	
	for(i=0;i<round;i++) // 把整圈的转完
	{
		int j;
		for(j=0;j<512;j++)
		{
			OneRound8(direction,T);
		}
	}
	
	for(i=0;i<remain;i++)     // 转剩下的不足一圈的
	{
		 OneRound8(direction,T);
	}

}
