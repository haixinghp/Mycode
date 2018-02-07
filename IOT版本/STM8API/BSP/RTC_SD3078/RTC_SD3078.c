#include "RTC_SD3078.h"

_struRTC_SD3078_Time struRTC_SD3078_Time = {0x55,0x59,0x14,0x01,0x12,0x11,0x14};	//初始化时间结构体变量（设置时间：2014年11月12日 14:59:55  星期一）

/***************************************************************************************
**函数名:         --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_Init(void)
{
    fn_IIC_SOFT_Init();
}

/***************************************************************************************
**函数名:         --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_SetPar(u8 addr,u8 cmd,u8 data)
{
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(addr);
  IIC_SOFT_SendByte(cmd);
  IIC_SOFT_SendByte(data);
  IIC_SOFT_Stop();
}
/***************************************************************************************
**函数名:         --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_WriteON()
{
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(RTC_SD3078_CTR2);
  IIC_SOFT_SendByte(0x80);
  IIC_SOFT_Stop();
  
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(0x0f);
  IIC_SOFT_SendByte(0x84);
  IIC_SOFT_Stop();
}

/***************************************************************************************
**函数名:         --
**功能描述:     --
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_WriteOFF()
{
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(0x10);
  IIC_SOFT_SendByte(0x00);
  IIC_SOFT_Stop();
  
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(0x0f);
  IIC_SOFT_SendByte(0x00);
  IIC_SOFT_Stop();
}
/***************************************************************************************
**函数名:       RTC_SD3078_ReadTime
**功能描述:     读SD30XX实时数据寄存器
00H 秒
01H 分钟
02H 小时
03H 星期
04H 日
05H 月 
06H 年
**输入参数:     u8 *pPar
**输出参数:     
**备注:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_ReadTime(u8 *pPar)
{
  int i;
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(RTC_SD3078_READ_ADDR);
  for(i=0;i<7;i++)
  {
    *pPar = IIC_SOFT_RecvByte();
    pPar++;
    if(6==i)
    {
      IIC_SOFT_SendACK(1);
    }
    IIC_SOFT_SendACK(0);
  }
  IIC_SOFT_Stop();
}
/***************************************************************************************
**函数名:        RTC_SD3078_SetTime
**功能描述:     
写时间操作要求一次对实时时间寄存器(00H~06H)依次写入
不可以单独对7个时间数据中的某一位进行写操作,否则可能会引起时间数据的错误进位
要修改其中某一个数据 , 应一次性写入全部 7 个实时时钟数据
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_SetTime(u8 *pPar)
{
  int i;
  RTC_SD3078_WriteON(); //关闭写保护
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(0x00);
  for(i=0;i<7;i++)
  {
    IIC_SOFT_SendByte(*pPar);
    pPar++;
  }
  IIC_SOFT_Stop();
  RTC_SD3078_WriteOFF();//打开写保护
}
/***************************************************************************************
**函数名:        RTC_SD3078_CountDownAlarm
**功能描述:     
freq 30 选择定时器频率源（TDS1、TDS0）为1/60HZ
time 5 倒计时初值寄存器，设置8位倒计时计数初值（5min）
**输入参数:     u8 freq,u8 time
**输出参数:     --
**备注:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_CountDownAlarm(u8 freq,u8 time)
{
  RTC_SD3078_WriteON(); //关闭写保护
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_CTR2,0x0f);//先清倒计时中断总允许位（INTDE）
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_CTR2,0xf4);//设置周期性中断（IM=1）INT中断选通（INTS1，INTS0），配置倒计时中断总允许位（INTDE）
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_CTR3,freq);//选择定时器频率源（TDS1、TDS0）为1/60HZ
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_COUNTDOWN1,time);//倒计时初值寄存器，设置8位倒计时计数初值（5min）
  RTC_SD3078_WriteOFF();//打开写保护
}
/***************************************************************************************
**函数名:        RTC_SD3078_ClearCountDownAlarm
**功能描述:     
清除倒计时告警
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_ClearCountDownAlarm(void)
{
  RTC_SD3078_WriteON(); //关闭写保护
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_CTR2,0xf0);
  RTC_SD3078_WriteOFF();//打开写保护
}

/***************************************************************************************
**函数名:        RTC_SD3078_WriteData
**功能描述:     
SD3078在addr写入len长度的数据
**输入参数:     u8 addr,u8 *pPar,u8 len
**输出参数:     --
**备注:         YY.c,2017-11-16,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_WriteData(u8 addr,u8 *pPar,u8 len)
{
    int i;
  RTC_SD3078_WriteON(); //关闭写保护
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(addr);
  for(i=0;i<len;i++)
  {
    IIC_SOFT_SendByte(*pPar);
    pPar++;
  }
  IIC_SOFT_Stop();
  RTC_SD3078_WriteOFF();//打开写保护
}
/***************************************************************************************
**函数名:        RTC_SD3078_ReadData
**功能描述:     
SD3078在addr读取len长度的数据
**输入参数:     u8 addr,u8 *pPar,u8 len
**输出参数:     --
**备注:         YY.c,2017-11-16,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_ReadData(u8 addr,u8 *pPar,u8 len)
{
  int i;
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(addr);
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(RTC_SD3078_READ_ADDR);
  for(i=0;i<len;i++)
  {
    *pPar = IIC_SOFT_RecvByte();
    pPar++;
    if((len-1) ==i)
    {
      IIC_SOFT_SendACK(1);
    }
    IIC_SOFT_SendACK(0);
  }
  IIC_SOFT_Stop();
}

u8 const SD3078_CheckWriteTab[8]={0xAA,0XBB,0XCC,0XDD,0XEE,0X11,0X22,0X33};
//u8  SD3078_CheckWriteTab[8]={0xAA,0XBB,0XCC,0XDD,0XEE,0X11,0X22,0X33};
///***************************************************************************************
//**函数名:        RTC_SD3078_WriteInitFlag
//**功能描述:     
//清除倒计时告警
//**输入参数:     --
//**输出参数:     --
//**备注:         YY.c,2017-09-02,By Haixing.Huang
//****************************************************************************************/
//void RTC_SD3078_WriteInitFlag()
//{
//  RTC_SD3078_WriteData(RTC_SD3078_USER_RAM,SD3078_CheckWriteTab,8)
//}
/***************************************************************************************
**函数名:        RTC_SD3078_CheckInitFlag
**功能描述:     
清除倒计时告警
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
s8 RTC_SD3078_CheckInitFlag()
{
  u8 tmp_tab[8];
  u16 i;
//  RTC_SD3078_WriteData(RTC_SD3078_USER_RAM,(u8 *)SD3078_CheckWriteTab,8);
  RTC_SD3078_ReadData(RTC_SD3078_USER_RAM,tmp_tab,8);
  for(i=0;i<8;i++)
  {
    if(SD3078_CheckWriteTab[i] != tmp_tab[i])
    {
      break;
    }
  }
  if(8 != i) //校验未通过
  {
//    RTC_SD3078_WriteData(RTC_SD3078_USER_RAM,(u8 *)SD3078_CheckWriteTab,8);
    return 1;
  }
  else //校验通过
  {
    return 0;
  }
}
/***************************************************************************************
**函数名:        RTC_SD3078_WriteInitFlag
**功能描述:     
清除倒计时告警
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
s8 RTC_SD3078_WriteInitFlag()
{
  RTC_SD3078_WriteData(RTC_SD3078_USER_RAM,(u8 *)SD3078_CheckWriteTab,8);
  return RTC_SD3078_CheckInitFlag();
}
