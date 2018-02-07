#include "config.h"
#include "USART.h"

/*********************************************************************************************/
/* �ڲ������������� */
static void UartStruct_Init(void);
static void USART1_Init(void);
static void USART2_Init(void);
static void USART3_Init(void);
//static void USART4_Init(void);
//static void USART5_Init(void);

/*********************************************************************************************/
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void UART4_IRQHandler(void);
void UART5_IRQHandler(void);

/*********************************************************************************************/
#define     MPDevVer            2.0	 //����汾

/*********************************************************************************************/
/*
ֹͣλ���ò���
#define USART_StopBits_1                     ((uint16_t)0x0000)
#define USART_StopBits_0_5                   ((uint16_t)0x1000)
#define USART_StopBits_2                     ((uint16_t)0x2000)
#define USART_StopBits_1_5                   ((uint16_t)0x3000)

У��λ���ò���
#define USART_Parity_No                      ((uint16_t)0x0000)
#define USART_Parity_Even                    ((uint16_t)0x0400)
#define USART_Parity_Odd                     ((uint16_t)0x0600) 

 ���ݳ��ȿ��ò���
#define USART_WordLength_8b                  ((uint16_t)0x0000)
#define USART_WordLength_9b                  ((uint16_t)0x1000)

Ӳ����ģʽ
#define USART_HardwareFlowControl_None       ((uint16_t)0x0000)
#define USART_HardwareFlowControl_RTS        ((uint16_t)0x0100)
#define USART_HardwareFlowControl_CTS        ((uint16_t)0x0200)
#define USART_HardwareFlowControl_RTS_CTS    ((uint16_t)0x0300)

ģʽ
#define USART_Mode_Rx                        ((uint16_t)0x0004)
#define USART_Mode_Tx                        ((uint16_t)0x0008)
*/
#define USART_HardwareFlowControl_None 0
/*********************************************************************************************/
//ӳ���ϵ����

/*********************************************************************************************/
//#if BUILD_PROJECT == BOOT_HD

	static UartCom _gUartComArr[USART_TYPR_MAX]	=
	{
	//	{���ڳ�ʼ������	�����ж���Ӧ���� 	��ַ	������	У��λ				���ݳ���				ֹͣλ				Ӳ����ģʽ						ģʽ						}
		{USART1_Init,	USART1_IRQHandler,	USART1,	{9600,	USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}},//����1
		{USART2_Init,	USART2_IRQHandler,	USART2,	{9600,  USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}},//����2
		{USART3_Init,	USART3_IRQHandler,	USART3,	{9600,	USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}},//����3
//		{USART4_Init,	UART4_IRQHandler,	UART4,	{9600,	USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}},//����4
//		{USART5_Init,	UART5_IRQHandler,	UART5,	{9600,	USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}} //����5
	};
	static UsartTypeToFunctionStruct  _gUTypeToFunArr[USART_TYPR_MAX] =
	{
		{USART_TYPR_COM1,USART_MODE_TYPE_IRQ,USART_FUNCTION_WCDMA},
		{USART_TYPR_COM2,USART_MODE_TYPE_IRQ,USART_FUNCTION_MSG},
		{USART_TYPR_COM3,USART_MODE_TYPE_IRQ,USART_FUNCTION_DEBUG},
//		{USART_TYPR_COM4,USART_MODE_TYPE_IRQ,(USART_FUNCTION)(USART_FUNCTION_MOUNT + 0)},
//		{USART_TYPR_COM5,USART_MODE_TYPE_IRQ,(USART_FUNCTION)(USART_FUNCTION_MOUNT + 1)}
	};

//#endif
/*********************************************************************************************/
static UartStruct _gUartStuct[USART_TYPR_MAX] = { 				//���ڳ�ʼ�������մ��ڵ���˳������
 	0
};

static UartStruct* _gUartStuctBaseFun[USART_FUNCTION_MAX] = { 	//���ڰ��չ��ܽ�������
 	0
};


USART_FUNCTION Get_Usart_Function(USART_TYPE ut)
{
	if(ut<USART_TYPR_MAX)
	{
		return _gUartStuct[ut].uTypeToFun->usartFun;
	}

	return USART_FUNCTION_MAX;
}

UartStruct* GetUartStructBaseFunction(USART_FUNCTION fun)
{
 	if(fun < USART_FUNCTION_MAX)
 		return	 _gUartStuctBaseFun[fun];
	return NULL; 
}

ProtocolResource * GetUtProtocolResource(USART_TYPE ut)
{
	if(ut<USART_TYPR_MAX)
	{
		return	GetProtocolResource(_gUartStuct[ut].uTypeToFun->usartFun);	
	}
	return NULL;	
}
/*********************************************************************************************/
typedef struct
{
	uint8_t     USART_BufOk;    			      //BUF�Ƿ��Ѿ����� 
    __IO int HALDMAUART_OverTime ;  		      //DMA���ճ�ʱ����      
}UARTDMA_CFG;

/*����DMA��ʼ������*/
//DMA_InitTypeDef DMA_InitStructure[USART_TYPR_MAX];

/*DMA���ڽ������ݳ�ʱʱ��*/
#define DMA_USATR_RX_TIMEOUT  20

