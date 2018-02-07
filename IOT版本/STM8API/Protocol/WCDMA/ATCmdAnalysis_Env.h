#ifndef AT_CMD_ANALYSIS_H
#define   AT_CMD_ANALYSIS_H

#define 	MAX_RECV_BUFF_COUNT				2			//Ӧ����ȷ����ȷЭ�����
#define 	MAX_RECV_ERROR_BUFF_COUNT		1			//Ӧ������Э�����
#define 	MAX_MSG_PHONE_SIZE				80			//���ŵ绰������󳤶�
#define 	MAX_CALL_BACK_PARAMETER_COUNT 	4			//�ص���������������
#define		MAX_CALL_BACK_PARAMETER_SIZE	50			//�ص�����������С
#define 	MAX_CID_SIZE					20			//CID��С
#define 	MAX_IPADDRESS_SIZE				20			//IP��ַ����
#define 	MAX_PHONE_SIZE					20			//�澯�绰���볤��
#define 	MAX_BEATSTRING_SIZE				100			//�����ַ�������
#define 	MAX_CARRIEROPERATOR_NAME_SIZE	50			//��Ӫ�����ֳ���
#define		MAX_APN_SIZE						20			//APN������


#define 	MAX_HUGE_BUFFER_SIZE			5			//���ʹ�����ʱ����������
#define 	MAX_SEND_DATA_SIZE_ONCE			960			//���η����ļ����ݴ�С

#define 	MAX_SEND_BUFFER_SIZE			1124		//�����ַ�����
#define 	MAX_AT_CMD_ERROR_COUNT			20			//���ErrCode��¼
#define 	MAX_FUN_ARG_SIZE				100			//�����ߴ�

#define 	MAX_ERROR_CODE_SIZE				15			//�����¼����
#define 	MAX_RUNERROR_CODE_SIZE			31			//ʵʱ�����¼����

/******************************************************************************/

typedef int (*ATCmdRecvProcessFun)(uchar *Info,int len);
typedef int (*CallBackFun)(int argc, char argv[][MAX_CALL_BACK_PARAMETER_SIZE]);
typedef int(*ErrorProcFun)(char* addr, size_t size);
typedef int (*ATCmd_Run_Fun)(void);



/******************************************************************************/

typedef enum
{
	NETWORK_PRIORIYY_TYPE_AUTO,		 //�Զ�ѡ��
	NETWORK_PRIORIYY_TYPE_1G_2G,	//1,2G����
	NETWORK_PRIORIYY_TYPE_2G_3G,	//2,3G����
	NETWORK_PRIORIYY_TYPE_MAX		
}NETWORK_PRIORIYY_TYPE;

typedef enum
{
	SIM_CARD_STATUS_NO_DETECT,
	SIM_CARD_STATUS_OK,
	SIM_CARD_STATUS_ERROR
}SIM_CARD_STATUS;

typedef enum {
	PROTOCOL_ANALYSIS_MODE_NONE,			//�����н���
	PROTOCOL_ANALYSIS_MODE_SHORT_MESSAGE,	//����
	PROTOCOL_ANALYSIS_MODE_RECEIVE			//������������
}PROTOCOL_ANALYSIS_MODE; 

