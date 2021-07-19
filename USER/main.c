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
#include "lcd.h"
#include "touch.h"
#include "rocker.h"
#include "usb_conf.h" 
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h" 
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "piclib.h"	 
#include "usbh_usr.h"
#include "usmart.h"

#define USBH_USR_OTG_FS_IRQH

void test_uart1(void);
void test_uart2(void);
void test_uart3(void);
void test_uart4(void);
void test_uart5(void);
void test_uart6(void);
void test_led(void);
void test_key(void); //uart1���ӵ��ԣ�ͨ��XCOM�鿴��֤
void test_stepper_motor(void);
void test_pwm_TIM13_CH1(void);
void test_pwm_TIM14_CH1(void);
void test_pwm_and_uart1(void);
void test_steer1_and_uart1(void); // ���
void test_pid_camera_and_steer1(void);
void test_parse_bytes_and_uart1(void);
void test_lcd_show(void);
void test_lcd_touch_screen(void);
void test_rocker(void);
//void test_usb_slave(void);
void test_usb_host(void);

int main(void)
{	
	test_usb_host();
}


//test_usb_host
//���otg����USB_APP/usb_conf.h��173��������������н⿪define USE_HOST_MODE��ע�ͣ���������ע�͵�
USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;
//�û�����������
//����ֵ:0,����
//       1,������
u8 USH_User_App(void)
{ 
	u32 total,free;
	u8 res=0;
	Show_Str(30,140,200,16,"�豸���ӳɹ�!.",16,0);	 
	res=exf_getfree("2:",&total,&free);
	if(res==0)
	{
		POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
		LCD_ShowString(30,160,200,16,16,"FATFS OK!");	
		LCD_ShowString(30,180,200,16,16,"U Disk Total Size:     MB");	 
		LCD_ShowString(30,200,200,16,16,"U Disk  Free Size:     MB"); 	    
		LCD_ShowNum(174,180,total>>10,5,16); //��ʾU�������� MB
		LCD_ShowNum(174,200,free>>10,5,16);	
	} 
 
	while(HCD_IsDeviceConnected(&USB_OTG_Core))//�豸���ӳɹ�
	{	
		led_switch(LED0);
		delay_ms(200);
	}
	POINT_COLOR=RED;//��������Ϊ��ɫ	   
	Show_Str(30,140,200,16,"�豸������...",16,0);
	LCD_Fill(30,160,239,220,WHITE);
	return res;
} 
void test_usb_host(void)
{        
	u8 t;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();  //��ʼ����ʱ����
	uart1_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	led_init();				//��ʼ����LED���ӵ�Ӳ���ӿ�
	key_init();				//����
  	LCD_Init();				//��ʼ��LCD 
	W25QXX_Init();			//SPI FLASH��ʼ��
	usmart_dev.init(84); 	//��ʼ��USMART	 
	my_mem_init(SRAMIN);	//��ʼ���ڲ��ڴ��	
 	exfuns_init();			//Ϊfatfs��ر��������ڴ� 
	piclib_init();			//��ʼ����ͼ
  	f_mount(fs[0],"0:",1); 	//����SD��  
  	f_mount(fs[1],"1:",1); 	//����SD��  
  	f_mount(fs[2],"2:",1); 	//����U��
	POINT_COLOR=RED;      
 	while(font_init()) 				//����ֿ�
	{	    
		LCD_ShowString(60,50,200,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}
	Show_Str(30,50,200,16,"̽����STM32F407������",16,0);				    	 
	Show_Str(30,70,200,16,"USB U��ʵ��",16,0);					    	 
	Show_Str(30,90,200,16,"2014��7��22��",16,0);	    	 
	Show_Str(30,110,200,16,"����ԭ��@ALIENTEK",16,0); 
	Show_Str(30,140,200,16,"�豸������...",16,0);			 		
	//��ʼ��USB����
  	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);  
	while(1)
	{
		USBH_Process(&USB_OTG_Core, &USB_Host);
		delay_ms(1);
		t++;
		if(t==200)
		{
			led_switch(LED0);
			t=0;
		}
	}	
}

//// ���sd������USB_APP/usb_conf.h��173��������������н⿪define USE_DEVICE_MODE��ע�ͣ���������ע�͵�
// USB_OTG_CORE_HANDLE USB_OTG_dev;
//void test_usb_slave(void)
//{
//	u8 offline_cnt=0;
//	u8 tct=0;
//	u8 USB_STA;
//	u8 Divece_STA;

