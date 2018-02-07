#ifndef __FLASH_H
#define __FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
enum
{
	Error = 0,
	Right = 1,
};
enum
{
  Flash_EEPROM_Unlock = 1,
  Flash_EEPROM_Lock = 2,
  Flash_FLASH_Unlock = 3,
  Flash_FLASH_Lock = 4,
};
  
extern void Flash_Config(u8 mode);
extern u8 Flash_EEPROM_ReadByte(u32 add);
extern void Flash_EEPROM_WriteByte(u32 add,u8 data);
extern u8 Flash_FLASH_ReadByte(u32 add);
extern void Flash_FLASH_WriteByte(u32 add,u8 data);
extern u8 Flash_EEPROM_Test(void);
extern u8 Flash_FLASH_Test(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
