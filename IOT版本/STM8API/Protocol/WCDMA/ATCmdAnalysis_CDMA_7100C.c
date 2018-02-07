#include "config.h"
#include "ATCmdAnalysis_Env.h"
#include "MsgUtf8String.h"
/******************************************************************************/
#define SYSTEM_MAP_ENABLE		1
static tagATCmdSetInfo* 		_gATCmdSetInfoPtr 	= NULL;
static tagModuleSetValStruct*	_gModuleSetVal 		= NULL;
#if SYSTEM_MAP_ENABLE
static tagSystemSetValStruct* 	_gSystemSetValPtr	= NULL;
#endif
static int 						_gModuleCount 		= 0;
static tagModeRunStateStruct*	_gModeRunStatePtr;
static tagAtCmdRunCtrlItem* 	_gAtCmdRunCtrlArray;
static LinkQueue*				_gLinkQueue;

static tagCnsModSet _gCnsModSet[] =
{
	{0,STRING_NO_SERVICE,"CMNET"},
	{2,"CDMA,","CMNET"},
	{4,"HDR","CMNET"},
	{8,"HYBRID","CMNET"}
};
#define CNSMODSET_SIZE	sizeof(_gCnsModSet)/sizeof(tagCnsModSet)

/******************************************************************************/

#define _gModeRunState		(*_gModeRunStatePtr)
#define _gATCmdSetInfo		(*_gATCmdSetInfoPtr) 
#define _gSystemSetVal		(*_gSystemSetValPtr) 
#define PResFreeQueue		_gLinkQueue[0]			//���еĶ���
#define	PResFullQueue		_gLinkQueue[1]	   		//��æ�Ķ���
#define	PResFreeQueuePtr	&PResFreeQueue
#define	PResFullQueuePtr	&PResFullQueue

/******************************************************************************/

ATCMDRECV_FUN(CMGL);
ATCMDRECV_FUN(CSQ);
ATCMDRECV_FUN(CNSMOD);
ATCMDRECV_FUN(CGMR);
ATCMDRECV_FUN(COPS);
//ATCMDRECV_FUN(CPMS);
//ATCMDRECV_FUN(CGSOCKCONT);
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
ATCMDRECV_FUN(CCLK);

/******************************************************************************/

ATCMDGROUP_CALLBACK_FUN(ConnectNet);
ATCMDGROUP_CALLBACK_FUN(DeviceSend);
ATCMDGROUP_CALLBACK_FUN(Init);

/******************************************************************************/

/*��ѯ��*/
ATCMDGROUP_FUN(DoLoop);
/*����ģ��*/
ATCMDGROUP_FUN(Restart);
/*��ȡģ�����Ϣ*/
ATCMDGROUP_FUN(GetModuleInfo);
/*ģ���ʼ��*/
ATCMDGROUP_FUN(Init);
/*��ȡ����*/
ATCMDGROUP_FUN(ReadMsg);
/*���Ͷ���*/
ATCMDGROUP_FUN(SendMsg);
/*����*/
ATCMDGROUP_FUN(ConnectNet);
/*�Ͽ�����*/
ATCMDGROUP_FUN(DisConnectNet);
/*�Ӳɼ��ڻ����Ϣ�������˷�������*/
ATCMDGROUP_FUN(DeviceSend);
/*���ͼ�̵���Ϣ��һ��������*/
ATCMDGROUP_FUN(InfoSend);
/*����3g�汾��*/
ATCMDGROUP_FUN(Set3GNetVersion);

/******************************************************************************/

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
//	AT_CMD_SET_CNAOP 		=	 29,  //��������˳��
	AT_CMD_GET_CNSMOD 		=	 30,  //��ѯ����ϵͳģʽ
	AT_CMD_REV_MSMNOTE		=	 31,  //�յ���������
	AT_CMD_SET_ATE 			=	 32,  //�����رջ���
	AT_CMD_SET_CIPCCFG		=	 33,  //���ò������׽���
	AT_CMD_SET_CMGS			=	 34,  //���Ͷ���+�绰����
	AT_CMD_SET_CMGSDATA		=	 35,  //���Ͷ���������
	AT_CMD_GET_CMGRO		=	 36,  //��ѯ����
	AT_CMD_GET_CCLK			=	 37,  //��ѯʱ��
	AT_CMD_GET_CGMR			=	 38,  //��ѯ�汾��
	AT_CMD_SET_CSOCKSETPN	=	 39,  //�ı�PDPģʽ	
	AT_CMD_GET_COPS			=		40,	//��ѯע������״̬
	AT_CMD_SET_CNVW			=	41,	//����
	AT_CMD_GET_CGREG		=42,		// GPRS״̬�ϱ�
	AT_CMD_GET_IPADDR		=43,		//��ѯIP��ַ
//	AT_CMD_GET_CGATT		=44,		//GPRS״̬��ѯ
	AT_CMD_SET_FSCD			=	45,	//���õ�ǰĿ¼
	AT_CMD_SET_FSMKDIR		=	46,	//����Ŀ¼
	AT_CMD_SET_FSRMDIR		=	47,	//ɾ��Ŀ¼
	AT_CMD_SET_FSLS			=	48,	//�ļ��б�
	AT_CMD_SET_CSOCKAUTH		=	49,	//����APN
	AT_CMD_SET_CIPMODE		=	50,	//����͸��ģʽ
	AT_CMD_SET_CIPSENDMODE	=	51,	//���÷���ģʽ
	AT_CMD_SET_CATR			=	52,	//����ͨѶ��ʽ
	AT_CMD_GET_CSUB			= 	53,	//
	AT_CMD_SET_CGREG			=	54,	//����GPRS�ϱ�״̬
	AT_CMD_SET_IPREX			=	55,	//�趨������
	AT_CMD_MAX
}AT_CMD;

typedef enum {
	WIRELESS_ERROR_SIM,		//SIM��������
	WIRELESS_ERROR_NET 		//���������
}WIRELESS_ERROR;	//����ģ�����

