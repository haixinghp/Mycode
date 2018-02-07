#include "h_IIC_Soft.h"


/***************************************************************************************
**������:       fn_IIC_SOFT_Init
**��������:     --
**�������:     void
**�������:     --
**��ע:c_IIC_Soft.c,2015/8/4,By Haixing.Huang
****************************************************************************************/
void fn_IIC_SOFT_Init(void)
{
  IIC_SOFT_SCL_OUT;
}

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
**������:        IIC_SOFT_Delay5ms
**��������:      IICʱ����ʱ
**�������:      --
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
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
**������:        IIC_SOFT_Start
**��������:      IIC��ʼ�ź�
**�������:      --
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
void IIC_SOFT_Start()
{
    IIC_SOFT_SCL_OUT;
    IIC_SOFT_SDA_OUT;
    IIC_SOFT_SDA_1;                    //����������
//    _NOP();
    IIC_SOFT_SCL_1;                    //����ʱ����
    IIC_SOFT_Delay5us();                 //��ʱ
    IIC_SOFT_SDA_0;                    //�����½���
    IIC_SOFT_Delay5us();                 //��ʱ
    IIC_SOFT_SCL_0;                    //����ʱ����
}

/***************************************************************************************
**������:        IIC_SOFT_Stop
**��������:      IICֹͣ�ź�
**�������:      --
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
void IIC_SOFT_Stop(void)
{
    IIC_SOFT_SDA_OUT;
    IIC_SOFT_SDA_0;                    //����������
//    _NOP();
    IIC_SOFT_SCL_1;                    //����ʱ����
    IIC_SOFT_Delay5us();                 //��ʱ
    IIC_SOFT_SDA_1;                    //����������
    IIC_SOFT_Delay5us();                 //��ʱ
}

/***************************************************************************************
**������:        IIC_SOFT_SendACK
**��������:      IIC����Ӧ���ź�
**�������:      ack (0:ACK 1:NAK)
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
void IIC_SOFT_SendACK(char ack)
{
   IIC_SOFT_SDA_OUT;
   if(ack)
    IIC_SOFT_SDA_1;
   else
    IIC_SOFT_SDA_0;
//   _NOP();
    IIC_SOFT_SCL_1;                    //����ʱ����
    IIC_SOFT_Delay5us();                 //��ʱ
    IIC_SOFT_SCL_0;                    //����ʱ����
  //  IIC_SOFT_Delay5us();                 //��ʱ
    IIC_SOFT_SDA_1;
}

/***************************************************************************************
**������:        IIC_SOFT_RecvACK
**��������:      IIC����Ӧ���ź�
**�������:
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
char IIC_SOFT_RecvACK(void)
{
    char ack;
    IIC_SOFT_SDA_IN;
    IIC_SOFT_SCL_0;                    //����ʱ����
    IIC_SOFT_Delay5us();                 //��ʱ
    IIC_SOFT_SCL_1;
    if(IIC_SOFT_SDA)
      ack=0;                   //��Ӧ���ź�
    else
      ack=1;
    IIC_SOFT_SCL_0;                    //����ʱ����
    IIC_SOFT_Delay5us();                 //��ʱ
    IIC_SOFT_SDA_OUT;
    return ack ;
}

/***************************************************************************************
**������:        IIC_SOFT_SendByte
**��������:      ��IIC���߷���һ���ֽ�����
**�������:      char dat
**�������:      --
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
void IIC_SOFT_SendByte(char dat)
{
    char i;
    IIC_SOFT_SDA_OUT;
    for (i=0; i<8; i++)         //8λ������
    {
        IIC_SOFT_Delay5us();             //��ʱ
        if(dat&0x80)        //�������ݵ����λ
        {
          IIC_SOFT_SDA_1;
        }
        else
        {
          IIC_SOFT_SDA_0;
        }//�����ݿ�
//        _NOP();

        IIC_SOFT_SCL_1;                //����ʱ����
        IIC_SOFT_Delay5us();             //��ʱ
        IIC_SOFT_SCL_0;                //����ʱ����
         dat<<=1;

    }
    IIC_SOFT_RecvACK();
}

/***************************************************************************************
**������:        IIC_SOFT_SendByte
**��������:      ��IIC���߽���һ���ֽ�����
**�������:      --
**�������:      char
**��ע:c_IIC_Soft.c,2014/12/4,By Haixing.Huang
****************************************************************************************/
char IIC_SOFT_RecvByte()
{

    char i;
    char dat = 0;
    IIC_SOFT_SDA_IN;
//    _NOP();
     for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        IIC_SOFT_SCL_1;                //����ʱ����
        IIC_SOFT_Delay5us();             //��ʱ
        if(IIC_SOFT_SDA)
          dat++;
        IIC_SOFT_SCL_0;                //����ʱ����
        IIC_SOFT_Delay5us();             //��ʱ
    }
    return dat;
}
///***************************************************************************************
//**������:       fn_IIC_SOFT_Write
//**��������:     --
//**�������:     uchar addr,uchar *sendbuf,uint num
//**�������:     --
//**��ע:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
//****************************************************************************************/
//void fn_IIC_SOFT_Write(uchar addr,uchar *buf,uint num)
//{
//  IIC_SOFT_Start();   //��ʼIIC����
//  IIC_SOFT_SendByte(addr);    //����IIC��ַ
//  for(int i = 0; i < num; i++)    //��������
//  {
//    IIC_SOFT_SendByte(*buf);
//    buf++;
//  }
//  IIC_SOFT_Stop();    //����IIC����
//}
//
///***************************************************************************************
//**������:       fn_IIC_SOFT_Write
//**��������:     --
//**�������:     uchar addr,uchar *sendbuf,uint num
//**�������:     --
//**��ע:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
//****************************************************************************************/
//void fn_IIC_SOFT_Read(uchar addr,uchar *buf,uint num)
//{
//  IIC_SOFT_Start();   //��ʼIIC����
//  IIC_SOFT_SendByte(addr);    //����IIC��ַ
//  for(int i = 0; i < num; i++)    //��������
//  {
//    *buf = IIC_SOFT_RecvByte();
//    IIC_SOFT_SendACK(0);    //Ӧ��
//    buf++;
//  }
//  IIC_SOFT_SendACK(1);    //��Ӧ��
//  IIC_SOFT_Stop();    //����IIC����
//}

/***************************************************************************************
**������:       fn_IIC_SOFT_Write
**��������:     --
**�������:     uchar addr,uchar *sendbuf,uint num
**�������:     --
**��ע:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
****************************************************************************************/
void fn_IIC_SOFT_Write(uchar addr,uchar *buf,uint num)
{
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(0xa0);
  IIC_SOFT_SendByte(addr);    //����IIC��ַ
  IIC_SOFT_SendByte(addr);    //����IIC��ַ
  for(int i = 0; i < num; i++)    //��������
  {
    IIC_SOFT_SendByte(*buf);
    buf++;
  }
  IIC_SOFT_Stop();    //����IIC����
}

/***************************************************************************************
**������:       fn_IIC_SOFT_Write
**��������:     --
**�������:     uchar addr,uchar *sendbuf,uint num
**�������:     --
**��ע:c_IIC_Soft.c,2014/12/27,By Haixing.Huang
****************************************************************************************/
void fn_IIC_SOFT_Read(uchar addr,uchar *buf,uint num)
{
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(0xa0);
  IIC_SOFT_SendByte(addr);    //����IIC��ַ
//  IIC_SOFT_SendByte(addr);    //����IIC��ַ
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(0xa1);
  for(int i = 0; i < num; i++)    //��������
  {
    *buf = IIC_SOFT_RecvByte();
    IIC_SOFT_SendACK(0);    //Ӧ��
    buf++;
  }
  IIC_SOFT_SendACK(1);    //��Ӧ��
  IIC_SOFT_Stop();    //����IIC����
}

