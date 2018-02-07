#ifndef __RCT_H
#define __RCT_H
void CLK_Config(void);
void RTC_Config(void);
void RTC_Config_4_secondquery(u16 interval_second_count);

extern int32_t GetTick(void);
extern u8 GetTime(int32_t tick);
#endif