/******************************************************************************/
static const tagATCmd	 _gATCmd[]={
//���						����ͷ����						Ӧ��ͷ����		ʧ��Ӧ��ͷ		�ȴ�Ӧ��ʱ��	�ط�����	��Ӧ����
{AT_CMD_TEST_AT,         {"AT"},	                        {"OK",""},		{""},		    1000,	       	5,			NULL	,true},	//����CDMA�Ƿ��з���
{AT_CMD_SET_CFUN,        {"AT+CFUN?"},	                 	{"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//��������ģʽ
{AT_CMD_GET_CSQ,         {"AT+CSQ"},	                    {"+CSQ:",""},	{""},	1000,	       	5,			ATCMDRECV_NAME(CSQ)   ,true },	//��ѯ�����ź�
{AT_CMD_GET_CREG,        {"AT+CREG?"},	                 	{"+CREG: ",""}, {""},  1000,	       	5,			NULL    		,true},	//����ע����Ϣ
{AT_CMD_GET_CICCID,      {"AT+CICCID"},	                 	{"+ICCID: ",""},{""},	1000,	       	5,			NULL    		,true},	//��ѯUIM���к�
{AT_CMD_GET_IPR,         {"AT+IPR?"},	                 	{"+IPR: ",""},	{""},    1000,	       	5,			NULL    		,true},	//��ѯ������
{AT_CMD_SET_IPR,         {"AT+IPR="},	                 	{"OK",""},	    {""},    1000,	       	5,			NULL    		,true},	//���ò�����
{AT_CMD_GET_CCLK,		 {"AT+CCLK?"},						{"+CCLK:","OK"},{""},		1000,		1,			ATCMDRECV_NAME(CCLK)			,true},	//��ȡϵͳʱ��
//******************************************����*******************************************************************
{AT_CMD_SET_CMGF,        {"AT+CMGF="},	                 	{"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//���ö��Ÿ�ʽ 0,PDUģʽ 1,textģʽ
{AT_CMD_SET_CSCS,        {"AT+CSCS="},			            {"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//ѡ��TE�ַ���GSM��UCS2��IRA
{AT_CMD_STE_CPMS,       {"AT+CPMS=\"ME\",\"ME\",\"ME\""}, 	{"+CPMS",""},	{""},	1000,	       	5,			/*ATCMDRECV_NAME(CPMS)*/NULL  ,true},	//���Ŵ洢λ��ѡ������
{AT_CMD_SET_CNMI,       {"AT+CNMI=2,1"},	  	            {"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//�����¶���Ϣ��ʾ���� 2,1:�洢���� 1,2:���洢���ţ��¶���ֱ�Ӵ�ӡ������
{AT_CMD_SET_CSMP,       {"AT+CSMP=17,167,0,8"},         	{"OK",""},		{""},    1000,	       	5,			NULL    		,true},	//����TEXTģʽ���������һ��Ϊ���ݱ������ͣ�0��ʾĬ���ַ���(GSM) 241:�ַ� 25:����
{AT_CMD_SET_CMGSO,       {"AT+CMGSO="},	                 	{"OK",""},		{"ERROR"},    10000,	       	2,			NULL    		,true},	//���ٷ���Ϣ
{AT_CMD_SET_CMGS,		{"AT+CMGS="},						{">",""},		{""},	500,			1,			NULL			,true},
{AT_CMD_SET_CMGSDATA,	{""},								{"+CMGS","OK"},	{""},	30000,			1,			NULL			,true},
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
{AT_CMD_SET_CGSOCKCONT, {"AT+CGSOCKCONT"},      				{"OK","+CGSOCKCONT:"},		{"ERROR"},   1000,	       	1,			NULL    		,true},	//����APN 
//{AT_CMD_SET_NETOPEN,    {"AT+NETOPEN=,,1"},               	{"+NETOPEN: 0","Network opened"},{"ERROR"},		    30000,	       	2,			NULL    ,true},	//�����ӷ�ʽ
{AT_CMD_SET_NETOPEN,    {"AT+NETOPEN"},               	{"+NETOPEN: 0",""},{"ERROR"},		    20000,	       	2,			NULL    ,true},	//�����ӷ�ʽ
{AT_CMD_SET_CIPOPEN,    {"AT+CIPOPEN="},	                {"+CIPOPEN:","+IP ERROR: Connection is already created"},{"ERROR"},	5000,	       	1,			ATCMDRECV_NAME(CIPOPEN)    ,true},	//����һ·TCP/IP ���� 
{AT_CMD_SET_CIPSEND,    {"AT+CIPSEND="},	                {">",""},		  {""},  2500,	       	1,			NULL    		,true},	//��һ·TCP/IP �·�������(����) 
{AT_CMD_SET_TCPWDATA,   {""},	                         	{"+CIPSEND:",""},	{"ERROR"},	30000,	       	1,			NULL    	,true},	//��һ·TCP/IP �·�������(������) 
{AT_CMD_SET_NETCLOSE,  {"AT+CIPCLOSE="},	             	{"OK",""},		{""},    1000,	       	1,			NULL    		,true},	//�ر�һ·TCP/IP����
{AT_CMD_GET_CIPOPEN,    {"AT+CIPOPEN?"},	                {"OK",""},      {""},   500,	       	1,			NULL    		,false},	//��ѯ��������״̬ 
{AT_CMD_REV_NETDATA,    {""},	                         	{"+IPD",""},	{""},	1000,	       	0,			NULL    		,true},	//�յ���������
//******************************************************************************************************************
{AT_CMD_GET_CNUM,       {"AT+CNUM"},	                    {"+CNUM:",""},	{""},	1000,	       	5,			NULL    		,true},	//��ѯ��������	 
//{AT_CMD_SET_CNAOP,      {"AT+CNAOP=0"},	                 	{"OK",""},		{""},   1000,	       	5,			NULL    		,true},	//��������˳��0��Automatic	1��GSM,WCDMA  2��WCDMA,GSM  
{AT_CMD_GET_CNSMOD,     {"AT+CNSMOD?"},	                 	{"+CNSMOD:",""},{""},   1000,	       	5,			ATCMDRECV_NAME(CNSMOD),true},	//��ѯ����ϵͳģʽ
{AT_CMD_SET_ATE,        {"ATE0"},	                     	{"OK",""},      {""},   1000,	       	5,			NULL    		,true},	//�����رջ�������
{AT_CMD_SET_CIPCCFG,    {"AT+CIPCCFG=10,0,0,1,1,0,500"},	     	{"OK",""},      {""},   1000,	       	5,			NULL    		,true},  //���ò������׽���
{AT_CMD_GET_CGMR,    	{"AT+CGMR"},	     				{"+CGMR:","OK"},      {""},   1000,	       	2,			ATCMDRECV_NAME(CGMR)   		,true},  //��ȡ�汾��
{AT_CMD_SET_CSOCKSETPN,	{"AT+CSOCKSETPN="},					{"OK",""},		{""},	1000,			2,			NULL			,true},
{AT_CMD_GET_COPS,				{"AT+COPS?"},								{"+COPS:",""},{""},	1000,			2,			ATCMDRECV_NAME(COPS),			true},
{AT_CMD_SET_CNVW,		{"AT+CNVW=1030,0,"},				{"+CNVW:1",""},	{"+CNVW:0"},	1000,	1,			NULL			,true},	//����3G�汾
{AT_CMD_GET_CGREG,		{"AT+CGREG"},				{"+CGREG: 1,1",""},{""},	10000,	4,		NULL,			true},	//��ȡGPRS״̬
{AT_CMD_GET_IPADDR,		{"AT+IPADDR"},				{"+IPADDR:",""},{"+IP ERROR:"},	1000,	1,		NULL,			true},	//��ȡIP��ַ
//{AT_CMD_GET_CGATT,		{"AT+CGATT=1"},			{"OK",""},	{"ERROR"},			10000,		3,	NULL,		true}
{AT_CMD_SET_CSOCKAUTH,	{"AT+CSOCKAUTH"},			{"OK",""},	{"ERROR"},		1000,		1,			NULL,			true},//����APN
{AT_CMD_SET_CIPMODE,		{"AT+CIPMODE"},				{"OK",""},	{"ERROR"},		1000,		1,	NULL,	true},		//����͸��ģʽ 
{AT_CMD_SET_CIPSENDMODE,	{"AT+CIPSENDMODE"},			{"OK",""},	{"ERROR"},		1000,		1,	NULL,	true},
{AT_CMD_SET_CATR,			{"AT+CATR"},				{"OK",""},	{"ERROR"},		1000,		1,	NULL,	true},
{AT_CMD_GET_CSUB,			{"AT+CSUB"},				{"OK",""},	{"ERROR"},		1000,		1,	NULL,	true},
{AT_CMD_SET_CGREG,		{"AT+CGREG=1"},				{"OK",""},	{"ERROR"},	1000,	2,	NULL,	true},
{AT_CMD_SET_IPREX,		{"AT+IPREX="},				{"OK",""},	{"ERROR"},	1000,	2,	NULL,	true}
//******************************************************************************************************************* 
};

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

//��ʼ�����ñ�
const static  tagAtCmdRunCtrlItem _gAtCmdRunFunTypeArray[ATCMD_RUN_TYPE_MAX] =
{
	/*��������										������ִ�к���						���������	��������	������ִ��״̬				״̬��*/
	{ATCMD_RUN_TYPE_RESTART,							ATCMDGROUP_NAME(Restart),		"",			0,			ATCMDGROUP_RUN_STATUS_OFF,  false},
	{ATCMD_RUN_TYPE_GET_MODULE_INFO,			ATCMDGROUP_NAME(GetModuleInfo),"",	0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{ATCMD_RUN_TYPE_INIT, 								ATCMDGROUP_NAME(Init),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{ATCMD_RUN_TYPE_SET3GNETVERSION,					ATCMDGROUP_NAME(Set3GNetVersion),	"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{ATCMD_RUN_TYPE_READ_MSG,							ATCMDGROUP_NAME(ReadMsg),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{ATCMD_RUN_TYPE_SEND_MSG,							ATCMDGROUP_NAME(SendMsg),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,  false},
	{ATCMD_RUN_TYPE_DISCONNECT_NET,						ATCMDGROUP_NAME(DisConnectNet),		"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_CONNECT_NET + 0),	ATCMDGROUP_NAME(ConnectNet),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_CONNECT_NET + 1),	ATCMDGROUP_NAME(ConnectNet),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_DEVICE_SEND +0), 	ATCMDGROUP_NAME(DeviceSend),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_DEVICE_SEND +1),	ATCMDGROUP_NAME(DeviceSend),			"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_INFO_SEND +0),		ATCMDGROUP_NAME(InfoSend),				"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
	{(ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_INFO_SEND +1),		ATCMDGROUP_NAME(InfoSend),				"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false},
 	{ATCMD_RUN_TYPE_LOOP,								ATCMDGROUP_NAME(DoLoop),				"",			0,			ATCMDGROUP_RUN_STATUS_OFF,	false}
};

/******************************************************************************/

#define AT_CMD_COUNT	sizeof(_gATCmd)/sizeof(tagATCmd)	
#define RECV_AT_CMD_SIZE	(sizeof(_gtagATCmdRecvProcessItemArray)/sizeof(tagATCmdRecvProcessItem))

/******************************************************************************/

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
	LINE_LOG();
	if(argc < 2)
		return -1;
	enable = (BOOL)atoi(argv[0]);
	port =  atoi(argv[1]);
	LINE_LOG();

	if(port < _gModuleCount)
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
					//AtCmdAnalysisi_DisConnect(port);
				}
			}
		}
	}
	
	if(_gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_ON && port == _gSystemSetVal.sysSetValCtrl.controlPort)
	{
		if(enable)
		{
			API_Led_Set(LED_TYPE_2, LED_STATE_ON);
	  		TIMER_RUN(DeviceBeat);
		}
		else
		{
			
		}
	}
	
	LINE_LOG();	
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
	LINE_LOG();
	if(argc < 1)
		return -1;
	for(index = 0; index < _gModuleCount; index++)
	{
		if(_gModeRunState.NetIsOpen[index] > 0)
		{
			_gModeRunState.NetIsOpen[index]--;
		}
	}
	LINE_LOG();

	API_SetTimer(20L,1,TIMER_TYPE_WCDMA_AT_SEND,NULL,TIMER_NAME(StartToSendAtCmd));
	LINE_LOG();
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
	LINE_LOG();
	_gModeRunState.SimCard = SIM_CARD_STATUS_ERROR;	
	API_SetTimer(60000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);
	AtCmdAnalysis_ErrorMsg(WCDMA_ERROR_NO_SIM_CARD,"",0);
	LINE_LOG();
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
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus != ATCMD_RUN_STATUS_NOT_RUN)
	{
		char* str = "Unkown";
		AtCmdAnalysis_ErrorMsg(WCDMA_ERROR_MODULE_RESTART,str,strlen(str));
	}
	_gATCmdSetInfo.atCmdRunStatus = ATCMD_RUN_STATUS_NOT_RUN;
	API_SetTimerFast(60000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);
	LINE_LOG();
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
	DEBUG("PbDone ==> Ok\n");
	LINE_LOG();
	if(_gModeRunState.SimCard == SIM_CARD_STATUS_OK
		&&_gATCmdSetInfo.atCmdRunStatus == ATCMD_RUN_STATUS_NOT_RUN)
	{
		RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_GET_MODULE_INFO);
	}
	LINE_LOG();
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
	LINE_LOG();
	_gModeRunState.SimCard = SIM_CARD_STATUS_OK;
	LINE_LOG();	
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
	LINE_LOG();
	p = GetStrFromTwoKey((char*)Info, ",",",",buf ,sizeof(buf));
	LINE_LOG();
	if(p != NULL)
	{
		port = atoi(buf);	//���յĶ˿ں�
		p = InStr(p,",",32);
		p++;
		//p = GetStrFromTwoKey(p, ",","\0",buf ,sizeof(buf));
		if(p != NULL)
		{
			length= atoi(p);	//���ݳ���
			#if SYSTEM_MAP_ENABLE
			if(_gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_ON)
			{
				if(_gSystemSetVal.sysSetValCtrl.controlPort == port)
				{
					//���ܱ�����
					if(_gSystemSetVal.sysSetValCtrl.mapDeviceID < MOUNT_DEVICE_COUNT)
					{
						port = _gSystemSetVal.sysSetValCtrl.mapDeviceID;
						//_gSystemSetVal.sysMountMapStatus = SYSTEM_MOUNT_MAP_STATUS_ON;	//����ӳ��ģʽ
						API_SetTimerFast(_gSystemSetVal.sysSetValCtrl.timeOut*1000,1,TIMER_TYPE_WCDMA_SYSTEM_MAP,TIMER_NAME(SystemMap),NULL);
					}
					else
					{
						return -1;
					}

				}
				else if(_gSystemSetVal.sysSetValCtrl.mapDeviceID == port)
				{
					return -1;
				}
			}
			#endif
			_gATCmdSetInfo.RecvFunction = 	(USART_FUNCTION)(port + USART_FUNCTION_MOUNT);
			SEL_ANLYSIS_MODE(PROTOCOL_ANALYSIS_MODE_RECEIVE,length);
			return 0;
		}
	}
	LINE_LOG();
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
	LINE_LOG();
	#if SYSTEM_MAP_ENABLE
	if(_gSystemSetVal.sysSetValCtrl.controlPort== portInt)
	{
		//if(_gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_ON)
		//��⵽ϵͳ���ӿ���
		l = ReadStrLen((char*)(Info+10));
		if(l > 10)
		{
			if(_gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_ON
				&& _gSystemSetVal.NetIsOpen != MAX_NET_IS_OPEN_NUM )
			{
				_gSystemSetVal.NetIsOpen = MAX_NET_IS_OPEN_NUM;
				START_CMD(EnableHeartBeat);
				ADD_ARGV_BOOL(true);
				ADD_ARGV_INT(portInt);
				END_CMD();
			}
		}
		else if(l > 3 && l<= 10)
		{
			return 1;
		}
		else
		{
			if(_gSystemSetVal.NetIsOpen > 0)
			{
				_gSystemSetVal.NetIsOpen--;
			}
		}

	}
	else
	{
		if(portInt >= _gModuleCount || portInt < 0) {
			return -1;
		}
		
		LINE_LOG();
		 
		if(_gModuleSetVal[portInt].deviceFun != 1)
		{
			LINE_LOG();
			return -1;
		}
	}
	#else
	if(portInt >= _gModuleCount || portInt < 0)
		return -1;
	LINE_LOG();
	 
	if(_gModuleSetVal[portInt].deviceFun != 1)
	{
		return -1;
	}
	#endif
	LINE_LOG();
	/*��֡β�س����������ַ���������*/
	/*�ж��ַ�����,����ַ����ȴ���2,˵���˶Ͽ��Ѿ���������*/
	l = ReadStrLen((char*)(Info+10));
	LINE_LOG();
	if(l > 10)
	{ 
		ret = true;
	}
	else if(l > 3 && l<= 10)
	{
		return 1;
	}
	else
	{	
		ret = false;
	}
	LINE_LOG();
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
				//RUN_ATCMD_GROUP((ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_DISCONNECT_NET + portInt));	
				AtCmdAnalysisi_DisConnect(portInt);
				LINE_LOG();
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
			LINE_LOG();
			for(index = 0; index < _gModuleCount; index++)
			{
				if(_gModeRunState.NetIsOpen[index] > 0 && _gModuleSetVal[index].deviceFun == 1)
					break;
			}
			//���
			if(index >= _gModuleCount)
			{
				char buf[30] = {0};
				_gModeRunState.NetIsOpen[portInt] = MAX_NET_IS_OPEN_NUM-1;
				sprintf(buf,"%d",portInt);
				AtCmdAnalysis_ErrorMsg(WCDMA_ERROR_NET_DISCONNECT,buf,strlen(buf));
				AtCmd_RECV_WCDMA_Init();
			}
			//�����豸			
		}
	}
	LINE_LOG();
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
	LINE_LOG();
	port=InStr((char*)Info, ",",len);
	if(port == NULL)
		return -1;
	port++;
	DEBUG("IpClose ==> Ok\n");
	START_CMD(EnableHeartBeat);
	ADD_ARGV_BOOL(false);
	ADD_ARGV(port);
	END_CMD();
	LINE_LOG();
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
	LINE_LOG();
	DEBUG("SIM Check ==>Failed\n");
	START_CMD(SIMFailed);
	ADD_ARGV_INT(WIRELESS_ERROR_SIM);
	END_CMD();
	LINE_LOG();
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
	LINE_LOG();
	if(InStr((char*)Info,"Connection is already created",len) == NULL)
	{	
	 	START_CMD(NoticeError);
		ADD_ARGV_INT(WIRELESS_ERROR_NET);
		END_CMD();
	}
	else
	{
		  
	}
	LINE_LOG();
	return 0;//API_Wireless_NoticeError(WIRELESS_ERROR_NET);
}

// ��������  : ATCMDRECV_FUN(CPMS)
// ��������  : ���ղ�ѯ���ж�������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

//ATCMDRECV_FUN(CPMS)
//{
//	char* p = NULL;
//	int count;
//	LINE_LOG();
//	p = (char*)(Info+7);
//	count = String_To_Int(&p , ",");
//	/*�ж���Ϣ*/
//	if(count>0 && _gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
//	{
//		RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_READ_MSG);		
//	}
//	LINE_LOG();
//	return 0;
//}

//// ��������  : ATCMDRECV_FUN(CGSOCKCONT)
//// ��������  : APN����
//// �������  : None
//// �������  : None
//// ���ز���  : None
////������������������������������������������������������������������������������

//ATCMDRECV_FUN(CGSOCKCONT)
//{
//	char* p = NULL;
//	LINE_LOG();
//	p = (char*)(Info+7);

//	if((StrNcmp((uchar*)Info,(uchar*)"+CGSOCKCONT:") == 0))
//	{
//		char apn[23];
//		p = InStr((char*)Info,",",30);
//		if(p != NULL)
//		{
//			p++;
//			p = GetStrFromTwoKey(p,",\"","\",",apn,20);
//			if(p != NULL)
//			{
//				if(_gModeRunState.CnsMod <  CNSMODSET_SIZE
//					&& _gModeRunState.CnsMod >= 0)
//				{
//					if(!StrNcmp((uchar*)apn,(uchar*)_gCnsModSet[_gModeRunState.CnsMod].apn) == 0)
//					{
//						//��������
//						_gATCmdSetInfo.apnStatus = -1;
//						
//					}
//				}
//				//MAX_CARRIEROPERATOR_NAME_SIZE
//			}
//		}
//		
//	}
//	LINE_LOG();
//	return 0;
//}


// ��������  : ATCMDRECV_FUN(COPS)
// ��������  : ��ѯ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
ATCMDRECV_FUN(COPS)
{
	char* p = NULL;

	LINE_LOG();

	p = GetStrFromTwoKey((char*)Info,"\"","\"",_gATCmdSetInfo.carrierOperator,MAX_CARRIEROPERATOR_NAME_SIZE);
	if(p != NULL)
	{
		//MAX_CARRIEROPERATOR_NAME_SIZE
	}	
	LINE_LOG();

	return 0;
}
// ��������  : ATCMDRECV_FUN(CGMR)
// ��������  : ��ѯ�̼��汾��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(CGMR)
{
	if(!(StrNcmp((uchar*)Info,(uchar*)"OK") == 0))
	{
		int Start = 30;
		while(*Info != ':' && Start > 0)
		{
			Start--;
			Info++;
		}
		//�ҵ����;
		if(*Info != ':')
			return -1;
		Info ++;
		Start = 0;
		while(Info[Start] != '\0' && Start < 49)
		{
			_gATCmdSetInfo.wirelssModuleVision[Start] =  Info[Start];
			Start++;
		}
		_gATCmdSetInfo.wirelssModuleVision[Start] ='\0';
		return 0;
	}
	return -1;
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
	LINE_LOG();
	p=InStr((char*)Info,",",len);
	if(p!=NULL)
	{
		p++;
		_gModeRunState.CnsMod  = String_To_Int(&p,"\0");
	}
	LINE_LOG();
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
	LINE_LOG();
	if(GetStrFromTwoKey((char*)Info, ":",",",buf ,len) != NULL)
	{
		csq = atoi(buf);
		csq = csq > 98?0:csq;
		csq = csq < 0?0:csq;
		_gModeRunState.CSQ =  csq;
	}
	LINE_LOG();
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
	LINE_LOG();
	if(!(StrNcmp((uchar*)Info,(uchar*)"OK") == 0))
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
			SEL_ANLYSIS_MODE(PROTOCOL_ANALYSIS_MODE_SHORT_MESSAGE,-1);
			memcpy(_gATCmdSetInfo.phoneNum,Info,Start);
		}
		_gATCmdSetInfo.haveShortMsg = true;
	}
	LINE_LOG();
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
	LINE_LOG();
	SEL_ANLYSIS_MODE(PROTOCOL_ANALYSIS_MODE_SHORT_MESSAGE,-1);
	GetStrFromTwoKey((char*)Info, ",",",",_gATCmdSetInfo.phoneNum ,len);
	LINE_LOG();
	return 0;	
}
// ��������  : ATCMDRECV_FUN(CCLK)
// ��������  : �˿�����״̬
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDRECV_FUN(CCLK)
{
	LINE_LOG();
	GetStrFromTwoKey((char*)Info, "\"","\"",_gATCmdSetInfo.timeStr ,30);
	LINE_LOG();
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
	LINE_LOG();
	return 0;
}

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
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		char StrTempBuf[100] = {0};
		int deviceIndex = 0;
		char* buf =  _gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_DISCONNECT_NET].funArg;

		if(ReadStrLen(buf) < 1)
			return -1;
		deviceIndex = atoi(buf);
		if(deviceIndex >= _gModuleCount )
			return -1;
		LINE_LOG();	  
		sprintf(StrTempBuf,"%d",deviceIndex);

