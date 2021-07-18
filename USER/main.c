#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "string.h"
#include "led.h"
#include "key.h"
#include "stepper_motor.h"
#include "pwm.h"
#include "steer.h"
#include "utils.h"

void test_uart1(void);
void test_uart2(void);
void test_uart3(void);
void test_uart4(void);
void test_uart5(void);
void test_uart6(void);
void test_led(void);
void test_key(void); //uart1连接电脑，通过XCOM查看验证
void test_stepper_motor(void);
void test_pwm(void);
void test_pwm_and_uart1(void);
void test_steer1_and_uart1(void); // 舵机
void test_pid_camera_and_steer1(void);
void test_parse_bytes_and_uart1(void);

int main(void)
{	
	test_parse_bytes_and_uart1();
}


// 测试用例
//1		len: 1	num.i: 1	num.f: 1.000000	
//1.1	len: 3	num.i: 1	num.f: 1.100000	
//1.5	len: 3	num.i: 2	num.f: 1.500000	
//-1	len: 2	num.i: -1	num.f: -1.000000	
//-1.1	len: 4	num.i: -1	num.f: -1.100000	
//-1.5	len: 4	num.i: -2	num.f: -1.500000	
//999999999999999	len: 15	num.i: 2147483647	num.f: 999999986991104.000000	
//99999999999999999999999999999999999999999999999999	len: 50	num.i: 2147483647	num.f: inf	
//-999999999999999	len: 16	num.i: -2147483648	num.f: -999999986991104.000000	
//999999999999999999999999999999999999999999999999999999	len: 55	num.i: -2147483648	num.f: -inf	
void test_parse_bytes_and_uart1(void)
{
	uart1_init(115200);
	
	while(1)
	{
		int len = uart1_buf_status();
		number num;
		if (len != 0)
		{
			printf("len: %d\t", len);
			u8 buf[200];
			uart1_read_buf(buf, len);
			buf[len] = 0; // 这条语句非常关键；这条语句也可以改成《清空buf》
			
			num = parse_string((char*) buf);
			printf("num.i: %d\tnum.f: %f\t\r\n",num.i, num.f);
		}
	}
	
}


// steer1连接9g舵机，k210pin15连接本机uart2发送口，k210所烧文件见TEST文件夹
void test_pid_camera_and_steer1(void)
{
	int target_x = 160;  
	int extern_x; // 0 - 320
	int pwmv = 1500; // 500 - 2500
	
	int error, derror, ierror;
	int error_last = 0;
	float kp = 0.1, kd = -1, ki = 0;
	
	int dia;
	
	u8 buf[2];
	led_init();
	delay_init();
	uart1_init(115200);
	uart2_init(115200);
	
	steer1_init();
	steer1_set_compare(1500);
	
	while(1)
	{
		int len1 = uart1_buf_status();
		int len2 = uart2_buf_status();
		if (len2 == 2)
		{
			//读取摄像头数据
			uart2_read_buf(buf, 2);
			extern_x = (buf[1] << 8) + buf[0];
			printf("extern_x: %d\t", extern_x);
			
			//更新误差值
			error = target_x - extern_x;
			derror = error - error_last;
			ierror = ierror + error;
			error_last = error;
			printf("error: %d\t", error);
			
			//修剪ierror防止它太大
			if (ierror > 3000) ierror = 3000;
			if (ierror < -3000) ierror = -3000;
			
			//计算提供给pwm的Compare的更新差值
			dia = (int) (kp * error + kd * derror + ki * ierror);
			printf("dia: %d\t", dia);
			
			//更新pwmv值
			pwmv += dia;
			
			//修剪pwmv值防止它超出范围
			if (pwmv > 2500) pwmv = 2500;
			if (pwmv < 500) pwmv = 500;
			printf("new_pwmv:%d\t", pwmv);
			
			//设置舵机新位置
			steer1_set_compare(pwmv);
			printf("\r\n");
		}
		if (len1 == 3)
		{
			//读取摄像头数据
			uart2_read_buf(buf, 2);
			extern_x = (buf[1] << 8) + buf[0];
			printf("extern_x: %d\t", extern_x);
			
			//更新误差值
			error = target_x - extern_x;
			derror = error - error_last;
			ierror = ierror + error;
			error_last = error;
			printf("error: %d\t", error);
			
			//修剪ierror防止它太大
			if (ierror > 3000) ierror = 3000;
			if (ierror < -3000) ierror = -3000;
			
			//计算提供给pwm的Compare的更新差值
			dia = (int) (kp * error + kd * derror + ki * ierror);
			printf("dia: %d\t", dia);
			
			//更新pwmv值
			pwmv += dia;
			
			//修剪pwmv值防止它超出范围
			if (pwmv > 2500) pwmv = 2500;
			if (pwmv < 500) pwmv = 500;
			printf("new_pwmv:%d\t", pwmv);
			
			//设置舵机新位置
			steer1_set_compare(pwmv);
			printf("\r\n");
		}
		else
		{
//			printf("format error\r\n");
			uart2_clear_buf();
		}
		delay_ms(10);
	}
}

