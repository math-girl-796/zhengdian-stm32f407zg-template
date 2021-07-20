#include "flash.h"

u8 flash_init(void)
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
			led_blink5(LED0);	//��ʽ��ʧ��
		delay_ms(1000);
		return 0;
	}		
	return 1;
}


//���ļ���д��pid���ݣ�д��Ḳ��ԭ���ļ��е����ݡ�д��ɹ�����1��д��ʧ�ܷ���0
u8 flash_write_pid(char* filename, float kp, float ki, float kd)
{
	//ȷ��Ҫ��д���ļ���
	char _filename[40];
	strcpy(_filename, "1:");
	strcat(_filename, filename);
	FIL *fil = (FIL*)mymalloc(SRAMIN,sizeof(FIL));
	
	u8 open_val;
	//��д��ʽ���ļ�
	//�Ȳ鿴�ļ��Ƿ���ڣ����������򿪣�����������򴴽�����
	if ((open_val = f_open(fil, (const TCHAR*)_filename, 2)) !=0)
	{
		open_val = f_open(fil, (const TCHAR*)_filename, 7); // �����ļ������Զ�дģʽ��
	}
	
	if(open_val != 0) return 0;
	
	//��byte��ʽд������float
	u32 ret_len;
	u8 buf[12];
	byte_float bf;
	
	bf.f = kp;
	buf[0] = bf.bytes.b0;
	buf[1] = bf.bytes.b1;
	buf[2] = bf.bytes.b2;
	buf[3] = bf.bytes.b3;
	
	bf.f = ki;
	buf[4] = bf.bytes.b0;
	buf[5] = bf.bytes.b1;
	buf[6] = bf.bytes.b2;
	buf[7] = bf.bytes.b3;
	
	bf.f = kd;
	buf[8] = bf.bytes.b0;
	buf[9] = bf.bytes.b1;
	buf[10] = bf.bytes.b2;
	buf[11] = bf.bytes.b3;
	
	u8 ret_val = f_write(fil, buf, 12, &ret_len);
	if (ret_val != 0) return 0;
	
	//�ر��ļ�
	f_close(fil);
	
	if (ret_len != 12) return 0;
	
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
	FIL *fil = (FIL*)mymalloc(SRAMIN,sizeof(FIL));
	
	u8 open_val = 0;
	open_val += 1; //avoid keil compiler warning
	
	//�Զ���ʽ���ļ�������ɹ�����1�����ʧ�ܷ���0��ʧ��ͨ������Ϊ�ļ������ڣ�
	if ((open_val = f_open(fil, (const TCHAR*)_filename, 1)) !=0)
	{
		return 0;
	}
	
	//��byte��ʽд������float
	u32 ret_len;
	u8 buf[12];
	
	u8 ret_val = f_read(fil, buf, 12, &ret_len);
	
	if (ret_val != 0) return 0;
	
	byte_float bf;
	
	bf.bytes.b0 = buf[0];
	bf.bytes.b1 = buf[1];	
	bf.bytes.b2 = buf[2];
	bf.bytes.b3 = buf[3];
	*kp = bf.f;
	
	bf.bytes.b0 = buf[4];
	bf.bytes.b1 = buf[5];	
	bf.bytes.b2 = buf[6];
	bf.bytes.b3 = buf[7];
	*ki = bf.f;
	
	bf.bytes.b0 = buf[8];
	bf.bytes.b1 = buf[9];	
	bf.bytes.b2 = buf[10];
	bf.bytes.b3 = buf[11];
	*kd = bf.f;
	
	if (ret_len != 12) return 0;
	else
	{
		return 1;
	}
}