/*DMA��������״̬��������ṹ*/
UARTDMA_CFG	_gucfg[USART_TYPR_MAX] = {
/*--------------------------------------*/
/*  ��������־        ��ʱ   	        */
	{false, 	DMA_USATR_RX_TIMEOUT	},
	{false, 	DMA_USATR_RX_TIMEOUT	},
	{false, 	DMA_USATR_RX_TIMEOUT	},
//	{false, 	DMA_USATR_RX_TIMEOUT	},
//	{false, 	DMA_USATR_RX_TIMEOUT	},
};

//������������������������������������������������������������������������������
// ��������  : DMA_UsartConfiguration
// ��������  : ���ڽ�������DMA����
// �������  : df DMAͨ��; ut ���ڱ�� 
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������


//int DMA_UsartConfiguration(DMA_InitTypeDef *df,USART_TYPE ut)
//{
//	DMA_InitTypeDef *def = &DMA_InitStructure[ut];
//	ProtocolResource *Protocol = GetUtProtocolResource(ut);
//
//	if((Protocol == NULL)||(def == NULL))
//		return -1;
//		  	
//	DMA_DeInit(df); 
//        
//                                	def->DMA_PeripheralBaseAddr = (uint32_t)(&(_gUartComArr[ut].USARTx->DR));
//                                	def->DMA_MemoryBaseAddr = (uint32_t)Protocol->RxMessageBuffer;
//                                	def->DMA_DIR = DMA_DIR_PeripheralSRC;
//                                	def->DMA_BufferSize = Protocol->size;
//                                	def->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//                                	def->DMA_MemoryInc = DMA_MemoryInc_Enable;
//                                	def->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//                                	def->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//                                	def->DMA_Mode = DMA_Mode_Circular;
//                                	def->DMA_Priority = DMA_Priority_Medium;
//                                	def->DMA_M2M = DMA_M2M_Disable;
//                                        
//                                        DMA_Init_1(df,def);
//                                //	DMA_Init(df, def);
//                 
//       
//        
//
//	/* Enable USART DMA Rx request */
//	USART_DMACmd(_gUartComArr[ut].USARTx, USART_DMAReq_Rx, ENABLE);
//	DMA_ITConfig(df, DMA_IT_TC, ENABLE);
//
//	return 0;
//}

//DMA_Init_1(DMA_InitTypeDef *df,DMA_InitTypeDef *def)
//{
//  DMA_Init(     DMA1_Channel3,\
//                (uint32_t)(&Protocol->RxMessageBuffer), \
//                (uint16_t)(&(_gUartComArr[ut].USARTx->DR)),\
//                 (u8)(Protocol->size),\
//                  DMA_DIR_PeripheralToMemory, \
//                  DMA_Mode_Circular, \
//                  DMA_MemoryIncMode_Inc,\
//                DMA_Priority_Medium, \
//                 DMA_MemoryDataSize_Byte);
//  
//}
                                        
void DMA_UsartConfiguration(uint32_t BaudRate,USART_TYPE ut)      //
{

  ProtocolResource *Protocol = GetUtProtocolResource(ut);
  
  /************ UTRA��ʼ�� ***************************/
  GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow); //USART2_TX
  GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT); //USART2_RX
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE); //һ��Ҫ�򿪣��ϵ�Ĭ���ǹرգ�8S��Ĭ�ϴ�
  USART_DeInit(_gUartComArr[ut].USARTx);
//  USART_Init(USART2, BaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx);  
  USART_Init(_gUartComArr[ut].USARTx, BaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,  (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));  //??
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
    DMA_Init(DMA1_Channel3,(uint32_t)(&Protocol->RxMessageBuffer), (uint16_t)(&(_gUartComArr[ut].USARTx->DR)),(u8)(Protocol->size), DMA_DIR_PeripheralToMemory, DMA_Mode_Circular, DMA_MemoryIncMode_Inc, DMA_Priority_Medium, DMA_MemoryDataSize_Byte);  
   
    /* DMA channel Tx of USART Configuration */  
    //�ú�����Ҫ���÷������飬�Լ�USART�����ݼĴ�����ַ�������С���Լ�DMAģʽ  
 //  DMA_Init(DMA1_Channel0, (uint16_t)(&(usart2Info->usartBuf)), (uint16_t)0x53E1,10, DMA_DIR_MemoryToPeripheral, DMA_Mode_Normal, DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_Byte);  
      
    /* Enable the USART Tx/Rx DMA requests */  
    DMA_ITConfig(DMA1_Channel3, DMA_ITx_TC, ENABLE);
    DMA_ClearFlag(DMA1_FLAG_TC3);
//    DMA_ITConfig(DMA1_Channel0, DMA_ITx_TC, ENABLE);
//    USART_DMACmd(USART2, USART_DMAReq_TX, ENABLE);  
    USART_DMACmd(_gUartComArr[ut].USARTx, USART_DMAReq_RX, ENABLE);  
   
    /* Global DMA Enable */  
   DMA_GlobalCmd(ENABLE);  
   
    /* Enable the USART Tx DMA channel */ 
}	
//������������������������������������������������������������������������������
// ��������  : DMA_Usart_Init
// ��������  : ����DMA���ճ�ʼ��
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
int DMA_Usart_Init(USART_FUNCTION fun)
{
	UartStruct* uts = GetUartStructBaseFunction(fun);
//	NVIC_InitTypeDef NVIC_InitStructure;
//	IRQn_Type irqtype = (IRQn_Type)0xff;
	DMA_Channel_TypeDef *df = NULL;
	USART_TYPE ut;
 	ut = uts->uTypeToFun->usartType;
	if((uts == NULL)||(_gUTypeToFunArr[ut].mode != USART_MODE_TYPE_DMA))
		return -1;


	if(ut >= USART_TYPR_MAX)
		return -1;

	switch(ut)
	{
		case USART_TYPR_COM1:		  	//����1
			/*����DMA1ʱ��*/
                        CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
                        
			break;		 
		case USART_TYPR_COM2:	   		//����2
			/*����DMA1ʱ��*/
                        CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
                  
			break;
		case USART_TYPR_COM3:		   	//����3
			/*����DMA1ʱ��*/
                        CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
			break;
//		case USART_TYPR_COM4:		   	//����4
//			/*����DMA2ʱ��*/
//			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
//			irqtype = DMA2_Channel3_IRQn;	
//			df = DMA2_Channel3;
//			break;
//		case USART_TYPR_COM5:		   	//����5
//			break;
	}

	if(df == NULL)
		return -1;

	/* DMA��ʼ��*/
//	DMA_UsartConfiguration(df,ut);

//	/* Enable DMA1 channel IRQ Channel _Rx */
//	NVIC_InitStructure.NVIC_IRQChannel = irqtype;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	

	DMA_Cmd(df, ENABLE);

	return 0;
}


//������������������������������������������������������������������������������
// ��������  : DMA_UART_Receive
// ��������  : ����DMAͨ���жϽ�������
// �������  : df DMAͨ��; ut ���ڱ�� 
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
int DMA_UART_Receive(DMA_Channel_TypeDef *df,USART_TYPE ut)
{
//	uint8_t ITStatus_TC = 0;
//	uint8_t ITStatus_GL = 0;
////	DMA_InitTypeDef *def = &DMA_InitStructure[ut];
//	UARTDMA_CFG *ucfg = &_gucfg[ut];
//	ProtocolResource *Protocol = GetUtProtocolResource(ut);
//
//	if((def == NULL)||(ucfg == NULL)||(Protocol == NULL)||(_gUTypeToFunArr[ut].mode != USART_MODE_TYPE_DMA))
//		return -1;
//
//	switch((uint16_t)df)
//	{
//		case DMA1_Channel1_BASE:	 /*����1 DMA����*/
//			 ITStatus_TC = DMA1_IT_TC1;
//	         //ITStatus_GL = DMA1_IT_GL5;
//			break;
//		case DMA1_Channel2_BASE:	 /*����2 DMA����*/
//			 ITStatus_TC = DMA1_IT_TC2;
//	         //ITStatus_GL = DMA1_IT_GL6;
//			break;
//		case DMA1_Channel3_BASE:	 /*����3 DMA����*/
//			 ITStatus_TC = DMA1_IT_TC3;
//	         //ITStatus_GL = DMA1_IT_GL3;
//			break;
////		case (uint32_t)DMA2_Channel3:	 /*����4 DMA����*/
////			 ITStatus_TC = DMA2_IT_TC3;
////	         ITStatus_GL = DMA2_IT_GL3;
////			break;
//	}
//	
//	/*ͨ����������ж�TC ���д��� �����ж�HT  �����ж�TE  ȫ���ж�GL*/
//	if(DMA_GetITStatus(ITStatus_TC))  
//	{
//		/*���ȫ���жϱ�־*/
//		DMA_ClearITPendingBit(ITStatus_GL);    
//         
//		DMA_Init(DMA1_Channel3,(uint32_t)(&Protocol->RxMessageBuffer), (uint16_t)(&(_gUartComArr[ut].USARTx->DR)),(u8)(Protocol->size), DMA_DIR_PeripheralToMemory, DMA_Mode_Circular, DMA_MemoryIncMode_Inc, DMA_Priority_Medium, DMA_MemoryDataSize_Byte);  
//   
////		def->DMA_MemoryBaseAddr = (uint32_t)(Protocol->RxMessageBuffer);   
////		def->DMA_PeripheralBaseAddr = (uint32_t)(&(_gUartComArr[ut].USARTx->DR));
////		def->DMA_Mode = DMA_Mode_Circular;                                //������ѭ������ģʽ
////		def->DMA_DIR = DMA_DIR_PeripheralSRC;
////		def->DMA_BufferSize = Protocol->size;
////		DMA_Init(df,def);
//
//		ucfg->USART_BufOk = true; //��׼���õ�������		
//	}

	return 0;
}

//������������������������������������������������������������������������������
// ��������  : USART1_Timeout
// ��������  : ���ڽ��ճ�ʱ������
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
void USART1_Timeout(void)
{
	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM1);
//	Reset_Protocol(Protocol);
//        Reset_Protocol_Rx(Protocol); // ֻ��λ���ջ��棬by hx 2017-11-13
        Reset_And_Init_Protocol_Rx(Protocol); //by hx 2017-12-16
}

//������������������������������������������������������������������������������
// ��������  : USART2_Timeout
// ��������  : ���ڽ��ճ�ʱ������
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
void USART2_Timeout(void)
{
	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM2);
//	Reset_Protocol(Protocol);
//        Reset_Protocol_Rx(Protocol); // ֻ��λ���ջ��棬by hx 2017-11-13
        Reset_And_Init_Protocol_Rx(Protocol); //by hx 2017-12-16
}

//������������������������������������������������������������������������������
// ��������  : USART3_Timeout
// ��������  : ���ڽ��ճ�ʱ������
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
void USART3_Timeout(void)
{
	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM3);
//	Reset_Protocol(Protocol);
//        Reset_Protocol_Rx(Protocol); // ֻ��λ���ջ��棬by hx 2017-11-13
        Reset_And_Init_Protocol_Rx(Protocol); //by hx 2017-12-16
}

