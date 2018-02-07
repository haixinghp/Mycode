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
#define MAX_ATCMD_LEN	30
#define CK(str1,str2)	 (StrNcmp((uchar*)str1,(uchar*)str2) == 0)

#define ATCMDRECV_NAME(fun) 	  ATCmdRecv_##fun
#define ATCMDRECV_FUN(fun)	static int ATCMDRECV_NAME(fun)##(uchar *Info,int len)
ATCMDRECV_FUN(CMGL);
ATCMDRECV_FUN(CSQ);
ATCMDRECV_FUN(CNSMOD);
ATCMDRECV_FUN(CPMS);
/*����*/
ATCMDRECV_FUN(Start);
/*������������*/
ATCMDRECV_FUN(Receive);
/*�������Ӵ���*/
ATCMDRECV_FUN(IpOpen);
/*�������ӹر���*/
ATCMDRECV_FUN(IpClose);
/*SIM����*/
ATCMDRECV_FUN(SIMFailed);
/*�������*/
ATCMDRECV_FUN(NetErr);
/*׼�����*/
ATCMDRECV_FUN(PbDone);
/*SIM������*/
ATCMDRECV_FUN(SmsDone);
ATCMDRECV_FUN(CMGRD);
ATCMDRECV_FUN(CIPOPEN);

/*ATCmd ��ʼ��*/
static int AtCmdAnalysis_ATCmdRunCtrlArray_Init(void);

#define ATCMDGROUP_NAME(fun) 	  ATCmdGroup_##fun
#define ATCMDGROUP_FUN(fun)	static int ATCMDGROUP_NAME(fun)##(void)

#define ATCMDGROUP_CALLBACK_NAME(fun) 	  ATCmdGroupCallBack_##fun
#define ATCMDGROUP_CALLBACK_FUN(fun) static int ATCMDGROUP_CALLBACK_NAME(fun)(const tagATCmd* atCmd,WORD errType)
	
ATCMDGROUP_CALLBACK_FUN(DeviceSend);
ATCMDGROUP_CALLBACK_FUN(Init);

/*��ѯ��*/
ATCMDGROUP_FUN(DoLoop);

ATCMDGROUP_FUN(Restart);

ATCMDGROUP_FUN(Init);

ATCMDGROUP_FUN(ReadMsg);

ATCMDGROUP_FUN(SendMsg);

ATCMDGROUP_FUN(ConnectNet);

ATCMDGROUP_FUN(DisConnectNet);

ATCMDGROUP_FUN(DeviceSend);

ATCMDGROUP_FUN(InfoSend);

#define TIMER_NAME(fun) 	  Timer_##fun
#define TIMER_FUN(fun)	static void TIMER_NAME(fun)##(void)
#define TIMER_RUN(fun)	  TIMER_NAME(fun)()

TIMER_FUN(StartToSendAtCmd);
/*��ʱ�����������������ķ�����������*/
TIMER_FUN(DeviceBeat);

TIMER_FUN(StartOk);

TIMER_FUN(MainLoop);
		
typedef struct
{
	char*	ATcmd;	
	ATCmdRecvProcessFun	fun;
}tagATCmdRecvProcessItem;

static const tagATCmd	 _gATCmd[]={
//���						����ͷ����						Ӧ��ͷ����		ʧ��Ӧ��ͷ		�ȴ�Ӧ��ʱ��	�ط�����	��Ӧ����
{AT_CMD_TEST_AT,         {"AT"},	                        {"OK",""},		{""},		    1000,	       	5,			NULL	,true},	//����CDMA�Ƿ��з���
{AT_CMD_SET_CFUN,        {"AT+CFUN=1"},	                 	{"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//��������ģʽ
{AT_CMD_GET_CSQ,         {"AT+CSQ"},	                    {"+CSQ:",""},	{""},	1000,	       	5,			ATCMDRECV_NAME(CSQ)   ,true },	//��ѯ�����ź�
{AT_CMD_GET_CREG,        {"AT+CREG?"},	                 	{"+CREG: ",""}, {""},  1000,	       	5,			NULL    		,true},	//����ע����Ϣ
{AT_CMD_GET_CICCID,      {"AT+CICCID"},	                 	{"+ICCID: ",""},{""},	1000,	       	5,			NULL    		,true},	//��ѯUIM���к�
{AT_CMD_GET_IPR,         {"AT+IPR?"},	                 	{"+IPR: ",""},	{""},    1000,	       	5,			NULL    		,true},	//��ѯ������
{AT_CMD_SET_IPR,         {"AT+IPR="},	                 	{"OK",""},	    {""},    1000,	       	5,			NULL    		,true},	//���ò�����
//******************************************����*******************************************************************
{AT_CMD_SET_CMGF,        {"AT+CMGF="},	                 	{"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//���ö��Ÿ�ʽ 0,PDUģʽ 1,textģʽ
{AT_CMD_SET_CSCS,        {"AT+CSCS="},			            {"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//ѡ��TE�ַ���GSM��UCS2��IRA
{AT_CMD_STE_CPMS,       {"AT+CPMS=\"SM\",\"SM\",\"SM\""}, 	{"+CPMS",""},	{""},	1000,	       	5,			ATCMDRECV_NAME(CPMS)  ,true},	//���Ŵ洢λ��ѡ������
{AT_CMD_SET_CNMI,       {"AT+CNMI=2,1"},	  	            {"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//�����¶���Ϣ��ʾ���� 2,1:�洢���� 1,2:���洢���ţ��¶���ֱ�Ӵ�ӡ������
{AT_CMD_SET_CSMP,       {"AT+CSMP=17,167,0,"},         	{"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//����TEXTģʽ���������һ��Ϊ���ݱ������ͣ�0��ʾĬ���ַ���(GSM) 241:�ַ� 25:����
{AT_CMD_SET_CMGSO,       {"AT+CMGSO="},	                 	{"OK",""},		{"ERROR"},    10000,	       	2,			NULL    		,true},	//���ٷ���Ϣ
{AT_CMD_SET_CMGS,		{"AT+CMGS="},						{">",""},		{""},	500,			1,			NULL			,true},
{AT_CMD_SET_CMGSDATA,	{""},								{"+CMGS","OK"},	{""},	5000,			1,			NULL			,true},
{AT_CMD_SET_CMGR,        {"AT+CMGR="},	                 	{"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//��ȡ���ţ�����λ��
{AT_CMD_SET_CMGRD,      {"AT+CMGRD="},	                 	{"+CMGRD:",""}, {""},   2000,	       	3,			ATCMDRECV_NAME(CMGRD) ,false},	//��ȡ��Ϣ���SIM ���н�����Ϣɾ��������λ�� 
{AT_CMD_SET_CMGD,       {"AT+CMGD=,4"},	                 	{"OK",""},		{""},    3000,	       	5,			NULL    		,true},	//ɾ����ѡ�洢���ϵĶ��ţ�ģʽ
{AT_CMD_SET_CMGL,       {"AT+CMGL="},		       			{"+CMGL:","OK"},		{""},    1000,	1,			ATCMDRECV_NAME(CMGL)    		,true},	//��ȡ���� "REC UNREAD"�յ�δ����Ϣ "REC READ"�յ��Ѷ���Ϣ "STO UNSENT"δ������Ϣ "STO SENT"�洢�ѷ�����Ϣ "ALL"ȫ����Ϣ
{AT_CMD_REV_MSDATA,      {""},	                         	{"+CMT:",""},	{""},	1000,	       	0,			NULL    		,true},	//�յ��������� AT+CNMI=1,2
{AT_CMD_REV_MSMNOTE,    {""},                             	{"+CMTI:",""},  {""},   1000,         	0,			NULL    		,true},  //�յ��������� AT+CNMI=2,1
{AT_CMD_GET_CMGRO,		{"AT+CMGRO="},						{"OK",""},		{""},	1000,			1,			NULL			,true},	//��ѯ����
//******************************************����*******************************************************************
{AT_CMD_SET_CNMP,     	 {"AT+CNMP="},                     	{"OK",""},		{""},   1000,	       	5,			NULL    		,true},	//������������ѡ��ģʽ 2,�Զ� 13,GSM 14,WCDMA
{AT_CMD_SET_CIPSRIP,    {"AT+CIPSRIP=0"},                 	{"OK",""},		{""},   1000,	       	5,			NULL    		,true},	//����ʾ����IP
{AT_CMD_SET_CGSOCKCONT, {"AT+CGSOCKCONT=1,\"IP\","},      	{"OK",""},		{""},   1000,	       	5,			NULL    		,true},	//����APN 
{AT_CMD_SET_NETOPEN,    {"AT+NETOPEN=,,1"},               	{"Network opened",""},{""},		    30000,	       	2,			NULL    ,true},	//�����ӷ�ʽ
{AT_CMD_SET_CIPOPEN,    {"AT+CIPOPEN="},	                {"Connect ok","+IP ERROR: Connection is already created"},{"Connect fail"},	20000,	       	1,			ATCMDRECV_NAME(CIPOPEN)    ,true},	//����һ·TCP/IP ���� 
{AT_CMD_SET_CIPSEND,    {"AT+CIPSEND="},	                {">",""},		  {""},  500,	       	1,			NULL    		,true},	//��һ·TCP/IP �·�������(����) 
{AT_CMD_SET_TCPWDATA,   {""},	                         	{"Send ok",""},	{""},	10000,	       	1,			NULL    	,true},	//��һ·TCP/IP �·�������(������) 
{AT_CMD_SET_NETCLOSE,  {"AT+CIPCLOSE="},	             	{"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//�ر�һ·TCP/IP����
{AT_CMD_GET_CIPOPEN,    {"AT+CIPOPEN?"},	                {"OK",""},      {""},   500,	       	1,			NULL    		,false},	//��ѯ��������״̬ 
{AT_CMD_REV_NETDATA,    {""},	                         	{"+IPD",""},	{""},	1000,	       	0,			NULL    		,true},	//�յ���������
//******************************************************************************************************************
{AT_CMD_GET_CNUM,       {"AT+CNUM"},	                    {"+CNUM:",""},	{""},	1000,	       	5,			NULL    		,true},	//��ѯ��������	 
{AT_CMD_SET_CNAOP,      {"AT+CNAOP=0"},	                 	{"OK",""},		{""},   1000,	       	5,			NULL    		,true},	//��������˳��0��Automatic	1��GSM,WCDMA  2��WCDMA,GSM  
{AT_CMD_GET_CNSMOD,     {"AT+CNSMOD?"},	                 	{"+CNSMOD:",""},{""},   1000,	       	5,			ATCMDRECV_NAME(CNSMOD),true},	//��ѯ����ϵͳģʽ
{AT_CMD_SET_ATE,        {"ATE0"},	                     	{"OK",""},      {""},   1000,	       	5,			NULL    		,true},	//�����رջ�������
{AT_CMD_SET_CIPCCFG,    {"AT+CIPCCFG=3,0,1,1,1"},	     	{"OK",""},      {""},   1000,	       	5,			NULL    		,true},  //���ò������׽���
//******************************************************************************************************************* 
};
#define AT_CMD_COUNT	sizeof(_gATCmd)/sizeof(tagATCmd)

static const tagATCmdRecvProcessItem	_gtagATCmdRecvProcessItemArray[] =
{
	{"+CIPOPEN:",ATCMDRECV_NAME(IpOpen)},		  //����״̬
	{"+RECEIVE",ATCMDRECV_NAME(Receive)},		  //������������

	{"+IPCLOSE:",ATCMDRECV_NAME(IpClose)},		  //δ����״̬
	{"+IP ERROR:",ATCMDRECV_NAME(NetErr)},	//�������
	{"+CME ERROR: SIM failure",ATCMDRECV_NAME(SIMFailed)}, 	//SIM��������
	{"START",ATCMDRECV_NAME(Start)},	 	//����
	{"PB DONE",ATCMDRECV_NAME(PbDone)}, 				//�ϵ����
	{"SMS DONE",ATCMDRECV_NAME(SmsDone)}
};	
#define RECV_AT_CMD_SIZE	(sizeof(_gtagATCmdRecvProcessItemArray)/sizeof(tagATCmdRecvProcessItem))

typedef enum
{
	ATCMDGROUP_RUN_STATUS_ON,
	ATCMDGROUP_RUN_STATUS_OFF,
	ATCMDGROUP_RUN_STATUS_ALWAYSON 	
}ATCMDGROUP_RUN_STATUS;
typedef int (*ATCmd_Run_Fun)(void);
#define MAX_FUN_ARG_SIZE	100

typedef struct
{
	ATCMD_RUN_TYPE	type;
	ATCmd_Run_Fun fun;	//�����еĳ���
	char	funArg[MAX_FUN_ARG_SIZE];		//�����г���Ĳ���
	WORD	funArgSize;						//������С
	ATCMDGROUP_RUN_STATUS status;	//���е�״̬
	BOOL lock;		//״̬��	
}tagAtCmdRunCtrlItem;

//��ʼ�����ñ�
const static  tagAtCmdRunCtrlItem _gAtCmdRunFunTypeArray[ATCMD_RUN_TYPE_MAX] =
{
	/*��������										������ִ�к���						���������	��������	������ִ��״̬				״̬��*/
	{ATCMD_RUN_TYPE_RESTART,							ATCMDGROUP_NAME(Restart),		"",			0,			ATCMDGROUP_RUN_STATUS_OFF,  false},
	{ATCMD_RUN_TYPE_INIT, 								ATCMDGROUP_NAME(Init),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{ATCMD_RUN_TYPE_READ_MSG,							ATCMDGROUP_NAME(ReadMsg),		"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{ATCMD_RUN_TYPE_SEND_MSG,							ATCMDGROUP_NAME(SendMsg),		"",			0,			ATCMDGROUP_RUN_STATUS_OFF,  false},
	{ATCMD_RUN_TYPE_DISCONNECT_NET,						ATCMDGROUP_NAME(DisConnectNet),	"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_CONNECT_NET + 0),	ATCMDGROUP_NAME(ConnectNet),		"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_CONNECT_NET + 1),	ATCMDGROUP_NAME(ConnectNet),		"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_DEVICE_SEND +0), 	ATCMDGROUP_NAME(DeviceSend),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_DEVICE_SEND +1),	ATCMDGROUP_NAME(DeviceSend),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_INFO_SEND +0),		ATCMDGROUP_NAME(InfoSend),				"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_INFO_SEND +1),		ATCMDGROUP_NAME(InfoSend),				"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
 	{ATCMD_RUN_TYPE_LOOP,								ATCMDGROUP_NAME(DoLoop),				"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false}
};
static tagAtCmdRunCtrlItem _gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_MAX];

#define RUN_ATCMD_GROUP(ATCMD_RUN_TYPE)	  if(_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE].status == ATCMDGROUP_RUN_STATUS_OFF && !_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE].lock)	\
			 {_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE].status = ATCMDGROUP_RUN_STATUS_ON,	   \
			 _gAtCmdRunCtrlArray[ATCMD_RUN_TYPE].lock = true;								\
			 }

#define MAX_QUEUE_CALLBACK_SIZE		5
static tagCallBackQueueItem	_gCallBackQueueItemArray[MAX_QUEUE_CALLBACK_SIZE] = {0};


#define MAX_SEND_DATA_SIZE_ONCE	960		//���η����ļ����ݴ�С
#define MAX_QUEUE_SEND_BUFFER_SIZE	20

#define MAX_HUGE_BUFFER_SIZE	5
typedef struct
{
	int Count;
	char* Buffer[MAX_HUGE_BUFFER_SIZE];
	uint16_t size[MAX_HUGE_BUFFER_SIZE];
}tagHugeSendBuffer;
typedef enum
{
	SEND_BUFFER_TYPE_COMMON,
	SEND_BUFFER_TYPE_HUGE
}SEND_BUFFER_TYPE;
typedef struct
{
   const tagATCmd*	SendAtCmd; 		//AT�����
   uchar	sendBuffer[100]; 		//�����͵���չ����,������Ҫƥ���������ݳ���
   int 		sendBufferSize;			//�����͵���չ���ݳ���
   SEND_BUFFER_TYPE sendBufferType;	//�����������ͣ������ݣ���ͨ����
   tagHugeSendBuffer hugeBuffer;	//�����������ṹ��

}tagSendBufferQueueItem;
static tagSendBufferQueueItem _gSendBufferQueueItemArray[MAX_QUEUE_SEND_BUFFER_SIZE] = {0};

typedef enum
{
	SEND_ERROR_TYPE_NONE	= 0x0000,	//û���κδ���
	SEND_ERROR_TYPE_UNKONWN	= 0x0001,	//δ֪ԭ���µĴ���
}SEND_ERROR_TYPE;


tagATCmdSetInfo _gATCmdSetInfo = {0};
#define SetATCmdSendCallBack(fun)	_gATCmdSetInfo.aTCmdSendCallBack = fun	

#define CLEAR_ERROR_TYPE()  _gATCmdSetInfo.errBits = SEND_ERROR_TYPE_NONE
#define SET_ERROR_TYPE(errCode)	 _gATCmdSetInfo.errBits |= 	errCode
#define CHECK_ERROR()	(_gATCmdSetInfo.errBits != SEND_ERROR_TYPE_NONE)
#define	_gRecvFunction _gATCmdSetInfo.RecvFunction
#define _gSendAtCmd	_gATCmdSetInfo.SendAtCmd	
#define AT_CALL_BACK_MODE()	(_gSendAtCmd != NULL)
#define	SEL_ANLYSIS_MODE(mode,size)	{_gATCmdSetInfo.protocolAnlysisMode = mode;		 \
									_gATCmdSetInfo.RecvSize = size;	\
									API_SetTimerFast(10000L,1,TIMER_TYPE_WCDMA_ANLYSIS_MODE,NULL,AtCmdAnalysis_AtCmd_EndAnlysisMode);	\
									}

static LinkQueue	_gLinkQueue[2]	= {0};
#define PResFreeQueue	_gLinkQueue[0]		//���еĶ���
#define	PResFullQueue	_gLinkQueue[1]	   	//��æ�Ķ���
#define	PResFreeQueuePtr	&PResFreeQueue
#define	PResFullQueuePtr	&PResFullQueue
/*���ջ���������*/

static LinkQueue	_gSendBufferLinkQueue[2]	= {0};
#define PResFreeSendBufferQueue	_gSendBufferLinkQueue[0]		//���еĶ���
#define	PResFullSendBufferQueue	_gSendBufferLinkQueue[1]	   	//��æ�Ķ���
#define	PResFreeSendBufferQueuePtr	&PResFreeSendBufferQueue
#define	PResFullSendBufferQueuePtr	&PResFullSendBufferQueue

#define PROCESS_NAME(fun) 	  ATCmdRecv_Process_##fun
#define ATCMDRECV_PROCESS(fun)	static int PROCESS_NAME(fun)##(int argc, char	argv[][MAX_CALL_BACK_PARAMETER_SIZE])

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
		1,
	}
 };


static 	tagModeRunStateStruct	_gModeRunState = {0};

/******************************************************************************/
//�ڲ���������ʼ
#define _INTR(fun)	fun


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

	EepAddr =  _EEP_ADDR_MODULE_VAL; 
	  						
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

	EepAddr =  _EEP_ADDR_MODULE_VAL;

	memcpy(m,_gModuleSetVal,sizeof(tagModuleSetValStruct) * MOUNT_DEVICE_COUNT);  
	if(API_EEPROM_Read(EepAddr,sizeof(tagModuleSetValStruct) * MOUNT_DEVICE_COUNT,(uchar*)_gModuleSetVal,true) < 0)
	{
		memcpy(_gModuleSetVal,m,sizeof(tagModuleSetValStruct) * MOUNT_DEVICE_COUNT);
		return -1;  
	}
 						
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
	_gATCmdSetInfo.protocolAnlysisMode = PROTOCOL_ANALYSIS_MODE_NONE;
	
}
//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_DeviceBeat_ReStart
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void _INTR(AtCmdAnalysis_DeviceBeat_ReStart)(void)
{
	API_SetTimer(_gModuleSetVal[0].beatTime *  30 * 1000,-1,TIMER_TYPE_WCDMA_HEART_BEAT,TIMER_NAME(DeviceBeat),NULL);	
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
static int _INTR(AtCmdAnalysis_AtCmd_GetIndex)(AT_CMD cmd)
{
	int i = 0;

	for(i=0; i < AT_CMD_COUNT;i++)
	{
		if(_gATCmd[i].ATCmd == cmd)
			return i; 
	}

	return -1;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AtCmd_Send
// ��������  : ����AT��������
// �������  : CmdNo,������; Info,�������������; len,�������ݳ���;
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

static int _INTR(AtCmdAnalysis_AtCmd_Send)(AT_CMD cmd,char *buf,int len)
{
	tagSendBufferQueueItem* pItem = NULL;
	QueuePtr QPtr = NULL;
	int index = -1;

	index =  AtCmdAnalysis_AtCmd_GetIndex(cmd);
	if(index < 0)
		return -1;

	if((QPtr = DeQueue(PResFreeSendBufferQueuePtr)) == NULL )
		return -1;
	pItem = QPtr->data;
	pItem->SendAtCmd = &_gATCmd[index];
	if(buf != NULL)
	{
   		memcpy(pItem->sendBuffer,buf,len);
		pItem->sendBufferSize = len;
   	}
	pItem->sendBufferType = SEND_BUFFER_TYPE_COMMON;
	EnQueue(PResFullSendBufferQueuePtr,QPtr);

	return 0;			

}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_AtCmd_SendHugeBuffer
// ��������  : ������������ݰ�
// �������  : cmd,������; hugeBuffer,�����ݱ������ṹ��;
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int _INTR(AtCmdAnalysis_AtCmd_SendHugeBuffer)(AT_CMD cmd,tagHugeSendBuffer hugeBuffer)
{
	tagSendBufferQueueItem* pItem = NULL;
	QueuePtr QPtr = NULL;
	int index = -1;

	index =  AtCmdAnalysis_AtCmd_GetIndex(cmd);
	if(index < 0)
		return -1;

	if((QPtr = DeQueue(PResFreeSendBufferQueuePtr)) == NULL )
		return -1;

	pItem = QPtr->data;
	pItem->SendAtCmd = &_gATCmd[index];
   	pItem->sendBufferType = SEND_BUFFER_TYPE_HUGE;
	pItem->hugeBuffer =  hugeBuffer;

	EnQueue(PResFullSendBufferQueuePtr,QPtr);

	return 0;			

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

	if(_gATCmdSetInfo.atCmdRunStatus ==  ATCMD_RUN_STATUS_RUN_BUSY)
		return -1;
	_gATCmdSetInfo.aTCmdSendCallBack = NULL;

	CLEAR_ERROR_TYPE();	
	for(i = 0; i < ATCMD_RUN_TYPE_MAX;i++)
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

	for(i=0; i < ATCMD_RUN_TYPE_MAX;i++)
	{
		memcpy(&_gAtCmdRunCtrlArray[_gAtCmdRunFunTypeArray[i].type],&_gAtCmdRunFunTypeArray[i],sizeof(tagAtCmdRunCtrlItem));
	}

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
	if(deviceID < MOUNT_DEVICE_COUNT )
	{
		tagProtocolAnalysis pa;
		char data[51] = "";

		char* pBuf = _gModuleSetVal[deviceID].beatString;
		char* pCid= NULL; 

		size_t dataSize = sizeof(data);
		int cidLen = ReadStrLen(_gModuleSetVal[deviceID].CID);

		memcpy(data +(20-cidLen),_gModuleSetVal[deviceID].CID,cidLen);
		pCid = data +(20-cidLen);
		while(cidLen--)
		{
			(*pCid) = (*pCid) - '0';
			pCid++;	
		}

	 	if(Netdata_ProtocolAnalysis_Packet(&pa,PROTOCOL_ANALYSIS_TYPE_DEVICE_BEAT,data, dataSize) > -1)
		{
			//�ı䵱ǰ��״̬Ϊδ����״̬
			memcpy(pBuf,pa.headerSender,MAX_PROTOCOL_HEADER_SIZE);
			pBuf += MAX_PROTOCOL_HEADER_SIZE;
			memcpy(pBuf,data,dataSize);
			pBuf +=  dataSize;
			memcpy(pBuf,pa.tailSender,MAX_PROTOCOL_TAIL_SIZE);
			return 0;
		}
	}		

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

	memset(&_gModeRunState,NULL,sizeof(tagModeRunStateStruct));

	for(index =0; index < MOUNT_DEVICE_COUNT; index++)
	{
		_gModeRunState.NetIsOpen[index] = MAX_NET_IS_OPEN_NUM -1;
	}

	return 0;
}
//�ڲ������������
/******************************************************************************/
#define ERROR_PROCCESS_NAME(fun) 	  Error_Process_##fun
#define ERROR_PROCCESS(fun) static int ERROR_PROCCESS_NAME(fun)(char* addr, size_t size)

ERROR_PROCCESS(Init);
ERROR_PROCCESS(Device);
ERROR_PROCCESS(System);
ERROR_PROCCESS(SimCard);
ERROR_PROCCESS(Net);
const static tagErrorCallBack _gErrorCallBack[] = 
{
	{WCDMA_ERROR_INIT,ERROR_PROCCESS_NAME(Init)},		//��ʼ��û�ɹ�
	{WCDMA_ERROR_DEVICE,ERROR_PROCCESS_NAME(Device)},		//�豸���Ĵ���
	{WCDMA_ERROR_SYSTEM,ERROR_PROCCESS_NAME(System)},			//ϵͳ����
	{WCDMA_ERROR_NO_SIM_CARD,ERROR_PROCCESS_NAME(SimCard)},	//û��SIM��
	{WCDMA_ERROR_NET_DISCONNECT,ERROR_PROCCESS_NAME(Net)},	//�������Ӳ���

};
#define ERROR_CALL_BACK_SIZE	sizeof(_gErrorCallBack)/sizeof(tagErrorCallBack)

#define MAX_ERROR_CODE_SIZE	15

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
	Date_Time dataTime;
	char bufs[256] = {0};
	DWORD	address;
	if(API_EEPROM_Read(_EEP_ADDR_ERROR_RECORD_CONTEXT, sizeof(tagErrorEERomContext), (uchar*)(&menu), true) < 0)
	{
		memset(&menu,NULL,sizeof(menu));
	}
	if(menu.writePointer > menu.size)
	{
		menu.writePointer = 0;	
	}
	address = _EEP_ADDR_ERROR_RECORD + 256 * menu.writePointer;

	memset(&dataTime,NULL,sizeof(dataTime));
	sprintf(bufs,
		"%.2d-%.2d-%.2d %.2d:%.2d:%.2d %s %s\n",
		dataTime.StuDate.Year,
		dataTime.StuDate.Month,
		dataTime.StuDate.Day,
		dataTime.StuDate.Hour,
		dataTime.StuDate.Minute,
		dataTime.StuDate.Second,
		title,
		buf
		);//��¼ʱ��
	DEBUG("%s\n",bufs);
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
		return 0;
	}

	 return -1;
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
	return Error_EEROM_Record("Net Error",addr);
}
 
 /******************************************************************************/

typedef enum {
	WIRELESS_ERROR_SIM,		//SIM��������
	WIRELESS_ERROR_NET 		//���������
}WIRELESS_ERROR;	//����ģ�����



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

	if(argc < 1)
		return -1;		
	uf = (USART_FUNCTION)atoi(argv[0]);

	if(uf < USART_FUNCTION_MAX)
		return API_StartSendBuffer(uf);
	return -1;		
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

	if(argc < 1)
		return -1; 		
	uf = (USART_FUNCTION)atoi(argv[0]);
	if(uf < USART_FUNCTION_MOUNT ||uf >= USART_FUNCTION_MOUNT + MOUNT_DEVICE_COUNT)
		return -1;

	deviceID = uf - USART_FUNCTION_MOUNT;
	Netdata_System_Ctrl(deviceID);

	return 0;
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

	if(argc < 1)
		return -1; 		
	uf = (USART_FUNCTION)atoi(argv[0]);
	if(uf < USART_FUNCTION_MOUNT ||uf >= USART_FUNCTION_MOUNT + MOUNT_DEVICE_COUNT)
		return -1;

	deviceID = uf - USART_FUNCTION_MOUNT;
	Netdata_Program_Update(deviceID);

	return 0;		
}
// ��������  : ATCMDRECV_PROCESS(PbDone)
// ��������  : ģ���������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_PROCESS(PbDone)
{
	API_SetTimer(60000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);
	return 0;
}

// ��������  : ATCMDRECV_PROCESS(EnableHeartBeat)
// ��������  : ��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_PROCESS(EnableHeartBeat)
{
	BOOL enable;
	WORD port;

	if(argc < 2)
		return -1;
	enable = (BOOL)atoi(argv[0]);
	port =  atoi(argv[1]);

	if(port < MOUNT_DEVICE_COUNT)
	{
		if(enable)	 //˵������ͨ��
		{
			API_Led_Set(LED_TYPE_2, LED_STATE_ON);
	  		TIMER_RUN(DeviceBeat);
		}
		else
		{
			if(_gModeRunState.NetIsOpen[port] > 0)
			{
				API_Led_Set(LED_TYPE_2, LED_STATE_OFF);
				if(_gModeRunState.NetIsOpen[port] == MAX_NET_IS_OPEN_NUM)
				{
					_gModeRunState.NetIsOpen[port]--;
					//�ڶ�����һɲ��ִ���º���
					API_SetTimer(20L,1,TIMER_TYPE_WCDMA_AT_SEND,NULL,TIMER_NAME(StartToSendAtCmd));
				}
			}
		}
	}
		
	return 0;
}
// ��������  : ATCMDRECV_PROCESS(NoticeError)
// ��������  : ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_PROCESS(NoticeError)
{
	int index = 0;

	if(argc < 1)
		return -1;
	for(index = 0; index < MOUNT_DEVICE_COUNT; index++)
	{
		if(_gModeRunState.NetIsOpen[index] > 0)
		{
			_gModeRunState.NetIsOpen[index]--;
		}
	}

	API_SetTimer(20L,1,TIMER_TYPE_WCDMA_AT_SEND,NULL,TIMER_NAME(StartToSendAtCmd));

	return 	0;
}
// ��������  : ATCMDRECV_PROCESS(SIMFailed)
// ��������  : sim��������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_PROCESS(SIMFailed)
{
	_gModeRunState.SimCard = SIM_CARD_STATUS_ERROR;	
	API_SetTimer(60000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);
	AtCmdAnalysis_ErrorMsg(WCDMA_ERROR_NO_SIM_CARD,"",0);
	return 0;
}

/******************************************************************************/
// ��������  : ATCMDRECV_FUN(Start)
// ��������  : ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(Start)
{
	return 0;
}

// ��������  : ATCMDRECV_FUN(PbDone)
// ��������  : ģ���������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(PbDone)
{
	START_CMD(PbDone);
	END_CMD();
	DEBUG("PbDone Cmd Ok\n");

	if(_gModeRunState.SimCard == SIM_CARD_STATUS_OK)
	{
		RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_INIT);
	}
	return 0;
}

// ��������  : ATCMDRECV_FUN(SmsDone)
// ��������  : Sim�����OK
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(SmsDone)
{
	_gModeRunState.SimCard = SIM_CARD_STATUS_OK;	
	return 0;
}

// ��������  : ATCMDRECV_FUN(Receive)
// ��������  : ���������ݼ�������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(Receive)
{
	char *p;
	int length;
	int port;
	char buf[50] = {0};
	/*��ȡ���ݽ��յĶ˿ڱ��*/

	p = GetStrFromTwoKey((char*)Info, ",",",",buf ,sizeof(buf));

	if(p != NULL)
	{
		port = atoi(buf);
		p = GetStrFromTwoKey(p, ",",":",buf ,sizeof(buf));
		if(p != NULL)
		{
			length= atoi(buf);
			_gATCmdSetInfo.RecvFunction = 	(USART_FUNCTION)(port + USART_FUNCTION_MOUNT);
			SEL_ANLYSIS_MODE(PROTOCOL_ANALYSIS_MODE_RECEIVE,length);
			return 0;
		}
	}


	return -1;//API_Wireless_StartReceive(port,length);
}

// ��������  : ATCMDRECV_FUN(IpOpen)
// ��������  : �������Ӵ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(IpOpen)
{
	int l;
	char* p	;

	BOOL ret = false;
	WORD portInt = 0;

	p = (char*)(Info+10);                 
	portInt = (*p) - '0';

	if(portInt >= MOUNT_DEVICE_COUNT)
		return -1;

	if(_gModuleSetVal[portInt].deviceFun != 1)
		return -1;

	/*��֡β�س����������ַ���������*/
	/*�ж��ַ�����,����ַ����ȴ���2,˵���˶Ͽ��Ѿ���������*/
	l = ReadStrLen((char*)(Info+10));

	if(l > 5)
	{ 
		ret = true;
	}
	else
	{	
		ret = false;
	}


	if(ret)	 //˵������ͨ��
	{
		 if(_gModeRunState.NetIsOpen[portInt] != MAX_NET_IS_OPEN_NUM)	//���ָ̻�
		 {
		  	_gModeRunState.NetIsOpen[portInt] = MAX_NET_IS_OPEN_NUM;
			START_CMD(EnableHeartBeat);
			ADD_ARGV_BOOL(true);
			ADD_ARGV_INT(portInt);
			END_CMD();
		 }	
	}
	else
	{
		if(_gModeRunState.NetIsOpen[portInt] > 0)
		{
		 	if(_gModeRunState.NetIsOpen[portInt] == MAX_NET_IS_OPEN_NUM)	//���ָ̻�
		  	{
			 	API_SetTimerFast(20L,1,TIMER_TYPE_WCDMA_AT_SEND,NULL,TIMER_NAME(StartToSendAtCmd));
				
				//�˿ڶ���һɲ��
		  		START_CMD(EnableHeartBeat);
				ADD_ARGV_BOOL(false);
				ADD_ARGV_INT(portInt);
				END_CMD();	
		  	}	
			_gModeRunState.NetIsOpen[portInt]--;
		}
		else
		{
			int index = 0;
	
			for(index = 0; index < MOUNT_DEVICE_COUNT; index++)
			{
				if(_gModeRunState.NetIsOpen[index] > 0)
					break;
			}
	
			if(index >= MOUNT_DEVICE_COUNT)
			{
				char buf[30] = {0};
				_gModeRunState.NetIsOpen[portInt] = MAX_NET_IS_OPEN_NUM-1;
				sprintf(buf,"Service %d disConnect",portInt);
				AtCmdAnalysis_ErrorMsg(WCDMA_ERROR_NET_DISCONNECT,buf,strlen(buf));
				AtCmd_RECV_WCDMA_Init();
			}
	
			//�����豸			
		}
	}

	return 0;
}

// ��������  : ATCMDRECV_FUN(IpClose)
// ��������  : �������ӹر���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(IpClose)
{
	char *port;

	port=InStr((char*)Info, ",",len);
	if(port == NULL)
		return -1;
	port++;
	DEBUG("EnableHeartBeat Cmd Ok\n");
	START_CMD(EnableHeartBeat);
	ADD_ARGV_BOOL(false);
	ADD_ARGV(port);
	END_CMD();
	return 0;
}

// ��������  : ATCMDRECV_FUN(SIMFailed)
// ��������  : SIM����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(SIMFailed)
{
	DEBUG("SIMFailed Cmd Ok\n");
	START_CMD(SIMFailed);
	ADD_ARGV_INT(WIRELESS_ERROR_SIM);
	END_CMD();
	return 0;
}

// ��������  : ATCMDRECV_FUN(NetErr)
// ��������  : �������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(NetErr)
{
	if(InStr((char*)Info,"Connection is already created",len) == NULL)
	{	
	 	START_CMD(NoticeError);
		ADD_ARGV_INT(WIRELESS_ERROR_NET);
		END_CMD();
	}
	else
	{
		  
	}
	return 0;//API_Wireless_NoticeError(WIRELESS_ERROR_NET);
}

// ��������  : ATCMDRECV_FUN(CPMS)
// ��������  : ���ղ�ѯ���ж�������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(CPMS)
{
	char* p = NULL;
	int count;

	p = (char*)(Info+7);
	count = String_To_Int(&p , ",");
	/*�ж���Ϣ*/
	if(count>0 && _gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_READ_MSG);		
	}

	return 0;
}

// ��������  : ATCMDRECV_FUN(CNSMOD)
// ��������  : ��ѯ����ϵͳģʽ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(CNSMOD)
{
	char* p = NULL;

	p=InStr((char*)Info,",",len);
	if(p!=NULL)
	{
		p++;
		_gModeRunState.CnsMod  = String_To_Int(&p,"\0");

	}

	return 0;

}

// ��������  : ATCMDRECV_FUN(CSQ)
// ��������  : ��ѯ�����ź�
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(CSQ)
{
	int16 csq;
	char buf[20] = {0}; 

	if(GetStrFromTwoKey((char*)Info, ":",",",buf ,len) != NULL)
	{
		csq = atoi(buf);
		csq = csq > 98?0:csq;
		csq = csq < 0?0:csq;
		_gModeRunState.CSQ =  csq;
	}

	return 0;
}

// ��������  : ATCMDRECV_FUN(CMGL)
// ��������  : ��ȡ��Ϣ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(CMGL)
{
	if(!CK(Info,"OK"))
	{
		int Start = 30;
		while((*Info != '\"' || *(Info+1) != ',') &&  Start > 0)
		{
			Start--;
			Info++;
		}
		//�ҵ����;
		Info += 2;
		Start = 0;
		while(Info[Start] != ',' && Start < MAX_MSG_PHONE_SIZE)
		{
			Start++;
		}
		if(Start < MAX_MSG_PHONE_SIZE)
		{
			_gATCmdSetInfo.haveShortMsg = true;
			SEL_ANLYSIS_MODE(PROTOCOL_ANALYSIS_MODE_SHORT_MESSAGE,-1);
			memcpy(_gATCmdSetInfo.phoneNum,Info,Start);
	
		}
		//GetStrFromTwoKey((char*)Info, "\",",",",_gATCmdSetInfo.phoneNum ,len);
	}

	return 0;	
	
}
// ��������  : ATCMDRECV_FUN(CMGRD)
// ��������  : ��ȡ��Ϣ���SIM ���ж���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(CMGRD)
{
	SEL_ANLYSIS_MODE(PROTOCOL_ANALYSIS_MODE_SHORT_MESSAGE,-1);
	GetStrFromTwoKey((char*)Info, ",",",",_gATCmdSetInfo.phoneNum ,len);

	return 0;	
}

// ��������  : ATCMDRECV_FUN(CIPOPEN)
// ��������  : �˿�����״̬
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(CIPOPEN)
{
	return 0;
}

/******************************************************************************/
/*��ʱ������ʼ*/
// ��������  : TIMER_FUN(MainLoop)
// ��������  : ��ѭ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

TIMER_FUN(MainLoop)
{
	if(_gATCmdSetInfo.atCmdRunStatus ==  ATCMD_RUN_STATUS_RUN_FREE)
	{
		RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_LOOP);
	}
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
			   		AtCmdAnalysis_Net_Connect(i);

				}
				else
				{
					_gModuleSetVal[i].tryCount--;	
				}
			}
		}
	
	}
	//һ���ӷ���һ�� 	

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
	AtCmdAnalysis_RunStatus_Init();
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		AtCmd_RECV_WCDMA_Init();
	}
	else
	{
		API_WCDMA_Init();
	}
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
	_gATCmdSetInfo.atCmdRunStatus =  ATCMD_RUN_STATUS_RUN_BUSY;
	if(_gATCmdSetInfo.SendAtCmd == NULL || _gATCmdSetInfo.sendRepeatCount <= 0)
	{
	 	QueuePtr QPtr = NULL;
	 	tagSendBufferQueueItem* pItem = NULL;

		if(CHECK_ERROR())
		{
			DEBUG("Check Error\n");
			while((QPtr = DeQueue(PResFullSendBufferQueuePtr)) != NULL)
			{
				EnQueue(PResFreeSendBufferQueuePtr,QPtr); 	//������е�Cmd	,�����������ִ����
			}			
		}
		else
		{
			 _gModeRunState.AtErrorCount = 0;
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

	 if(_gATCmdSetInfo.SendAtCmd != NULL && _gATCmdSetInfo.sendRepeatCount > 0)
	 {
	 	 if(_gATCmdSetInfo.sendRepeatCount < _gATCmdSetInfo.SendAtCmd->RepeatSendCnt)
		{
			_gModeRunState.AtErrorCount++;
			if(_gModeRunState.AtErrorCount >= MAX_AT_CMD_ERROR_COUNT)
			{
				 TIMER_RUN(StartOk);
			}
			DEBUG("there is least %d count to try Sending Buffer:cmd:%d\n",_gATCmdSetInfo.sendRepeatCount,_gATCmdSetInfo.SendAtCmd->ATCmd);
		}
		_gATCmdSetInfo.cmpSuccess = false;
		API_ResetProtocolBuffer(USART_FUNCTION_WCDMA,RESETRTBUFF_TYPE_TX);
		API_AddProtocolBuffer(USART_FUNCTION_WCDMA,(uchar*)(_gATCmdSetInfo.SendAtCmd->ATCmdSendBuff),ReadStrLen((char*)_gATCmdSetInfo.SendAtCmd->ATCmdSendBuff));
		API_AddProtocolBuffer(USART_FUNCTION_WCDMA,_gATCmdSetInfo.sendBuffer,_gATCmdSetInfo.sendBufferSize);
		API_AddProtocolBuffer(USART_FUNCTION_WCDMA,"\r\n",2);
		API_StartSendBuffer(USART_FUNCTION_WCDMA);
		_gATCmdSetInfo.sendRepeatCount--;

	 }

	 API_SetTimer(_gATCmdSetInfo.SendAtCmd->WaitTimer,1,TIMER_TYPE_WCDMA_AT_SEND,NULL,TIMER_NAME(StartToSendAtCmd));
}
/*��ʱ���������*/
/******************************************************************************/

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(DisConnectNet)
// ��������  : ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_FUN(DisConnectNet)
{
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		char StrTempBuf[100] = {0};
		int deviceIndex = 0;
		char* buf =  _gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_DISCONNECT_NET].funArg;
		if(ReadStrLen(buf) < 1)
			return -1;
		deviceIndex = atoi(buf);
		if(deviceIndex >= MOUNT_DEVICE_COUNT )
			return -1;
			  
		sprintf(StrTempBuf,"%d",deviceIndex);
		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_NETCLOSE,StrTempBuf,strlen(StrTempBuf));
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CIPOPEN,"",0);
		TIMER_RUN(StartToSendAtCmd);

		return 0;

	}

	return -1;

}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(ConnectNet)
// ��������  : ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_FUN(ConnectNet)
{ 
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		char StrTempBuf[100] = {0};
		int deviceIndex = 0;

		if(_gATCmdSetInfo.atCmdGroup < ATCMD_RUN_TYPE_CONNECT_NET || _gATCmdSetInfo.atCmdGroup >= ATCMD_RUN_TYPE_CONNECT_NET + MOUNT_DEVICE_COUNT)
			return -1; 

		deviceIndex = _gATCmdSetInfo.atCmdGroup - ATCMD_RUN_TYPE_CONNECT_NET;

		if(deviceIndex >= MOUNT_DEVICE_COUNT )
			return -1;
	  
		sprintf(StrTempBuf,"%d,\"TCP\",\"%s\",%d",deviceIndex,_gModuleSetVal[deviceIndex].IPAddress,
		                                         _gModuleSetVal[deviceIndex].Port);
		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPOPEN,StrTempBuf,ReadStrLen(StrTempBuf));
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CIPOPEN,"",0);
		TIMER_RUN(StartToSendAtCmd);
		return 0;
	   
	} 
	return -1;
}			 

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(ReadMsg)
// ��������  : ��ȡ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_FUN(ReadMsg)
{
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGRD,"0",1);
		TIMER_RUN(StartToSendAtCmd);

		return 0;
	}

	return -1;
}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(SendMsg)
// ��������  : ���Ͷ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_FUN(SendMsg)
{
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		const int shortMsgMaxSize = 70 * 4;		//����������󳤶���70���ַ�,һ���ַ�ռ��4��HEX 
	   	int size = 0;
		int phoneNumSize = 0;
		int index = 0;
		static char MsgEndChar = 0x1a;
		phoneNumSize = strlen(_gATCmdSetInfo.sendPhoneNum);
		size = ReadStrLen(_gATCmdSetInfo.msgBuffer);

		while(size > 0)
		{
			int shortMsgSize = size > shortMsgMaxSize?shortMsgMaxSize:size;
			
			tagHugeSendBuffer hugeBuffer;
			hugeBuffer.Count = 0;
			hugeBuffer.Buffer[hugeBuffer.Count] = _gATCmdSetInfo.msgBuffer + (index *shortMsgMaxSize) ;
			hugeBuffer.size[hugeBuffer.Count] = shortMsgSize;
			hugeBuffer.Count++;
			hugeBuffer.Buffer[hugeBuffer.Count] = &MsgEndChar;
			hugeBuffer.size[hugeBuffer.Count] = 1;
			hugeBuffer.Count++;
			AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGS,_gATCmdSetInfo.sendPhoneNum,phoneNumSize);
			AtCmdAnalysis_AtCmd_SendHugeBuffer(AT_CMD_SET_CMGSDATA,hugeBuffer);
			size -= shortMsgSize;
			index++;
		}

		TIMER_RUN(StartToSendAtCmd);
		return 0;

	}

	return -1;
}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_CALLBACK_FUN(DeviceSend)
// ��������  : �����豸����Ϣ��Ļص�����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_CALLBACK_FUN(DeviceSend)
{
	int deviceIndex = 0;

	deviceIndex = _gATCmdSetInfo.atCmdGroup - ATCMD_RUN_TYPE_DEVICE_SEND;
	if(errType != SEND_ERROR_TYPE_NONE)
	{
		API_ResetProtocolBuffer((USART_FUNCTION)(deviceIndex + USART_FUNCTION_MOUNT),RESETRTBUFF_TYPE_AND_INT_RX);

		//�����⵽����
	}
	else
	{
		//Delay_MS(1000);
		AtCmdAnalysis_Net_DeviceSend(deviceIndex);
	 	//���û�м�⵽����,������������
	}

	return 0;

}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(DeviceSend)
// ��������  : �����豸��Ϣ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_FUN(DeviceSend)
{
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		char StrTempBuf[100] = {0};
		int deviceIndex = 0;
		uint16_t size = 0;
		uint16_t retSize =0;
		tagHugeSendBuffer hugeBuffer;

		if(_gATCmdSetInfo.atCmdGroup < ATCMD_RUN_TYPE_DEVICE_SEND || _gATCmdSetInfo.atCmdGroup >= ATCMD_RUN_TYPE_DEVICE_SEND + MOUNT_DEVICE_COUNT)
			return -1; 

		deviceIndex = _gATCmdSetInfo.atCmdGroup - ATCMD_RUN_TYPE_DEVICE_SEND;
		if(_gModeRunState.NetIsOpen[deviceIndex] < MAX_NET_IS_OPEN_NUM)
			return -1;
	  	size = Netdata_Protocol_ReadSize(deviceIndex,MAX_SEND_DATA_SIZE_ONCE);
		if(size == 0) {
			API_ResetProtocolBuffer((USART_FUNCTION)(deviceIndex + USART_FUNCTION_MOUNT),RESETRTBUFF_TYPE_AND_INT_RX);
			return -1;
		}
		/*�����ݶ�����*/

		SetATCmdSendCallBack(ATCMDGROUP_CALLBACK_NAME(DeviceSend));	//���÷��ͳɹ���Ļص�����
		hugeBuffer.Count = 0;
		size = 0;
		if(Netdata_Protocol_GetPostion(deviceIndex) == PROTOCOL_READ_POS_START)
		{
			size +=Netdata_Protocol_ReadAddressAndSize(deviceIndex,PROTOCOL_READ_POS_START, &hugeBuffer.Buffer[hugeBuffer.Count],&hugeBuffer.size[hugeBuffer.Count],20);
			hugeBuffer.Count++;
										
		}

		if((retSize = Netdata_Protocol_ReadAddressAndSize(deviceIndex,PROTOCOL_READ_POS_CENTER, &hugeBuffer.Buffer[hugeBuffer.Count],&hugeBuffer.size[hugeBuffer.Count],MAX_SEND_DATA_SIZE_ONCE)) < MAX_SEND_DATA_SIZE_ONCE)
		{
			hugeBuffer.Count++;
			size += Netdata_Protocol_ReadAddressAndSize(deviceIndex,PROTOCOL_READ_POS_END, &hugeBuffer.Buffer[hugeBuffer.Count],&hugeBuffer.size[hugeBuffer.Count],MAX_SEND_DATA_SIZE_ONCE);


		}

		hugeBuffer.Count++;
		size += retSize;
		//���ݶ���
		sprintf(StrTempBuf,"%d,%d",deviceIndex,size);
		AtCmdAnalysis_AtCmd_Send(AT_CMD_TEST_AT,"",0);
 		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPSEND,StrTempBuf,ReadStrLen(StrTempBuf));

		AtCmdAnalysis_AtCmd_SendHugeBuffer(AT_CMD_SET_TCPWDATA,hugeBuffer);

		AtCmdAnalysis_DeviceBeat_ReStart();	//������ͣ
		//		DEBUG("L:%d\n",__LINE__);
		TIMER_RUN(StartToSendAtCmd);

		return 0;

	}
	return -1;
}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(InfoSend)
// ��������  : ����������Ϣ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_FUN(InfoSend)
{
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		char StrTempBuf[100] = {0};
		int deviceIndex = 0;
		uint16_t size = 0;

		if(_gATCmdSetInfo.atCmdGroup < ATCMD_RUN_TYPE_INFO_SEND || _gATCmdSetInfo.atCmdGroup >= ATCMD_RUN_TYPE_INFO_SEND + MOUNT_DEVICE_COUNT)
			return -1; 


		deviceIndex = _gATCmdSetInfo.atCmdGroup - ATCMD_RUN_TYPE_INFO_SEND;

		if(_gModeRunState.NetIsOpen[deviceIndex] < MAX_NET_IS_OPEN_NUM)
			return -1;
		//���ݶ���
		size = _gAtCmdRunCtrlArray[_gATCmdSetInfo.atCmdGroup].funArgSize;// strlen(_gAtCmdRunFunTypeArray[_gATCmdSetInfo.atCmdGroup].funArg);
		

		AtCmdAnalysis_DeviceBeat_ReStart();	//������ͣ
		sprintf(StrTempBuf,"%d,%d",deviceIndex,size);
 		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPSEND,StrTempBuf,strlen(StrTempBuf));

		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_TCPWDATA,
			_gAtCmdRunCtrlArray[_gATCmdSetInfo.atCmdGroup].funArg,
			size);

		//		DEBUG("L:%d\n",__LINE__);
		TIMER_RUN(StartToSendAtCmd);

		return 0;

	}
	return -1;
}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(Restart)
// ��������  : ģ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_FUN(Restart)
{
	_gATCmdSetInfo.atCmdRunStatus =  ATCMD_RUN_STATUS_NOT_RUN;
	_gATCmdSetInfo.sendRepeatCount = 0;

	API_WCDMA_Init();

	return 0;	
}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_CALLBACK_FUN(Init)
// ��������  : ��ʼ���Ļص�����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_CALLBACK_FUN(Init)
{
	if(errType != SEND_ERROR_TYPE_NONE)
	{
		char buf[20] = {0};

		_gATCmdSetInfo.atCmdRunStatus =  ATCMD_RUN_STATUS_NOT_RUN;	//��־��Not Run״̬
		DEBUG("Error detect to Init setting ev,errType:%d!\n",errType);
		sprintf(buf,"errorType:%d,atCmd:%d",errType,atCmd->ATCmd);
		AtCmdAnalysis_ErrorMsg(WCDMA_ERROR_INIT,buf,strlen(buf));
		AtCmd_RECV_WCDMA_Init();
		//�����⵽����
	}
	else
	{

		DEBUG("Success to Init setting ev!\n");

		TIMER_RUN(DeviceBeat);

	 	//���û�м�⵽����
	}
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(Init)
// ��������  : ��ʼ���ĺ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_FUN(Init)
{
	char* netPriBuf = NULL;
	char* apnBuffer = NULL;

	DEBUG("Start to Init setting ev!\n");
	SetATCmdSendCallBack(ATCMDGROUP_CALLBACK_NAME(Init));	//���ó�ʼ���ɹ���Ļص�����

	switch(_gModuleSetVal[0].Cns_CNMP)
	{
		case NETWORK_PRIORIYY_TYPE_1G_2G:	//1,2G����
			 netPriBuf = "13";
			 apnBuffer= "\"3gwap\"";
			 break;
		case NETWORK_PRIORIYY_TYPE_2G_3G:	//2,3G����
			netPriBuf = "14";
			apnBuffer = "\"3gnet\"";
			break;
		case NETWORK_PRIORIYY_TYPE_AUTO:	//�Զ�ѡ��
		default:
			netPriBuf = "2";
			apnBuffer = "\"3gnet\"";
	}

	AtCmdAnalysis_AtCmd_Send(AT_CMD_TEST_AT,"",0);
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CNMP,netPriBuf,ReadStrLen(netPriBuf));
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CNAOP,"",0);  //��������˳��
	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CNSMOD,"",0);  //��������˳��
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CFUN,"",0);  //��������˳��	
	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CSQ,"",0);  //��������˳��
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGF,"1",1);  //���ö��Ÿ�ʽ 

	//֧������		
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CSMP,"25",2);  //����TEXTģʽ����
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CSCS,"\"UCS2\"",6);  //ѡ��TE�ַ���GSM��UCS2��IRA

 	AtCmdAnalysis_AtCmd_Send(AT_CMD_STE_CPMS,"",0);  //��������˳��
 	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CNMI,"",0);  //��������˳��

 	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPSRIP,"",0);  //��������˳��
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CGSOCKCONT,apnBuffer,ReadStrLen(apnBuffer));  //��������˳��
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGD,"",0);
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_NETOPEN,"",0);  //��������˳��
 	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPCCFG,"",0);  //��������˳��	
 //	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_ATE,"",0);  //��������˳��

	//		DEBUG("L:%d\n",__LINE__);
	TIMER_RUN(StartToSendAtCmd);
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(DoLoop)
// ��������  : ��������������в�ѯ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_FUN(DoLoop)
{
	if(_gATCmdSetInfo.atCmdRunStatus ==  ATCMD_RUN_STATUS_RUN_FREE)
	{
		API_SetTimer(600000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);
		//ֻ���ڿ���״̬�²Ż�����

		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CSQ,"",0);
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CIPOPEN,"",0);
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CNSMOD,"",0);
		if(_gATCmdSetInfo.haveShortMsg)
		{
			AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGD,"",0);
			_gATCmdSetInfo.haveShortMsg = false;
		}
		else
		{
			AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGL,"\"ALL\"",5);

		}
		//		DEBUG("L:%d\n",__LINE__);
		TIMER_RUN(StartToSendAtCmd);

		return 0;

	}
	return -1;
	 
}


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

		START_CMD(StartUpdate);
		ADD_ARGV_INT(uf);
		END_CMD();
		return 0;
	}

	return 1;
}

int AtCmdAnalysis_WCDMA_StartWcdmaCtrl(USART_FUNCTION uf)
{
	if(uf < USART_FUNCTION_MAX)
	{

		START_CMD(StartWcdmaCtrl);
		ADD_ARGV_INT(uf);
		END_CMD();
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
		START_CMD(StartSend);
		ADD_ARGV_INT(uf);
		END_CMD();

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

	 if(pItem != NULL)
	 {
	 	if(pItem->fun != NULL)
		{
			if(pItem->fun(Info,len) < 0)
				return -1;	
		}
		return 0;
	 }

	 if(AT_CALL_BACK_MODE())
	 {
	 	int i =  0;
		BOOL ret = false;

	 	for(i = 0; i < MAX_RECV_BUFF_COUNT; i++)
		{	
			if((ret = CK(Info,_gSendAtCmd->ATCmdReceBuff[i])) == TRUE)
				break;
		}

	 	if(ret)
		{
			if(_gSendAtCmd->fun != NULL)
			{
				_gSendAtCmd->fun(Info,len);

			}

  			_gATCmdSetInfo.sendRepeatCount = 0;
			CLEAR_ERROR_TYPE();
			_gATCmdSetInfo.cmpSuccess  = true;
		
			API_SetTimerFast(10L,1,TIMER_TYPE_WCDMA_AT_SEND,NULL,TIMER_NAME(StartToSendAtCmd));
		
			return 0;	 
			//Ӧ����ȷ
		}
		else
		{
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
		return -1;
	 }
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
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		if(deviceID < MOUNT_DEVICE_COUNT )
		{	
		 
			WORD index = ATCMD_RUN_TYPE_INFO_SEND + deviceID;
			if(_gAtCmdRunCtrlArray[index].status == ATCMDGROUP_RUN_STATUS_OFF)
			{
				memset(_gAtCmdRunCtrlArray[index].funArg,NULL,MAX_FUN_ARG_SIZE);
				memcpy(_gAtCmdRunCtrlArray[index].funArg,buf,size);
				_gAtCmdRunCtrlArray[index].funArgSize = size;
				RUN_ATCMD_GROUP((ATCMD_RUN_TYPE)index);
			}		
		}
	}
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

	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{

		if(deviceID < MOUNT_DEVICE_COUNT )
		{
			WORD index = ATCMD_RUN_TYPE_DEVICE_SEND + deviceID;
			if(_gAtCmdRunCtrlArray[index].status == ATCMDGROUP_RUN_STATUS_OFF)
			{
				RUN_ATCMD_GROUP((ATCMD_RUN_TYPE)index);
			}		
		}
	}
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
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		if(deviceID < MOUNT_DEVICE_COUNT )
		{
			if(_gModeRunState.NetIsOpen[deviceID] != MAX_NET_IS_OPEN_NUM)
			{
				return 0;
			}
			else
			{
				if(_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_DISCONNECT_NET].status == ATCMDGROUP_RUN_STATUS_OFF)
				{
					sprintf(_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_DISCONNECT_NET].funArg,"%d",deviceID);
					RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_DISCONNECT_NET);
					return 0;	
				}
			}		
		}
	} 
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
	if(size < MAX_IPADDRESS_SIZE && deviceID < MOUNT_DEVICE_COUNT)
	{
		memset(_gModuleSetVal[0].phonelNum,NULL, MAX_PHONE_SIZE);
		memcpy(_gModuleSetVal[0].phonelNum,phoneNum,size);
		AtCmdAnalysis_EERom_Story();

		return 0;
	}

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
	if(size < MAX_PHONE_SIZE && deviceID < MOUNT_DEVICE_COUNT)
	{
		memset(_gModuleSetVal[deviceID].IPAddress,NULL, MAX_PHONE_SIZE);
		memcpy(_gModuleSetVal[deviceID].IPAddress,ipAddress,size);
		_gModuleSetVal[deviceID].Port = port;
		AtCmdAnalysis_EERom_Story();
		AtCmdAnalysisi_DisConnect(deviceID);
		AtCmdAnalysis_Net_Connect(deviceID);
		return 0;
	}

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
	if(beatTime >= 1 && beatTime <= 10)
	{
		int i = 0;
		for(i = 0; i < MOUNT_DEVICE_COUNT;i++)
		{
			_gModuleSetVal[i].beatTime = beatTime;
		}
		if(store)
		{
			AtCmdAnalysis_EERom_Story();
		}
		AtCmdAnalysis_DeviceBeat_ReStart();

		return 0;
	}
	else
	{
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
	if(deviceID < MOUNT_DEVICE_COUNT )
	{
		_gModuleSetVal[deviceID].deviceFun = deviceFun;
		AtCmdAnalysis_EERom_Story();
		if(deviceFun == 1)
		{
			
			AtCmdAnalysis_Net_Connect(deviceID);	
		}
		else
		{
			AtCmdAnalysisi_DisConnect(deviceID);
		}
		
	}
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
	if(cnmp < NETWORK_PRIORIYY_TYPE_MAX)
	{
		_gModuleSetVal[0].Cns_CNMP = (NETWORK_PRIORIYY_TYPE)cnmp;
		AtCmdAnalysis_EERom_Story();
	   AtCmd_RECV_WCDMA_Init();		//�����豸
	   return 0;
	}

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

	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
 		if(deviceID < MOUNT_DEVICE_COUNT )
		{
 			if(_gModeRunState.NetIsOpen[deviceID] == MAX_NET_IS_OPEN_NUM)
			{
 				return 0;
			}
			if(_gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_CONNECT_NET + deviceID].status == ATCMDGROUP_RUN_STATUS_OFF)
			{
				if(AtCmdAnalysis_BeatString_Build(deviceID) > -1)
				{
					RUN_ATCMD_GROUP((ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_CONNECT_NET + deviceID));	
				}	
			}
		}
	}

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
	if(runState != NULL)
	{
		memcpy(runState,&_gModeRunState,sizeof(tagModeRunStateStruct));
		return 0;
	}
	return -1;
}

//������������������������������������������������������������������������������
// ��������  : AtCmdAnalysis_ModuleSetVal_Get
// ��������  : ĳ���豸�Ĳ������û�ȡ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

int AtCmdAnalysis_ModuleSetVal_Get(WORD deviceID,tagModuleSetValStruct* moduleSetVal)
{
	if(deviceID < MOUNT_DEVICE_COUNT && moduleSetVal != NULL)
	{
		memcpy(moduleSetVal,&_gModuleSetVal[deviceID],sizeof(tagModuleSetValStruct));
		return 0;
	}

	return -1;
}

const tagModuleSetValStruct* AtCmdAnalysis_ModuleSetVal_Read(WORD deviceID)
{
	if(deviceID < MOUNT_DEVICE_COUNT)
	{
		return &_gModuleSetVal[deviceID];
	}

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
	if(phoneNum != NULL && buf != NULL)
	{
		int retSize = 0;
		strcpy(_gATCmdSetInfo.sendPhoneNum,phoneNum);
		retSize = Msg_Buffer_UTF8ToPDU(buf,strlen(buf),_gATCmdSetInfo.msgBuffer,sizeof(_gATCmdSetInfo.msgBuffer)-1);
		if(retSize > 0)
		{
			//_gATCmdSetInfo.msgBuffer[retSize] = 0x1a;
			_gATCmdSetInfo.msgBuffer[retSize] = 0x0;
			RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_SEND_MSG);	
		}
		
		return 0;
	}
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
	if(deviceID < MOUNT_DEVICE_COUNT)
	{
		char phoneNum[90] = {0};
		int retSize = 0;
		phoneNum[0] = '\"';
		retSize = Msg_Buffer_UTF8ToPDU(_gModuleSetVal[deviceID].phonelNum,strlen(_gModuleSetVal[deviceID].phonelNum),&phoneNum[1],80);
		if(retSize > 0)
		{
			phoneNum[retSize+1] = '\"';
			return AtCmdAnalysis_Msg_Send(phoneNum,buf);
		}
	}
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
	if(phoneNum != NULL && buf != NULL)
	{
		int retSize = 0;
		strcpy(_gATCmdSetInfo.sendPhoneNum,phoneNum);
		retSize = strlen(buf);

		if(retSize > 0 && retSize < 600)
		{
			strcpy(_gATCmdSetInfo.msgBuffer,buf);
			_gATCmdSetInfo.msgBuffer[retSize] = 0x1a;
			RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_SEND_MSG);	
		}
		//sprintf(_gATCmdSetInfo.msgBuffer,"%s%c",buf,0x1a);
		
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
	AtCmdAnalysis_EERom_Read();
	AtCmdAnalysis_RunStatus_Init();
	ATCmdRecv_Queue_Init();
	ATCmdRecv_SendBufferQueue_Init();
	Netdata_Init();
	Msg_Init();
	API_SetOutsideAnalysisFun(USART_FUNCTION_WCDMA,AtCmdAnalysis_Protocol_Analysis);
	AtCmdAnalysis_ATCmdRunCtrlArray_Init();
	API_SetTimer(3000,-1,TIMER_TYPE_WCDMA_ATCMD_PROCESS_LOOP,TIMER_NAME(MainLoop),NULL);
	AtCmdAnalysis_BeatTime_Set(_gModuleSetVal[0].beatTime,false);
	API_SetTimer(60000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);

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
	if(size < MAX_IPADDRESS_SIZE && deviceID < MOUNT_DEVICE_COUNT)
	{	
		memset(_gModuleSetVal[deviceID].CID,NULL, MAX_CID_SIZE);
		memcpy(_gModuleSetVal[deviceID].CID,cid,size);
		AtCmdAnalysis_EERom_Story();

		return AtCmdAnalysis_BeatString_Build(deviceID);
	}

	return -1;
}

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
	for(i = 0; i < MOUNT_DEVICE_COUNT; i++)
	{
		memcpy(&_gModuleSetVal[i],&val[i],sizeof(tagModuleSetValStruct));
	}
	AtCmdAnalysis_EERom_Story();
	AtCmd_RECV_WCDMA_Init();		//�����豸
		
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
	return 0;
}