void test_steer1_and_uart1(void) //使用前先配置TIM14_CH1_PWM管脚为PA7.电脑会不停地收到hello。电脑给f4发0-20000的int型数据（两个字节），会改变pwm占空比，其他范围数据可能会导致出错
	//有效范围为500-2500，表示0°-180°
{
	int light_level = 1500;
	int temp_light_level;
	u8 buf[4];
	u8 hello_count = 0;
	led_init();
	delay_init();
	uart1_init(115200);
	
	steer1_init();
	
	while(1)
	{
		int len = uart1_buf_status();
		if (len == 4)
		{
			uart1_read_buf(buf, 4);
			temp_light_level = (buf[3] << 24) + (buf[2] << 16) + (buf[1] << 8) + buf[0];
			light_level = temp_light_level;
			printf("------------> SET HIGH VOLTAGE us in a cycle: %d\r\n", light_level);
			steer1_set_compare(light_level);
		}
		else
		{
			uart1_clear_buf();
		}
		delay_ms(100);
		hello_count++;
		if (hello_count == 10)
		{
			printf("hello\r\n");
			hello_count = 0;
		}
	}
}

void test_pwm_and_uart1(void) //使用前先配置TIM14_CH1_PWM管脚为PF9.电脑会不停地收到hello。电脑给f4发0-500的short型数据（两个字节），会改变led亮度，其他范围数据可能会导致出错
{
	int light_level = 150;
	int temp_light_level;
	u8 buf[2];
	led_init();
	delay_init();
	uart1_init(115200);
	TIM14_CH1_PWM_Init_us(500);
	
	while(1)
	{
		int len = uart1_buf_status();
		if (len == 4)
		{
			uart1_read_buf(buf, 2);
			temp_light_level = (buf[1] << 8) + buf[0];
			light_level = temp_light_level;
			printf("------------> SET HIGH VOLTAGE us in a cycle: %d\r\n", light_level);
		}
		TIM14_CH1_PWM_Set_Compare(light_level);
		delay_ms(100);
		printf("hello\r\n");
	}
}


void test_pwm(void) // 使用前先配置TIM14_CH1_PWM管脚为PF9，看led灯
{
	int light_level;
	led_init();
	delay_init();
	uart1_init(115200);
	TIM14_CH1_PWM_Init_us(500);
	while(1)
	{
		for(light_level = 300; light_level < 500; light_level++)
		{
			TIM14_CH1_PWM_Set_Compare(light_level);
			delay_ms(5);
		}
		for(light_level = 500; light_level > 300; light_level--)
		{
			TIM14_CH1_PWM_Set_Compare(light_level);
			delay_ms(5);
		}
	}
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


