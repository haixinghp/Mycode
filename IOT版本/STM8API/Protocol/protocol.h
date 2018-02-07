#ifndef PROTOCOL_H
#define  PROTOCOL_H
#include "config.h"
//#include "stm8l15x.h"
//#define MAX_NET_IS_OPEN_NUM		200L
/*********************************************************************************************/
typedef enum {
  RESOURCE_USE_STATUS_FREE, 			//����
  RESOURCE_USE_STATUS_RECEIVE,		//����
  RESOURCE_USE_STATUS_SEND,			//����
  RESOURCE_USE_STATUS_MUTSEND,		//��η���
  RESOURCE_USE_STATUS_END,			//���ͽ������
  RESOURCE_USE_STATUS_MAX		    //����
}RESOURCE_USE_STATUS;

/*********************************************************************************************/
typedef enum {
  PORT_TYPE_SERIAL,					//����
  PORT_TYPE_WCDMA,					//����ģʽ
  PORT_TYPE_MAX 		
}PORT_TYPE;
typedef enum {
  MODE_TYPE_NONE,					//��ģ��
  MODE_TYPE_ZTE,					//NBģ��
  MODE_TYPE_BC95,					//����ģ�飬��Ϊ�ҵ�ģ���ƶ���ͨ���ż��ݣ�������ģ��Ϊʶ��
  MODE_TYPE_MAX 		
}MODE_TYPE;

typedef enum {
  CONNECT_STATE_NONE,					//������
  CONNECT_STATE_OK,					//����OK
  CONNECT_STATE_LINK,					//������
  CONNECT_STATE_MAX 		
}CONNECT_STATE;
/*********************************************************************************************/
typedef enum {
  PROTOCOL_RX_MODE_ANALYSIS,		   //Э�����
  //	PROTOCOL_RX_MODE_DIRECT, 		   //͸��		
}PROTOCOL_RX_MODE;
/*********************************************************************************************/
//���ö�����һ��Э�������һ��Э��
#define MAX_HEAD_BYTES 4  				//Э��ͷ�ַ�������
#define MAX_TAIL_BYTES 4  				//Э��β�ַ�������

#define SIGN_PROTOCAL_WITH_NO_HEAD	0xFF	  	//��ͷ��־
#define SIGN_PROTOCAL_WITH_NO_TAIL	0xFF	 	//��β��־
/*********************************************************************************************/
typedef struct
{	u8 RHLenth;						//ͷ����
u8 RHead[MAX_HEAD_BYTES];		//ͷ�ַ���
u8 RSLenth;						//Э����̳���--->��ҪΪ�˷�ֹ��Э��ͷ�г�����ͷ�ַ�����β�ַ������Ƶ�ֵ������
u8 RTLenth;						//β����
u8 RTail[MAX_TAIL_BYTES];		//β�ַ���
PROTOCOL_TYPE ProtcolID;			//��Э���Ӧ��Э������,�磺ID==0��˾Э��,==1���ʵ粿Э�飬==����
}PINFO;					    			//��PC����Э��ṹ
/*********************************************************************************************/
typedef struct
{	u8 RHLenth;						//ͷ����
u8 RHead[MAX_HEAD_BYTES+4];		//ͷ�ַ���
u8 RSLenth;						//Э����̳���--->��ҪΪ�˷�ֹ��Э��ͷ�г�����ͷ�ַ�����β�ַ������Ƶ�ֵ������
u8 RTLenth;						//β����
u8 RTail[MAX_TAIL_BYTES+4];		//β�ַ���
PROTOCOL_TYPE ProtcolID;			//��Э���Ӧ��Э������,�磺ID==0��˾Э��,==1���ʵ粿Э�飬==����
}PINFO_BC95;					    			//��PC����Э��ṹ
/*********************************************************************************************/
typedef int (*ProtocolFun)(void);		//���ڷ��ͺͽ��պ���

/*********************************************************************************************/
typedef struct 
{	uint16_t SendLen;
uint8_t *pSendBuf;	
uint8_t  SendNum;
uint8_t  SendFlag;		
}NetSendNum;

extern NetSendNum gNetSend;				//����ְ����ͱ�־

