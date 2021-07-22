#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

/*************************************************************** uart1:TX-PA9，RX-PA10 ************************************************************************/
/* 说明：从机可向本机不停地发送数据，每发送一个字节，本机会中断一次，并将这个字节积累到缓存中，直到本机接收到连续的 0D 0A 为止 */
/* 若本机收到了 0D 0A，缓冲区会停止对外界数据的接收(但仍会中断，中断后不对新接收到的数据进行处理，直接丢弃)，直到使用uart1_read_buf或uart1_clear_buf清空缓冲区为止 */
/* 本机若想实时获得从机数据，需要不断地用uart1_buf_status查看是否有新数据传来，一旦传来，需尽快读走。 */

/* 改进策略：当本机接收到0D 0A时，触发另一个自定义中断，该中断更新一个全局变量，维护最新传来的数据 */
#define USART1_REC_LEN  		200  	//定义最大接收字节数。如果一次收到的数据总量超过此数，缓冲区将被清空
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
extern u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
extern u16 USART1_RX_STA;       //接收状态标记	

#define UART1_GPIOx			GPIOA
#define UART1_Pin_TX		GPIO_Pin_9
#define UART1_Pin_RX		GPIO_Pin_10
#define UART1_PinSource_TX	GPIO_PinSource9
#define UART1_PinSource_RX	GPIO_PinSource10

#define UART1_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART1_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOA
#define UART1_SELF_RCC_FUN	RCC_APB2PeriphClockCmd
#define UART1_SELF_RCC_ADDR	RCC_APB2Periph_USART1

void uart1_init(u32 bound);							// 初始化串口
void uart1_send_bytes(const u8* bytes, int len);	// bytes: 字节数字；len：字节数组长度
int uart1_buf_status(void);							// 检查接收缓冲区中是否有数据，如果有，返回数据长度，如果没有，返回0
void uart1_read_buf(u8* buf, int len);				// 将串口接收寄存器内的内容前len个字节读入到buf中，然后清空串口接收缓冲区
void uart1_clear_buf(void);							// 清空串口接收缓冲区
/*************************************************************************************************************************************************************/




/*************************************************************** uart2 ***************************************************************************************/
// 可选项：	TX-PA2,PD5
//			RX-PA3,PD6

/* 说明：数据接收方法同uart1 */

/* 改进策略：当本机接收到0D 0A时，触发另一个自定义中断，该中断更新一个全局变量，维护最新传来的数据 */
#define USART2_REC_LEN  		200  	//定义最大接收字节数。如果一次收到的数据总量超过此数，缓冲区将被清空
#define EN_USART2_RX 			1		//使能（1）/禁止（0）串口1接收

#define UART2_GPIOx			GPIOD					// 修改管脚只需修改左侧GPIOx的x，以及管脚数字
#define UART2_Pin_TX		GPIO_Pin_5
#define UART2_Pin_RX		GPIO_Pin_6
#define UART2_PinSource_TX	GPIO_PinSource5
#define UART2_PinSource_RX	GPIO_PinSource6

#define UART2_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART2_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOD
#define UART2_SELF_RCC_FUN	RCC_APB1PeriphClockCmd
#define UART2_SELF_RCC_ADDR	RCC_APB1Periph_USART2

void uart2_init(u32 bound);							// 初始化串口
void uart2_send_bytes(const u8* bytes, int len);	// bytes: 字节数字；len：字节数组长度
int uart2_buf_status(void);							// 检查接收缓冲区中是否有数据，如果有，返回数据长度，如果没有，返回0
void uart2_read_buf(u8* buf, int len);				// 将串口接收寄存器内的内容前len个字节读入到buf中，然后清空串口接收缓冲区
void uart2_clear_buf(void);							// 清空串口接收缓冲区
/*************************************************************************************************************************************************************/






/*************************************************************** uart3 ***************************************************************************************/
// 可选项：	TX-PB10,PC10,PD8
//			RX-PB11,PC11,PD9

/* 说明：数据接收方法同uart1 */

/* 改进策略：当本机接收到0D 0A时，触发另一个自定义中断，该中断更新一个全局变量，维护最新传来的数据 */
#define USART3_REC_LEN  		200  	//定义最大接收字节数。如果一次收到的数据总量超过此数，缓冲区将被清空
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收

#define UART3_GPIOx			GPIOD					// 修改管脚只需修改左侧GPIOx的x，以及管脚数字
#define UART3_Pin_TX		GPIO_Pin_8
#define UART3_Pin_RX		GPIO_Pin_9
#define UART3_PinSource_TX	GPIO_PinSource8
#define UART3_PinSource_RX	GPIO_PinSource9

#define UART3_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART3_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOD
#define UART3_SELF_RCC_FUN	RCC_APB1PeriphClockCmd
#define UART3_SELF_RCC_ADDR	RCC_APB1Periph_USART3

void uart3_init(u32 bound);							// 初始化串口
void uart3_send_bytes(const u8* bytes, int len);	// bytes: 字节数字；len：字节数组长度
int uart3_buf_status(void);							// 检查接收缓冲区中是否有数据，如果有，返回数据长度，如果没有，返回0
void uart3_read_buf(u8* buf, int len);				// 将串口接收寄存器内的内容前len个字节读入到buf中，然后清空串口接收缓冲区
void uart3_clear_buf(void);							// 清空串口接收缓冲区
/*************************************************************************************************************************************************************/



/*************************************************************** uart4 ***************************************************************************************/
// 可选项：	TX-PA0,PC10
//			RX-PA1,PC11

/* 说明：数据接收方法同uart1 */

/* 改进策略：当本机接收到0D 0A时，触发另一个自定义中断，该中断更新一个全局变量，维护最新传来的数据 */
#define USART4_REC_LEN  		200  	//定义最大接收字节数。如果一次收到的数据总量超过此数，缓冲区将被清空
#define EN_USART4_RX 			1		//使能（1）/禁止（0）串口1接收

#define UART4_GPIOx			GPIOC					// 修改管脚只需修改左侧GPIOx的x，以及管脚数字
#define UART4_Pin_TX		GPIO_Pin_10
#define UART4_Pin_RX		GPIO_Pin_11
#define UART4_PinSource_TX	GPIO_PinSource10
#define UART4_PinSource_RX	GPIO_PinSource11

#define UART4_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART4_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOC
#define UART4_SELF_RCC_FUN	RCC_APB1PeriphClockCmd
#define UART4_SELF_RCC_ADDR	RCC_APB1Periph_UART4

void uart4_init(u32 bound);							// 初始化串口
void uart4_send_bytes(const u8* bytes, int len);	// bytes: 字节数字；len：字节数组长度
int uart4_buf_status(void);							// 检查接收缓冲区中是否有数据，如果有，返回数据长度，如果没有，返回0
void uart4_read_buf(u8* buf, int len);				// 将串口接收寄存器内的内容前len个字节读入到buf中，然后清空串口接收缓冲区
void uart4_clear_buf(void);							// 清空串口接收缓冲区
/*************************************************************************************************************************************************************/



/*************************************************************** uart5 ***************************************************************************************/
// 可选项：	TX-PC12
//			RX-PD2

/* 说明：数据接收方法同uart1 */

/* 改进策略：当本机接收到0D 0A时，触发另一个自定义中断，该中断更新一个全局变量，维护最新传来的数据 */
#define USART5_REC_LEN  		200  	//定义最大接收字节数。如果一次收到的数据总量超过此数，缓冲区将被清空
#define EN_USART5_RX 			1		//使能（1）/禁止（0）串口1接收

#define UART5_GPIOx_TX			GPIOC					// 修改管脚只需修改左侧GPIOx的x，以及管脚数字
#define UART5_GPIOx_RX			GPIOD					// 修改管脚只需修改左侧GPIOx的x，以及管脚数字
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

void uart5_init(u32 bound);							// 初始化串口
void uart5_send_bytes(const u8* bytes, int len);	// bytes: 字节数字；len：字节数组长度
int uart5_buf_status(void);							// 检查接收缓冲区中是否有数据，如果有，返回数据长度，如果没有，返回0
void uart5_read_buf(u8* buf, int len);				// 将串口接收寄存器内的内容前len个字节读入到buf中，然后清空串口接收缓冲区
void uart5_clear_buf(void);							// 清空串口接收缓冲区
/*************************************************************************************************************************************************************/



/*************************************************************** uart6 ***************************************************************************************/
// 可选项：	TX-PC6
//			RX-PC7

/* 说明：数据接收方法同uart1 */

/* 改进策略：当本机接收到0D 0A时，触发另一个自定义中断，该中断更新一个全局变量，维护最新传来的数据 */
#define USART6_REC_LEN  		200  	//定义最大接收字节数。如果一次收到的数据总量超过此数，缓冲区将被清空
#define EN_USART6_RX 			1		//使能（1）/禁止（0）串口1接收

#define UART6_GPIOx			GPIOC					// 修改管脚只需修改左侧GPIOx的x，以及管脚数字
#define UART6_Pin_TX		GPIO_Pin_6
#define UART6_Pin_RX		GPIO_Pin_7
#define UART6_PinSource_TX	GPIO_PinSource6
#define UART6_PinSource_RX	GPIO_PinSource7

#define UART6_GPIO_RCC_FUN	RCC_AHB1PeriphClockCmd
#define UART6_GPIO_RCC_ADDR	RCC_AHB1Periph_GPIOC
#define UART6_SELF_RCC_FUN	RCC_APB2PeriphClockCmd
#define UART6_SELF_RCC_ADDR	RCC_APB2Periph_USART6

void uart6_init(u32 bound);							// 初始化串口
void uart6_send_bytes(const u8* bytes, int len);	// bytes: 字节数字；len：字节数组长度
int uart6_buf_status(void);							// 检查接收缓冲区中是否有数据，如果有，返回数据长度，如果没有，返回0
void uart6_read_buf(u8* buf, int len);				// 将串口接收寄存器内的内容前len个字节读入到buf中，然后清空串口接收缓冲区
void uart6_clear_buf(void);							// 清空串口接收缓冲区
/*************************************************************************************************************************************************************/


#endif


