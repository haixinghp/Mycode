#include "RTC_SD3078.h"

_struRTC_SD3078_Time struRTC_SD3078_Time = {0x55,0x59,0x14,0x01,0x12,0x11,0x14};	//��ʼ��ʱ��ṹ�����������ʱ�䣺2014��11��12�� 14:59:55  ����һ��

/***************************************************************************************
**������:         --
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_Init(void)
{
    fn_IIC_SOFT_Init();
}

/***************************************************************************************
**������:         --
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_SetPar(u8 addr,u8 cmd,u8 data)
{
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(addr);
  IIC_SOFT_SendByte(cmd);
  IIC_SOFT_SendByte(data);
  IIC_SOFT_Stop();
}
/***************************************************************************************
**������:         --
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_WriteON()
{
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(RTC_SD3078_CTR2);
  IIC_SOFT_SendByte(0x80);
  IIC_SOFT_Stop();
  
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(0x0f);
  IIC_SOFT_SendByte(0x84);
  IIC_SOFT_Stop();
}

/***************************************************************************************
**������:         --
**��������:     --
**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_WriteOFF()
{
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(0x10);
  IIC_SOFT_SendByte(0x00);
  IIC_SOFT_Stop();
  
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(0x0f);
  IIC_SOFT_SendByte(0x00);
  IIC_SOFT_Stop();
}
/***************************************************************************************
**������:       RTC_SD3078_ReadTime
**��������:     ��SD30XXʵʱ���ݼĴ���
00H ��
01H ����
02H Сʱ
03H ����
04H ��
05H �� 
06H ��
**�������:     u8 *pPar
**�������:     
**��ע:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_ReadTime(u8 *pPar)
{
  int i;
  IIC_SOFT_Start();   //��ʼIIC����
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
**������:        RTC_SD3078_SetTime
**��������:     
дʱ�����Ҫ��һ�ζ�ʵʱʱ��Ĵ���(00H~06H)����д��
�����Ե�����7��ʱ�������е�ĳһλ����д����,������ܻ�����ʱ�����ݵĴ����λ
Ҫ�޸�����ĳһ������ , Ӧһ����д��ȫ�� 7 ��ʵʱʱ������
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_SetTime(u8 *pPar)
{
  int i;
  RTC_SD3078_WriteON(); //�ر�д����
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(0x00);
  for(i=0;i<7;i++)
  {
    IIC_SOFT_SendByte(*pPar);
    pPar++;
  }
  IIC_SOFT_Stop();
  RTC_SD3078_WriteOFF();//��д����
}
/***************************************************************************************
**������:        RTC_SD3078_CountDownAlarm
**��������:     
freq 30 ѡ��ʱ��Ƶ��Դ��TDS1��TDS0��Ϊ1/60HZ
time 5 ����ʱ��ֵ�Ĵ���������8λ����ʱ������ֵ��5min��
**�������:     u8 freq,u8 time
**�������:     --
**��ע:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_CountDownAlarm(u8 freq,u8 time)
{
  RTC_SD3078_WriteON(); //�ر�д����
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_CTR2,0x0f);//���嵹��ʱ�ж�������λ��INTDE��
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_CTR2,0xf4);//�����������жϣ�IM=1��INT�ж�ѡͨ��INTS1��INTS0�������õ���ʱ�ж�������λ��INTDE��
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_CTR3,freq);//ѡ��ʱ��Ƶ��Դ��TDS1��TDS0��Ϊ1/60HZ
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_COUNTDOWN1,time);//����ʱ��ֵ�Ĵ���������8λ����ʱ������ֵ��5min��
  RTC_SD3078_WriteOFF();//��д����
}
/***************************************************************************************
**������:        RTC_SD3078_ClearCountDownAlarm
**��������:     
�������ʱ�澯
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_ClearCountDownAlarm(void)
{
  RTC_SD3078_WriteON(); //�ر�д����
  RTC_SD3078_SetPar(RTC_SD3078_WRITE_ADDR,RTC_SD3078_CTR2,0xf0);
  RTC_SD3078_WriteOFF();//��д����
}

/***************************************************************************************
**������:        RTC_SD3078_WriteData
**��������:     
SD3078��addrд��len���ȵ�����
**�������:     u8 addr,u8 *pPar,u8 len
**�������:     --
**��ע:         YY.c,2017-11-16,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_WriteData(u8 addr,u8 *pPar,u8 len)
{
    int i;
  RTC_SD3078_WriteON(); //�ر�д����
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(addr);
  for(i=0;i<len;i++)
  {
    IIC_SOFT_SendByte(*pPar);
    pPar++;
  }
  IIC_SOFT_Stop();
  RTC_SD3078_WriteOFF();//��д����
}
/***************************************************************************************
**������:        RTC_SD3078_ReadData
**��������:     
SD3078��addr��ȡlen���ȵ�����
**�������:     u8 addr,u8 *pPar,u8 len
**�������:     --
**��ע:         YY.c,2017-11-16,By Haixing.Huang
****************************************************************************************/
void RTC_SD3078_ReadData(u8 addr,u8 *pPar,u8 len)
{
  int i;
  IIC_SOFT_Start();   //��ʼIIC����
  IIC_SOFT_SendByte(RTC_SD3078_WRITE_ADDR);
  IIC_SOFT_SendByte(addr);
  IIC_SOFT_Start();   //��ʼIIC����
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
//**������:        RTC_SD3078_WriteInitFlag
//**��������:     
//�������ʱ�澯
//**�������:     --
//**�������:     --
//**��ע:         YY.c,2017-09-02,By Haixing.Huang
//****************************************************************************************/
//void RTC_SD3078_WriteInitFlag()
//{
//  RTC_SD3078_WriteData(RTC_SD3078_USER_RAM,SD3078_CheckWriteTab,8)
//}
/***************************************************************************************
**������:        RTC_SD3078_CheckInitFlag
**��������:     
�������ʱ�澯
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-02,By Haixing.Huang
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
  if(8 != i) //У��δͨ��
  {
//    RTC_SD3078_WriteData(RTC_SD3078_USER_RAM,(u8 *)SD3078_CheckWriteTab,8);
    return 1;
  }
  else //У��ͨ��
  {
    return 0;
  }
}
/***************************************************************************************
**������:        RTC_SD3078_WriteInitFlag
**��������:     
�������ʱ�澯
**�������:     --
**�������:     --
**��ע:         YY.c,2017-09-02,By Haixing.Huang
****************************************************************************************/
s8 RTC_SD3078_WriteInitFlag()
{
  RTC_SD3078_WriteData(RTC_SD3078_USER_RAM,(u8 *)SD3078_CheckWriteTab,8);
  return RTC_SD3078_CheckInitFlag();
}
