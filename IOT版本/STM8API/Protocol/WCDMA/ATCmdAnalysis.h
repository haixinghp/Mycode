#if 1
#include "ATCmdAnalysis_Env.h"
#else

#ifndef AT_CMD_ANALYSIS_H
#define   AT_CMD_ANALYSIS_H

//AT�������
typedef enum { 
	AT_CMD_TEST_AT			=    0,	  //����CDMA�Ƿ��з���
	AT_CMD_SET_CFUN			=    1,	  //��������ģʽ
	AT_CMD_GET_CSQ  		=    2,	  //��ѯ�����ź�
	AT_CMD_GET_CREG  		=    3,	  //����ע����Ϣ
	AT_CMD_GET_CICCID  		=    4,	  //��ѯUIM���к�
	AT_CMD_GET_IPR  		=    5,	  //��ѯ������
	AT_CMD_SET_IPR  		=    6,	  //���ò�����
	AT_CMD_SET_CMGF  		=    7,   //���ö��Ÿ�ʽ 0,PDUģʽ 1,textģʽ
	AT_CMD_SET_CSCS   		=    8,	  //ѡ��TE�ַ���GSM��UCS2��IRA
	AT_CMD_STE_CPMS   		=    9,	  //���Ŵ洢λ��ѡ������
	AT_CMD_SET_CNMI   		=    10,  //�����¶���Ϣ��ʾ���� 2,1:�洢���� 1,2:���洢���ţ��¶���ֱ�Ӵ�ӡ������
	AT_CMD_SET_CSMP   		=    11,  //����TEXTģʽ���������һ��Ϊ���ݱ������ͣ�0��ʾĬ���ַ���(GSM) 241:�ַ� 25:����
	AT_CMD_SET_CMGSO  		=    12,  //���ٷ���Ϣ
	AT_CMD_SET_CMGR   		=    13,  //��ȡ���ţ�����λ��
	AT_CMD_SET_CMGRD  		=    14,  //��ȡ��Ϣ���SIM ���н�����Ϣɾ��������λ��
	AT_CMD_SET_CMGD  		=    15,  //ɾ����ѡ�洢���ϵĶ��ţ�ģʽ
	AT_CMD_SET_CMGL 		=    16,  //��ȡ���� "REC UNREAD"�յ�δ����Ϣ "REC READ"�յ��Ѷ���Ϣ "STO UNSENT"δ������Ϣ "STO SENT"�洢�ѷ�����Ϣ "ALL"ȫ����Ϣ
	AT_CMD_REV_MSDATA 		=    17,  //�յ���������
	AT_CMD_SET_CNMP  		=    18,  //������������ѡ��ģʽ 2,�Զ� 13,GSM 14,WCDMA
	AT_CMD_SET_CIPSRIP 		=    19,  //���ò���ʾ����IP
	AT_CMD_SET_CGSOCKCONT  	=    20,  //����APN 
	AT_CMD_SET_NETOPEN  	=    21,  //���ö����ӷ�ʽ
	AT_CMD_SET_CIPOPEN  	=    22,  //����һ·TCP/IP ����
	AT_CMD_SET_CIPSEND   	=    23,  //��һ·TCP/IP �·�������(����) 
	AT_CMD_SET_TCPWDATA		=	 24,  //��һ·TCP/IP �·�������(������)
 	AT_CMD_SET_NETCLOSE  	=    25,  //�ر�һ·TCP/IP���� 
	AT_CMD_GET_CIPOPEN 		=	 26,  //��ѯ��������״̬
	AT_CMD_REV_NETDATA		=	 27,  //�յ���������
	AT_CMD_GET_CNUM			=	 28,  //��ѯ��������
	AT_CMD_SET_CNAOP 		=	 29,  //��������˳��
	AT_CMD_GET_CNSMOD 		=	 30,  //��ѯ����ϵͳģʽ
	AT_CMD_REV_MSMNOTE		=	 31,  //�յ���������
	AT_CMD_SET_ATE 			=	 32,  //�����رջ���
	AT_CMD_SET_CIPCCFG		=	 33,  //���ò������׽���
	AT_CMD_SET_CMGS			=	 34,  //���Ͷ���+�绰����
	AT_CMD_SET_CMGSDATA		=	 35,  //���Ͷ���������
	AT_CMD_GET_CMGRO		=	 36,  //��ѯ����
	AT_CMD_MAX
}AT_CMD;

typedef int (*ATCmdRecvProcessFun)(uchar *Info,int len);
//#define	     MaxATCmdProlLen			50		//ATЭ�����󳤶�
#define 	MAX_RECV_BUFF_COUNT		2			//Ӧ����ȷ����ȷЭ�����
#define 	MAX_RECV_ERROR_BUFF_COUNT		1			//Ӧ������Э�����
#define 	MAX_MSG_PHONE_SIZE		80

typedef struct
{
	AT_CMD	 ATCmd;								//ATCmd	
	uint8*    ATCmdSendBuff/*[MaxATCmdProlLen]*/;	//����Э��ͷ����
	uint8*    ATCmdReceBuff[MAX_RECV_BUFF_COUNT]/*[MaxATCmdProlLen]*/;	//����Э��ͷ����
	uint8*	  ATCmdRecvErrBuff[MAX_RECV_ERROR_BUFF_COUNT];				//���մ���Э��ͷ����
	uint16   WaitTimer;							//�ȴ�Ӧ������ʱ��
	uint8    RepeatSendCnt;                     //ʧ���ط��������
	ATCmdRecvProcessFun	fun;					//���ճɹ�����Ӧ����
	BOOL callBackIfOk;							//���ճɹ����Ƿ�������̷���	
}tagATCmd;	//CDMA����ṹ

#define MAX_CALL_BACK_PARAMETER_COUNT 	4
#define	MAX_CALL_BACK_PARAMETER_SIZE	50
typedef int (*CallBackFun)(int argc, char argv[][MAX_CALL_BACK_PARAMETER_SIZE]);

typedef struct
{
	CallBackFun callbackFun;		//�ص�����
	int	argc;		//�������� 
	/*��������*/
	char argv[MAX_CALL_BACK_PARAMETER_COUNT][MAX_CALL_BACK_PARAMETER_SIZE];

}tagCallBackQueueItem;
typedef enum
{
	NETWORK_PRIORIYY_TYPE_AUTO,		 //�Զ�ѡ��
	NETWORK_PRIORIYY_TYPE_1G_2G,	//1,2G����
	NETWORK_PRIORIYY_TYPE_2G_3G,	//2,3G����
	NETWORK_PRIORIYY_TYPE_MAX		
}NETWORK_PRIORIYY_TYPE;

#define MAX_CID_SIZE		20
#define MAX_IPADDRESS_SIZE	20
#define MAX_PHONE_SIZE		20
#define MAX_BEATSTRING_SIZE	100	
typedef __packed struct
{
	char  CID[MAX_CID_SIZE];				//CID���
	char  phonelNum[MAX_IPADDRESS_SIZE]; 	//��������   
	uchar BeatTime;        //��Զ�����ķ����������ʱ��(��)	   
	char  IPAddress[MAX_PHONE_SIZE];   //Զ������IP��ַ
	int32 Port;            //Զ�����Ķ˿ں�	
	BOOL SendRunValFlag;  //�Ƿ���˶˿��µĲɼ��ڷ������в���
	BOOL OpenFlag;	  	   //�Ƿ����Զ����� =false,������ =true,����
	NETWORK_PRIORIYY_TYPE Cns_CNMP;	 //���ȵ�ǰ��������
	char beatString[MAX_BEATSTRING_SIZE];		//�����ķ��͵���������
	int tryCount;				//ʵ�����	
	DWORD beatTime;			//����ʱ�� 
	uchar deviceFun;			//�豸����ѡ��
}tagModuleSetValStruct;