//������������������������������������������������������������������������������
// ��������  : USART4_Timeout
// ��������  : ���ڽ��ճ�ʱ������
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
//void USART4_Timeout(void)
//{
//	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM4);
//	Reset_Protocol(Protocol);
//}

//������������������������������������������������������������������������������
// ��������  : USART5_Timeout
// ��������  : ���ڽ��ճ�ʱ������
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
//void USART5_Timeout(void)
//{
//	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM5);
//	Reset_Protocol(Protocol);
//}

//������������������������������������������������������������������������������
// ��������  : DMA_USART_Timeout
// ��������  : ���ڽ��ճ�ʱ����
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
void DMA_USART_Timeout(USART_TYPE ut)
{
	USART_FUNCTION ufun = Get_Usart_Function(ut);

	if((ut<USART_TYPR_MAX)&&(ufun<USART_FUNCTION_MAX))
	{
		switch(ut)
		{
			case USART_TYPR_COM1:		  	//����1
				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART1_Timeout,NULL);
				break;		 
			case USART_TYPR_COM2:	   		//����2
				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART2_Timeout,NULL);
				break;
			case USART_TYPR_COM3:		   	//����3
				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART3_Timeout,NULL);
				break;
//			case USART_TYPR_COM4:		   	//����4
//				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART4_Timeout,NULL);
//				break;
//			case USART_TYPR_COM5:		   	//����5
//				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART5_Timeout,NULL);
//				break; 		
		}
	}	
}
//������������������������������������������������������������������������������
// ��������  : DMA_UART_Receive_Process
// ��������  : ����DMA���մ�������
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
void DMA_UART_Receive_Process(void)
{
//	uint16_t tempLen = 0;
//	static volatile uint16_t oldRecvLen[USART_TYPR_MAX] = {0};
//	int ut = 0;
//
//	for(ut=0;ut<USART_TYPR_MAX;ut++)
//	{
//		UARTDMA_CFG	*gucfg = &_gucfg[ut];
////		DMA_InitTypeDef *def = &DMA_InitStructure[ut];
//		DMA_Channel_TypeDef *df  = NULL;
//		ProtocolResource *Protocol = GetUtProtocolResource((USART_TYPE)ut);
//	
//		switch(ut)
//		{
//			case USART_TYPR_COM1:		  	//����1
//				df = DMA1_Channel1;
//				break;		 
//			case USART_TYPR_COM2:	   		//����2
//				df = DMA1_Channel2;
//				break;
//			case USART_TYPR_COM3:		   	//����3
//				df = DMA1_Channel3;
//				break;
////			case USART_TYPR_COM4:		   	//����4
////				df = DMA2_Channel3;
////				break;
////			case USART_TYPR_COM5:		   	//����5
////				break; 		
//		}
//
//		if((Protocol == NULL)||
//		   (df == NULL)||
//		   (def == NULL)||
//		   (gucfg == NULL)||
//		   (_gUTypeToFunArr[ut].mode != USART_MODE_TYPE_DMA))
//		{
//			continue;
//		}
//
//		if (gucfg->HALDMAUART_OverTime != 0x00)
//		{ 
//			gucfg->HALDMAUART_OverTime--;
//		}
//	
//		if(gucfg->USART_BufOk == true)
//		{
//			gucfg->USART_BufOk = false; //������׼���õ�����
//	
//			/**********************************************************/
//			Protocol->Port_Type = PORT_TYPE_SERIAL;
//
//				DMA_USART_Timeout((USART_TYPE)ut);
//				DoProtocol_DMA_ReceiveAnalysis(Protocol,Protocol->size);
//			/**********************************************************/
//			gucfg->HALDMAUART_OverTime = DMA_USATR_RX_TIMEOUT;	
//		}
//		else if(gucfg->HALDMAUART_OverTime == 0)
//		{
//			tempLen = Protocol->size - DMA_GetCurrDataCounter(df);
//			if(tempLen > 0)
//			{
//				DMA_Cmd(df, DISABLE);
////				def->DMA_BufferSize = Protocol->size;
////				DMA_Init(df, def);
//                                DMA_Init(DMA1_Channel3,(uint32_t)(&Protocol->RxMessageBuffer), (uint16_t)(&(_gUartComArr[ut].USARTx->DR)),(u8)(Protocol->size), DMA_DIR_PeripheralToMemory, DMA_Mode_Circular, DMA_MemoryIncMode_Inc, DMA_Priority_Medium, DMA_MemoryDataSize_Byte);  
//   
//				DMA_Cmd(df, ENABLE);	
//				
////------------------------------------------------------------------------
//				Protocol->Port_Type = PORT_TYPE_SERIAL;
//
//					DMA_USART_Timeout((USART_TYPE)ut);
//					DoProtocol_DMA_ReceiveAnalysis(Protocol,tempLen);
////-----------------------------------------------------------------------------				
//
//			}
//			oldRecvLen[ut] = tempLen; 
//			gucfg->HALDMAUART_OverTime = DMA_USATR_RX_TIMEOUT;
//		}
//		else			  
//		{
//			tempLen = Protocol->size - DMA_GetCurrDataCounter(df);
//			
//			if((tempLen == 0)||(tempLen != oldRecvLen[ut]))
//			{
//				gucfg->HALDMAUART_OverTime = DMA_USATR_RX_TIMEOUT;	
//			}	
//			oldRecvLen[ut] = tempLen; 							
//		}
//	}
}

