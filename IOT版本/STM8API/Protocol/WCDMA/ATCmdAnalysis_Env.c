/****************************************Copyright (c)****************************************************
**                                      
**                                 
**
**--------------�ļ���Ϣ---------------------------------------------------------------------------------
** �ļ���:				ATCmdAnalysis.c
** ����:				WCDMA�������ϵͳ
**
**--------------------------------------------------------------------------------------------------------
** ������:				����
** ��������:			2014-5-24
** �汾��:				v1.0
** ����:				����汾
**
**--------------------------------------------------------------------------------------------------------
** �޸���:             
** �޸�����:           
** �汾:                 
** ����:            
**
*********************************************************************************************************/
 

#include "config.h"
#include "AtCmdAnalysis.h"

/******************************************************************************/

#define MAX_QUEUE_CALLBACK_SIZE		5 
#define MAX_QUEUE_SEND_BUFFER_SIZE	20


/******************************************************************************/
#define ERROR_PROCCESS_NAME(fun) 	  Error_Process_##fun
#define ERROR_PROCCESS(fun) static int ERROR_PROCCESS_NAME(fun)(char* addr, size_t size)
/******************************************************************************/
//����������
/*ATCmd ��ʼ��*/
static int AtCmdAnalysis_ATCmdRunCtrlArray_Init(void);

ERROR_PROCCESS(Check);
ERROR_PROCCESS(Init);
ERROR_PROCCESS(Device);
ERROR_PROCCESS(System);
ERROR_PROCCESS(SimCard);
ERROR_PROCCESS(Net);
ERROR_PROCCESS(Module);

/******************************************************************************/

static const tagATCmd*	 _gATCmd = NULL;
static int _gATCmdCount = 0;

static const tagATCmdRecvProcessItem*	_gtagATCmdRecvProcessItemArray = NULL;
static int _gATCmdRecvProcessItemSize = 0;
 
//��ʼ�����ñ�
const static  tagAtCmdRunCtrlItem* _gAtCmdRunFunTypeArray = NULL;
static tagAtCmdRunCtrlItem _gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_MAX];

static tagCallBackQueueItem	_gCallBackQueueItemArray[MAX_QUEUE_CALLBACK_SIZE] = {0};

static tagSendBufferQueueItem _gSendBufferQueueItemArray[MAX_QUEUE_SEND_BUFFER_SIZE] = {0};

tagATCmdSetInfo _gATCmdSetInfo = {0};
static 	tagModeRunStateStruct	_gModeRunState = {0};

static LinkQueue	_gLinkQueue[2]	= {0};
/*���ջ���������*/
static LinkQueue	_gSendBufferLinkQueue[2]	= {0};

static tagModuleSetValStruct _gModuleSetVal[MOUNT_DEVICE_COUNT] =
 {
 	{	"10010",
		"10010",
		1,
		"192.168.0.2",
		8800,
		true,
		true,
		NETWORK_PRIORIYY_TYPE_AUTO,
		"",
		0,
		1,
		1,
	},
	{	"10010",
		"10010",
		1,
		"192.168.0.2",
		8800,
		true,
		true,
		NETWORK_PRIORIYY_TYPE_AUTO,
		"",
		0,
		1,
		0,
	}
 };
#if CONF_WARN_SEND_EN
static tagAlarmPhoneStruct	_gAlarmPhone =
{
	0,
	"123456",	
	"123456",	
	"123456",
	"123456",
	"123456",
	"123456",
	"help",
};
#endif
#if 1
/*
typedef __packed struct
{
	WORD  mapDeviceID;							//ӳ����豸ID
	WORD  controlPort;							//�����������ӵĲɼ���,���������еĲɼ���һ��
	char  CID[MAX_CID_SIZE];					//CID���
	char  IPAddress[MAX_PHONE_SIZE];   			//Զ������IP��ַ
	int32 Port;            						//Զ�����Ķ˿ں�
	DWORD  timeOut;								//��ʱ�ָ�ʱ�䣬��λ����
	uchar deviceFun;							//�豸����
}tagSystemSetValCtrlStruct;

typedef __packed struct
{
	tagSystemSetValCtrlStruct sysSetValCtrl;	//ֵ���ƽṹ��
	SYSTEM_MOUNT_MAP_STATUS sysMountMapStatus;	//ӳ��״̬
}tagSystemSetValStruct;

*/
/*ϵͳ���ӣ������ճ�ά��*/
static tagSystemSetValStruct _gSystemSetVal =
{
	{
		0,								//ӳ����豸ID
		9,								//����ӿ�
		"123.159.193.22",   				//Զ������IP��ַ
		8383,            				//Զ�����Ķ˿ں�
		60000							//��ʱ10min
	},
	SYSTEM_MOUNT_MAP_STATUS_OFF,			//Ĭ��״̬��OFF
	MAX_NET_IS_OPEN_NUM -1
};
#endif
const static tagErrorCallBack _gErrorCallBack[] = 
{
	{WCDMA_ERROR_CHECK,ERROR_PROCCESS_NAME(Check)},	//ģ�������
	{WCDMA_ERROR_INIT,ERROR_PROCCESS_NAME(Init)},		//��ʼ��û�ɹ�
	{WCDMA_ERROR_DEVICE,ERROR_PROCCESS_NAME(Device)},		//�豸���Ĵ���
	{WCDMA_ERROR_SYSTEM,ERROR_PROCCESS_NAME(System)},			//ϵͳ����
	{WCDMA_ERROR_NO_SIM_CARD,ERROR_PROCCESS_NAME(SimCard)},	//û��SIM��
	{WCDMA_ERROR_NET_DISCONNECT,ERROR_PROCCESS_NAME(Net)},	//�������Ӳ���
	{WCDMA_ERROR_MODULE_RESTART,ERROR_PROCCESS_NAME(Module)}	//ģ�����

};

static tagCnsModSet* _gCnsMod = NULL;
static int _gCnsModSize = 0;
//static tagAPNCustom	_gAPNCustom = {
//	FALSE,			//�����Զ���APN
//	"uninet",		//APN����
//	"",				//�����������ַ
//	0u,				//�˿�
//	"",				//�û���
//	""				//����	
//};
/******************************************************************************/

#define AT_CMD_COUNT	_gATCmdCount	
#define RECV_AT_CMD_SIZE	_gATCmdRecvProcessItemSize
#define ERROR_CALL_BACK_SIZE	sizeof(_gErrorCallBack)/sizeof(tagErrorCallBack)

#define CLEAR_ERROR_TYPE()  _gATCmdSetInfo.errBits = SEND_ERROR_TYPE_NONE
#define SET_ERROR_TYPE(errCode)	 _gATCmdSetInfo.errBits |= 	errCode
#define CHECK_ERROR()	(_gATCmdSetInfo.errBits != SEND_ERROR_TYPE_NONE)
#define	_gRecvFunction _gATCmdSetInfo.RecvFunction
#define _gSendAtCmd	_gATCmdSetInfo.SendAtCmd	
#define AT_CALL_BACK_MODE()	(_gSendAtCmd != NULL)

#define PResFreeQueue	_gLinkQueue[0]		//���еĶ���
#define	PResFullQueue	_gLinkQueue[1]	   	//��æ�Ķ���
#define	PResFreeQueuePtr	&PResFreeQueue
#define	PResFullQueuePtr	&PResFullQueue

#define PResFreeSendBufferQueue	_gSendBufferLinkQueue[0]		//���еĶ���
#define	PResFullSendBufferQueue	_gSendBufferLinkQueue[1]	   	//��æ�Ķ���
#define	PResFreeSendBufferQueuePtr	&PResFreeSendBufferQueue
#define	PResFullSendBufferQueuePtr	&PResFullSendBufferQueue

#define CK(str1,str2)	 (StrNcmp((uchar*)str1,(uchar*)str2) == 0)
//�ڲ���������ʼ
#define _INTR(fun)	fun

