#include "config.h"
#include "USART.h"

/*********************************************************************************************/
/* 内部函数区域声明 */
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
#define     MPDevVer            2.0	 //程序版本

/*********************************************************************************************/
/*
停止位可用参数
#define USART_StopBits_1                     ((uint16_t)0x0000)
#define USART_StopBits_0_5                   ((uint16_t)0x1000)
#define USART_StopBits_2                     ((uint16_t)0x2000)
#define USART_StopBits_1_5                   ((uint16_t)0x3000)

校验位可用参数
#define USART_Parity_No                      ((uint16_t)0x0000)
#define USART_Parity_Even                    ((uint16_t)0x0400)
#define USART_Parity_Odd                     ((uint16_t)0x0600) 

 数据长度可用参数
#define USART_WordLength_8b                  ((uint16_t)0x0000)
#define USART_WordLength_9b                  ((uint16_t)0x1000)

硬件流模式
#define USART_HardwareFlowControl_None       ((uint16_t)0x0000)
#define USART_HardwareFlowControl_RTS        ((uint16_t)0x0100)
#define USART_HardwareFlowControl_CTS        ((uint16_t)0x0200)
#define USART_HardwareFlowControl_RTS_CTS    ((uint16_t)0x0300)

模式
#define USART_Mode_Rx                        ((uint16_t)0x0004)
#define USART_Mode_Tx                        ((uint16_t)0x0008)
*/
#define USART_HardwareFlowControl_None 0
/*********************************************************************************************/
//映射关系建立

/*********************************************************************************************/
//#if BUILD_PROJECT == BOOT_HD

	static UartCom _gUartComArr[USART_TYPR_MAX]	=
	{
	//	{串口初始化函数	串口中断响应函数 	地址	波特率	校验位				数据长度				停止位				硬件流模式						模式						}
		{USART1_Init,	USART1_IRQHandler,	USART1,	{9600,	USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}},//串口1
		{USART2_Init,	USART2_IRQHandler,	USART2,	{9600,  USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}},//串口2
		{USART3_Init,	USART3_IRQHandler,	USART3,	{9600,	USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}},//串口3
//		{USART4_Init,	UART4_IRQHandler,	UART4,	{9600,	USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}},//串口4
//		{USART5_Init,	UART5_IRQHandler,	UART5,	{9600,	USART_Parity_No,	USART_WordLength_8b,	USART_StopBits_1,	USART_HardwareFlowControl_None,	USART_Mode_Rx|USART_Mode_Tx}} //串口5
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
static UartStruct _gUartStuct[USART_TYPR_MAX] = { 				//串口初始化，按照串口地理顺序排列
 	0
};

static UartStruct* _gUartStuctBaseFun[USART_FUNCTION_MAX] = { 	//串口按照功能进行排列
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
	uint8_t     USART_BufOk;    			      //BUF是否已经可用 
    __IO int HALDMAUART_OverTime ;  		      //DMA接收超时计数      
}UARTDMA_CFG;

/*串口DMA初始化参数*/
//DMA_InitTypeDef DMA_InitStructure[USART_TYPR_MAX];

/*DMA串口接收数据超时时间*/
#define DMA_USATR_RX_TIMEOUT  20

/*DMA接收数据状态、及缓存结构*/
UARTDMA_CFG	_gucfg[USART_TYPR_MAX] = {
/*--------------------------------------*/
/*  缓存满标志        超时   	        */
	{false, 	DMA_USATR_RX_TIMEOUT	},
	{false, 	DMA_USATR_RX_TIMEOUT	},
	{false, 	DMA_USATR_RX_TIMEOUT	},
//	{false, 	DMA_USATR_RX_TIMEOUT	},
//	{false, 	DMA_USATR_RX_TIMEOUT	},
};

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DMA_UsartConfiguration
// 功能描述  : 串口接收数据DMA设置
// 输入参数  : df DMA通道; ut 串口编号 
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※


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
  
  /************ UTRA初始化 ***************************/
  GPIO_Init(GPIOE, GPIO_Pin_4, GPIO_Mode_Out_PP_High_Slow); //USART2_TX
  GPIO_Init(GPIOE, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT); //USART2_RX
  CLK_PeripheralClockConfig(CLK_Peripheral_USART2, ENABLE); //一定要打开，上电默认是关闭，8S是默认打开
  USART_DeInit(_gUartComArr[ut].USARTx);
//  USART_Init(USART2, BaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx);  
  USART_Init(_gUartComArr[ut].USARTx, BaudRate, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,  (USART_Mode_TypeDef)(USART_Mode_Rx | USART_Mode_Tx));  //??
//  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//关闭接收中断
//  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//打开串口2接收中断 
  USART_ClearFlag(USART2, USART_FLAG_RXNE);//清楚接收中断标志
// USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//打开串口2空闲中断 //TEST HX  2017-05-25 09:33:26
//  SetUart3RxAsExInt();
   CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);//</span>    //打开时钟，很重要  
    /* Deinitialize DMA channels */    
    DMA_GlobalDeInit();
    DMA_DeInit(DMA1_Channel3);  
//    DMA_DeInit(DMA1_Channel3);  
      
    /* DMA channel Rx of USART Configuration */  
    //该函数主要要配置好接受的数组，以及USART的数据寄存器地址，数组大小，以及DMA模式  
    DMA_Init(DMA1_Channel3,(uint32_t)(&Protocol->RxMessageBuffer), (uint16_t)(&(_gUartComArr[ut].USARTx->DR)),(u8)(Protocol->size), DMA_DIR_PeripheralToMemory, DMA_Mode_Circular, DMA_MemoryIncMode_Inc, DMA_Priority_Medium, DMA_MemoryDataSize_Byte);  
   
    /* DMA channel Tx of USART Configuration */  
    //该函数主要配置发送数组，以及USART的数据寄存器地址，数组大小，以及DMA模式  
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
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DMA_Usart_Init
// 功能描述  : 串口DMA接收初始化
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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
		case USART_TYPR_COM1:		  	//串口1
			/*开启DMA1时钟*/
                        CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
                        
			break;		 
		case USART_TYPR_COM2:	   		//串口2
			/*开启DMA1时钟*/
                        CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
                  
			break;
		case USART_TYPR_COM3:		   	//串口3
			/*开启DMA1时钟*/
                        CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);
			break;
//		case USART_TYPR_COM4:		   	//串口4
//			/*开启DMA2时钟*/
//			RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
//			irqtype = DMA2_Channel3_IRQn;	
//			df = DMA2_Channel3;
//			break;
//		case USART_TYPR_COM5:		   	//串口5
//			break;
	}

	if(df == NULL)
		return -1;

	/* DMA初始化*/
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


//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DMA_UART_Receive
// 功能描述  : 串口DMA通道中断接收数据
// 输入参数  : df DMA通道; ut 串口编号 
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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
//		case DMA1_Channel1_BASE:	 /*串口1 DMA接收*/
//			 ITStatus_TC = DMA1_IT_TC1;
//	         //ITStatus_GL = DMA1_IT_GL5;
//			break;
//		case DMA1_Channel2_BASE:	 /*串口2 DMA接收*/
//			 ITStatus_TC = DMA1_IT_TC2;
//	         //ITStatus_GL = DMA1_IT_GL6;
//			break;
//		case DMA1_Channel3_BASE:	 /*串口3 DMA接收*/
//			 ITStatus_TC = DMA1_IT_TC3;
//	         //ITStatus_GL = DMA1_IT_GL3;
//			break;
////		case (uint32_t)DMA2_Channel3:	 /*串口4 DMA接收*/
////			 ITStatus_TC = DMA2_IT_TC3;
////	         ITStatus_GL = DMA2_IT_GL3;
////			break;
//	}
//	
//	/*通道传输完成中断TC 还有传输 过半中断HT  错误中断TE  全局中断GL*/
//	if(DMA_GetITStatus(ITStatus_TC))  
//	{
//		/*清除全部中断标志*/
//		DMA_ClearITPendingBit(ITStatus_GL);    
//         
//		DMA_Init(DMA1_Channel3,(uint32_t)(&Protocol->RxMessageBuffer), (uint16_t)(&(_gUartComArr[ut].USARTx->DR)),(u8)(Protocol->size), DMA_DIR_PeripheralToMemory, DMA_Mode_Circular, DMA_MemoryIncMode_Inc, DMA_Priority_Medium, DMA_MemoryDataSize_Byte);  
//   
////		def->DMA_MemoryBaseAddr = (uint32_t)(Protocol->RxMessageBuffer);   
////		def->DMA_PeripheralBaseAddr = (uint32_t)(&(_gUartComArr[ut].USARTx->DR));
////		def->DMA_Mode = DMA_Mode_Circular;                                //工作在循环缓存模式
////		def->DMA_DIR = DMA_DIR_PeripheralSRC;
////		def->DMA_BufferSize = Protocol->size;
////		DMA_Init(df,def);
//
//		ucfg->USART_BufOk = true; //有准备好的数据了		
//	}

	return 0;
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART1_Timeout
// 功能描述  : 串口接收超时处理函数
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void USART1_Timeout(void)
{
	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM1);
//	Reset_Protocol(Protocol);
//        Reset_Protocol_Rx(Protocol); // 只复位接收缓存，by hx 2017-11-13
        Reset_And_Init_Protocol_Rx(Protocol); //by hx 2017-12-16
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART2_Timeout
// 功能描述  : 串口接收超时处理函数
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void USART2_Timeout(void)
{
	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM2);
//	Reset_Protocol(Protocol);
//        Reset_Protocol_Rx(Protocol); // 只复位接收缓存，by hx 2017-11-13
        Reset_And_Init_Protocol_Rx(Protocol); //by hx 2017-12-16
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART3_Timeout
// 功能描述  : 串口接收超时处理函数
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void USART3_Timeout(void)
{
	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM3);
//	Reset_Protocol(Protocol);
//        Reset_Protocol_Rx(Protocol); // 只复位接收缓存，by hx 2017-11-13
        Reset_And_Init_Protocol_Rx(Protocol); //by hx 2017-12-16
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART4_Timeout
// 功能描述  : 串口接收超时处理函数
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
//void USART4_Timeout(void)
//{
//	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM4);
//	Reset_Protocol(Protocol);
//}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART5_Timeout
// 功能描述  : 串口接收超时处理函数
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
//void USART5_Timeout(void)
//{
//	ProtocolResource *Protocol = GetUtProtocolResource(USART_TYPR_COM5);
//	Reset_Protocol(Protocol);
//}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DMA_USART_Timeout
// 功能描述  : 串口接收超时设置
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void DMA_USART_Timeout(USART_TYPE ut)
{
	USART_FUNCTION ufun = Get_Usart_Function(ut);

	if((ut<USART_TYPR_MAX)&&(ufun<USART_FUNCTION_MAX))
	{
		switch(ut)
		{
			case USART_TYPR_COM1:		  	//串口1
				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART1_Timeout,NULL);
				break;		 
			case USART_TYPR_COM2:	   		//串口2
				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART2_Timeout,NULL);
				break;
			case USART_TYPR_COM3:		   	//串口3
				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART3_Timeout,NULL);
				break;
//			case USART_TYPR_COM4:		   	//串口4
//				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART4_Timeout,NULL);
//				break;
//			case USART_TYPR_COM5:		   	//串口5
//				API_SetTimer(200,1,(TIMER_TYPE)ufun,USART5_Timeout,NULL);
//				break; 		
		}
	}	
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DMA_UART_Receive_Process
// 功能描述  : 处理DMA接收串口数据
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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
//			case USART_TYPR_COM1:		  	//串口1
//				df = DMA1_Channel1;
//				break;		 
//			case USART_TYPR_COM2:	   		//串口2
//				df = DMA1_Channel2;
//				break;
//			case USART_TYPR_COM3:		   	//串口3
//				df = DMA1_Channel3;
//				break;
////			case USART_TYPR_COM4:		   	//串口4
////				df = DMA2_Channel3;
////				break;
////			case USART_TYPR_COM5:		   	//串口5
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
//			gucfg->USART_BufOk = false; //操作了准备好的数据
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

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DMA1_Channel5_IRQHandler
// 功能描述  : 串口1 DMA接收中断
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void DMA1_Channel1_IRQHandler(void)
{ 
	DMA_UART_Receive(DMA1_Channel1,USART_TYPR_COM1);	
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DMA1_Channel6_IRQHandler
// 功能描述  : 串口2 DMA接收中断
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void DMA1_Channel2_IRQHandler(void)
{ 
	DMA_UART_Receive(DMA1_Channel2,USART_TYPR_COM2);	
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DMA1_Channel3_IRQHandler
// 功能描述  : 串口3 DMA接收中断
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void DMA1_Channel3_IRQHandler(void)
{ 
	DMA_UART_Receive(DMA1_Channel3,USART_TYPR_COM3);	
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DMA2_Channel3_IRQHandler
// 功能描述  : 串口4 DMA接收中断
// 输入参数  : None
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
//void DMA2_Channel3_IRQHandler(void)
//{ 
////	DMA_UART_Receive(DMA2_Channel3,USART_TYPR_COM4);	
//}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : SettingSave
// 功能描述  : 串口设置
// 输入参数  : uCom,串口控制结构体指针
// 输出参数  : None								  
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
static void SettingSave(UartCom* uCom)
{
	USART_TYPE ut = USART_TYPR_MAX;
	if(uCom != NULL)
	{
          
                CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); //一定要打开，上电默认是关闭，8S是默认打开
                USART_DeInit(uCom->USARTx);
                USART_Init(uCom->USARTx, uCom->UartCommAag.baudRate, uCom->UartCommAag.wordLength, uCom->UartCommAag.stopBits, USART_Parity_No,
                           USART_Mode_Rx | USART_Mode_Tx);
                USART_ITConfig(uCom->USARTx, USART_IT_RXNE, ENABLE);
                USART_Cmd(uCom->USARTx, ENABLE);
                
//		USART_TypeDef* USARTx = uCom->USARTx;
//		USART_InitTypeDef USART_InitStructure;
//		USART_ClockInitTypeDef  USART_ClockInitStructure;
	
		// USART 初始化  
//		USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
//		USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
//		USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//		USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;	
//		USART_ClockInit(USARTx, &USART_ClockInitStructure);
//		 
//		USART_Cmd(USARTx, DISABLE);
//		USART_InitStructure.USART_BaudRate =uCom->UartCommAag.baudRate;
//	
//		//帧停止位，USART_StopBits_1：1个停止位，USART_StopBits_0.5：0.5个停止位 USART_StopBits_2：2个停止位，USART_StopBits_1.5：1.5个停止位
//		USART_InitStructure.USART_StopBits = uCom->UartCommAag.stopBits;
//		//定义奇偶模式，USART_Parity_No：奇偶失能，USART_Parity_Even：偶模式USART_Parity_Odd：奇模式
//		//由于STM32定义为奇偶校验时，需要设置数据位为9位，无校验时用8位数据
//
//		USART_InitStructure.USART_Parity = uCom->UartCommAag.parity;
//		USART_InitStructure.USART_WordLength = uCom->UartCommAag.wordLength;
//		USART_InitStructure.USART_HardwareFlowControl = uCom->UartCommAag.hardwareFlowControl;
//		//使能发送和接收
//		USART_InitStructure.USART_Mode = uCom->UartCommAag.mode;
//		USART_Init(USARTx, &USART_InitStructure);
//		//USART 使能开中断 
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
				USART_ClearITPendingBit(uCom->USARTx,USART_IT_RXNE);  //清串口接收中断标志 
				USART_ITConfig(uCom->USARTx,USART_IT_RXNE, ENABLE);   //使能串口接收中断
			}
		}
	}
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : SettingSaveByUartStruct
// 功能描述  : 串口设置
// 输入参数  : us,映射控制结构体指针
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

static void SettingSaveByUartStruct(UartStruct* uS)
{
	if(uS != NULL)
	{
		 SettingSave(uS->uCom);		 
	}
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : SettingSaveByUSART_TYPE
// 功能描述  : 串口设置
// 输入参数  : ut,串口号
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
static void SettingSaveByUSART_TYPE(USART_TYPE ut)
{
	if(ut < USART_TYPR_MAX)
	{
		 SettingSaveByUartStruct(&_gUartStuct[ut]);
	}
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : SettingSaveByUSART_FUNCTION
// 功能描述  : 串口设置
// 输入参数  : uf,串口的功能
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void SettingSaveByUSART_FUNCTION(USART_FUNCTION uf)
{
	if(uf < USART_FUNCTION_MAX)
	{
		 SettingSaveByUartStruct(_gUartStuctBaseFun[uf]);
	}
}
	
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : ReadSettingByUSART_TYPE
// 功能描述  : 从EEROM内读取设定值
// 输入参数  : ut,串口号
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
static void ReadSettingByUSART_TYPE(USART_TYPE ut)
{
	if(ut < USART_TYPR_MAX)
	{
	}
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART1_Init
// 功能描述  : 串口1初始化
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
static void USART1_Init(void)
{							  
#if  CONF_UART1_EN

  
  
    // Configure USART Tx as alternate function push-pull  (software pull up)
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_3, ENABLE);

    // Configure USART Rx as alternate function push-pull  (software pull up)
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_2, ENABLE);
    
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Mode_Out_PP_High_Slow);//串口1发送
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_No_IT);//串口1接收
//  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);//串口1发送
//  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Mode_In_FL_IT);//串口1接收
  CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE); //一定要打开，上电默认是关闭，8S是默认打开
  USART_DeInit(USART1);
  USART_Init(USART1, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No,
             USART_Mode_Rx | USART_Mode_Tx);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART1, ENABLE);
	/*对串口参数进行设置开始*/

	/*对串口参数进行设置结束*/
	ReadSettingByUSART_TYPE(USART_TYPR_COM1);  //从EEROM内读取串口设置参数
	SettingSaveByUSART_TYPE(USART_TYPR_COM1);

