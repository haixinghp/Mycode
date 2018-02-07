#include "config.h"
#include "Protocol.h"

#define USART_FUNCTION_TYPE	 USART_FUNCTION_DEBUG		/*���ڹ�������*/

#define MAX_BUFFER_SIZE		200

static uchar _gRxMessageBuffer[MAX_BUFFER_SIZE]= {0};
static uchar _gTxMessageBuffer[1]= {0};
static ProtocolResource _gProtocol = {0};	/*Э�黺��*/
static UartStruct* _gUartStruct =NULL;		/*����ӳ��ṹ��ָ��*/

//������������������������������������������������������������������������������
// ��������  : Protocol_Debug_Send
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
	Reset_Protocol(&_gProtocol);
}

//������������������������������������������������������������������������������
// ��������  : Protocol_Debug_Receive
// ��������  : ���Խ��պ���
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static int Protocol_Receive()
{
 	uchar chr;		
	if(_gProtocol.Port_Type==PORT_TYPE_SERIAL) //�˿�Ϊ���ڷ�ʽ
	{
        if((_gProtocol.ResState==RESOURCE_USE_STATUS_FREE)
			||(_gProtocol.ResState==RESOURCE_USE_STATUS_RECEIVE))
		{
			
			_gProtocol.Mode_Type = MODE_TYPE_NONE;
		    chr = ReceiveDataFromUartStruct(_gUartStruct);
			DoProtocolReceiveAnalysis(&_gProtocol,chr);
        }
	}
	else
	{ 
		if(_gProtocol.ResState==RESOURCE_USE_STATUS_FREE)
		{
			_gProtocol.Port_Type=PORT_TYPE_SERIAL;
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
// ��������  : Protocol_Debug_DoProtocol
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
// ��������  : Init_Protocol_Debug
// ��������  : ���Գ�ʼ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Init_Protocol_Debug(void)
{
	_gProtocol.RxMessageBuffer = _gRxMessageBuffer;
	_gProtocol.TxMessageBuffer = _gTxMessageBuffer;
	_gProtocol.size	= MAX_BUFFER_SIZE;
	_gProtocol.RxMode = PROTOCOL_RX_MODE_ANALYSIS;

	Reset_Protocol(&_gProtocol);
	_gProtocol.Port_Type = PORT_TYPE_SERIAL;
	_gUartStruct = GetUartStructBaseFunction(USART_FUNCTION_TYPE);
	RegisterProtocolFun(USART_FUNCTION_TYPE,Protocol_Send,Protocol_Receive,Protocol_DoProtocol,&_gProtocol);

	DMA_Usart_Init(USART_FUNCTION_TYPE);
}

//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������
