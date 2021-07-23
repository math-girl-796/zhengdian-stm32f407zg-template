#include "sys.h"
#include "flash.h"
#include "key.h"
#include "keyboard.h"
#include "test.h"
#include <string.h>
#include "utils.h"

float kp = 0, ki = 0, kd = 0;
u32 ctrl_cycle = 0;
u32 last_loop_ms = 0;

struct obj
{
	int x;
	int y;
} obj;

struct
{
	u8 show_vision;
} prog_state;

void init(void);
void loop(void);
void print_param(void);

int main(void)
{ 
//	test_uart2();
	init();
	loop();
}


void init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();
	uart1_init(115200); // 电脑调试
	uart2_init(115200); // 与k210通信
	led_init();
	key_init();
	
	flash_init();		//flash，用于存取参数
	keyboard_init();	//键盘，用于输入命令
	for (int i = 0; i < 10000; i++) keyboard_check(); // 初始化键盘的一部分
	
	time_init(); // 在TIM2上启动以ms为单位的时钟
	
	prog_state.show_vision = 1;
	
	flash_read_pid("pid", &kp, &ki, &kd); // 恢复pid参数
	flash_read_cycle("cycle", &ctrl_cycle); // 恢复pid参数
}

void loop(void)
{
	while(1)
	{
		keyboard_check(); //时常检查键盘的连接情况
		int loop_start_ms = get_time_ms();
		
		/* 收到一条shell指令 */
		if (keyboard_LF())
		{
			//获取指令内容
			u8 buf[50]; // 使用者需要注意输入的指令不要太长
			int len = keyboard_buf_state();
			keyboard_read_buf(buf, len);
			buf[len] = 0;
			printf("Your Input Command: %s\r\n", (char*)buf);
			char* command = strtok((char*)buf, " "); //分割命令串，得到命令
			
			//存储pid参数到flash
			if (strcmp(command, "svpm") == 0 || strcmp(command, "saveparam") == 0)
			{
				u8 ret = flash_write_pid("pid", kp, ki, kd);
				printf("存储pid参数结果：%s\r\n", ret?"成功":"失败");
				print_param();
			}
			//从flash加载pid参数
			else if (strcmp(command, "ldpm") == 0 || strcmp(command, "loadparam") == 0)
			{
				u8 ret = flash_read_pid("pid", &kp, &ki, &kd);
				printf("读取pid参数结果：%s\r\n", ret?"成功":"失败");
				print_param();
			}
			//查看当前参数
			else if (strcmp(command, "shpm") == 0 || strcmp(command, "showparam") == 0)
			{
				print_param();
			}
			//查看主循环用时
			else if (strcmp(command, "llt") == 0 || strcmp(command, "lastlooptime") == 0)
			{
				printf("上轮主循环执行时间：%d ms\r\n", last_loop_ms);
			}
			//存储控制定时器周期(单位：us)参数到flash
			else if (strcmp(command, "svcc") == 0 || strcmp(command, "savecycle") == 0)
			{
				u8 ret = flash_write_cycle("cycle", ctrl_cycle);
				printf("存储cycle参数结果：%s\r\n", ret?"成功":"失败");
				print_param();
			}
			//从flash加载控制定时器周期(单位：us)参数
			else if (strcmp(command, "ldcc") == 0 || strcmp(command, "loadcycle") == 0)
			{
				u8 ret = flash_read_cycle("cycle", &ctrl_cycle);
				printf("读取cycle参数结果：%s\r\n", ret?"成功":"失败");
				print_param();
			}
			else if (strcmp(command, "start") == 0 || strcmp(command, "startcontrol") == 0)
			{
				number n = parse_string(strtok(NULL, " "));
				if (n.i == 0 && ctrl_cycle != 0) 
					TIM3_Int_Init_us(ctrl_cycle);
				else if (n.i != 0 )
					TIM3_Int_Init_us(n.i);
				else 
					goto ERR_CMD;
				printf("开始控制！\r\n");
			}
			else if (strcmp(command, "stop") == 0 || strcmp(command, "stopcontrol") == 0)
			{
				TIM3_Int_Stop();
				printf("停止控制！\r\n");
			}
			else if (strcmp(command, "kp") == 0 || strcmp(command, "setkp") == 0)
			{
				kp = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "ki") == 0 || strcmp(command, "setki") == 0)
			{
				ki = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "kd") == 0 || strcmp(command, "setkd") == 0)
			{
				kd = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "cc") == 0 || strcmp(command, "setcycle") == 0)
			{
				ctrl_cycle = parse_string(strtok(NULL, " ")).i;
				TIM3_Int_Init_us(ctrl_cycle);
				print_param();
			}
			else if (strcmp(command, "shvs") == 0 || strcmp(command, "showvision") == 0)
			{
				prog_state.show_vision = 1;
				printf("开始打印视觉信息\r\n");
			}
			else if (strcmp(command, "stpshvs") == 0 || strcmp(command, "stopshowvision") == 0)
			{
				prog_state.show_vision = 0;
				printf("停止打印视觉信息\r\n");
			}
			else
			{
			ERR_CMD: 
				printf("指令错误！\r\n");
			}
			
		}
		
		

		
		/* 收到一条来自uart2的信息 */
		int uart2_len = uart2_buf_status();
		if (uart2_len == 4)
		{
			u8 buf[4];
			//读取摄像头数据
			uart2_read_buf(buf, 4);
			obj.x = (buf[1] << 8) + buf[0];
			obj.y = (buf[3] << 8) + buf[2];
			
			if(prog_state.show_vision) printf("收到视觉数据：centor_x - %d\tcentor_y - %d\r\n", obj.x, obj.y);
		}
		else if (uart2_len > 4) 
		{
			printf("视觉数据格式有误！\r\n");
			uart2_clear_buf();
		}
		
		last_loop_ms = get_time_ms() - loop_start_ms;
	}
}

void print_param(void)
{
	printf("当前参数：    kp: %f\tki: %f\tkd: %f\tcycle: %d\r\n", kp, ki, kd, ctrl_cycle);
	float kp = 0, ki = 0, kd = 0;
	u32 ctrl_cycle = 0;
	flash_read_pid("pid", &kp, &ki, &kd);
	flash_read_cycle("cycle", &ctrl_cycle);
	printf("存储器中参数：kp: %f\tki: %f\tkd: %f\tcycle: %d\r\n", kp, ki, kd, ctrl_cycle);
}

