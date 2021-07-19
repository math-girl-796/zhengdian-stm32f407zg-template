#include "keyboard.h"

extern u8* keyboard_buf; // 键盘输入缓冲区，总是以\x00结尾(但是这个\x00不属于缓冲区)
extern u16 keyboard_buf_pos; // 键盘缓冲区目前长度，keyboard_buf[keyboard_buf_pos]总是\x00
extern HID_Machine_TypeDef HID_Machine;	
extern u8 LF_FLAG;

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;

void USBH_HID_Reconnect(void)
{
	//关闭之前的连接
	USBH_DeInit(&USB_OTG_Core_dev,&USB_Host);	//复位USB HOST
	USB_OTG_StopHost(&USB_OTG_Core_dev);		//停止USBhost
	if(USB_Host.usr_cb->DeviceDisconnected)		//存在,才禁止
	{
		USB_Host.usr_cb->DeviceDisconnected(); 	//关闭USB连接
		USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);
		USB_Host.usr_cb->DeInit();
		USB_Host.class_cb->DeInit(&USB_OTG_Core_dev,&USB_Host.device_prop);
	}
	USB_OTG_DisableGlobalInt(&USB_OTG_Core_dev);//关闭所有中断
	//重新复位USB
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS 复位
	delay_ms(5);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//复位结束  

	memset(&USB_OTG_Core_dev,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));
	//重新连接USB HID设备
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks);  
}

void keyboard_init(void) 					// 初始化键盘
{
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks); 
	USBH_Process(&USB_OTG_Core_dev, &USB_Host);
//	while(1)
//	{
//		USBH_Process(&USB_OTG_Core_dev, &USB_Host);
//		if(bDeviceState==1)//连接建立了
//		{ 
//			if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))//检测USB HID通信,是否还正常? 
//			{ 	    
//				USBH_HID_Reconnect();//重连
//			}				
//			
//		}else	//连接未建立的时候,检测
//		{
//			if(USBH_Check_EnumeDead(&USB_Host))	//检测USB HOST 枚举是否死机了?死机了,则重新初始化 
//			{ 	    
//				USBH_HID_Reconnect();//重连
//			}			
//		}
//	}
}


void keyboard_check(void) 
{
	USBH_Process(&USB_OTG_Core_dev, &USB_Host);
	/* 连接建立，通信异常 */
	if (bDeviceState==1 && USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))
	{
		led_switch(LED0);
		delay_ms(20);
		USBH_HID_Reconnect();//重连
		keyboard_check();
	}		
	/* 连接未建立，USB HOST枚举死机 */
	else if (bDeviceState==0 && USBH_Check_EnumeDead(&USB_Host)) 
	{
		led_switch(LED0);
		delay_ms(20);
		USBH_HID_Reconnect();//重连
		keyboard_check();
	}
	else
	{
		return;
	}
}

int keyboard_buf_state(void) 				// 返回缓冲区中字符个数，范围在0~KEYBOARD_BUF_MAX_LEN之间
{
	return keyboard_buf_pos;
}

u8 keyboard_LF(void) 				// 返回缓冲区中最后一个字符。缓冲区中字符由所有可见字符和\n组成。如果缓冲区为空，返回0
{
	return LF_FLAG;
}

void keyboard_read_buf(u8* buf, int len) 	// 将键盘缓冲区中前len个字符读入buf，然后清空键盘缓冲区
{
	int i;
	for (i = 0; i < len; i++)
	{
		buf[i] = keyboard_buf[i];
	}
	keyboard_buf_pos = 0;
	LF_FLAG = 0;
}
void keyboard_clear_buf(void)				// 清空键盘缓冲区
{
	keyboard_buf_pos = 0;
	LF_FLAG = 0;
}

