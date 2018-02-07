//#include "stm8l15x.h"
#include "config.h"
#include <stdlib.h>
#include <stdio.h>

#define X_CHANNEL 0
#define Y_CHANNEL 1
#define Z_CHANNEL 2

//static ACCEL_AD  adcval;
//SYS_XYZ_DATA adxl362TestData;
//ADXL_XYZ_ANGLE Adxl_XYZ_Angle;
//ACCEL_AD0 FifoBufArr[128];
//ACCEL_AD0 TempFifoDataBuf;
//u16 FifoBufLen = 0;
//u8 temparray[32];

SYS_XYZ_DATA SysXyzData;//系统状态值
//FIFO_ONE_WORD U16FifoWordArr[MAX_BUF_NUM];//1024byte
//SYS_BUF_INFO SysBuf;//系统数据缓冲区 201byte
//FIFO_BUF_INFO FifoData;//FIFO数据缓冲区 //1082byte

u16 receive_len = 0;
u16 max_len = 0;
u16 detail_len = 0;
u16 idx = 0;
u8 temp_state_before = 0;
u8 temp_state_after = 0;
u16 temp_data = 0;
void adxl362_io_config() { //端口配置
  //GPIO_Init(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow);
  //GPIO_Init(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  //GPIO_Init(GPIOB,GPIO_Pin_3 | GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOB, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);


  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);//电源脚 设置为高输出 打开ADXL电源
  GPIO_SetBits(GPIOE, GPIO_Pin_7); //ADXL电源打开

  //GPIO_Init(GPIOB,GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);
  //EXTI_SetPinSensitivity(EXTI_Pin_6,EXTI_Trigger_Falling);
}

void spi_config(void) {
  adxl362_io_config();
  SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_16, SPI_Mode_Master, \
           SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex, SPI_NSS_Hard, 1);
}


void adxl362_power_off() {
  ADXL362_POWER_OFF;
}

void adxl362_power_on() {
  ADXL362_POWER_ON;
}

void adxl362_reset() {
  ADXL362_POWER_OFF;
  DelayT_ms(200);
  ADXL362_POWER_ON;
}

// **************************************************************
// 函数:adxl362_spi_config
// 参数:无
// 返回:无
// 描述:SPI初始化
// **************************************************************
void adxl362_spi_config() {
  //adxl362_reset();

  //spi_config();


  adxl362_io_config();
  adxl362_reset();

  //ADXL362_RESET_KEY();

  CS_H();
  CLK_L();
  DIO_H();
  if (ADXL362_Init()) {
    //debug_send("ADXL362 init success!");
    PrintString_("三轴初始化成功!");
  } else {
    //debug_send("ADXL362 init failed!");
    PrintString_("三轴初始化失败，系统即将重启!");
    sys_restart();
  }
  CS_L();
  CLK_L();
  DIO_L();
}

// **************************************************************
// 函数:Send_OneByte
// 参数:Value
// 返回:无
// 描述:SPI写一字节数据
// **************************************************************


void Send_OneByte(unsigned char DATA8) //从SPI发8位数
{ //上升沿写入
  unsigned char x;

  for (x = 0; x < 8; x++)
  {
    CLK_L();
    if (DATA8 & 0x80)DIO_H();
    else DIO_L();
    asm("nop");
    CLK_H();
    DATA8 = DATA8 << 1;
  }
  DIO_H();
  CLK_L();
  //SPI_SendData(SPI1, DATA8);
}  //end of Send_OneByte






// **************************************************************
// 函数:Read_OneByte
// 参数:无
// 返回:Value
// 描述:SPI读一字节数据
// **************************************************************
unsigned char Read_OneByte(void)     //从SPI收8位数
{ //下降沿输出
  unsigned char DATA8;
  unsigned char x;
  CLK_L();
  DATA8 = 0x00;
  asm("nop");
  for (x = 0; x < 8; x++)
  {
    //asm("nop");
    CLK_H();
    //asm("nop");
    DATA8 = DATA8 << 1;
    if (DO_READ())DATA8 = DATA8 | 0x01;
    CLK_L();
  }
  DIO_H();
  CLK_L();
  return (DATA8);
  //return SPI_ReceiveData(SPI1);
}//end of Read_OneByte



// **************************************************************
// 函数:ADXL362_WriteReg
// 参数:Reg Address,Value
// 返回:无
// 描述:写ADXL362寄存器
// **************************************************************
void ADXL362_WriteReg(unsigned char addr, unsigned char val)
{
  CLK_L();
  CS_L();
  Send_OneByte(ADXL362_WRITE_REG);
  Send_OneByte(addr);
  Send_OneByte(val);
  CS_H();
  DIO_H();
  CLK_L();
  CS_H();
}

