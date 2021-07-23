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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();
	uart1_init(115200); // ���Ե���
	uart2_init(115200); // ��k210ͨ��
	led_init();
	key_init();
	
	flash_init();		//flash�����ڴ�ȡ����
	keyboard_init();	//���̣�������������
	for (int i = 0; i < 10000; i++) keyboard_check(); // ��ʼ�����̵�һ����
	
	time_init(); // ��TIM2��������msΪ��λ��ʱ��
	
	prog_state.show_vision = 1;
	
	flash_read_pid("pid", &kp, &ki, &kd); // �ָ�pid����
	flash_read_cycle("cycle", &ctrl_cycle); // �ָ�pid����
}

void loop(void)
{
	while(1)
	{
		keyboard_check(); //ʱ�������̵��������
		int loop_start_ms = get_time_ms();
		
		/* �յ�һ��shellָ�� */
		if (keyboard_LF())
		{
			//��ȡָ������
			u8 buf[50]; // ʹ������Ҫע�������ָ�Ҫ̫��
			int len = keyboard_buf_state();
			keyboard_read_buf(buf, len);
			buf[len] = 0;
			printf("Your Input Command: %s\r\n", (char*)buf);
			char* command = strtok((char*)buf, " "); //�ָ�������õ�����
			
			//�洢pid������flash
			if (strcmp(command, "svpm") == 0 || strcmp(command, "saveparam") == 0)
			{
				u8 ret = flash_write_pid("pid", kp, ki, kd);
				printf("�洢pid���������%s\r\n", ret?"�ɹ�":"ʧ��");
				print_param();
			}
			//��flash����pid����
			else if (strcmp(command, "ldpm") == 0 || strcmp(command, "loadparam") == 0)
			{
				u8 ret = flash_read_pid("pid", &kp, &ki, &kd);
				printf("��ȡpid���������%s\r\n", ret?"�ɹ�":"ʧ��");
				print_param();
			}
			//�鿴��ǰ����
			else if (strcmp(command, "shpm") == 0 || strcmp(command, "showparam") == 0)
			{
				print_param();
			}
			//�鿴��ѭ����ʱ
			else if (strcmp(command, "llt") == 0 || strcmp(command, "lastlooptime") == 0)
			{
				printf("������ѭ��ִ��ʱ�䣺%d ms\r\n", last_loop_ms);
			}
			//�洢���ƶ�ʱ������(��λ��us)������flash
			else if (strcmp(command, "svcc") == 0 || strcmp(command, "savecycle") == 0)
			{
				u8 ret = flash_write_cycle("cycle", ctrl_cycle);
				printf("�洢cycle���������%s\r\n", ret?"�ɹ�":"ʧ��");
				print_param();
			}
			//��flash���ؿ��ƶ�ʱ������(��λ��us)����
			else if (strcmp(command, "ldcc") == 0 || strcmp(command, "loadcycle") == 0)
			{
				u8 ret = flash_read_cycle("cycle", &ctrl_cycle);
				printf("��ȡcycle���������%s\r\n", ret?"�ɹ�":"ʧ��");
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
				printf("��ʼ���ƣ�\r\n");
			}
			else if (strcmp(command, "stop") == 0 || strcmp(command, "stopcontrol") == 0)
			{
				TIM3_Int_Stop();
				printf("ֹͣ���ƣ�\r\n");
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
				printf("��ʼ��ӡ�Ӿ���Ϣ\r\n");
			}
			else if (strcmp(command, "stpshvs") == 0 || strcmp(command, "stopshowvision") == 0)
			{
				prog_state.show_vision = 0;
				printf("ֹͣ��ӡ�Ӿ���Ϣ\r\n");
			}
			else
			{
			ERR_CMD: 
				printf("ָ�����\r\n");
			}
			
		}
		
		

		
		/* �յ�һ������uart2����Ϣ */
		int uart2_len = uart2_buf_status();
		if (uart2_len == 4)
		{
			u8 buf[4];
			//��ȡ����ͷ����
			uart2_read_buf(buf, 4);
			obj.x = (buf[1] << 8) + buf[0];
			obj.y = (buf[3] << 8) + buf[2];
			
			if(prog_state.show_vision) printf("�յ��Ӿ����ݣ�centor_x - %d\tcentor_y - %d\r\n", obj.x, obj.y);
		}
		else if (uart2_len > 4) 
		{
			printf("�Ӿ����ݸ�ʽ����\r\n");
			uart2_clear_buf();
		}
		
		last_loop_ms = get_time_ms() - loop_start_ms;
	}
}

void print_param(void)
{
	printf("��ǰ������    kp: %f\tki: %f\tkd: %f\tcycle: %d\r\n", kp, ki, kd, ctrl_cycle);
	float kp = 0, ki = 0, kd = 0;
	u32 ctrl_cycle = 0;
	flash_read_pid("pid", &kp, &ki, &kd);
	flash_read_cycle("cycle", &ctrl_cycle);
	printf("�洢���в�����kp: %f\tki: %f\tkd: %f\tcycle: %d\r\n", kp, ki, kd, ctrl_cycle);
}