#if SYSTEM_MAP_ENABLE
		if(_gSystemSetVal.sysSetValCtrl.mapDeviceID == deviceIndex
			&& _gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_OFF
			&& _gSystemSetVal.NetIsOpen  == MAX_NET_IS_OPEN_NUM
		)
		{
			//���ϵͳ����Ҳ�Ͽ��ˣ���ôͬʱ����ϵͳ����
			sprintf(StrTempBuf,"%d",_gSystemSetVal.sysSetValCtrl.controlPort);
			AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_NETCLOSE,StrTempBuf,ReadStrLen(StrTempBuf));


			//������Ŀ�������ͬʱ����
		}
		else
		{
			//ֻ����ԭ�ȵ��豸
			if(_gModeRunState.NetIsOpen[deviceIndex] < MAX_NET_IS_OPEN_NUM)
			{
				AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_NETCLOSE,StrTempBuf,ReadStrLen(StrTempBuf));
			}
		}
#else
		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_NETCLOSE,StrTempBuf,ReadStrLen(StrTempBuf));
#endif		
		
		//AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_NETCLOSE,StrTempBuf,strlen(StrTempBuf));
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CIPOPEN,"",0);
		TIMER_RUN(StartToSendAtCmd);
		return 0;

	}
	LINE_LOG();
	return -1;

}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_CALLBACK_FUN(ConnectNet)
// ��������  : ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_CALLBACK_FUN(ConnectNet)
{
	int deviceIndex = 0;
	LINE_LOG();
	deviceIndex = _gATCmdSetInfo.atCmdGroup - ATCMD_RUN_TYPE_CONNECT_NET;
	if(errType != SEND_ERROR_TYPE_NONE)
	{
		//�����⵽����
	}
	else
	{

	 	//���û�м�⵽����,������������
	}
	if(deviceIndex < _gModuleCount && deviceIndex >= 0)
	{
		_gModuleSetVal[deviceIndex].tryCount = 2;
	//	DEBUG("set :%d count:%d\n",deviceIndex,_gModuleSetVal[deviceIndex].tryCount );
	}

	LINE_LOG();

	return 0;
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
	LINE_LOG(); 
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		char StrTempBuf[100] = {0};
		int deviceIndex = 0;

		if(_gATCmdSetInfo.atCmdGroup < ATCMD_RUN_TYPE_CONNECT_NET || _gATCmdSetInfo.atCmdGroup >= ATCMD_RUN_TYPE_CONNECT_NET + _gModuleCount)
			return -1; 
		deviceIndex = _gATCmdSetInfo.atCmdGroup - ATCMD_RUN_TYPE_CONNECT_NET;

		if(deviceIndex >= _gModuleCount )
			return -1;
	  	AtCmdAnalysis_SetATCmdSendCallBack(ATCMDGROUP_CALLBACK_NAME(ConnectNet));	//���÷��ͳɹ���Ļص�����

		
#if SYSTEM_MAP_ENABLE
		if(_gSystemSetVal.sysSetValCtrl.mapDeviceID == deviceIndex
			&& _gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_ON)
		{
							//��������				

			if(_gSystemSetVal.NetIsOpen < MAX_NET_IS_OPEN_NUM)
			{
				//���ϵͳ����Ҳ�Ͽ��ˣ���ôͬʱ����ϵͳ����
				sprintf(StrTempBuf,"%d,\"TCP\",\"%s\",%d",_gSystemSetVal.sysSetValCtrl.controlPort,_gSystemSetVal.sysSetValCtrl.IPAddress,
		                                         _gSystemSetVal.sysSetValCtrl.Port);
				AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPOPEN,StrTempBuf,ReadStrLen(StrTempBuf));
			}
			//���ӳ��˿ں���Ҫ���ӵĶ˿�һ��
			//������ԭ�ȵ��豸
			if(_gModeRunState.NetIsOpen[deviceIndex] < MAX_NET_IS_OPEN_NUM
					&& _gModuleSetVal[deviceIndex].deviceFun == 1)
			{
				sprintf(StrTempBuf,"%d,\"TCP\",\"%s\",%d",deviceIndex,_gModuleSetVal[deviceIndex].IPAddress,
		                                         _gModuleSetVal[deviceIndex].Port);
					//���ԭ�ȵĶ˿ڻ�û�������ϣ�������ԭ�ȵĶ˿�
				AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPOPEN,StrTempBuf,ReadStrLen(StrTempBuf));
			}


			//������Ŀ�������ͬʱ����
		}
		else
		{
			sprintf(StrTempBuf,"%d,\"TCP\",\"%s\",%d",deviceIndex,_gModuleSetVal[deviceIndex].IPAddress,
		                                         _gModuleSetVal[deviceIndex].Port);
			//ֻ����ԭ�ȵ��豸
			if(_gModeRunState.NetIsOpen[deviceIndex] < MAX_NET_IS_OPEN_NUM)
			{
				AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPOPEN,StrTempBuf,ReadStrLen(StrTempBuf));
			}
		}