/*********************************************************************************************/
//ͨ�Ŷ˿�Э����Դ�ṹ
typedef  struct
{	
  u8 RxMode;						//����ģģʽ: ��Э����� ͸��
  int size;							//�շ������С
  u8* RxMessageBuffer;				//���ջ�����
  u8 *PRxDaBuf;					//��������ָ��					
  u8* TxMessageBuffer; 			//���ͻ�����
  u8 *PTxDaBuf;				   	//��������ָ��
  u16 RxPro_Lenth;	        	   	//ͳ�ƽ����ֽڳ���
  u16 TxPro_Lenth;	        	   	//ͳ�Ʒ����ֽڳ���
  RESOURCE_USE_STATUS ResState;		//��Դʹ��״̬ =0.���� =1.���ڽ������� =2.���ڷ������� =3.׼����η������� =4.һ��Э������������׼�����
  PROTOCOL_TYPE Pro_Type;        	   	//Э������  =0.�Ž�Э�� =1.�ʵ粿Э�� =2.��˾Э�� =3.DL/T645-2007Э��
  PORT_TYPE Port_Type;		    	//ͨ�Ŷ˿� =0.Ϊ���� =1.Ϊ����                  
  MODE_TYPE Mode_Type;					//ģ��ѡ��MODE_TYPE_ZTE=0,MODE_TYPE_BC95=1ѡ��
  CONNECT_STATE Connect_State;	//����״̬
}ProtocolResource;
/*********************************************************************************************/
#define Init_Protocol(Protocol)		{	(Protocol)->RxPro_Lenth = 0;	\
(Protocol)->TxPro_Lenth = 0;	\
  (Protocol)->Pro_Type = PROTOCOL_TYPE_NONE;	\
    (Protocol)->ResState = RESOURCE_USE_STATUS_FREE;	\
									}

#define Reset_Protocol(Protocol)    {	Init_Protocol(Protocol);			\
(Protocol)->PRxDaBuf = (u8*)((Protocol)->RxMessageBuffer);	\
  (Protocol)->PTxDaBuf = (u8*)((Protocol)->TxMessageBuffer);	\
									}



#define Reset_Protocol_Rx(Protocol) {	(Protocol)->RxPro_Lenth = 0;	\
(Protocol)->PRxDaBuf = (u8*)((Protocol)->RxMessageBuffer);	\
									}

#define Reset_And_Init_Protocol_Rx(Protocol)  { Reset_Protocol_Rx(Protocol);	\
(Protocol)->Pro_Type = PROTOCOL_TYPE_NONE;	\
  (Protocol)->ResState = RESOURCE_USE_STATUS_FREE;	\
											}	

#define Reset_Protocol_Tx(Protocol) {	(Protocol)->TxPro_Lenth = 0;	\
(Protocol)->PTxDaBuf = (u8*)((Protocol)->TxMessageBuffer);	\
									}

#define Init_ProSendPrt(Protocol)   {	(Protocol)->PTxDaBuf = (u8*)((Protocol)->TxMessageBuffer);  \
									}
/*********************************************************************************************/
#include "Protocol_debug.h"
#include "Protocol_wcdma.h"
/*********************************************************************************************/
const PINFO* getPINFO(PROTOCOL_TYPE pt);	  //��ȡЭ�����Ϣ
//ע��һ��Э�����ز���
int RegisterProtocolFun(USART_FUNCTION uf,ProtocolFun send,ProtocolFun recv,ProtocolFun doProtocol,ProtocolResource* pRes);
int ProtocolSendByte(USART_FUNCTION uf);  //��һ��Э�鷢��һ���ַ�
int ProtocolReceiveByte(USART_FUNCTION uf);	//����һ���ַ�
int DoProtocolReceiveAnalysis(ProtocolResource *Protocol,u8 ChrData);	//�������չ��������ݵ�Э��
int DoProtocol_DMA_ReceiveAnalysis(ProtocolResource *Protocol,int len);
int DoProtocol_DMA_ReceiveAnalysis(ProtocolResource *Protocol,int len); 	
int ReceiveCallBack(USART_FUNCTION uf,PROTOCOL_TYPE Pro_Type,u8* buffer, size_t size);

ProtocolResource * GetProtocolResource(USART_FUNCTION uf);	

/*�ⲿAPI���ÿ�ʼ*/
/*Э���ʼ��*/
void Protocols_Init(void);			
/*��ѭ������õ�ִ������Э�飬���ڷ������չ���������*/	
int DoProtocols(void);
/*��������*/	
int SendProtocolBuffer(USART_FUNCTION uf,u8* buffer, size_t size);
/*��λ�շ�����*/
int ResetProtocolBuffer(USART_FUNCTION uf,RESETRTBUFFTYPE ResetType);
/*��ָ����Э�����ͻ�����ӷ�������*/
int AddProtocolBuffer(USART_FUNCTION uf,u8* buffer, size_t size);
/*��ȡָ����Э�黺��ṹ*/
int GetProtocolBufferOrLen(USART_FUNCTION uf,u8 **ptr,uint16 **len);
/*ָ����Э������������������*/
int StartSendBuffer(USART_FUNCTION uf);
/*�������ݽ���*/	
void NetDeal(void);
/*���ý��ջص�����*/
int SetProtocolReceiveCallBack(USART_FUNCTION uf,ReceiveFun fun);

/*���ڲ�������*/
int SetUartParameter(
                     USART_FUNCTION uf,					//����Э��
                     uint32_t baudRate,	 				//������
                     uint16_t parity,					//У��λ
                     uint16_t wordLength,				//���ݳ���
                     uint16_t stopBits					//ֹͣλ
                       );
/*�ⲿAPI���ý���*/
/*********************************************************************************************/
#endif