// **************************************************************
// 函数:ADXL362_ReadReg
// 参数:Reg Addres
// 返回:Reg Value
// 描述:读ADXL362寄存器
// **************************************************************
unsigned char ADXL362_ReadReg(unsigned char addr)
{
  unsigned char res;
  CLK_L();
  CS_L();
  Send_OneByte(ADXL362_READ_REG);
  Send_OneByte(addr);
  res = Read_OneByte();
  CS_H();
  DIO_H();
  CLK_L();
  return res;
}

ACCEL_AD16 ADXL362_ReadAccel(void)
{
  unsigned char i;
  //unsigned char
  //static unsigned char tempcount = 0;
  static ACCEL_AD16 accelval;
  static unsigned char addr[6];// = (char *)(&accelval);
  CS_H();
  DelayT_ms(10);
  CS_L();
  //Send_OneByte(ADXL362_READ_REG);
  //ADXL362_ReadReg(ADXL362_REG_XDATA_L);

  Send_OneByte(ADXL362_READ_REG);
  Send_OneByte(ADXL362_REG_XDATA_L);
  for (i = 0; i < 6; i++)
  {
    addr[i] = Read_OneByte();
  }
  CS_H();

  accelval.xadc = (addr[0] + ((unsigned int)(addr[1]) << 8) & 0xffff);
  accelval.yadc = (addr[2] + ((unsigned int)(addr[3]) << 8) & 0xffff);
  accelval.zadc = (addr[4] + ((unsigned int)(addr[5]) << 8) & 0xffff);
  //accelval.tadc = addr[6] + ((unsigned int)(addr[7]) << 8) & 0x0fff;
  return accelval;
}

void printNumStr(s8 num) {
  //static char str[10]="";
  u8 str_temp[10];
  u8 i=0;
  if(num<0){
    str_temp[i++] = '-';
    num = 0 - num;
  }
  if(num<10){
    str_temp[i++] = num + '0';
    str_temp[i++] = 0;
  }
  else if(num<100){
    str_temp[i++] = num/10 + '0';
    str_temp[i++] = num%10 + '0';
    str_temp[i++] = 0;
  }
  else if(num<125){
    str_temp[i++] = num/100       + '0';
    str_temp[i++] = num/10%10     + '0';
    str_temp[i++] = num%10        + '0';
    str_temp[i++] = 0;
  }
  else{
    debug_send("打印数字不符合范围！");
  }
  
  //sprintf(str, "%d", num);
  PrintString((char*)str_temp);
}
void printNumStr_(s8 num) {
  
  printNumStr(num);
  
  PrintString_("");
}
void printUNum2Str(u16 num)
{  
  u8 str_temp[10];
  u8 i=0;
  if(num<10){
    str_temp[i++] = num + '0';
    str_temp[i++] = 0;
  }
  else if(num<100){
    str_temp[i++] = num/10 + '0';
    str_temp[i++] = num%10 + '0';
    str_temp[i++] = 0;
  }
  else if(num<1000){
    str_temp[i++] = num/100       + '0';
    str_temp[i++] = num/10%10     + '0';
    str_temp[i++] = num%10        + '0';
    str_temp[i++] = 0;
  }
  else if(num<10000){
    str_temp[i++] = num/1000      + '0';
    str_temp[i++] = num/100%10    + '0';
    str_temp[i++] = num/10%10     + '0';
    str_temp[i++] = num%10        + '0';
    str_temp[i++] = 0;
  }
  else if(num<=65535){
    str_temp[i++] = num/10000      + '0';
    str_temp[i++] = num/1000%10    + '0';
    str_temp[i++] = num/100%10     + '0';
    str_temp[i++] = num/10%10     + '0';
    str_temp[i++] = num%10        + '0';
    str_temp[i++] = 0;
  }
  else{
    debug_send("打印数字不符合范围！");
  }
  
  //sprintf(str, "%d", num);
  PrintString_((char*)str_temp);
}
void printNum2Str(int num) {
  //static char str[10]="";
  u8 str_temp[10];
  u8 i=0;
  if(num<0){
    str_temp[i++] = '-';
    num = 0 - num;
  }
  if(num<10){
    str_temp[i++] = num + '0';
    str_temp[i++] = 0;
  }
  else if(num<100){
    str_temp[i++] = num/10 + '0';
    str_temp[i++] = num%10 + '0';
    str_temp[i++] = 0;
  }
  else if(num<1000){
    str_temp[i++] = num/100       + '0';
    str_temp[i++] = num/10%10     + '0';
    str_temp[i++] = num%10        + '0';
    str_temp[i++] = 0;
  }
  else if(num<10000){
    str_temp[i++] = num/1000      + '0';
    str_temp[i++] = num/100%10    + '0';
    str_temp[i++] = num/10%10     + '0';
    str_temp[i++] = num%10        + '0';
    str_temp[i++] = 0;
  }
  else if(num<=32767){
    str_temp[i++] = num/10000      + '0';
    str_temp[i++] = num/1000%10    + '0';
    str_temp[i++] = num/100%10     + '0';
    str_temp[i++] = num/10%10     + '0';
    str_temp[i++] = num%10        + '0';
    str_temp[i++] = 0;
  }
  else{
    debug_send("打印数字不符合范围！");
  }
  
  //sprintf(str, "%d", num);
  PrintString_((char*)str_temp);
}


void get_value(unsigned char option) {
  unsigned int temp_value = 0;
  unsigned char tempaddr[3] = {ADXL362_REG_XDATA, ADXL362_REG_YDATA, ADXL362_REG_ZDATA};
  //unsigned char tempstr[3] = {0x78,0x79,0x7a};
  //unsigned char tempstr[3] = {0xf0,0xf1,0xf2};


  temp_value = ADXL362_ReadReg(tempaddr[option]);

  printNum2Str(temp_value);
}

void get_z(void) {
  unsigned char i;
  unsigned int temp_z = 0;
  
  Send_OneByte(ADXL362_READ_REG);
  Send_OneByte(ADXL362_REG_ZDATA);
  temp_z = Read_OneByte();
  for (i = 0; i < 2; i++)
  {
    CS_H();
    DelayT_ms(1);
    CS_L();
    Send_OneByte(ADXL362_READ_REG);
    Send_OneByte(ADXL362_REG_ZDATA);
    temp_z = ((temp_z + Read_OneByte()) >> 1);

    CS_H();
  }
  alarmer_uart3_send_byte(temp_z);
  //static char str[29]="";

  //sprintf(str,"%d",temp_z);
  //debug_send(str);
}

void printDebugInfo(ACCEL_AD16 tempaccelval) {
  //static char str[29]="";
  //float angle_x, angle_y, angle_z = 0;

  s8 angle_x = ADXL362_Get_Angle(tempaccelval.xadc, tempaccelval.yadc, tempaccelval.zadc, 1);
  s8 angle_y = ADXL362_Get_Angle(tempaccelval.xadc, tempaccelval.yadc, tempaccelval.zadc, 2);
  s8 angle_z = ADXL362_Get_Angle(tempaccelval.xadc, tempaccelval.yadc, tempaccelval.zadc, 0);


  /*sprintf(str, "angele X Y Z:   %-08.2f  %-08.2f  %-08.2f     x:%d y:%d z:%d", \
          angle_x, \
          angle_y, \
          angle_z, \
          tempaccelval.xadc, tempaccelval.yadc, tempaccelval.zadc);
  debug_send(str);*/
  sprintf(str, "%-d", angle_z);
  debug_send(str);
  //sprintf(str,"x:%d y:%d z:%d t:%d",tempaccelval.xadc,tempaccelval.yadc,tempaccelval.zadc,tempaccelval.tadc);
  //debug_send(str);
}


//x,y,z:x,y,z方向的重力加速度分量(不需要单位,直接数值即可)
//dir:要获得的角度.0,与Z轴的角度;1,与X轴的角度;2,与Y轴的角度.
//返回值:角度值.单位0.1°.
s8 ADXL362_Get_Angle(float x, float y, float z, u8 dir)
{
  float temp, res = 0;
  float tempres = 0;
  s8 char_temp_angle = 0;
  switch (dir)
  {
  case 0://与自然Z轴的角度
    temp = sqrt((x * x + y * y)) / z;
    res = atan(temp);
    break;
  case 1://与自然X轴的角度
    temp = x / sqrt((y * y + z * z));
    res = atan(temp);
    break;
  case 2://与自然Y轴的角度
    temp = y / sqrt((x * x + z * z));
    res = atan(temp);
    break;
  }
  //tempres =  res * 180 / 3.14;
  tempres =  res * 57.32;
  //return tempres;
  char_temp_angle = (s8)(tempres);
  return char_temp_angle;
}