#else
		sprintf(StrTempBuf,"%d,\"TCP\",\"%s\",%d",deviceIndex,_gModuleSetVal[deviceIndex].IPAddress,
		                                         _gModuleSetVal[deviceIndex].Port);
		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPOPEN,StrTempBuf,ReadStrLen(StrTempBuf));
#endif
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CIPOPEN,"",0);
		TIMER_RUN(StartToSendAtCmd);
		LINE_LOG();
		return 0;
	}
	LINE_LOG(); 
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
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGRD,"0",1);
		TIMER_RUN(StartToSendAtCmd);
		LINE_LOG();
		return 0;
	}
	LINE_LOG();
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
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		const int shortMsgMaxSize = 70 * 4;		//����������󳤶���70���ַ�,һ���ַ�ռ��4��HEX 
	   	int size = 0;
		int phoneNumSize = 0;
		int index = 0;
		static char MsgEndChar = 0x1a;
		phoneNumSize = strlen(_gATCmdSetInfo.sendPhoneNum);
		size = ReadStrLen(_gATCmdSetInfo.msgBuffer);
		LINE_LOG();
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
			Msg_SendStatus_Notice(1);
		}
		LINE_LOG();
		TIMER_RUN(StartToSendAtCmd);
		return 0;
	}
	LINE_LOG();
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
	LINE_LOG();
	deviceIndex = _gATCmdSetInfo.atCmdGroup - ATCMD_RUN_TYPE_DEVICE_SEND;
	if(errType != SEND_ERROR_TYPE_NONE)
	{
		API_ResetProtocolBuffer((USART_FUNCTION)(deviceIndex + USART_FUNCTION_MOUNT),RESETRTBUFF_TYPE_AND_INT_RX);
		//�����⵽����
	}
	else
	{
		LINE_LOG();
		//�ó�������ִ�з�������ֱ�����
		if(_gATCmdSetInfo.atCmdGroup > 0)
		{
			_gATCmdSetInfo.atCmdGroup--;
		}
		else
		{
			_gATCmdSetInfo.atCmdGroup = (ATCMD_RUN_TYPE)(ATCMD_RUN_TYPE_MAX - 1);
		}
		AtCmdAnalysis_Net_DeviceSend(deviceIndex);
	 	//���û�м�⵽����,������������
	}
	LINE_LOG();
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
	int deviceIndex = 0;
	if(_gATCmdSetInfo.atCmdGroup < ATCMD_RUN_TYPE_DEVICE_SEND || _gATCmdSetInfo.atCmdGroup >= ATCMD_RUN_TYPE_DEVICE_SEND + _gModuleCount)
		return -1; 

	deviceIndex = _gATCmdSetInfo.atCmdGroup - ATCMD_RUN_TYPE_DEVICE_SEND;	
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		char StrTempBuf[100] = {0};
		int sendPortID = 0;

		uint16_t size = 0;
		uint16_t retSize =0;
		tagHugeSendBuffer hugeBuffer;

		sendPortID = deviceIndex;
		if(_gModeRunState.NetIsOpen[deviceIndex] < MAX_NET_IS_OPEN_NUM
			#if SYSTEM_MAP_ENABLE
			&& (//_gSystemSetVal.sysSetValCtrl.deviceFun == 0 || 
				_gSystemSetVal.NetIsOpen < MAX_NET_IS_OPEN_NUM
				|| _gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_OFF
				|| _gSystemSetVal.sysSetValCtrl.mapDeviceID != deviceIndex
			
				)
			#endif 
			)
		{
			//���豸���ڶϿ���״̬���������û�б�ӳ�䣬����û������״̬�£���ôֱ�ӷ���
			//API_KillTimer(TIMER_TYPE_WCDMA_SEND);
			//API_KillTimer((TIMER_TYPE)(TIMER_TYPE_WCDMA_DEVICE_SEND + deviceIndex));
			//API_ResetProtocolBuffer((USART_FUNCTION)(deviceIndex + USART_FUNCTION_MOUNT),RESETRTBUFF_TYPE_AND_INT_RX);
			goto DEVICE_SEND_ERROR;
			//return -1;
		}
		//���ڷ������ݵ�����
		
		//��ȡ�����͵����ݳ���
	  	size = Netdata_Protocol_ReadSize(deviceIndex,MAX_SEND_DATA_SIZE_ONCE);
		if(size == 0) {
			//�����Ѿ������
			//API_KillTimer(TIMER_TYPE_WCDMA_SEND);
			//API_KillTimer((TIMER_TYPE)(TIMER_TYPE_WCDMA_DEVICE_SEND + deviceIndex));
			//API_ResetProtocolBuffer((USART_FUNCTION)(deviceIndex + USART_FUNCTION_MOUNT),RESETRTBUFF_TYPE_AND_INT_RX);
			goto DEVICE_SEND_ERROR;
			//return -1;
		}
		/*�����ݶ�����*/
		
		AtCmdAnalysis_SetATCmdSendCallBack(ATCMDGROUP_CALLBACK_NAME(DeviceSend));	//���÷��ͳɹ���Ļص�����
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
		#if SYSTEM_MAP_ENABLE
		if(//_gSystemSetVal.sysSetValCtrl.deviceFun == 1&&
				_gSystemSetVal.NetIsOpen == MAX_NET_IS_OPEN_NUM
				&& _gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_ON
				&& _gSystemSetVal.sysSetValCtrl.mapDeviceID == deviceIndex			
				)
		{
			sendPortID = _gSystemSetVal.sysSetValCtrl.controlPort;
		}
		#endif
		sprintf(StrTempBuf,"%d,%d",sendPortID,size);
		AtCmdAnalysis_AtCmd_Send(AT_CMD_TEST_AT,"",0);
 		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPSEND,StrTempBuf,ReadStrLen(StrTempBuf));

		AtCmdAnalysis_AtCmd_SendHugeBuffer(AT_CMD_SET_TCPWDATA,hugeBuffer);
		//AtCmdAnalysis_DeviceBeat_ReStartByDeviceID(deviceIndex,2);
		//AtCmdAnalysis_DeviceBeat_ReStart();	//������ͣ
		TIMER_RUN(StartToSendAtCmd);
		LINE_LOG();
		return 0;

	}
DEVICE_SEND_ERROR:
	API_KillTimer(TIMER_TYPE_WCDMA_SEND);
	API_KillTimer((TIMER_TYPE)(TIMER_TYPE_WCDMA_DEVICE_SEND + deviceIndex));
	API_ResetProtocolBuffer((USART_FUNCTION)(deviceIndex + USART_FUNCTION_MOUNT),RESETRTBUFF_TYPE_AND_INT_RX);

	LINE_LOG();
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
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus !=  ATCMD_RUN_STATUS_NOT_RUN)
	{
		char StrTempBuf[100] = {0};
		int deviceIndex = 0;
		uint16_t size = 0;
		int sendPortID = 0;
		
		if(_gATCmdSetInfo.atCmdGroup < ATCMD_RUN_TYPE_INFO_SEND || _gATCmdSetInfo.atCmdGroup >= ATCMD_RUN_TYPE_INFO_SEND + _gModuleCount)
			return -1; 

		deviceIndex = _gATCmdSetInfo.atCmdGroup - ATCMD_RUN_TYPE_INFO_SEND;
		sendPortID = deviceIndex;
		if(_gModeRunState.NetIsOpen[deviceIndex] < MAX_NET_IS_OPEN_NUM
			#if SYSTEM_MAP_ENABLE
			&& (//_gSystemSetVal.sysSetValCtrl.deviceFun == 0 
				_gSystemSetVal.NetIsOpen < MAX_NET_IS_OPEN_NUM
				|| _gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_OFF
				|| _gSystemSetVal.sysSetValCtrl.mapDeviceID != deviceIndex
				) 
			#endif
			)
		{
			return -1;
		}
		//���ݶ���
		size = _gAtCmdRunCtrlArray[_gATCmdSetInfo.atCmdGroup].funArgSize;// strlen(_gAtCmdRunFunTypeArray[_gATCmdSetInfo.atCmdGroup].funArg);

		//AtCmdAnalysis_DeviceBeat_ReStartByDeviceID(deviceIndex,2);
		#if SYSTEM_MAP_ENABLE
		if(//_gSystemSetVal.sysSetValCtrl.deviceFun == 1 && 
				_gSystemSetVal.NetIsOpen == MAX_NET_IS_OPEN_NUM
				&& _gSystemSetVal.sysMountMapStatus == SYSTEM_MOUNT_MAP_STATUS_ON
				&& _gSystemSetVal.sysSetValCtrl.mapDeviceID == deviceIndex
				)
		{
			sendPortID = _gSystemSetVal.sysSetValCtrl.controlPort;	//�ı�ɼ��ڷ�������
		}
		#endif	
		AtCmdAnalysis_AtCmd_Send(AT_CMD_TEST_AT,"",0);		
		sprintf(StrTempBuf,"%d,%d",sendPortID,size);
 		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPSEND,StrTempBuf,strlen(StrTempBuf));

		AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_TCPWDATA,
			_gAtCmdRunCtrlArray[_gATCmdSetInfo.atCmdGroup].funArg,
			size);

		TIMER_RUN(StartToSendAtCmd);
		LINE_LOG();
		return 0;
	}
	LINE_LOG();

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
	LINE_LOG();
	_gATCmdSetInfo.atCmdRunStatus =  ATCMD_RUN_STATUS_NOT_RUN;
	_gATCmdSetInfo.sendRepeatCount = 0;

	API_WCDMA_Init();
	LINE_LOG();
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
	LINE_LOG();
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
		API_SetTimer(600000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);

		TIMER_RUN(DeviceBeat);
	 	//���û�м�⵽����
	}
	LINE_LOG();
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
	char apnBuffer[50];

	DEBUG("Start to Init setting ev!\n");

	//_gATCmdSetInfo.atCmdRunStatus = ATCMD_RUN_STATUS_RUN_FREE;
	AtCmdAnalysis_SetATCmdSendCallBack(ATCMDGROUP_CALLBACK_NAME(Init));	//���ó�ʼ���ɹ���Ļص�����

	switch(_gModuleSetVal[0].Cns_CNMP)
	{
		case NETWORK_PRIORIYY_TYPE_1G_2G:	//1,2G����
			 netPriBuf = "4";
			 break;
		case NETWORK_PRIORIYY_TYPE_2G_3G:	//2,3G����
			netPriBuf = "8";
			break;
		case NETWORK_PRIORIYY_TYPE_AUTO:	//�Զ�ѡ��
		default:
			netPriBuf = "2";
	}
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_IPREX,"115200",6);
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CATR,"=1",2);
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CNMP,"2",2);
	
	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CSQ,"",0);  //��ѯ�����ź�
	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_COPS,"",0);	//��ѯע������״̬
	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CNSMOD,"",0);  //��ѯ����ϵͳģʽ

	sprintf(apnBuffer,"=1,\"IP\",\"%s\"",_gCnsModSet[0].apn);
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CGSOCKCONT,apnBuffer,ReadStrLen(apnBuffer));  //����APN 
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CSOCKSETPN,"1",1);
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CGREG,"",0);
	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CGREG,"?",1);	//GPRS	

	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGF,"1",1);  //���ö��Ÿ�ʽ 
	//֧������		
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CSMP,"",0);  //����TEXTģʽ����
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CSCS,"\"UCS2\"",6);  //ѡ��TE�ַ���GSM��UCS2��IRA

 	AtCmdAnalysis_AtCmd_Send(AT_CMD_STE_CPMS,"",0);  //���Ŵ洢λ��ѡ������

 	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CNMI,"",0);  //�����¶���Ϣ��ʾ���� 2,1:�洢���� 1,2:���洢���ţ��¶���ֱ�Ӵ�ӡ������

 	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPSRIP,"",0);  //���ò���ʾ����IP 
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CIPCCFG,"",0);  //���ò������׽���
/**/
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_NETOPEN,"",0);  //���ö����ӷ�ʽ
	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_IPADDR,"",0);	//��ȡIP��ַ

 //	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_ATE,"",0);  //�����رջ���





	TIMER_RUN(StartToSendAtCmd);
	LINE_LOG();
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_CALLBACK_FUN(Set3GNetVersion)
// ��������  : ��ʼ���Ļص�����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_CALLBACK_FUN(Set3GNetVersion)
{
	LINE_LOG();
	if(errType != SEND_ERROR_TYPE_NONE)
	{
		//�����⵽����
	}
	else
	{

	}
	DEBUG("Success to Reset!\n");
	API_SetTimer(10000,1,TIMER_TYPE_SYSTEM_RESTART,NULL,TIMER_NAME(SystemReStart));
	LINE_LOG();
	return 0;
}
//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(Set3GNetVersion)
// ��������  : ����3G�汾��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
ATCMDGROUP_FUN(Set3GNetVersion)
{
	char* buf = NULL;
	int len = NULL;
	LINE_LOG();
	buf = _gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_SET3GNETVERSION].funArg;
	len = _gAtCmdRunCtrlArray[ATCMD_RUN_TYPE_SET3GNETVERSION].funArgSize;
	DEBUG("Set 3GNet Version!\n");
	
	AtCmdAnalysis_SetATCmdSendCallBack(ATCMDGROUP_CALLBACK_NAME(Set3GNetVersion));	//���ó�ʼ���ɹ���Ļص�����
	
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CNVW,buf,len);	//����3G�汾��

	TIMER_RUN(StartToSendAtCmd);

	LINE_LOG();

	return 0;
}