//	extern vu8 USB_STATUS_REG;		//USB״̬
//	extern vu8 bDeviceState;		//USB���� ���

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
//	delay_init();  //��ʼ����ʱ����
//	uart1_init(115200);		//��ʼ�����ڲ�����Ϊ115200
//	led_init();					//��ʼ��LED  
// 	LCD_Init();					//LCD��ʼ��  
// 	key_init();					//������ʼ��  
//	W25QXX_Init();				//��ʼ��W25Q128  
//  
// 	POINT_COLOR=RED;//��������Ϊ��ɫ	   
//	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
//	LCD_ShowString(30,70,200,16,16,"USB Card Reader TEST");	
//	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
//	LCD_ShowString(30,110,200,16,16,"2014/7/21");	
//	if(SD_Init())LCD_ShowString(30,130,200,16,16,"SD Card Error!");	//���SD������
//	else //SD ������
//	{   															  
//		LCD_ShowString(30,130,200,16,16,"SD Card Size:     MB"); 
// 		LCD_ShowNum(134,130,SDCardInfo.CardCapacity>>20,5,16);	//��ʾSD������
// 	}
//	if(W25QXX_ReadID()!=W25Q128 && W25QXX_ReadID()!=NM25Q128)
//		LCD_ShowString(30,130,200,16,16,"W25Q128 Error!");	//���W25Q128����
//	else //SPI FLASH ����
//	{   														 
//		LCD_ShowString(30,150,200,16,16,"SPI FLASH Size:12MB");	 
//	}  
// 	LCD_ShowString(30,170,200,16,16,"USB Connecting...");//��ʾ���ڽ������� 	    
//	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
//	delay_ms(1800);	
//	while(1)
//	{	
//		delay_ms(1);				  
//		if(USB_STA!=USB_STATUS_REG)//״̬�ı��� 
//		{	 						   
//			LCD_Fill(30,190,240,190+16,WHITE);//�����ʾ			  	   
//			if(USB_STATUS_REG&0x01)//����д		  
//			{
//				led_on(LED1);
//				LCD_ShowString(30,190,200,16,16,"USB Writing...");//��ʾUSB����д������	 
//			}
//			if(USB_STATUS_REG&0x02)//���ڶ�
//			{
//				led_on(LED1);
//				LCD_ShowString(30,190,200,16,16,"USB Reading...");//��ʾUSB���ڶ�������  		 
//			}	 										  
//			if(USB_STATUS_REG&0x04)LCD_ShowString(30,210,200,16,16,"USB Write Err ");//��ʾд�����
//			else LCD_Fill(30,210,240,210+16,WHITE);//�����ʾ	  
//			if(USB_STATUS_REG&0x08)LCD_ShowString(30,230,200,16,16,"USB Read  Err ");//��ʾ��������
//			else LCD_Fill(30,230,240,230+16,WHITE);//�����ʾ    
//			USB_STA=USB_STATUS_REG;//��¼����״̬
//		}
//		if(Divece_STA!=bDeviceState) 
//		{
//			if(bDeviceState==1)LCD_ShowString(30,170,200,16,16,"USB Connected    ");//��ʾUSB�����Ѿ�����
//			else LCD_ShowString(30,170,200,16,16,"USB DisConnected ");//��ʾUSB���γ���
//			Divece_STA=bDeviceState;
//		}
//		tct++;
//		if(tct==200)
//		{
//			tct=0;
//			led_off(LED1);
//			led_switch(LED0);//��ʾϵͳ������
//			if(USB_STATUS_REG&0x10)
//			{
//				offline_cnt=0;//USB������,�����offline������
//				bDeviceState=1;
//			}else//û�еõ���ѯ 
//			{
//				offline_cnt++;  
//				if(offline_cnt>10)bDeviceState=0;//2s��û�յ����߱��,����USB���γ���
//			}
//			USB_STATUS_REG=0;
//		}
//	};  
//}



void test_pwm_TIM13_CH1(void)
{
	int light_level;
	led_init();
	delay_init();
	uart1_init(115200);
	TIM13_CH1_PWM_Init_us(200);
	while(1)
	{
		for(light_level = 0; light_level < 200; light_level++)
		{
			TIM13_CH1_PWM_Set_Compare(light_level);
			delay_ms(5);
		}
		for(light_level = 200; light_level > 0; light_level--)
		{
			TIM13_CH1_PWM_Set_Compare(light_level);
			delay_ms(5);
		}
	}
}

//ע��ҡ�˵�Դ����3v3�������ܽſ�rocker.c
void test_rocker(void)
{
	int16_t adcx1 = 0;
	int16_t adcx2 = 0;
	u8 key = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	delay_init();	    	 //��ʱ������ʼ��	  
	uart1_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	led_init();			     //LED�˿ڳ�ʼ��
 	
 	rocker_init();		  		//ADC��ʼ��
	
	while(1)
	{
		
		adcx1=rocker_x();
		adcx2=rocker_y();
		key=rocker_sw();
		
		
		printf("x:%d\ty:%d\tkey:%d\r\n", adcx1, adcx2, key);
		
		led_switch(LED0);
		delay_ms(500);	
	}
}


