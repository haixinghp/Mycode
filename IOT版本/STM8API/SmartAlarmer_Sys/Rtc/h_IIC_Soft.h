/***************************************************************************************
**文件名:     h_IIC_Soft.h
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       --
**修改记录:   版本     修改者    时间
**
**备注:       2014/12/4

****************************************************************************************/
#ifndef   __IIC_SOFT_H__
#define   __IIC_SOFT_H__
//#include "stm8l15x.h"
#include "config.h"
//#define uchar u8
//#define uint u16

//#include "MyType.h"
///************************************根据实际情况修改**********************************/
//#include"msp430g2553.h"   //用于MSP430 Launchpad平台
//#include "MSP430F5310.h"
//#include "h_Typedef.h"

#define	  IIC_SOFT_SCL_1    (GPIO_SetBits(GPIOC,GPIO_Pin_1))      //IIC时钟引脚定义，根据实际情况修改
#define	  IIC_SOFT_SCL_0    (GPIO_ResetBits(GPIOC,GPIO_Pin_1))
#define   IIC_SOFT_SCL_OUT  (GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_Out_OD_HiZ_Slow))
//#define   IIC_SOFT_SCL_OUT  (GPIO_Init(GPIOC,GPIO_Pin_1,GPIO_Mode_Out_PP_High_Slow))

#define   IIC_SOFT_SDA      (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))      //IIC数据引脚定义，根据实际情况修改
#define   IIC_SOFT_SDA_1    (GPIO_SetBits(GPIOC,GPIO_Pin_0))
#define   IIC_SOFT_SDA_0    (GPIO_ResetBits(GPIOC,GPIO_Pin_0))
#define   IIC_SOFT_SDA_IN   (GPIO_Init(GPIOC,GPIO_Pin_0,GPIO_Mode_In_FL_No_IT))
#define   IIC_SOFT_SDA_OUT  (GPIO_Init(GPIOC,GPIO_Pin_0,GPIO_Mode_Out_OD_HiZ_Slow))
//#define   IIC_SOFT_SDA_OUT  (GPIO_Init(GPIOC,GPIO_Pin_0,GPIO_Mode_Out_PP_High_Slow))


//#define   IIC_SOFT_DELAY5US _NOP();_NOP();_NOP();_NOP();_NOP();   //根据实际情况修改
//#define   IIC_SOFT_DELAY5MS 560                                   //根据实际情况修改
//#define   IIC_SOFT_DELAY5US _NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();    //根据实际情况修改
//#define   IIC_SOFT_DELAY5MS 4480                                 //根据实际情况修改
#define   IIC_SOFT_DELAY5US //_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();_NOP();    //根据实际情况修改
#define   IIC_SOFT_DELAY5MS 10000                                //根据实际情况修改
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
**函数名:        IIC_SOFT_Delay5us
**功能描述:      IIC时序延时
**输入参数:      --
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
延时5微秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数，注意时钟过快时需要修改
当改用1T的MCU时,请调整此延时函数
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_Delay5us(void);

/***************************************************************************************
**函数名:        IIC_SOFT_Delay5ms
**功能描述:      IIC时序延时
**输入参数:      --
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
延时5毫秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_Delay5ms(void);

/***************************************************************************************
**函数名:        fn_IIC_SOFT_Init
**功能描述:      IIC 初始化
**输入参数:      --
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
延时5毫秒(STC90C52RC@12M)
不同的工作环境,需要调整此函数
当改用1T的MCU时,请调整此延时函数
****************************************************************************************/
IIC_SOFT_EXT void fn_IIC_SOFT_Init(void);

/***************************************************************************************
**函数名:        IIC_SOFT_Start
**功能描述:      IIC起始信号
**输入参数:      --
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_Start(void);

/***************************************************************************************
**函数名:        IIC_SOFT_Stop
**功能描述:      IIC停止信号
**输入参数:      --
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_Stop(void);

/***************************************************************************************
**函数名:        IIC_SOFT_SendACK
**功能描述:      IIC发送应答信号
**输入参数:      ack (0:ACK 1:NAK)
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_SendACK(char ack);

/***************************************************************************************
**函数名:        IIC_SOFT_RecvACK
**功能描述:      IIC接收应答信号
**输入参数:
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT char IIC_SOFT_RecvACK(void);

/***************************************************************************************
**函数名:        IIC_SOFT_SendByte
**功能描述:      向IIC总线发送一个字节数据
**输入参数:      char dat
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void IIC_SOFT_SendByte(char dat);

/***************************************************************************************
**函数名:        IIC_SOFT_SendByte
**功能描述:      从IIC总线接收一个字节数据
**输入参数:      --
**输出参数:      char
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT char IIC_SOFT_RecvByte(void);

/***************************************************************************************
**函数名:       fn_IIC_SOFT_Write
**功能描述:     --
**输入参数:     uchar addr,uchar *sendbuf,uint num
**输出参数:     --
**备注:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void fn_IIC_SOFT_Write(uchar addr,uchar *buf,uint num);

/***************************************************************************************
**函数名:       fn_IIC_SOFT_Write
**功能描述:     --
**输入参数:     uchar addr,uchar *sendbuf,uint num
**输出参数:     --
**备注:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
****************************************************************************************/
IIC_SOFT_EXT void fn_IIC_SOFT_Read(uchar addr,uchar *buf,uint num);
#ifdef  __cplusplus
}
#endif
#endif
/************************************Code End******************************************/
