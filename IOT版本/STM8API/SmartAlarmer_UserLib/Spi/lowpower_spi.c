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

SYS_XYZ_DATA SysXyzData;//ϵͳ״ֵ̬
//FIFO_ONE_WORD U16FifoWordArr[MAX_BUF_NUM];//1024byte
//SYS_BUF_INFO SysBuf;//ϵͳ���ݻ����� 201byte
//FIFO_BUF_INFO FifoData;//FIFO���ݻ����� //1082byte

u16 receive_len = 0;
u16 max_len = 0;
u16 detail_len = 0;
u16 idx = 0;
u8 temp_state_before = 0;
u8 temp_state_after = 0;
u16 temp_data = 0;
void adxl362_io_config() { //�˿�����
  //GPIO_Init(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_High_Slow);
  //GPIO_Init(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  //GPIO_Init(GPIOB,GPIO_Pin_3 | GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);
  GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_1  | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_Out_PP_Low_Slow);
  GPIO_Init(GPIOB, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);


  GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_High_Slow);//��Դ�� ����Ϊ����� ��ADXL��Դ
  GPIO_SetBits(GPIOE, GPIO_Pin_7); //ADXL��Դ��

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
// ����:adxl362_spi_config
// ����:��
// ����:��
// ����:SPI��ʼ��
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
    PrintString_("�����ʼ���ɹ�!");
  } else {
    //debug_send("ADXL362 init failed!");
    PrintString_("�����ʼ��ʧ�ܣ�ϵͳ��������!");
    sys_restart();
  }
  CS_L();
  CLK_L();
  DIO_L();
}

// **************************************************************
// ����:Send_OneByte
// ����:Value
// ����:��
// ����:SPIдһ�ֽ�����
// **************************************************************


void Send_OneByte(unsigned char DATA8) //��SPI��8λ��
{ //������д��
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
// ����:Read_OneByte
// ����:��
// ����:Value
// ����:SPI��һ�ֽ�����
// **************************************************************
unsigned char Read_OneByte(void)     //��SPI��8λ��
{ //�½������
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
// ����:ADXL362_WriteReg
// ����:Reg Address,Value
// ����:��
// ����:дADXL362�Ĵ���
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
// ����:ADXL362_ReadReg
// ����:Reg Addres
// ����:Reg Value
// ����:��ADXL362�Ĵ���
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
    debug_send("��ӡ���ֲ����Ϸ�Χ��");
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
    debug_send("��ӡ���ֲ����Ϸ�Χ��");
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
    debug_send("��ӡ���ֲ����Ϸ�Χ��");
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


//x,y,z:x,y,z������������ٶȷ���(����Ҫ��λ,ֱ����ֵ����)
//dir:Ҫ��õĽǶ�.0,��Z��ĽǶ�;1,��X��ĽǶ�;2,��Y��ĽǶ�.
//����ֵ:�Ƕ�ֵ.��λ0.1��.
s8 ADXL362_Get_Angle(float x, float y, float z, u8 dir)
{
  float temp, res = 0;
  float tempres = 0;
  s8 char_temp_angle = 0;
  switch (dir)
  {
  case 0://����ȻZ��ĽǶ�
    temp = sqrt((x * x + y * y)) / z;
    res = atan(temp);
    break;
  case 1://����ȻX��ĽǶ�
    temp = x / sqrt((y * y + z * z));
    res = atan(temp);
    break;
  case 2://����ȻY��ĽǶ�
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
// ����:ADXL362_Init
// ����:��
// ����:>0 �ɹ�
// ����:ADXL362���ٶȼƳ�ʼ��
// **************************************************************
unsigned char ADXL362_Init(void)
{
//  volatile unsigned char read;
//  static unsigned char lastpin=0xFF;
  //UsciB0_Init();
  //Smclk_Init(1);

  //��ʼ��ģ�� ���мĴ������ ���������ڴ���״̬ �ж����ű�����Ϊ������迹ģʽ �����߱���������Ϊ��Ч״̬
  ADXL362_WriteReg(ADXL362_REG_SOFT_RESET, ADXL362_RESET_KEY);
  DelayT_ms(2000);

  //��ȡADI��˾����ID ��ȷֵӦ��Ϊ0xAD
  if (ADXL362_ReadReg(ADXL362_REG_DEVID_AD) != ADXL362_DEVICE_AD)
  {
    return 0;
  }
  //��ȡADI��˾MEMS����ID ��ȷֵӦ��Ϊ0x1D
  if (ADXL362_ReadReg(ADXL362_REG_DEVID_MST) != ADXL362_DEVICE_MST)
  {
    return 0;
  }
  //��ȡ����ID ��ȷֵӦ��Ϊ0xF2
  if (ADXL362_ReadReg(ADXL362_REG_PARTID) != ADXL362_PART_ID)
  {
    return 0;
  }

  //ADXL362�˲������ƼĴ���0x2c ��λֵ0x13
  ADXL362_WriteReg( ADXL362_REG_FILTER_CTL,                 \
                    ADXL362_FILTER_CTL_RANGE(ADXL362_RANGE_2G)        \
                    | ADXL362_FILTER_CTL_ODR(ADXL362_ODR_12_5_HZ)       \
                  );//20170106֮ǰΪ400HZ ����Ƶ��

  //INT1����ӳ��Ĵ��� ��λ0x00
  //INT1�͵�ƽ��Ч
  //������״̬ӳ�䵽INT1����
  //ADXL362_WriteReg( ADXL362_REG_INTMAP1,ADXL362_INTMAP1_INT_LOW|ADXL362_INTMAP1_AWAKE);

  //INT1����ӳ��Ĵ��� ��λ0x00
  //INT1�͵�ƽ��Ч
  //������״̬ӳ�䵽INT1����
  ADXL362_WriteReg( ADXL362_REG_INTMAP1, ADXL362_INTMAP1_INT_LOW | ADXL362_INTMAP1_INACT);

  //INT2����ӳ��Ĵ��� ��λ0x00
  //INT2�͵�ƽ��Ч
  //������״̬ӳ�䵽INT2����
  ADXL362_WriteReg( ADXL362_REG_INTMAP2, ADXL362_INTMAP2_INT_LOW | ADXL362_INTMAP2_AWAKE);

  //�����˶���ֵ�Ĵ��� ��λ0x00 ����Ϊ1023
  //ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_L, 50);//15������
  //ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_H, 0);

  ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_L, 50);//
  ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_H, 0);
  
  /*
  ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_L, 200);//15������
  ADXL362_WriteReg( ADXL362_REG_THRESH_ACT_H, 0);
  */

  //�����˶�ʱ��Ĵ���  ��λ0x00  ���㣺ADXL362_REG_TIME_ACT/ODR ����Ϊ����
  //ADXL362_WriteReg( ADXL362_REG_TIME_ACT, 250);//20170106֮ǰ ����ʹ�� ���400HZ����Ƶ��
  
  //ADXL362_WriteReg( ADXL362_REG_TIME_ACT, 5);
  ADXL362_WriteReg( ADXL362_REG_TIME_ACT, 5);

  //���þ�ֹ��ֵ�Ĵ��� ��λ0x00 ����Ϊ128
  ADXL362_WriteReg( ADXL362_REG_THRESH_INACT_L, 100);
  ADXL362_WriteReg( ADXL362_REG_THRESH_INACT_H, 0);
  //���þ�ֹʱ��Ĵ���  ��λ0x00  ���㣺ADXL362_REG_TIME_INACT/ODR ����Ϊ2��
  
  ADXL362_WriteReg( ADXL362_REG_TIME_INACT_L, 36);
  ADXL362_WriteReg( ADXL362_REG_TIME_INACT_H, 0);
  
  /*
  ADXL362_WriteReg( ADXL362_REG_TIME_INACT_L, 0);
  ADXL362_WriteReg( ADXL362_REG_TIME_INACT_H, 1);//���400HZ����Ƶ��ʹ��
  */
  
  //�����˶�/��ֹ�Ĵ���
  //����ģʽ
  //��ֹ��⹦�������ģʽ����
  //ʹ�ܾ�ֹ(Ƿ��ֵ)����
  //�˶���⹦�������ģʽ����
  //ʹ���˶�(����ֵ)����
  ADXL362_WriteReg( ADXL362_REG_ACT_INACT_CTL,
                    ADXL362_ACT_INACT_CTL_LINKLOOP(ADXL362_MODE_LOOP) \
                    | ADXL362_ACT_INACT_CTL_INACT_REF       \
                    | ADXL362_ACT_INACT_CTL_INACT_EN        \
                    | ADXL362_ACT_INACT_CTL_ACT_REF       \
                    | ADXL362_ACT_INACT_CTL_ACT_EN        \
                  );


  //����FIFO���ƼĴ���
  //��������
  //�洢�¶� û��
  //����ģʽ ADXL362_FIFO_TRIGGERED ��  ��ģʽ ADXL362_FIFO_STREAM
  ADXL362_WriteReg(ADXL362_REG_FIFO_CTL,
                   ADXL362_FIFO_CTL_FIFO_MODE(ADXL362_FIFO_STREAM));

  //ָ��FIFO��������
  ADXL362_WriteReg(ADXL362_REG_FIFO_SAMPLES, 0);

  ADXL362_ReadReg(ADXL362_REG_FIFO_CTL);
  ADXL362_ReadReg(ADXL362_REG_FIFO_SAMPLES);



  //ADXL362��Դ���ƼĴ��� ��λֵ0x00
  //������������ģʽ
  //����ģʽ
  //����ģʽ
  //�Զ����� Ҫ�����Զ����� �˶��;��¼����봦������ģʽ��·ģʽ
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
  case 0://����ȻZ��ĽǶ�
    temp = sqrt((xyz.xadc * xyz.xadc + xyz.yadc * xyz.yadc)) / xyz.zadc;
    res = atan(temp);
    break;
  case 1://����ȻX��ĽǶ�
    temp = xyz.xadc / sqrt((xyz.yadc * xyz.yadc + xyz.zadc * xyz.zadc));
    res = atan(temp);
    break;
  case 2://����ȻY��ĽǶ�
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
  //20170518 BUG�޸�  ���Ǻ�  �Ƕ�Ϊ0 �����ϱ� ������
  
  
  return tempres;
}

s8 ADXL362_Get_Angle_Char(s8 x, s8 y, s8 z, u8 dir)
{
  float temp, res = 0;
  s8 tempres = 0;
  switch (dir)
  {
  case 0://����ȻZ��ĽǶ�
    temp = sqrt((x * x + y * y)) / z;
    res = atan(temp);
    break;
  case 1://����ȻX��ĽǶ�
    temp = x / sqrt((y * y + z * z));
    res = atan(temp);
    break;
  case 2://����ȻY��ĽǶ�
    temp = y / sqrt((x * x + z * z));
    res = atan(temp);
    break;
  }
  tempres =  (s8)(res * 180 / 3.14);
  return tempres;
}
#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
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

//��ӡ��̬״̬�µĽǶ�
void GetNewInactAngle_Fun(void){
  static s8 pre_temp_new_angle = 0;
  static s8 temp_new_angle = 0;
  static u8 get_sameAngle_Count = 0;
  static u8 retry_count = 0;
  //////////////////////////////////////////////////////������ѯ ����д���  �����ȥ1 ���Բ�ѯ
  if(sysFunState != 5 ){  //BUG�޸�20170828
    if ((ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE)){
      GetNewInactAngle_Fun_state = 1;//ֻҪ����  �ؽ���1
    }
  }
  ////////////////////////////////////////////
  switch(GetNewInactAngle_Fun_state){
  case 0:
    if(Analyse_data_para.inact_flag==0){//����
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
  case 2://���λ��Ѻ�
    if(Analyse_data_para.inact_flag==1){//���Ѻ��ٴν�ȥ����̬
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
  case 4://���»ص���̬�� ֻ������ȫȷ�Ͼ�̬�ǶȺ� ����ת�侲̬�Ƕ�
    //ÿ��һ��ʱ�� ȡһ��ֵ 
    //ֻ���ڷ�����ɣ���û�ж�������������£��Ż�ȡ��̬�Ƕ�,��Ȼ�Ļ���״̬����
    if((TashSchedule_State|sysFunState) && (sysFunState != 22) ){
      pre_temp_new_angle = 0;
      temp_new_angle = 0;
      get_sameAngle_Count = 0;
      retry_count = 0;
      break;
    }
    if(GetNewInactAngle_timecount>=100){
      temp_new_angle = Get_Angle_Char(ADXL362_Get_Value(), VALUE_OF_Z);
      if(abs(temp_new_angle-pre_temp_new_angle)<=1){//�ǶȲ�Ϊ1��
        printNumStr_(get_sameAngle_Count+1);
        PrintString(">>>Read Inact Agnle:");
        printNumStr_(temp_new_angle);
        if(++get_sameAngle_Count>=10){//����10�� ��ȡ��һ���ĽǶ�ֵ ˵���Ƕ��л�����
          PrintString("!!!!!!!!!!!New Agnle:");
          printNumStr_(temp_new_angle);
          PrintString("!!!!!!!!");
          Analyse_data_para.zero_angle = temp_new_angle;
          
          
          pre_temp_new_angle = 0;
          temp_new_angle = 0;
          get_sameAngle_Count = 0;
          retry_count = 0;
          GetNewInactAngle_Fun_state ++;//ȡ���Ƕ�ֵ
        }
      }else{
        pre_temp_new_angle = temp_new_angle;
        PrintString("pre_temp_new_angle:");
        printNumStr(pre_temp_new_angle);
        
        get_sameAngle_Count = 0;//�ۻ���������
        
        if(++retry_count>10){//�ڳ���10�� ��û�л�ȡ����̬�ǶȵĻ� �����Ѿ��ص���̬������� ��ת����̬����̬�Ƕȱ��ֲ��䣩
          
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
    if (!(ADXL362_ReadReg(ADXL362_REG_STATUS)&ADXL362_STATUS_AWAKE)) {//��ѯ��̬��־
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
    
    //GetNewInactAngle_Fun_state = 0;//�ڳ���10�� ��û�л�ȡ����̬�ǶȵĻ� �����Ѿ��ص���̬������� ��ת����̬����̬�Ƕȱ��ֲ��䣩
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

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
/***************************************************************************************
**������:       GetNewInactAngle
**��������:     ��ӡ��̬״̬�µĽǶȡ�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-11-23,By Haixing.Huang
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

