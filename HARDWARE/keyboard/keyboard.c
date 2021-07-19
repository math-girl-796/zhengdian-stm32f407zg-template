#include "keyboard.h"

extern u8* keyboard_buf; // �������뻺������������\x00��β(�������\x00�����ڻ�����)
extern u16 keyboard_buf_pos; // ���̻�����Ŀǰ���ȣ�keyboard_buf[keyboard_buf_pos]����\x00
extern HID_Machine_TypeDef HID_Machine;	
extern u8 LF_FLAG;

USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev;

void USBH_HID_Reconnect(void)
{
	//�ر�֮ǰ������
	USBH_DeInit(&USB_OTG_Core_dev,&USB_Host);	//��λUSB HOST
	USB_OTG_StopHost(&USB_OTG_Core_dev);		//ֹͣUSBhost
	if(USB_Host.usr_cb->DeviceDisconnected)		//����,�Ž�ֹ
	{
		USB_Host.usr_cb->DeviceDisconnected(); 	//�ر�USB����
		USBH_DeInit(&USB_OTG_Core_dev, &USB_Host);
		USB_Host.usr_cb->DeInit();
		USB_Host.class_cb->DeInit(&USB_OTG_Core_dev,&USB_Host.device_prop);
	}
	USB_OTG_DisableGlobalInt(&USB_OTG_Core_dev);//�ر������ж�
	//���¸�λUSB
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,ENABLE);//USB OTG FS ��λ
	delay_ms(5);
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS,DISABLE);	//��λ����  

	memset(&USB_OTG_Core_dev,0,sizeof(USB_OTG_CORE_HANDLE));
	memset(&USB_Host,0,sizeof(USB_Host));
	//��������USB HID�豸
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks);  
}

void keyboard_init(void) 					// ��ʼ������
{
	USBH_Init(&USB_OTG_Core_dev,USB_OTG_FS_CORE_ID,&USB_Host,&HID_cb,&USR_Callbacks); 
	USBH_Process(&USB_OTG_Core_dev, &USB_Host);
//	while(1)
//	{
//		USBH_Process(&USB_OTG_Core_dev, &USB_Host);
//		if(bDeviceState==1)//���ӽ�����
//		{ 
//			if(USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))//���USB HIDͨ��,�Ƿ�����? 
//			{ 	    
//				USBH_HID_Reconnect();//����
//			}				
//			
//		}else	//����δ������ʱ��,���
//		{
//			if(USBH_Check_EnumeDead(&USB_Host))	//���USB HOST ö���Ƿ�������?������,�����³�ʼ�� 
//			{ 	    
//				USBH_HID_Reconnect();//����
//			}			
//		}
//	}
}


void keyboard_check(void) 
{
	USBH_Process(&USB_OTG_Core_dev, &USB_Host);
	/* ���ӽ�����ͨ���쳣 */
	if (bDeviceState==1 && USBH_Check_HIDCommDead(&USB_OTG_Core_dev,&HID_Machine))
	{
		led_switch(LED0);
		delay_ms(20);
		USBH_HID_Reconnect();//����
		keyboard_check();
	}		
	/* ����δ������USB HOSTö������ */
	else if (bDeviceState==0 && USBH_Check_EnumeDead(&USB_Host)) 
	{
		led_switch(LED0);
		delay_ms(20);
		USBH_HID_Reconnect();//����
		keyboard_check();
	}
	else
	{
		return;
	}
}

int keyboard_buf_state(void) 				// ���ػ��������ַ���������Χ��0~KEYBOARD_BUF_MAX_LEN֮��
{
	return keyboard_buf_pos;
}

u8 keyboard_LF(void) 				// ���ػ����������һ���ַ������������ַ������пɼ��ַ���\n��ɡ����������Ϊ�գ�����0
{
	return LF_FLAG;
}

void keyboard_read_buf(u8* buf, int len) 	// �����̻�������ǰlen���ַ�����buf��Ȼ����ռ��̻�����
{
	int i;
	for (i = 0; i < len; i++)
	{
		buf[i] = keyboard_buf[i];
	}
	keyboard_buf_pos = 0;
	LF_FLAG = 0;
}
void keyboard_clear_buf(void)				// ��ռ��̻�����
{
	keyboard_buf_pos = 0;
	LF_FLAG = 0;
}

