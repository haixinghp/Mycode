#include "config.h"

/*�����ӳ��Ĵ��ڶ˿�*/
#define   ETH_MAP_PORT	 USART_FUNCTION_DEBUG
const PINFO _gProtocol_info[PROTOCOL_TYPE_MAX]=
{
  {1,{0x7E,0x00,0x00,0x00}, 9, 1,{0x0D,0x00,0x00,0x00},PROTOCOL_TYPE_POSTS},	//�ʵ粿Э��
  //	{1,{0x2A,0x00,0x00,0x00}, 6, 1,{0x0D,0x00,0x00,0x00},PROTOCOL_TYPE_COMPANY},//��˾Э��
  //	{1,{0xF0,0x00,0x00,0x00},18,1,{0x0D,0x00,0x00,0x00},PROTOCOL_TYPE_WIRELESS},//����Э��
  {1,{0xAA,0x00,0x00,0x00},11,4,{0xCC,0xEE,0xC3,0x3C},PROTOCOL_TYPE_DOWNLOAD},//��������Э��,
  //ע�⣬�����Ľ�βλ{0xCC,0x33,0xC3,0x3C}��������ִ��ʱ��س�����ȱ�ݣ���ҪԶ�������������
};
#if CONF_QWL_NET_EN
const PINFO_BC95 _gProtocol_info1[PROTOCOL_TYPE_MAX]=
{
  {2,{'7','E',0x00,0x00,0x00,0x00,0x00,0x00},16,2,{'0','D',0x00,0x00,0x00,0x00,0x00,0x00},PROTOCOL_TYPE_POSTS},	//�ʵ粿Э��
  {2,{'A','A',0x00,0x00,0x00,0x00,0x00,0x00},22,8,{'C','C','E','E','C','3','3','C'},PROTOCOL_TYPE_DOWNLOAD},//��������Э��,
  //ע�⣬�����Ľ�βλ{0xCC,0x33,0xC3,0x3C}��������ִ��ʱ��س�����ȱ�ݣ���ҪԶ�������������
};
#endif
#define PINFO_SIZE	  2//��Э������

typedef struct {
  ProtocolFun send;		//���ͺ���
  ProtocolFun recv;		//���պ���
  ProtocolFun doProtocol;	//���յ�������Ϻ�������������ִ��
  ProtocolResource* pRes;	//Э��
  ReceiveFun rFun;		//�������ݺ�Ļص�����
}ProtocolFunStruct;

static ProtocolFunStruct _gProtocolFun[USART_FUNCTION_MAX] ={
  0
};

NetSendNum gNetSend;			   //����ְ����ͱ�־

//������������������������������������������������������������������������������
// ��������  : Protocols_Init
// ��������  : Э���ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void Protocols_Init(void)
{
  Uart_Init();
  //Init_Protocol_Debug();
#if CONF_QWL_NET_EN
  Init_Protocol_Wcdma();
#endif
  /*ע��debugЭ��˿����ݽ��մ���ص�����*/
  //SetProtocolReceiveCallBack(USART_FUNCTION_DEBUG,ReceivePro_Debug_Dispose);
#if CONF_QWL_NET_EN
  /*ע��wcdmaЭ��˿����ݽ��մ���ص�����*/
  SetProtocolReceiveCallBack(USART_FUNCTION_WCDMA,ReceivePro_Debug_Dispose);
#endif
}

