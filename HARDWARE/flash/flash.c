#include "flash.h"

void flash_init()
{
	u8 res;
	
	delay_init();
	uart1_init(115200);
	W25QXX_Init();				//初始化W25Q128
	my_mem_init(SRAMIN);		//初始化内部内存池 
	my_mem_init(SRAMCCM);		//初始化CCM内存池
	exfuns_init();							//为fatfs相关变量申请内存
	
	res=f_mount(fs[1],"1:",1); 				//挂载FLASH.	
	if(res==0X0D)//FLASH磁盘,FAT文件系统错误,重新格式化FLASH
	{
		res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		if(res==0)
		{
			f_setlabel((const TCHAR *)"1:ALIENTEK");	//设置Flash磁盘的名字为：ALIENTEK
		}else 
			led_blink3(LED0);	//格式化失败
		delay_ms(1000);
	}		
}


//向文件中写入pid数据，写入会覆盖原本文件中的内容。写入成功返回1，写入失败返回0
u8 flash_write_pid(char* filename, float kp, float ki, float kd)
{
	//确定要读写的文件名
	char _filename[40];
	strcpy(_filename, "1:");
	strcat(_filename, filename);
	FIL *fil;
	
	//以写方式打开文件
	//先查看文件是否存在，如果存在则备份并打开，如果不存在则创建并打开
	if (f_open(fil, (const TCHAR*)_filename, 0) !=0)
	{
		f_open(fil, (const TCHAR*)_filename, 7); // 创建文件，并以读写模式打开
	}
	
	//以byte形式写入三个float
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
	
	//关闭文件
	f_close(fil);
	
	if (ret_len != 6) return 0;
	
	return 1;
}


//将存在于对应文件的pid数据写入传入的后三个参数中
//返回读取情况：1-成功，0-失败
u8 flash_read_pid(char* filename, float* kp, float* ki, float* kd)
{
	//确定要读写的文件名
	char _filename[40];
	strcpy(_filename, "1:");
	strcat(_filename, filename);
	FIL *fil;
	
	
	//以读方式打开文件，如果成功返回1，如果失败返回0（失败通常是因为文件不存在）
	if (f_open(fil, (const TCHAR*)_filename, 1) !=0)
	{
		return 0;
	}
	
	//以byte形式写入三个float
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












