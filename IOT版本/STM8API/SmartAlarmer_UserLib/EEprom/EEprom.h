#ifndef __EEPROM_H
#define __EEPROM_H

#define EEPROM_STARTADDR 0x1000 
#define EEPROM_WRITE_FLAG_A 0X55
#define EEPROM_WRITE_FLAG_B 0XBB
extern u8 EEPROM_Read(u16 Addr,u8 *RxBuffer,u8 Length);
extern u8 EEPROM_Write(u16 Addr,u8 *RxBuffer,u8 Lenth);


//��ȡϵͳ�ؼ�����
void Read_sys_keydata(void);
//�洢ϵͳ�ؼ�����
void Write_sys_keydata(void);
//�ָ�Ĭ�ϲ���
void Init_sys_keydata(void);
//�洢�����ܴ���
void Write_sys_keydata_totalcount(void);

#define SYS_KEYDATA_ADDR 2  //�ӵڶ�����ʼ���
//#define SYS_KEYDATA_SEND_TOTALCOUNT_ADDR_OFFSET	25
//#define SYS_KEYDATA_SEND_TOTALCOUNT_ADDR_OFFSET	27	
#define SYS_KEYDATA_SEND_TOTALCOUNT_ADDR_OFFSET	21	
#endif