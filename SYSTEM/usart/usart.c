#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif
/////////////////////////////////////////////////////////////////


/*************************************************************** uart1:TX-PA9��RX-PA10 ************************************************************************/
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA;       //����״̬���	


int uart1_buf_status(void)
{
	if (USART1_RX_STA & 0x8000) return USART1_RX_STA & 0x3fff;
	else return 0;
}

void uart1_read_buf(u8* buf, int len)
{
	int i = 0;
	for (i = 0; i< len; i++)
	{
		buf[i] = USART1_RX_BUF[i];
	}
	USART1_RX_STA=0;
}

void uart1_clear_buf(void)
{
	USART1_RX_STA=0;
}

void uart1_send_bytes(const u8* bytes, int len)
{
	int i = 0;
	for(i = 0; i < len; i++)
	{
		USART_SendData(USART1, bytes[i]);         //�򴮿�1��������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

//��ʼ��IO ����1 
//bound:������
void uart1_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	UART1_GPIO_RCC_FUN(UART1_GPIO_RCC_ADDR,ENABLE);
	UART1_SELF_RCC_FUN(UART1_SELF_RCC_ADDR,ENABLE);

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(UART1_GPIOx, UART1_PinSource_TX, GPIO_AF_USART1); 
	GPIO_PinAFConfig(UART1_GPIOx, UART1_PinSource_RX, GPIO_AF_USART1);

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = UART1_Pin_TX | UART1_Pin_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART1_GPIOx, &GPIO_InitStructure);

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE); 

	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	#if EN_USART1_RX	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);

	#endif
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
	#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART1);//��ȡ���յ�������(1bit)
		
		if((USART1_RX_STA&0x8000)==0)//����δ���
		{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
				{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART1_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	} 
	#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
	#endif
} 
/*************************************************************************************************************************************************************/


/*************************************************************** uart2 ***************************************************************************************/
#if EN_USART2_RX   //���ʹ���˽���

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���	

int uart2_buf_status(void)
{
	if (USART2_RX_STA & 0x8000) return USART2_RX_STA & 0x3fff;
	else return 0;
}

void uart2_read_buf(u8* buf, int len)
{
	int i = 0;
	for (i = 0; i< len; i++)
	{
		buf[i] = USART2_RX_BUF[i];
	}
	USART2_RX_STA=0;
}

void uart2_clear_buf(void)
{
	USART2_RX_STA=0;
}

void uart2_send_bytes(const u8* bytes, int len)
{
	int i = 0;
	for(i = 0; i < len; i++)
	{
		USART_SendData(USART2, bytes[i]);         //�򴮿�2��������
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

//��ʼ��IO ����2
//bound:������
void uart2_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	UART2_GPIO_RCC_FUN(UART2_GPIO_RCC_ADDR, ENABLE); 
	UART2_SELF_RCC_FUN(UART2_SELF_RCC_ADDR, ENABLE);//ʹ��USART2ʱ��

	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(UART2_GPIOx, UART2_PinSource_TX, GPIO_AF_USART2);
	GPIO_PinAFConfig(UART2_GPIOx, UART2_PinSource_RX, GPIO_AF_USART2);

	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = UART2_Pin_TX | UART2_Pin_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART2_GPIOx, &GPIO_InitStructure);

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);

	USART_Cmd(USART2, ENABLE); 

	//USART_ClearFlag(USART2, USART_FLAG_TC);
	
	#if EN_USART2_RX	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;  //   ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//       ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

	#endif
}

void USART2_IRQHandler(void)                	//�жϷ������
{
	u8 Res;
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
	#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART2);//��ȡ���յ�������(1bit)
		
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	} 
	#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
	#endif
} 
#endif	
/*************************************************************************************************************************************************************/




/*************************************************************** uart3 ***************************************************************************************/
#if EN_USART3_RX   //���ʹ���˽���

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART3_RX_STA=0;       //����״̬���	

int uart3_buf_status(void)
{
	if (USART3_RX_STA & 0x8000) return USART3_RX_STA & 0x3fff;
	else return 0;
}

void uart3_read_buf(u8* buf, int len)
{
	int i = 0;
	for (i = 0; i< len; i++)
	{
		buf[i] = USART3_RX_BUF[i];
	}
	USART3_RX_STA=0;
}

void uart3_clear_buf(void)
{
	USART3_RX_STA=0;
}