typedef enum
{
	ATCMD_RUN_TYPE_RESTART,		//�豸���������ȼ����
	ATCMD_RUN_TYPE_GET_MODULE_INFO,	//��ȡģ����Ϣ,������Ӫ��֮���
	ATCMD_RUN_TYPE_INIT,		//�豸��ʼ��,���ȼ��θߵ�	
	ATCMD_RUN_TYPE_SET3GNETVERSION,	//����3G�İ汾��
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

typedef enum
{
	WCDMA_ERROR_CHECK,		//ģ����
	WCDMA_ERROR_INIT,		//��ʼ��û�ɹ�
	WCDMA_ERROR_DEVICE,		//�豸���Ĵ���
	WCDMA_ERROR_SYSTEM,			//ϵͳ����
	WCDMA_ERROR_NO_SIM_CARD,	//û��SIM��
	WCDMA_ERROR_NET_DISCONNECT,	//�������Ӳ���
	WCDMA_ERROR_MODULE_RESTART,	//ģ���Զ�������
	WCDMA_ERROR_MAX
}WCDMA_ERROR; 

typedef enum
{
	SEND_BUFFER_TYPE_COMMON,
	SEND_BUFFER_TYPE_HUGE
}SEND_BUFFER_TYPE;

typedef enum
{
	SEND_ERROR_TYPE_NONE	= 0x0000,	//û���κδ���
	SEND_ERROR_TYPE_UNKONWN	= 0x0001,	//δ֪ԭ���µĴ���
}SEND_ERROR_TYPE;

typedef enum
{
	ATCMDGROUP_RUN_STATUS_ON,
	ATCMDGROUP_RUN_STATUS_OFF,
	ATCMDGROUP_RUN_STATUS_ALWAYSON 	
}ATCMDGROUP_RUN_STATUS;

/*ϵͳӳ���״̬*/
typedef enum
{
	SYSTEM_MOUNT_MAP_STATUS_OFF,	//û����map״̬
	SYSTEM_MOUNT_MAP_STATUS_ON,		//����map״̬
	SYSTEM_MOUNT_MAP_STATUS_MAX		//map״̬
}SYSTEM_MOUNT_MAP_STATUS;

/******************************************************************************/
typedef struct
{
	uint8	 ATCmd;								//ATCmd	
	uint8*   ATCmdSendBuff/*[MaxATCmdProlLen]*/;	//����Э��ͷ����
	uint8*   ATCmdReceBuff[MAX_RECV_BUFF_COUNT]/*[MaxATCmdProlLen]*/;	//����Э��ͷ����
	uint8*	 ATCmdRecvErrBuff[MAX_RECV_ERROR_BUFF_COUNT];				//���մ���Э��ͷ����
	uint16   WaitTimer;							//�ȴ�Ӧ������ʱ��
	uint8    RepeatSendCnt;                     //ʧ���ط��������
	ATCmdRecvProcessFun	fun;					//���ճɹ�����Ӧ����
	BOOL callBackIfOk;							//���ճɹ����Ƿ�������̷���	
}tagATCmd;	//CDMA����ṹ

typedef  int(* ATCmdSendCallBack)(const tagATCmd* atCmd,WORD errType);
		
typedef struct
{
	char*	ATcmd;	
	ATCmdRecvProcessFun	fun;
}tagATCmdRecvProcessItem;

typedef struct
{
	CallBackFun callbackFun;		//�ص�����
	int	argc;		//�������� 
	/*��������*/
	char argv[MAX_CALL_BACK_PARAMETER_COUNT][MAX_CALL_BACK_PARAMETER_SIZE];

}tagCallBackQueueItem;

typedef __packed struct
{
	char  CID[MAX_CID_SIZE];				//CID���
	char  phonelNum[MAX_IPADDRESS_SIZE]; 	//��������   
	uchar BeatTime;        					//��Զ�����ķ����������ʱ��(��)	   
	char  IPAddress[MAX_PHONE_SIZE];   		//Զ������IP��ַ
	int32 Port;            					//Զ�����Ķ˿ں�	
	BOOL SendRunValFlag;  					//�Ƿ���˶˿��µĲɼ��ڷ������в���
	BOOL OpenFlag;	  	   					//�Ƿ����Զ����� =false,������ =true,����
	NETWORK_PRIORIYY_TYPE Cns_CNMP;	 		//���ȵ�ǰ��������
	char beatString[MAX_BEATSTRING_SIZE];	//�����ķ��͵���������
	int tryCount;							//ʵ�����	
	DWORD beatTime;							//����ʱ�� 
	uchar deviceFun;						//�豸����
}tagModuleSetValStruct;
#if CONF_WARN_SEND_EN
typedef __packed struct
{
	BOOL		Warn_Function_Enable;
	char 	phonelNum1[MAX_IPADDRESS_SIZE]; 	//�澯����   
	char 	phonelNum2[MAX_IPADDRESS_SIZE]; 	//�澯����   
	char 	phonelNum3[MAX_IPADDRESS_SIZE]; 	//�澯����   
	char 	phonelNum4[MAX_IPADDRESS_SIZE]; 	//�澯����   
	char 	phonelNum5[MAX_IPADDRESS_SIZE]; 	//�澯����   
	char 	phonelNum6[MAX_IPADDRESS_SIZE]; 	//�澯���� 
	char 	Warning_type[60];

}tagAlarmPhoneStruct;
#endif
typedef __packed struct
{
	WORD  mapDeviceID;							//ӳ����豸ID
	WORD  controlPort;							//�����������ӵĲɼ���,���������еĲɼ���һ��
//	char  CID[MAX_CID_SIZE];					//CID���
	char  IPAddress[MAX_IPADDRESS_SIZE];   			//Զ������IP��ַ
	int32 Port;            						//Զ�����Ķ˿ں�
	DWORD  timeOut;								//��ʱ�ָ�ʱ�䣬��λ����
//	uchar deviceFun;							//�豸����
}tagSystemSetValCtrlStruct;

typedef __packed struct
{
	tagSystemSetValCtrlStruct sysSetValCtrl;	//ֵ���ƽṹ��
	SYSTEM_MOUNT_MAP_STATUS sysMountMapStatus;	//ӳ��״̬
	int16 NetIsOpen;  	   						//��������
}tagSystemSetValStruct;

typedef __packed struct
{
	int16 CSQ;             //�ź�ǿ��
	int16 NetIsOpen[MOUNT_DEVICE_COUNT];  	   //��������
	int16 CnsMod;	   	   //����ģʽ
	SIM_CARD_STATUS SimCard;			//SIMcard�Ƿ�ok
	int16 AtErrorCount;					//�������������AT������������ﵽָ��ֵʱ���Զ�����
	BOOL	sendLock;					//������
}tagModeRunStateStruct;   	   //����ģ�����в����ṹ

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
	int 	apnStatus;							//APN������ʵ�ʲ���
	char	timeStr[256];
	char	wirelssModuleVision[50];		//ģ��汾��
	char 	carrierOperator[MAX_CARRIEROPERATOR_NAME_SIZE];			//��Ӫ��		
}tagATCmdSetInfo;
 
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

