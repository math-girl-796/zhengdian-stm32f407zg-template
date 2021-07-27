#include "main.h"

pid pid1, pid2;
pid spid1, spid2;
u32 ctrl_cycle;    // �����ж����ڣ���λus��
u32 last_ctrl_ms;  // �ϴ��жϴ���������ʱ��
u32 last_loop_ms;  // �ϴ���ѭ����ʱ����λms��
u16 camera_width, camera_height; // �������ͼ�����ط�Χ

coord err, ierr, derr, lerr;
coord serr, sierr, sderr, slerr, sllerr;
coord steer_compare;

int16_t InLim = 0; // �����޷�
int16_t InSep = 0; // ���ַ���

coord obj, lobj, tgt;
coord sobj, slobj, stgt;
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
	lcd_init();
	LCD_ShowString(0,16,240,16,16,"usb connect successfully"); // ��ֹusb���Ӻ��ˣ�lcd��û��ʼ�������usb����ʧ�ܣ���Щ������Ȼ�ᱻ���ǡ�
	LCD_Show_Msg("here is message area");
	
	// ��ʼ��һЩ��ֵ
	prog_state.show_vision = 0;
	last_loop_ms = 0;
	camera_width = 240;
	camera_height = 240;
	tgt.x = camera_width / 2;
	tgt.y = camera_height / 2;
	flash_read_pid("pid1", &pid1.kp, &pid1.ki, &pid1.kd); // �ָ�pid����
	flash_read_pid("pid2", &pid2.kp, &pid2.ki, &pid2.kd); // �ָ�pid����
	flash_read_pid("spid1", &spid1.kp, &spid1.ki, &spid1.kd);  // �ָ�pid����
	flash_read_pid("spid2", &spid2.kp, &spid2.ki, &spid2.kd);  // �ָ�pid����
	flash_read_cycle("cycle", &ctrl_cycle); // �ָ�����
	steer1_set_compare(1000);
	steer2_set_compare(2000);
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
				ret = flash_write_pid("spid1", spid1.kp, spid1.ki, spid1.kd);
				printf("�洢spid1���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_write_pid("spid2", spid2.kp, spid2.ki, spid2.kd);
				printf("�洢spid2���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_write_In("In", InLim, InSep);
				printf("�洢In���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_write_cycle("cycle", ctrl_cycle);
				printf("�洢cycle���������%s\r\n", ret?"�ɹ�":"ʧ��");
				
				print_param();
			}
			//��flash����pid����
			else if (strcmp(command, "ldpm") == 0 || strcmp(command, "loadparam") == 0)
			{
				u8 ret = flash_read_pid("pid1", &pid1.kp, &pid1.ki, &pid1.kd); 
				printf("��ȡpid1���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_read_pid("pid2", &pid2.kp, &pid2.ki, &pid2.kd); 
				printf("��ȡpid2���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_read_pid("spid1", &spid1.kp, &spid1.ki, &spid1.kd); 
				printf("��ȡspid1���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_read_pid("spid2", &spid2.kp, &spid2.ki, &spid2.kd); 
				printf("��ȡspid2���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_read_In("In", &InLim, &InSep);
				printf("��ȡIn���������%s\r\n", ret?"�ɹ�":"ʧ��");
				ret = flash_read_cycle("cycle", &ctrl_cycle);
				printf("��ȡcycle���������%s\r\n", ret?"�ɹ�":"ʧ��");
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
				LCD_Show_Msg("start control");
			}
			else if (strcmp(command, "stop") == 0 || strcmp(command, "stopcontrol") == 0)
			{
				TIM3_Int_Stop();
				printf("ֹͣ���ƣ�\r\n");
				LCD_Show_Msg("stop control");
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
			else if (strcmp(command, "skp1") == 0 || strcmp(command, "setskp1") == 0)
			{
				spid1.kp = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "ski1") == 0 || strcmp(command, "setski1") == 0)
			{
				spid1.ki = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "skd1") == 0 || strcmp(command, "setskd1") == 0)
			{
				spid1.kd = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "skp2") == 0 || strcmp(command, "setskp2") == 0)
			{
				spid2.kp = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "ski2") == 0 || strcmp(command, "setski2") == 0)
			{
				spid2.ki = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "skd2") == 0 || strcmp(command, "setskd2") == 0)
			{
				spid2.kd = parse_string(strtok(NULL, " ")).f;
				print_param();
			}
			else if (strcmp(command, "lim") == 0 || strcmp(command, "setInLimit") == 0) // �����޷�
			{
				InLim = parse_string(strtok(NULL, " ")).i;
				print_param();
			}
			else if (strcmp(command, "sep") == 0 || strcmp(command, "setInSep") == 0)  // ���ַ���
			{
				InSep = parse_string(strtok(NULL, " ")).i;
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
			else if (strcmp(command, "shvs") == 0 || strcmp(command, "showvision") == 0)
			{
				prog_state.show_vision = 1;
				printf("��ʼ��ӡ�Ӿ���Ϣ\r\n");
				LCD_Show_Msg("printing vision info");
			}
			else if (strcmp(command, "stpshvs") == 0 || strcmp(command, "stopshowvision") == 0)
			{
				prog_state.show_vision = 0;
				printf("ֹͣ��ӡ�Ӿ���Ϣ\r\n");
				LCD_Show_Msg("stop print vision info");
			}
			else if (strcmp(command, "sk1") == 0 || strcmp(command, "shake1") == 0)
			{
				printf("ҡ�ζ��1\r\n");
				LCD_Show_Msg("shake steer 1");
				steer1_set_compare(500);
				delay_ms(500);
				steer1_set_compare(1500);
				delay_ms(500);
				steer1_set_compare(1000);
			}
			else if (strcmp(command, "sk2") == 0 || strcmp(command, "shake2") == 0)
			{
				printf("ҡ�ζ��2\r\n");
				LCD_Show_Msg("shake steer 2");
				steer2_set_compare(2500);
				delay_ms(500);
				steer2_set_compare(1500);
				delay_ms(500);
				steer2_set_compare(2000);
			}
			else if (strcmp(command, "sk") == 0 || strcmp(command, "shake") == 0)
			{
				printf("ҡ�ζ��\r\n");
				LCD_Show_Msg("shake steer");
				steer1_set_compare(500);
				delay_ms(200);
				steer2_set_compare(2500);
				delay_ms(500);
				steer1_set_compare(1500);
				delay_ms(200);
				steer2_set_compare(1500);
				delay_ms(500);
				steer1_set_compare(1000);
				delay_ms(200);
				steer2_set_compare(2000);
			}
			else if (strcmp(command, "dj") == 0 || strcmp(command, "steer") == 0)
			{ 
				int which = parse_string(strtok(NULL, " ")).i;
				int compare = parse_string(strtok(NULL, " ")).i;
				if (which == 1) steer1_set_compare(compare);
				if (which == 2) steer2_set_compare(compare);
				printf("���ö��%d compare = %d\r\n", which, compare);
				char buf[50];
				sprintf(buf, "set steer%d compare = %d", which, compare);
				LCD_Show_Msg(buf);
			}
			else if (strcmp(command, "lcd") == 0)
			{ 
				lcd_init();
			}
			//��k210������Ϣ
			else if (strcmp(command, "k210") == 0)
			{
				char* k210_command = strtok(NULL, " ");
				uart2_send_bytes((u8*)k210_command, strlen(k210_command));
				printf("�㷢�͵���ϢΪ��%s\r\n", k210_command);
				char buf[85];
				sprintf(buf, "send to k210: %s", k210_command);
				LCD_Show_Msg(buf);
			}
			else
			{
			ERR_CMD: 
				printf("ָ�����\r\n");
				LCD_Show_Msg("illegal command!");
			}
		}
		
		if (key1_state() == 1)
		{
			delay_ms(10);
			TIM3_Int_Stop();
			printf("ֹͣ����");
		}

		
		/* ��ӡȫ�ֱ�����obj������ */
		if(prog_state.show_vision)
		{
			printf("�յ��Ӿ����ݣ�centor_x - %d\tcentor_y - %d\r\n", obj.x, obj.y);
			char buf[25];
			sprintf(buf, "obj pos: (%d, %d)", obj.x, obj.y);
			LCD_Fill(0, 32,240,48,WHITE);	
			LCD_ShowString(0,32,240,16,16,buf);
		}
	}
}

