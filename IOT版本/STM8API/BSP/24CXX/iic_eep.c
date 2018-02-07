/*********************************************************************
*                                                                    
*              (c) Copyright 2008-2058，MAKEPOWER                    
*                      All Rights Reserved                           
*                                                                    
* File        :  iic.c                                                 
* Date        :  2017.08.01                                           
* Ver         :  1.0                                                
* By          :  suqi V1.0                                         
* De          :      
* Description :                                        
*********************************************************************/
/*==============详细说明==============================================
1、
2、
3、
====================================================================*/


#include "config.h"
uchar 	IFACK;                      
uchar 	NO_ACK; 

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : EEPROM_Init
// 功能描述  : EEPROM初始化
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
//void EEPROM_Init(void)
//{	
//	//GPIO_EEPROM_INIT();  
//
//       EEPROM_I2C_WP_1();//I2C_WP = 1;     //开启写保护
//}
void GPIO_EEPROM_INIT(void)
{
        GPIO_Init(I2C_GPIO_PORT, I2C_SCL_PIN|I2C_SDA_PIN, GPIO_Mode_Out_OD_HiZ_Fast);
        GPIO_Init(I2C_GPIO_PORT, I2C_WP_PIN, GPIO_Mode_Out_PP_High_Fast);
        I2C_WP_1();    //I2C_WP = 1;     //开启写保护
        CS_I2C_Stop();
}


//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : I2CStart
// 功能描述  : 开启SD2400的I2C总线
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : read_eep_byte
// 功能描述  : 读一个字节
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

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





//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : I2CStop
// 功能描述  : 关闭SD2400的I2C总线
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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



//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : I2CAck
// 功能描述  : 发送 ACK
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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



//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : I2CNoAck
// 功能描述  : 发送NO ACK
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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



//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : I2CSendByte
// 功能描述  : MCU向SD2400发送一个字节
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
static void CS_I2C_Send_Byte(uchar demand) //数据从高位到低位//
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


//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : I2CReceiveByte
// 功能描述  : MCU从SD2400读入一字节
// 输入参数  : None
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
static uchar CS_I2C_Receive_Byte(void) //数据从高位到低位//
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


//static uchar CS_I2C_WaitAck(void)  //返回为:1=有ACK,0=无ACK
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

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : write_eep_byte
// 功能描述  : 写一个字节
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void  write_eep_byte(uchar Data,uint address)               
{		
  CS_I2C_Start();
  CS_I2C_Send_Byte( 0xa0 );
  CS_I2C_Send_Byte( address>>8 );
  CS_I2C_Send_Byte( address %256 );
  CS_I2C_Send_Byte(Data);
  CS_I2C_Stop();
  
  Delay_MS(10);//Delay_MS(10);       // 等待写周期完成
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : write_multi_eep_byte_aa
// 功能描述  : 传地址
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void write_multi_eep_byte_aa(uint TeAddr)
{
  CS_I2C_Start();
  CS_I2C_Send_Byte(0xa0);
  CS_I2C_Send_Byte(TeAddr>>8);
  CS_I2C_Send_Byte(TeAddr%256);
  
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : write_multi_eep_byte_bb
// 功能描述  : 连续写一个字节
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void write_multi_eep_byte_bb(uchar Data)
{
  CS_I2C_Send_Byte(Data);
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : write_multi_eep_byte_end
// 功能描述  : 连续写结束
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : None
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
void write_multi_eep_byte_end(void)
{
  CS_I2C_Stop();
  Delay_MS(10);
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : ChekAndReadEepromSum
// 功能描述  : 读出EEPROM数据，并计算校验和
// 对24c64进行连续读操作时候地址指针在整个数据内(8K字节内)自加
// 输入参数  : 要读数据地址、读取数据的长度；存储数据指针
// 输出参数  : dat：输出数据指针地址
// 返回参数  : cheksum：=0：读取数据正确 !=0:读取数据错误
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
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
      CS_I2C_Ack();					//发送ACK进行连续读
      cheksum += *dat; 				//计算校验和
      *dat++;                			//主机接收存储单元加一           
    }
    tempData = CS_I2C_Receive_Byte();   //@@edit by zl  *dat
    CS_I2C_Nack();				   		//读最后一个数据,不发送ACK
    cheksum += tempData;  				//计算校验和
    
    CS_I2C_Stop();
  }
  return cheksum;
}
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : WriteEeprom
// 功能描述  : 写EEPROM数据，并写校验和,对24c64写操作时候地址指针子在单页内自加
// 24c64共有8K空间,256页,每页32个地址,每次最多可连续对页写入32个数据
// 输入参数  : 要写据地址、写数据的长度；存储数据指针
// 输出参数  : None
// 返回参数  : false：写数据失败 true:写数据成功
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
uchar WriteEeprom(uint address, uint cont, uchar *dat)
{ 
  uchar  chk_sum=0,Temp;
  I2C_WP_0();//I2C_WP = 0;     //关闭写保护
  if (cont != 0) 									  // 写数据个数不为零 
  {
    write_multi_eep_byte_aa(address);    
    while (cont--)
    {	Watchdog_Feed();   						  // 喂看门狗
    write_multi_eep_byte_bb( *dat );  		  // 发送数据 
    Temp = *dat;
    chk_sum += Temp;
    *dat++;    					  			  // 主机发送地址指针自加 
    address++; 								  // 地址同步加一,只做地址标志用 
    if (!(address%PageByte) && (cont != 0))   // 地址低五位全为零,换页 
    {	Watchdog_Feed();   					  // 喂看门狗
    write_multi_eep_byte_end();
    write_multi_eep_byte_aa(address); 
    }
    }
    write_multi_eep_byte_end();
    chk_sum = ~chk_sum;    	
    // 把校验和写进去
    write_eep_byte(chk_sum,address);
    
    //检查校验和
    if(read_eep_byte(address)!=chk_sum)
    {
      I2C_WP_1();//I2C_WP = 1;      //开启写保护
      return false;
    }
  }
  I2C_WP_1();//I2C_WP = 1;     //开启写保护
  return true;
}

//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : WriteEepromNoChek
// 功能描述  : 写EEPROM数据，不写校验和,对24c64写操作时候地址指针子在单页内自加
// 24c64共有8K空间,256页,每页32个地址,每次最多可连续对页写入32个数据
// 输入参数  : 要写据地址、写数据的长度；存储数据指针
// 输出参数  : dat：输出数据指针地址
// 返回参数  : true：写入数据成功
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
uchar WriteEepromNoChek(uint address, uint cont, uchar *dat)
{ 
  //return true;//临时注释~！！！！！！！！！！！！
  uchar  chk_sum=0,Temp;
  I2C_WP_0();//I2C_WP = 0;     //关闭写保护
  Delay_MS(10);
  if (cont != 0) 									  // 写数据个数不为零 
  {
    write_multi_eep_byte_aa(address);    
    while (cont--)
    {	Watchdog_Feed();   						  // 喂看门狗
    write_multi_eep_byte_bb( *dat );  		  // 发送数据 
    Temp = *dat;
    chk_sum += Temp;
    *dat++;    					  			  // 主机发送地址指针自加 
    address++; 								  // 地址同步加一,只做地址标志用 
    if (!(address%PageByte) && (cont != 0))   // 地址低五位全为零,换页 
    {	
      Watchdog_Feed();   					  // 喂看门狗
      write_multi_eep_byte_end();
      write_multi_eep_byte_aa(address); 
    }
    }
    write_multi_eep_byte_end();
  }
  I2C_WP_1();//I2C_WP = 1;     //开启写保护
  Delay_MS(10);
  return true;
}


//uchar ReadEeprom(uint Adder,uint Lenth,uchar *Point);
//uchar WriteEeprom(uint address, uint cont, uchar *dat);
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : ReadEEPROM
// 功能描述  : 读取EEPROM数据，不正确时重复3次
// 输入参数  : None.
// 输出参数  : None
// 返回参数  : true：读取数据正确 false:读取数据错误
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
uchar ReadEeprom(uint Adder,uint Lenth,uchar *Point)
{
  uchar Cnt=3;
  while(Cnt!=0)
  {
    if(ChekAndReadEepromSum(Adder,Lenth,Point)!=0)//校验和出错
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
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
// 函数名称  : EEPROM_CheckOk
// 功能描述  : EEPROM芯片检查
// 输入参数  : None
// 输出参数  : None
// 返回参数  : ERROR错误  SUCCESS成功
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※

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
  WriteEepromNoChek(OutStorage_text_add,sizeof(TestValue),TestValue);//写在空白无用区域
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
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※
//                          程序结束(END)
//※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※※













































