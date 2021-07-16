#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "led.h"
#include "key.h"
#include "stepper_motor.h"

void test_uart1(void);
void test_uart2(void);
void test_uart3(void);
void test_uart4(void);
void test_uart5(void);
void test_uart6(void);
void test_led(void);
void test_key(void); //uart1���ӵ��ԣ�ͨ��XCOM�鿴��֤
void test_stepper_motor(void);

int main(void)
{	
	test_uart6();
}


void test_stepper_motor(void)
{
	u8 direction = CW;
	int cycle = 8000;
	int len = 0;
	u8 buf[5];
	
	delay_init();
	uart1_init(115200);
	stepper_motor_init();
	

	while(1)
	{
		if ((len = uart1_buf_status()) == 5)
		{
			u8 temp_direction;
			int temp_cycle;
			uart1_read_buf(buf, 5);
			temp_direction = buf[0];
			temp_cycle = buf[1] + (buf[2] << 8) + (buf[3] << 16) + (buf[4] << 24);
			printf("%d\t%d\t", temp_direction, temp_cycle);
			
			if (temp_direction == CW || temp_direction == CCW)
			{
				direction = temp_direction;
				printf("�����޸ĳɹ�\t");
			}
			if (temp_cycle >= 7000)
			{
				cycle = temp_cycle;
				printf("�ٶ��޸ĳɹ�");
			}
			printf("\r\n");
		}
		else if (len > 0)
		{
			uart1_clear_buf();
		}
		
		stepper_motor_ctrl_512(direction, 2, cycle);
	}
}


void test_key()
{
	uart1_init(115200);
	delay_init();
	key_init();

	while(1)
	{
		delay_ms(50);
		printf("key0:%d\tkey1:%d\r\n", key0_state(), key1_state());
	}
}


void test_led(void)
{
	led_init();
	delay_init();
	led_on(LED1);
	while(1)
	{
		delay_ms(500);
		led_switch(LED0);
		led_switch(LED1);
	}
}


void test_uart1(void)
{
	u8 len;	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();		//��ʱ��ʼ�� 
	
	uart1_init(115200);	//���ڳ�ʼ��������Ϊ115200
	led_init();
	
	while(1)
	{
		if((len = uart1_buf_status()) != 0)
		{					   
			u8 str[20];
			
			uart1_read_buf(str, len);
			
			uart1_send_bytes(str, len);
			
			uart1_clear_buf();
			
		}else
		{
			times++;
			if(times%200==0)
			{
				static u8 a = 0;
				
				char str[10];
				
				sprintf(str, "%c%c\r\n", a+65, a+66);
				
				uart1_send_bytes((u8 *)str, 4);
				
				a ++;
				a = a % 26;
				
				led_switch(LED0);
			}
			delay_ms(10);   
		}
	}
}


// ���ߣ�uart2��TX��RX����
// Ŀ��������test_uart1��ͬ
void test_uart2(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();		//��ʱ��ʼ�� 
	
	uart2_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* ������2�Ľ��տ��յ����ݣ����Դ���2�ķ��Ϳڣ������ڴ���2��������ȡ���ô���1���͵����� */
		if((len2 = uart2_buf_status()) != 0)
		{					   
			u8 str[20];
			uart2_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* ������1�յ����ݣ����Ե��ԣ������ڴ���1��������ȡ���ô���2���Ϳڷ��͵�����2���տ� */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart2_send_bytes(str, len1 + 2);
		}
		
		
		/* ����2�ķ��Ϳڲ�ͣ���򴮿�2�Ľ��տڷ������ݣ���������1�ķ��Ϳ�ת�������� */
		times++;
		if(times%200==0)
		{
			static u8 a = 0;
			
			char str[10];
			
			sprintf(str, "%c%c\r\n", a+65, a+66);
			
			uart2_send_bytes((u8 *)str, 4);
			
			a ++;
			a = a % 26;
			
			led_switch(LED1);
		}
		delay_ms(10);   
	}
}

// ���ߣ�uart3��TX��RX����
// Ŀ��������test_uart1��ͬ
void test_uart3(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();		//��ʱ��ʼ�� 
	
	uart3_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* ������2�Ľ��տ��յ����ݣ����Դ���2�ķ��Ϳڣ������ڴ���2��������ȡ���ô���1���͵����� */
		if((len2 = uart3_buf_status()) != 0)
		{					   
			u8 str[20];
			uart3_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* ������1�յ����ݣ����Ե��ԣ������ڴ���1��������ȡ���ô���2���Ϳڷ��͵�����2���տ� */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart3_send_bytes(str, len1 + 2);
		}
		
		
		/* ����2�ķ��Ϳڲ�ͣ���򴮿�2�Ľ��տڷ������ݣ���������1�ķ��Ϳ�ת�������� */
		times++;
		if(times%200==0)
		{
			static u8 a = 0;
			
			char str[10];
			
			sprintf(str, "%c%c\r\n", a+65, a+66);
			
			uart3_send_bytes((u8 *)str, 4);
			
			a ++;
			a = a % 26;
			
			led_switch(LED1);
		}
		delay_ms(10);   
	}
}


// ���ߣ�uart4��TX��RX����
// Ŀ��������test_uart1��ͬ
void test_uart4(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();		//��ʱ��ʼ�� 
	
	uart4_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* ������2�Ľ��տ��յ����ݣ����Դ���2�ķ��Ϳڣ������ڴ���2��������ȡ���ô���1���͵����� */
		if((len2 = uart4_buf_status()) != 0)
		{					   
			u8 str[20];
			uart4_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* ������1�յ����ݣ����Ե��ԣ������ڴ���1��������ȡ���ô���2���Ϳڷ��͵�����2���տ� */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart4_send_bytes(str, len1 + 2);
		}
		
		
		/* ����2�ķ��Ϳڲ�ͣ���򴮿�2�Ľ��տڷ������ݣ���������1�ķ��Ϳ�ת�������� */
		times++;
		if(times%200==0)
		{
			static u8 a = 0;
			
			char str[10];
			
			sprintf(str, "%c%c\r\n", a+65, a+66);
			
			uart4_send_bytes((u8 *)str, 4);
			
			a ++;
			a = a % 26;
			
			led_switch(LED1);
		}
		delay_ms(10);   
	}
}


// ���ߣ�uart5��TX��RX����
// Ŀ��������test_uart1��ͬ
void test_uart5(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();		//��ʱ��ʼ�� 
	
	uart5_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* ������2�Ľ��տ��յ����ݣ����Դ���2�ķ��Ϳڣ������ڴ���2��������ȡ���ô���1���͵����� */
		if((len2 = uart5_buf_status()) != 0)
		{					   
			u8 str[20];
			uart5_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* ������1�յ����ݣ����Ե��ԣ������ڴ���1��������ȡ���ô���2���Ϳڷ��͵�����2���տ� */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart5_send_bytes(str, len1 + 2);
		}
		
		
		/* ����2�ķ��Ϳڲ�ͣ���򴮿�2�Ľ��տڷ������ݣ���������1�ķ��Ϳ�ת�������� */
		times++;
		if(times%200==0)
		{
			static u8 a = 0;
			
			char str[10];
			
			sprintf(str, "%c%c\r\n", a+65, a+66);
			
			uart5_send_bytes((u8 *)str, 4);
			
			a ++;
			a = a % 26;
			
			led_switch(LED1);
		}
		delay_ms(10);   
	}
}


// ���ߣ�uart6��TX��RX����
// Ŀ��������test_uart1��ͬ
void test_uart6(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();		//��ʱ��ʼ�� 
	
	uart6_init(115200);	//���ڳ�ʼ��������Ϊ115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* ������2�Ľ��տ��յ����ݣ����Դ���2�ķ��Ϳڣ������ڴ���2��������ȡ���ô���1���͵����� */
		if((len2 = uart6_buf_status()) != 0)
		{					   
			u8 str[20];
			uart6_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* ������1�յ����ݣ����Ե��ԣ������ڴ���1��������ȡ���ô���2���Ϳڷ��͵�����2���տ� */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart6_send_bytes(str, len1 + 2);
		}
		
		
		/* ����2�ķ��Ϳڲ�ͣ���򴮿�2�Ľ��տڷ������ݣ���������1�ķ��Ϳ�ת�������� */
		times++;
		if(times%200==0)
		{
			static u8 a = 0;
			
			char str[10];
			
			sprintf(str, "%c%c\r\n", a+65, a+66);
			
			uart6_send_bytes((u8 *)str, 4);
			
			a ++;
			a = a % 26;
			
			led_switch(LED1);
		}
		delay_ms(10);   
	}
}