/******************************************************************************/
#if CONF_WARN_SEND_EN
static int _INTR(AtCmdAnalysis_WarnPhone_EERom_Story)(void)
{
		uint EepAddr;
		LINE_LOG();
		EepAddr =  _EEP_ADDR_ALARM_PHONE; 
		LINE_LOG();  						
		return API_EEPROM_Write(EepAddr,sizeof(tagAlarmPhoneStruct) ,(uchar*)&_gAlarmPhone,true);
}
#endif
//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_EERom_Story
// ��������  : �洢������EEROM��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int _INTR(AtCmdAnalysis_EERom_Story)(void)
{
	uint EepAddr;
	LINE_LOG();
	EepAddr =  _EEP_ADDR_MODULE_VAL; 
	LINE_LOG();  						
	return API_EEPROM_Write(EepAddr,sizeof(tagModuleSetValStruct) * MOUNT_DEVICE_COUNT ,(uchar*)_gModuleSetVal,true);    
}
//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_EERom_Read
// ��������  : ��EEROM�ڶ�ȡ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int _INTR(AtCmdAnalysis_EERom_Read)(void)
{
	uint EepAddr;
	tagModuleSetValStruct m[MOUNT_DEVICE_COUNT];
	int i = 0;
#if CONF_WARN_SEND_EN
	uint EepAddr1;
	tagAlarmPhoneStruct n;
	EepAddr1 =  _EEP_ADDR_ALARM_PHONE;
#endif
	EepAddr =  _EEP_ADDR_MODULE_VAL;
	LINE_LOG();
	memcpy(m,_gModuleSetVal,sizeof(tagModuleSetValStruct) * MOUNT_DEVICE_COUNT);  
	if(API_EEPROM_Read(EepAddr,sizeof(tagModuleSetValStruct) * MOUNT_DEVICE_COUNT,(uchar*)_gModuleSetVal,true) < 0)
	{
		memcpy(_gModuleSetVal,m,sizeof(tagModuleSetValStruct) * MOUNT_DEVICE_COUNT);
		return -1;  
	}
#if CONF_WARN_SEND_EN
	memcpy(&n,&_gAlarmPhone,sizeof(tagAlarmPhoneStruct));  
	if(API_EEPROM_Read(EepAddr1,sizeof(tagAlarmPhoneStruct),(uchar*)&_gAlarmPhone,true) < 0)
	{
		memcpy(&_gAlarmPhone,&n,sizeof(tagAlarmPhoneStruct));
		return -1;  
	}
#endif
	for(i = 0; i < MOUNT_DEVICE_COUNT; i++)
	{
		_gModuleSetVal[i].tryCount = 0;
 	}
	LINE_LOG();					
	return 0;    
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AtCmd_EndAnlysisMode
// ��������  : �ı����ģʽ����ͨ��ģʽ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void _INTR(AtCmdAnalysis_AtCmd_EndAnlysisMode)(void)
{
	LINE_LOG();
	_gATCmdSetInfo.protocolAnlysisMode = PROTOCOL_ANALYSIS_MODE_NONE;
	LINE_LOG();
}

//������������������������������������������������������������������������������
// ��������  : ATCmdRecv_Queue_Init
// ��������  : ���ж�����ת�Ƶ������������еĹ��ȶ��г�ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int _INTR(ATCmdRecv_Queue_Init)(void)
 {
 	LINE_LOG();
 	QueueInit(_gCallBackQueueItemArray,MAX_QUEUE_CALLBACK_SIZE,PResFreeQueuePtr,PResFullQueuePtr);
 }
//������������������������������������������������������������������������������
// ��������  : ATCmdRecv_SendBufferQueue_Init
// ��������  : ATCmd���Ͷ��г�ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int _INTR(ATCmdRecv_SendBufferQueue_Init)(void)
 {
 	LINE_LOG();
 	QueueInit(_gSendBufferQueueItemArray,MAX_QUEUE_SEND_BUFFER_SIZE,PResFreeSendBufferQueuePtr,PResFullSendBufferQueuePtr);
 }

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Protocol_Analysis
// ��������  : ���������Լ��������ݵ�ʶ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int _INTR(AtCmdAnalysis_Protocol_Analysis)(USART_FUNCTION uf,uchar c)
{
	int ret = 0;
	LINE_LOG();
	switch(_gATCmdSetInfo.protocolAnlysisMode)
	{
		case PROTOCOL_ANALYSIS_MODE_SHORT_MESSAGE: //����
			//���Ž���
			ret = Msg_Buffer_Receive(_gATCmdSetInfo.phoneNum,c);
			break;
		case PROTOCOL_ANALYSIS_MODE_RECEIVE: //������������
			//�������ݽ��� 	
			ret = Netdata_Protocol_Anlysis(_gATCmdSetInfo.RecvFunction,c);
			break;
		case PROTOCOL_ANALYSIS_MODE_NONE:
		default:
			return -1;
	}
	if(_gATCmdSetInfo.RecvSize >0)
		_gATCmdSetInfo.RecvSize--;	
	LINE_LOG();
	if(ret < 0 || _gATCmdSetInfo.RecvSize == 0)
	{	//���������κ��������⣬�����Ե����ⲿ����ģʽ����
		_gATCmdSetInfo.RecvSize = 0;
   		_gATCmdSetInfo.protocolAnlysisMode = PROTOCOL_ANALYSIS_MODE_NONE;
		API_KillTimerFast(TIMER_TYPE_WCDMA_ANLYSIS_MODE);

	}
	return 0;
} 

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AtCmd_GetIndex
// ��������  : ��ȡAtCmd������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int _INTR(AtCmdAnalysis_AtCmd_GetIndex)(uint8 cmd)
{
	int i = 0;
	LINE_LOG();
	for(i=0; i < AT_CMD_COUNT;i++)
	{
		if(_gATCmd[i].ATCmd == cmd)
			return i; 
	}
	LINE_LOG();
	return -1;
}


//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AtCmdRun_Process
// ��������  : ��ѭ���ڵ��ã��������ִ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

static int _INTR(AtCmdAnalysis_AtCmdRun_Process)(void)
{
	int i = 0;
	int startGroupID = 0;
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus ==  ATCMD_RUN_STATUS_RUN_BUSY)
		return -1;
	_gATCmdSetInfo.aTCmdSendCallBack = NULL;
	//startGroupID = _gATCmdSetInfo.atCmdGroup < 0? 0:_gATCmdSetInfo.atCmdGroup;
	startGroupID = _gATCmdSetInfo.atCmdGroup >(ATCMD_RUN_TYPE_MAX-1)? (ATCMD_RUN_TYPE_MAX-1) : _gATCmdSetInfo.atCmdGroup;
	if(startGroupID <  ATCMD_RUN_TYPE_MAX -1)
	{
		startGroupID++;
	}
	else
	{
		startGroupID = 0;
	}
	CLEAR_ERROR_TYPE();	
	for(i = startGroupID; i < ATCMD_RUN_TYPE_MAX;i++)
	{
		if(_gAtCmdRunCtrlArray[i].status == ATCMDGROUP_RUN_STATUS_ON && _gAtCmdRunCtrlArray[i].lock)
		{
			_gATCmdSetInfo.atCmdGroup = (ATCMD_RUN_TYPE)i;
			if(_gAtCmdRunCtrlArray[i].fun != NULL)
			{
				_gAtCmdRunCtrlArray[i].fun();	
			}
			else
			{
				_gATCmdSetInfo.atCmdRunStatus =  ATCMD_RUN_STATUS_RUN_FREE;	
			}
			if(_gAtCmdRunCtrlArray[i].status == ATCMDGROUP_RUN_STATUS_ON)   //��̬���е�ֻ����һ��������ֻ�������һ����
			{
				_gAtCmdRunCtrlArray[i].status = ATCMDGROUP_RUN_STATUS_OFF;
			}
			_gAtCmdRunCtrlArray[i].lock = false;
			return 0;
			//break;	//ִ����һ�������̽���,��һ����ͷ��ʼִ��				
		}
	}
	
	for(i = 0; i < startGroupID;i++)
	{
		if(_gAtCmdRunCtrlArray[i].status == ATCMDGROUP_RUN_STATUS_ON && _gAtCmdRunCtrlArray[i].lock)
		{
			_gATCmdSetInfo.atCmdGroup = (ATCMD_RUN_TYPE)i;
			if(_gAtCmdRunCtrlArray[i].fun != NULL)
			{
				_gAtCmdRunCtrlArray[i].fun();	
			}
			else
			{
				_gATCmdSetInfo.atCmdRunStatus =  ATCMD_RUN_STATUS_RUN_FREE;	
			}
			if(_gAtCmdRunCtrlArray[i].status == ATCMDGROUP_RUN_STATUS_ON)   //��̬���е�ֻ����һ��������ֻ�������һ����
			{
				_gAtCmdRunCtrlArray[i].status = ATCMDGROUP_RUN_STATUS_OFF;
			}
			_gAtCmdRunCtrlArray[i].lock = false;
			break;	//ִ����һ�������̽���,��һ����ͷ��ʼִ��				
		}
	}
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_ATCmdRunCtrlArray_Init
// ��������  : �������ϵͳ�ĳ�ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

static int _INTR(AtCmdAnalysis_ATCmdRunCtrlArray_Init)(void)
{
	int i = 0;
	LINE_LOG();
	for(i=0; i < ATCMD_RUN_TYPE_MAX;i++)
	{
		memcpy(&_gAtCmdRunCtrlArray[_gAtCmdRunFunTypeArray[i].type],&_gAtCmdRunFunTypeArray[i],sizeof(tagAtCmdRunCtrlItem));
	}
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_BeatString_Build
// ��������  : �����ַ����Ľ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

static int _INTR(AtCmdAnalysis_BeatString_Build)(WORD deviceID)
{
	LINE_LOG();
	if(deviceID < MOUNT_DEVICE_COUNT )
	{
		tagProtocolAnalysis pa;
		char data[51] = "";

		char* pBuf = _gModuleSetVal[deviceID].beatString;
		char* pCid= NULL; 

		size_t dataSize = sizeof(data);
		int cidLen = ReadStrLen(_gModuleSetVal[deviceID].CID);
		LINE_LOG();
		memcpy(data +(20-cidLen),_gModuleSetVal[deviceID].CID,cidLen);
		pCid = data +(20-cidLen);
		while(cidLen--)
		{
			(*pCid) = (*pCid) - '0';
			pCid++;	
		}
		LINE_LOG();
	 	if(Netdata_ProtocolAnalysis_Packet(&pa,PROTOCOL_ANALYSIS_TYPE_DEVICE_BEAT,data, dataSize) > -1)
		{
			//�ı䵱ǰ��״̬Ϊδ����״̬
			memcpy(pBuf,pa.headerSender,MAX_PROTOCOL_HEADER_SIZE);
			pBuf += MAX_PROTOCOL_HEADER_SIZE;
			memcpy(pBuf,data,dataSize);
			pBuf +=  dataSize;
			memcpy(pBuf,pa.tailSender,MAX_PROTOCOL_TAIL_SIZE);
			LINE_LOG();
			return 0;
		}
	}		
	LINE_LOG();
	return -1;
} 

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_RunStatus_Init
// ��������  : �����ַ����Ľ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

static int _INTR(AtCmdAnalysis_RunStatus_Init)(void)
{
	int index = 0;
	LINE_LOG();
	memset(&_gModeRunState,NULL,sizeof(tagModeRunStateStruct));

	for(index =0; index < MOUNT_DEVICE_COUNT; index++)
	{
		_gModeRunState.NetIsOpen[index] = MAX_NET_IS_OPEN_NUM -1;
	}
	LINE_LOG();
	return 0;
}
//�ڲ������������
/******************************************************************************/

//������������������������������������������������������������������������������
// ��������  : Error_EEROM_Record
// ��������  : �����¼
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

static int _INTR(Error_EEROM_Record)(char* title,char* buf)
{
	tagErrorEERomContext menu;
//	Date_Time dataTime;
	char bufs[256] = {0};
	DWORD	address;
	LINE_LOG();
	if(API_EEPROM_Read(_EEP_ADDR_ERROR_RECORD_CONTEXT, sizeof(tagErrorEERomContext), (uchar*)(&menu), true) < 0)
	{
		memset(&menu,NULL,sizeof(menu));
	}
	if(menu.writePointer > menu.size)
	{
		menu.writePointer = 0;	
	}
	address = _EEP_ADDR_ERROR_RECORD + 256 * menu.writePointer;
	LINE_LOG();
//	memset(&dataTime,NULL,sizeof(dataTime));
	sprintf(bufs,
		"%s %s %s\n",
//		dataTime.StuDate.Year,
//		dataTime.StuDate.Month,
//		dataTime.StuDate.Day,
//		dataTime.StuDate.Hour,
//		dataTime.StuDate.Minute,
//		dataTime.StuDate.Second,
		_gATCmdSetInfo.timeStr,
		title,
		buf
		);//��¼ʱ��
	DEBUG("%s\n",bufs);
	LINE_LOG();
	if(API_EEPROM_Write(address,200,(uchar*)bufs,true) > -1)
	{
		if(menu.size < MAX_ERROR_CODE_SIZE)
		{
			menu.size++;

		}
		menu.writePointer++;
		if(menu.writePointer >= MAX_ERROR_CODE_SIZE)
		{
			menu.writePointer = 0;	
		}

		DEBUG("Write error Address:%d\n",address);
		API_EEPROM_Write(_EEP_ADDR_ERROR_RECORD_CONTEXT, sizeof(tagErrorEERomContext), (uchar*)(&menu), true);
		LINE_LOG();
		return 0;
	}
	 LINE_LOG();
	 return -1;
}

//������������������������������������������������������������������������������
// ��������  : ERROR_PROCCESS(Check)
// ��������  : ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ERROR_PROCCESS(Check)
{
	LINE_LOG();
	return Error_EEROM_Record("Check Error",addr);
}

//������������������������������������������������������������������������������
// ��������  : ERROR_PROCCESS(Init)
// ��������  : ��ʼ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ERROR_PROCCESS(Init)
{
	LINE_LOG();
	return Error_EEROM_Record("Init Error",addr);
}
//������������������������������������������������������������������������������
// ��������  : ERROR_PROCCESS(Device)
// ��������  : �豸����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ERROR_PROCCESS(Device)
{
	LINE_LOG();
	return Error_EEROM_Record("Device Error",addr);
}
// ��������  : ERROR_PROCCESS(System)
// ��������  : ϵͳ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ERROR_PROCCESS(System)
{
	LINE_LOG();
	return Error_EEROM_Record("SysErr",addr);
}
// ��������  : ERROR_PROCCESS(SimCard)
// ��������  : sim������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ERROR_PROCCESS(SimCard)
{
	return Error_EEROM_Record("SimCard Error",addr);
}
// ��������  : ERROR_PROCCESS(Net)
// ��������  : �������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ERROR_PROCCESS(Net)
{
	LINE_LOG();
	return Error_EEROM_Record("Net Error",addr);
}

// ��������  : ERROR_PROCCESS(Module)
// ��������  : ģ�����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ERROR_PROCCESS(Module)
{
	LINE_LOG();
	return Error_EEROM_Record("Module Error",addr);
}



/******************************************************************************/

// ��������  : ATCMDRECV_PROCESS(StartSend)
// ��������  : ��ʼ��ĳ�����ܿڷ�������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_PROCESS(StartSend)
{
	USART_FUNCTION uf;
	LINE_LOG();
	if(argc < 1)
		return -1;		
	uf = (USART_FUNCTION)atoi(argv[0]);
	LINE_LOG();
	if(uf < USART_FUNCTION_MAX)
		return API_StartSendBuffer(uf);
	return -1;		
}

// ��������  : ATCMDRECV_PROCESS(StartUpdate)
// ��������  : ��ʼ����
// �������  : None
// �������  : None
// ���ز���  : None					 
//������������������������������������������������������������������������������

ATCMDRECV_PROCESS(StartUpdate)
{
	USART_FUNCTION uf;
	WORD deviceID = 0;
	LINE_LOG();
	if(argc < 1)
		return -1; 		
	uf = (USART_FUNCTION)atoi(argv[0]);
	if(uf < USART_FUNCTION_MOUNT ||uf >= USART_FUNCTION_MOUNT + MOUNT_DEVICE_COUNT)
		return -1;

	deviceID = uf - USART_FUNCTION_MOUNT;
	Netdata_Program_Update(deviceID);
	LINE_LOG();
	return 0;		
}

// ��������  : ATCMDRECV_PROCESS(StartWcdmaCtrl)
// ��������  : ��ʼWCDMA����
// �������  : None
// �������  : None
// ���ز���  : None					 
//������������������������������������������������������������������������������

ATCMDRECV_PROCESS(StartWcdmaCtrl)
{
	USART_FUNCTION uf;
	WORD deviceID = 0;
	LINE_LOG();
	if(argc < 1)
		return -1; 		
	uf = (USART_FUNCTION)atoi(argv[0]);
	if(uf < USART_FUNCTION_MOUNT ||uf >= USART_FUNCTION_MOUNT + MOUNT_DEVICE_COUNT)
		return -1;

	deviceID = uf - USART_FUNCTION_MOUNT;
	Netdata_System_Ctrl(deviceID);
	LINE_LOG();
	return 0;
}

/******************************************************************************/
/*��ʱ������ʼ*/

////������������������������������������������������������������������������������
//// ��������  : TIMER_FUN(SystemReStart)
//// ��������  : ϵͳ����
//// �������  : None
//// �������  : None
//// ���ز���  : None
////������������������������������������������������������������������������������
//TIMER_FUN(SystemReStart)
//{
//	NVIC_SystemReset();	
//}
//������������������������������������������������������������������������������
// ��������  : TIMER_FUN(MainLoop)
// ��������  : ��ѭ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

TIMER_FUN(MainLoop)
{
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus ==  ATCMD_RUN_STATUS_RUN_FREE)
	{
		RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_LOOP);
	}
	LINE_LOG();
}