// �ڴ��ں�lcd��ӡ������Ϣ
void print_param(void)
{
	printf("��ǰ������    kp1: %f\tki1: %f\tkd1: %f\tcycle: %d\t\r\n\
              kp2: %f\tki2: %f\tkd2: %f\tĿ���(%d, %d)\r\n\
              skp1: %f\tski1: %f\tskd1: %f\tInLim: %d\r\n\
              skp2: %f\tski2: %f\tskd2: %f\tInSep: %d\r\n", 
				pid1.kp, pid1.ki, pid1.kd, ctrl_cycle, 
				pid2.kp, pid2.ki, pid2.kd, tgt.x, tgt.y,
				spid1.kp, spid1.ki, spid1.kd, InLim, 
				spid2.kp, spid2.ki, spid2.kd, InSep);
	char buf1[200];
	sprintf(buf1, "pm:kp1:%f ki1:%f kd1:%f cycle:%d kp2:%f ki2:%f kd2:%f target(%d,%d)", pid1.kp, pid1.ki, pid1.kd, ctrl_cycle, pid2.kp, pid2.ki, pid2.kd, tgt.x, tgt.y);
	
	//���洢�Ĳ���������ʱ����������ʾ
	pid pid1, pid2;
	pid spid1, spid2;
	int16_t InLim, InSep;
	u32 ctrl_cycle = 0;
	flash_read_pid("pid1", &pid1.kp, &pid1.ki, &pid1.kd); 
	flash_read_pid("pid2", &pid2.kp, &pid2.ki, &pid2.kd); 
	flash_read_pid("spid1", &spid1.kp, &spid1.ki, &spid1.kd);
	flash_read_pid("spid2", &spid2.kp, &spid2.ki, &spid2.kd);
	flash_read_In("In", &InLim, &InSep);
	flash_read_cycle("cycle", &ctrl_cycle);
	printf("�洢���в�����kp1: %f\tki1: %f\tkd1: %f\tcycle: %d\r\n\
              kp2: %f\tki2: %f\tkd2: %f\r\n\
              skp1: %f\tski1: %f\tskd1: %f\tInLim: %d\r\n\
              skp2: %f\tski2: %f\tskd2: %f\tInSep: %d\r\n",  
			  pid1.kp, pid1.ki, pid1.kd, ctrl_cycle, 
			  pid2.kp, pid2.ki, pid2.kd,
				spid1.kp, spid1.ki, spid1.kd, InLim,
				spid2.kp, spid2.ki, spid2.kd, InSep);
	char buf2[200];
	sprintf(buf2, "%s saved_pm:kp1:%f ki1:%f kd1:%f cycle:%d kp2:%f ki2:%f kd2:%f", buf1, pid1.kp, pid1.ki, pid1.kd, ctrl_cycle, pid2.kp, pid2.ki, pid2.kd);
	LCD_Show_Msg(buf2);
}


