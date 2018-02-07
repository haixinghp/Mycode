/*********************************************************************
*                                                                    
*              (c) Copyright 2008-2058��MAKEPOWER                    
*                      All Rights Reserved                           
*                                                                    
* File        :  iic.c                                                 
* Date        :  2017.08.01                                           
* Ver         :  1.0                                                
* By          :  suqi V1.0                                         
* De          :      
* Description :                                        
*********************************************************************/
/*==============��ϸ˵��==============================================
1��
2��
3��
====================================================================*/


#include "config.h"
uchar 	IFACK;                      
uchar 	NO_ACK; 

//������������������������������������������������������������������������������
// ��������  : EEPROM_Init
// ��������  : EEPROM��ʼ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
//void EEPROM_Init(void)
//{	
//	//GPIO_EEPROM_INIT();  
//
//       EEPROM_I2C_WP_1();//I2C_WP = 1;     //����д����
//}
void GPIO_EEPROM_INIT(void)
{
        GPIO_Init(I2C_GPIO_PORT, I2C_SCL_PIN|I2C_SDA_PIN, GPIO_Mode_Out_OD_HiZ_Fast);
        GPIO_Init(I2C_GPIO_PORT, I2C_WP_PIN, GPIO_Mode_Out_PP_High_Fast);
        I2C_WP_1();    //I2C_WP = 1;     //����д����
        CS_I2C_Stop();
}


//������������������������������������������������������������������������������
// ��������  : I2CStart
// ��������  : ����SD2400��I2C����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

//������������������������������������������������������������������������������
// ��������  : read_eep_byte
// ��������  : ��һ���ֽ�
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������

static void CS_I2C_Start(void)
{
  CS_Delay_10_uS();
  I2C_SDA_1();//I2C_SDA=1;
  CS_Delay_10_uS();
  I2C_SCL_1();//I2C_SCL=1;
  CS_Delay_10_uS();
  //if ( !I2C_SDAin) return false;
  //if ( !I2C_SDA_READ()) return false;
  I2C_SDA_0();//I2C_SDA=0;
  CS_Delay_10_uS();
  I2C_SCL_0();//I2C_SCL=0;
  CS_Delay_10_uS();
  
}





//������������������������������������������������������������������������������
// ��������  : I2CStop
// ��������  : �ر�SD2400��I2C����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void CS_I2C_Stop(void)
{
  CS_Delay_10_uS();
  I2C_SDA_0();//I2C_SDA=0;
  CS_Delay_10_uS();
  I2C_SCL_1();//I2C_SCL=1;
  CS_Delay_10_uS();
  I2C_SDA_1();//I2C_SDA=1;
  CS_Delay_10_uS();   
}



//������������������������������������������������������������������������������
// ��������  : I2CAck
// ��������  : ���� ACK
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void CS_I2C_Ack(void)
{	
  CS_Delay_10_uS();
  I2C_SDA_0();//I2C_SDA=0;
  CS_Delay_10_uS();
  I2C_SCL_1();//I2C_SCL=1;
  CS_Delay_10_uS();
  I2C_SCL_0();//I2C_SCL=0;
  CS_Delay_10_uS();
}



//������������������������������������������������������������������������������
// ��������  : I2CNoAck
// ��������  : ����NO ACK
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void CS_I2C_Nack(void)
{	
  CS_Delay_10_uS();
  I2C_SDA_1();//I2C_SDA=1;
  CS_Delay_10_uS();
  I2C_SCL_1();//I2C_SCL=1;
  CS_Delay_10_uS();
  I2C_SCL_0();//I2C_SCL=0;
  CS_Delay_10_uS();
}



//������������������������������������������������������������������������������
// ��������  : I2CSendByte
// ��������  : MCU��SD2400����һ���ֽ�
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void CS_I2C_Send_Byte(uchar demand) //���ݴӸ�λ����λ//
{
  uchar i = 8;
  while( i-- )
  {
    CS_Delay_10_uS();
    if ( demand &0x80 )   I2C_SDA_1();//I2C_SDA=1;
    else                  I2C_SDA_0();//I2C_SDA=0;
    CS_Delay_10_uS();
    I2C_SCL_1();//I2C_SCL=1;
    CS_Delay_10_uS();
    I2C_SCL_0();//I2C_SCL=0;
    demand = demand << 1;
  }
  CS_I2C_Nack();
}


//������������������������������������������������������������������������������
// ��������  : I2CReceiveByte
// ��������  : MCU��SD2400����һ�ֽ�
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static uchar CS_I2C_Receive_Byte(void) //���ݴӸ�λ����λ//
{
  uchar i = 8, d = 0;
  CS_Delay_10_uS();
  I2C_SDA_1();//I2C_SDA=1;	
  while ( i--)
  {
    d = d << 1;
    CS_Delay_10_uS();
    I2C_SCL_1();//I2C_SCL=1;
    //if ( I2C_SDAin )   d++;
    if ( I2C_SDA_READ() )   d++;
    CS_Delay_10_uS();
    I2C_SCL_0();//I2C_SCL=0;
  }
  return d;
}


//static uchar CS_I2C_WaitAck(void)  //����Ϊ:1=��ACK,0=��ACK
//{	//uchar errtime=255;
//        uchar re;
//	I2C_SCL_0();//SCL=0;
//	I2C_SDA_1();//SDA=1;
//	CS_Delay_10_uS();
//	I2C_SCL_1();//SCL=1;
//	CS_Delay_10_uS();
//        if(I2C_SDA_READ())
//        {
//          re = true;
//        }
//        else
//        {
//          re = false;
//        }
//	I2C_SCL_0();//SCL=0;
//	return re;
//}


uchar read_eep_byte(uint address)
{
  uchar temp;
  CS_I2C_Start();	
  CS_I2C_Send_Byte( 0xa0 );
  CS_I2C_Send_Byte( address>>8 );
  CS_I2C_Send_Byte( address % 256 );
  CS_I2C_Start();
  CS_I2C_Send_Byte( 0xa1 );
  temp = CS_I2C_Receive_Byte();
  CS_I2C_Nack();
  CS_I2C_Stop();
  
  return temp;
}

//������������������������������������������������������������������������������
// ��������  : write_eep_byte
// ��������  : дһ���ֽ�
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void  write_eep_byte(uchar Data,uint address)               
{		
  CS_I2C_Start();
  CS_I2C_Send_Byte( 0xa0 );
  CS_I2C_Send_Byte( address>>8 );
  CS_I2C_Send_Byte( address %256 );
  CS_I2C_Send_Byte(Data);
  CS_I2C_Stop();
  
  Delay_MS(10);//Delay_MS(10);       // �ȴ�д�������
}

//������������������������������������������������������������������������������
// ��������  : write_multi_eep_byte_aa
// ��������  : ����ַ
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void write_multi_eep_byte_aa(uint TeAddr)
{
  CS_I2C_Start();
  CS_I2C_Send_Byte(0xa0);
  CS_I2C_Send_Byte(TeAddr>>8);
  CS_I2C_Send_Byte(TeAddr%256);
  
}

//������������������������������������������������������������������������������
// ��������  : write_multi_eep_byte_bb
// ��������  : ����дһ���ֽ�
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void write_multi_eep_byte_bb(uchar Data)
{
  CS_I2C_Send_Byte(Data);
}

//������������������������������������������������������������������������������
// ��������  : write_multi_eep_byte_end
// ��������  : ����д����
// �������  : None.
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void write_multi_eep_byte_end(void)
{
  CS_I2C_Stop();
  Delay_MS(10);
}

//������������������������������������������������������������������������������
// ��������  : ChekAndReadEepromSum
// ��������  : ����EEPROM���ݣ�������У���
// ��24c64��������������ʱ���ַָ��������������(8K�ֽ���)�Լ�
// �������  : Ҫ�����ݵ�ַ����ȡ���ݵĳ��ȣ��洢����ָ��
// �������  : dat���������ָ���ַ
// ���ز���  : cheksum��=0����ȡ������ȷ !=0:��ȡ���ݴ���
//������������������������������������������������������������������������������
uchar ChekAndReadEepromSum(uint address, uint cont,uchar *dat)
{   
  uchar cheksum=1;
  
  uchar tempData = 0; //@@edit by zl
  
  if (cont != 0) 
  {   
    CS_I2C_Start();	
    CS_I2C_Send_Byte( 0xa0 );
    CS_I2C_Send_Byte( address>>8 );
    CS_I2C_Send_Byte( address % 256 );
    CS_I2C_Start();
    CS_I2C_Send_Byte( 0xa1 ); 
    
    while (cont--)
    {
      Watchdog_Feed(); 
      *dat = CS_I2C_Receive_Byte();
      CS_I2C_Ack();					//����ACK����������
      cheksum += *dat; 				//����У���
      *dat++;                			//�������մ洢��Ԫ��һ           
    }
    tempData = CS_I2C_Receive_Byte();   //@@edit by zl  *dat
    CS_I2C_Nack();				   		//�����һ������,������ACK
    cheksum += tempData;  				//����У���
    
    CS_I2C_Stop();
  }
  return cheksum;
}
//������������������������������������������������������������������������������
// ��������  : WriteEeprom
// ��������  : дEEPROM���ݣ���дУ���,��24c64д����ʱ���ַָ�����ڵ�ҳ���Լ�
// 24c64����8K�ռ�,256ҳ,ÿҳ32����ַ,ÿ������������ҳд��32������
// �������  : Ҫд�ݵ�ַ��д���ݵĳ��ȣ��洢����ָ��
// �������  : None
// ���ز���  : false��д����ʧ�� true:д���ݳɹ�
//������������������������������������������������������������������������������
uchar WriteEeprom(uint address, uint cont, uchar *dat)
{ 
  uchar  chk_sum=0,Temp;
  I2C_WP_0();//I2C_WP = 0;     //�ر�д����
  if (cont != 0) 									  // д���ݸ�����Ϊ�� 
  {
    write_multi_eep_byte_aa(address);    
    while (cont--)
    {	Watchdog_Feed();   						  // ι���Ź�
    write_multi_eep_byte_bb( *dat );  		  // �������� 
    Temp = *dat;
    chk_sum += Temp;
    *dat++;    					  			  // �������͵�ַָ���Լ� 
    address++; 								  // ��ַͬ����һ,ֻ����ַ��־�� 
    if (!(address%PageByte) && (cont != 0))   // ��ַ����λȫΪ��,��ҳ 
    {	Watchdog_Feed();   					  // ι���Ź�
    write_multi_eep_byte_end();
    write_multi_eep_byte_aa(address); 
    }
    }
    write_multi_eep_byte_end();
    chk_sum = ~chk_sum;    	
    // ��У���д��ȥ
    write_eep_byte(chk_sum,address);
    
    //���У���
    if(read_eep_byte(address)!=chk_sum)
    {
      I2C_WP_1();//I2C_WP = 1;      //����д����
      return false;
    }
  }
  I2C_WP_1();//I2C_WP = 1;     //����д����
  return true;
}

//������������������������������������������������������������������������������
// ��������  : WriteEepromNoChek
// ��������  : дEEPROM���ݣ���дУ���,��24c64д����ʱ���ַָ�����ڵ�ҳ���Լ�
// 24c64����8K�ռ�,256ҳ,ÿҳ32����ַ,ÿ������������ҳд��32������
// �������  : Ҫд�ݵ�ַ��д���ݵĳ��ȣ��洢����ָ��
// �������  : dat���������ָ���ַ
// ���ز���  : true��д�����ݳɹ�
//������������������������������������������������������������������������������
uchar WriteEepromNoChek(uint address, uint cont, uchar *dat)
{ 
  //return true;//��ʱע��~������������������������
  uchar  chk_sum=0,Temp;
  I2C_WP_0();//I2C_WP = 0;     //�ر�д����
  Delay_MS(10);
  if (cont != 0) 									  // д���ݸ�����Ϊ�� 
  {
    write_multi_eep_byte_aa(address);    
    while (cont--)
    {	Watchdog_Feed();   						  // ι���Ź�
    write_multi_eep_byte_bb( *dat );  		  // �������� 
    Temp = *dat;
    chk_sum += Temp;
    *dat++;    					  			  // �������͵�ַָ���Լ� 
    address++; 								  // ��ַͬ����һ,ֻ����ַ��־�� 
    if (!(address%PageByte) && (cont != 0))   // ��ַ����λȫΪ��,��ҳ 
    {	
      Watchdog_Feed();   					  // ι���Ź�
      write_multi_eep_byte_end();
      write_multi_eep_byte_aa(address); 
    }
    }
    write_multi_eep_byte_end();
  }
  I2C_WP_1();//I2C_WP = 1;     //����д����
  Delay_MS(10);
  return true;
}


//uchar ReadEeprom(uint Adder,uint Lenth,uchar *Point);
//uchar WriteEeprom(uint address, uint cont, uchar *dat);
//������������������������������������������������������������������������������
// ��������  : ReadEEPROM
// ��������  : ��ȡEEPROM���ݣ�����ȷʱ�ظ�3��
// �������  : None.
// �������  : None
// ���ز���  : true����ȡ������ȷ false:��ȡ���ݴ���
//������������������������������������������������������������������������������
uchar ReadEeprom(uint Adder,uint Lenth,uchar *Point)
{
  uchar Cnt=3;
  while(Cnt!=0)
  {
    if(ChekAndReadEepromSum(Adder,Lenth,Point)!=0)//У��ͳ���
    {	Cnt--;	
    }
    else
    {	
      Cnt=1;    
      return true;
    }
  }
  return false;
}
void AT24C256_ALL_CLEAR(void)
{ 
  I2C_WP_0();
  u8 temp = 0;
  u8 *p = &temp;
  for(u16 i=0; i<= 0x7FFF; i++)
    WriteEepromNoChek(i,1,p);
  
}
//������������������������������������������������������������������������������
// ��������  : EEPROM_CheckOk
// ��������  : EEPROMоƬ���
// �������  : None
// �������  : None
// ���ز���  : ERROR����  SUCCESS�ɹ�
//������������������������������������������������������������������������������

uint8_t EEPROM_CheckOk(void)
{
  uint8_t i = 0;
  uint8_t TestValue[5] ={0xa0,0xa1,0xa2,0xa3,0xa4};
  u8 eeprom_store[5] = {0,0,0,0,0};
  eeprom_store[0] = read_eep_byte(OutStorage_text_add);
  eeprom_store[1] = read_eep_byte(OutStorage_text_add+1);
  eeprom_store[2] = read_eep_byte(OutStorage_text_add+2);
  eeprom_store[3] = read_eep_byte(OutStorage_text_add+3);
  eeprom_store[4] = read_eep_byte(OutStorage_text_add+4);
  WriteEepromNoChek(OutStorage_text_add,sizeof(TestValue),TestValue);//д�ڿհ���������
  for(i=0;i<5;i++)
  {
    TestValue[i] = 0;    
  }
  TestValue[0] = read_eep_byte(OutStorage_text_add);
  TestValue[1] = read_eep_byte(OutStorage_text_add+1);
  TestValue[2] = read_eep_byte(OutStorage_text_add+2);
  TestValue[3] = read_eep_byte(OutStorage_text_add+3);
  TestValue[4] = read_eep_byte(OutStorage_text_add+4);
  for(i=0;i<5;i++)
  {
    if(TestValue[i] != (0xa0+i))
    {
      //          printf("error\r\n");
      //          printf("%d\r\n",TestValue[i]);
      return  ERROR;           
    }
    //      printf("%d\r\n",TestValue[i]);  
  }
  WriteEepromNoChek(OutStorage_text_add,sizeof(eeprom_store),eeprom_store);
  //    printf("success\r\n");
  return SUCCESS ;
  
}

void AT24C256_TO_Print(void)
{
  for(u16 Temp_ADDR=0; Temp_ADDR<0x7FFF; Temp_ADDR++){
    USART_SendData8(USART3, read_eep_byte(Temp_ADDR));
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
  }
}
//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������













































