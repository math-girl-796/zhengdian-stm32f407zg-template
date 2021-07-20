#include "flash.h"

void flash_init()
{
	u8 res;
	
	delay_init();
	uart1_init(115200);
	W25QXX_Init();				//��ʼ��W25Q128
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	exfuns_init();							//Ϊfatfs��ر��������ڴ�
	
	res=f_mount(fs[1],"1:",1); 				//����FLASH.	
	if(res==0X0D)//FLASH����,FAT�ļ�ϵͳ����,���¸�ʽ��FLASH
	{
		res=f_mkfs("1:",1,4096);//��ʽ��FLASH,1,�̷�;1,����Ҫ������,8������Ϊ1����
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//����Flash���̵�����Ϊ��ALIENTEK
		}else 
			led_blink3(LED0);	//��ʽ��ʧ��
		delay_ms(1000);
	}		
}


//���ļ���д��pid���ݣ�д��Ḳ��ԭ���ļ��е����ݡ�д��ɹ�����1��д��ʧ�ܷ���0
u8 flash_write_pid(char* filename, float kp, float ki, float kd)
{
	//ȷ��Ҫ��д���ļ���
	char _filename[40];
	strcpy(_filename, "1:");
	strcat(_filename, filename);
	FIL *fil;
	
	//��д��ʽ���ļ�
	//�Ȳ鿴�ļ��Ƿ���ڣ���������򱸷ݲ��򿪣�����������򴴽�����
	if (f_open(fil, (const TCHAR*)_filename, 0) !=0)
	{
		f_open(fil, (const TCHAR*)_filename, 7); // �����ļ������Զ�дģʽ��
	}
	
	//��byte��ʽд������float
	u32 ret_len;
	u8 buf[6];
	byte_float bf;
	
	bf.f = kp;
	buf[0] = bf.b[0];
	buf[1] = bf.b[1];
	
	bf.f = ki;
	buf[2] = bf.b[0];
	buf[3] = bf.b[1];
	
	bf.f = kd;
	buf[4] = bf.b[0];
	buf[5] = bf.b[1];
	
	f_write(fil, buf, 6, &ret_len);
	
	//�ر��ļ�
	f_close(fil);
	
	if (ret_len != 6) return 0;
	
	return 1;
}


//�������ڶ�Ӧ�ļ���pid����д�봫��ĺ�����������
//���ض�ȡ�����1-�ɹ���0-ʧ��
u8 flash_read_pid(char* filename, float* kp, float* ki, float* kd)
{
	//ȷ��Ҫ��д���ļ���
	char _filename[40];
	strcpy(_filename, "1:");
	strcat(_filename, filename);
	FIL *fil;
	
	
	//�Զ���ʽ���ļ�������ɹ�����1�����ʧ�ܷ���0��ʧ��ͨ������Ϊ�ļ������ڣ�
	if (f_open(fil, (const TCHAR*)_filename, 1) !=0)
	{
		return 0;
	}
	
	//��byte��ʽд������float
	u32 ret_len;
	u8 buf[6];
	f_read(fil, buf, 6, &ret_len);
	
	byte_float bf;
	
	bf.b[0] = buf[0];
	bf.b[1] = buf[1];
	*kp = bf.f;
	
	bf.b[0] = buf[2];
	bf.b[1] = buf[3];
	*ki = bf.f;
	
	bf.b[0] = buf[4];
	bf.b[1] = buf[5];
	*kd = bf.f;
	
	if (ret_len != 6) return 0;
	else
	{
		return 1;
	}
}












