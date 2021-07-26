#include "usbh_usr.h"
#include "usb_hcd_int.h"
#include "usbh_hid_mouse.h"
#include "usbh_hid_keybd.h"  
#include "delay.h"  
#include "lcd.h"  
#include "string.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//USBH-USR 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/7/23
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//*******************************************************************************
//修改信息
//无
////////////////////////////////////////////////////////////////////////////////// 	   
 
//表示USB连接状态
//0,没有连接;
//1,已经连接;
vu8 bDeviceState=0;		//默认没有连接  


extern USB_OTG_CORE_HANDLE USB_OTG_Core_dev;
void USBH_Msg_Show(u8 msgx)
{
	POINT_COLOR=RED;
	switch(msgx)
	{
		case 0:	//USB无连接
			LCD_ShowString(30,130,200,16,16,"USB Connecting...");	
			LCD_Fill(0,150,lcddev.width,lcddev.height,WHITE);
			break;
		case 1:	//USB键盘
			LCD_ShowString(30,130,200,16,16,"USB Connected    ");	
			LCD_ShowString(30,150,200,16,16,"USB KeyBoard");	 
			LCD_ShowString(30,180,210,16,16,"KEYVAL:");	
			LCD_ShowString(30,200,210,16,16,"INPUT STRING:");	
			break;
		case 2:	//USB鼠标
			LCD_ShowString(30,130,200,16,16,"USB Connected    ");	
			LCD_ShowString(30,150,200,16,16,"USB Mouse");	 
			LCD_ShowString(30,180,210,16,16,"BUTTON:");	
			LCD_ShowString(30,200,210,16,16,"X POS:");	
			LCD_ShowString(30,220,210,16,16,"Y POS:");	
			LCD_ShowString(30,240,210,16,16,"Z POS:");	
			break; 		
		case 3:	//不支持的USB设备
			LCD_ShowString(30,130,200,16,16,"USB Connected    ");	
			LCD_ShowString(30,150,200,16,16,"Unknow Device");	 
			break; 	 
	} 
}  
u8 USB_FIRST_PLUGIN_FLAG=0;	//USB第一次插入标志,如果为1,说明是第一次插入

//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void)
{ 
	USBH_OTG_ISR_Handler(&USB_OTG_Core_dev);
}  

//USB HOST 用户回调函数.
USBH_Usr_cb_TypeDef USR_Callbacks =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  NULL,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};
 
//USB HOST 初始化 
void USBH_USR_Init(void)
{
	printf("USB OTG FS MSC Host\r\n");
	printf("> USB Host library started.\r\n");
	printf("  USB Host Library v2.1.0\r\n\r\n");
	
}
//检测到U盘插入
void USBH_USR_DeviceAttached(void)//U盘插入
{ 
	printf("检测到USB设备插入!\r\n");
}
//检测到U盘拔出
void USBH_USR_DeviceDisconnected (void)//U盘移除
{ 
	printf("USB设备拔出!\r\n");
	bDeviceState=0;	//USB设备拔出了
//	USBH_Msg_Show(0);
}  
//复位从机
void USBH_USR_ResetDevice(void)
{
	printf("复位设备...\r\n");
}
//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
		printf("高速(HS)USB设备!\r\n");
 	}  
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
		printf("全速(FS)USB设备!\r\n"); 
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{
		printf("低速(LS)USB设备!\r\n");  
	}
	else
	{
		printf("设备错误!\r\n");  
	}
}
//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
	USBH_DevDesc_TypeDef *hs;
	hs=DeviceDesc;   
	printf("VID: %04Xh\r\n" , (uint32_t)(*hs).idVendor); 
	printf("PID: %04Xh\r\n" , (uint32_t)(*hs).idProduct); 
}
//从机地址分配成功
void USBH_USR_DeviceAddressAssigned(void)
{
	printf("从机地址分配成功!\r\n");   
}
//配置描述符获有效
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id; 
	id = itfDesc;   
	if((*id).bInterfaceClass==0x08)
	{
		printf("可移动存储器设备!\r\n"); 
	}else if((*id).bInterfaceClass==0x03)
	{
		printf("HID 设备!\r\n"); 
	}    
}
//获取到设备Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}
//获取到设备Product String 
void USBH_USR_Product_String(void *ProductString)
{
	printf("Product: %s\r\n",(char *)ProductString);  
}
//获取到设备SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
	printf("Serial Number: %s\r\n",(char *)SerialNumString);    
} 
//设备USB枚举完成
void USBH_USR_EnumerationDone(void)
{ 
	printf("设备枚举完成!\r\n\r\n");    
} 
//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void)
{ 
//	USBH_Msg_Show(3);//无法识别的USB设备
	printf("无法识别的USB设备!\r\n\r\n");    
}  
//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void)
{ 
	printf("跳过用户确认步骤!\r\n");
	LCD_ShowString(0,16,240,16,16,"连接成功");
	bDeviceState=1;//USB设备已经连接成功
	
	return USBH_USR_RESP_OK;
} 
//USB接口电流过载
void USBH_USR_OverCurrentDetected (void)
{
	printf("端口电流过大!!!\r\n");
}  
//重新初始化
void USBH_USR_DeInit(void)
{
	printf("重新初始化!!!\r\n");
	LCD_ShowString(0,16,240,16,16,"usb连接中");
}
//无法恢复的错误!!  
void USBH_USR_UnrecoveredError (void)
{
	printf("无法恢复的错误!!!\r\n\r\n");	
}
//////////////////////////////////////////////////////////////////////////////////////////
//下面两个函数,为ALIENTEK添加,以防止USB死机