#endif
	/********************************************************************************/	
}



//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART2_Init
// 功能描述  : 串口2初始化
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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
  
  // 清串口接收中断标志
  USART_ClearITPendingBit(USART2,USART_IT_RXNE);     
  
  
  
	ReadSettingByUSART_TYPE(USART_TYPR_COM2);  //从EEROM内读取串口设置参数
	SettingSaveByUSART_TYPE(USART_TYPR_COM2);

#endif
	/********************************************************************************/
}
 //※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART3_Init
// 功能描述  : 串口3初始化
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

static void USART3_Init(void)
{
#if CONF_UART3_EN
  
 
  GPIO_Init(GPIOG, GPIO_Pin_1, GPIO_Mode_Out_PP_High_Slow);
  GPIO_Init(GPIOG, GPIO_Pin_0, GPIO_Mode_In_PU_No_IT); //串口3
  CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE); //一定要打开，上电默认是关闭，8S是默认打开
  USART_DeInit(USART3);
  USART_Init(USART3, 9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, USART_Mode_Rx | USART_Mode_Tx);
  USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//关闭接收中断
//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//打开串口3接收中断
//  SetUart3RxAsExInt();
  USART_Cmd(USART3, ENABLE);
	/********************************************************************************/
	ReadSettingByUSART_TYPE(USART_TYPR_COM3);  //从EEROM内读取串口设置参数
	SettingSaveByUSART_TYPE(USART_TYPR_COM3);
	/********************************************************************************/ 
        
#endif
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : DoProtocol
// 功能描述  : 执行协议
// 输入参数  : ut,串口号 isSend,是否是发送数据
// 输出参数  : None
// 返回参数  : 0成功,-1失败
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

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

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART1_IRQHandler
// 功能描述  : 串口1中断请求
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

void USART1_IRQHandler(void)
{

#if CONF_UART1_EN
   if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断    
  {USART_ClearFlag(USART1, USART_FLAG_RXNE);
    Protocol_Receive();
    //DoProtocol(USART_TYPR_COM1,false);
//    DATA_Buf = USART1->DR;
//    qwl_sendchar(DATA_Buf);
    
  }
//	if( USART_GetITStatus(USART1,USART_IT_RXNE) )
//	{
//        USART_ClearITPendingBit(USART1,USART_IT_RXNE);  //清串口接收中断标志
//	   	DoProtocol(USART_TYPR_COM1,false);
//	}
//
//	/* 假如溢出条件满足，清除该标志，并恢复通信 */
//	if (USART_GetFlagStatus(USART1, USART_FLAG_OR) == SET)
//	{
//		USART_ClearFlag(USART1,USART_FLAG_OR);                    
//		USART_ReceiveData8(USART1);
//	}
//
//	if( USART_GetITStatus(USART1,USART_IT_TXE) )
//	{	
//        USART_ClearITPendingBit(USART1,USART_IT_TXE);   //清串口接收中断标志
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
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);  //清串口接收中断标志
		DoProtocol(USART_TYPR_COM2,false);   
	}

	/* 假如溢出条件满足，清除该标志，并恢复通信 */
//	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) == SET)
//	{
//		USART_ClearFlag(USART2,USART_FLAG_ORE);                    
//		USART_ReceiveData(USART2);
//	}

	if( USART_GetITStatus(USART2,USART_IT_TXE) )
	{	
        USART_ClearITPendingBit(USART2,USART_IT_TXE);   //清串口接收中断标志
		DoProtocol(USART_TYPR_COM2,true);						
	}
#endif	
}

  
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : USART3_IRQHandler
// 功能描述  : 串口3中断请求
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void USART3_IRQHandler(void)
{
#if CONF_UART3_EN
	if( USART_GetITStatus(USART3,USART_IT_RXNE) )
	{		
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);  //清串口接收中断标志
		DoProtocol(USART_TYPR_COM3,false);   
	}

	/* 假如溢出条件满足，清除该标志，并恢复通信 */
	if (USART_GetFlagStatus(USART3, USART_FLAG_OR) == SET)
	{
		USART_ClearFlag(USART3,USART_FLAG_OR);                    
		USART_ReceiveData8(USART3);
	}

	if( USART_GetITStatus(USART3,USART_IT_TXE) )
	{	
        USART_ClearITPendingBit(USART3,USART_IT_TXE);   //清串口接收中断标志
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
//        USART_ClearITPendingBit(UART4,USART_IT_RXNE);  //清串口接收中断标志
//		DoProtocol(USART_TYPR_COM4,false);	  
//	}
//
//	/* 假如溢出条件满足，清除该标志，并恢复通信 */
//	if (USART_GetFlagStatus(UART4, USART_FLAG_ORE) == SET)
//	{
//		USART_ClearFlag(UART4,USART_FLAG_ORE);                    
//		USART_ReceiveData(UART4);
//	}
//
//	if( USART_GetITStatus(UART4,USART_IT_TXE) )
//	{	
//        USART_ClearITPendingBit(UART4,USART_IT_TXE);   //清串口接收中断标志
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
//        USART_ClearITPendingBit(UART5,USART_IT_RXNE);  //清串口接收中断标志
//		DoProtocol(USART_TYPR_COM5,false); 
//	}
//	/* 假如溢出条件满足，清除该标志，并恢复通信 */
//	if (USART_GetFlagStatus(UART5, USART_FLAG_ORE) == SET)
//	{
//		USART_ClearFlag(UART5,USART_FLAG_ORE);                    
//		USART_ReceiveData(UART5);
//	}
//
//	if( USART_GetITStatus(UART5,USART_IT_TXE))
//	{	
//        USART_ClearITPendingBit(UART5,USART_IT_TXE);   //清串口接收中断标志
//		DoProtocol(USART_TYPR_COM5,true);							
//	}
//#endif	
//}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : UartComInit
// 功能描述  : 根据串口和功能映射关系，对串口进行初始化.
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
static void UartComInit()
{
	int i = 0;
	for(i = 0; i< 3;i++)
	{
		if(_gUartComArr[i].init != NULL)
		{
			_gUartComArr[i].init();	//初始化串口
		}
	}
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : UartStruct_Init
// 功能描述  : 串口映射关系的初始化
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : Uart_Init
// 功能描述  : 串口初始化
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void Uart_Init(void)
{
	UartStruct_Init();	//串口功能初始化
	UartComInit();	
	API_SetTimer(1,-1,TIMER_TYPE_UART_DMA,DMA_UART_Receive_Process,NULL);
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : ReceiveDataFromUartStruct
// 功能描述  : 从串口内读取一个数据
// 输入参数  : us,串口映射结构体指针
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : SendDataFromUartStruct
// 功能描述  :向串口发送一个数据
// 输入参数  : us,串口映射结构体指针 data,待发送的数据
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : EnableCom
// 功能描述  :控制串口的使能
// 输入参数  : com,串口控制结构体指针 enable,串口使能否
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : SetupComDir
// 功能描述  :控制串口的DIR引脚
// 输入参数  : us,串口映射结构体指针
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
//                          程序结束(END)
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

