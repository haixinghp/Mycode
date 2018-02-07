#ifndef GPIO_MAP_H
#define GPIO_MAP_H

/******************************************************************************/
/*GPIO�ڳ�ʼ������*/
/******************************************************************************/
#define _MG(PORT,INDEX)	       P##PORT##INDEX
#define _MGIN(PORT,INDEX)	   P##PORT##INDEX##in

#define  MG(PORT,INDEX,XXX)	   _MG(PORT,INDEX)
#define  MGIN(PORT,INDEX,XXX)  _MGIN(PORT,INDEX)

//void Gpio_Init(uint32_t RCC_APB2Periph,    
//			   GPIO_TypeDef * GPIO_Port,  
//			   uint16_t GPIO_Pin,         
//			   GPIOMode_TypeDef GPIO_Mode, 
//			   GPIOSpeed_TypeDef GPIO_Speed );
//void GPIO_Init(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, GPIO_Mode_TypeDef GPIO_Mode);



#define  _GPIO_INIT(PORT,INDEX,SPEED,MODE)    Gpio_Init(RCC_APB2Periph_GPIO##PORT ,\
                                              GPIO##PORT,INDEX, \
                                                  GPIO_Mode_##MODE, \
                                                  GPIO_Speed_##SPEED##MHz)
#define  _GPIO_INITS(PORT,SHILFT,INITVALUE,SPEED,MODE)  _GPIO_INIT(PORT,1<<SHILFT,SPEED,MODE); \
								MG(PORT,SHILFT,XXX)= INITVALUE;
#define  GPIO_INITS(PORTSHILFT,SPEED,MODE)	  _GPIO_INITS(PORTSHILFT,SPEED,MODE)

/*����ڶ���*/
#define  PINO(P)	MG(P)
#define  PINOUT(N)      
/*����ڶ���*/
#define  PINI(P)	MGIN(P)
/******************************************************************************/ 
/*IO�ڶ���     ����1:IO�˿� ����2:���� ����3:��ʼ��״̬*/
/*ע��: �ڱ༭���ж˿ڶ���ʱ��Ҫ�ڶ���ǰ��ʹ�ÿո�*/
/******************************************************************************/
#if	 BUILD_PROJECT == BOOT_HD

	#define		PIN_LED			E,0,0      /*LED*/	
#endif	


/*���������ų�ʼ��*/
	#define   _GPIO_LED_INIT() 	 GPIO_Init(GPIOE, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Slow);
/******************************************************************************/	
#endif