//USB枚举状态死机检测,防止USB枚举失败导致的死机
//phost:USB_HOST结构体指针
//返回值:0,没有死机
//       1,死机了,外部必须重新启动USB连接.
u8 USBH_Check_EnumeDead(USBH_HOST *phost)
{
	static u16 errcnt=0;
	//这个状态,如果持续存在,则说明USB死机了.
	if(phost->gState==HOST_CTRL_XFER&&(phost->EnumState==ENUM_IDLE||phost->EnumState==ENUM_GET_FULL_DEV_DESC))
	{
		errcnt++;
		if(errcnt>2000)//死机了
		{ 
			errcnt=0;
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS 复位
			delay_ms(5);
			RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//复位结束  
			return 1;
		} 
	}else errcnt=0;
	return 0;
} 
//USB HID通信死机检测,防止USB通信死机(暂时仅针对:DTERR,即Data toggle error)
//pcore:USB_OTG_Core_dev_HANDLE结构体指针
//phidm:HID_Machine_TypeDef结构体指针 
//返回值:0,没有死机
//       1,死机了,外部必须重新启动USB连接.
u8 USBH_Check_HIDCommDead(USB_OTG_CORE_HANDLE *pcore,HID_Machine_TypeDef *phidm)
{
 	if(pcore->host.HC_Status[phidm->hc_num_in]==HC_DATATGLERR)//检测到DTERR错误,直接重启USB.
	{  
		return 1;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//USB键盘鼠标数据处理

//鼠标初始化
void USR_MOUSE_Init	(void)
{
 	USBH_Msg_Show(2);		//USB 鼠标
	USB_FIRST_PLUGIN_FLAG=1;//标记第一次插入
}















//键盘初始化
void  USR_KEYBRD_Init(void)
{ 
// 	USBH_Msg_Show(1);		//USB 键盘
	USB_FIRST_PLUGIN_FLAG=1;//标记第一次插入
}


/************************************** 用于用户函数查询，开放为外部变量 *************************************/
//零时数组,用于存放鼠标坐标/键盘输入内容(4.3屏,最大可以输入2016字节)
#define KEYBOARD_BUF_MAX_LEN 2017
__align(4) u8 tbuf[KEYBOARD_BUF_MAX_LEN]; 
u8* keyboard_buf = tbuf; // 给tbuf取个别名，供外部查看方便
u16 keyboard_buf_pos = 0;
u8 LF_FLAG = 0;
/********************************************************************************************/

//USB鼠标数据处理
//data:USB鼠标数据结构体指针
void USR_MOUSE_ProcessData(HID_MOUSE_Data_TypeDef *data)
{  
	static signed short x,y,z; 
	if(USB_FIRST_PLUGIN_FLAG)//第一次插入,将数据清零
	{
		USB_FIRST_PLUGIN_FLAG=0;
		x=y=z=0;
	}
	x+=(signed char)data->x;
	if(x>9999)x=9999;
	if(x<-9999)x=-9999;
	y+=(signed char)data->y;
	if(y>9999)y=9999;
	if(y<-9999)y=-9999;
	z+=(signed char)data->z;
	if(z>9999)z=9999;
	if(z<-9999)z=-9999;
	POINT_COLOR=BLUE;
	sprintf((char*)tbuf,"BUTTON:");
	if(data->button&0X01)strcat((char*)tbuf,"LEFT");
	if((data->button&0X03)==0X02)strcat((char*)tbuf,"RIGHT");
	else if((data->button&0X03)==0X03)strcat((char*)tbuf,"+RIGHT");
	if((data->button&0X07)==0X04)strcat((char*)tbuf,"MID");
	else if((data->button&0X07)>0X04)strcat((char*)tbuf,"+MID");  
 	LCD_Fill(30+56,180,lcddev.width,180+16,WHITE);	
	LCD_ShowString(30,180,210,16,16,(char*)tbuf);	
	sprintf((char*)tbuf,"X POS:%05d",x); 
	LCD_ShowString(30,200,200,16,16,(char*)tbuf);	
	sprintf((char*)tbuf,"Y POS:%05d",y);
	LCD_ShowString(30,220,200,16,16,(char*)tbuf);	
	sprintf((char*)tbuf,"Z POS:%05d",z);
	LCD_ShowString(30,240,200,16,16,(char*)tbuf);	 
	//printf("btn,X,Y,Z:0x%x,%d,%d,%d\r\n",data->button,(signed char)data->x,(signed char)data->y,(signed char)data->z);   
} 


//USB键盘数据处理
//data:USB键盘输入内容
void  USR_KEYBRD_ProcessData (uint8_t data)
{ 
	static u16 endx,endy;
	
	if(USB_FIRST_PLUGIN_FLAG)//第一次插入,将数据清零
	{
		USB_FIRST_PLUGIN_FLAG=0;
		endx=240;		//得到endx值
		endy=16;	//得到endy值
		keyboard_buf_pos = 0;
	} 
	
	if(data>=' ' && data<='~' && keyboard_buf_pos < KEYBOARD_BUF_MAX_LEN)
	{
		tbuf[keyboard_buf_pos++]=data;
		tbuf[keyboard_buf_pos]=0;		//添加结束符. 
		LF_FLAG = 0;
	}else if(data==0X0D)	//退格键
	{
		if(keyboard_buf_pos)keyboard_buf_pos--;
		tbuf[keyboard_buf_pos]=0;		//添加结束符. 
		LF_FLAG = 0;
	} else if (data == 0X0A)
	{
		LF_FLAG = 1;
	}
	
	if(keyboard_buf_pos<=KEYBOARD_BUF_MAX_LEN)	//没有超过显示区
	{
		if(data>=' ' && data<='~' && keyboard_buf_pos < KEYBOARD_BUF_MAX_LEN)
		{
			LCD_Fill(0, 0,endx,endy,WHITE);		
			LCD_ShowString(0,0,endx,endy,16,(char*)tbuf);
		}else if(data==0X0D)	//退格键
		{
			LCD_Fill(0, 0,endx,endy,WHITE);		
			LCD_ShowString(0,0,endx,endy,16,(char*)tbuf);
		} else if (data == 0X0A)
		{
			LCD_Fill(0, 0,endx,endy,WHITE);	
		}
	}		
	//printf("KEY Board Value:%02X\r\n",data);
	//printf("KEY Board Char:%c\r\n",data); 
}













