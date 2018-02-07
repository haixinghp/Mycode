/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**						
**��ע:       2016/3/23
****************************************************************************************/
#ifndef   __PRESSURE_H__
#define   __PRESSURE_H__

//#include "stm8l15x.h"
#include "config.h"

// ����KEY���Ŷ��壬LED(PB0), KEY(PB1)
#define LED_PORT        GPIOE
#define LED_PIN         GPIO_Pin_0

// 24V���Ŷ��壬LED(PB0), KEY(PB1)
//#define PORT_24V        GPIOD
//#define PIN_24V         GPIO_Pin_4
#define PORT_24V        GPIOG
#define PIN_24V         GPIO_Pin_2

// ���Ŷ��壬LED(PB0), KEY(PB1)
//#define PORT_POW        GPIOG
//#define PIN_POW         GPIO_Pin_3
//#define PORT_POW1        GPIOE
//#define PIN_POW1         GPIO_Pin_7

// LED����������(ON)��, (OFF)�رգ�(TOG)��ת
#define LED1_OFF()        GPIO_ResetBits(LED_PORT, LED_PIN)        
#define LED1_ON()         GPIO_SetBits(LED_PORT, LED_PIN)
#define LED1_TOG()        GPIO_ToggleBits(LED_PORT, LED_PIN)
        
#define POW24_ON()         GPIO_ResetBits(PORT_24V, PIN_24V)
#define POW24_OFF()        GPIO_SetBits(PORT_24V, PIN_24V)

//#define POWSW_ON()         GPIO_ResetBits(PORT_POW, PIN_POW)
//#define POWSW_OFF()        GPIO_SetBits(PORT_POW, PIN_POW)

#define POWSW1_ON()         GPIO_ResetBits(PORT_POW1, PIN_POW1)
#define POWSW1_OFF()        GPIO_SetBits(PORT_POW1, PIN_POW1)


#define CONST_PRESSUREDATA_MAX_LEN (10)  //ѹ�����ݵ���ȣ� by hx 2017-06-22
//#define CONST_PRESSURE_SENDBUFF_LEN   
#define RESET_UPDATA_ENABLE (sysResetUpdata=TRUE)
#define RESET_UPDATA_DISABLE (sysResetUpdata=FALSE)
#define RESET_UPDATA_GET() (sysResetUpdata)




//extern _struPressureOriginal struPressureAData;
typedef struct 
{
  u32 TimesTamp;  //ʱ��� by hx 2017-06-22
  u16 PressureData; //ѹ������ by hx 2017-06-22
  u16 AlarmList; //�澯��
}_struPressureOriginal;

typedef struct 
{
  u8 len;
  _struPressureOriginal struDataOriginal[CONST_PRESSUREDATA_MAX_LEN];
}_struPressureDataOriginal;
//extern _struPressureDataOriginal struPressureDataOriginal;

typedef struct 
{
  HEX2STR_4 TimesTamp;  //ʱ��� by hx 2017-06-22
  HEX2STR_2 PressureData; //ѹ������ by hx 2017-06-22
}_struPressureData;
//extern _struPressureData struPressureData;
typedef struct 
{  
  HEX2STR_16 Id_Cmd; //�豸ID
  HEX2STR_1 DataLen;   //���ݳ��� by hx 2017-06-22
  _struPressureData struPressureData[CONST_PRESSUREDATA_MAX_LEN]; //��ʷѹ�����ݡ�
}_struUploadData; 

typedef struct 
{
    u8 Soi_Cmd;//1���ֽ�
  HEX2STR_1 Ver_Cmd;//1*2�ֽ�
  HEX2STR_1 Adr_Cmd;//1*2�ֽ�
  HEX2STR_1 Cid1_Cmd;//1*2�ֽ�
  HEX2STR_1 Cid2_Cmd;//1*2�ֽ�
  HEX2STR_2 Len_Cmd;//2*2�ֽ�
  _struUploadData Data_Info;//93*2�ֽ�
  HEX2STR_2 Chksum_Cmd;//2*2�ֽ�
  u8 Eoi_Cmd;//1���ֽ�
}_struPressureUploadData;
//extern _struPressureUploadData struPressureUploadData;

typedef struct 
{
  char cmd_start[17];
  _struPressureUploadData TcpData_S;
  char cmd_end[2];
}_struSendBuf;


union _uniSendBuf
{
  char senddata[(sizeof(_struSendBuf))]; //by hx 2017-06-22
  _struSendBuf struSendBuf;
};
//extern union _uniSendBuf uniSendBuf;

typedef struct
{
  u8 readFlag:1;
  u8 enableFlag:1;
  u8 alarmFlag:1;
  u8 alarmFlagbuckup:1;
  u8 save:1;
  u8 startupdata:1; //��ʼ�ϴ����ݱ�־��
  u8 endupdata:1; //�����ϴ����ݱ�־��
  u8 refreshupdata:1;//ˢ�����ݱ�־��
  u8 SendRepeatedlyFlag:1;  //��־��
  u8 failCnt; //����ʧ�ܴ�����
  u8 updataStatus;      //�ϴ����ݵ�״̬ת����
  u16 CurrentValue;     //��ǰ����
  u16 CurrentValuePre; //�ϴεĵ�����
  u16 CurrentValueDifference; //�м�Ĳ�ֵ��
}_strPressureControl;

typedef struct
{
  bool haveKey;
  bool confirmKey;
}_strPressureKeyUpdata;

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef PRESSURE_GLOBALS
#define PRESSURE_EXT
#else
#define PRESSURE_EXT extern
#endif
/*****************************ȫ�ֱ�����************************************/
PRESSURE_EXT _struPressureOriginal struPressureAData;
PRESSURE_EXT _struPressureDataOriginal struPressureDataOriginal;
PRESSURE_EXT _strPressureControl strPressureControl;
PRESSURE_EXT union _uniSendBuf uniSendBuf;
//PRESSURE_EXT _uniAlarm uniAlarm;
PRESSURE_EXT bool sysResetUpdata; //TRUE ���ͣ�FALSE��ֹ����
PRESSURE_EXT _strPressureKeyUpdata strPressureKeyUpdata;
/*****************************ȫ�ֺ�����************************************/
PRESSURE_EXT void Pressure_Start(_strPressureControl *parP);
PRESSURE_EXT void Pressure_GatValue(_strPressureControl *parP);
PRESSURE_EXT u8 Pressure_Updata(void);
PRESSURE_EXT u8 Pressure_CheckBuff(_struPressureDataOriginal *parP);
PRESSURE_EXT u8 Pressure_AnalyseState(SYS_KEYDATA *parP,_struPressureOriginal *parPa);
PRESSURE_EXT u8 Pressure_SendBufClear(_struPressureDataOriginal *parP);
PRESSURE_EXT u8 Pressure_RamBuf(_struPressureDataOriginal *parP,u16 parNum);
PRESSURE_EXT void PressureUploadData_Init(union _uniSendBuf *parP);
PRESSURE_EXT u8 Pressure_Send(_struPressureDataOriginal *parP);
PRESSURE_EXT u8 PressureUpDataDeal();
PRESSURE_EXT void PressureKeyUpdataIntterSetFlag();
PRESSURE_EXT s8 PressureKeyUpdataConfirm();

PRESSURE_EXT u32 *getTimesTamp();
PRESSURE_EXT u16 *getPressureData();
PRESSURE_EXT u16 *getAlarmList();
PRESSURE_EXT u16 GetADCValue();
PRESSURE_EXT u16 GetPressureValue(u16 value);
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/