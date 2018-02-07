#include "LowPrcisionTime.h"

/*��ʱ1MS������ÿ1MS��1*/
__IO uint16_t Timer50msCount = 0;	
/*��ʱ������ͷ*/
TimerNode* _gTimerLinkerHeader = NULL;

//������������������������������������������������������������������������������
// ��������  : LowPrcisionTime_Init
// ��������  : ��ʼ����ʱ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void LowPrcisionTime_Init(void)	
{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);			//ʹ�� TIM5 ��ʱ��
//	TIM_DeInit(TIM5);
//	
//	TIM_TimeBaseStructure.TIM_Period		=	1000;		 		//���ӳ�ʹ�õ�ʱ���ڽ������� ,1msʱ��
//	TIM_TimeBaseStructure.TIM_Prescaler		=	71;		  			//36Mhz/(35+1)=1Mhz ��λ1us ?? ��Ȼ��Ҫ71����
//	TIM_TimeBaseStructure.TIM_ClockDivision =	TIM_CKD_DIV1; 
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	  	//���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
//	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);	
//	TIM_ARRPreloadConfig(TIM5, DISABLE);						 	//ֱ������ARPE = 0	  ,����������
//	TIM_Cmd(TIM5, DISABLE);										 	//�رն�ʱ��
//	
//	
//	// ʹ��TIM5��ʱ�ж�
//	NVIC_InitStructure.NVIC_IRQChannel =  TIM5_IRQn;  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	
  
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);  //ʹ��TIM4ʱ��
    TIM4_ARRPreloadConfig(ENABLE);         //ʹ���Զ�����Ӱ�ӼĴ���
   //TIM4_TimeBaseInit(TIM4_Prescaler_64,CLK_GetClockFreq()/128/1000-1);  //TIM4 64��Ƶ���ж�Ƶ��1000Hz  
    TIM4_TimeBaseInit(TIM4_Prescaler_128,249);

    TIM4_Cmd(ENABLE);                      //����TIM4��ʱ��                  
    TIM4_ITConfig(TIM4_IT_Update,ENABLE);  //��TIM4����ж�   
}

//������������������������������������������������������������������������������
// ��������  : TIM4_IRQHandler
// ��������  : ��ʱ��1�����жϷ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void TIM4_IRQHandler(void)
{
//	if (TIM4_GetITStatus(TIM_IT_Update) == SET)
//	{		
//		 TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//		 Timer50msCount++;
//	} 
  disableInterrupts();
        Timer50msCount++;

  TIM4_ClearFlag(TIM4_FLAG_Trigger);
    TIM4->SR1 = 0;//
  //while (1);
  enableInterrupts();
}

//������������������������������������������������������������������������������
// ��������  : LowPrcisionTime_Start
// ��������  : ������ʱ��
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void LowPrcisionTime_Start(void)
{
    /*������ʱ��*/
    TIM4_Cmd(ENABLE); 
}	

//������������������������������������������������������������������������������
// ��������  : CheckNode
// ��������  : ��鶨ʱ���Ƿ����
// �������  : timerType,Ψһ��ʱ������
// �������  : None
// ���ز���  : ���ڷ���pNode �����ڷ���NULL
//������������������������������������������������������������������������������
TimerNode* CheckNode(TIMER_TYPE timerType)
{
	TimerNode* 	pNode =   _gTimerLinkerHeader;
	while(pNode != NULL)
	{
		if(pNode->timeType == timerType)
			return pNode;
		pNode = pNode->next;
	}

	return NULL;
}

//������������������������������������������������������������������������������
// ��������  : SetTimer
// ��������  : ����һ���µĶ�ʱ��������Ѿ�����������¶�ʱ������
// �������  : time ��ʱ��ʱ��; 
//             circleCount ѭ������, =-1ʱ�ظ�ѭ��;
//             timerType ��ʱ������;	
//             TimerEvent ����ѭ��ִ�лص�����; 
//             CircleEvent ѭ������ִ�лص�����;
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int SetTimer(int time,int circleCount,TIMER_TYPE timerType,TimerEvent TEvent,TimerEvent CircleEvent)
{
	TimerNode* pNode = NULL;
	if((pNode =CheckNode(timerType)) == NULL)
	{
		pNode = (TimerNode*)malloc(sizeof(TimerNode));
		memset(pNode,NULL,sizeof(TimerNode));
		if(_gTimerLinkerHeader == NULL) 
		{
			_gTimerLinkerHeader =  pNode;		
		} 
		else 
		{
			TimerNode* pN =   _gTimerLinkerHeader;
			while(pN->next != NULL) 
			{
				pN = pN->next;
			}
			pN->next = 	pNode;
			
		}
	}
	 	
	pNode->time = time;
	pNode->curTime = time;
	pNode->circleCount =  circleCount;
	pNode->timeType =  timerType;
	pNode->TimerEvent = TEvent;	
	pNode->CircleEvent = CircleEvent;	
	
	return 0;	
}

//������������������������������������������������������������������������������
// ��������  : DelTimer
// ��������  : ɾ��һ����ʱ��
// �������  : timerType Ψһ��ʱ������
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
int DelTimer(TIMER_TYPE timerType)
{
	TimerNode* 	pNode =   _gTimerLinkerHeader;
	if(pNode == NULL)
		return -1;
	if(pNode->timeType == timerType)	 //����ͷ���ж�
	{
		_gTimerLinkerHeader =  pNode->next;		
		free(pNode);
		return 0;
	}
	while(pNode->next != NULL)
	{
		if(pNode->next->timeType == timerType)
		{
			TimerNode* 	pN =  pNode->next;
		   	pNode->next = pN->next;
			if(pN->CircleEvent != NULL)
			{
				pN->CircleEvent();
			}
			free(pN);
			return 0;	
		}
			
		pNode = pNode->next;
	}
	return -1;
}

//������������������������������������������������������������������������������
// ��������  : DoTimerCallBack
// ��������  : ����ִ�лص�����
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
static void DoTimerCallBack(void (*TimerEvent)(void))
{
	 if(TimerEvent != NULL)
	 {
	 	  TimerEvent();
	 }
}

//������������������������������������������������������������������������������
// ��������  : DoTimer
// ��������  : ��ʱ������
// �������  : None
// �������  : None
// ���ز���  : None
//������������������������������������������������������������������������������
void DoTimer(void)
{
	TimerNode* 	pNode;

	while(Timer50msCount)
	{	
		Timer50msCount--;

		pNode =  _gTimerLinkerHeader;
		while(pNode != NULL)
		{
			TimerNode* pN =  pNode->next;

			if(pNode->curTime > 0)
			{
				  pNode->curTime--;
			} 
			else 
			{
				pNode->curTime = pNode->time;
				if(pNode->circleCount > 0)	        /*���޴�������*/
				{ 		 
					pNode->circleCount--;
					DoTimerCallBack(pNode->TimerEvent);
				} 
				else if(pNode->circleCount == -1) /*��Զ������ȥ*/
				{
					 DoTimerCallBack(pNode->TimerEvent);
				} 
				else			/*��������*/
				{
				  	 DelTimer(pNode->timeType);
				}
			}

			pNode =  pN;
		}

	}

}

//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������
