/***************************************************************************************
**�ļ���:     h_IIC_Soft.h
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       --
**�޸ļ�¼:   �汾     �޸���    ʱ��
**
**��ע:       2014/12/4

****************************************************************************************/
#ifndef   __IIC_SOFT_H__
#define   __IIC_SOFT_H__
//#include "stm8l15x.h"
#include "config.h"
//#define uchar u8
//#define uint u16

//#include "MyType.h"
///************************************����ʵ������޸�**********************************/
//#include"msp430g2553.h"   //����MSP430 Launchpadƽ̨
//#include "MSP430F5310.h"
//#include "h_Typedef.h"

#define	  IIC_SOFT_SCL_1    (GPIO_SetBits(GPIOC,GPIO_Pin_1))      //IICʱ�����Ŷ��壬����ʵ������޸�
#define	  IIC_SOFT_SCL_0    (GPIO_ResetBits(GPIOC,GPIO_Pin_1))
#define   IIC_SOFT_SCL_OUT  (GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_Out_OD_HiZ_Slow))
//#define   IIC_SOFT_SCL_OUT  (GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_Out_PP_High_Slow))

#define   IIC_SOFT_SDA      (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))      //IIC�������Ŷ��壬����ʵ������޸�
#define   IIC_SOFT_SDA_1    (GPIO_SetBits(GPIOC,GPIO_Pin_0))
#define   IIC_SOFT_SDA_0    (GPIO_ResetBits(GPIOC,GPIO_Pin_0))
#define   IIC_SOFT_SDA_IN   (GPIO_Init(GPIOC,GPIO_Pin_0,GPIO_Mode_In_FL_No_IT))
#define   IIC_SOFT_SDA_OUT  (GPIO_Init(GPIOC,GPIO_Pin_0,GPIO_Mode_Out_OD_HiZ_Slow))
//#define   IIC_SOFT_SDA_OUT  (GPIO_Init(GPIOC,GPIO_Pin_0,GPIO_Mode_Out_PP_High_Slow))


//#define   IIC_SOFT_DELAY5US _NOP();_NOP();_NOP();_NOP();_NOP();   //����ʵ������޸�
//#define   IIC_SOFT_DELAY5MS 560                                   //����ʵ������޸�
//#define   IIC_SOFT_DELAY5US _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();    //����ʵ������޸�
//#define   IIC_SOFT_DELAY5MS 4480                                 //����ʵ������޸�
#define   IIC_SOFT_DELAY5US //_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();    //����ʵ������޸�
#define   IIC_SOFT_DELAY5MS 10000                                //����ʵ������޸�
/*************************************************************************************/

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef IIC_SOFT_GLOBALS
#define IIC_SOFT_EXT
#else
#define IIC_SOFT_EXT extern
#endif


/***************************************************************************************
**������:        IIC_SOFT_Delay5us
**��������:      IICʱ����ʱ
**�������:      --
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
��ʱ5΢��(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺�����ע��ʱ�ӹ���ʱ��Ҫ�޸�
������1T��MCUʱ,���������ʱ����
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_Delay5us(void);

/***************************************************************************************
**������:        IIC_SOFT_Delay5ms
**��������:      IICʱ����ʱ
**�������:      --
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_Delay5ms(void);

/***************************************************************************************
**������:        fn_IIC_SOFT_Init
**��������:      IIC ��ʼ��
**�������:      --
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
****************************************************************************************/
IIC_SOFT_EXT void fn_IIC_SOFT_Init(void);

/***************************************************************************************
**������:        IIC_SOFT_Start
**��������:      IIC��ʼ�ź�
**�������:      --
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_Start(void);

/***************************************************************************************
**������:        IIC_SOFT_Stop
**��������:      IICֹͣ�ź�
**�������:      --
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_Stop(void);

/***************************************************************************************
**������:        IIC_SOFT_SendACK
**��������:      IIC����Ӧ���ź�
**�������:      ack (0:ACK 1:NAK)
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_SendACK(char ack);

/***************************************************************************************
**������:        IIC_SOFT_RecvACK
**��������:      IIC����Ӧ���ź�
**�������:
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT char IIC_SOFT_RecvACK(void);

/***************************************************************************************
**������:        IIC_SOFT_SendByte
**��������:      ��IIC���߷���һ���ֽ�����
**�������:      char dat
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_SendByte(char dat);

/***************************************************************************************
**������:        IIC_SOFT_SendByte
**��������:      ��IIC���߽���һ���ֽ�����
**�������:      --
**�������:      char
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT char IIC_SOFT_RecvByte(void);

/***************************************************************************************
**������:       fn_IIC_SOFT_Write
**��������:     --
**�������:     uchar addr,uchar *sendbuf,uint num
**�������:     --
**��ע:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void fn_IIC_SOFT_Write(uchar addr,uchar *buf,uint num);

/***************************************************************************************
**������:       fn_IIC_SOFT_Write
**��������:     --
**�������:     uchar addr,uchar *sendbuf,uint num
**�������:     --
**��ע:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void fn_IIC_SOFT_Read(uchar addr,uchar *buf,uint num);
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/
