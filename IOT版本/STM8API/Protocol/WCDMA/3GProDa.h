#ifndef _3GPRODA_H_
#define _3GPRODA_H_
#if CONF_WCDMA_EN
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_PROTOCOL_HEADER_SIZE	15
#define MAX_PROTOCOL_TAIL_SIZE	3

typedef enum
{
   PROTOCOL_READ_POS_START,
   PROTOCOL_READ_POS_CENTER,
   PROTOCOL_READ_POS_END,
   PROTOCOL_READ_POS_UNKNOWN
}PROTOCOL_READ_POS;

typedef enum
{
	PROTOCOL_ANALYSIS_TYPE_DEVICE_BEAT,		//0x10 ==>����Ӧ��
	PROTOCOL_ANALYSIS_TYPE_CENTER_REPLY,	//0x20 ==>����Ӧ��
	PROTOCOL_ANALYSIS_TYPE_BEATE_TEST,		//0x30 ==>��������
	PROTOCOL_ANALYSIS_TYPE_SEND_TO_DEVICE,	//0x50 ==>�·����ݵ��豸
	PROTOCOL_ANALYSIS_TYPE_WCDMA_PROGRAM,	//0x52 ==>�·����ݵ�WCDMA
	PROTOCOL_ANALYSIS_TYPE_WCDMA_CTRL,	//0x54 ==>�·����ݵ�WCDMA
	PROTOCOL_ANALYSIS_TYPE_SEND_TO_CENTER,	//0x60 ==>���͵�����
	PROTOCOL_ANALYSIS_TYPE_WCDMA_PROGRAM_REPLY,	//0x62 ==>�����Ļ�Ӧ
	PROTOCOL_ANALYSIS_TYPE_WCDMA_CTRL_REPLY,	//0x64 ==>���߿��ƽ����Ļ�Ӧ
	PROTOCOL_ANALYSIS_TYPE_UNKNOWN,			//δ֪��Э��
	PROTOCOL_ANALYSIS_TYPE_MAX
}PROTOCOL_ANALYSIS_TYPE;

#define MAX_RECEIVE_BUFFER	1024

typedef struct						  
{
	char header[MAX_PROTOCOL_HEADER_SIZE];
	int headerCount;
	char headerSender[MAX_PROTOCOL_HEADER_SIZE];
	char tail[MAX_PROTOCOL_TAIL_SIZE];
	char tailSender[MAX_PROTOCOL_TAIL_SIZE];
	int tailCount;
	PROTOCOL_ANALYSIS_TYPE paType;		//Э������
	int16	dataSize;				//���ݳ���
	int16	dataSizeConst;			//���ݳ��ȣ������ֵ
	uint16_t checkSum;
	int16	dataSizeSend;			//���������ݳ���
	int16 	dataSizeSendConst;		//���������ݳ��ȣ������ֵ	  
	char*	recvBuffer/*[MAX_RECEIVE_BUFFER]*/;	//����,����
	int16	recvSize;						//�ѽ������ݵĳ���

}tagProtocolAnalysis;

//�����豸�Ŵ��
int Netdata_Protocol_Packet(WORD deviceID,PROTOCOL_ANALYSIS_TYPE paType ,char* data,uint16_t size);

int Netdata_ProtocolAnalysis_Packet(tagProtocolAnalysis* pa,PROTOCOL_ANALYSIS_TYPE paType ,char* data,uint16_t size);

uint16_t Netdata_Protocol_ReadSize(WORD deviceID,uint16_t size);

PROTOCOL_READ_POS  Netdata_Protocol_GetPostion(WORD deviceID);

uint16_t Netdata_Protocol_Read(WORD deviceID,char* data, uint16_t size);
/*����һ��ȫ����������	*/
int Netdata_Protocol_Anlysis(USART_FUNCTION uf,uchar ChrData);

uint16_t Netdata_Protocol_ReadAddressAndSize(WORD deviceID,PROTOCOL_READ_POS pos, char** buf,uint16_t* size,uint16_t sizeMax);

int Netdata_Program_Update(WORD deviceID);

int Netdata_System_Ctrl(WORD deviceID);

int Netdata_Init(void);

#include "SystemCtrl.h"
#ifdef __cplusplus
} // extern "C"
#endif
#endif
#endif


