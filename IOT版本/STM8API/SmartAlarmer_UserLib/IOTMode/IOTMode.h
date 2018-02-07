#ifndef __IOTMODE_H
#define __IOTMODE_H

#include "stm8l15x.h"
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////
#define POWERON_MC              GPIO_ResetBits(GPIOG,GPIO_Pin_3);GPIO_SetBits(GPIOG,GPIO_Pin_2)
#define POWEROFF_MC             GPIO_SetBits(GPIOG,GPIO_Pin_3);GPIO_ResetBits(GPIOG,GPIO_Pin_2)



#define RESET_MC_POWER_ONOFF      GPIO_SetBits(GPIOD,GPIO_Pin_6)
#define SET_MC_POWER_ONOFF    GPIO_ResetBits(GPIOD,GPIO_Pin_6)

#define RESET_MC_RESET            GPIO_SetBits(GPIOD,GPIO_Pin_7)
#define SET_MC_RESET          GPIO_ResetBits (GPIOD,GPIO_Pin_7)


#define MAX_RECONNECT_COUNT 3


typedef struct
{
        unsigned char Mode_Restart : 4; //ģ������׼����־
        unsigned char reconnect_count : 4; //�ظ����Ӵ���
        unsigned char send_count : 8;  //���ʹ���
        
	unsigned char initStep : 5;
        unsigned char ack_config : 1;//�ظ��������˱�־
        unsigned char send_success_flag : 1;//��Ϣ���ͳɹ���־
	unsigned char cardType : 1; //�ֻ���Ϊ1��������Ϊ0
} IOTMode_INFO;


typedef enum
{
  ACK_CONFIG_FAILED       = 0, /*!< Channel 00 */
  ADC_Channel_1           = 1, /*!< Channel 01 */
  ADC_Channel_2           = 2, /*!< Channel 02 */
  ADC_Channel_3           = 3, /*!< Channel 03 */
  ADC_Channel_4           = 4, /*!< Channel 04 */
}ACK_CONFIG_TypeDef;

typedef enum
{
  RTN_OK              = 0x00, /*!< ����*/
  RTN_ERR_VER         = 0x01, /*!< VER���� */
  RTN_ERR_CHKSUM      = 0x02, /*!< CHKSUM���� */
  RTN_ERR_LCHKSUM     = 0x03, /*!< LCHKSUM���� */
  RTN_ERR_CID2        = 0x04, /*!< CID2��Ч */
  RTN_ERR_FORMAT      = 0x05, /*!< ���ݸ�ʽ���� */
  RTN_ERR_INVALID     = 0x06, /*!< ��Ч���� */
  RTN_NULL            = 0x07, /*!< ���ظ� */
}RTN_TypeDef;

extern IOTMode_INFO iotmodeInfo;


//extern u16 bat_sendcount;

void IOTModeRestart(void);

void IOTModePoweroff(void);

void IOTModeRset(void);

_Bool GetIotNum(void);
_Bool GetNBIotNum(void);
_Bool GetSigStenValue(void);

_Bool GetNBSigStenValue(void);
_Bool Report_Data(void);
_Bool NB95_Report(void);
_Bool MC8618_Report(void);
_Bool IOTMode_SendCmd(char *, char *, u16);

void IOTMode_SendCmdWithoutResult(char *);

char *IOTMode_GetStatus(void);

_Bool IOTMode_SendCmd_with_length_nb_iot(char *cmd, char *res, u8 len);

_Bool IOTMode_SendCmd_with_length(char *, char *, u16);

_Bool WaitForReturnCmd(char *res, u16 delaytime);


u8* NBiotToPosts(char *parPin,u8 *parPout);
//������Ϣ  ��ȡ����
RTN_TypeDef GetStrFromStr(char *source_str);
RTN_TypeDef GetRebackMsg(char *rck_msg);


#endif