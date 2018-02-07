#ifndef USART_H
#define USART_H

#include "protocol.h"

/*����ͨ������EEPROM�洢��ַ,����0x0091�ֽ�*/
#define	EEP_ADDR_COMMBPS    _EEP_ADDR_COMMBPS

/*********************************************************************************************/
 /*����ѡ��*/
typedef enum
{
	USART_TYPR_COM1,		  	//����1		 
	USART_TYPR_COM2,	   		//����2
	USART_TYPR_COM3,		   	//����3
//	USART_TYPR_COM4,		   	//����4
//	USART_TYPR_COM5,		   	//����5
   	USART_TYPR_MAX
}USART_TYPE;

typedef struct
{
  uint32_t DMA_PeripheralBaseAddr; /*!< Specifies the peripheral base address for DMAy Channelx. */

  uint32_t DMA_MemoryBaseAddr;     /*!< Specifies the memory base address for DMAy Channelx. */

  uint32_t DMA_DIR;                /*!< Specifies if the peripheral is the source or destination.
                                        This parameter can be a value of @ref DMA_data_transfer_direction */

  uint32_t DMA_BufferSize;         /*!< Specifies the buffer size, in data unit, of the specified Channel. 
                                        The data unit is equal to the configuration set in DMA_PeripheralDataSize
                                        or DMA_MemoryDataSize members depending in the transfer direction. */

  uint32_t DMA_PeripheralInc;      /*!< Specifies whether the Peripheral address register is incremented or not.
                                        This parameter can be a value of @ref DMA_peripheral_incremented_mode */

  uint32_t DMA_MemoryInc;          /*!< Specifies whether the memory address register is incremented or not.
                                        This parameter can be a value of @ref DMA_memory_incremented_mode */

  uint32_t DMA_PeripheralDataSize; /*!< Specifies the Peripheral data width.
                                        This parameter can be a value of @ref DMA_peripheral_data_size */

  uint32_t DMA_MemoryDataSize;     /*!< Specifies the Memory data width.
                                        This parameter can be a value of @ref DMA_memory_data_size */

  uint32_t DMA_Mode;               /*!< Specifies the operation mode of the DMAy Channelx.
                                        This parameter can be a value of @ref DMA_circular_normal_mode.
                                        @note: The circular buffer mode cannot be used if the memory-to-memory
                                              data transfer is configured on the selected Channel */

  uint32_t DMA_Priority;           /*!< Specifies the software priority for the DMAy Channelx.
                                        This parameter can be a value of @ref DMA_priority_level */

  uint32_t DMA_M2M;                /*!< Specifies if the DMAy Channelx will be used in memory-to-memory transfer.
                                        This parameter can be a value of @ref DMA_memory_to_memory */
}DMA_InitTypeDef;
/*����ģʽ*/
typedef enum
{
	USART_MODE_TYPE_IRQ,
	USART_MODE_TYPE_DMA
}USART_MODE_TYPE;
/*********************************************************************************************/
typedef int (*UartFun)(void);			//���ڷ��ͺͽ��պ���
typedef void (*USARTx_Init)(void);		//���ڳ�ʼ����������
typedef void (*USARTx_IRQHandler)(void);//���ڴ�������
/*********************************************************************************************/
/*���ڲ������ƽṹ��*/
typedef struct {
	uint32_t baudRate;	 				//������
	uint8_t parity;					//У��λ
	uint8_t wordLength;				//���ݳ���
	uint8_t stopBits;					//ֹͣλ
	uint8_t hardwareFlowControl;	   	//Ӳ��������
	uint8_t mode;						//ģʽ
}tagUartCommAag;

/*���ڲ������ƽṹ��*/
typedef struct {
	USARTx_Init init;			 		//��ʼ������
	USARTx_IRQHandler irqHandler;	   	//��������
	USART_TypeDef* USARTx;				//��Ӧ�Ĵ��ڵ�ַ
	tagUartCommAag UartCommAag;			//���ڲ���
}UartCom;

/*********************************************************************************************/
/*�����빦��ӳ��*/
typedef struct {
	USART_TYPE usartType;				//�������ͣ�Com1- Com5
	USART_MODE_TYPE mode;				//����ģʽ IRQ DMA
	USART_FUNCTION 	usartFun;			//���ܺ�����WCDMA��
}UsartTypeToFunctionStruct;

/*********************************************************************************************/
/*����ӳ�书�ܽṹ��*/
typedef struct 
{
   	UsartTypeToFunctionStruct* uTypeToFun;		//���ڹ��ܵ�ӳ��	
	UartCom *uCom;								//ֻ��ĳ������������صĲ���
	BOOL ready;									//�Ƿ���� 
}UartStruct;
/*********************************************************************************************/
void Uart_Init(void);
int DMA_Usart_Init(USART_FUNCTION fun);
UartStruct* GetUartStructBaseFunction(USART_FUNCTION fun);	//����Э�����ͻ�ȡ��صĴ���������Ϣ
U8 ReceiveDataFromUartStruct(UartStruct* us);				//�Ӵ��ڽ���һ������
int SendDataFromUartStruct(UartStruct* us, u8 data);  		//�򴮿ڷ���һ������
int EnableCom(UartCom* com,BOOL enable);  					//ʹ�ܻ���ʧЧһ������
int SetupComDir(UartStruct* us,BOOL State);   				//���Ʒ���
void SettingSaveByUSART_FUNCTION(USART_FUNCTION uf);		//���ô��ڲ���
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

int DoProtocol(USART_TYPE ut,BOOL isSend);

/*********************************************************************************************/
#endif


