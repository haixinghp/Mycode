/**
  ******************************************************************************
  * @file    delay.h
  * @author  Microcontroller Division
  * @version V0.0.1
  * @date    10/2016
  * @brief   uart functions header
  ******************************************************************************

  */
#ifndef __LOWPOWER_UART_H
#define __LOWPOWER_UART_H


//#define USARTBUF_MAX 400
#define USARTBUF_MAX 400
typedef struct
{

	unsigned char startRcv : 1;
	unsigned char stopRcv : 1;
	unsigned char cmdFlag : 1; //平台下发命令的标志
	unsigned char reverse : 5;
	
	unsigned char usartReceiveFlag;
	
	unsigned short usartLen;
	unsigned short usartLenPre;
	unsigned short usartCmdLen;
	unsigned short usartExtLen;
	
	unsigned char usartBuf[USARTBUF_MAX];
//	unsigned char usartCmdBuf[100];
//	unsigned char usartExtBuf[30];

} USART_INFO;

#define REV_OK		1
#define REV_WAIT	0

//extern USART_INFO usart1Info;
extern USART_INFO usart3Info;//uart3


void alarmer_uart3_send_byte(char);
void alarmer_uart3_send_word(char);
void debug_send(char *string_x);
void debug_send_no_end(char *string_x);
void uart3_init(uint32_t);

void uart1_send_byte(char);
void SendCmd2Mc8618(char *cmd);
void uart1_init(uint32_t);

void uart1_disable();
void uart3_disable();

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

void UsartPrintf(USART_TypeDef *USARTx, char *data);

void UsartReciveFlag(USART_INFO *usartInfo);

void Usart_RcvClr(USART_INFO *usartInfo);

void uart2_init(uint32_t BaudRate);
void uart2_disable(void);

void Usart1_RcvClr(void);
void Usart3_RcvClr(void);

void SetUart3RxAsExInt(void);

//void _printf(char *s, ...) ;


extern char str[250];
#endif

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