//������������������������������������������������������������������������������
// ��������  : DMA1_Channel5_IRQHandler
// ��������  : ����1 DMA�����ж�
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
void DMA1_Channel1_IRQHandler(void)
{ 
	DMA_UART_Receive(DMA1_Channel1,USART_TYPR_COM1);	
}

//������������������������������������������������������������������������������
// ��������  : DMA1_Channel6_IRQHandler
// ��������  : ����2 DMA�����ж�
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
void DMA1_Channel2_IRQHandler(void)
{ 
	DMA_UART_Receive(DMA1_Channel2,USART_TYPR_COM2);	
}

//������������������������������������������������������������������������������
// ��������  : DMA1_Channel3_IRQHandler
// ��������  : ����3 DMA�����ж�
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
void DMA1_Channel3_IRQHandler(void)
{ 
	DMA_UART_Receive(DMA1_Channel3,USART_TYPR_COM3);	
}

//������������������������������������������������������������������������������
// ��������  : DMA2_Channel3_IRQHandler
// ��������  : ����4 DMA�����ж�
// �������  : None
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
//void DMA2_Channel3_IRQHandler(void)
//{ 
////	DMA_UART_Receive(DMA2_Channel3,USART_TYPR_COM4);	
//}

//������������������������������������������������������������������������������
// ��������  : SettingSave
// ��������  : ��������
// �������  : uCom,���ڿ��ƽṹ��ָ��
// �������  : None								  
// ���ز���  : None
//������������������������������������������������������������������������������
static void SettingSave(UartCom* uCom)
{
	USART_TYPE ut = USART_TYPR_MAX;
	if(uCom != NULL)
	{
          
                CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); //һ��Ҫ�򿪣��ϵ�Ĭ���ǹرգ�8S��Ĭ�ϴ�
                USART_DeInit(uCom->USARTx);
                USART_Init(uCom->USARTx, uCom->UartCommAag.baudRate, uCom->UartCommAag.wordLength, uCom->UartCommAag.stopBits, USART_Parity_No,
                           USART_Mode_Rx | USART_Mode_Tx);
                USART_ITConfig(uCom->USARTx, USART_IT_RXNE, ENABLE);
                USART_Cmd(uCom->USARTx, ENABLE);
                
//		USART_TypeDef* USARTx = uCom->USARTx;
//		USART_InitTypeDef USART_InitStructure;
//		USART_ClockInitTypeDef  USART_ClockInitStructure;
	
		// USART ��ʼ��  
//		USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
//		USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
//		USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//		USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;	
//		USART_ClockInit(USARTx, &USART_ClockInitStructure);
//		 
//		USART_Cmd(USARTx, DISABLE);
//		USART_InitStructure.USART_BaudRate =uCom->UartCommAag.baudRate;
//	
//		//ֹ֡ͣλ��USART_StopBits_1��1��ֹͣλ��USART_StopBits_0.5��0.5��ֹͣλ USART_StopBits_2��2��ֹͣλ��USART_StopBits_1.5��1.5��ֹͣλ
//		USART_InitStructure.USART_StopBits = uCom->UartCommAag.stopBits;
//		//������żģʽ��USART_Parity_No����żʧ�ܣ�USART_Parity_Even��żģʽUSART_Parity_Odd����ģʽ
//		//����STM32����Ϊ��żУ��ʱ����Ҫ��������λΪ9λ����У��ʱ��8λ����
//
//		USART_InitStructure.USART_Parity = uCom->UartCommAag.parity;
//		USART_InitStructure.USART_WordLength = uCom->UartCommAag.wordLength;
//		USART_InitStructure.USART_HardwareFlowControl = uCom->UartCommAag.hardwareFlowControl;
//		//ʹ�ܷ��ͺͽ���
//		USART_InitStructure.USART_Mode = uCom->UartCommAag.mode;
//		USART_Init(USARTx, &USART_InitStructure);
//		//USART ʹ�ܿ��ж� 
//	  	USART_Cmd(USARTx, ENABLE);
		
		switch((uint16_t)uCom->USARTx)
		{
			case USART1_BASE:
				ut = USART_TYPR_COM1;
				break;
			case USART2_BASE:
				ut = USART_TYPR_COM2;
				break;
			case USART3_BASE:
				ut = USART_TYPR_COM3;
				break;
//			case (uint32_t)UART4:
//				ut = USART_TYPR_COM4;
//				break;
//			case (uint32_t)UART5:
//				ut = USART_TYPR_COM5;
				break;
		} 
		if(ut < USART_TYPR_MAX)
		{
			if(_gUTypeToFunArr[ut].mode == USART_MODE_TYPE_IRQ)
			{
				USART_ClearITPendingBit(uCom->USARTx,USART_IT_RXNE);  //�崮�ڽ����жϱ�־ 
				USART_ITConfig(uCom->USARTx,USART_IT_RXNE, ENABLE);   //ʹ�ܴ��ڽ����ж�
			}
		}
	}
}

//������������������������������������������������������������������������������
// ��������  : SettingSaveByUartStruct
// ��������  : ��������
// �������  : us,ӳ����ƽṹ��ָ��
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

static void SettingSaveByUartStruct(UartStruct* uS)
{
	if(uS != NULL)
	{
		 SettingSave(uS->uCom);		 
	}
}

