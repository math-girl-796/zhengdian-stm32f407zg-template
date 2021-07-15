#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

/*************************************************************** uart1:TX-PA9��RX-PA10 ************************************************************************/

/* ˵�����ӻ����򱾻���ͣ�ط������ݣ�ÿ����һ���ֽڣ��������ж�һ�Σ���������ֽڻ��۵������У�ֱ���������յ������� 0D 0A Ϊֹ */
/* �������յ��� 0D 0A����������ֹͣ��������ݵĽ���(���Ի��жϣ��жϺ󲻶��½��յ������ݽ��д���ֱ�Ӷ���)��ֱ��ʹ��uart1_read_buf��uart1_clear_buf��ջ�����Ϊֹ */
/* ��������ʵʱ��ôӻ����ݣ���Ҫ���ϵ���uart1_buf_status�鿴�Ƿ��������ݴ�����һ���������辡����ߡ� */

/* �Ľ����ԣ����������յ�0D 0Aʱ��������һ���Զ����жϣ����жϸ���һ��ȫ�ֱ�����ά�����´��������� */
#define USART1_REC_LEN  		200  	//�����������ֽ��������һ���յ����������������������������������
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define UART1_GPIOx			GPIOA
#define UART1_Pin_TX		GPIO_Pin_9
#define UART1_Pin_RX		GPIO_Pin_10
#define UART1_PinSource_TX	GPIO_PinSource9
#define UART1_PinSource_RX	GPIO_PinSource10
#define UART1_GPIO_CLOCK	RCC_AHB1Periph_GPIOA

void uart1_init(u32 bound);							// ��ʼ��uart1����
void uart1_send_bytes(const u8* bytes, int len);	// bytes: �ֽ����֣�len���ֽ����鳤��
int uart1_buf_status(void);							// �����ջ��������Ƿ������ݣ�����У��������ݳ��ȣ����û�У�����0
void uart1_read_buf(u8* buf, int len);				// �����ڽ��ռĴ����ڵ�����ǰlen���ֽڶ��뵽buf�У�Ȼ����մ��ڽ��ջ�����
void uart1_clear_buf(void);							// ��մ��ڽ��ջ�����

/*************************************************************************************************************************************************************/




/*************************************************************** uart2:TX-PD5��RX-PD6 ************************************************************************/

/* ˵�������ݽ��շ���ͬuart1 */

/* �Ľ����ԣ����������յ�0D 0Aʱ��������һ���Զ����жϣ����жϸ���һ��ȫ�ֱ�����ά�����´��������� */
#define USART2_REC_LEN  		200  	//�����������ֽ��������һ���յ����������������������������������
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define UART2_GPIOx			GPIOD
#define UART2_Pin_TX		GPIO_Pin_5
#define UART2_Pin_RX		GPIO_Pin_6
#define UART2_PinSource_TX	GPIO_PinSource5
#define UART2_PinSource_RX	GPIO_PinSource6
#define UART2_GPIO_CLOCK	RCC_AHB1Periph_GPIOD

void uart2_init(u32 bound);							// ��ʼ��uart1����
void uart2_send_bytes(const u8* bytes, int len);	// bytes: �ֽ����֣�len���ֽ����鳤��
int uart2_buf_status(void);							// �����ջ��������Ƿ������ݣ�����У��������ݳ��ȣ����û�У�����0
void uart2_read_buf(u8* buf, int len);				// �����ڽ��ռĴ����ڵ�����ǰlen���ֽڶ��뵽buf�У�Ȼ����մ��ڽ��ջ�����
void uart2_clear_buf(void);							// ��մ��ڽ��ջ�����

/*************************************************************************************************************************************************************/


#endif