typedef enum
{
	SIM_CARD_STATUS_NO_DETECT,
	SIM_CARD_STATUS_OK,
	SIM_CARD_STATUS_ERROR
}SIM_CARD_STATUS;
#define MAX_AT_CMD_ERROR_COUNT	100
typedef __packed struct
{
	int16 CSQ;             //�ź�ǿ��
	int16 NetIsOpen[MOUNT_DEVICE_COUNT];  	   //��������
	int16 CnsMod;	   	   //����ģʽ
	SIM_CARD_STATUS SimCard;			//SIMcard�Ƿ�ok
	int16 AtErrorCount;					//�������������AT������������ﵽָ��ֵʱ���Զ�����
}tagModeRunStateStruct;   	   //����ģ�����в����ṹ

typedef enum {
	PROTOCOL_ANALYSIS_MODE_NONE,			//�����н���
	PROTOCOL_ANALYSIS_MODE_SHORT_MESSAGE,	//����
	PROTOCOL_ANALYSIS_MODE_RECEIVE			//������������
}PROTOCOL_ANALYSIS_MODE; 

typedef enum
{
	ATCMD_RUN_TYPE_RESTART,		//�豸���������ȼ����
	ATCMD_RUN_TYPE_INIT,		//�豸��ʼ��,���ȼ��θߵ�	
	ATCMD_RUN_TYPE_READ_MSG,	//��ȡ����
	ATCMD_RUN_TYPE_SEND_MSG,	//���Ͷ���
	ATCMD_RUN_TYPE_DISCONNECT_NET,	//�Ͽ�����
	ATCMD_RUN_TYPE_CONNECT_NET,	//��������
	ATCMD_RUN_TYPE_DEVICE_SEND = ATCMD_RUN_TYPE_CONNECT_NET+ MOUNT_DEVICE_COUNT,	//�豸��Ϣ����
	ATCMD_RUN_TYPE_INFO_SEND = ATCMD_RUN_TYPE_DEVICE_SEND + MOUNT_DEVICE_COUNT,	//���Ͷ���Ϣ������100���ֽ�
	ATCMD_RUN_TYPE_LOOP = ATCMD_RUN_TYPE_INFO_SEND + MOUNT_DEVICE_COUNT,		//��ѯ��ѯ,���ȼ���͵�
	ATCMD_RUN_TYPE_MAX			//ATCMD���з�ʽ����		
}ATCMD_RUN_TYPE;

typedef enum
{
	ATCMD_RUN_STATUS_NOT_RUN,	//δ����
	ATCMD_RUN_STATUS_RUN_BUSY,		//��������ָ��
	ATCMD_RUN_STATUS_RUN_FREE,		//ָ���������
	ATCMD_RUN_STATUS_MAX		//����״̬
}ATCMD_RUN_STATUS;	//ATCMD������״̬

typedef  int(* ATCmdSendCallBack)(const tagATCmd* atCmd,WORD errType);
#define MAX_SEND_BUFFER_SIZE	1124		
typedef struct
{
	const tagATCmd*	SendAtCmd; 	//AT�����
	uint8		sendRepeatCount;	//ʧ�ܺ��ط�����
	uchar	sendBuffer[MAX_SEND_BUFFER_SIZE];		//�����͵�AT���������
 	int		sendBufferSize;			//������AT��������ݵĳ���

	char	phoneNum[MAX_MSG_PHONE_SIZE];			//�����ն�Ϣ�ķ��͵��ֻ�����
	char	msgBuffer[600];			//�����Ͷ��ŵĻ���
	char	sendPhoneNum[MAX_MSG_PHONE_SIZE];		//�����Ͷ��ŵĵ绰����
	PROTOCOL_ANALYSIS_MODE	protocolAnlysisMode;	//����ģʽ
	USART_FUNCTION	RecvFunction; 	//���յĶ˿�
	int RecvSize;				//�������ݵĴ�С
	BOOL cmpSuccess;			//ƥ��ɹ�	
	ATCMD_RUN_TYPE atCmdGroup;		//��ǰ���е�atCmdGroup

	ATCmdSendCallBack aTCmdSendCallBack;	//���ָ����Ϊ�վ�ִ�и���ָ�� 
	WORD errBits;				//�����͵Ĵ���

	
	ATCMD_RUN_STATUS atCmdRunStatus;		//AT��������е�״̬
	BOOL	haveShortMsg;	
}tagATCmdSetInfo;