//������������������������������������������������������������������������������
// ��������  : RegisterProtocolFun
// ��������  : ע��һ��Э�����ز���
// �������  : uf,Э������ send,���ͺ��� recv,���պ���  doProtocol,�ص����� pRes,Э�黺��
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int RegisterProtocolFun(USART_FUNCTION uf,ProtocolFun send,ProtocolFun recv,ProtocolFun doProtocol,ProtocolResource* pRes)
{
  if(uf < USART_FUNCTION_MAX)
  {
    _gProtocolFun[uf].send = send;
    _gProtocolFun[uf].recv = recv;
    _gProtocolFun[uf].doProtocol = doProtocol;
    _gProtocolFun[uf].pRes = pRes;
    
    return 0;
  }
  return -1;
}
//������������������������������������������������������������������������������
// ��������  : getPINFO
// ��������  : ��ȡЭ��ṹ
// �������  : pt,Э������
// �������  : None
// ���ز���  : Э��ṹ
//������������������������������������������������������������������������������
#if CONF_QWL_NET_EN
const PINFO_BC95* getPINFO_BC95(PROTOCOL_TYPE pt)
{
  int i = 0;
  for(i = 0; i < PINFO_SIZE; i++)
  {
    if(_gProtocol_info1[i].ProtcolID == pt )
    {
      return &_gProtocol_info1[i];
    }
  }
  return NULL;
}
#endif
//������������������������������������������������������������������������������
// ��������  : getPINFO
// ��������  : ��ȡЭ��ṹ
// �������  : pt,Э������
// �������  : None
// ���ز���  : Э��ṹ
//������������������������������������������������������������������������������
const PINFO* getPINFO(PROTOCOL_TYPE pt)
{
  int i = 0;
  for(i = 0; i < PINFO_SIZE; i++)
  {
    if(_gProtocol_info[i].ProtcolID == pt )
    {
      return &_gProtocol_info[i];
    }
  }
  return NULL;
}

//������������������������������������������������������������������������������
// ��������  : SendProtocolBuffer
// ��������  : ��ָ����Э�����ͷ�������
// �������  : uf,Э������ buffer,���� size,���ݴ�С
// �������  : None
// ���ز���  : 0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int SendProtocolBuffer(USART_FUNCTION uf,uchar* buffer, size_t size)
{
  if(AddProtocolBuffer(uf,buffer,size)>-1) 
  {
    return StartSendBuffer(uf);
  }
  return -1;
}

//������������������������������������������������������������������������������
// ��������  : ResetProtocolBuffer
// ��������  : ��λ�շ�����
// �������  : uf,Э������	 ResetType��λѡ��
// �������  : None
// ���ز���  : 0:�ɹ� -1:ʧ��
//������������������������������������������������������������������������������
int ResetProtocolBuffer(USART_FUNCTION uf,RESETRTBUFFTYPE ResetType)
{
  ProtocolResource *Protocol = NULL;
  if(uf >=USART_FUNCTION_MAX)
    return -1;
  Protocol = _gProtocolFun[uf].pRes;
  if(Protocol == NULL)
    return -1;
  switch(ResetType)
  {
  case RESETRTBUFF_TYPE_ALL:
    Reset_Protocol(Protocol);
    break;
  case RESETRTBUFF_TYPE_RX:
    Reset_Protocol_Rx(Protocol);
    break;
  case RESETRTBUFF_TYPE_TX:
    Reset_Protocol_Tx(Protocol);
    break;
  }
  return 0;
}


//������������������������������������������������������������������������������
// ��������  : AddProtocolBuffer
// ��������  : ��ָ����Э�����ͻ�����ӷ�������
// �������  : uf,Э������ buffer,���� size,���ݴ�С
// �������  : None
// ���ز���  : 0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int AddProtocolBuffer(USART_FUNCTION uf,uchar* buffer, size_t size)
{
  ProtocolResource *Protocol = NULL;
  if(uf >=USART_FUNCTION_MAX || buffer == NULL || size < 1)
    return -1;
  Protocol = _gProtocolFun[uf].pRes;
  if(Protocol == NULL)
    return -1;
  if(((Protocol->PTxDaBuf+size)<Protocol->TxMessageBuffer)||((Protocol->PTxDaBuf+size)>=(Protocol->TxMessageBuffer+Protocol->size)))
  {
    Reset_Protocol(Protocol);
    return -1;
  }
  memcpy(Protocol->PTxDaBuf,buffer,size);
  Protocol->PTxDaBuf += size;
  Protocol->TxPro_Lenth += size;
  
  return 0;
}

