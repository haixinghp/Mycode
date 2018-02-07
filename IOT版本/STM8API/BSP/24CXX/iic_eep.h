#ifndef __IIC_EEP_H
#define __IIC_EEP_H

#include "config.h"
//#include "iic_basic.h"
// #include "stm8l15x_gpio.h"

/****************************************************************************
*������:                                        
*����  :                       
*����  :                                          
*���  :                                     
*����  :         
*����  :  
****************************************************************************/

#ifndef ENABLE_CONF_EEPROM

/******************************************************************************/
/*IIC��ʽ��дeeprom �洢оƬIO����*/
/******************************************************************************/
#define EEPROM_I2C_WR	0		/* D��????bit */
#define EEPROM_I2C_RD	1		/* ?��????bit */






/*�洢оƬ����*/

#define 	IIC_READ   		0xa1      //�����ָ��    
#define 	IIC_WRITE  		0xa0      //����дָ�� 
/******************************************************************************/
/*�洢оƬ����ѡ��*/ 
#define 	EEP24CXX		1
#define		FM24CXX			2
//#define	   	MEMORYCHIP	    EEP24CXX//FM24CXX
//#define    	PageByte     	        32   	  //24c64   EEPROMҳ�Ĵ�С
#define         PageByte                64        //24c256  EEPROMҳ�Ĵ�С  


/******************************************************************************/
/*IIC�ӿ�GPIO�ڶ���*/
/******************************************************************************/
#define I2C_GPIO_PORT	        GPIOC			/* GPIOC */
#define I2C_SCL_PIN		GPIO_Pin_1			/* ��??����?SCL����?��??��?GPIO */
#define I2C_SDA_PIN		GPIO_Pin_0			/* ��??����?SDA��y?Y??��?GPIO */
#define I2C_WP_PIN		GPIO_Pin_4			/* ��??����?SDA��y?Y??��?GPIO */

#define I2C_SCL_1()  GPIO_SetBits(I2C_GPIO_PORT, I2C_SCL_PIN)		/* SCL = 1 */
#define I2C_SCL_0()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SCL_PIN)		/* SCL = 0 */

#define I2C_SDA_1()  GPIO_SetBits(I2C_GPIO_PORT, I2C_SDA_PIN)		/* SDA = 1 */
#define I2C_SDA_0()  GPIO_ResetBits(I2C_GPIO_PORT, I2C_SDA_PIN)		/* SDA = 0 */
     
#define I2C_WP_1()   GPIO_SetBits(I2C_GPIO_PORT, I2C_WP_PIN)		/* WP = 1 */
#define I2C_WP_0()   GPIO_ResetBits(I2C_GPIO_PORT, I2C_WP_PIN)		/* WP = 0 */


#define I2C_SDA_READ()  GPIO_ReadInputDataBit(I2C_GPIO_PORT, I2C_SDA_PIN)	/* ��ȡsda��״̬*/
/******************************************************************************/




#define   Start_EEP             0x01
#define   ADDR_EEP              Start_EEP    // ��ַ 
#define   DCCoeffic_EEP         ADDR_EEP+(sizeof(ModInfoStruct)+1)                      // ��ѹУ��ϵ������ѹ4·
#define   DaSetInfo_EEP         DCCoeffic_EEP+(sizeof(DCVoltParaStruct)+1)*4            // ������Ϣ����
#define   SynthePara_EEP        DaSetInfo_EEP+(sizeof(DaSetInfoStruct)+1)*MAXSetDaCNT   // ͳһ���ò�������
#define   LongDaSetInfo_EEP     SynthePara_EEP+sizeof(SyntheticallyParaStruct)+2        // long�����ݵ����ã�������

  // SynthePara_EEP+sizeof(SyntheticallyParaStruct)+2  // 
//----N����ĵ�ַ����------------
#define   Sheet41_EEP           LongDaSetInfo_EEP+(sizeof(LongDaSetInfoStruct)+1)*50          // ��81 �ɼ�������81��ͨ������
#define   Sheet81_EEP           Sheet41_EEP+(sizeof(Sheet41InfoStruct)+1)          // ��81 �ɼ�������81��ͨ������

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