void test_lcd_touch_screen(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();
	
	led_init();
	LCD_Init();
	key_init();
	
	tp_dev.init();				//��������ʼ��
	POINT_COLOR=RED;
	
	LCD_ShowString(0,0,200,16,16,"fuck");
	
	while(1)
	{
		tp_dev.scan(0); //ɨ�败����.0,��Ļɨ��;1,��������;	 
		if (key0_state() == 1) // ������0����У׼�����ҵ���LED0��У׼��ɹر�LED0
		{
			led_on(LED0);
			LCD_Clear(WHITE);	//����
		    TP_Adjust();  		//��ĻУ׼ 
			TP_Save_Adjdata();	//����У׼���
			led_off(LED0);		
		}
		if (tp_dev.sta & TP_PRES_DOWN) // ����Ļ�ϰ�LED11�����°���
		{
			if(tp_dev.y[0] < lcddev.height / 2) led_on(LED1);
			if(tp_dev.y[0] > lcddev.height / 2) led_off(LED1);
			delay_ms(10); // ����
		}
	}
}

void test_lcd_show(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();      //��ʼ����ʱ����
	
	led_init();					  //��ʼ��LED
 	LCD_Init();           //��ʼ��LCD FSMC�ӿ�
	POINT_COLOR=RED;      //������ɫ����ɫ
	LCD_ShowString(0,0,200,16,16,"fuck");
	LCD_ShowString(0,16,200,16,16,"your");		//��ʾLCD ID	      					 
	LCD_ShowString(0,32,200,16,16,"mother");	
	LCD_ShowxNum(0,48,123456,9,16,(1 << 7) + 1); // ���Ӳ���
	LCD_ShowxNum(0,48,654321,9,16,(1 << 7) + 1); // ���Ӳ���
	LCD_ShowxNum(0,64,123456,9,16,(1 << 7) + 0); // ���Ӳ���
	LCD_ShowxNum(0,64,654321,9,16,(1 << 7) + 0); // �ǵ��Ӳ���
}


// ��������
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
			u8 buf[200];
			printf("len: %d\t", len);
			uart1_read_buf(buf, len);
			buf[len] = 0; // �������ǳ��ؼ����������Ҳ���Ըĳɡ����buf��
			
			num = parse_string((char*) buf);
			printf("num.i: %d\tnum.f: %f\t\r\n",num.i, num.f);
		}
	}
	
}


// steer1����9g�����k210pin15���ӱ���uart2���Ϳڣ�k210�����ļ���TEST�ļ���
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
			//��ȡ����ͷ����
			uart2_read_buf(buf, 2);
			extern_x = (buf[1] << 8) + buf[0];
			printf("extern_x: %d\t", extern_x);
			
			//�������ֵ
			error = target_x - extern_x;
			derror = error - error_last;
			ierror = ierror + error;
			error_last = error;
			printf("error: %d\t", error);
			
			//�޼�ierror��ֹ��̫��
			if (ierror > 3000) ierror = 3000;
			if (ierror < -3000) ierror = -3000;
			
			//�����ṩ��pwm��Compare�ĸ��²�ֵ
			dia = (int) (kp * error + kd * derror + ki * ierror);
			printf("dia: %d\t", dia);
			
			//����pwmvֵ
			pwmv += dia;
			
			//�޼�pwmvֵ��ֹ��������Χ
			if (pwmv > 2500) pwmv = 2500;
			if (pwmv < 500) pwmv = 500;
			printf("new_pwmv:%d\t", pwmv);
			
			//���ö����λ��
			steer1_set_compare(pwmv);
			printf("\r\n");
		}
		if (len1 == 3)
		{
			//��ȡ����ͷ����
			uart2_read_buf(buf, 2);
			extern_x = (buf[1] << 8) + buf[0];
			printf("extern_x: %d\t", extern_x);
			
			//�������ֵ
			error = target_x - extern_x;
			derror = error - error_last;
			ierror = ierror + error;
			error_last = error;
			printf("error: %d\t", error);
			
			//�޼�ierror��ֹ��̫��
			if (ierror > 3000) ierror = 3000;
			if (ierror < -3000) ierror = -3000;
			
			//�����ṩ��pwm��Compare�ĸ��²�ֵ
			dia = (int) (kp * error + kd * derror + ki * ierror);
			printf("dia: %d\t", dia);
			
			//����pwmvֵ
			pwmv += dia;
			
			//�޼�pwmvֵ��ֹ��������Χ
			if (pwmv > 2500) pwmv = 2500;
			if (pwmv < 500) pwmv = 500;
			printf("new_pwmv:%d\t", pwmv);
			
			//���ö����λ��
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

void test_steer1_and_uart1(void) //ʹ��ǰ������TIM14_CH1_PWM�ܽ�ΪPA7.���Ի᲻ͣ���յ�hello�����Ը�f4��0-20000��int�����ݣ������ֽڣ�����ı�pwmռ�ձȣ�������Χ���ݿ��ܻᵼ�³���
	//��Ч��ΧΪ500-2500����ʾ0��-180��
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

void test_pwm_and_uart1(void) //ʹ��ǰ������TIM14_CH1_PWM�ܽ�ΪPF9.���Ի᲻ͣ���յ�hello�����Ը�f4��0-500��short�����ݣ������ֽڣ�����ı�led���ȣ�������Χ���ݿ��ܻᵼ�³���
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


void test_pwm_TIM14_CH1(void) // ʹ��ǰ������TIM14_CH1_PWM�ܽ�ΪPF9����led��
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