//������������������������������������������������������������������������������
// ��������  : TIMER_FUN(DeviceBeat)
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
TIMER_FUN(DeviceBeat)
{
	int  i = 0;
	LINE_LOG();
	for(i = 0; i < MOUNT_DEVICE_COUNT; i++)
	{
		if(_gModuleSetVal[i].deviceFun == 1)
		{
			if(_gModeRunState.NetIsOpen[i] == MAX_NET_IS_OPEN_NUM)
			{
				AtCmdAnalysis_Net_InfoSend(i,_gModuleSetVal[i].beatString,69);	//������������	
			}
			else
			{
				if(_gModuleSetVal[i].tryCount < 1) {
					//û�����ϡ�
					_gModuleSetVal[i].tryCount = 1;
					//DEBUG("start :%d count:%d\n",i,_gModuleSetVal[i].tryCount );
					
			   		AtCmdAnalysis_Net_Connect(i);

				}
				else
				{
					_gModuleSetVal[i].tryCount--;	
				}
			}
		}
	
	}
	if(_gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_ON)
	{
		//���������ϵͳ��������
		if(_gSystemSetVal.NetIsOpen < MAX_NET_IS_OPEN_NUM)
		{
			AtCmdAnalysis_Net_Connect(_gSystemSetVal.sysSetValCtrl.mapDeviceID);//��ʼ���Ӻ�̨����	
		}
		else
		{
			AtCmdAnalysis_Net_InfoSend(_gSystemSetVal.sysSetValCtrl.mapDeviceID,_gModuleSetVal[_gSystemSetVal.sysSetValCtrl.mapDeviceID].beatString,69);
		}
		
	}
	else
	{
		if(_gSystemSetVal.NetIsOpen == MAX_NET_IS_OPEN_NUM)
		{
			AtCmdAnalysisi_DisConnect(_gSystemSetVal.sysSetValCtrl.mapDeviceID);//��ʼ���Ӻ�̨����	
		}
	}
	//һ���ӷ���һ�� 	
	LINE_LOG();
}

//������������������������������������������������������������������������������
// ��������  : TIMER_FUN(StartOk)
// ��������  : ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

TIMER_FUN(StartOk)
{
	LINE_LOG();
	AtCmdAnalysis_RunStatus_Init();
	LINE_LOG();
	DEBUG_RECORD("Re Start!");

	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
 		LINE_LOG();
		AtCmd_RECV_WCDMA_Init();

	}
	else
	{
		LINE_LOG();
		API_WCDMA_Init();
	}
	LINE_LOG();
	API_SetTimer(60000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);
	LINE_LOG();
}

//������������������������������������������������������������������������������
// ��������  : TIMER_FUN(SystemMap)
// ��������  : ϵͳӳ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

TIMER_FUN(SystemMap)
{
	_gSystemSetVal.sysMountMapStatus = SYSTEM_MOUNT_MAP_STATUS_OFF;
	//TIMER_RUN(StartOk);
}

//������������������������������������������������������������������������������
// ��������  : TIMER_FUN(DelayStart)
// ��������  : ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

TIMER_FUN(DelayStart)
{
	TIMER_RUN(StartOk);
}

int AtCmdAnalysis_AtcmdError_Record(void)
{
	LINE_LOG();
	if(!_gATCmdSetInfo.cmpSuccess && _gATCmdSetInfo.SendAtCmd != NULL)
	{
		char errBuf[30] = {0};
		_gModeRunState.AtErrorCount++;
		if(_gModeRunState.AtErrorCount >= MAX_AT_CMD_ERROR_COUNT)
		{
			char* str = "AtCmd Error";
			_gModeRunState.AtErrorCount = 0;
			AtCmdAnalysis_ErrorMsg(WCDMA_ERROR_MODULE_RESTART,str,strlen(str));
			TIMER_RUN(StartOk);
		}
		sprintf(errBuf,"No:%d cmd:%d\n",_gATCmdSetInfo.sendRepeatCount,_gATCmdSetInfo.SendAtCmd->ATCmd);
	//	DEBUG_RECORD(errBuf);
		LINE_LOG();
	}
	LINE_LOG();	
	return 0;
}
//������������������������������������������������������������������������������
// ��������  : TIMER_FUN(StartToSendAtCmd)
// ��������  : AtCmd����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

TIMER_FUN(StartToSendAtCmd)
{
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus == ATCMD_RUN_STATUS_NOT_RUN)
	{
		//�������ִ����ȥ��
	 	QueuePtr QPtr = NULL;

		DEBUG_RECORD("System must be Restart\n");
		while((QPtr = DeQueue(PResFullSendBufferQueuePtr)) != NULL)
		{
			EnQueue(PResFreeSendBufferQueuePtr,QPtr); 	//������е�Cmd	,�����������ִ����
		}
		LINE_LOG();			
		API_WCDMA_Init();
		LINE_LOG();
		return;
	}
	LINE_LOG();
	_gATCmdSetInfo.atCmdRunStatus =  ATCMD_RUN_STATUS_RUN_BUSY;
	if(_gATCmdSetInfo.SendAtCmd == NULL || _gATCmdSetInfo.sendRepeatCount <= 0)
	{
	 	QueuePtr QPtr = NULL;
	 	tagSendBufferQueueItem* pItem = NULL;
		LINE_LOG();
		if(CHECK_ERROR())
		{
			AtCmdAnalysis_AtcmdError_Record();
			//DEBUG("Check Error\n");
			if(_gATCmdSetInfo.SendAtCmd != NULL)
			{
				char errBuf[30] = {0};
				sprintf(errBuf,"Check Cmd Error:%d",_gATCmdSetInfo.SendAtCmd->ATCmd);
				DEBUG_RECORD(errBuf); 
			}
			LINE_LOG();
			while((QPtr = DeQueue(PResFullSendBufferQueuePtr)) != NULL)
			{
				EnQueue(PResFreeSendBufferQueuePtr,QPtr); 	//������е�Cmd	,�����������ִ����
			}			
		}
		else
		{
//			 if(_gATCmdSetInfo.SendAtCmd != NULL)
//			 {
//			 	if(_gATCmdSetInfo.SendAtCmd->RepeatSendCnt > 1)
//				{
//			 		_gModeRunState.AtErrorCount = 0;
//				}
//			 }
		}


		if((QPtr = DeQueue(PResFullSendBufferQueuePtr)) == NULL )
		{
			_gATCmdSetInfo.atCmdRunStatus=  ATCMD_RUN_STATUS_RUN_FREE;	//����;

			if(_gATCmdSetInfo.aTCmdSendCallBack != NULL)
			{
				//����ʧ�ܺ�Ĵ���
				_gATCmdSetInfo.aTCmdSendCallBack(_gATCmdSetInfo.SendAtCmd,_gATCmdSetInfo.errBits);				
			}
	 		_gATCmdSetInfo.SendAtCmd = NULL;
			_gATCmdSetInfo.sendRepeatCount = 0;

			CLEAR_ERROR_TYPE();	
			API_KillTimer(TIMER_TYPE_WCDMA_AT_SEND);
			LINE_LOG();
			//һ������ȫ������
			return ;
		}
		else
		{
			int index = 0;
			pItem = QPtr->data;
			_gATCmdSetInfo.SendAtCmd = pItem->SendAtCmd;

			switch(pItem->sendBufferType)
			{
				case SEND_BUFFER_TYPE_COMMON:
					memcpy(_gATCmdSetInfo.sendBuffer,pItem->sendBuffer,pItem->sendBufferSize);
					_gATCmdSetInfo.sendBufferSize = pItem->sendBufferSize;
					break;
				case SEND_BUFFER_TYPE_HUGE:
					//���ʹ�������	���ܳ���1250������
					pItem->hugeBuffer.Count = pItem->hugeBuffer.Count < MAX_HUGE_BUFFER_SIZE?pItem->hugeBuffer.Count: MAX_HUGE_BUFFER_SIZE;
					_gATCmdSetInfo.sendBufferSize = 0;
					for(index = 0; index < pItem->hugeBuffer.Count;index++)
					{
						if(pItem->hugeBuffer.Buffer[index] != NULL && _gATCmdSetInfo.sendBufferSize + pItem->hugeBuffer.size[index] <= MAX_SEND_BUFFER_SIZE )
						{
							//û�г������buffer�������ͼ����������
							memcpy(_gATCmdSetInfo.sendBuffer + _gATCmdSetInfo.sendBufferSize,pItem->hugeBuffer.Buffer[index],pItem->hugeBuffer.size[index]);
							_gATCmdSetInfo.sendBufferSize += pItem->hugeBuffer.size[index];
						}
	
					}
			}			
			_gATCmdSetInfo.sendRepeatCount = _gATCmdSetInfo.SendAtCmd->RepeatSendCnt;
			EnQueue(PResFreeSendBufferQueuePtr,QPtr);
		}
		
	}
	LINE_LOG();
	 if(_gATCmdSetInfo.SendAtCmd != NULL && _gATCmdSetInfo.sendRepeatCount > 0)
	 {
		AtCmdAnalysis_AtcmdError_Record();
		_gATCmdSetInfo.cmpSuccess = false;
		API_ResetProtocolBuffer(USART_FUNCTION_WCDMA,RESETRTBUFF_TYPE_TX);
		API_AddProtocolBuffer(USART_FUNCTION_WCDMA,(uchar*)(_gATCmdSetInfo.SendAtCmd->ATCmdSendBuff),ReadStrLen((char*)_gATCmdSetInfo.SendAtCmd->ATCmdSendBuff));
		API_AddProtocolBuffer(USART_FUNCTION_WCDMA,_gATCmdSetInfo.sendBuffer,_gATCmdSetInfo.sendBufferSize);
		API_AddProtocolBuffer(USART_FUNCTION_WCDMA,"\r\n",2);
		API_StartSendBuffer(USART_FUNCTION_WCDMA);
		_gATCmdSetInfo.sendRepeatCount--;

	 }
	 LINE_LOG();
	 API_SetTimer(_gATCmdSetInfo.SendAtCmd->WaitTimer,1,TIMER_TYPE_WCDMA_AT_SEND,NULL,TIMER_NAME(StartToSendAtCmd));
	LINE_LOG();
}
/*��ʱ���������*/

