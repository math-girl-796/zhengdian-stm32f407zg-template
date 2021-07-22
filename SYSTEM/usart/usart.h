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

//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
extern u8 USART1_RX_BUF[USART1_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
extern u16 USART1_RX_STA;       //����״̬���	

#define UART1_GPIOx			GPIOA
#define UART1_Pin_TX		GPIO_Pin_9
#define UART1_Pin_RX		GPIO_Pin_10
#define UART1_PinSource_TX	GPIO_PinSource9
#define UART1_PinSource_RX	GPIO_PinSource10

#define UART1_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART1_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOA
#define UART1_SELF_RCC_FUN	RCC_APB2PeriphClockCmd
#define UART1_SELF_RCC_ADDR	RCC_APB2Periph_USART1

void uart1_init(u32 bound);							// ��ʼ������
void uart1_send_bytes(const u8* bytes, int len);	// bytes: �ֽ����֣�len���ֽ����鳤��
int uart1_buf_status(void);							// �����ջ��������Ƿ������ݣ�����У��������ݳ��ȣ����û�У�����0
void uart1_read_buf(u8* buf, int len);				// �����ڽ��ռĴ����ڵ�����ǰlen���ֽڶ��뵽buf�У�Ȼ����մ��ڽ��ջ�����
void uart1_clear_buf(void);							// ��մ��ڽ��ջ�����
/*************************************************************************************************************************************************************/




/*************************************************************** uart2 ***************************************************************************************/
// ��ѡ�	TX-PA2,PD5
//			RX-PA3,PD6

/* ˵�������ݽ��շ���ͬuart1 */

/* �Ľ����ԣ����������յ�0D 0Aʱ��������һ���Զ����жϣ����жϸ���һ��ȫ�ֱ�����ά�����´��������� */
#define USART2_REC_LEN  		200  	//�����������ֽ��������һ���յ����������������������������������
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define UART2_GPIOx			GPIOD					// �޸Ĺܽ�ֻ���޸����GPIOx��x���Լ��ܽ�����
#define UART2_Pin_TX		GPIO_Pin_5
#define UART2_Pin_RX		GPIO_Pin_6
#define UART2_PinSource_TX	GPIO_PinSource5
#define UART2_PinSource_RX	GPIO_PinSource6

#define UART2_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART2_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOD
#define UART2_SELF_RCC_FUN	RCC_APB1PeriphClockCmd
#define UART2_SELF_RCC_ADDR	RCC_APB1Periph_USART2

void uart2_init(u32 bound);							// ��ʼ������
void uart2_send_bytes(const u8* bytes, int len);	// bytes: �ֽ����֣�len���ֽ����鳤��
int uart2_buf_status(void);							// �����ջ��������Ƿ������ݣ�����У��������ݳ��ȣ����û�У�����0
void uart2_read_buf(u8* buf, int len);				// �����ڽ��ռĴ����ڵ�����ǰlen���ֽڶ��뵽buf�У�Ȼ����մ��ڽ��ջ�����
void uart2_clear_buf(void);							// ��մ��ڽ��ջ�����
/*************************************************************************************************************************************************************/






/*************************************************************** uart3 ***************************************************************************************/
// ��ѡ�	TX-PB10,PC10,PD8
//			RX-PB11,PC11,PD9

/* ˵�������ݽ��շ���ͬuart1 */

/* �Ľ����ԣ����������յ�0D 0Aʱ��������һ���Զ����жϣ����жϸ���һ��ȫ�ֱ�����ά�����´��������� */
#define USART3_REC_LEN  		200  	//�����������ֽ��������һ���յ����������������������������������
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define UART3_GPIOx			GPIOD					// �޸Ĺܽ�ֻ���޸����GPIOx��x���Լ��ܽ�����
#define UART3_Pin_TX		GPIO_Pin_8
#define UART3_Pin_RX		GPIO_Pin_9
#define UART3_PinSource_TX	GPIO_PinSource8
#define UART3_PinSource_RX	GPIO_PinSource9

#define UART3_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART3_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOD
#define UART3_SELF_RCC_FUN	RCC_APB1PeriphClockCmd
#define UART3_SELF_RCC_ADDR	RCC_APB1Periph_USART3

void uart3_init(u32 bound);							// ��ʼ������
void uart3_send_bytes(const u8* bytes, int len);	// bytes: �ֽ����֣�len���ֽ����鳤��
int uart3_buf_status(void);							// �����ջ��������Ƿ������ݣ�����У��������ݳ��ȣ����û�У�����0
void uart3_read_buf(u8* buf, int len);				// �����ڽ��ռĴ����ڵ�����ǰlen���ֽڶ��뵽buf�У�Ȼ����մ��ڽ��ջ�����
void uart3_clear_buf(void);							// ��մ��ڽ��ջ�����
/*************************************************************************************************************************************************************/



/*************************************************************** uart4 ***************************************************************************************/
// ��ѡ�	TX-PA0,PC10
//			RX-PA1,PC11

/* ˵�������ݽ��շ���ͬuart1 */

/* �Ľ����ԣ����������յ�0D 0Aʱ��������һ���Զ����жϣ����жϸ���һ��ȫ�ֱ�����ά�����´��������� */
#define USART4_REC_LEN  		200  	//�����������ֽ��������һ���յ����������������������������������
#define EN_USART4_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define UART4_GPIOx			GPIOC					// �޸Ĺܽ�ֻ���޸����GPIOx��x���Լ��ܽ�����
#define UART4_Pin_TX		GPIO_Pin_10
#define UART4_Pin_RX		GPIO_Pin_11
#define UART4_PinSource_TX	GPIO_PinSource10
#define UART4_PinSource_RX	GPIO_PinSource11

#define UART4_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART4_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOC
#define UART4_SELF_RCC_FUN	RCC_APB1PeriphClockCmd
#define UART4_SELF_RCC_ADDR	RCC_APB1Periph_UART4

void uart4_init(u32 bound);							// ��ʼ������
void uart4_send_bytes(const u8* bytes, int len);	// bytes: �ֽ����֣�len���ֽ����鳤��
int uart4_buf_status(void);							// �����ջ��������Ƿ������ݣ�����У��������ݳ��ȣ����û�У�����0
void uart4_read_buf(u8* buf, int len);				// �����ڽ��ռĴ����ڵ�����ǰlen���ֽڶ��뵽buf�У�Ȼ����մ��ڽ��ջ�����
void uart4_clear_buf(void);							// ��մ��ڽ��ջ�����
/*************************************************************************************************************************************************************/



/*************************************************************** uart5 ***************************************************************************************/
// ��ѡ�	TX-PC12
//			RX-PD2

/* ˵�������ݽ��շ���ͬuart1 */

/* �Ľ����ԣ����������յ�0D 0Aʱ��������һ���Զ����жϣ����жϸ���һ��ȫ�ֱ�����ά�����´��������� */
#define USART5_REC_LEN  		200  	//�����������ֽ��������һ���յ����������������������������������
#define EN_USART5_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define UART5_GPIOx_TX			GPIOC					// �޸Ĺܽ�ֻ���޸����GPIOx��x���Լ��ܽ�����
#define UART5_GPIOx_RX			GPIOD					// �޸Ĺܽ�ֻ���޸����GPIOx��x���Լ��ܽ�����
#define UART5_Pin_TX		GPIO_Pin_12
#define UART5_Pin_RX		GPIO_Pin_2
#define UART5_PinSource_TX	GPIO_PinSource12
#define UART5_PinSource_RX	GPIO_PinSource2

#define UART5_GPIO_RCC_FUN_TX	RCC_AHB1PeriphClockCmd
#define UART5_GPIO_RCC_ADDR_TX	RCC_AHB1Periph_GPIOC
#define UART5_GPIO_RCC_FUN_RX	RCC_AHB1PeriphClockCmd
#define UART5_GPIO_RCC_ADDR_RX	RCC_AHB1Periph_GPIOD
#define UART5_SELF_RCC_FUN	RCC_APB1PeriphClockCmd
#define UART5_SELF_RCC_ADDR	RCC_APB1Periph_UART5

void uart5_init(u32 bound);							// ��ʼ������
void uart5_send_bytes(const u8* bytes, int len);	// bytes: �ֽ����֣�len���ֽ����鳤��
int uart5_buf_status(void);							// �����ջ��������Ƿ������ݣ�����У��������ݳ��ȣ����û�У�����0
void uart5_read_buf(u8* buf, int len);				// �����ڽ��ռĴ����ڵ�����ǰlen���ֽڶ��뵽buf�У�Ȼ����մ��ڽ��ջ�����
void uart5_clear_buf(void);							// ��մ��ڽ��ջ�����
/*************************************************************************************************************************************************************/



/*************************************************************** uart6 ***************************************************************************************/
// ��ѡ�	TX-PC6
//			RX-PC7

/* ˵�������ݽ��շ���ͬuart1 */

/* �Ľ����ԣ����������յ�0D 0Aʱ��������һ���Զ����жϣ����жϸ���һ��ȫ�ֱ�����ά�����´��������� */
#define USART6_REC_LEN  		200  	//�����������ֽ��������һ���յ����������������������������������
#define EN_USART6_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define UART6_GPIOx			GPIOC					// �޸Ĺܽ�ֻ���޸����GPIOx��x���Լ��ܽ�����
#define UART6_Pin_TX		GPIO_Pin_6
#define UART6_Pin_RX		GPIO_Pin_7
#define UART6_PinSource_TX	GPIO_PinSource6
#define UART6_PinSource_RX	GPIO_PinSource7

#define UART6_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART6_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOC
#define UART6_SELF_RCC_FUN	RCC_APB2PeriphClockCmd
#define UART6_SELF_RCC_ADDR	RCC_APB2Periph_USART6

void uart6_init(u32 bound);							// ��ʼ������
void uart6_send_bytes(const u8* bytes, int len);	// bytes: �ֽ����֣�len���ֽ����鳤��
int uart6_buf_status(void);							// �����ջ��������Ƿ������ݣ�����У��������ݳ��ȣ����û�У�����0
void uart6_read_buf(u8* buf, int len);				// �����ڽ��ռĴ����ڵ�����ǰlen���ֽڶ��뵽buf�У�Ȼ����մ��ڽ��ջ�����
void uart6_clear_buf(void);							// ��մ��ڽ��ջ�����
/*************************************************************************************************************************************************************/


#endif