// k210 uartЭ�飺[0xb3, 0xb3, cmd, data * 8]
// cmd: 0x01-���ݣ�0x02-��Ϣ
extern u16 USART2_RX_STA;
extern u8 USART2_RX_BUF[USART2_REC_LEN]; 
void USART2_IRQHandler(void)                	//�жϷ������
{
	static uint8_t rebuf[20]={0},i=0;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
	{
		rebuf[i++]=USART_ReceiveData(USART2);
		
		if(rebuf[0]!=0xb3)//֡ͷ
			i=0;
		else if((i==2)&&(rebuf[1]!=0xb3))//�ж�֡ͷ
			i=0;
		else if(i>=11)//����һ֡�������
		{
			if (rebuf[2] == 0x01) // ����֡
			{
				lobj = obj; // ��¼�ϸ��۲�����
				memcpy(&obj,&rebuf[3], 4);
			}
			else  // ��Ϣ֡
			{
				rebuf[19] = 0; //ȷ���ַ����������Ľ�β
				printf("%s", &rebuf[3]);
			}
			i = 0;
		}
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
	}
} 


/* �˴�д���ƴ��� */
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
	{
		
		//��¼��ʼʱ��
		u32 ctrl_start_ms = get_time_ms();
		
		//����������� - λ��ʽpid
		err.x = tgt.x - obj.x;
		err.y = tgt.y - obj.y;
		
		//���ַ���
		if (abs(ierr.x) < InSep)
		{
			ierr.x += err.x;
		}
		else
		{
			ierr.x = 0;
		}
		if (abs(ierr.y) < InSep)
		{
			ierr.y += err.y;
		}
		else
		{
			ierr.y = 0;
		}
		
		//�����޷�
		ierr.x = clip(ierr.x, 0 - InLim, InLim);
		ierr.y = clip(ierr.y, 0 - InLim, InLim);
		
		derr.x = err.x - lerr.x;
		derr.y = err.y - lerr.y;
		
		lerr = err;
		
		//����Ŀ���ٶ�
		stgt.x = (int) (pid1.kp * err.x + pid1.kd * derr.x + pid1.ki * ierr.x);
		stgt.y = (int) (pid2.kp * err.y + pid2.kd * derr.y + pid2.ki * ierr.y);
		
		//��ȡ��ǰ�ٶ�
		sobj.x = obj.x - lobj.x;
		sobj.y = obj.y - lobj.y;
		
		//�����ٶ���� - ����ʽpid
		serr.x = stgt.x - sobj.x;
		serr.y = stgt.y - sobj.y;
		
		sierr.x = sobj.x;
		sierr.y = sobj.y;
		
		sderr.x = serr.x - 2 * slerr.x + sllerr.x;
		sderr.y = serr.y - 2 * slerr.y + sllerr.y;
		
		sllerr = slerr;
		slerr = serr;
		
		//ͨ������ֵ��ƶ��ֵ
		int derta_compare1 = (int) (spid1.kp * serr.x + spid1.kd * sderr.x + spid1.ki * sierr.x);
		int derta_compare2 = (int) (spid2.kp * serr.y + spid2.kd * sderr.y + spid2.ki * sierr.y);
		
		steer_compare.x = clip_1500_2500(steer_compare.x + derta_compare1);
		steer_compare.y = clip_500_1500(steer_compare.y + derta_compare2);
		
		steer2_set_compare(steer_compare.x); // steer2����x����
		steer1_set_compare(steer_compare.y); // steer1����y����
		
		led_switch(LED1);
				
		
		//��¼�жϴ���������ʱ��
		last_ctrl_ms = get_time_ms() - ctrl_start_ms;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
}








