#ifndef PROTOCOL_SEND_H
#define  PROTOCOL_SEND_H
#include "config.h"
#include "stm8l15x.h"
#define CONST_REPEATEDLY_SEND_DATA (API_StartSendBuffer(USART_FUNCTION_WCDMA)) //重发一次数据。
typedef enum {
        SEND_INIT, //初始化状态。
	SEND_READY,  //准备发送状态
        SEND_ONE,      //发送一次
        SEND_REPEATEDLY,//重复发送状态。
        WAIT_ACK_ONE,   //发送一次等待状态，一次发送完成后，收到回复后立刻进入EXIT状态。
	WAIT_ACK,//等待应答状态,重复发送等待回复状态，会回到SEND_REPEATEDLY
        FREE, //空闲状态
        EXIT
}SEND_STATE;
typedef enum
{
  SEND_CONTINUE, //继续发送标志。
  SEND_STOP//停止发送。
}SEND_CONTROL; //发送标志。用于在发送完成后，后续的工作状态，如果想发送后一次停止需要在发送前切换切换到SEND_STOP

typedef enum {

	WAIT_REC,
	REC_OK
}REC_STATE;
typedef enum {
	UPLOAD,
        HEARTBEAT,
        UPDATA,
}LINK_TYPE;
typedef struct
{
	u8 TIMES;	
	u8 delays;
	u8 MAX_delays;
	SEND_STATE send_state;
        SEND_CONTROL send_control;
	REC_STATE REC_state;
}SEND_TYPE;
typedef struct
{
    SEND_TYPE upload_handshake;//升级握手
    SEND_TYPE SA_Heartbeat;//心跳
    SEND_TYPE Updata;  //历史数据上报
    LINK_TYPE Link_Type;
}PROTOCOL_SEND_TYPE;

extern PROTOCOL_SEND_TYPE Net_Data_Send;
void net_data_send(void);
void net_data_send_init(void);
void net_data_send_delay(void);
#endif 

