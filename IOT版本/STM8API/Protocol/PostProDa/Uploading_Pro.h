#ifndef	 _UPLOADING_PRO_H
#define  _UPLOADING_PRO_H

typedef enum {

	RUN,
	STOP
}RECOUT_STATE;

typedef struct
{
	RECOUT_STATE Rec_State;	
	u8 delays;
	u8 MAX_delays;
}REC_TYPE;

typedef struct
{
    REC_TYPE Post_REC_TIMEOUT;//�ʵ粿���ճ�ʱ
    REC_TYPE Download_REC_TIMEOUT;//�ʵ粿���ճ�ʱ
    REC_TYPE NET_STATE_TIMEOUT;//�������ӳ�ʱ
}PROTOCOL_REC_TYPE;

extern uchar platform_soft_version[2];
extern PROTOCOL_REC_TYPE Rec_Timeout;
void post_data_rec_delay(void);
void Uploading_Pro(USART_FUNCTION uf,uchar *DaPtr,u16 len);
/*********************************************************************************************/
#endif