/******************************************************************************/
//ȫ�ֺ�������

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_WCDMA_StartUpadate
// ��������  : ��ʼ��������
// �������  : uf,���ĸ��ڷ��͹���������
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_WCDMA_StartUpadate(USART_FUNCTION uf)
{
	if(uf < USART_FUNCTION_MAX)
	{
	    LINE_LOG();
		START_CMD(StartUpdate);
		ADD_ARGV_INT(uf);
		END_CMD();
		LINE_LOG();
		return 0;
	}

	return 1;
}

int AtCmdAnalysis_WCDMA_StartWcdmaCtrl(USART_FUNCTION uf)
{
	if(uf < USART_FUNCTION_MAX)
	{
		LINE_LOG();
		START_CMD(StartWcdmaCtrl);
		ADD_ARGV_INT(uf);
		END_CMD();
		LINE_LOG();
		return 0;
	}

	return 1;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_UartFuntion_StartSend
// ��������  : ��ʼ���ܿڷ�������
// �������  : uf,���ĸ��ڷ�������
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_UartFuntion_StartSend(USART_FUNCTION uf)
{

	if(uf < USART_FUNCTION_MAX)
	{
		LINE_LOG();
		START_CMD(StartSend);
		ADD_ARGV_INT(uf);
		END_CMD();
		LINE_LOG();
		return 0;
	}
	return 1;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_ATCmd_Analysis
// ��������  : ����3Gģ��AT�����������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_ATCmdRecv_Analysis(uchar *Info,int len)
{
	 int  i = 0;
	 const tagATCmdRecvProcessItem* pItem = NULL;
	 LINE_LOG();
	 if(Info == NULL || len < 1)
	 	return -1;
	 for(i= 0; i < RECV_AT_CMD_SIZE; i++)
	 {
	 	 if(CK(Info,_gtagATCmdRecvProcessItemArray[i].ATcmd))
		 {
		 	pItem = &_gtagATCmdRecvProcessItemArray[i];
		 	break;
		 }
	 }
	 LINE_LOG();
	 if(pItem != NULL)
	 {
	 	if(pItem->fun != NULL)
		{
			int ret = pItem->fun(Info,len);
			ret = ret < 0?-1:ret;
			if(ret <= 0)
			{
				return ret;
			} 
		}
		else
		{
			return 0;
		}
	 }
	 LINE_LOG();
	 if(AT_CALL_BACK_MODE())
	 {
	 	int i =  0;
		BOOL ret = false;
		LINE_LOG();
	 	for(i = 0; i < MAX_RECV_BUFF_COUNT; i++)
		{	
			if((ret = CK(Info,_gSendAtCmd->ATCmdReceBuff[i])) == TRUE)
				break;
		}

	 	if(ret)
		{
			LINE_LOG();
			_gModeRunState.AtErrorCount = 0;
			if(_gSendAtCmd->fun != NULL)
			{
				_gSendAtCmd->fun(Info,len);

			}

  			_gATCmdSetInfo.sendRepeatCount = 0;
			CLEAR_ERROR_TYPE();
			_gATCmdSetInfo.cmpSuccess  = true;
		
			API_SetTimerFast(10L,1,TIMER_TYPE_WCDMA_AT_SEND,NULL,TIMER_NAME(StartToSendAtCmd));
			LINE_LOG();
			return 0;	 
			//Ӧ����ȷ
		}
		else
		{
			LINE_LOG();
			//���ǰ���Ѿ��жϳ�����ƥ���ֵ�ˣ�����ľͲ������ж���
			if(!_gATCmdSetInfo.cmpSuccess)
			{
				for(i = 0; i < MAX_RECV_ERROR_BUFF_COUNT; i++)
				{	
					if((ret = CK(Info,_gSendAtCmd->ATCmdRecvErrBuff[i])) == TRUE)
						break;
				}
				if(ret)	//���յ�ָ���Ĵ�����Ϣ����ֱ�ӷ���
				{
				 
					API_SetTimerFast(10L,1,TIMER_TYPE_WCDMA_AT_SEND,NULL,TIMER_NAME(StartToSendAtCmd));

				}
				SET_ERROR_TYPE(SEND_ERROR_TYPE_UNKONWN);

			}
			//���·�������
			//Ӧ����Ԥ�ڵĽ����һ��
		}
		LINE_LOG(); 
		return -1;
	 }
	 LINE_LOG();
	 return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Queue_Callback
// ��������  : �ص�����ִ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_Queue_Callback(void)
{
	QueuePtr qPtr = NULL;

	while((qPtr = DeQueue(PResFullQueuePtr)) != NULL)
	{
		tagCallBackQueueItem* pItem = qPtr->data;
		if( pItem != NULL)
		{
			if(pItem->callbackFun != NULL)
			{
				pItem->callbackFun(pItem->argc,pItem->argv);

			}	
		}
		//ȫ��ִ�л���ֱ��Ϊ��Ϊֹ
		EnQueue(PResFreeQueuePtr,qPtr);
	}
	Msg_BufferQueue_Process();
	AtCmdAnalysis_AtCmdRun_Process();

	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmd_RECV_WCDMA_Init
// ��������  : ����ģ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmd_RECV_WCDMA_Init(void)
{
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_RESTART);
	}		   

	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Net_InfoSend
// ��������  : �������ݷ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_Net_InfoSend(WORD deviceID,char* buf, WORD size)	
{
LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		LINE_LOG();
		if(deviceID < MOUNT_DEVICE_COUNT )
		{	
		 
			WORD index = ATCMD_RUN_TYPE_INFO_SEND + deviceID;
			if(_gAtCmdRunCtrlArray[index].status == ATCMDGROUP_RUN_STATUS_OFF)
			{  LINE_LOG();
				memset(_gAtCmdRunCtrlArray[index].funArg,NULL,MAX_FUN_ARG_SIZE);
				memcpy(_gAtCmdRunCtrlArray[index].funArg,buf,size);
				_gAtCmdRunCtrlArray[index].funArgSize = size;
				RUN_ATCMD_GROUP((ATCMD_RUN_TYPE)index);
			}		
		}
	}
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Net_DeviceSend
// ��������  : �豸���ݷ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_Net_DeviceSend(WORD deviceID)		//�����豸�ķ��ص���Ϣ
{
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		LINE_LOG();
		if(deviceID < MOUNT_DEVICE_COUNT )
		{
			WORD index = ATCMD_RUN_TYPE_DEVICE_SEND + deviceID;
			if(_gAtCmdRunCtrlArray[index].status == ATCMDGROUP_RUN_STATUS_OFF)
			{
				RUN_ATCMD_GROUP((ATCMD_RUN_TYPE)index);
			}		
		}
	}
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysisi_DisConnect
// ��������  : �ж�ĳ���豸����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysisi_DisConnect(WORD deviceID)
{
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		LINE_LOG();
		if(deviceID < MOUNT_DEVICE_COUNT )
		{
			LINE_LOG();
			if(_gModeRunState.NetIsOpen[deviceID] != MAX_NET_IS_OPEN_NUM
				&& 	(deviceID != _gSystemSetVal.sysSetValCtrl.mapDeviceID
				|| _gSystemSetVal.sysMountMapStatus != SYSTEM_MOUNT_MAP_STATUS_OFF
				||_gSystemSetVal.NetIsOpen < MAX_NET_IS_OPEN_NUM)
				)
			{
				LINE_LOG();
				return 0;
			}
			else
			{
				if(_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_DISCONNECT_NET].status == ATCMDGROUP_RUN_STATUS_OFF)
				{
					sprintf(_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_DISCONNECT_NET].funArg,"%d",deviceID);
					RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_DISCONNECT_NET);
					LINE_LOG();
					return 0;	
				}
			}		
		}
		
	}
	LINE_LOG(); 
	return -1;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_PhoneNum_Set