//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_CALLBACK_FUN(GetModuleInfo)
// ��������  : ��ȡģ����Ϣ�ص�����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

ATCMDGROUP_CALLBACK_FUN(GetModuleInfo)
{
	LINE_LOG();
	if(errType != SEND_ERROR_TYPE_NONE)
	{
		char buf[20] = {0};

		_gATCmdSetInfo.atCmdRunStatus =  ATCMD_RUN_STATUS_NOT_RUN;	//��־��Not Run״̬
		DEBUG("Check Module ==>failed ,errType:%d!\n",errType);
		sprintf(buf,"errorType:%d,atCmd:%d",errType,atCmd->ATCmd);
		AtCmdAnalysis_ErrorMsg(WCDMA_ERROR_CHECK,buf,strlen(buf));
		AtCmd_RECV_WCDMA_Init();
		//�����⵽����
	}
	else
	{
		DEBUG("Check Module ==>OK!\n");
		API_SetTimer(80000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);	//��ʮ��������������ֱ������

		RUN_ATCMD_GROUP(ATCMD_RUN_TYPE_INIT);
	 	//���û�м�⵽����
	}
	LINE_LOG();
	return 0;
}
//������������������������������������������������������������������������������
// ��������  : ATCMDGROUP_FUN(GetModuleInfo)
// ��������  : ��ȡģ����Ϣ
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
ATCMDGROUP_FUN(GetModuleInfo)
{
	DEBUG("Check Module Info!\n");
	LINE_LOG();

	_gATCmdSetInfo.atCmdRunStatus = ATCMD_RUN_STATUS_RUN_FREE;
	AtCmdAnalysis_SetATCmdSendCallBack(ATCMDGROUP_CALLBACK_NAME(GetModuleInfo));	//���ó�ʼ���ɹ���Ļص�����
	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CFUN,"",0);
	//AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CSQ,"",0);  //��ѯ�����ź�
	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_COPS,"",0);	//��ѯע������״̬

	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CGMR,"",0); //��ѯ�汾��
	AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CSUB,"",0);
//	AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CNAOP,"",0);  //��������˳��
	//AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CNSMOD,"",0);  //��ѯ����ϵͳģʽ

	TIMER_RUN(StartToSendAtCmd);

	LINE_LOG();

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
	LINE_LOG();
	if(_gATCmdSetInfo.atCmdRunStatus ==  ATCMD_RUN_STATUS_RUN_FREE)
	{
		API_SetTimer(600000,-1,TIMER_TYPE_WCDMA_AT_RESTART,TIMER_NAME(StartOk),NULL);
		//ֻ���ڿ���״̬�²Ż�����

		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CSQ,"",0);
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CGREG,"?",1);	//GPRS
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CNSMOD,"",0);
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CCLK,"",0);
		if(_gATCmdSetInfo.haveShortMsg)
		{
			AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGD,"",0);
			_gATCmdSetInfo.haveShortMsg = false;
		}
		else
		{
			AtCmdAnalysis_AtCmd_Send(AT_CMD_SET_CMGL,"\"ALL\"",5);
		}
		AtCmdAnalysis_AtCmd_Send(AT_CMD_GET_CIPOPEN,"",0);
		TIMER_RUN(StartToSendAtCmd);
		LINE_LOG();
		return 0;

	}
	LINE_LOG();
	return -1;
	 
}

int AtCmd_Init(tagModuleSetValStruct* module,
				int moduleCount,
				tagModeRunStateStruct* runState,
				tagATCmdSetInfo* ATCmdSetInfo,
				LinkQueue* cmdLinkQueue,
				tagAtCmdRunCtrlItem* GroupCtrlArray,
				tagSystemSetValStruct*	systemSetVal)
{
	LINE_LOG();
	if(module == NULL || moduleCount < 1 || moduleCount > 10 || runState == NULL || cmdLinkQueue == NULL)
		return -1;
	_gAtCmdRunCtrlArray = GroupCtrlArray;
	_gModuleSetVal = module;
	_gModuleCount =  moduleCount;
	_gModeRunStatePtr = runState;
	_gATCmdSetInfoPtr = ATCmdSetInfo;
	#if SYSTEM_MAP_ENABLE
	_gSystemSetValPtr = systemSetVal;
	#endif
	_gLinkQueue = cmdLinkQueue;
	AtCmdAnalysis_CnsMod_Init(_gCnsModSet,CNSMODSET_SIZE);
	LINE_LOG();
	return AtCmdAnalysis_AtCmd_Resgister(_gATCmd,	 //ATCmd������
									AT_CMD_COUNT,	//��������������
									_gtagATCmdRecvProcessItemArray,	//�ؼ��ֽ�����
									RECV_AT_CMD_SIZE,	//�ؼ�����������
									_gAtCmdRunFunTypeArray
									);
}