void uart3_send_bytes(const u8* bytes, int len)
{
	int i = 0;
	for(i = 0; i < len; i++)
	{
		USART_SendData(USART3, bytes[i]);         //����1bit����
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

//��ʼ��
//bound:������
void uart3_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	UART3_GPIO_RCC_FUN(UART3_GPIO_RCC_ADDR, ENABLE); 
	UART3_SELF_RCC_FUN(UART3_SELF_RCC_ADDR, ENABLE);//ʹ��USART2ʱ��

	//���Ÿ���ӳ��
	GPIO_PinAFConfig(UART3_GPIOx, UART3_PinSource_TX, GPIO_AF_USART3);
	GPIO_PinAFConfig(UART3_GPIOx, UART3_PinSource_RX, GPIO_AF_USART3);

	//�˿�����
	GPIO_InitStructure.GPIO_Pin = UART3_Pin_TX | UART3_Pin_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART3_GPIOx, &GPIO_InitStructure);

	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); 

	USART_Cmd(USART3, ENABLE); 
	
	#if EN_USART3_RX	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�

	//NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;  //   ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//       ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

	#endif
}

void USART3_IRQHandler(void)                	//�жϷ������
{
	u8 Res;
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
	#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART3);//��ȡ���յ�������(1bit)
		
		if((USART3_RX_STA&0x8000)==0)//����δ���
		{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
				else USART3_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
				{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART3_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	} 
	#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
	#endif
} 
#endif	
/*************************************************************************************************************************************************************/


/*************************************************************** uart4 ***************************************************************************************/
#if EN_USART4_RX   //���ʹ���˽���

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART4_RX_BUF[USART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART4_RX_STA=0;       //����״̬���	

int uart4_buf_status(void)
{
	if (USART4_RX_STA & 0x8000) return USART4_RX_STA & 0x3fff;
	else return 0;
}

void uart4_read_buf(u8* buf, int len)
{
	int i = 0;
	for (i = 0; i< len; i++)
	{
		buf[i] = USART4_RX_BUF[i];
	}
	USART4_RX_STA=0;
}

void uart4_clear_buf(void)
{
	USART4_RX_STA=0;
}

void uart4_send_bytes(const u8* bytes, int len)
{
	int i = 0;
	for(i = 0; i < len; i++)
	{
//		while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)!=SET);//�ȴ����ͽ���
		USART_SendData(UART4, bytes[i]);         //����1bit����
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

//��ʼ��
//bound:������
void uart4_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	UART4_GPIO_RCC_FUN(UART4_GPIO_RCC_ADDR, ENABLE); 
	UART4_SELF_RCC_FUN(UART4_SELF_RCC_ADDR, ENABLE);//ʹ��USART2ʱ��

	//���Ÿ���ӳ��
	GPIO_PinAFConfig(UART4_GPIOx, UART4_PinSource_TX, GPIO_AF_UART4);
	GPIO_PinAFConfig(UART4_GPIOx, UART4_PinSource_RX, GPIO_AF_UART4);

	//�˿�����
	GPIO_InitStructure.GPIO_Pin = UART4_Pin_TX | UART4_Pin_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART4_GPIOx, &GPIO_InitStructure);

	//USART5 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(UART4, &USART_InitStructure); 
	USART_Cmd(UART4, ENABLE); 
	
	#if EN_USART4_RX	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�

	//NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;  //   ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//       ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	#endif
}

void UART4_IRQHandler(void)                	//�жϷ������
{
	u8 Res;
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
	#endif
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(UART4);//��ȡ���յ�������(1bit)
		
		if((USART4_RX_STA&0x8000)==0)//����δ���
		{
			if(USART4_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART4_RX_STA=0;//���մ���,���¿�ʼ
				else USART4_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART4_RX_STA|=0x4000;
				else
				{
					USART4_RX_BUF[USART4_RX_STA&0X3FFF]=Res ;
					USART4_RX_STA++;
					if(USART4_RX_STA>(USART4_REC_LEN-1))USART4_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	} 
	#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
	#endif
} 
#endif	
/*************************************************************************************************************************************************************/


/*************************************************************** uart5 ***************************************************************************************/
#if EN_USART5_RX   //���ʹ���˽���

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART5_RX_BUF[USART5_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART5_RX_STA=0;       //����״̬���	

int uart5_buf_status(void)
{
	if (USART5_RX_STA & 0x8000) return USART5_RX_STA & 0x3fff;
	else return 0;
}

void uart5_read_buf(u8* buf, int len)
{
	int i = 0;
	for (i = 0; i< len; i++)
	{
		buf[i] = USART5_RX_BUF[i];
	}
	USART5_RX_STA=0;
}

void uart5_clear_buf(void)
{
	USART5_RX_STA=0;
}

void uart5_send_bytes(const u8* bytes, int len)
{
	int i = 0;
	for(i = 0; i < len; i++)
	{
//		while(USART_GetFlagStatus(UART5,USART_FLAG_TXE)!=SET);//�ȴ����ͽ���
		USART_SendData(UART5, bytes[i]);         //����1bit����
		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

//��ʼ��
//bound:������
void uart5_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	UART5_GPIO_RCC_FUN_TX(UART5_GPIO_RCC_ADDR_TX, ENABLE); 
	UART5_GPIO_RCC_FUN_RX(UART5_GPIO_RCC_ADDR_RX, ENABLE); 
	UART5_SELF_RCC_FUN(UART5_SELF_RCC_ADDR, ENABLE);//ʹ��USART2ʱ��

	//���Ÿ���ӳ��
	GPIO_PinAFConfig(UART5_GPIOx_TX, UART5_PinSource_TX, GPIO_AF_UART5);
	GPIO_PinAFConfig(UART5_GPIOx_RX, UART5_PinSource_RX, GPIO_AF_UART5);

	//�˿�����
	GPIO_InitStructure.GPIO_Pin = UART5_Pin_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART5_GPIOx_TX, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = UART5_Pin_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART5_GPIOx_RX, &GPIO_InitStructure);

	//USART5 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(UART5, &USART_InitStructure); 
	USART_Cmd(UART5, ENABLE); 
	
	#if EN_USART5_RX	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�

	//NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;  //   ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//       ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	#endif
}

void UART5_IRQHandler(void)                	//�жϷ������
{
	u8 Res;
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
	#endif
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(UART5);//��ȡ���յ�������(1bit)
		
		if((USART5_RX_STA&0x8000)==0)//����δ���
		{
			if(USART5_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART5_RX_STA=0;//���մ���,���¿�ʼ
				else USART5_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART5_RX_STA|=0x4000;
				else
				{
					USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res ;
					USART5_RX_STA++;
					if(USART5_RX_STA>(USART5_REC_LEN-1))USART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	} 
	#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
	#endif
} 
#endif	
/*************************************************************************************************************************************************************/


/*************************************************************** uart6 ***************************************************************************************/
#if EN_USART6_RX   //���ʹ���˽���

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART6_RX_BUF[USART6_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART6_RX_STA=0;       //����״̬���	

int uart6_buf_status(void)
{
	if (USART6_RX_STA & 0x8000) return USART6_RX_STA & 0x3fff;
	else return 0;
}

void uart6_read_buf(u8* buf, int len)
{
	int i = 0;
	for (i = 0; i< len; i++)
	{
		buf[i] = USART6_RX_BUF[i];
	}
	USART6_RX_STA=0;
}

void uart6_clear_buf(void)
{
	USART6_RX_STA=0;
}

void uart6_send_bytes(const u8* bytes, int len)
{
	int i = 0;
	for(i = 0; i < len; i++)
	{
		USART_SendData(USART6, bytes[i]);         //����1bit����
		while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
	}
}

//��ʼ��
//bound:������
void uart6_init(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	UART6_GPIO_RCC_FUN(UART6_GPIO_RCC_ADDR, ENABLE); 
	UART6_SELF_RCC_FUN(UART6_SELF_RCC_ADDR, ENABLE);//ʹ��USART2ʱ��

	//���Ÿ���ӳ��
	GPIO_PinAFConfig(UART6_GPIOx, UART6_PinSource_TX, GPIO_AF_USART6);
	GPIO_PinAFConfig(UART6_GPIOx, UART6_PinSource_RX, GPIO_AF_USART6);

	//�˿�����
	GPIO_InitStructure.GPIO_Pin = UART6_Pin_TX | UART6_Pin_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(UART6_GPIOx, &GPIO_InitStructure);

	//USART6 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); 

	USART_Cmd(USART6, ENABLE); 
	
	#if EN_USART6_RX	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

	//NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;  //   ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//       ******��Ҫ����
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

	#endif
}

void USART6_IRQHandler(void)                	//�жϷ������
{
	u8 Res;
	#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
	#endif
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res = USART_ReceiveData(USART6);//��ȡ���յ�������(1bit)
		
		if((USART6_RX_STA&0x8000)==0)//����δ���
		{
			if(USART6_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART6_RX_STA=0;//���մ���,���¿�ʼ
				else USART6_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART6_RX_STA|=0x4000;
				else
				{
					USART6_RX_BUF[USART6_RX_STA&0X3FFF]=Res ;
					USART6_RX_STA++;
					if(USART6_RX_STA>(USART6_REC_LEN-1))USART6_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	} 
	#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
	#endif
} 
#endif	
/*************************************************************************************************************************************************************/