//������������������������������������������������������������������������������
// ��������  : SettingSaveByUSART_TYPE
// ��������  : ��������
// �������  : ut,���ں�
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void SettingSaveByUSART_TYPE(USART_TYPE ut)
{
	if(ut < USART_TYPR_MAX)
	{
		 SettingSaveByUartStruct(&_gUartStuct[ut]);
	}
}

//������������������������������������������������������������������������������
// ��������  : SettingSaveByUSART_FUNCTION
// ��������  : ��������
// �������  : uf,���ڵĹ���
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void SettingSaveByUSART_FUNCTION(USART_FUNCTION uf)
{
	if(uf < USART_FUNCTION_MAX)
	{
		 SettingSaveByUartStruct(_gUartStuctBaseFun[uf]);
	}
}
	
//������������������������������������������������������������������������������
// ��������  : ReadSettingByUSART_TYPE
// ��������  : ��EEROM�ڶ�ȡ�趨ֵ
// �������  : ut,���ں�
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void ReadSettingByUSART_TYPE(USART_TYPE ut)
{
	if(ut < USART_TYPR_MAX)
	{
	}
}

//������������������������������������������������������������������������������
// ��������  : USART1_Init
// ��������  : ����1��ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void USART1_Init(void)
{							  
#if  CONF_UART1_EN

  
  
    // Configure USART Tx as alternate function push-pull  (software pull up)
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_3, ENABLE);

    // Configure USART Rx as alternate function push-pull  (software pull up)
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_2, ENABLE);
    
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Slow);//����1����
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_No_IT);//����1����
//  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);//����1����
//  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_IT);//����1����
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); //һ��Ҫ�򿪣��ϵ�Ĭ���ǹرգ�8S��Ĭ�ϴ�
  USART_DeInit(USART1);
  USART_Init(USART1, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,
             USART_Mode_Rx | USART_Mode_Tx);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);
	/*�Դ��ڲ����������ÿ�ʼ*/

	/*�Դ��ڲ����������ý���*/
	ReadSettingByUSART_TYPE(USART_TYPR_COM1);  //��EEROM�ڶ�ȡ�������ò���
	SettingSaveByUSART_TYPE(USART_TYPR_COM1);

#endif
	/********************************************************************************/	
}



//������������������������������������������������������������������������������
// ��������  : USART2_Init
// ��������  : ����2��ʼ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void USART2_Init(void)
{
#if CONF_UART2_EN
    CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE);
    
    // Configure USART Tx as alternate function push-pull  (software pull up)
    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_4, ENABLE);

    // Configure USART Rx as alternate function push-pull  (software pull up)
    GPIO_ExternalPullUpConfig(GPIOE, GPIO_Pin_3, ENABLE);

    
//      USART_Init(USART2, 9600,
//               (USART_WordLength_TypeDef)_USART.USART_WordLenth,
//               (USART_StopBits_TypeDef)_USART.USART_StopBits,
//               (USART_Parity_TypeDef)_USART.USART_Parity,
//               (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));
   USART_Init(USART2, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,  (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));  //??
 
  // Enable the USART Receive interrupt: this interrupt is generated when the USART
  // receive data register is not empty
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  
  // Enable USART
  USART_Cmd(USART2, ENABLE);
  
  // Enable general interrupts
  enableInterrupts();
  
  // �崮�ڽ����жϱ�־
  USART_ClearITPendingBit(USART2,USART_IT_RXNE);     
  
  
  
	ReadSettingByUSART_TYPE(USART_TYPR_COM2);  //��EEROM�ڶ�ȡ�������ò���
	SettingSaveByUSART_TYPE(USART_TYPR_COM2);

#endif
	/********************************************************************************/
}
 //������������������������������������������������������������������������������
// ��������  : USART3_Init
// ��������  : ����3��ʼ��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

static void USART3_Init(void)
{
#if CONF_UART3_EN
  
 
  GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT); //����3
  CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE); //һ��Ҫ�򿪣��ϵ�Ĭ���ǹرգ�8S��Ĭ�ϴ�
  USART_DeInit(USART3);
  USART_Init(USART3, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx);
  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//�رս����ж�
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�򿪴���3�����ж�
//  SetUart3RxAsExInt();
  USART_Cmd(USART3, ENABLE);
	/********************************************************************************/
	ReadSettingByUSART_TYPE(USART_TYPR_COM3);  //��EEROM�ڶ�ȡ�������ò���
	SettingSaveByUSART_TYPE(USART_TYPR_COM3);
	/********************************************************************************/ 
        
#endif
}

//������������������������������������������������������������������������������
// ��������  : DoProtocol
// ��������  : ִ��Э��
// �������  : ut,���ں� isSend,�Ƿ��Ƿ�������
// �������  : None
// ���ز���  : 0�ɹ�,-1ʧ��
//������������������������������������������������������������������������������

int DoProtocol(USART_TYPE ut,BOOL isSend)
{
	if(ut < USART_TYPR_MAX)
	{
		UsartTypeToFunctionStruct* uTypeToFun = _gUartStuct[ut].uTypeToFun;

		if(uTypeToFun != NULL)
		{
			 
			if(isSend)
			{
				return ProtocolSendByte(uTypeToFun->usartFun);
			} else {
				return ProtocolReceiveByte(uTypeToFun->usartFun);	
			} 				
		}
	}
	return -1;	
}

//������������������������������������������������������������������������������
// ��������  : USART1_IRQHandler
// ��������  : ����1�ж�����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

void USART1_IRQHandler(void)
{

#if CONF_UART1_EN
   if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�    
  {USART_ClearFlag(USART1, USART_FLAG_RXNE);
    Protocol_Receive();
    //DoProtocol(USART_TYPR_COM1,false);
//    DATA_Buf = USART1->DR;
//    qwl_sendchar(DATA_Buf);
    
  }
//	if( USART_GetITStatus(USART1,USART_IT_RXNE) )
//	{
//        USART_ClearITPendingBit(USART1,USART_IT_RXNE);  //�崮�ڽ����жϱ�־
//	   	DoProtocol(USART_TYPR_COM1,false);
//	}
//
//	/* ��������������㣬����ñ�־�����ָ�ͨ�� */
//	if (USART_GetFlagStatus(USART1, USART_FLAG_OR) == SET)
//	{
//		USART_ClearFlag(USART1,USART_FLAG_OR);                    
//		USART_ReceiveData8(USART1);
//	}
//
//	if( USART_GetITStatus(USART1,USART_IT_TXE) )
//	{	
//        USART_ClearITPendingBit(USART1,USART_IT_TXE);   //�崮�ڽ����жϱ�־
//		DoProtocol(USART_TYPR_COM1,true);				
//	}
#endif	
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
#if CONF_UART2_EN
	if( USART_GetITStatus(USART2,USART_IT_RXNE) )
	{		
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);  //�崮�ڽ����жϱ�־
		DoProtocol(USART_TYPR_COM2,false);   
	}

	/* ��������������㣬����ñ�־�����ָ�ͨ�� */
//	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET)
//	{
//		USART_ClearFlag(USART2,USART_FLAG_ORE);                    
//		USART_ReceiveData(USART2);
//	}

	if( USART_GetITStatus(USART2,USART_IT_TXE) )
	{	
        USART_ClearITPendingBit(USART2,USART_IT_TXE);   //�崮�ڽ����жϱ�־
		DoProtocol(USART_TYPR_COM2,true);						
	}
#endif	
}

  
//������������������������������������������������������������������������������
// ��������  : USART3_IRQHandler
// ��������  : ����3�ж�����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void USART3_IRQHandler(void)
{
#if CONF_UART3_EN
	if( USART_GetITStatus(USART3,USART_IT_RXNE) )
	{		
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);  //�崮�ڽ����жϱ�־
		DoProtocol(USART_TYPR_COM3,false);   
	}

	/* ��������������㣬����ñ�־�����ָ�ͨ�� */
	if (USART_GetFlagStatus(USART3, USART_FLAG_OR) == SET)
	{
		USART_ClearFlag(USART3,USART_FLAG_OR);                    
		USART_ReceiveData8(USART3);
	}

	if( USART_GetITStatus(USART3,USART_IT_TXE) )
	{	
        USART_ClearITPendingBit(USART3,USART_IT_TXE);   //�崮�ڽ����жϱ�־
		DoProtocol(USART_TYPR_COM3,true);				
	}
#endif	
}

/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void UART4_IRQHandler(void)
//{
//#if CONF_UART4_EN
//	if( USART_GetITStatus(UART4,USART_IT_RXNE) )
//	{		
//        USART_ClearITPendingBit(UART4,USART_IT_RXNE);  //�崮�ڽ����жϱ�־
//		DoProtocol(USART_TYPR_COM4,false);	  
//	}
//
//	/* ��������������㣬����ñ�־�����ָ�ͨ�� */
//	if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) == SET)
//	{
//		USART_ClearFlag(UART4,USART_FLAG_ORE);                    
//		USART_ReceiveData(UART4);
//	}
//
//	if( USART_GetITStatus(UART4,USART_IT_TXE) )
//	{	
//        USART_ClearITPendingBit(UART4,USART_IT_TXE);   //�崮�ڽ����жϱ�־
//		DoProtocol(USART_TYPR_COM4,true);							
//	}
//#endif	
//}


/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void UART5_IRQHandler(void)
//{
//#if CONF_UART5_EN
//	if( USART_GetITStatus(UART5,USART_IT_RXNE) )
//	{		
//        USART_ClearITPendingBit(UART5,USART_IT_RXNE);  //�崮�ڽ����жϱ�־
//		DoProtocol(USART_TYPR_COM5,false); 
//	}
//	/* ��������������㣬����ñ�־�����ָ�ͨ�� */
//	if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) == SET)
//	{
//		USART_ClearFlag(UART5,USART_FLAG_ORE);                    
//		USART_ReceiveData(UART5);
//	}
//
//	if( USART_GetITStatus(UART5,USART_IT_TXE))
//	{	
//        USART_ClearITPendingBit(UART5,USART_IT_TXE);   //�崮�ڽ����жϱ�־
//		DoProtocol(USART_TYPR_COM5,true);							
//	}
//#endif	
//}

//������������������������������������������������������������������������������
// ��������  : UartComInit
// ��������  : ���ݴ��ں͹���ӳ���ϵ���Դ��ڽ��г�ʼ��.
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void UartComInit()
{
	int i = 0;
	for(i = 0; i< 3;i++)
	{
		if(_gUartComArr[i].init != NULL)
		{
			_gUartComArr[i].init();	//��ʼ������
		}
	}
}

//������������������������������������������������������������������������������
// ��������  : UartStruct_Init
// ��������  : ����ӳ���ϵ�ĳ�ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void UartStruct_Init(void)
{
	int i = 0;
	for(i = 0; i < USART_TYPR_MAX; i++)
	{
		int usartType = _gUTypeToFunArr[i].usartType;
		if(usartType < USART_TYPR_MAX)
		{
			_gUartStuct[usartType].uTypeToFun =&_gUTypeToFunArr[i];  	
			_gUartStuct[usartType].uCom = &_gUartComArr[usartType];
			_gUartStuct[usartType].ready = true;
			if(_gUTypeToFunArr[i].usartFun < USART_FUNCTION_MAX)
			{
				_gUartStuctBaseFun[_gUTypeToFunArr[i].usartFun] = &_gUartStuct[usartType];
			}
		}
		else 
		{
			_gUartStuct[usartType].ready = false;
		}
	}
}

//������������������������������������������������������������������������������
// ��������  : Uart_Init
// ��������  : ���ڳ�ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Uart_Init(void)
{
	UartStruct_Init();	//���ڹ��ܳ�ʼ��
	UartComInit();	
	API_SetTimer(1,-1,TIMER_TYPE_UART_DMA,DMA_UART_Receive_Process,NULL);
}

//������������������������������������������������������������������������������
// ��������  : ReceiveDataFromUartStruct
// ��������  : �Ӵ����ڶ�ȡһ������
// �������  : us,����ӳ��ṹ��ָ��
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
U8 ReceiveDataFromUartStruct(UartStruct* us)
{
	if(us != NULL)
	{
		UartCom* com =  us->uCom;

		if(com != NULL)
		{
			USART_TypeDef* USARTx =com->USARTx;
			if(USARTx != NULL)
			{
				return (U8)(USART_ReceiveData8(USARTx));
			}
		}
	}
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : SendDataFromUartStruct
// ��������  :�򴮿ڷ���һ������
// �������  : us,����ӳ��ṹ��ָ�� data,�����͵�����
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int SendDataFromUartStruct(UartStruct* us, u8 data)
{
	if(us != NULL)
	{
		UartCom* com =  us->uCom;

		if(com != NULL)
		{
			USART_TypeDef* USARTx =com->USARTx;
			if(USARTx != NULL)
			{
				while(!(USARTx->SR & USART_FLAG_TXE));
				USART_SendData8(USARTx,data);
				return 0;
			}
		}
	}
	return -1;
}

//������������������������������������������������������������������������������
// ��������  : EnableCom
// ��������  :���ƴ��ڵ�ʹ��
// �������  : com,���ڿ��ƽṹ��ָ�� enable,����ʹ�ܷ�
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int EnableCom(UartCom* com,BOOL enable)
{
 	USART_TypeDef* 	USARTx = NULL;
 	if(com == NULL)
	 	return -1;
	USARTx =com->USARTx;
	if(USARTx != NULL)
	{
		USART_TypeDef* USARTx =com->USARTx;
		if(enable)
		{
			USART_ITConfig(USARTx,USART_IT_TXE,ENABLE);
		} else {
			USART_ITConfig(USARTx,USART_IT_TXE,DISABLE);
		}
	}
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : SetupComDir
// ��������  :���ƴ��ڵ�DIR����
// �������  : us,����ӳ��ṹ��ָ��
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int SetupComDir(UartStruct* us,BOOL State)
{
 	UsartTypeToFunctionStruct* uTypeToFun;
 	if(us == NULL)
	 	return -1;
	uTypeToFun = us->uTypeToFun;

	if(uTypeToFun != NULL) 
	{
		GPIO_TypeDef *Gp = NULL;
		uint16_t Pin = NULL;

		switch(uTypeToFun->usartType)
		{
		case USART_TYPR_COM1:
//			Pin = GPIO_Pin_11;
			Gp = GPIOA;
			break;
		case USART_TYPR_COM2:
//			Pin = GPIO_Pin_9;
			Gp = GPIOB;
			break;
		case USART_TYPR_COM3:
//			Pin = GPIO_Pin_10;
			Gp = GPIOD;
			break;
//		case USART_TYPR_COM4:
//			Pin = GPIO_Pin_0;
//			Gp = GPIOD;
//			break;
//		case USART_TYPR_COM5:	
//			Pin = GPIO_Pin_4;
//			Gp = GPIOD;
//			
//			break;
		default:
			return -1;
		}
		if(State)
		{
			Delay_MS(1);
			GPIO_ResetBits(Gp,Pin);
		}
		else
		{
			Delay_MS(1);
			GPIO_SetBits(Gp,Pin);
		}
	}
	return 0; 
}
/*******************************************************************************
* Function Name  : fputc
* Description    : Retargets the C library printf function to the USART.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

#if DEBUG_MODE
int fputc(int ch, FILE *f)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_11);
	/* Place your implementation of fputc here */
    /* e.g. write a character to the USART */
    while(USART_GetFlagStatus(CONSOLE_UART, USART_FLAG_TC) == RESET){
    }
    if(ch=='\n')
        fputc('\r',f);
    USART_SendData(CONSOLE_UART, (u8)ch);
    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(CONSOLE_UART, USART_FLAG_TC) == RESET){
    }
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
    return ch;
}

/*******************************************************************************/
void send_char(char ch)
{
    
	USART_SendData(CONSOLE_UART, (u8) ch);
    while(USART_GetFlagStatus(CONSOLE_UART, USART_FLAG_TC) == RESET);
}

/*******************************************************************************/
char get_char(void)
{
    while(!(USART_GetFlagStatus(CONSOLE_UART, USART_FLAG_RXNE) == SET)){
    }
    return (USART_ReceiveData(CONSOLE_UART));
}
#endif
//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������

