#ifndef _PUBLIC_H_
#define _PUBLIC_H_

typedef enum {
  MODE_ING_SA_SYS,				//运行井盖程序
  MODE_ING_DOWNLOAD,					//运行远程下载程序
  MODE_ING_MAX 		
}MODE_ING;

typedef enum {
  PRO_INIT,				//初始化
  PRO_RUN,				//运行
  PRO_END,                              //结束
  PRO_MAX 		
}PRO;


extern u32 channel_en_bit;//平台下发参数使能位
extern u8 SA_Pro;  //井盖进程
extern u8 Down_Pro;  //发送下载进程

//获取信号强度
void Get_SigStren(u8 TempVal);

//获取卡号
void Get_SimID(u8 *TempPointer);

//发送12
void Uploading_Send_12(USART_FUNCTION uf,uchar *DaPtr,uint16 len);

void pack_heat(void);

extern s8 pack_updata(void); //把历史数据内容注入缓存

void Get_DowndataFlag(uchar *DaPtr);

uint16 Getparameters_Str(uchar *DaPtr,uchar *Par_Str,uint16 with);

uint16 Getparameters_StrEN(uchar *DaPtr,uchar *Par_Str,uint16 with,u32 Enable);

void Getparameters(USART_FUNCTION uf,uchar *DaPtr,uint16 len);

void heartsend_Onehour(void);

void IOTMode_RESET(void);
#endif
