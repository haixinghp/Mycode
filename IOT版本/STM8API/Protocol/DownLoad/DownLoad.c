#include "config.h"

void DownLoad_DealPro(USART_FUNCTION uf,uchar *DaPtr,uint16 len);

//������������������������������������������������������������������������������
// ��������  : DownLoad_SendByte
// ��������  : ����һ�ֽ�����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void DownLoad_SendByte(USART_FUNCTION uf,uchar ChDa)
{
	API_AddProtocolBuffer(uf,(uchar *)&ChDa, 1);
}

//������������������������������������������������������������������������������
// ��������  : DownLoad_SendHead
// ��������  : ��������֡ͷ����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void DownLoad_SendHead(USART_FUNCTION uf,uchar Addr,uchar cmd)
{	
    API_ResetProtocolBuffer(uf,RESETRTBUFF_TYPE_TX);
	DownLoad_SendByte(uf,0xAA);		//Э��ͷ
	DownLoad_SendByte(uf,Addr); 
	DownLoad_SendByte(uf,cmd);   	
	DownLoad_SendByte(uf,0x00);   	//���ⳤ�ȸ��ֽ�
	DownLoad_SendByte(uf,0x00);   	//���ⳤ�ȵ��ֽ�
}


//������������������������������������������������������������������������������
// ��������  : DownLoad_SendChksum
// ��������  : �ط���У��ͺ���
// �������  : len У�����ݳ���.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void DownLoad_SendChksum(USART_FUNCTION uf)  
{
	uchar *TxBuf;
	uint16 *Len;
	uchar *p;
	uint i,ChksumData=0;

	if(API_GetProtocolBufferOrLen(uf,&TxBuf,&Len)>-1)
	{
		/*��������峤��*/
		TxBuf[3] = (uchar)((*Len-5)>>8);
		TxBuf[4] = (uchar)(*Len-5);	

		/*����У���*/
		p = TxBuf+1;
		for(i=0;i<(*Len)-1;i++)
		{
			ChksumData+=*p++;
		}
	}
	DownLoad_SendByte(uf,(uchar)(ChksumData>>8));
	DownLoad_SendByte(uf,(uchar)ChksumData);
}

//������������������������������������������������������������������������������
// ��������  : LCDSendTail
// ��������  : ����֡β
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void DownLoad_SendTail(USART_FUNCTION uf)
{	
	DownLoad_SendByte(uf,0xCC);
	DownLoad_SendByte(uf,0x33);
	DownLoad_SendByte(uf,0xC3);
	DownLoad_SendByte(uf,0x3C);
	StartSendBuffer(uf);
        ResetProtocolBuffer(uf,RESETRTBUFF_TYPE_TX);
}

//������������������������������������������������������������������������������
// ��������  : DownLoad_DealProtocol
// ��������  : ͨ��Э�����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
#define INFO_START 5
#define INFO_END   6
void DownLoad_DealProtocol(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{
        uint16  chksum_len = 0;
	if((DaPtr[1] == ModAddress)||(DaPtr[1] == 0xff))
	{
          chksum_len = (uint16)(DaPtr[3]<<8)+(uint16)DaPtr[4];
          if(chksum_len == (len - INFO_START - INFO_END)){//У�鳤��  ����ʱ��2017/10/26
		if(chk_LcdMode_chksum((uchar *)&DaPtr[1],len-7) == 1)//У������
		{
			DownLoad_DealPro(uf,DaPtr,len);
		}
          }
	}
	else
	{
		API_ResetProtocolBuffer(uf,RESETRTBUFF_TYPE_ALL);
	}
}

//������������������������������������������������������������������������������
// ��������  : DownLoad_DealPro
// ��������  : Э�鴦��
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������ 
void DownLoad_DealPro(USART_FUNCTION uf,uchar *DaPtr,uint16 len)
{
	switch(uf)
	{
		case USART_FUNCTION_WCDMA:
			DownLoad_Pro(uf,DaPtr,len);
			break;
		case USART_FUNCTION_MOUNT + 0:
			break;
		case USART_FUNCTION_MOUNT + 1:
			break;
		case USART_FUNCTION_MSG:
			break;
		case USART_FUNCTION_DEBUG:
			DownLoad_Pro(uf,DaPtr,len);
			break;
	}
}
//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������

