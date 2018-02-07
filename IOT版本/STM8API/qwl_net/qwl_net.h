#ifndef _qwl_net_H
#define _qwl_net_H
#include "stm8l15x.h"
#include "config.h"
typedef enum {

	LINK_OK,			//连接OK
	LINK_IP,			//连接IP
	OPEN_GPRS,		//打开网络
	MODE_INIT,	//模块初始化
	MODE_CHOICE,//模块选择
	SETP_TYPE_MAX 		
}SETP_TYPE;

typedef  struct
{
			
			u8 delay_times; //超时次数
			u8 delay;			//步骤中的超时机制
			u8 setp_setp;	//每个重连步骤中的步骤
			SETP_TYPE reconnect_setp;//重连步骤
			MODE_TYPE mode_type;//模块类型
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
extern u8 SystemBuf[];  //储存出口接收数据

const extern	u8 ASCII_hex[24];
extern u8 net_error_flag;//网络错误标志
extern u8 deal_process;
#endif 
