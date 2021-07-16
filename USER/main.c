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
void test_key(void); //uart1连接电脑，通过XCOM查看验证
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
				printf("方向修改成功\t");
			}
			if (temp_cycle >= 7000)
			{
				cycle = temp_cycle;
				printf("速度修改成功");
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();		//延时初始化 
	
	uart1_init(115200);	//串口初始化波特率为115200
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


// 接线：uart2的TX与RX相连
// 目标现象：与test_uart1相同
void test_uart2(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();		//延时初始化 
	
	uart2_init(115200);	//串口初始化波特率为115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* 若串口2的接收口收到数据（来自串口2的发送口），则在串口2缓冲区读取，用串口1发送到电脑 */
		if((len2 = uart2_buf_status()) != 0)
		{					   
			u8 str[20];
			uart2_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* 若串口1收到数据（来自电脑），则在串口1缓冲区读取，用串口2发送口发送到串口2接收口 */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart2_send_bytes(str, len1 + 2);
		}
		
		
		/* 串口2的发送口不停地向串口2的接收口发送数据，经过串口1的发送口转发到电脑 */
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

// 接线：uart3的TX与RX相连
// 目标现象：与test_uart1相同
void test_uart3(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();		//延时初始化 
	
	uart3_init(115200);	//串口初始化波特率为115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* 若串口2的接收口收到数据（来自串口2的发送口），则在串口2缓冲区读取，用串口1发送到电脑 */
		if((len2 = uart3_buf_status()) != 0)
		{					   
			u8 str[20];
			uart3_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* 若串口1收到数据（来自电脑），则在串口1缓冲区读取，用串口2发送口发送到串口2接收口 */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart3_send_bytes(str, len1 + 2);
		}
		
		
		/* 串口2的发送口不停地向串口2的接收口发送数据，经过串口1的发送口转发到电脑 */
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


// 接线：uart4的TX与RX相连
// 目标现象：与test_uart1相同
void test_uart4(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();		//延时初始化 
	
	uart4_init(115200);	//串口初始化波特率为115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* 若串口2的接收口收到数据（来自串口2的发送口），则在串口2缓冲区读取，用串口1发送到电脑 */
		if((len2 = uart4_buf_status()) != 0)
		{					   
			u8 str[20];
			uart4_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* 若串口1收到数据（来自电脑），则在串口1缓冲区读取，用串口2发送口发送到串口2接收口 */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart4_send_bytes(str, len1 + 2);
		}
		
		
		/* 串口2的发送口不停地向串口2的接收口发送数据，经过串口1的发送口转发到电脑 */
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


// 接线：uart5的TX与RX相连
// 目标现象：与test_uart1相同
void test_uart5(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();		//延时初始化 
	
	uart5_init(115200);	//串口初始化波特率为115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* 若串口2的接收口收到数据（来自串口2的发送口），则在串口2缓冲区读取，用串口1发送到电脑 */
		if((len2 = uart5_buf_status()) != 0)
		{					   
			u8 str[20];
			uart5_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* 若串口1收到数据（来自电脑），则在串口1缓冲区读取，用串口2发送口发送到串口2接收口 */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart5_send_bytes(str, len1 + 2);
		}
		
		
		/* 串口2的发送口不停地向串口2的接收口发送数据，经过串口1的发送口转发到电脑 */
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


// 接线：uart6的TX与RX相连
// 目标现象：与test_uart1相同
void test_uart6(void)
{
	
	u16 times=0;  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();		//延时初始化 
	
	uart6_init(115200);	//串口初始化波特率为115200
	uart1_init(115200);
	led_init();
	
	
	while(1)
	{
		int len1 = 0;
		int len2 = 0;	
		
		/* 若串口2的接收口收到数据（来自串口2的发送口），则在串口2缓冲区读取，用串口1发送到电脑 */
		if((len2 = uart6_buf_status()) != 0)
		{					   
			u8 str[20];
			uart6_read_buf(str, len2);
			str[len2] = '\r';
			str[len2 + 1] = '\n';
			uart1_send_bytes(str, len2 + 2);
			
		}
		
		/* 若串口1收到数据（来自电脑），则在串口1缓冲区读取，用串口2发送口发送到串口2接收口 */
		if((len1 = uart1_buf_status()) != 0)
		{
			u8 str[20];
			uart1_read_buf(str, len1);
			str[len1] = '\r';
			str[len1 + 1] = '\n';
			uart6_send_bytes(str, len1 + 2);
		}
		
		
		/* 串口2的发送口不停地向串口2的接收口发送数据，经过串口1的发送口转发到电脑 */
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