// **************************************************************
// 函数:ADXL362_Init
// 参数:无
// 返回:>0 成功
// 描述:ADXL362加速度计初始化
// **************************************************************
unsigned char ADXL362_Init(void)
{
//  volatile unsigned char read;
//  static unsigned char lastpin=0xFF;
  //UsciB0_Init();
  //Smclk_Init(1);

  //初始化模块 所有寄存器清除 传感器处于待机状态 中断引脚被配置为高输出阻抗模式 由总线保持器保持为有效状态
  ADXL362_WriteReg(ADXL362_REG_SOFT_RESET, ADXL362_RESET_KEY);
  DelayT_ms(2000);

  //读取ADI公司器件ID 正确值应该为0xAD
  if (ADXL362_ReadReg(ADXL362_REG_DEVID_AD) != ADXL362_DEVICE_AD)
  {
    return 0;
  }
  //读取ADI公司MEMS器件ID 正确值应该为0x1D
  if (ADXL362_ReadReg(ADXL362_REG_DEVID_MST) != ADXL362_DEVICE_MST)
  {
    return 0;
  }
  //读取器件ID 正确值应该为0xF2
  if (ADXL362_ReadReg(ADXL362_REG_PARTID) != ADXL362_PART_ID)
  {
    return 0;
  }

  //ADXL362滤波器控制寄存器0x2c 复位值0x13
  ADXL362_WriteReg( ADXL362_REG_FILTER_CTL,                 \
                    ADXL362_FILTER_CTL_RANGE(ADXL362_RANGE_2G)        \
                    | ADXL362_FILTER_CTL_ODR(ADXL362_ODR_12_5_HZ)       \
                  );//20170106之前为400HZ 采样频率

  //INT1功能映射寄存器 复位0x00
  //INT1低电平有效
  //将唤醒状态映射到INT1引脚
  //ADXL362_WriteReg( ADXL362_REG_INTMAP1,ADXL362_INTMAP1_INT_LOW|ADXL362_INTMAP1_AWAKE);

  //INT1功能映射寄存器 复位0x00
  //INT1低电平有效
  //将唤醒状态映射到INT1引脚
  ADXL362_WriteReg( ADXL362_REG_INTMAP1, ADXL362_INTMAP1_INT_LOW | ADXL362_INTMAP1_INACT);

  //INT2功能映射寄存器 复位0x00
  //INT2低电平有效
  //将唤醒状态映射到INT2引脚
  ADXL362_WriteReg( ADXL362_REG_INTMAP2, ADXL362_INTMAP2_INT_LOW | ADXL362_INTMAP2_AWAKE);

  //设置运动阈值寄存器 复位0x00 设置为1023
  //ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_L, 50);//15度左右
  //ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_H, 0);

  ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_L, 50);//
  ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_H, 0);
  
  /*
  ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_L, 200);//15度左右
  ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_H, 0);
  */

  //设置运动时间寄存器  复位0x00  计算：ADXL362_REG_TIME_ACT/ODR 设置为？秒
  //ADXL362_WriteReg( ADXL362_REG_TIME_ACT, 250);//20170106之前 调试使用 配合400HZ采样频率
  
  //ADXL362_WriteReg( ADXL362_REG_TIME_ACT, 5);
  ADXL362_WriteReg( ADXL362_REG_TIME_ACT, 5);

  //设置静止阈值寄存器 复位0x00 设置为128
  ADXL362_WriteReg( ADXL362_REG_THRESH_INACT_L, 100);
  ADXL362_WriteReg( ADXL362_REG_THRESH_INACT_H, 0);
  //设置静止时间寄存器  复位0x00  计算：ADXL362_REG_TIME_INACT/ODR 设置为2秒
  
  ADXL362_WriteReg( ADXL362_REG_TIME_INACT_L, 36);
  ADXL362_WriteReg( ADXL362_REG_TIME_INACT_H, 0);
  
  /*
  ADXL362_WriteReg( ADXL362_REG_TIME_INACT_L, 0);
  ADXL362_WriteReg( ADXL362_REG_TIME_INACT_H, 1);//配合400HZ采样频率使用
  */
  
  //设置运动/静止寄存器
  //链接模式
  //静止检测功能以相对模式工作
  //使能静止(欠阈值)功能
  //运动检测功能以相对模式工作
  //使能运动(过阈值)功能
  ADXL362_WriteReg( ADXL362_REG_ACT_INACT_CTL,
                    ADXL362_ACT_INACT_CTL_LINKLOOP(ADXL362_MODE_LOOP) \
                    | ADXL362_ACT_INACT_CTL_INACT_REF       \
                    | ADXL362_ACT_INACT_CTL_INACT_EN        \
                    | ADXL362_ACT_INACT_CTL_ACT_REF       \
                    | ADXL362_ACT_INACT_CTL_ACT_EN        \
                  );


  //设置FIFO控制寄存器
  //过半清零
  //存储温度 没开
  //触发模式 ADXL362_FIFO_TRIGGERED 打开  流模式 ADXL362_FIFO_STREAM
  ADXL362_WriteReg(ADXL362_REG_FIFO_CTL,
                   ADXL362_FIFO_CTL_FIFO_MODE(ADXL362_FIFO_STREAM));

  //指定FIFO样本数量
  ADXL362_WriteReg(ADXL362_REG_FIFO_SAMPLES, 0);

  ADXL362_ReadReg(ADXL362_REG_FIFO_CTL);
  ADXL362_ReadReg(ADXL362_REG_FIFO_SAMPLES);



  //ADXL362电源控制寄存器 复位值0x00
  //正常工作噪声模式
  //测量模式
  //唤醒模式
  //自动休眠 要是能自动休眠 运动和精致检测必须处于链接模式或环路模式
  // ADXL362_WriteReg( ADXL362_REG_POWER_CTL,                  \
  //                   ADXL362_POWER_CTL_LOW_NOISE(ADXL362_NOISE_MODE_NORMAL)  \
  //                   | ADXL362_POWER_CTL_MEASURE(ADXL362_MEASURE_ON)     \
  //                   | ADXL362_POWER_CTL_WAKEUP                \
  //                   | ADXL362_POWER_CTL_AUTOSLEEP               \
  //                 );

  /*
  ADXL362_WriteReg( ADXL362_REG_POWER_CTL,                  \
                    ADXL362_POWER_CTL_LOW_NOISE(ADXL362_NOISE_MODE_NORMAL)  \
                    | ADXL362_POWER_CTL_MEASURE(ADXL362_MEASURE_ON)     \
                    | ADXL362_POWER_CTL_AUTOSLEEP               \
                  );
  */
  ADXL362_WriteReg( ADXL362_REG_POWER_CTL,                  \
                    ADXL362_POWER_CTL_LOW_NOISE(ADXL362_NOISE_MODE_NORMAL)  \
                    | ADXL362_POWER_CTL_MEASURE(ADXL362_MEASURE_ON)  \
                  );                
  DelayT_ms(200);
  //ADXL362_ReadAccel();
  return 1;
}


