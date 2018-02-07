#include "stm8l15x.h"

#define RELOAD_VALUE 254
void watchdog_init(void)
{
IWDG_Enable(); 
/* Enable write access to IWDG_PR and IWDG_RLR registers */
IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
/* Set IWDG timeout */
IWDG_SetPrescaler(IWDG_Prescaler_256);
IWDG_SetReload(RELOAD_VALUE);
/* Refresh IWDG */
IWDG_ReloadCounter();
}


//初始化看门狗
void Watchdog_INIT(void)
{
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable(); 
  
  /* IWDG timeout equal to 214 ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG configuration: IWDG is clocked by LSI = 38KHz */
  IWDG_SetPrescaler(IWDG_Prescaler_256);
  
  /* IWDG timeout equal to 214.7 ms (the timeout may varies due to LSI frequency dispersion) */
  /* IWDG timeout = (RELOAD_VALUE + 1) * Prescaler / LSI 
                  = (254 + 1) * 32 / 38 000 
                  = 214.7 ms */
  IWDG_SetReload((u8)RELOAD_VALUE);//(RELOAD_VALUE+1)*IWDG_Prescaler_256/LSI
  
  /* Reload IWDG counter */
  IWDG_ReloadCounter(); 	
} 


//喂狗 
void Watchdog_Feed(void)
{ 	  	
   /* Reload IWDG counter */
  IWDG_ReloadCounter();
} 