typedef struct
{
	int Count;
	char* Buffer[MAX_HUGE_BUFFER_SIZE];
	uint16_t size[MAX_HUGE_BUFFER_SIZE];
}tagHugeSendBuffer;

typedef struct
{
   const tagATCmd*	SendAtCmd; 		//AT�����
   uchar	sendBuffer[100]; 		//�����͵���չ����,������Ҫƥ���������ݳ���
   int 		sendBufferSize;			//�����͵���չ���ݳ���
   SEND_BUFFER_TYPE sendBufferType;	//�����������ͣ������ݣ���ͨ����
   tagHugeSendBuffer hugeBuffer;	//�����������ṹ��

}tagSendBufferQueueItem;

typedef struct
{
	ATCMD_RUN_TYPE	type;
	ATCmd_Run_Fun fun;	//�����еĳ���
	char	funArg[MAX_FUN_ARG_SIZE];		//�����г���Ĳ���
	WORD	funArgSize;						//������С
	ATCMDGROUP_RUN_STATUS status;	//���е�״̬
	BOOL lock;		//״̬��	
}tagAtCmdRunCtrlItem;

typedef __packed struct
{
	uchar index;
	char* CnsModStr;
	char apn[MAX_APN_SIZE];
}tagCnsModSet;

/*�Զ���APN���� */
typedef __packed struct
{
	BOOL enable;			//�����Զ���APN
	char apnName[20];		//APN����
	//BOOL enableProxy;		//��������
	char proxy[MAX_IPADDRESS_SIZE];			//�����������ַ
	unsigned short port;	//����������˿�
	//BOOL enableUserCheck;	//�����û���֤
	char username[20];		//�û���
	char password[20];		//����
}tagAPNCustom;

/******************************************************************************/

#define START_CMD(fun)	do{	\
							QueuePtr qCmdPtr = NULL;  	\
							int argc = 0;		 \
							tagCallBackQueueItem* pItem;	\
							char buf[20] = {0};	\
							if((qCmdPtr = DeQueue(PResFreeQueuePtr)) == NULL)	break;	\
							pItem = qCmdPtr->data;	\
							pItem->callbackFun = PROCESS_NAME(fun);

#define ADD_ARGV(arg1)		if(argc < MAX_CALL_BACK_PARAMETER_COUNT)	\
							{	\
								strcpy(pItem->argv[argc],arg1);	\
								argc++;	\
								pItem->argc = argc;	\
							}

#define ADD_ARGV_INT(arg1)	sprintf(buf,"%d",arg1);	\
							ADD_ARGV(buf)							

#define ADD_ARGV_BOOL(arg1)	ADD_ARGV_INT(arg1)

#define END_CMD()			if(argc == 0)	\
								buf[0] = buf[1];	\
							EnQueue(PResFullQueuePtr,qCmdPtr);	\
						}while(0);

/******************************************************************************/

#define ATCMDGROUP_NAME(fun) 	  ATCmdGroup_##fun
#define ATCMDGROUP_FUN(fun)	 int ATCMDGROUP_NAME(fun)##(void)

#define ATCMDGROUP_CALLBACK_NAME(fun) 	  ATCmdGroupCallBack_##fun
#define ATCMDGROUP_CALLBACK_FUN(fun)  int ATCMDGROUP_CALLBACK_NAME(fun)(const tagATCmd* atCmd,WORD errType)

#define PROCESS_NAME(fun) 	  ATCmdRecv_Process_##fun
#define ATCMDRECV_PROCESS(fun)	static int PROCESS_NAME(fun)##(int argc, char	argv[][MAX_CALL_BACK_PARAMETER_SIZE])

#define ATCMDRECV_NAME(fun) 	  ATCmdRecv_##fun
#define ATCMDRECV_FUN(fun)	static int ATCMDRECV_NAME(fun)##(uchar *Info,int len)


/******************************************************************************/
TIMER_FUN(StartToSendAtCmd);
/*��ʱ�����������������ķ�����������*/
TIMER_FUN(DeviceBeat);

TIMER_FUN(StartOk);

TIMER_FUN(MainLoop);

TIMER_FUN(DelayStart);

TIMER_FUN(SystemMap);

//TIMER_FUN(SystemReStart);

/******************************************************************************/

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
#if CONF_WARN_SEND_EN
int AtCmdAnalysis_AlarmPhone_Get(tagAlarmPhoneStruct* AlarmPhone);
#endif
const tagModuleSetValStruct* AtCmdAnalysis_ModuleSetVal_Read(WORD deviceID);


int AtCmdAnalysis_Msg_Send(char* phoneNum, char* buf);
int AtCmdAnalysis_Msg_Send_Direct(char* phoneNum, char* buf);
int AtCmdAnalysis_Msg_Send_FromDevice(WORD deviceID, char* buf);

int AtCmdAnalysis_BeatTime_Set(DWORD beatTime,BOOL store);
int AtCmdAnalysis_Device_Fun(WORD deviceID,uchar deviceFun);
int AtCmdAnalysis_CNMP_Set(WORD	cnmp);
int AtCmd_RECV_WCDMA_Init(void);
int AtCmdAnalysis_ModuleSetVal_Set(tagModuleSetValStruct val[MOUNT_DEVICE_COUNT]);
#if CONF_WARN_SEND_EN
int AtCmdAnalysis_AlarmPhone_Set(tagAlarmPhoneStruct val[MOUNT_DEVICE_COUNT]);
#endif
int AtCmdAnalysis_DeviceBeat_ReStartByDeviceID(int deviceID,int tryCount);
int AtCmdAnalysis_DeviceBeat_ReStart(void);
int AtCmdAnalysis_AtCmd_Send(uint8 cmd,char *buf,int len);
int AtCmdAnalysis_AtCmd_SendHugeBuffer(uint8 cmd,tagHugeSendBuffer hugeBuffer);

int AtCmdAnalysis_AtCmd_Resgister(const tagATCmd* ATCmd,	 //ATCmd������
									int ATCmdCount,	//��������������
									const tagATCmdRecvProcessItem*	ATCmdRecvProcessItemArray,	//�ؼ��ֽ�����
									int ATCmdRecvProcessItemSize,	//�ؼ�����������
									const tagAtCmdRunCtrlItem* AtCmdRunFunTypeArray
									);
int AtCmdAnalysis_RunGroup(ATCMD_RUN_TYPE runType);
int AtCmdAnalysis_SelectAnlysisMode(PROTOCOL_ANALYSIS_MODE mode,int size);
int AtCmdAnalysis_SetATCmdSendCallBack(ATCmdSendCallBack fun);
char* AtCmdAnalysis_WirelessModuleVision_Get(void);
char* AtCmdAnalysis_CarrierOperator_Get(void);
int AtCmdAnalysis_RunError_EEROM_Record(char* buf);
																		
tagSystemSetValStruct*	AtCmdAnalysis_SystemSetValCtrl_Get(void);

int AtCmdAnalysis_CnsMod_Init(tagCnsModSet* mod, int size);
									
int AtCmdAnalysis_CnsMod_Get(tagCnsModSet** mod, int* size);
									
int AtCmdAnalysis_CnsMod_EERom_Story(void);
int AtCmdAnslysis_3GNetVersion_Set(int mode);	
int AtCmdAnslysis_MsgAnalysis99(USART_FUNCTION uf,uchar *DaPtr,uint16 len);
int AtCmdAnslysis_MsgAnalysis9A(USART_FUNCTION uf,uchar *DaPtr,uint16 len);									
/******************************************************************************/
int AtCmd_Init(tagModuleSetValStruct* module,
				int moduleCount,
				tagModeRunStateStruct* runState,
				tagATCmdSetInfo* ATCmdSetInfo,
				LinkQueue* cmdLinkQueue,
				tagAtCmdRunCtrlItem* GroupCtrlArray,
				tagSystemSetValStruct*	systemSetVal
				);

/******************************************************************************/
#define RUN_ATCMD_GROUP(RUN_TYPE) AtCmdAnalysis_RunGroup(RUN_TYPE)
#define	SEL_ANLYSIS_MODE(mode,size)	AtCmdAnalysis_SelectAnlysisMode(mode,size)

#define DEBUG_RECORD(BUF) DEBUG(BUF);	\
	AtCmdAnalysis_RunError_EEROM_Record(BUF);
/******************************************************************************/

#include "3GProDa.h"
#include "SendShortMsg.h"
#endif

