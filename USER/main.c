#include "main.h"

pid pid1;
pid pid2;
u32 ctrl_cycle;    // �����ж����ڣ���λus��
u32 last_ctrl_ms;  // �ϴ��жϴ���������ʱ��
u32 last_loop_ms;  // �ϴ���ѭ����ʱ����λms��
u16 camera_width, camera_height; // �������ͼ�����ط�Χ
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
	steer1_init();
	steer2_init();
	
	// ��ʼ��һЩ��ֵ
	prog_state.show_vision = 0;
	last_loop_ms = 0;
	camera_width = 240;
	camera_height = 240;
	tgt.x = camera_width / 2;
	tgt.y = camera_height / 2;
	flash_read_pid("pid1", &pid1.kp, &pid1.ki, &pid1.kd); // �ָ�pid����
	flash_read_pid("pid2", &pid2.kp, &pid2.ki, &pid2.kd); // �ָ�pid����
	flash_read_cycle("cycle", &ctrl_cycle); // �ָ�����
	steer1_set_compare(1000);
	steer2_set_compare(1000);
	UART2_TIMEOUT = 500;
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
				u8 ret = flash_write_pid("pid1", pid1.kp, pid1.ki, pid1.kd);
				printf("�洢pid1���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_write_pid("pid2", pid2.kp, pid2.ki, pid2.kd);
				printf("�洢pid2���������%s\r\n", ret?"�ɹ�":"ʧ��");
				print_param();
			}
			//��flash����pid����
			else if (strcmp(command, "ldpm") == 0 || strcmp(command, "loadparam") == 0)
			{
				u8 ret = flash_read_pid("pid1", &pid1.kp, &pid1.ki, &pid1.kd); 
				printf("��ȡpid1���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_read_pid("pid2", &pid2.kp, &pid2.ki, &pid2.kd); 
				printf("��ȡpid2���������%s\r\n", ret?"�ɹ�":"ʧ��");
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
				printf("������ѭ��ִ��ʱ�䣺%d ms\r\n���ֿ��ƴ���ִ��ʱ�䣺%d ms\r\n", last_loop_ms, last_ctrl_ms);
				printf("�յ��Ӿ����ݣ�centor_x - %d\tcentor_y - %d\r\n", obj.x, obj.y);
//				int len = uart2_buf_status();
//				char buf[10];
//				uart2_read_buf((u8*)buf, len);
//				printf("uart2�������ַ�������%d\t���ݣ�%s\r\n", uart2_buf_status(), buf);
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
				printf("��ʼ��ӡ�Ӿ���Ϣ\r\n");
			}
			else if (strcmp(command, "stpshvs") == 0 || strcmp(command, "stopshowvision") == 0)
			{
				prog_state.show_vision = 0;
				printf("ֹͣ��ӡ�Ӿ���Ϣ\r\n");
			}
			//��k210������Ϣ
			else if (strcmp(command, "k210") == 0)
			{
				char* k210_command = strtok(NULL, " ");
				uart2_send_bytes((u8*)k210_command, strlen(k210_command));
				printf("�㷢�͵���ϢΪ��%s\r\n", k210_command);
			}
			else
			{
			ERR_CMD: 
				printf("ָ�����\r\n");
			}
		}
		
		

		
		/* �յ�һ������uart2����Ϣ */
		if(prog_state.show_vision) printf("�յ��Ӿ����ݣ�centor_x - %d\tcentor_y - %d\r\n", obj.x, obj.y);
	}
}

void print_param(void)
{
	printf("��ǰ������    kp1: %f\tki1: %f\tkd1: %f\tcycle: %d\t\r\n\
              kp2: %f\tki2: %f\tkd2: %f\tĿ���(%d, %d)\r\n", pid1.kp, pid1.ki, pid1.kd, ctrl_cycle, pid2.kp, pid2.ki, pid2.kd, tgt.x, tgt.y);
	pid pid1, pid2;
	u32 ctrl_cycle = 0;
	flash_read_pid("pid1", &pid1.kp, &pid1.ki, &pid1.kd); 
	flash_read_pid("pid2", &pid2.kp, &pid2.ki, &pid2.kd); 
	flash_read_cycle("cycle", &ctrl_cycle);
	printf("�洢���в�����kp1: %f\tki1: %f\tkd1: %f\tcycle: %d\r\n\
              kp2: %f\tki2: %f\tkd2: %f\r\n",  pid1.kp, pid1.ki, pid1.kd, ctrl_cycle, pid2.kp, pid2.ki, pid2.kd);
}


extern u16 USART2_RX_STA;
extern u8 USART2_RX_BUF[USART2_REC_LEN]; 
void USART2_IRQHandler(void)                	//�жϷ������
{
	static uint8_t rebuf[8]={0},i=0;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		rebuf[i++]=USART_ReceiveData(USART2);	
		if(rebuf[0]!=0xb3)//֡ͷ
			i=0;
		else if((i==2)&&(rebuf[1]!=0xb3))//�ж�֡ͷ
			i=0;
		else if(i>=6)//����һ֡�������
		{
			memcpy(&obj,&rebuf[2], 4);
			i = 0;
		}
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
	}
} 