ACCEL_AD0 ADXL362_Get_Value(void) {
  ACCEL_AD0 temp_xyz_value;
  temp_xyz_value.xadc = ADXL362_ReadReg(ADXL362_REG_XDATA);
  temp_xyz_value.yadc = ADXL362_ReadReg(ADXL362_REG_YDATA);
  temp_xyz_value.zadc = ADXL362_ReadReg(ADXL362_REG_ZDATA);
  return temp_xyz_value;
}

s8 Get_Angle_Char(ACCEL_AD0 xyz, u8 dir)
{
  float temp, res = 0;
  s8 tempres = 0;
  switch (dir)
  {
  case 0://与自然Z轴的角度
    temp = sqrt((xyz.xadc * xyz.xadc + xyz.yadc * xyz.yadc)) / xyz.zadc;
    res = atan(temp);
    break;
  case 1://与自然X轴的角度
    temp = xyz.xadc / sqrt((xyz.yadc * xyz.yadc + xyz.zadc * xyz.zadc));
    res = atan(temp);
    break;
  case 2://与自然Y轴的角度
    temp = xyz.yadc / sqrt((xyz.xadc * xyz.xadc + xyz.zadc * xyz.zadc));
    res = atan(temp);
    break;
  }
  tempres =  (s8)(res * 180 / 3.14);
    if(xyz.zadc>=70){
    if(tempres<=3)
    {
      tempres=3;
    }
  }
  //20170518 BUG修复  翻盖后  角度为0 不能上报 已修正
  
  
  return tempres;
}

s8 ADXL362_Get_Angle_Char(s8 x, s8 y, s8 z, u8 dir)
{
  float temp, res = 0;
  s8 tempres = 0;
  switch (dir)
  {
  case 0://与自然Z轴的角度
    temp = sqrt((x * x + y * y)) / z;
    res = atan(temp);
    break;
  case 1://与自然X轴的角度
    temp = x / sqrt((y * y + z * z));
    res = atan(temp);
    break;
  case 2://与自然Y轴的角度
    temp = y / sqrt((x * x + z * z));
    res = atan(temp);
    break;
  }
  tempres =  (s8)(res * 180 / 3.14);
  return tempres;
}
#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
s8 get_angle_Z(ACCEL_AD0 zeropisition_xyz) 
{
  s8 temp_value_x, temp_value_y, temp_value_z = 0;
  ADXL_XYZ_ANGLE temp_xyz_angle;
  temp_value_x = ADXL362_ReadReg(ADXL362_REG_XDATA);
  temp_value_y = ADXL362_ReadReg(ADXL362_REG_YDATA);
  temp_value_z = ADXL362_ReadReg(ADXL362_REG_ZDATA);
  
  temp_xyz_angle.z_angle = ADXL362_Get_Angle_Char(temp_value_x, temp_value_y, temp_value_z, 0);
  
  //sprintf(str, " %-d", temp_xyz_angle.z_angle);
  //debug_send_no_end(str);
  return temp_xyz_angle.z_angle;
}

s8 get_angle_X(ACCEL_AD0 zeropisition_xyz) 
{
  s8 temp_value_x, temp_value_y, temp_value_z = 0;
  ADXL_XYZ_ANGLE temp_xyz_angle;
  temp_value_x = ADXL362_ReadReg(ADXL362_REG_XDATA);
  temp_value_y = ADXL362_ReadReg(ADXL362_REG_YDATA);
  temp_value_z = ADXL362_ReadReg(ADXL362_REG_ZDATA);

//  temp_xyz_angle.z_angle = ADXL362_Get_Angle_Char(temp_value_x, temp_value_y, temp_value_z, 0);
//
////sprintf(str, " %-d", temp_xyz_angle.z_angle);  //by hx 2017-06-13
////debug_send_no_end(str);
  
    temp_xyz_angle.z_angle = ADXL362_Get_Angle_Char(temp_value_x, temp_value_y, temp_value_z, 0);
    temp_xyz_angle.x_angle = ADXL362_Get_Angle_Char(temp_value_x, temp_value_y, temp_value_z, 1);
    temp_xyz_angle.y_angle = ADXL362_Get_Angle_Char(temp_value_x, temp_value_y, temp_value_z, 2);
//sprintf(str, " C%-d\r\n", temp_xyz_angle.z_angle);  //by hx 2017-06-13
//debug_send_no_end(str);
//
//sprintf(str, "B%-d\r\n", temp_xyz_angle.x_angle);  //by hx 2017-06-13
//debug_send_no_end(str);
//
//sprintf(str, "A%-d\r\n", temp_xyz_angle.y_angle);  //by hx 2017-06-13
//debug_send_no_end(str); 

//    sprintf(str, " %-d", temp_xyz_angle.z_angle);  //by hx 2017-06-13
//debug_send_no_end(str);

//sprintf(str, "B%-d\r\n", temp_xyz_angle.x_angle);  //by hx 2017-06-13
//debug_send_no_end(str);
//
//sprintf(str, "A%-d\r\n", temp_xyz_angle.y_angle);  //by hx 2017-06-13
//debug_send_no_end(str); 
  return temp_xyz_angle.x_angle;
}
#endif 


void SetMode2Measure(void) {
  ADXL362_WriteReg( ADXL362_REG_POWER_CTL,                  \
                    ADXL362_POWER_CTL_LOW_NOISE(ADXL362_NOISE_MODE_NORMAL)  \
                    | ADXL362_POWER_CTL_MEASURE(ADXL362_MEASURE_ON)     \
                  );
}
void SetMode2Idile(void) {
  ADXL362_WriteReg( ADXL362_REG_POWER_CTL,                  \
                    ADXL362_POWER_CTL_LOW_NOISE(ADXL362_NOISE_MODE_NORMAL)  \
                    | ADXL362_POWER_CTL_MEASURE(ADXL362_MEASURE_ON)     \
                    | ADXL362_POWER_CTL_AUTOSLEEP               \
                  );
}

//打印静态状态下的角度
void GetNewInactAngle_Fun(void){
  static s8 pre_temp_new_angle = 0;
  static s8 temp_new_angle = 0;
  static u8 get_sameAngle_Count = 0;
  static u8 retry_count = 0;
  //////////////////////////////////////////////////////主动查询 如果有触发  必须进去1 尝试查询
  if(sysFunState != 5 ){  //BUG修复20170828
    if ((ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE)){
      GetNewInactAngle_Fun_state = 1;//只要触发  必进入1
    }
  }
  ////////////////////////////////////////////
  switch(GetNewInactAngle_Fun_state){
  case 0:
    if(Analyse_data_para.inact_flag==0){//唤醒
      GetNewInactAngle_Fun_state ++;
      pre_temp_new_angle = 0;
      temp_new_angle = 0;
      get_sameAngle_Count = 0;
      retry_count = 0;
    }
    break;
  case 1:
    GetNewInactAngle_Fun_state ++;
    pre_temp_new_angle = 0;
    temp_new_angle = 0;
    get_sameAngle_Count = 0;
    retry_count = 0;
    break;
  case 2://初次唤醒后
    if(Analyse_data_para.inact_flag==1){//唤醒后，再次进去到静态
      GetNewInactAngle_Fun_state ++;
    }
    break;
  case 3:
    if (!(ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE)){
      pre_temp_new_angle = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z);
      PrintString("pre_temp_new_angle:");
      printNum2Str(pre_temp_new_angle);
      GetNewInactAngle_Fun_state ++;
    }
    break;
  case 4://重新回到静态后 只有在完全确认静态角度后 才能转变静态角度
    //每隔一段时间 取一个值 
    //只有在发送完成，并没有动作触发的情况下，才会取静态角度,不然的话，状态清零
    if((TashSchedule_State|sysFunState) && (sysFunState != 22) ){
      pre_temp_new_angle = 0;
      temp_new_angle = 0;
      get_sameAngle_Count = 0;
      retry_count = 0;
      break;
    }
    if(GetNewInactAngle_timecount>=100){
      temp_new_angle = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z);
      if(abs(temp_new_angle-pre_temp_new_angle)<=1){//角度差为1度
        printNumStr_(get_sameAngle_Count+1);
        PrintString(">>>Read Inact Agnle:");
        printNumStr_(temp_new_angle);
        if(++get_sameAngle_Count>=10){//连续10次 都取到一样的角度值 说明角度切换正常
          PrintString("!!!!!!!!!!!New Agnle:");
          printNumStr_(temp_new_angle);
          PrintString("!!!!!!!!");
          Analyse_data_para.zero_angle = temp_new_angle;
          
          
          pre_temp_new_angle = 0;
          temp_new_angle = 0;
          get_sameAngle_Count = 0;
          retry_count = 0;
          GetNewInactAngle_Fun_state ++;//取到角度值
        }
      }else{
        pre_temp_new_angle = temp_new_angle;
        PrintString("pre_temp_new_angle:");
        printNumStr(pre_temp_new_angle);
        
        get_sameAngle_Count = 0;//累积计数清零
        
        if(++retry_count>10){//在尝试10次 扔没有获取到静态角度的话 并且已经回到静态的情况下 跳转到静态（静态角度保持不变）
          
          pre_temp_new_angle = 0;
          temp_new_angle = 0;
          get_sameAngle_Count = 0;
          retry_count = 0;
          GetNewInactAngle_Fun_state ++;
        }
        PrintString("<<<<<<<<<<retry_count:");
        printNumStr(retry_count);
      }
      GetNewInactAngle_timecount = 0;
    }
    /*
    s8 pre_temp_new_angle = 0;
    s8 temp_new_angle = 0;
    
    for(u8 re_count = 0;re_count<10;re_count++){
    if (!(ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE)) {//查询静态标志
    pre_temp_new_angle = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z);
    for(u8 i = 0;i<30;i++){
    temp_new_angle = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z);
    PrintString("Read Inact Agnle:");
    printNum2Str(temp_new_angle);
    if(abs(temp_new_angle-pre_temp_new_angle))
  }
    
    Analyse_data_para.zero_angle = temp_new_angle;
  }else{
    GetNewInactAngle_Fun_state = 1;
  }
  }*/
    
    //GetNewInactAngle_Fun_state = 0;//在尝试10次 扔没有获取到静态角度的话 并且已经回到静态的情况下 跳转到静态（静态角度保持不变）
    //re_count = 0;
    break;
  case 5:
    GetNewInactAngle_Fun_state = 0;
//    if((!TashSchedule_State)&&(!sysFunState)&&(!(ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE))){
//      PrintString("<<<<<<<<<<<<<<<<<<<<<<<<<<<<< into lowpower_mode!!!");
//      PrintString_(">>>>>>>>>>>>>>>>>>>>>>>>>>>>");
//      Watchdog_Feed();
//      //GPIO_LowPower_Config(); 
//      
//      CLK_HaltConfig(CLK_Halt_FastWakeup,ENABLE);
//      GPIO_LowPower_Config(); 
//      PWR_FastWakeUpCmd(ENABLE);
//      PWR_UltraLowPowerCmd(ENABLE);  
//      RTC_ITConfig(RTC_IT_WUT, ENABLE);
//      RTC_WakeUpCmd(ENABLE);
//      Watchdog_Feed();
//      halt(); 
//    }
    if((!TashSchedule_State)&&(!sysFunState)&&(!(ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE))){

          IntoLowpowerMode();
    }
    break;
  default:
    GetNewInactAngle_Fun_state = 0;
    break;
  }
  
}


//s8 TempOffsetArray[2][10]={{-30,-20,-10,0,10,20,30,40,50,60},{0,0,0,0,1,2,3,5,6,6}};//-30 ~ 60
//
//u8 GetTemp(void) {
//  //u16 total_temp_data = 0;      
//  //u8 tempdata = 0;
//  /*for (u8 i = 0; i < 4; i++) {
//    //total_temp_data += (ADXL362_ReadReg(ADXL362_REG_TEMP_L) >> 4) + (ADXL362_ReadReg(ADXL362_REG_TEMP_H) << 4);
//    total_temp_data += ADXL362_ReadReg(ADXL362_REG_TEMP_L);
//    total_temp_data += ((ADXL362_ReadReg(ADXL362_REG_TEMP_H)&0x0f) << 8);
//  }
//  tempdata = (u8)((total_temp_data>>2)*0.05);*/
//  /*total_temp_data = ADXL362_ReadReg(ADXL362_REG_TEMP_L)+((ADXL362_ReadReg(ADXL362_REG_TEMP_H)&0x0f) << 4);
//  tempdata = (u8)(total_temp_data*0.05);
//  PrintString("tempdata:");
//  PrintU8int2String_(tempdata);
//  PrintString("before tempdata:");
//  PrintU16int2String_(total_temp_data);
//  return tempdata;*/
//  /***************************************************************************//**
// * @brief Reads the temperature of the device.
// *
// * @return tempCelsius - The value of the temperature(degrees Celsius).
//*******************************************************************************/
//    u8 rawTempData[2] = {0, 0};
//    short         signedTemp     = 0;
//   // float         tempCelsius    = 0;
//    float         tempCelsius1    = 0;
//    //u8 tempdata = 0;
//   // u8 tempdata1 = 0;
//    s8 temp_result = 0;
//    //s8 temp_offset_result = 0;
//    //ADXL362_GetRegisterValue(rawTempData, ADXL362_REG_TEMP_L, 2);
//    rawTempData[0] = ADXL362_ReadReg(ADXL362_REG_TEMP_L);
//    rawTempData[1] = ADXL362_ReadReg(ADXL362_REG_TEMP_H);
//    signedTemp = (short)((rawTempData[1] << 8) + rawTempData[0]);
//    
//    //PrintString("rawTempData[0]:");
//    //PrintU8int2String_(rawTempData[0]);
//    
//    //PrintString("rawTempData[1]:");
//    //PrintU8int2String_(rawTempData[1]);
//    
//    //  PrintString("signedTemp:");
//    //  sprintf(str, " %-d", signedTemp);
//    //debug_send(str);
//    
//    tempCelsius1 = (float)(signedTemp * 0.065);
//    
//    temp_result = ((s8)tempCelsius1)+25;
//     PrintString("========================>>>temp_result:");
//    //PrintU8int2String_(temp_result);
//    sprintf(str, " %-d", temp_result);
//    debug_send(str);
//    
//    // PrintString("tempCelsius1:");
//    //sprintf(str, " %-f", tempCelsius1);
//    //debug_send(str);
//    
//    
//    
//    /*
//    u8 i = 0;
//    for(i = 0;i<9;i++){
//    if(temp_result<TempOffsetArray[0][i])break;
//    }
//    temp_offset_result = temp_result+TempOffsetArray[1][i];
//    
//    PrintString("========================>>>temp_offset_result:");
//    //PrintU8int2String_(temp_result);
//    sprintf(str, " %-d", temp_offset_result);
//    debug_send(str);
//    */
//    
//    
//    
//    /*tempCelsius = (float)(signedTemp * 0.003);
//    tempdata = (u8)tempCelsius;
//    PrintString("tempdata:");
//    PrintU8int2String_(tempdata);
//    PrintString("tempCelsius:");
//    sprintf(str, " %-f", tempCelsius);
//    debug_send(str);*/
//    
//    
//    return temp_result;
//}

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
/***************************************************************************************
**函数名:       GetNewInactAngle
**功能描述:     打印静态状态下的角度。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-11-23,By Haixing.Huang
****************************************************************************************/
_Bool GetNewInactAngle(void) 
{
  //debug_send("ADXL362_REG_STATUS !");
  //printChar2bin(ADXL362_ReadReg(ADXL362_REG_STATUS));
  s8 temp_new_angle = 0;
  if (!(ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE)) 
  {
    PrintString("Z New Agnle:");  //by hx 2017-06-14
    temp_new_angle = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z);
    for(u8 i = 0;i<3;i++){
      temp_new_angle =((temp_new_angle>>1)+(Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z)>>1));
    }
    printNum2Str(temp_new_angle);
   Analyse_data_para.zero_angle = temp_new_angle;
   Analyse_data_para_XiaoFangZ.zero_angle = temp_new_angle;//by hx 2017-06-14
     
     
   temp_new_angle = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_X);
    for(u8 i = 0;i<3;i++){
      temp_new_angle =((temp_new_angle>>1)+(Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_X)>>1));
    }
    PrintString("X New Agnle:");
    printNum2Str(temp_new_angle);
    Analyse_data_para_XiaoFangX.zero_angle = temp_new_angle;    
    //printNum2Str(Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z));
     return 1;
  }
  return 0;
}


#endif 