// ��������  : �绰��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_PhoneNum_Set(WORD deviceID,uchar* phoneNum,size_t size)
{
	LINE_LOG();
	if(size < MAX_IPADDRESS_SIZE && deviceID < MOUNT_DEVICE_COUNT)
	{
		LINE_LOG();
		memset(_gModuleSetVal[0].phonelNum,NULL, MAX_PHONE_SIZE);
		memcpy(_gModuleSetVal[0].phonelNum,phoneNum,size);
		AtCmdAnalysis_EERom_Story();
		LINE_LOG();
		return 0;
	}
	LINE_LOG();
	return -1;	
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_IP_Set
// ��������  : IP����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_IP_Set(WORD deviceID,uchar* ipAddress,size_t size,int32 port)
{
	LINE_LOG();
	if(size < MAX_PHONE_SIZE && deviceID < MOUNT_DEVICE_COUNT)
	{
		LINE_LOG();
		memset(_gModuleSetVal[deviceID].IPAddress,NULL, MAX_PHONE_SIZE);
		memcpy(_gModuleSetVal[deviceID].IPAddress,ipAddress,size);
		_gModuleSetVal[deviceID].Port = port;
		AtCmdAnalysis_EERom_Story();
		AtCmdAnalysisi_DisConnect(deviceID);
		AtCmdAnalysis_Net_Connect(deviceID);
		LINE_LOG();
		return 0;
	}
	LINE_LOG();
	return -1;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_BeatTime_Set
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_BeatTime_Set(DWORD beatTime,BOOL store)
{
	LINE_LOG();
	if(beatTime >= 1 && beatTime <= 10)
	{
		int i = 0;
		for(i = 0; i < MOUNT_DEVICE_COUNT;i++)
		{
			_gModuleSetVal[i].beatTime = beatTime;
		}
		LINE_LOG();
		if(store)
		{
			AtCmdAnalysis_EERom_Story();
		}
		LINE_LOG();
		AtCmdAnalysis_DeviceBeat_ReStart();
		LINE_LOG();
		return 0;
	}
	else
	{
		LINE_LOG();
		return -1;
	}	
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Device_Fun
// ��������  : �豸ʹ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_Device_Fun(WORD deviceID,uchar deviceFun)
{
	LINE_LOG();
	if(deviceID < MOUNT_DEVICE_COUNT && deviceFun < 3)
	{
		_gModuleSetVal[deviceID].deviceFun = deviceFun;
		AtCmdAnalysis_EERom_Story();
		if(deviceFun == 1)
		{
			LINE_LOG();
			AtCmdAnalysis_Net_Connect(deviceID);	
		}
		else
		{
			LINE_LOG();
			AtCmdAnalysisi_DisConnect(deviceID);
		}
		
	}
	LINE_LOG();
	return 0;	
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_CNMP_Set
// ��������  : ����ģʽ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_CNMP_Set(WORD	cnmp)
{
	LINE_LOG();
	if(cnmp < NETWORK_PRIORIYY_TYPE_MAX)
	{
		LINE_LOG();
		_gModuleSetVal[0].Cns_CNMP = (NETWORK_PRIORIYY_TYPE)cnmp;
		AtCmdAnalysis_EERom_Story();
	   AtCmd_RECV_WCDMA_Init();		//�����豸
	   LINE_LOG();
	   return 0;
	}
	LINE_LOG();
	return -1;	
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Net_Connect
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_Net_Connect(WORD deviceID)
{
	LINE_LOG();

	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{

		LINE_LOG();
 		if(deviceID < MOUNT_DEVICE_COUNT )
		{

			LINE_LOG();
 			if(_gModeRunState.NetIsOpen[deviceID] == MAX_NET_IS_OPEN_NUM
			&& 	(deviceID != _gSystemSetVal.sysSetValCtrl.mapDeviceID
				|| _gSystemSetVal.sysMountMapStatus != SYSTEM_MOUNT_MAP_STATUS_ON
				||_gSystemSetVal.NetIsOpen == MAX_NET_IS_OPEN_NUM)
			)
			{
				LINE_LOG();
 				return 0;
			}

			if(_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_CONNECT_NET + deviceID].status == ATCMDGROUP_RUN_STATUS_OFF)
			{
				LINE_LOG();

				if(AtCmdAnalysis_BeatString_Build(deviceID) > -1)
				{
					RUN_ATCMD_GROUP((ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_CONNECT_NET + deviceID));	
				}	
			}
		}
	}
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_ModeRunState_Get
// ��������  : ����״̬��ȡ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_ModeRunState_Get(tagModeRunStateStruct* runState)
{
	LINE_LOG();
	if(runState != NULL)
	{
		LINE_LOG();
		memcpy(runState,&_gModeRunState,sizeof(tagModeRunStateStruct));
		return 0;
	}
	LINE_LOG();
	return -1;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AlarmPhone_Get
// ��������  : ĳ���豸�Ĳ������û�ȡ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
#if CONF_WARN_SEND_EN
int AtCmdAnalysis_AlarmPhone_Get(tagAlarmPhoneStruct* AlarmPhone)
{
	LINE_LOG();
	memcpy(AlarmPhone,&_gAlarmPhone,sizeof(tagAlarmPhoneStruct));
	return 0;
}
#endif
//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_ModuleSetVal_Get
// ��������  : ĳ���豸�Ĳ������û�ȡ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_ModuleSetVal_Get(WORD deviceID,tagModuleSetValStruct* moduleSetVal)
{
	LINE_LOG();
	if(deviceID < MOUNT_DEVICE_COUNT && moduleSetVal != NULL)
	{
		LINE_LOG();
		memcpy(moduleSetVal,&_gModuleSetVal[deviceID],sizeof(tagModuleSetValStruct));
		return 0;
	}
	LINE_LOG();
	return -1;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_ModuleSetVal_Read
// ��������  : ĳ���豸�Ĳ������û�ȡ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

const tagModuleSetValStruct* AtCmdAnalysis_ModuleSetVal_Read(WORD deviceID)
{
	LINE_LOG();
	if(deviceID < MOUNT_DEVICE_COUNT)
	{
		LINE_LOG();
		return &_gModuleSetVal[deviceID];
	}
	LINE_LOG();
	return NULL;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Msg_Send
// ��������  : ���ŷ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_Msg_Send(char* phoneNum, char* buf)
{
	LINE_LOG();
	if(phoneNum != NULL && buf != NULL)
	{
		int retSize = 0;
		strcpy(_gATCmdSetInfo.sendPhoneNum,phoneNum);
		retSize = Msg_Buffer_UTF8ToPDU(buf,strlen(buf),_gATCmdSetInfo.msgBuffer,sizeof(_gATCmdSetInfo.msgBuffer)-1);
		LINE_LOG();
		if(retSize > 0)
		{
			LINE_LOG();
			//_gATCmdSetInfo.msgBuffer[retSize] = 0x1a;
			_gATCmdSetInfo.msgBuffer[retSize] = 0x0;
			RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_SEND_MSG);	
		}
		//sprintf(_gATCmdSetInfo.msgBuffer,"%s%c",buf,0x1a);
		LINE_LOG();
		return 0;
	}
	LINE_LOG();
	return -1;	
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Msg_Send_FromDevice
// ��������  : ���ŷ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_Msg_Send_FromDevice(WORD deviceID, char* buf)
{
	LINE_LOG();
	if(deviceID < MOUNT_DEVICE_COUNT)
	{
		char phoneNum[90] = {0};
		int retSize = 0;
		phoneNum[0] = '\"';
		retSize = Msg_Buffer_UTF8ToPDU(_gModuleSetVal[deviceID].phonelNum,strlen(_gModuleSetVal[deviceID].phonelNum),&phoneNum[1],80);
		if(retSize > 0)
		{
			LINE_LOG();
			phoneNum[retSize+1] = '\"';
			return AtCmdAnalysis_Msg_Send(phoneNum,buf);
		}
	}
	LINE_LOG();
	return -1;	
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Msg_Send_Direct
// ��������  : ����ֱ�ӷ��ͣ����ؽ���ת�빤��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_Msg_Send_Direct(char* phoneNum, char* buf)
{
	LINE_LOG();
	if(phoneNum != NULL && buf != NULL)
	{
		int retSize = 0;
		strcpy(_gATCmdSetInfo.sendPhoneNum,phoneNum);
		retSize = strlen(buf);
		LINE_LOG();
		if(retSize > 0 && retSize < 600)
		{
			strcpy(_gATCmdSetInfo.msgBuffer,buf);
			_gATCmdSetInfo.msgBuffer[retSize] = 0x1a;
			RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_SEND_MSG);	
		}
		//sprintf(_gATCmdSetInfo.msgBuffer,"%s%c",buf,0x1a);
		LINE_LOG();
		return 0;
	}
	return -1;	
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_Init
// ��������  : ϵͳ�ĳ�ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_Init(void)
{
	LINE_LOG();
	AtCmdAnalysis_EERom_Read();
	AtCmdAnalysis_RunStatus_Init();

	AtCmd_Init(_gModuleSetVal, MOUNT_DEVICE_COUNT,&_gModeRunState,&_gATCmdSetInfo,_gLinkQueue,_gAtCmdRunCtrlArray,&_gSystemSetVal);
	LINE_LOG();
	ATCmdRecv_Queue_Init();
	ATCmdRecv_SendBufferQueue_Init();
	Netdata_Init();
	Msg_Init();
	API_SetOutsideAnalysisFun(USART_FUNCTION_WCDMA,AtCmdAnalysis_Protocol_Analysis);
	AtCmdAnalysis_ATCmdRunCtrlArray_Init();
	API_SetTimer(3000,-1,TIMER_TYPE_WCDMA_ATCMD_PROCESS_LOOP,TIMER_NAME(MainLoop),NULL);
	AtCmdAnalysis_BeatTime_Set(_gModuleSetVal[0].beatTime,false);
	API_SetTimer(60000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);
	DEBUG_RECORD("Start OK!");
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_CID_Set
// ��������  : CID���� 
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_CID_Set(WORD deviceID,uchar* cid,size_t size)
{
	LINE_LOG();
	if(size < MAX_IPADDRESS_SIZE && deviceID < MOUNT_DEVICE_COUNT)
	{	
		memset(_gModuleSetVal[deviceID].CID,NULL, MAX_CID_SIZE);
		memcpy(_gModuleSetVal[deviceID].CID,cid,size);
		AtCmdAnalysis_EERom_Story();
		LINE_LOG();
		return AtCmdAnalysis_BeatString_Build(deviceID);
	}
	LINE_LOG();
	return -1;
}
//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AlarmPhone_Set
// ��������  : ������������ 
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
#if CONF_WARN_SEND_EN
int AtCmdAnalysis_AlarmPhone_Set(tagAlarmPhoneStruct* val)
{
	LINE_LOG();
	memcpy(&_gAlarmPhone,val,sizeof(tagAlarmPhoneStruct));
	LINE_LOG();
	AtCmdAnalysis_WarnPhone_EERom_Story();
//	AtCmd_RECV_WCDMA_Init();		//�����豸
//	ʮ�������ģ��
	API_SetTimer(10000L,1,TIMER_TYPE_WCDMA_DELAY_RESTART,NULL,TIMER_NAME(DelayStart));	
	LINE_LOG();	
	return 0;
}
#endif
//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_ModuleSetVal_Set
// ��������  : �豸���������� 
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_ModuleSetVal_Set(tagModuleSetValStruct val[MOUNT_DEVICE_COUNT])
{
	int i = 0;
	LINE_LOG();
	for(i = 0; i < MOUNT_DEVICE_COUNT; i++)
	{
		memcpy(&_gModuleSetVal[i],&val[i],sizeof(tagModuleSetValStruct));
	}
	LINE_LOG();
	AtCmdAnalysis_EERom_Story();
//	AtCmd_RECV_WCDMA_Init();		//�����豸
	//ʮ�������ģ��
	API_SetTimer(10000L,1,TIMER_TYPE_WCDMA_DELAY_RESTART,NULL,TIMER_NAME(DelayStart));

	
	LINE_LOG();	
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_ErrorMsg
// ��������  : ��������Ϣ 
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_ErrorMsg(WCDMA_ERROR errType,char* buf,size_t size)
{
	int i = 0;
	LINE_LOG();
	for(i = 0; i < ERROR_CALL_BACK_SIZE; i++)
	{
		if(errType == _gErrorCallBack[i].errorType)
		{
			if(_gErrorCallBack[i].fun != NULL)
			{
				_gErrorCallBack[i].fun(buf,size);
				break;
			}
		}
	}
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AtCmd_Send
// ��������  : ����AT��������
// �������  : CmdNo,������; Info,�������������; len,�������ݳ���;
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_AtCmd_Send(uint8 cmd,char *buf,int len)
{
	tagSendBufferQueueItem* pItem = NULL;
	QueuePtr QPtr = NULL;
	int index = -1;
	LINE_LOG();
	index =  AtCmdAnalysis_AtCmd_GetIndex(cmd);
	if(index < 0 && len > MAX_SEND_BUFFER_SIZE)
		return -1;
	LINE_LOG();
	if((QPtr = DeQueue(PResFreeSendBufferQueuePtr)) == NULL )
	{
		DEBUG("Error for Queue!\n");
		return -1;
	}
	pItem = QPtr->data;
	pItem->SendAtCmd = &_gATCmd[index];
	if(buf != NULL)
	{
   		memcpy(pItem->sendBuffer,buf,len);
		pItem->sendBufferSize = len;
   	}
	pItem->sendBufferType = SEND_BUFFER_TYPE_COMMON;
	EnQueue(PResFullSendBufferQueuePtr,QPtr);
	LINE_LOG();
	return 0;			

}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AtCmd_SendHugeBuffer
// ��������  : ������������ݰ�
// �������  : cmd,������; hugeBuffer,�����ݱ������ṹ��;
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_AtCmd_SendHugeBuffer(uint8 cmd,tagHugeSendBuffer hugeBuffer)
{
	tagSendBufferQueueItem* pItem = NULL;
	QueuePtr QPtr = NULL;
	int index = -1;
	LINE_LOG();
	index =  AtCmdAnalysis_AtCmd_GetIndex(cmd);
	if(index < 0)
		return -1;
	LINE_LOG();
	if((QPtr = DeQueue(PResFreeSendBufferQueuePtr)) == NULL )
		return -1;
	LINE_LOG();
	pItem = QPtr->data;
	pItem->SendAtCmd = &_gATCmd[index];
   	pItem->sendBufferType = SEND_BUFFER_TYPE_HUGE;
	pItem->hugeBuffer =  hugeBuffer;
	LINE_LOG();
	EnQueue(PResFullSendBufferQueuePtr,QPtr);
	LINE_LOG();
	return 0;			

}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_DeviceBeat_ReStart
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_DeviceBeat_ReStart(void)
{
	LINE_LOG();
	return API_SetTimer(_gModuleSetVal[0].beatTime *  30 * 1000,-1,TIMER_TYPE_WCDMA_HEART_BEAT,TIMER_NAME(DeviceBeat),NULL);	
}

int AtCmdAnalysis_DeviceBeat_ReStartByDeviceID(int deviceID,int tryCount)
{
	LINE_LOG();
	if(deviceID < MOUNT_DEVICE_COUNT && deviceID >=0)
	{
		_gModuleSetVal[deviceID].tryCount = tryCount;
		_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_INFO_SEND + deviceID].status = ATCMDGROUP_RUN_STATUS_OFF;
		return API_SetTimer(_gModuleSetVal[0].beatTime *  30 * 1000,-1,TIMER_TYPE_WCDMA_HEART_BEAT,TIMER_NAME(DeviceBeat),NULL);	
	}
	return -1;	
}
//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AtCmd_Resgister
// ��������  : ע��ATCMD������ 
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_AtCmd_Resgister(const tagATCmd* ATCmd,	 //ATCmd������
									int ATCmdCount,	//��������������
									const tagATCmdRecvProcessItem*	ATCmdRecvProcessItemArray,	//�ؼ��ֽ�����
									int ATCmdRecvProcessItemSize,	//�ؼ�����������
									const tagAtCmdRunCtrlItem* AtCmdRunFunTypeArray
									)
{
	if(ATCmd == NULL || ATCmdRecvProcessItemArray == NULL)
	{
		return -1;
	}
	_gATCmd = ATCmd;
	_gATCmdCount = ATCmdCount;
	_gtagATCmdRecvProcessItemArray = ATCmdRecvProcessItemArray;
	_gATCmdRecvProcessItemSize = ATCmdRecvProcessItemSize;
	_gAtCmdRunFunTypeArray	 = AtCmdRunFunTypeArray;
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_SetATCmdSendCallBack
// ��������  :  ���������гɹ���Ļص����� 
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_SetATCmdSendCallBack(ATCmdSendCallBack fun)
{
	LINE_LOG();
	_gATCmdSetInfo.aTCmdSendCallBack = fun;
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_RunGroup
// ��������  : ������ 
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_RunGroup(ATCMD_RUN_TYPE runType)
{
	LINE_LOG();
	__disable_irq();
	 if(_gAtCmdRunCtrlArray[runType].status == ATCMDGROUP_RUN_STATUS_OFF
	  && !_gAtCmdRunCtrlArray[runType].lock)
	 {
	 	_gAtCmdRunCtrlArray[runType].status = ATCMDGROUP_RUN_STATUS_ON;
	 	_gAtCmdRunCtrlArray[runType].lock = true;
		__enable_irq();
		return 0;
	 }
	 __enable_irq();
	 LINE_LOG();
	return -1;
}
//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_SelectAnlysisMode
// ��������  : ѡ�����ģʽ 
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_SelectAnlysisMode(PROTOCOL_ANALYSIS_MODE mode,int size)
{
	LINE_LOG();
	_gATCmdSetInfo.protocolAnlysisMode = mode;
	_gATCmdSetInfo.RecvSize = size;
	API_SetTimerFast(10000L,1,TIMER_TYPE_WCDMA_ANLYSIS_MODE,NULL,AtCmdAnalysis_AtCmd_EndAnlysisMode);
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_WirelessModuleVision_Get
// ��������  : ��ȡ����ģ��汾��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
char* AtCmdAnalysis_WirelessModuleVision_Get(void)
{
	return _gATCmdSetInfo.wirelssModuleVision;	
}
//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_CarrierOperator_Get
// ��������  : ��ȡ��Ӫ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
char* AtCmdAnalysis_CarrierOperator_Get(void)
{
	return _gATCmdSetInfo.carrierOperator;	
}	

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_RunError_EEROM_Record
// ��������  : ʵʱ�����¼
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_RunError_EEROM_Record(char* buf)
{
	tagErrorEERomContext menu;
	const int cellSize = 32;	//һ����Ԫ�ĳߴ�
	const int maxSize = 30;	//32���ֽ�Ϊһ����Ԫ,һ��У��λ	,Ԥ��һλ
	int size = 0;
	DWORD	address;
	if(buf == NULL)
		return -1;
	LINE_LOG();
	size = 	strlen(buf);

	size = size > maxSize?maxSize:size;

	if(API_EEPROM_Read(_EEP_ADDR_RUN_ERROR_RECORD_CONTEXT, sizeof(tagErrorEERomContext), (uchar*)(&menu), true) < 0)
	{
		memset(&menu,NULL,sizeof(menu));
	}
	if(menu.writePointer > menu.size)
	{
		menu.writePointer = 0;	
	}
	address = _EEP_ADDR_RUN_ERROR_RECORD + cellSize * menu.writePointer;

	if(API_EEPROM_Write(address,maxSize,(uchar*)buf,true) > -1)
	{
		if(menu.size < MAX_RUNERROR_CODE_SIZE)
		{
			menu.size++;
		}
		menu.writePointer++;
		if(menu.writePointer >= MAX_RUNERROR_CODE_SIZE)
		{
			menu.writePointer = 0;	
		}

		API_EEPROM_Write(_EEP_ADDR_RUN_ERROR_RECORD_CONTEXT, sizeof(tagErrorEERomContext), (uchar*)(&menu), true);
		LINE_LOG();
		return 0;
	}
		LINE_LOG();
	 return -1;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_SystemSetValCtrl_Get
// ��������  : ��ȡ��̨��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
tagSystemSetValStruct*	AtCmdAnalysis_SystemSetValCtrl_Get(void)
{
	return (&_gSystemSetVal);
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_CnsMod_Init
// ��������  : APN������ģʽ��ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_CnsMod_Init(tagCnsModSet* mod, int size)
{
	uint EepAddr;
	char m[10*MAX_APN_SIZE];
	int i = 0;
	EepAddr =  _EEP_ADDR_APN_VAL;
	LINE_LOG();
	_gCnsMod = mod;
	_gCnsModSize = size;
	
	if(API_EEPROM_Read(EepAddr,MAX_APN_SIZE * size,(uchar*)m,true) >=0)
	{
		for(i = 0; i < _gCnsModSize; i++)
		{
			//�ı�APN��ֵ
			memcpy(_gCnsMod[i].apn,&m[MAX_APN_SIZE * i],MAX_APN_SIZE);
		
		}
	}

	LINE_LOG();					

	Msg_CnsModInit(_gCnsMod,_gCnsModSize);
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_CnsMod_EERom_Story
// ��������  : APN������ģʽ�洢
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_CnsMod_EERom_Story(void)
{
	uint EepAddr;
	char m[10*MAX_APN_SIZE];
	int i = 0;
	EepAddr =  _EEP_ADDR_APN_VAL;

	for(i = 0; i < _gCnsModSize; i++)
	{
		//�ı�APN��ֵ
		memcpy(&m[MAX_APN_SIZE * i],_gCnsMod[i].apn,MAX_APN_SIZE);		
	
	}
	return API_EEPROM_Write(EepAddr,MAX_APN_SIZE * _gCnsModSize,(uchar*)m,true);

}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_CnsMod_Get
// ��������  : APN������ģʽ��ȡ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int AtCmdAnalysis_CnsMod_Get(tagCnsModSet** mod, int* size)
{
	*mod = _gCnsMod ;
	*size = _gCnsModSize;
	return 0;
}
int AtCmdAnslysis_3GNetVersion_Set(int mode)
{
	char* buf = NULL;
	WORD index = ATCMD_RUN_TYPE_SET3GNETVERSION;
	WORD size;
	switch(mode)
	{
	case 0:
		buf ="\"00\"";
		break;
	case 1:
		buf = "\"01\"";
		break;
	default:	
		return -1;;
	}
	size = ReadStrLen(buf);
	size = size > MAX_FUN_ARG_SIZE? MAX_FUN_ARG_SIZE:size;
	memset(_gAtCmdRunCtrlArray[index].funArg,NULL,MAX_FUN_ARG_SIZE);
	memcpy(_gAtCmdRunCtrlArray[index].funArg,buf,size);
	_gAtCmdRunCtrlArray[index].funArgSize = size;
	RUN_ATCMD_GROUP((ATCMD_RUN_TYPE)index);

	return 0;
}

int AtCmdAnslysis_MsgAnalysis99(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{
	
	return Msg_Buffer_Analysis99(uf,DaPtr,len);
}
/*����9AЭ��*/
int AtCmdAnslysis_MsgAnalysis9A(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{

	
	return Msg_Buffer_Analysis9A(uf,DaPtr,len);
}