typedef enum
{	WCDMA_ERROR_INIT,		//��ʼ��û�ɹ�
	WCDMA_ERROR_DEVICE,		//�豸���Ĵ���
	WCDMA_ERROR_SYSTEM,			//ϵͳ����
	WCDMA_ERROR_NO_SIM_CARD,	//û��SIM��
	WCDMA_ERROR_NET_DISCONNECT,	//�������Ӳ���
	WCDMA_ERROR_MAX
}WCDMA_ERROR; 
typedef int(*ErrorProcFun)(char* addr, size_t size); 
typedef struct
{
	WCDMA_ERROR	errorType;
	ErrorProcFun fun;	
}tagErrorCallBack;

 typedef __packed struct
{
	uchar writePointer;
	uchar size;
}tagErrorEERomContext;

int AtCmdAnalysis_Init(void);
int AtCmdAnalysis_ATCmdRecv_Analysis(uchar *Info,int len);
int AtCmdAnalysis_Queue_Callback(void);
//�����������������ܺ����ķ��͹���
int AtCmdAnalysis_WCDMA_StartWcdmaCtrl(USART_FUNCTION uf);
int AtCmdAnalysis_WCDMA_StartUpadate(USART_FUNCTION uf);
int AtCmdAnalysis_UartFuntion_StartSend(USART_FUNCTION uf);
int AtCmdAnalysis_Net_Connect(WORD deviceID);
int AtCmdAnalysis_Net_DeviceSend(WORD deviceID);
int AtCmdAnalysis_Net_InfoSend(WORD deviceID,char* buf, WORD size);
int AtCmdAnalysisi_DisConnect(WORD deviceID);
int AtCmdAnalysis_PhoneNum_Set(WORD deviceID,uchar* phoneNum,size_t size);
int AtCmdAnalysis_CID_Set(WORD deviceID,uchar* phoneNum,size_t size);
int AtCmdAnalysis_IP_Set(WORD deviceID,uchar* ipAddress,size_t size,int32 port);

int AtCmdAnalysis_ErrorMsg(WCDMA_ERROR errType,char* buf,size_t size);
int AtCmdAnalysis_ModeRunState_Get(tagModeRunStateStruct* runState);
int AtCmdAnalysis_ModuleSetVal_Get(WORD deviceID,tagModuleSetValStruct* moduleSetVal);
const tagModuleSetValStruct* AtCmdAnalysis_ModuleSetVal_Read(WORD deviceID);


int AtCmdAnalysis_Msg_Send(char* phoneNum, char* buf);
int AtCmdAnalysis_Msg_Send_FromDevice(WORD deviceID, char* buf);
int AtCmdAnalysis_Msg_Send_Direct(char* phoneNum, char* buf);
int AtCmdAnalysis_BeatTime_Set(DWORD beatTime,BOOL store);
int AtCmdAnalysis_Device_Fun(WORD deviceID,uchar deviceFun);
int AtCmdAnalysis_CNMP_Set(WORD	cnmp);
int AtCmd_RECV_WCDMA_Init(void);
int AtCmdAnalysis_ModuleSetVal_Set(tagModuleSetValStruct val[MOUNT_DEVICE_COUNT]);

#include "3GProDa.h"
#include "SendShortMsg.h"
#endif
#endif