//������������������������������������������������������������������������������
// ��������  : GetProtocolBufferOrLen
// ��������  : ��ȡָ����Э�鷢�ͻ���ָ��ͳ���
// �������  : uf,Э������
// �������  : ptr���ͻ���ָ��	len���ͳ���
// ���ز���  : 0:�ɹ� -1:ʧ��
//������������������������������������������������������������������������������
int GetProtocolBufferOrLen(USART_FUNCTION uf,uchar **ptr,uint16 **len)
{
  ProtocolResource *Protocol = NULL;
  if(uf >=USART_FUNCTION_MAX)
    return -1;
  Protocol = _gProtocolFun[uf].pRes;
  if(Protocol == NULL)
    return -1;
  *ptr = (uchar *)&(Protocol->TxMessageBuffer[0]);
  *len = (uint16 *)&(Protocol->TxPro_Lenth);
  
  return 0;
}





//������������������������������������������������������������������������������
// ��������  : StartSendBuffer
// ��������  : ָ����Э������������������
// �������  : uf,Э������
// �������  : None
// ���ز���  : 0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int StartSendBuffer(USART_FUNCTION uf)
{
  ProtocolResource *Protocol = NULL;
  if(uf >=USART_FUNCTION_MAX)
    return -1;
  Protocol = _gProtocolFun[uf].pRes;
  if(Protocol == NULL)
    return -1;
  
  Protocol->ResState = RESOURCE_USE_STATUS_SEND;
  
  switch(Protocol->Port_Type)	  
  {
  case PORT_TYPE_SERIAL:
    /*��������ǰ�Ƚ�RS485������ƽ���λ����״̬*/
    SetupComDir(GetUartStructBaseFunction(uf),false);
    Init_ProSendPrt(Protocol);
    ProtocolSendByte(uf);
    break;
    
  case PORT_TYPE_WCDMA:
    /*��������ǰ�Ƚ�RS485������ƽ���λ����״̬*/
    //SetupComDir(GetUartStructBaseFunction(uf),false);
    //Init_ProSendPrt(Protocol);
#if CONF_QWL_NET_EN
    net_send(Protocol->TxMessageBuffer,Protocol->TxPro_Lenth);
#endif
    //ProtocolSendByte(uf);
    break;
  default:break;
  
  }
  
  return 0;
}

//������������������������������������������������������������������������������
// ��������  : ProtocolSendByte
// ��������  : ��ָ����Э�����ͷ��͵�������
// �������  : uf,Э������
// �������  : None
// ���ز���  : 0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int ProtocolSendByte(USART_FUNCTION uf)
{
  if(uf < USART_FUNCTION_MAX)
  {
    if(_gProtocolFun[uf].send != NULL)
    {
      return _gProtocolFun[uf].send();
    }
  }
  
  return -1;
}

