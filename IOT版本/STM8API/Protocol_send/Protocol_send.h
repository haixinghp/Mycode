#ifndef PROTOCOL_SEND_H
#define  PROTOCOL_SEND_H
#include "config.h"
#include "stm8l15x.h"
#define CONST_REPEATEDLY_SEND_DATA (API_StartSendBuffer(USART_FUNCTION_WCDMA)) //�ط�һ�����ݡ�
typedef enum {
        SEND_INIT, //��ʼ��״̬��
	SEND_READY,  //׼������״̬
        SEND_ONE,      //����һ��
        SEND_REPEATEDLY,//�ظ�����״̬��
        WAIT_ACK_ONE,   //����һ�εȴ�״̬��һ�η�����ɺ��յ��ظ������̽���EXIT״̬��
	WAIT_ACK,//�ȴ�Ӧ��״̬,�ظ����͵ȴ��ظ�״̬����ص�SEND_REPEATEDLY
        FREE, //����״̬
        EXIT
}SEND_STATE;
typedef enum
{
  SEND_CONTINUE, //�������ͱ�־��
  SEND_STOP//ֹͣ���͡�
}SEND_CONTROL; //���ͱ�־�������ڷ�����ɺ󣬺����Ĺ���״̬������뷢�ͺ�һ��ֹͣ��Ҫ�ڷ���ǰ�л��л���SEND_STOP

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
    SEND_TYPE upload_handshake;//��������
    SEND_TYPE SA_Heartbeat;//����
    SEND_TYPE Updata;  //��ʷ�����ϱ�
    LINK_TYPE Link_Type;
}PROTOCOL_SEND_TYPE;

extern PROTOCOL_SEND_TYPE Net_Data_Send;
void net_data_send(void);
void net_data_send_init(void);
void net_data_send_delay(void);
#endif 

