#ifndef _API_H
#define _API_H

//���صĿͻ�������
#define MOUNT_DEVICE_COUNT	2

/******************************************************************************/
/*���ڹ���ѡ��*/
typedef enum
{
	USART_FUNCTION_WCDMA,
	USART_FUNCTION_MOUNT,
	USART_FUNCTION_MSG = USART_FUNCTION_MOUNT + MOUNT_DEVICE_COUNT,
	USART_FUNCTION_DEBUG,
	USART_FUNCTION_MAX		
}USART_FUNCTION;

/******************************************************************************/
typedef enum {
	PROTOCOL_TYPE_POSTS,				//�ʵ粿Э��
	PROTOCOL_TYPE_COMPANY,				//��˾Э��
	PROTOCOL_TYPE_WIRELESS,				//���ߴ���Э��
	PROTOCOL_TYPE_DOWNLOAD,				//��������Э��
	PROTOCOL_TYPE_MAX,
	PROTOCOL_TYPE_CHECKING,				//�з�����Ҫ��Э�飬���ǻ�����Ҫ�����ж�
	PROTOCOL_TYPE_NONE = 0XFF			//û��Э��
}PROTOCOL_TYPE;

/*******************************************************************************/
/*��λ�շ�����ѡ��*/
typedef enum {
	RESETRTBUFF_TYPE_ALL,
	RESETRTBUFF_TYPE_RX,
	RESETRTBUFF_TYPE_TX,
}RESETRTBUFFTYPE;

/*******************************************************************************/
/*Ψһ��ʱ������*/
typedef enum {
	TIMER_TYPE_Beep = USART_FUNCTION_MAX,    /*��������ʱ��*/
	TIMER_TYPE_UART_DMA,					 /*����DMA���ݴ���*/
	TIMER_TYPE_SETCOMMBPS,					 /*���ô��ڲ�����ʱ*/
	TIMER_TYPE_HORSE_LED,								 /*����ƻص�����*/
	TIMER_TYPE_SEND_DELAY,								 /*���ݷ��͵ȴ�OK��ʱ�ص�����*/
	TIMER_TYPE_IAP_UPDATE_END,				 /*IAP����������ɺ���ʱ����*/
	TIMER_TYPE_IAP_REV_TIMEOUT,				 /*IAP����������ʱ*/
	TIMER_TYPE_IAP_SELECT_PROGRAM,			 /*ѡ������û�����������*/
	TIMER_TYPE_QWL_NET_DEAL,
        TIMER_TYPE_REC_DELAY,
	TIMER_TYPE_MAX 
}TIMER_TYPE;

/*******************************************************************************/

typedef  struct     //�ͻ���ģʽ��Ϣ��
{	vu16 net_type[4];		//ģʽ
	vu32 rip[4];   		    //Զ������ip��ַ
	vu16 rport[4]; 		    //Զ�������˿�
	vu32 reserve[4];        //Ԥ��
}client_struct; 
/*******************************************************************************/
typedef  struct 
{	vu16   Baud;            // 9600
	vu16   Parity;          //������żУ��λ
	vu16   WordLength;      //��������λ
	vu16   StopBits;        //����ֹͣλ
}ComConfigStruct; 
/*******************************************************************************/


/******************************************************************************/
typedef void (*TimerEvent)(void);
typedef int (*ReceiveFun)(USART_FUNCTION uf,PROTOCOL_TYPE Pro_Type,uchar* buffer, size_t size);
/*���Է����ַ���*/
int API_qwl_sendstring(u8 *p);	
/*���Է����ֽ�*/
int API_qwl_sendchar(u8 ch);	


/*��������ʼ��*/
int API_Led_Init(void);
/*FLASH��ʼ��*/
int API_SPI_Flash_Init(void);
/*LED��ʼ��*/
void API_horse_race_lamp(void);
/*API����Ԥ���忪ʼ*/								
/******************************************************************************/
/*���Ź���ʼ��*/
int API_WatchDog_Init(void);
/*ι��*/											
int API_WatchDog_Feed(void);
/*���Ź���ʱ*/								
int API_WatchDog_ClockPro(void);							
/******************************************************************************/
/*Э���ʼ��*/
int API_Protocols_Init(void);
/*��ѭ������õ�ִ������Э�飬���ڷ������չ���������*/
int API_DoProtocols(void);
/*ֱ�ӷ�������*/
int API_SendProtocolBuffer(USART_FUNCTION uf,uchar* buffer, size_t size);
/*��λ�շ�����*/
int API_ResetProtocolBuffer(USART_FUNCTION uf,RESETRTBUFFTYPE ResetType);
/*��ָ����Э�����ͻ�����ӷ�������*/
int API_AddProtocolBuffer(USART_FUNCTION uf,uchar* buffer, size_t size);
/*��ȡָ����Э�鷢�ͻ���ָ��ͳ���*/
int API_GetProtocolBufferOrLen(USART_FUNCTION uf,uchar **ptr,uint16 **len);
/*ָ����Э������������������*/
int API_StartSendBuffer(USART_FUNCTION uf);
/*���ý��ջص�����*/
int API_SetProtocolReceiveCallBack(USART_FUNCTION uf,ReceiveFun fun);

/*���ô��ڲ���*/
int API_SetComParameter(
	USART_FUNCTION uf,					//����Э��
	uint32_t baudRate,	 				//������
	uint16_t parity,					//У��λ
	uint16_t wordLength,				//���ݳ���
	uint16_t stopBits					//ֹͣλ
	); 
/******************************************************************************/
/*��ʱ����ʼ��*/
int API_Timers_Init(void);
/*������ʱ��*/
int API_SetTimer(int time,int circleCount,TIMER_TYPE timerType,TimerEvent tEvent,TimerEvent cEvent);
/*ɾ����ʱ��*/
int API_KillTimer(TIMER_TYPE timerType);
/*������ʱ��*/
int API_StartRunTimer(void);
/******************************************************************************/
/*��ַ��ת����������*/
void API_goto_iap(void);

/*��ȡ�������������*/
int API_Get_Iap_Config(void);
/*IAP�������д*/
int API_IAP_Update_Write(char* buf,size_t size);
/*IAP������¿�ʼ*/
DWORD API_IAP_Update_Start(BOOL force,char* buf,size_t size);
/*IAP������½���*/
int API_IAP_Update_End(void);
/*IAP���򿽱����û���*/
int API_IAP_copyflash(u32 src_add,u32 dest_add,u32 copy_long);
/*IAP���򿽱����û������ⲿflash*/
int API_IAP_copyflash1(u32 src_add,u32 dest_add,u32 copy_long);
/*��ת���û�����*/
int API_Jump_To_UserProgram(void);
/*ѡ������û�����������*/
void API_Select_Program_run(void);
/*�������û������������־*/
void API_Check_Run_User_Program_Falg(void);
/******************************************************************************/

//************************************************************
/*��ѯ����״̬*/
int API_qwl_net_init(void);
int API_net_state(void);
int API_net_rec(u8 ch);	
int API_net_data_send(void); 
int API_net_data_send_init(void);


#endif

