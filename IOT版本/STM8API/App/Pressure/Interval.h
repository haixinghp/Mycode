/***************************************************************************************
**文件名:     
**作者:       Haixing.Huang
**版本:       V1.00
**说明:       此文件可以根据实际情况修改。
**修改记录:   版本     修改者    时间                     
**						
**备注:       2016/3/23
****************************************************************************************/
#ifndef   __INTERVAL_H__
#define   __INTERVAL_H__
//#include "Interval.h"
#include "config.h"
//#define INTERVAL_MODULE_EN (1) //模块使能

#if  INTERVAL_MODULE_EN

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef INTERVAL_GLOBALS
#define INTERVAL_EXT
#else
#define INTERVAL_EXT extern
#endif
/*************************公共变量*************************/  
  
/*************************公共函数*************************/
INTERVAL_EXT void YY_Init(void);
INTERVAL_EXT void YY_Service(void);
#ifdef  __cplusplus
}
#endif
#endif
#endif
/************************************Code End******************************************/
