/***************************************************************************************
**�ļ���:     
**����:       Haixing.Huang
**�汾:       V1.00
**˵��:       ���ļ����Ը���ʵ������޸ġ�
**�޸ļ�¼:   �汾     �޸���    ʱ��                     
**						
**��ע:       2016/3/23
****************************************************************************************/
#ifndef   __INTERVAL_H__
#define   __INTERVAL_H__
//#include "Interval.h"
#include "config.h"
//#define INTERVAL_MODULE_EN (1) //ģ��ʹ��

#if  INTERVAL_MODULE_EN

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef INTERVAL_GLOBALS
#define INTERVAL_EXT
#else
#define INTERVAL_EXT extern
#endif
/*************************��������*************************/  
  
/*************************��������*************************/
INTERVAL_EXT void YY_Init(void);
INTERVAL_EXT void YY_Service(void);
#ifdef  __cplusplus
}
#endif
#endif
#endif
/************************************Code End******************************************/
