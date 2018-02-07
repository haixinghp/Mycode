#ifndef __IIC_EEP_H
#define __IIC_EEP_H

#include "config.h"
//#include "iic_basic.h"
// #include "stm8l15x_gpio.h"

/****************************************************************************
*函数名:                                        
*功能  :                       
*输入  :                                          
*输出  :                                     
*调用  :         
*配置  :  
****************************************************************************/

#ifndef ENABLE_CONF_EEPROM

/******************************************************************************/
/*IIC格式读写eeprom 存储芯片IO定义*/
/******************************************************************************/
#define EEPROM_I2C_WR	0		/* D′????bit */
#define EEPROM_I2C_RD	1		/* ?á????bit */






/*存储芯片参数*/

#define 	IIC_READ   		0xa1      //定义读指令    
#define 	IIC_WRITE  		0xa0      //定义写指令 
/******************************************************************************/
/*存储芯片类型选择*/ 
#define 	EEP24CXX		1
#define		FM24CXX			2
//#define	   	MEMORYCHIP	    EEP24CXX//FM24CXX
//#define    	PageByte     	        32   	  //24c64   EEPROM页的大小
#define         PageByte                64        //24c256  EEPROM页的大小  


/******************************************************************************/
/*IIC接口GPIO口定义*/
/******************************************************************************/
#define I2C_GPIO_PORT	        GPIOC			/* GPIOC */
#define I2C_SCL_PIN		GPIO_Pin_1			/* á??óμ?SCLê±?ó??μ?GPIO */
#define I2C_SDA_PIN		GPIO_Pin_0			/* á??óμ?SDAêy?Y??μ?GPIO */
#define I2C_WP_PIN		GPIO_Pin_4			/* á??óμ?SDAêy?Y??μ?GPIO */

#define I2C_SCL_1()  GPIO_SetBits(I2C_GPIO_PORT, I2C_SCL_PIN)		/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SCL_PIN)		/* SCL = 0 */

#define I2C_SDA_1()  GPIO_SetBits(I2C_GPIO_PORT, I2C_SDA_PIN)		/* SDA = 1 */
#define I2C_SDA_0()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SDA_PIN)		/* SDA = 0 */
     
#define I2C_WP_1()   GPIO_SetBits(I2C_GPIO_PORT, I2C_WP_PIN)		/* WP = 1 */
#define I2C_WP_0()   GPIO_ResetBits(I2C_GPIO_PORT, I2C_WP_PIN)		/* WP = 0 */


#define I2C_SDA_READ()  GPIO_ReadInputDataBit(I2C_GPIO_PORT, I2C_SDA_PIN)	/* 读取sda口状态*/
/******************************************************************************/




#define   Start_EEP             0x01
#define   ADDR_EEP              Start_EEP    // 地址 
#define   DCCoeffic_EEP         ADDR_EEP+(sizeof(ModInfoStruct)+1)                      // 电压校正系数，电压4路
#define   DaSetInfo_EEP         DCCoeffic_EEP+(sizeof(DCVoltParaStruct)+1)*4            // 数据信息设置
#define   SynthePara_EEP        DaSetInfo_EEP+(sizeof(DaSetInfoStruct)+1)*MAXSetDaCNT   // 统一配置参数命令
#define   LongDaSetInfo_EEP     SynthePara_EEP+sizeof(SyntheticallyParaStruct)+2        // long型数据的配置，特殊型

  // SynthePara_EEP+sizeof(SyntheticallyParaStruct)+2  // 
//----N个表的地址分配------------
#define   Sheet41_EEP           LongDaSetInfo_EEP+(sizeof(LongDaSetInfoStruct)+1)*50          // 表81 采集次命令81的通道配置
#define   Sheet81_EEP           Sheet41_EEP+(sizeof(Sheet41InfoStruct)+1)          // 表81 采集次命令81的通道配置

#define  End_EEP               Sheet81_EEP+sizeof(Sheet41InfoStruct)+1
void GPIO_EEPROM_INIT(void);
extern uint8_t EEPROM_CheckOk(void);
extern uchar read_eep_byte(uint address);
extern void  write_eep_byte(uchar Data,uint address) ;
extern void write_multi_eep_byte_aa(uint TeAddr);
extern void write_multi_eep_byte_bb(uchar Data);
extern void write_multi_eep_byte_end(void);
extern uchar ChekAndReadEepromSum(uint address, uint cont,uchar *dat);
extern uchar WriteEeprom(uint address, uint cont, uchar *dat);
extern uchar WriteEepromNoChek(uint address, uint cont, uchar *dat);
extern void  CS_I2C_Stop();
uchar ReadEeprom(uint Adder,uint Lenth,uchar *Point);
void AT24C256_ALL_CLEAR(void);
void AT24C256_TO_Print(void);
/******************************************************************************/

#endif
#endif

