#ifndef _LOWPRCIISIONTIME_H
#define _LOWPRCIISIONTIME_H
#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "stm8l15x.h"

/*******************************************************************************/
/*��ʱ������ڵ�ṹ*/
typedef  struct Node	{
  struct Node * next;  			/*��һ���ڵ�*/
  int time;		  				/*������ʱ��*/
  int curTime;					/*����ʱ�������*/
  int circleCount;				/*ѭ������*/
  TIMER_TYPE timeType;			/*ʱ�䴥������*/
  void (*TimerEvent)(void);		/*�ص�����*/
  void (*CircleEvent)(void);    /*ѭ�������ص�����*/	
}TimerNode;
/*******************************************************************************/
/*��������*/
void LowPrcisionTime_Init(void);
void LowPrcisionTime_Start(void);
TimerNode* CheckNode(TIMER_TYPE timerType);
int SetTimer(int time,int circleCount,TIMER_TYPE timerType,TimerEvent TEvent,TimerEvent CircleEvent);
int DelTimer(TIMER_TYPE timerType);
int SetTimerFast(int time,int circleCount,TIMER_TYPE timerType,TimerEvent TEvent,TimerEvent CircleEvent);
int DelTimerFast(TIMER_TYPE timerType);

void DoTimer(void);
void TIM4_IRQHandler(void);
/*******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������

