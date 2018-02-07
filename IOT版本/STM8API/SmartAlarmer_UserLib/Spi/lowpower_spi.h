/*
  ******************************************************************************
  * @file    delay.h
  * @author  Microcontroller Division
  * @version V0.0.1
  * @date    10/2016
  * @brief   spi functions header
  ******************************************************************************

  */
#ifndef __SMART_ALARMER_SPI_H
#define __SMART_ALARMER_SPI_H
/*    
#define CS_H()      //(GPIO_SetBits(GPIOB,GPIO_Pin_4))
#define CS_L()      //(GPIO_ResetBits(GPIOB,GPIO_Pin_4))
#define CLK_H()     //(GPIO_SetBits(GPIOB,GPIO_Pin_5))
#define CLK_L()     //(GPIO_ResetBits(GPIOB,GPIO_Pin_5))
#define DIO_H()     //(GPIO_SetBits(GPIOB,GPIO_Pin_6))
#define DIO_L()     //(GPIO_ResetBits(GPIOB,GPIO_Pin_6))
#define DIO_OUT()   //(GPIO_Init(GPIOB,GPIO_Pin_6,GPIO_Mode_Out_PP_High_Slow))
#define DIO_IN()    //(GPIO_Init(GPIOB,GPIO_Pin_7,GPIO_Mode_In_PU_No_IT))
#define DO_READ()   //(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))
*/
#define CS_H()      (GPIO_SetBits(GPIOB,GPIO_Pin_4))
#define CS_L()      (GPIO_ResetBits(GPIOB,GPIO_Pin_4))
#define CLK_H()     (GPIO_SetBits(GPIOB,GPIO_Pin_5))
#define CLK_L()     (GPIO_ResetBits(GPIOB,GPIO_Pin_5))
#define DIO_H()     (GPIO_SetBits(GPIOB,GPIO_Pin_6))
#define DIO_L()     (GPIO_ResetBits(GPIOB,GPIO_Pin_6))
#define DIO_OUT()   (GPIO_Init(GPIOB,GPIO_Pin_6,GPIO_Mode_Out_PP_High_Slow))
#define DIO_IN()    (GPIO_Init(GPIOB,GPIO_Pin_7,GPIO_Mode_In_PU_No_IT))
#define DO_READ()   (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7))

//#define	ADXL_CS_H()	CS_H()
//#define	ADXL_CS_L()	CS_L()

//#define    uint8    unsigned char
//#define    uint16    unsigned int
//#define    uchar    unsigned char
//#define    uint    unsigned int
//#define    uint32    unsigned long

//#define ADXL362_POWER_ON        GPIO_SetBits(GPIOE,GPIO_Pin_7);
//#define ADXL362_POWER_OFF       GPIO_ResetBits(GPIOE,GPIO_Pin_7);
#define ADXL362_POWER_ON        GPIO_SetBits(GPIOF,GPIO_Pin_4);
#define ADXL362_POWER_OFF       GPIO_ResetBits(GPIOF,GPIO_Pin_4);
    

#define ADXL362_WRITE_REG           0x0A
#define ADXL362_READ_REG            0x0B
#define ADXL362_READ_FIFO           0x0D

/* Registers */
#define ADXL362_REG_DEVID_AD            0x00
#define ADXL362_REG_DEVID_MST           0x01
#define ADXL362_REG_PARTID              0x02
#define ADXL362_REG_REVID               0x03
#define ADXL362_REG_XDATA               0x08
#define ADXL362_REG_YDATA               0x09
#define ADXL362_REG_ZDATA               0x0A
#define ADXL362_REG_STATUS              0x0B
#define ADXL362_REG_FIFO_L              0x0C
#define ADXL362_REG_FIFO_H              0x0D
#define ADXL362_REG_XDATA_L             0x0E
#define ADXL362_REG_XDATA_H             0x0F
#define ADXL362_REG_YDATA_L             0x10
#define ADXL362_REG_YDATA_H             0x11
#define ADXL362_REG_ZDATA_L             0x12
#define ADXL362_REG_ZDATA_H             0x13
#define ADXL362_REG_TEMP_L              0x14
#define ADXL362_REG_TEMP_H              0x15
#define ADXL362_REG_SOFT_RESET          0x1F
#define ADXL362_REG_THRESH_ACT_L        0x20
#define ADXL362_REG_THRESH_ACT_H        0x21
#define ADXL362_REG_TIME_ACT            0x22
#define ADXL362_REG_THRESH_INACT_L      0x23
#define ADXL362_REG_THRESH_INACT_H      0x24
#define ADXL362_REG_TIME_INACT_L        0x25
#define ADXL362_REG_TIME_INACT_H        0x26
#define ADXL362_REG_ACT_INACT_CTL       0x27
#define ADXL362_REG_FIFO_CTL            0x28
#define ADXL362_REG_FIFO_SAMPLES        0x29
#define ADXL362_REG_INTMAP1             0x2A
#define ADXL362_REG_INTMAP2             0x2B
#define ADXL362_REG_FILTER_CTL          0x2C
#define ADXL362_REG_POWER_CTL           0x2D
#define ADXL362_REG_SELF_TEST           0x2E

/* ADXL362_REG_STATUS definitions */
#define ADXL362_STATUS_ERR_USER_REGS        (1 << 7)
#define ADXL362_STATUS_AWAKE                (1 << 6)
#define ADXL362_STATUS_INACT                (1 << 5)
#define ADXL362_STATUS_ACT                  (1 << 4)
#define ADXL362_STATUS__OVERRUN             (1 << 3)
#define ADXL362_STATUS_FIFO_WATERMARK       (1 << 2)
#define ADXL362_STATUS_FIFO_RDY             (1 << 1)
#define ADXL362_STATUS_DATA_RDY             (1 << 0)

/* ADXL362_REG_ACT_INACT_CTL definitions */
#define ADXL362_ACT_INACT_CTL_LINKLOOP(x)   (((x) & 0x3) << 4)
#define ADXL362_ACT_INACT_CTL_INACT_REF     (1 << 3)
#define ADXL362_ACT_INACT_CTL_INACT_EN      (1 << 2)
#define ADXL362_ACT_INACT_CTL_ACT_REF       (1 << 1)
#define ADXL362_ACT_INACT_CTL_ACT_EN        (1 << 0)

/* ADXL362_ACT_INACT_CTL_LINKLOOP(x) options */
#define ADXL362_MODE_DEFAULT        0
#define ADXL362_MODE_LINK           1
#define ADXL362_MODE_LOOP           3

/* ADXL362_REG_FIFO_CTL */
#define ADXL362_FIFO_CTL_AH                 (1 << 3)
#define ADXL362_FIFO_CTL_FIFO_TEMP          (1 << 2)
#define ADXL362_FIFO_CTL_FIFO_MODE(x)       (((x) & 0x3) << 0)

/* ADXL362_FIFO_CTL_FIFO_MODE(x) options */
#define ADXL362_FIFO_DISABLE              0
#define ADXL362_FIFO_OLDEST_SAVED         1
#define ADXL362_FIFO_STREAM               2
#define ADXL362_FIFO_TRIGGERED            3

/* ADXL362_REG_INTMAP1 */
#define ADXL362_INTMAP1_INT_LOW             (1 << 7)
#define ADXL362_INTMAP1_AWAKE               (1 << 6)
#define ADXL362_INTMAP1_INACT               (1 << 5)
#define ADXL362_INTMAP1_ACT                 (1 << 4)
#define ADXL362_INTMAP1_FIFO_OVERRUN        (1 << 3)
#define ADXL362_INTMAP1_FIFO_WATERMARK      (1 << 2)
#define ADXL362_INTMAP1_FIFO_READY          (1 << 1)
#define ADXL362_INTMAP1_DATA_READY          (1 << 0)

/* ADXL362_REG_INTMAP2 definitions */
#define ADXL362_INTMAP2_INT_LOW             (1 << 7)
#define ADXL362_INTMAP2_AWAKE               (1 << 6)
#define ADXL362_INTMAP2_INACT               (1 << 5)
#define ADXL362_INTMAP2_ACT                 (1 << 4)
#define ADXL362_INTMAP2_FIFO_OVERRUN        (1 << 3)
#define ADXL362_INTMAP2_FIFO_WATERMARK      (1 << 2)
#define ADXL362_INTMAP2_FIFO_READY          (1 << 1)
#define ADXL362_INTMAP2_DATA_READY          (1 << 0)

/* ADXL362_REG_FILTER_CTL definitions */
#define ADXL362_FILTER_CTL_RANGE(x)         (((x) & 0x3) << 6)
#define ADXL362_FILTER_CTL_RES              (1 << 5)
#define ADXL362_FILTER_CTL_HALF_BW          (1 << 4)
#define ADXL362_FILTER_CTL_EXT_SAMPLE       (1 << 3)
#define ADXL362_FILTER_CTL_ODR(x)           (((x) & 0x7) << 0)

/* ADXL362_FILTER_CTL_RANGE(x) options */
#define ADXL362_RANGE_2G                0 /* +-2 g */
#define ADXL362_RANGE_4G                1 /* +-4 g */
#define ADXL362_RANGE_8G                2 /* +-8 g */

/* ADXL362_FILTER_CTL_ODR(x) options */
#define ADXL362_ODR_12_5_HZ             0 /* 12.5 Hz */
#define ADXL362_ODR_25_HZ               1 /* 25 Hz */
#define ADXL362_ODR_50_HZ               2 /* 50 Hz */
#define ADXL362_ODR_100_HZ              3 /* 100 Hz */
#define ADXL362_ODR_200_HZ              4 /* 200 Hz */
#define ADXL362_ODR_400_HZ              5 /* 400 Hz */

/* ADXL362_REG_POWER_CTL definitions */
#define ADXL362_POWER_CTL_RES               (1 << 7)
#define ADXL362_POWER_CTL_EXT_CLK           (1 << 6)
#define ADXL362_POWER_CTL_LOW_NOISE(x)      (((x) & 0x3) << 4)
#define ADXL362_POWER_CTL_WAKEUP            (1 << 3)
#define ADXL362_POWER_CTL_AUTOSLEEP         (1 << 2)
#define ADXL362_POWER_CTL_MEASURE(x)        (((x) & 0x3) << 0)

/* ADXL362_POWER_CTL_LOW_NOISE(x) options */
#define ADXL362_NOISE_MODE_NORMAL           0
#define ADXL362_NOISE_MODE_LOW              1
#define ADXL362_NOISE_MODE_ULTRALOW         2

/* ADXL362_POWER_CTL_MEASURE(x) options */
#define ADXL362_MEASURE_STANDBY         0
#define ADXL362_MEASURE_ON              2

/* ADXL362_REG_SELF_TEST */
#define ADXL362_SELF_TEST_ST            (1 << 0)

/* ADXL362 device information */
#define ADXL362_DEVICE_AD               0xAD
#define ADXL362_DEVICE_MST              0x1D
#define ADXL362_PART_ID                 0xF2

/* ADXL362 Reset settings */
#define ADXL362_RESET_KEY               0x52


#define VALUE_OF_X 1
#define VALUE_OF_Y 2
#define VALUE_OF_Z 0
#define VALUE_OF_A 3

#define MODIFY_DISABLE RESET
#define MODIFY_ENABLE SET


#define SetAngle 9


typedef struct _ACCEL_AD16_
{
  s16  xadc;
  s16  yadc;
  s16  zadc;
}ACCEL_AD16; 

    
typedef struct _ACCEL_AD0_
{
  s8  xadc;
  s8  yadc;
  s8  zadc;
}ACCEL_AD0; 

typedef	struct _ACCEL_AD_
{
	s8	xadc;
	s8	yadc;
	s8	zadc;
	s8	tadc;
}ACCEL_AD;    


typedef	struct _ADXL_XYZ_ANGLE_
{
    s8	x_angle;
    s8	y_angle;
    s8	z_angle;
}ADXL_XYZ_ANGLE;


typedef struct _SYS_XYZ_VALUE_
{
  ACCEL_AD0 value; //实际位置3轴值
  ACCEL_AD0 zero_value;//零点位置3轴值  d
  ACCEL_AD0 offset_value;//相对零点位置3轴值
}SYS_XYZ_VALUE;

typedef struct _SYS_XYZ_ANGLE_
{
  ADXL_XYZ_ANGLE angle; // 实际位置3轴角度
  ADXL_XYZ_ANGLE zero_angle;//零点位置3轴角度
  ADXL_XYZ_ANGLE offset_angle;//相对零点位置3轴角度
  ADXL_XYZ_ANGLE xyz_threshold_angle;//3轴阈值角度
}SYS_XYZ_ANGLE;

typedef struct _SYS_XYZ_DATA_
{
  FlagStatus ReadFlag;//读取标志
  FlagStatus ModifyFlag;//修正标志
  u8 TempValue;//温度值
  SYS_XYZ_VALUE xyz_value;//3轴值
  SYS_XYZ_ANGLE xyz_angle;//3轴角度
}SYS_XYZ_DATA;

/*
typedef struct _FIFO_ONE_WORD_
{
  u8 HighByte;
  u8 LowByte;
}FIFO_ONE_WORD;
*/
//#define MAX_BUF_NUM 512

/*
typedef struct _FIFO_BUF_INFO_//用来存储触发前的信息值 取出的FIFO数据 需要处理
{
  ACCEL_AD16 BufArray[170];//1080byte
  s8  z_angle[170];
  u16 BufLen;//2byte
}FIFO_BUF_INFO;//1082byte
*/
/*
typedef struct _SYS_BUF_INFO_//用来存储触发后 采样的信息值
{
  ACCEL_AD0 xyz_value[50];//150byte
  s8 z_angle[50];//50byte
  u8 len;//1byte
}SYS_BUF_INFO;//201byte
*/
 
//extern SYS_BUF_INFO SysBuf;
/*typedef struct _ACCEL_DATA_
{
  u8 ReadFlag;
  
  ADXL_XYZ_ANGLE Xyz_Angle;//实际位置角度值

}ACCEL_DATA;*/  

//extern SYS_XYZ_DATA SysXyzData;

//extern ADXL_XYZ_ANGLE Adxl_XYZ_Angle;
//extern SYS_XYZ_DATA adxl362TestData;    


extern u16 receive_len;
//extern FIFO_BUF_INFO FifoData;


void adxl362_spi_config();
void adxl362_config();
void adxl362_reset();
void adxl362_cmd_send(unsigned char);
void adxl362_lowpower();

void Send_OneByte(unsigned char);
unsigned char Read_OneByte(void);
void ADXL362_WriteReg(unsigned char,unsigned char);
unsigned char ADXL362_ReadReg(unsigned char);
ACCEL_AD16 ADXL362_ReadAccel(void);
unsigned char ADXL362_Init(void);
void printDebugInfo(ACCEL_AD16);
s8 ADXL362_Get_Angle(float,float,float,u8);

void ADXL362_Test(void);


s8 get_angle_Z(ACCEL_AD0 zeropisition_xyz);

ACCEL_AD0 ADXL362_Get_Value(void);
s8 ADXL362_Get_Angle_Char(s8,s8,s8,u8);
ADXL_XYZ_ANGLE get_value_XYZ(ACCEL_AD0,FlagStatus);
void get_value(u8);



void get_z(void);

//u16 GetLengthOfFifo(void);
//void GetDataFromFIFO(u8);


//extern ACCEL_AD0 FifoBufArr[128];
//extern u16 FifoBufLen;
//void TestFifoFun(void);//临时测试使用
//void Read_OneInfo_FIFO(void);
//extern ACCEL_AD0 TempFifoDataBuf;

//void printNum2bin(FIFO_ONE_WORD);
void printInt2bin(u16);
void printNumStr(s8 num);
void printNumStr_(s8 num);
void printUNum2Str(u16 num);
void printNum2Str(int num);


void GetDataFromFifobuf(void);
//void GetDataAndPrintInfo(void);


void SetMode2Measure(void);
void SetMode2Idile(void);

void PrintFifoBuf(void);//临时打印
void DetailFifobuf(void);
s8 Get_Angle_Char(ACCEL_AD0 , u8 );
//u8 GetTemp(void);


void FifoDataTrans2Angle(void);

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
_Bool GetNewInactAngle(void);
s8 get_angle_X(ACCEL_AD0 zeropisition_xyz);
#endif 
 
/*
void testPrint(void);
void setoffvalue(void);
void getAngle(void);
void setoffvalue(void);

*/
//char *itoa_temp(char *);
#endif

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
