#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

#define IIC_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define IIC_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOA

#define IIC_SCL_GPIO		GPIOA
#define IIC_SCL_GPIO_PIN	GPIO_Pin_4

#define IIC_SDA_GPIO		GPIOA
#define IIC_SDA_GPIO_PIN	GPIO_Pin_5
   	   		  
			  
//IO��������
#define SDA_IN()  {IIC_SDA_GPIO->MODER&=~(3<<(5*2));IIC_SDA_GPIO->MODER|=0<<5*2;}	//����ģʽ
#define SDA_OUT() {IIC_SDA_GPIO->MODER&=~(3<<(5*2));IIC_SDA_GPIO->MODER|=1<<5*2;} //���ģʽ
//IO��������	 
#define IIC_SCL    PAout(4) //SCL
#define IIC_SDA    PAout(5) //SDA	 
#define READ_SDA   PAin(5)  //����SDA 

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif
















