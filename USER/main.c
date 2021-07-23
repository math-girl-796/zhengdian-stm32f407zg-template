#include "main.h"

pid pid1;
pid pid2;
u32 ctrl_cycle;    // 控制中断周期（单位us）
u32 last_ctrl_ms;  // 上次中断处理函数运行时间
u32 last_loop_ms;  // 上次主循环用时（单位ms）
u16 camera_width, camera_height; // 相机处理图像像素范围
u32 UART2_TIMEOUT;
coord err;
coord ierr;
coord derr;
coord lerr; // last_error
coord steer_compare;

coord obj, tgt;
struct prog_state prog_state;

int main(void)
{ 
//	test_steers();
//	test_time();
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
	steer1_init();
	steer2_init();
	
	// 初始化一些数值
	prog_state.show_vision = 0;
	last_loop_ms = 0;
	camera_width = 240;
	camera_height = 240;
	tgt.x = camera_width / 2;
	tgt.y = camera_height / 2;
	flash_read_pid("pid1", &pid1.kp, &pid1.ki, &pid1.kd); // 恢复pid参数
	flash_read_pid("pid2", &pid2.kp, &pid2.ki, &pid2.kd); // 恢复pid参数
	flash_read_cycle("cycle", &ctrl_cycle); // 恢复参数
	steer1_set_compare(1000);
	steer2_set_compare(1000);
	UART2_TIMEOUT = 500;
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
				u8 ret = flash_write_pid("pid1", pid1.kp, pid1.ki, pid1.kd);
				printf("存储pid1参数结果：%s\r\n", ret?"成功":"失败");
				ret = flash_write_pid("pid2", pid2.kp, pid2.ki, pid2.kd);
				printf("存储pid2参数结果：%s\r\n", ret?"成功":"失败");
				print_param();
			}
			//从flash加载pid参数
			else if (strcmp(command, "ldpm") == 0 || strcmp(command, "loadparam") == 0)
			{
				u8 ret = flash_read_pid("pid1", &pid1.kp, &pid1.ki, &pid1.kd); 
				printf("读取pid1参数结果：%s\r\n", ret?"成功":"失败");
				ret = flash_read_pid("pid2", &pid2.kp, &pid2.ki, &pid2.kd); 
				printf("读取pid2参数结果：%s\r\n", ret?"成功":"失败");
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
				printf("上轮主循环执行时间：%d ms\r\n上轮控制代码执行时间：%d ms\r\n", last_loop_ms, last_ctrl_ms);
				printf("收到视觉数据：centor_x - %d\tcentor_y - %d\r\n", obj.x, obj.y);
//				int len = uart2_buf_status();
//				char buf[10];
//				uart2_read_buf((u8*)buf, len);
//				printf("uart2缓冲区字符个数：%d\t内容：%s\r\n", uart2_buf_status(), buf);
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
			else if (strcmp(command, "kp1") == 0 || strcmp(command, "setkp1") == 0)
			{
				pid1.kp = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "ki1") == 0 || strcmp(command, "setki1") == 0)
			{
				pid1.ki = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "kd1") == 0 || strcmp(command, "setkd1") == 0)
			{
				pid1.kd = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "kp2") == 0 || strcmp(command, "setkp2") == 0)
			{
				pid2.kp = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "ki2") == 0 || strcmp(command, "setki2") == 0)
			{
				pid2.ki = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "kd2") == 0 || strcmp(command, "setkd2") == 0)
			{
				pid2.kd = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "cc") == 0 || strcmp(command, "setcycle") == 0)
			{
				ctrl_cycle = parse_string(strtok(NULL, " ")).i;
				TIM3_Int_Init_us(ctrl_cycle);
				print_param();
			}
			else if (strcmp(command, "tgt") == 0 || strcmp(command, "settarget") == 0)
			{
				tgt.x = parse_string(strtok(NULL, " ")).i;
				tgt.y = parse_string(strtok(NULL, " ")).i;
				print_param();
			}
			else if (strcmp(command, "u2t") == 0 || strcmp(command, "setuart2timeout") == 0)
			{
				UART2_TIMEOUT = parse_string(strtok(NULL, " ")).i;
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
			//向k210发送信息
			else if (strcmp(command, "k210") == 0)
			{
				char* k210_command = strtok(NULL, " ");
				uart2_send_bytes((u8*)k210_command, strlen(k210_command));
				printf("你发送的消息为：%s\r\n", k210_command);
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
	printf("当前参数：    kp1: %f\tki1: %f\tkd1: %f\tcycle: %d\t\r\n\
              kp2: %f\tki2: %f\tkd2: %f\t目标点(%d, %d)\r\n", pid1.kp, pid1.ki, pid1.kd, ctrl_cycle, pid2.kp, pid2.ki, pid2.kd, tgt.x, tgt.y);
	pid pid1, pid2;
	u32 ctrl_cycle = 0;
	flash_read_pid("pid1", &pid1.kp, &pid1.ki, &pid1.kd); 
	flash_read_pid("pid2", &pid2.kp, &pid2.ki, &pid2.kd); 
	flash_read_cycle("cycle", &ctrl_cycle);
	printf("存储器中参数：kp1: %f\tki1: %f\tkd1: %f\tcycle: %d\r\n\
              kp2: %f\tki2: %f\tkd2: %f\r\n",  pid1.kp, pid1.ki, pid1.kd, ctrl_cycle, pid2.kp, pid2.ki, pid2.kd);
}

