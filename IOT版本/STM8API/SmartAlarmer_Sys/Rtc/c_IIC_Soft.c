#include "h_IIC_Soft.h"


/***************************************************************************************
**函数名:       fn_IIC_SOFT_Init
**功能描述:     --
**输入参数:     void
**输出参数:     --
**备注:c_IIC_Soft.c,2015/8/4,By Haixing.Huang
****************************************************************************************/
void fn_IIC_SOFT_Init(void)
{
  IIC_SOFT_SCL_OUT;
}

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
void IIC_SOFT_Delay5us(void)
{
//    IIC_SOFT_DELAY5US;
// DelayT_ms(1);
//  int i;
//  for(i=0;i<3000;i++);
//  {
//    Watchdog_Feed();
//  }
    
}
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
void IIC_SOFT_Delay5ms(void)
{
//    uint n = IIC_SOFT_DELAY5MS;
//    while (n--);
//  DelayT_ms(5);
  
//  DelayT_ms(50);
  
//  DelayT_ms(5);
}

/***************************************************************************************
**函数名:        IIC_SOFT_Start
**功能描述:      IIC起始信号
**输入参数:      --
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
void IIC_SOFT_Start()
{
    IIC_SOFT_SCL_OUT;
    IIC_SOFT_SDA_OUT;
    IIC_SOFT_SDA_1;                    //拉高数据线
//    _NOP();
    IIC_SOFT_SCL_1;                    //拉高时钟线
    IIC_SOFT_Delay5us();                 //延时
    IIC_SOFT_SDA_0;                    //产生下降沿
    IIC_SOFT_Delay5us();                 //延时
    IIC_SOFT_SCL_0;                    //拉低时钟线
}

/***************************************************************************************
**函数名:        IIC_SOFT_Stop
**功能描述:      IIC停止信号
**输入参数:      --
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
void IIC_SOFT_Stop(void)
{
    IIC_SOFT_SDA_OUT;
    IIC_SOFT_SDA_0;                    //拉低数据线
//    _NOP();
    IIC_SOFT_SCL_1;                    //拉高时钟线
    IIC_SOFT_Delay5us();                 //延时
    IIC_SOFT_SDA_1;                    //产生上升沿
    IIC_SOFT_Delay5us();                 //延时
}

/***************************************************************************************
**函数名:        IIC_SOFT_SendACK
**功能描述:      IIC发送应答信号
**输入参数:      ack (0:ACK 1:NAK)
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
void IIC_SOFT_SendACK(char ack)
{
   IIC_SOFT_SDA_OUT;
   if(ack)
    IIC_SOFT_SDA_1;
   else
    IIC_SOFT_SDA_0;
//   _NOP();
    IIC_SOFT_SCL_1;                    //拉高时钟线
    IIC_SOFT_Delay5us();                 //延时
    IIC_SOFT_SCL_0;                    //拉低时钟线
  //  IIC_SOFT_Delay5us();                 //延时
    IIC_SOFT_SDA_1;
}

/***************************************************************************************
**函数名:        IIC_SOFT_RecvACK
**功能描述:      IIC接收应答信号
**输入参数:
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
char IIC_SOFT_RecvACK(void)
{
    char ack;
    IIC_SOFT_SDA_IN;
    IIC_SOFT_SCL_0;                    //拉高时钟线
    IIC_SOFT_Delay5us();                 //延时
    IIC_SOFT_SCL_1;
    if(IIC_SOFT_SDA)
      ack=0;                   //读应答信号
    else
      ack=1;
    IIC_SOFT_SCL_0;                    //拉低时钟线
    IIC_SOFT_Delay5us();                 //延时
    IIC_SOFT_SDA_OUT;
    return ack ;
}

/***************************************************************************************
**函数名:        IIC_SOFT_SendByte
**功能描述:      向IIC总线发送一个字节数据
**输入参数:      char dat
**输出参数:      --
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
void IIC_SOFT_SendByte(char dat)
{
    char i;
    IIC_SOFT_SDA_OUT;
    for (i=0; i<8; i++)         //8位计数器
    {
        IIC_SOFT_Delay5us();             //延时
        if(dat&0x80)        //发送数据的最高位
        {
          IIC_SOFT_SDA_1;
        }
        else
        {
          IIC_SOFT_SDA_0;
        }//送数据口
//        _NOP();

        IIC_SOFT_SCL_1;                //拉高时钟线
        IIC_SOFT_Delay5us();             //延时
        IIC_SOFT_SCL_0;                //拉低时钟线
         dat<<=1;

    }
    IIC_SOFT_RecvACK();
}

/***************************************************************************************
**函数名:        IIC_SOFT_SendByte
**功能描述:      从IIC总线接收一个字节数据
**输入参数:      --
**输出参数:      char
**备注:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
char IIC_SOFT_RecvByte()
{

    char i;
    char dat = 0;
    IIC_SOFT_SDA_IN;
//    _NOP();
     for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        IIC_SOFT_SCL_1;                //拉高时钟线
        IIC_SOFT_Delay5us();             //延时
        if(IIC_SOFT_SDA)
          dat++;
        IIC_SOFT_SCL_0;                //拉低时钟线
        IIC_SOFT_Delay5us();             //延时
    }
    return dat;
}
///***************************************************************************************
//**函数名:       fn_IIC_SOFT_Write
//**功能描述:     --
//**输入参数:     uchar addr,uchar *sendbuf,uint num
//**输出参数:     --
//**备注:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
//****************************************************************************************/
//void fn_IIC_SOFT_Write(uchar addr,uchar *buf,uint num)
//{
//  IIC_SOFT_Start();   //开始IIC总线
//  IIC_SOFT_SendByte(addr);    //发送IIC地址
//  for(int i = 0; i < num; i++)    //发送数据
//  {
//    IIC_SOFT_SendByte(*buf);
//    buf++;
//  }
//  IIC_SOFT_Stop();    //结束IIC总线
//}
//
///***************************************************************************************
//**函数名:       fn_IIC_SOFT_Write
//**功能描述:     --
//**输入参数:     uchar addr,uchar *sendbuf,uint num
//**输出参数:     --
//**备注:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
//****************************************************************************************/
//void fn_IIC_SOFT_Read(uchar addr,uchar *buf,uint num)
//{
//  IIC_SOFT_Start();   //开始IIC总线
//  IIC_SOFT_SendByte(addr);    //发送IIC地址
//  for(int i = 0; i < num; i++)    //发送数据
//  {
//    *buf = IIC_SOFT_RecvByte();
//    IIC_SOFT_SendACK(0);    //应答
//    buf++;
//  }
//  IIC_SOFT_SendACK(1);    //非应答
//  IIC_SOFT_Stop();    //结束IIC总线
//}

/***************************************************************************************
**函数名:       fn_IIC_SOFT_Write
**功能描述:     --
**输入参数:     uchar addr,uchar *sendbuf,uint num
**输出参数:     --
**备注:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
****************************************************************************************/
void fn_IIC_SOFT_Write(uchar addr,uchar *buf,uint num)
{
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(0xa0);
  IIC_SOFT_SendByte(addr);    //发送IIC地址
  IIC_SOFT_SendByte(addr);    //发送IIC地址
  for(int i = 0; i < num; i++)    //发送数据
  {
    IIC_SOFT_SendByte(*buf);
    buf++;
  }
  IIC_SOFT_Stop();    //结束IIC总线
}

/***************************************************************************************
**函数名:       fn_IIC_SOFT_Write
**功能描述:     --
**输入参数:     uchar addr,uchar *sendbuf,uint num
**输出参数:     --
**备注:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
****************************************************************************************/
void fn_IIC_SOFT_Read(uchar addr,uchar *buf,uint num)
{
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(0xa0);
  IIC_SOFT_SendByte(addr);    //发送IIC地址
//  IIC_SOFT_SendByte(addr);    //发送IIC地址
  IIC_SOFT_Start();   //开始IIC总线
  IIC_SOFT_SendByte(0xa1);
  for(int i = 0; i < num; i++)    //发送数据
  {
    *buf = IIC_SOFT_RecvByte();
    IIC_SOFT_SendACK(0);    //应答
    buf++;
  }
  IIC_SOFT_SendACK(1);    //非应答
  IIC_SOFT_Stop();    //结束IIC总线
}