//������������������������������������������������������������������������������
// ��������  : ProtocolReceiveByte
// ��������  : ����ָ����Э�����ͻ�ȡ��������
// �������  : uf,Э������
// �������  : None
// ���ز���  : 0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int ProtocolReceiveByte(USART_FUNCTION uf)
{
  if(uf < USART_FUNCTION_MAX)
  {
    if(_gProtocolFun[uf].send != NULL)
    {
      return _gProtocolFun[uf].recv();
    }
  }
  
  return -1;
}
//������������������������������������������������������������������������������
// ��������  : CheckProtocolType
// ��������  : �ж�Э������
// �������  : Protocol,Э�黺��
// �������  : None
// ���ز���  : Э������
//������������������������������������������������������������������������������
#if CONF_QWL_NET_EN
static PROTOCOL_TYPE CheckProtocolType_BC95(ProtocolResource *Protocol)
{
  int index = 0;
  PROTOCOL_TYPE pt = 	PROTOCOL_TYPE_NONE;
  if(Protocol->RxPro_Lenth < 1)
  {
    return PROTOCOL_TYPE_NONE;	// ���յ�������Ϊ�գ���ô�޴��жϣ�ֻ��Ϊ��
  }
  for(index = 0; index <PINFO_SIZE; index++)
  {
    int RHIndex = 0; 
    uchar RHLenth = _gProtocol_info1[index].RHLenth;	//ͷ�ֽڵĳ���
    uchar minLen = Protocol->RxPro_Lenth > RHLenth?RHLenth:Protocol->RxPro_Lenth;	//ȡ�������ȵ���Сֵ����ֹ���
    for(RHIndex = 0; RHIndex < minLen; RHIndex++) {
      
      if(Protocol->PRxDaBuf[RHIndex] != _gProtocol_info1[index].RHead[RHIndex])
        break;
      
    }
    
    if(RHIndex < RHLenth && RHIndex == minLen && pt == PROTOCOL_TYPE_NONE)
      pt = PROTOCOL_TYPE_CHECKING;
    if(RHIndex == RHLenth) {
      pt =  _gProtocol_info1[index].ProtcolID;	//��ȡ��ȫƥ���Э��
      break;
    } 
  }
  
  return pt;
}
#endif
//������������������������������������������������������������������������������
// ��������  : CheckProtocolType
// ��������  : �ж�Э������
// �������  : Protocol,Э�黺��
// �������  : None
// ���ز���  : Э������
//������������������������������������������������������������������������������
static PROTOCOL_TYPE CheckProtocolType(ProtocolResource *Protocol)
{
  int index = 0;
  PROTOCOL_TYPE pt = 	PROTOCOL_TYPE_NONE;
  if(Protocol->RxPro_Lenth < 1)
  {
    return PROTOCOL_TYPE_NONE;	// ���յ�������Ϊ�գ���ô�޴��жϣ�ֻ��Ϊ��
  }
  for(index = 0; index <PINFO_SIZE; index++)
  {
    int RHIndex = 0; 
    uchar RHLenth = _gProtocol_info[index].RHLenth;	//ͷ�ֽڵĳ���
    uchar minLen = Protocol->RxPro_Lenth > RHLenth?RHLenth:Protocol->RxPro_Lenth;	//ȡ�������ȵ���Сֵ����ֹ���
    for(RHIndex = 0; RHIndex < minLen; RHIndex++) {
      
      if(Protocol->PRxDaBuf[RHIndex] != _gProtocol_info[index].RHead[RHIndex])
        break;
      
    }
    
    if(RHIndex < RHLenth && RHIndex == minLen && pt == PROTOCOL_TYPE_NONE)
      pt = PROTOCOL_TYPE_CHECKING;
    if(RHIndex == RHLenth) {
      pt =  _gProtocol_info[index].ProtcolID;	//��ȡ��ȫƥ���Э��
      break;
    } 
  }
  
  return pt;
}

//������������������������������������������������������������������������������
// ��������  : DoProtocols
// ��������  : ����ѭ������ִ�лص�����
// �������  : None
// �������  : None
// ���ز���  :  0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int DoProtocols(void)
{
  int index = 0;
  for(index = 0; index <USART_FUNCTION_MAX; index++)
  {
    
    if(_gProtocolFun[index].doProtocol != NULL)
    {
      
      _gProtocolFun[index].doProtocol();
    }
    
  }	
  
  return 0;
  
}

//������������������������������������������������������������������������������
// ��������  : GetProtocolResource
// ��������  : ��ȡЭ�黺��
// �������  : ufЭ������
// �������  : None
// ���ز���  : Э�黺��
//������������������������������������������������������������������������������
ProtocolResource * GetProtocolResource(USART_FUNCTION uf)
{
  if(uf < USART_FUNCTION_MAX)
  {
    return _gProtocolFun[uf].pRes; 		
  }
  return NULL;
}

