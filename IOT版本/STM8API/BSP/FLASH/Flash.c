//zhj
//20170120
//F2.1V1.0

#include "flash.h"
#include "iap.h"

void Flash_Config(u8 mode)
{
 // return ;
 // FLASH_DeInit();
  
  switch(mode)
  {
  case Flash_EEPROM_Unlock:
    disableInterrupts();
    /* Define flash programming Time*/
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    
    /* Unlock flash data eeprom memory */
    FLASH_Unlock(FLASH_MemType_Data);
    /* Wait until Data EEPROM area unlocked flag is set*/
    while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET)
    {}
    break;
    
  case Flash_EEPROM_Lock:
    FLASH_Lock(FLASH_MemType_Data);
    enableInterrupts();
    break;
    
  case Flash_FLASH_Unlock:
    disableInterrupts();
    /* Define flash programming Time*/
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    
    FLASH_Unlock(FLASH_MemType_Program);
    // Wait until Flash Program area unlocked flag is set
    while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
    {}
    break;
    
  case Flash_FLASH_Lock:
    FLASH_Lock(FLASH_MemType_Program);
    enableInterrupts();
    break;
    default:
          return ;
  break;
  }
return ;
}

u8 Flash_EEPROM_ReadByte(u32 add)
{
  u8 val=0;
  u32 address;
  address = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + add;
  if(address < FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS)
  {
    val = FLASH_ReadByte(address);
  }
  return val;
}

void Flash_EEPROM_WriteByte(u32 add,u8 data)
{
  u32 address;
  address = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + add;
  if(address < FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS)
  {
    FLASH_ProgramByte(address, data);
  }
}

u8 Flash_FLASH_ReadByte(u32 add)
{
  u8 val=0;
  
  val = FLASH_ReadByte(add);
  
  return val;
}

void Flash_FLASH_WriteByte(u32 add,u8 data)
{
  if((add >= FLASH_PROGRAM_START_PHYSICAL_ADDRESS) && (add <= FLASH_PROGRAM_END_PHYSICAL_ADDRESS))
  {
    FLASH_ProgramByte(add, data);
  }
}

u8 Flash_EEPROM_Test(void)
{
  u8 i;
  u8 flash_data[10];
  
  Flash_Config(Flash_EEPROM_Unlock);
  
  for(i=0;i<10;i++)
  {
    Flash_EEPROM_WriteByte(i,0);
  }
  for(i=0;i<10;i++)
  {
    flash_data[i] = Flash_EEPROM_ReadByte(i);
  }
  for(i=0;i<10;i++)
  {
    Flash_EEPROM_WriteByte(i,i);
  }
  for(i=0;i<10;i++)
  {
    flash_data[i] = Flash_EEPROM_ReadByte(i);
    if( flash_data[i] != i)
    {
      return Error;
    }
  }
  
  Flash_Config(Flash_EEPROM_Lock);
  
  for(i=0;i<10;i++)
  {
    Flash_EEPROM_WriteByte(i,0);
  }
  for(i=0;i<10;i++)
  {
    flash_data[i] = Flash_EEPROM_ReadByte(i);
  }
  
  return Right;
}

u8 Flash_FLASH_Test(void)
{
  u8 i;
  u8 flash_data[10];
  
  Flash_Config(Flash_FLASH_Unlock);
  
  for(i=0;i<10;i++)
  {
    Flash_FLASH_WriteByte(0x10000+i,0);
  }
  for(i=0;i<10;i++)
  {
    flash_data[i] = Flash_FLASH_ReadByte(0x10000+i);
  }
  for(i=0;i<10;i++)
  {
    Flash_FLASH_WriteByte(0x10000+i,i);
  }
  for(i=0;i<10;i++)
  {
    flash_data[i] = Flash_FLASH_ReadByte(0x10000+i);
    if( flash_data[i] != i)
    {
      return Error;
    }
  }
  
  Flash_Config(Flash_FLASH_Lock);
  
  for(i=0;i<10;i++)
  {
    Flash_FLASH_WriteByte(0x10000+i,0);
  }
  for(i=0;i<10;i++)
  {
    flash_data[i] = Flash_FLASH_ReadByte(0x10000+i);
  }
  
  return Right;
}
/*
u8 Flash_FLASH_Test(void)
{
  u8 i;
  u8 flash_data[10];
  
  Flash_Config(Flash_FLASH_Unlock);
  
  for(i=0;i<10;i++)
  {
    Flash_FLASH_WriteByte(iap_data_add+i,0);
  }
  for(i=0;i<10;i++)
  {
    flash_data[i] = Flash_FLASH_ReadByte(iap_data_add+i);
  }
  for(i=0;i<10;i++)
  {
    Flash_FLASH_WriteByte(iap_data_add+i,i);
  }
  for(i=0;i<10;i++)
  {
    flash_data[i] = Flash_FLASH_ReadByte(iap_data_add+i);
    if( flash_data[i] != i)
    {
      return Error;
    }
  }
  
  Flash_Config(Flash_FLASH_Lock);
  
  for(i=0;i<10;i++)
  {
    Flash_FLASH_WriteByte(iap_data_add+i,0);
  }
  for(i=0;i<10;i++)
  {
    flash_data[i] = Flash_FLASH_ReadByte(iap_data_add+i);
  }
  
  return Right;
}*/