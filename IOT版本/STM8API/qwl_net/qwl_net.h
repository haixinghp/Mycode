#ifndef _qwl_net_H
#define _qwl_net_H
#include "stm8l15x.h"
#include "config.h"
typedef enum {

	LINK_OK,			//����OK
	LINK_IP,			//����IP
	OPEN_GPRS,		//������
	MODE_INIT,	//ģ���ʼ��
	MODE_CHOICE,//ģ��ѡ��
	SETP_TYPE_MAX 		
}SETP_TYPE;

typedef  struct
{
			
			u8 delay_times; //��ʱ����
			u8 delay;			//�����еĳ�ʱ����
			u8 setp_setp;	//ÿ�����������еĲ���
			SETP_TYPE reconnect_setp;//��������
			MODE_TYPE mode_type;//ģ������
}CONNECT_TYPE;

extern CONNECT_TYPE net;

u8 net_state(void);
void net_rec(u8 k);
u8 net_send(u8 *p,u16 len);
void qwl_net_init(void);
void cdma_or_nblot(void);
void qwl_net_sendstring1(u8 *p);
void qwl_net_sendstring(u8 *p);
void qwl_net_sendchar(u8 ch);


void USART3_sendchar(u8 ch);
void USART3_sendstring(u8 *p,u8 len);

extern u8 cdma_or_nblot_flag;
extern u8 connect_flag;

extern u16 Rx;
#define  RxIn 200
extern u8 SystemBuf[];  //������ڽ�������

const extern	u8 ASCII_hex[24];
extern u8 net_error_flag;//��������־
extern u8 deal_process;
#endif 