//������������������������������������������������������������������������������
// ��������  : ReceiveCallBack
// ��������  : ִ�н��ջص�����
// �������  : ufЭ������ buffer�������ݻ���  size����
// �������  : None
// ���ز���  :  0:�ɹ� -1:ʧ��
//������������������������������������������������������������������������������
int ReceiveCallBack(USART_FUNCTION uf,PROTOCOL_TYPE Pro_Type,uchar* buffer, size_t size)
{
  if(uf < USART_FUNCTION_MAX)
  {
    if(_gProtocolFun[uf].rFun != NULL)
    {
      
      return _gProtocolFun[uf].rFun(uf,Pro_Type,buffer,size);
    } 		
  }
  
  return -1;
}

//������������������������������������������������������������������������������
// ��������  : SetProtocolReceiveCallBack
// ��������  : ���ý��ջص�����
// �������  : ufЭ������ fun�ص�����
// �������  : None
// ���ز���  :  0:�ɹ� -1:ʧ��
//������������������������������������������������������������������������������
int SetProtocolReceiveCallBack(USART_FUNCTION uf,ReceiveFun fun)
{
  if(uf < USART_FUNCTION_MAX)
  {
    _gProtocolFun[uf].rFun = fun;
    return 0;
  }
  
  return -1;	
}



//������������������������������������������������������������������������������
// ��������  : DoProtocolAnalysis_BC95
// ��������  : Э�����ݽ��ս�������,רΪNB��ͨ����ͨ��
// �������  : Protocol,Э�� ChrData,���յ�һ���ֽ�.
// �������  : None
// ���ز���  :  0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
#if CONF_QWL_NET_EN
int DoProtocolReceiveAnalysis_BC95(ProtocolResource *Protocol,uchar ChrData)  // ����һ��ȫ����������
{
  PROTOCOL_TYPE pt = PROTOCOL_TYPE_NONE;
  if(Protocol == NULL)
    return -1;
  if((Protocol->ResState == RESOURCE_USE_STATUS_FREE)
    ||(Protocol->RxPro_Lenth<1))  //���ڿ��е�״̬�����ҽ��յ����ݳ���Ϊ0.
  {
//    Reset_Protocol(Protocol);		//��Ҫ�Ƚ��и�λ	
//    Reset_Protocol_Rx(Protocol); // ֻ��λ���ջ��棬by hx 2017-12-09
    Reset_And_Init_Protocol_Rx(Protocol); //by hx 2017-12-16
    Protocol->ResState = RESOURCE_USE_STATUS_RECEIVE;	//��ռЭ������,����Ϊ����״̬��		
  }
  if(Protocol->ResState != RESOURCE_USE_STATUS_RECEIVE) //Э�鴦�ڷǽ���״̬��
    return -1;
  Protocol->PRxDaBuf[Protocol->RxPro_Lenth] = ChrData;
  Protocol->RxPro_Lenth++;  //�������ݹ��̡�
  
  if(Protocol->RxPro_Lenth + (Protocol->PRxDaBuf - (uchar*)Protocol->RxMessageBuffer) >= Protocol->size)	  //MAXPCReLen
  {
    //��������󻺴�ֵ
//    Reset_Protocol(Protocol);
//    Reset_Protocol_Rx(Protocol); // ֻ��λ���ջ��棬by hx 2017-12-09
    Reset_And_Init_Protocol_Rx(Protocol); //by hx 2017-12-16
  }
  if(Protocol->Pro_Type < PROTOCOL_TYPE_MAX)
  {
    //���Э���Ѿ�����ֵ
    pt =  Protocol->Pro_Type;
  } else {
    //û�б���ֵ����ʼ����Э����
    while((pt = CheckProtocolType_BC95(Protocol)) == PROTOCOL_TYPE_NONE 
        && Protocol->RxPro_Lenth > 0) 
        {
      Protocol->RxPro_Lenth--;
      Protocol->PRxDaBuf++;
    }
    Protocol->Pro_Type = pt;		
  }
  if(pt < PROTOCOL_TYPE_MAX)	//��⵽��׼��Э����
  {
    
    const PINFO_BC95* pinf = getPINFO_BC95(pt);
    if(pinf != NULL)
    {
      int k = 0;
      uchar* pChar = (Protocol->PRxDaBuf + (Protocol->RxPro_Lenth - pinf->RTLenth));/*���һ���ַ�*/
      u8 qwl_RTail[8]={'C','C','E','E','C','3','3','C'};
      if(pinf->RTLenth==8)
      {
        for(k = 0; k < pinf->RTLenth; k++)
        {
          if(qwl_RTail[k] != pinf->RTail[k])
            break;
        }
        
        if(k >= pinf->RTLenth)
        {
          qwl_RTail[2]='3';
          qwl_RTail[3]='3';
        }
      }
      else
      {
        for(k = 0; k < pinf->RTLenth; k++)
        {
          qwl_RTail[k] = pinf->RTail[k];
        }
      }
      
      
      
      for(k = 0; k <  pinf->RTLenth; k++)
      {
        if(pChar[k] != qwl_RTail[k]) {
          break;
        }	
      }
      if(k >= pinf->RTLenth)
      {
        if(Protocol->RxPro_Lenth >= pinf->RSLenth )
        {
          
          u8 BC95_rec_buf[300]={0};
          u16 BC95_rec_len=0;
          u16 i;
          for(i=0;i<(Protocol->RxPro_Lenth);i=i+2)
          {
            BC95_rec_buf[BC95_rec_len]=(ASCII_hex[(Protocol->PRxDaBuf[i]-0x30)]<<4)+ASCII_hex[(Protocol->PRxDaBuf[i+1]-0x30)];
            BC95_rec_len++;
          }
          
          for(i=0;i<BC95_rec_len;i++)
          {
            Protocol->PRxDaBuf[i]=BC95_rec_buf[i];	
          }
          
          
          Protocol->RxPro_Lenth=BC95_rec_len;
          
          Protocol->ResState = RESOURCE_USE_STATUS_END;	//ֱ�ӽ���
          //qwl_sendstring(Protocol->PRxDaBuf);
          
        }
        else
        {
//          Reset_Protocol(Protocol);		//��Ҫ�Ƚ��и�λ		
//          Reset_Protocol_Rx(Protocol); // ֻ��λ���ջ��棬by hx 2017-12-09
          Reset_And_Init_Protocol_Rx(Protocol); //by hx 2017-12-16
        }
      }  
    }
  }
  return 0;
}
#endif
//������������������������������������������������������������������������������
// ��������  : DoProtocolAnalysis
// ��������  : Э�����ݽ��ս�������
// �������  : Protocol,Э�� ChrData,���յ�һ���ֽ�.
// �������  : None
// ���ز���  :  0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int DoProtocolReceiveAnalysis_ZTE(ProtocolResource *Protocol,uchar ChrData)  // ����һ��ȫ����������
{
  PROTOCOL_TYPE pt = PROTOCOL_TYPE_NONE;
  if(Protocol == NULL)
    return -1;
  if((Protocol->ResState == RESOURCE_USE_STATUS_FREE)||(Protocol->RxPro_Lenth<1))
  {
    Reset_And_Init_Protocol_Rx(Protocol);
//    Reset_Protocol(Protocol);		//��Ҫ�Ƚ��и�λ	
//    Reset_Protocol_Rx(Protocol);
    Protocol->ResState = RESOURCE_USE_STATUS_RECEIVE;	//��ռЭ������			
  }
  if(Protocol->ResState != RESOURCE_USE_STATUS_RECEIVE) //Э�鴦�ڷǽ���״̬��
    return -1;
  Protocol->PRxDaBuf[Protocol->RxPro_Lenth] = ChrData;
  Protocol->RxPro_Lenth++;
  
  if(Protocol->RxPro_Lenth + (Protocol->PRxDaBuf - (uchar*)Protocol->RxMessageBuffer) >= Protocol->size)	  //MAXPCReLen
  {
    //��������󻺴�ֵ
    Reset_Protocol(Protocol);
  }
  if(Protocol->Pro_Type < PROTOCOL_TYPE_MAX)
  {
    //���Э���Ѿ�����ֵ
    pt =  Protocol->Pro_Type;
  } else {
    //û�б���ֵ����ʼ����Э����
    while((pt = CheckProtocolType(Protocol)) == PROTOCOL_TYPE_NONE && Protocol->RxPro_Lenth > 0) {
      Protocol->RxPro_Lenth--;
      Protocol->PRxDaBuf++;
    }
    Protocol->Pro_Type = pt;		
  }
  if(pt < PROTOCOL_TYPE_MAX)	//��⵽��׼��Э����
  {
    
    const PINFO* pinf = getPINFO(pt);
    if(pinf != NULL)
    {
      int k = 0;
      uchar* pChar = (Protocol->PRxDaBuf + (Protocol->RxPro_Lenth - pinf->RTLenth));/*���һ���ַ�*/
      u8 qwl_RTail[4]={0xCC,0xEE,0xC3,0x3C};
      if(pinf->RTLenth==4)
      {
        for(k = 0; k < pinf->RTLenth; k++)
        {
          if(qwl_RTail[k] != pinf->RTail[k])
            break;
        }
        
        if(k >= pinf->RTLenth)
        {
          qwl_RTail[1]=0x33;
        }
      }
      else
      {
        for(k = 0; k < pinf->RTLenth; k++)
        {
          qwl_RTail[k] = pinf->RTail[k];
        }
      }
      
      
      
      for(k = 0; k <  pinf->RTLenth; k++)
      {
        if(pChar[k] != qwl_RTail[k]) {
          break;
        }	
      }
      if(k >= pinf->RTLenth)
      {
        if(Protocol->RxPro_Lenth >= pinf->RSLenth )
        {
          Protocol->ResState = RESOURCE_USE_STATUS_END;	//ֱ�ӽ���
        }
        else
        {
          Reset_Protocol(Protocol);		//��Ҫ�Ƚ��и�λ		
        }
      }  
    }
  }
  return 0;
}

//������������������������������������������������������������������������������
// ��������  : DoProtocolAnalysis
// ��������  : Э�����ݽ��ս�������
// �������  : Protocol,Э�� ChrData,���յ�һ���ֽ�.
// �������  : None
// ���ز���  :  0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int DoProtocolReceiveAnalysis(ProtocolResource *Protocol,uchar ChrData)  // ����һ��ȫ����������
{
#if CONF_QWL_NET_EN
  if(Protocol->Mode_Type ==MODE_TYPE_BC95)
  {
    return DoProtocolReceiveAnalysis_BC95(Protocol,ChrData);
  }
  else
#endif
  {
    return DoProtocolReceiveAnalysis_ZTE(Protocol,ChrData);
  }
  
}

//������������������������������������������������������������������������������
// ��������  : DoProtocol_DMA_ReceiveAnalysis
// ��������  : Э������DMA���ս�������
// �������  : Protocol,Э�� len,DMA���ճ���.
// �������  : None
// ���ز���  :  0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int DoProtocol_DMA_ReceiveAnalysis(ProtocolResource *Protocol,int len)  // ����һ��ȫ����������
{	
  int i = 0;
  PROTOCOL_TYPE pt = PROTOCOL_TYPE_NONE;
  if(Protocol == NULL)
    return -1;
  for(i=0;i<len;i++)
  {
    if((Protocol->ResState == RESOURCE_USE_STATUS_FREE)||(Protocol->RxPro_Lenth<1))
    {
      Reset_Protocol(Protocol);		//��Ҫ�Ƚ��и�λ		
      Protocol->ResState = RESOURCE_USE_STATUS_RECEIVE;	//��ռЭ������			
    }
    if(Protocol->ResState != RESOURCE_USE_STATUS_RECEIVE) //Э�鴦�ڷǽ���״̬��
      return -1;
    Protocol->RxPro_Lenth++;
    
    if(Protocol->RxPro_Lenth + (Protocol->PRxDaBuf - (uchar*)Protocol->RxMessageBuffer) >= Protocol->size)	  //MAXPCReLen
    {
      //��������󻺴�ֵ
      Reset_Protocol(Protocol);
    }
    if(Protocol->Pro_Type < PROTOCOL_TYPE_MAX)
    {
      //���Э���Ѿ�����ֵ
      pt =  Protocol->Pro_Type;
    } 
    else 
    {
      //û�б���ֵ����ʼ����Э����
      while((pt = CheckProtocolType(Protocol)) == PROTOCOL_TYPE_NONE && Protocol->RxPro_Lenth > 0) 
      {
        Protocol->RxPro_Lenth--;
        Protocol->PRxDaBuf++;
      }
      Protocol->Pro_Type = pt;		
    }
    
    if(pt < PROTOCOL_TYPE_MAX)	//��⵽��׼��Э����
    {
      
      const PINFO* pinf = getPINFO(pt);
      if(pinf != NULL)
      {
        int k = 0;
        uchar* pChar = (Protocol->PRxDaBuf + (Protocol->RxPro_Lenth - pinf->RTLenth));/*���һ���ַ�*/
        u8 qwl_RTail[4]={0xCC,0xEE,0xC3,0x3C};
        if(pinf->RTLenth==4)
        {
          for(k = 0; k < pinf->RTLenth; k++)
          {
            if(qwl_RTail[k] != pinf->RTail[k])
              break;
          }
          
          if(k >= pinf->RTLenth)
          {
            qwl_RTail[1]=0x33;
          }
        }
        else
        {
          for(k = 0; k < pinf->RTLenth; k++)
          {
            qwl_RTail[k] = pinf->RTail[k];
          }
        }	
        
        for(k = 0; k <  pinf->RTLenth; k++)
        {
          if(pChar[k] != qwl_RTail[k]) {
            break;
          }	
        }
        if(k >= pinf->RTLenth)
        {
          if(Protocol->RxPro_Lenth >= pinf->RSLenth )
          {
            Protocol->ResState = RESOURCE_USE_STATUS_END;	//ֱ�ӽ���
          }
          else
          {
            Reset_Protocol(Protocol);		//��Ҫ�Ƚ��и�λ		
          }
        }  
      }
    }
  }
  return 0;
}

//������������������������������������������������������������������������������
// ��������  : SettingCommBps
// ��������  : ��Ч���ڲ�������
// �������  : None
// �������  : None
// ���ز���  :  0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
USART_FUNCTION _guf;
void SettingCommBps(void)
{
  if(_guf < USART_FUNCTION_MAX)
  {
    SettingSaveByUSART_FUNCTION(_guf);
  }
}
//������������������������������������������������������������������������������
// ��������  : SetUartParameter
// ��������  : ���ڲ�������
// �������  : None
// �������  : None
// ���ز���  :  0:�ɹ� 1:ʧ��
//������������������������������������������������������������������������������
int SetUartParameter(
                     USART_FUNCTION uf,					//����Э��
                     uint32_t baudRate,	 				//������
                     uint16_t parity,					//У��λ
                     uint16_t wordLength,				//���ݳ���
                     uint16_t stopBits					//ֹͣλ
                       )
{
  UartStruct* us = GetUartStructBaseFunction(uf);
  if(us != NULL)
  {
    UartCom * uCom =us->uCom;
    if(uCom != NULL)
    {
      uCom->UartCommAag.baudRate = baudRate;
      uCom->UartCommAag.parity = parity;
      uCom->UartCommAag.wordLength = 	wordLength;
      uCom->UartCommAag.stopBits = stopBits;
      
      _guf = uf;
      SetTimer(2000,1,TIMER_TYPE_SETCOMMBPS,SettingCommBps,NULL);//API_SetTimer(2000,1,TIMER_TYPE_SETCOMMBPS,SettingCommBps,NULL);
      
      return 0;
    }
  }
  return -1;	
}

//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������

