#ifndef _PUBLIC_H_
#define _PUBLIC_H_

typedef enum {
  MODE_ING_SA_SYS,				//���о��ǳ���
  MODE_ING_DOWNLOAD,					//����Զ�����س���
  MODE_ING_MAX 		
}MODE_ING;

typedef enum {
  PRO_INIT,				//��ʼ��
  PRO_RUN,				//����
  PRO_END,                              //����
  PRO_MAX 		
}PRO;


extern u32 channel_en_bit;//ƽ̨�·�����ʹ��λ
extern u8 SA_Pro;  //���ǽ���
extern u8 Down_Pro;  //�������ؽ���

//��ȡ�ź�ǿ��
void Get_SigStren(u8 TempVal);

//��ȡ����
void Get_SimID(u8 *TempPointer);

//����12
void Uploading_Send_12(USART_FUNCTION uf,uchar *DaPtr,uint16 len);

void pack_heat(void);

extern s8 pack_updata(void); //����ʷ��������ע�뻺��

void Get_DowndataFlag(uchar *DaPtr);

uint16 Getparameters_Str(uchar *DaPtr,uchar *Par_Str,uint16 with);

uint16 Getparameters_StrEN(uchar *DaPtr,uchar *Par_Str,uint16 with,u32 Enable);

void Getparameters(USART_FUNCTION uf,uchar *DaPtr,uint16 len);

void heartsend_Onehour(void);

void IOTMode_RESET(void);
#endif
