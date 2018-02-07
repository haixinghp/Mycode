/**
  ******************************************************************************
  * @file
  * @author  ll
  * @version V0.0.1
  * @date    10/2016
  * @brief
             QQ:308621800
  ******************************************************************************
  */
#include "config.h"
#include "stm8l15x.h"
#include "stdio.h"
#include "stdlib.h"

#include <stdarg.h>
#include <string.h>

//USART_INFO usart1Info;//uart1

USART_INFO usart3Info;//uart3


//u8 j = 0;

//char r_buf[160]="";

char str[250] = "";//250byte

void alarmer_uart3_send_byte(char string_byte) {
//  while ((USART3->SR & 0x80) == 0x00); //��ѯ���ͻ��������ֽ��Ƿ��Ѿ����ͳ�ȥ
  USART3->DR = string_byte;//�����ַ�
  while ((USART3->SR & 0x80) == 0x00); //��ѯ���ͻ��������ֽ��Ƿ��Ѿ����ͳ�ȥ
}

void alarmer_uart3_send_word(char string_byte) {
  USART3->DR = string_byte;//�����ַ�
  while ((USART3->SR & 0x80) == 0x00); //��ѯ���ͻ��������ֽ��Ƿ��Ѿ����ͳ�ȥ
  USART3->DR = *((&string_byte) + 1); //�����ַ�
  while ((USART3->SR & 0x80) == 0x00); //��ѯ���ͻ��������ֽ��Ƿ��Ѿ����ͳ�ȥ
}

void debug_send(char *string_x) {
  while (*string_x) {
    alarmer_uart3_send_byte(*string_x++);
  }
  alarmer_uart3_send_byte(0x0D);
  alarmer_uart3_send_byte(0x0A);
}

void debug_send_no_end(char *string_x) {
  while (*string_x) {
    alarmer_uart3_send_byte(*string_x++);
  }
}

//
//void uart2_init(uint32_t BaudRate) {
//  //************ UTRA��ʼ�� ***************************
//  GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow);
//  GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT); //����3
//  CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE); //һ��Ҫ�򿪣��ϵ�Ĭ���ǹرգ�8S��Ĭ�ϴ�
//    // Configure USART Tx as alternate function push-pull  (software pull up)
//    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_4, ENABLE);
//
//    // Configure USART Rx as alternate function push-pull  (software pull up)
//    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_3, ENABLE);
//  USART_DeInit(USART2);
//  USART_Init(USART2, BaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx);
//  //USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//�رս����ж�
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�򿪴���3�����ж�
//  USART_ClearITPendingBit(USART2,USART_IT_RXNE);   
//  
//  USART_Cmd(USART2, ENABLE);
//  
//  
//  
//  // Enable general interrupts
//  enableInterrupts();
//  
//  // �崮�ڽ����жϱ�־
//  
//}
//
//
//
//
//void uart2_disable(void)
//{  
//  USART_ClearFlag(USART2, USART_FLAG_RXNE);//��������жϱ�־
//  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//�رս����ж�
//
//  GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast);
//  GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast);
//  GPIO_ResetBits(GPIOE, GPIO_Pin_4);
//  GPIO_ResetBits(GPIOE, GPIO_Pin_3);
//}


void uart2_init(uint32_t BaudRate)      //
{

  /************ UTRA��ʼ�� ***************************/
  GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow); //USART2_TX
  GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT); //USART2_RX
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE); //һ��Ҫ�򿪣��ϵ�Ĭ���ǹرգ�8S��Ĭ�ϴ�
  USART_DeInit(USART2);
//  USART_Init(USART2, BaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx);  
  USART_Init(USART2, BaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,  (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));  //??
//  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//�رս����ж�
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�򿪴���2�����ж� 
  USART_ClearFlag(USART2, USART_FLAG_RXNE);//��������жϱ�־
// USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�򿪴���2�����ж� //TEST HX  2017-05-25 09:33:26
//  SetUart3RxAsExInt();
   CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);//</span>    //��ʱ�ӣ�����Ҫ  
    /* Deinitialize DMA channels */    
    DMA_GlobalDeInit();
    DMA_DeInit(DMA1_Channel3);  
//    DMA_DeInit(DMA1_Channel3);  
      
    /* DMA channel Rx of USART Configuration */  
    //�ú�����ҪҪ���úý��ܵ����飬�Լ�USART�����ݼĴ�����ַ�������С���Լ�DMAģʽ  
    DMA_Init(DMA1_Channel3,(uint32_t)(&(CheakValue_Buf)), (uint16_t)0x53E1,(u8)(CHECKVALUE_BUFF_LEN<<2), DMA_DIR_PeripheralToMemory, DMA_Mode_Normal,DMA_MemoryIncMode_Inc, DMA_Priority_VeryHigh, DMA_MemoryDataSize_Byte);  
   
    /* DMA channel Tx of USART Configuration */  
    //�ú�����Ҫ���÷������飬�Լ�USART�����ݼĴ�����ַ�������С���Լ�DMAģʽ  
 //  DMA_Init(DMA1_Channel0, (uint16_t)(&(usart2Info->usartBuf)), (uint16_t)0x53E1,10, DMA_DIR_MemoryToPeripheral, DMA_Mode_Normal, DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_Byte);  
      
    /* Enable the USART Tx/Rx DMA requests */  
    DMA_ITConfig(DMA1_Channel3, DMA_ITx_TC, ENABLE);
    DMA_ClearFlag(DMA1_FLAG_TC3);
//    DMA_ITConfig(DMA1_Channel0, DMA_ITx_TC, ENABLE);
//    USART_DMACmd(USART2, USART_DMAReq_TX, ENABLE);  
    USART_DMACmd(USART2, USART_DMAReq_RX, ENABLE);  
   
    /* Global DMA Enable */  
   DMA_GlobalCmd(ENABLE);  
   
    /* Enable the USART Tx DMA channel */ 
}
void uart2_disable(void) 
{
  
    CLK_PeripheralClockConfig(CLK_Peripheral_USART2, DISABLE);
    USART_Cmd(USART2,DISABLE);
//  USART_ClearFlag(USART2, USART_FLAG_RXNE);//��������жϱ�־
//  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//�رս����ж�
//  
//  GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast);//USART2_TX 
//  GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast);//USART2_RX
//  GPIO_ResetBits(GPIOE, GPIO_Pin_4);
//  GPIO_ResetBits(GPIOE, GPIO_Pin_3);
}


//����uart3�Ľ�������Ϊ�ⲿ�ж����� ��Ϊ��������ʱ��
void SetUart3RxAsExInt(void){
  GPIO_Init(GPIOG,GPIO_Pin_0,GPIO_Mode_In_PU_IT);//���ó���������  ���ж�
  EXTI_SetPinSensitivity(EXTI_Pin_0,EXTI_Trigger_Falling); //�½����ж�
}


void uart3_init(uint32_t BaudRate) {
  /************ UTRA��ʼ�� ***************************/
  GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT); //����3
  CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE); //һ��Ҫ�򿪣��ϵ�Ĭ���ǹرգ�8S��Ĭ�ϴ�
  USART_DeInit(USART3);
  USART_Init(USART3, BaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx);
  //USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//�رս����ж�
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�򿪴���3�����ж�
  SetUart3RxAsExInt();
  USART_Cmd(USART3, ENABLE);
  
  SetUart3RxAsExInt();
}

void uart1_send_byte(char string_byte) {
  USART1->DR = string_byte;//�����ַ�
  while ((USART1->SR & 0x80) == 0x00); //��ѯ���ͻ��������ֽ��Ƿ��Ѿ����ͳ�ȥ
}


void SendCmd2Mc8618(char *cmd) {
  while (*cmd) {
    uart1_send_byte(*cmd++);
  }
  uart1_send_byte(0x0D);
  uart1_send_byte(0x0A);
}


void uart1_init(uint32_t BaudRate) {
  /************ UTRA��ʼ�� ***************************/
  //GPIO_Init(GPIOC,GPIO_Pin_2,GPIO_Mode_In_PU_No_IT);//���տ�
  //GPIO_Init(GPIOC,GPIO_Pin_3,GPIO_Mode_Out_PP_Low_Fast);//���Ϳ�

  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Slow);//����1����
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_No_IT);//����1����
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); //һ��Ҫ�򿪣��ϵ�Ĭ���ǹرգ�8S��Ĭ�ϴ�
  USART_DeInit(USART1);
  USART_Init(USART1, BaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,
             USART_Mode_Rx | USART_Mode_Tx);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);

}

void uart3_disable() {
  USART_ClearFlag(USART3, USART_FLAG_RXNE);//��������жϱ�־
  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//�رս����ж�

  GPIO_Init(GPIOG, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Fast);
  GPIO_ResetBits(GPIOG, GPIO_Pin_0);
  GPIO_ResetBits(GPIOG, GPIO_Pin_1);
}


void uart1_disable() 
{
  USART_ClearFlag(USART1, USART_FLAG_RXNE);//��������жϱ�־
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�رս����ж�
  USART_DeInit(USART1);
  USART_Cmd(USART1, DISABLE);
#ifdef SYS_LDO_20V
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //���տ�
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Fast); //���Ϳ�
  GPIO_ResetBits(GPIOC, GPIO_Pin_2);
  GPIO_ResetBits(GPIOC, GPIO_Pin_3);
#endif
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //���տ�
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast); //���Ϳ�
  GPIO_ResetBits(GPIOC, GPIO_Pin_2);
  GPIO_ResetBits(GPIOC, GPIO_Pin_3);
#endif   
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast); //���տ�
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast); //���Ϳ�
  GPIO_ResetBits(GPIOC, GPIO_Pin_2);
  GPIO_ResetBits(GPIOC, GPIO_Pin_3);
#endif   
}

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{

  unsigned short count = 0;

  for (; count < len; count++)
  {
    USART_SendData8(USARTx, *str++);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    //DelayUs(2);
  }

}



void UsartPrintf(USART_TypeDef *USARTx, char *pStr)
{
  //unsigned char UsartPrintfBuf[500];
  //va_list ap;
  //unsigned char *pStr = UsartPrintfBuf;

  //va_start(ap, fmt);
  //vsprintf(UsartPrintfBuf, fmt, ap);
  //va_end(ap);

  while (*pStr != 0)
  {
    USART_SendData8(USARTx, *pStr++);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
  }

}

void UsartReciveFlag(USART_INFO *usartInfo)
{

  while (1)
  {
    if (usartInfo->usartLen == 0) //������ռ���Ϊ0 ��˵��û�д��ڽ��������У�����ֱ����������������
      break;

    if (usartInfo->usartLen == usartInfo->usartLenPre) //�����һ�ε�ֵ�������ͬ����˵���������
    {
      usartInfo->usartReceiveFlag = REV_OK; //

      usartInfo->usartLen = 0; //��0���ռ���

      break; //����
    }

    usartInfo->usartLenPre = usartInfo->usartLen; //��Ϊ��ͬ

    DelayT_ms(1); //�����ʱ��Ϊ������һ�����֮�������û�н�����ɣ�usartLen��usartLenPre���ǻ᲻��ȵ�
  }

}


void Usart_RcvClr(USART_INFO *usartInfo){
  usartInfo->startRcv = 0;
  usartInfo->stopRcv = 0;

  usartInfo->usartLen = 0;
  usartInfo->usartCmdLen = 0;
  usartInfo->usartExtLen = 0;

  memset(usartInfo->usartBuf, 0, sizeof(usartInfo->usartBuf));
//  memset(usartInfo->usartCmdBuf, 0, sizeof(usartInfo->usartCmdBuf));
//  memset(usartInfo->usartExtBuf, 0, sizeof(usartInfo->usartExtBuf));

}

//void Usart1_RcvClr(void)
//{
//
//  usart1Info.startRcv = 0;
//  usart1Info.stopRcv = 0;
//
//  usart1Info.usartLen = 0;
//  usart1Info.usartCmdLen = 0;
//  usart1Info.usartExtLen = 0;
//
//  memset(usart1Info.usartBuf, 0, sizeof(usart1Info.usartBuf));
//  memset(usart1Info.usartCmdBuf, 0, sizeof(usart1Info.usartCmdBuf));
//  memset(usart1Info.usartExtBuf, 0, sizeof(usart1Info.usartExtBuf));
//
//}

void Usart3_RcvClr(void)
{

  usart3Info.startRcv = 0;
  usart3Info.stopRcv = 0;

  usart3Info.usartLen = 0;
  usart3Info.usartCmdLen = 0;
  usart3Info.usartExtLen = 0;

  memset(usart3Info.usartBuf, 0, sizeof(usart3Info.usartBuf));
//  memset(usart3Info.usartCmdBuf, 0, sizeof(usart3Info.usartCmdBuf));
//  memset(usart3Info.usartExtBuf, 0, sizeof(usart3Info.usartExtBuf));

}


void Uart3SendStr(u8 *s, int len)
{
	int i=0;
        USART_ClearFlag(USART3,USART_FLAG_TC);
	while(len--)
	{
		USART_SendData8(USART3, s[i]);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
		i++;
	}
}

//void _printf(char *s, ...) 
//{
//	
//	va_list argptr;
//	static char buffer[100];
//	
//	buffer[0]=0;
//	
//	va_start(argptr, s);
//	
//	vsprintf(buffer,s, argptr);
//	
//	va_end(argptr);
//	
//	Uart3SendStr((u8*)buffer, strlen(buffer));
//	
//	
//}