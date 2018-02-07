#include "config.h"

//������������������������������������������������������������������������������
// ��������  : ReceivePro_Debug_Dispose
// ��������  : �յ���Debug��Э�����ݽ����жϴ���
// �������  : None
// �������  : None
// ���ز���  : �ɹ�����0��ʧ��-1
//������������������������������������������������������������������������������
int ReceivePro_Debug_Dispose(USART_FUNCTION uf,PROTOCOL_TYPE Pro_Type,uchar* buffer, size_t size)
{
	switch(Pro_Type)
	{
		case PROTOCOL_TYPE_POSTS:	 //�ʵ粿Э��
			DealPostProtocol(uf,buffer,size);
			break;
		case PROTOCOL_TYPE_COMPANY:	 //��˾Э��							   	
			break;
		case PROTOCOL_TYPE_DOWNLOAD: //��������Э��
			DownLoad_DealProtocol(uf,buffer,size);
			break;
		default:break;

	}
	
  Watchdog_Feed();
	API_ResetProtocolBuffer(uf,RESETRTBUFF_TYPE_RX);
//API_ResetProtocolBuffer(uf,RESETRTBUFF_TYPE_ALL);
	return 0;
}

//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������

