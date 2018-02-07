#include "config.h"
#include "Protocol.h"
#if CONF_QWL_NET_EN	
#define USART_FUNCTION_TYPE	 USART_FUNCTION_WCDMA		/*���ڹ�������*/

#define MAX_BUFFER_SIZE		800

static uchar _gRxMessageBuffer[MAX_BUFFER_SIZE]= {0}; //��������Լ570byte
static uchar _gTxMessageBuffer[550]= {0};   //����������Լ510byte
static ProtocolResource _gProtocol = {0};	/*Э�黺��*/
static UartStruct* _gUartStruct =NULL;		/*����ӳ��ṹ��ָ��*/

//������������������������������������������������������������������������������
// ��������  : Protocol_Wcdma_Send
// ��������  : ���Է��ͺ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int Protocol_Send()
{
	if(_gUartStruct == NULL)
		return -1;
	if(_gProtocol.TxPro_Lenth==0)
	{
			
		Reset_Protocol(&_gProtocol);		//��λЭ��
		/*����������ɺ�رշ����ж�*/
		EnableCom(_gUartStruct->uCom,false);
		SetupComDir(_gUartStruct,true);	
	}
	else
	{
		if((_gProtocol.PTxDaBuf<_gProtocol.TxMessageBuffer)||(_gProtocol.PTxDaBuf>=(_gProtocol.TxMessageBuffer+_gProtocol.size)))
		{
			Reset_Protocol(&_gProtocol);		//��λЭ��
			/*����������ɺ�رշ����ж�*/
			EnableCom(_gUartStruct->uCom,false);
			SetupComDir(_gUartStruct,true);	
			return -1;
		}
		SendDataFromUartStruct(_gUartStruct,*(_gProtocol.PTxDaBuf));	
		_gProtocol.PTxDaBuf++;
		_gProtocol.TxPro_Lenth--;
		/*�������ݺ󿪷����ж�*/
		EnableCom(_gUartStruct->uCom,true);
	}
 	return 0;
}

//������������������������������������������������������������������������������
// ��������  : ReceiveTimerOut
// ��������  : �������ݳ�ʱ����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void ReceiveTimerOut(void)
{
//	Reset_Protocol(&_gProtocol);
  Reset_And_Init_Protocol_Rx(&_gProtocol);
        
//  Reset_Protocol_Rx(&_gProtocol);
}

//������������������������������������������������������������������������������
// ��������  : Protocol_Wcdma_Receive
// ��������  : ���Խ��պ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int Protocol_Receive()
{
 	uchar chr;
chr = USART1->DR;//ReceiveDataFromUartStruct(_gUartStruct);
			API_net_rec(chr);
	//return 0;//��ʱ	
	if(_gProtocol.Port_Type==PORT_TYPE_WCDMA) //�˿�Ϊ���ڷ�ʽ
	{
        if((_gProtocol.ResState==RESOURCE_USE_STATUS_FREE)
			||(_gProtocol.ResState==RESOURCE_USE_STATUS_RECEIVE))
		{
			
			_gProtocol.Mode_Type =net.mode_type ;
			if(net.reconnect_setp==LINK_OK)
			_gProtocol.Connect_State =CONNECT_STATE_OK;
			else if(net.reconnect_setp<4)
			_gProtocol.Connect_State =CONNECT_STATE_LINK;	
			else
			_gProtocol.Connect_State =CONNECT_STATE_NONE;
			
		    chr = ReceiveDataFromUartStruct(_gUartStruct);

			DoProtocolReceiveAnalysis(&_gProtocol,chr);
        }
	}
	else
	{ 
		if(_gProtocol.ResState==RESOURCE_USE_STATUS_FREE)
		{
			_gProtocol.Port_Type=PORT_TYPE_WCDMA;
			chr = ReceiveDataFromUartStruct(_gUartStruct);
			DoProtocolReceiveAnalysis(&_gProtocol,chr);
		}
		else
		{
		    //���ش����������ڽ���
		}
	}
	API_SetTimer(200,1,(TIMER_TYPE)USART_FUNCTION_TYPE,ReceiveTimerOut,NULL);

	return 0;
}

//������������������������������������������������������������������������������
// ��������  : Protocol_Wcdma_DoProtocol
// ��������  : ���Իص�����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int Protocol_DoProtocol()
{

	if(_gProtocol.ResState == RESOURCE_USE_STATUS_END )  
	{
		API_KillTimer((TIMER_TYPE)USART_FUNCTION_TYPE);
		ReceiveCallBack(USART_FUNCTION_TYPE,_gProtocol.Pro_Type,_gProtocol.PRxDaBuf,_gProtocol.RxPro_Lenth);
	}
	return 0;
}

//������������������������������������������������������������������������������
// ��������  : Init_Protocol_Wcmda
// ��������  : ���Գ�ʼ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Init_Protocol_Wcdma(void)
{
	_gProtocol.RxMessageBuffer = _gRxMessageBuffer;
	_gProtocol.TxMessageBuffer = _gTxMessageBuffer;
	_gProtocol.size	= MAX_BUFFER_SIZE;
	_gProtocol.RxMode = PROTOCOL_RX_MODE_ANALYSIS;

	Reset_Protocol(&_gProtocol);
	_gProtocol.Port_Type = PORT_TYPE_WCDMA;
	_gUartStruct = GetUartStructBaseFunction(USART_FUNCTION_TYPE);
	RegisterProtocolFun(USART_FUNCTION_TYPE,Protocol_Send,Protocol_Receive,Protocol_DoProtocol,&_gProtocol);

	DMA_Usart_Init(USART_FUNCTION_TYPE);
}
#endif
//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������
