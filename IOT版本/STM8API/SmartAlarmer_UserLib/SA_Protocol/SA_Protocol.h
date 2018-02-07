#ifndef _SA_PROTOCOL_H_
#define _SA_PROTOCOL_H_


#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
#define CONFIG_BEGIN_LEN                        (1) 
#define CONFIG_PROTOCOL_VER_LEN                 (2) 
#define CONFIG_DEVICE_ADD_LEN                   (2) 
#define CONFIG_DEVICE_TYPE_LEN                  (2) 
#define CONFIG_CMD_LEN                          (2) 
#define CONFIG_LEN_LEN                          (4) 

#define CONFIG_CHANNEL_COUNT_LEN                (4) //ͨ������	UINT��	�ַ������� 15
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C            //19
#define CONFIG_BIT_FLAG_LEN                     (8) //ͨ����Ч��־	Long��	��Ӧ��Bitx�����Ƿ���Ч
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
#define CONFIG_ID_LEN                           (32)//ID��	�ַ���	16λ 
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
#define CONFIG_SOFT_VER_LEN                     (4) //����汾	�ַ���	2λ���硰10�� 
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
#define CONFIG_HARDWARE_VER_LEN	                (4)//Ӳ���汾	�ַ���	2λ���硰10�� 
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C    //69
#define CONFIG_STATUS_LEN	                (2)//�豸״̬	Uchar	���״̬ ����״̬ ��װ״̬ ����״̬ 
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C      73
#define CONFIG_SEND_TOTALCOUNT_LEN              (4)//Bit4	���ʹ���	UINT	0-65535   
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     79
#define CONFIG_ROTATION_TRIGGER_ANGLE_LEN       (2)//��ת�仯�Ƕ�	UINT	-360~360    	
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C      85
#define CONFIG_ROTATION_TRIGGER_SENS_LEN        (2)//��ת������	Uchar	0-99      
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     89
#define CONFIG_LEAN_TRIGGER_ANGLE_LEN           (2)//��б�仯�Ƕ�	Uchar	-90~90 	
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     93
#define CONFIG_LEAN_TRIGGER_SENS_LEN	        (2)//��б������	Uchar	0-99       
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     97
#define CONFIG_PRESSURE_TRIGGER_VALUE_LEN       (4)//ѹ���仯��ֵ	UINT	1-65535 
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     93
#define CONFIG_PRESSURE_INTERVAL_LEN            (4)//ѹ�������	UINT	0-65535  
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     99 
#define CONFIG_HEART_INTERVAL_LEN               (4)//�������ʱ��	UINT	0-65535ʱ  
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     105
#define CONFIG_ALARM_INTERVAL_LEN               (4)//�澯���ʱ��	UINT	0-65535��  
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     110
#define CONFIG_TRIGGER_INTERVAL_LEN             (4)//�����ʱ��	UINT	0-65535��
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     116
#define CONFIG_CLOUD_SERVER_ADD_LEN             (42)//����ɼ���ַ	�ַ���	�21    
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     160
#define CONFIG_UPDATE_SERVER_LEN                (42)//������������ַ	�ַ���	�21    
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     204
#define CONFIG_CARRIER_TYPE_LEN                 (2)//��Ӫ��	Uchar	0���� 1�ƶ� 2��ͨ  
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     
#define CONFIG_MODULE_TYPE_LEN                  (2)//������ʽ	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C   
#define CONFIG_CARD_ID_LEN                      (40)//���������š�20
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
#define CONFIG_TIMESTAMP_LEN                    (8)//ʱ��� 4 1970��01��01��8��0��0�뵽���ڵ���������1498010100 
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
#define CONFIG_PRESSURE_UPLIMIT_LEN             (4)//ѹ������	UINT	0~65535
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
#define CONFIG_PRESSURE_DOWNLIMIT_LEN          (4)//ѹ������	UINT	0~65535
//�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C

//CHKSUM 4
#define CONFIG_CHKSUM_LEN                        (4)
//EOI 1
#define CONFIG_EOI_LEN                           (1)


#define CONFIG_0X2C (2)

#define CONFIG_BEGIN_ADDR	                (0)     

#define CONFIG_PROTOCOL_VER_ADDR	        (CONFIG_BEGIN_LEN)  //

#define CONFIG_DEVICE_ADD_ADDR		        (CONFIG_PROTOCOL_VER_LEN+CONFIG_PROTOCOL_VER_ADDR)

#define CONFIG_DEVICE_TYPE_ADDR		        (CONFIG_DEVICE_ADD_LEN+CONFIG_DEVICE_ADD_ADDR)

#define CONFIG_CMD_ADDR			        (CONFIG_DEVICE_TYPE_LEN+CONFIG_DEVICE_TYPE_ADDR)

#define CONFIG_LEN_ADDR 		        (CONFIG_CMD_LEN+CONFIG_CMD_ADDR)

#define CONFIG_CHANNEL_COUNT_ADDR 	        (CONFIG_LEN_LEN+CONFIG_LEN_ADDR)  //ͨ������	UINT��	�ַ������� ��ַ13

#define CONFIG_BIT_FLAG_ADDR		        (CONFIG_CHANNEL_COUNT_LEN+CONFIG_CHANNEL_COUNT_ADDR+CONFIG_0X2C)  //ͨ����Ч��־	Long��	��Ӧ��Bitx�����Ƿ���Ч ��ַ19

#define CONFIG_ID_ADDR 			        (CONFIG_BIT_FLAG_LEN+CONFIG_BIT_FLAG_ADDR+CONFIG_0X2C)  //ID��	�ַ���	16λ ��ַ29

#define CONFIG_SOFT_VER_ADDR		        (CONFIG_ID_LEN+CONFIG_ID_ADDR+CONFIG_0X2C)  //����汾	�ַ���	2λ���硰10����ַ63

#define CONFIG_HARDWARE_VER_ADDR	        (CONFIG_SOFT_VER_LEN+CONFIG_SOFT_VER_ADDR+CONFIG_0X2C)  //Ӳ���汾	�ַ���	2λ���硰10��

#define CONFIG_STATUS_ADDR	                (CONFIG_HARDWARE_VER_LEN+CONFIG_HARDWARE_VER_ADDR+CONFIG_0X2C)//�豸״̬	Uchar	���״̬ ����״̬ ��װ״̬ ����״̬ �̼����µȴ�״̬

#define CONFIG_SEND_TOTALCOUNT_ADDR             (CONFIG_STATUS_LEN+CONFIG_STATUS_ADDR+CONFIG_0X2C)//Bit4	���ʹ���	UINT	0-65535

#define CONFIG_ROTATION_TRIGGER_ANGLE_ADDR	(CONFIG_SEND_TOTALCOUNT_LEN+CONFIG_SEND_TOTALCOUNT_ADDR+CONFIG_0X2C)//��ת�仯�Ƕ�	UINT	-360~360

#define CONFIG_ROTATION_TRIGGER_SENS_ADDR	(CONFIG_ROTATION_TRIGGER_ANGLE_LEN+CONFIG_ROTATION_TRIGGER_ANGLE_ADDR+CONFIG_0X2C)//��ת������	Uchar	0-99

#define CONFIG_LEAN_TRIGGER_ANGLE_ADDR          (CONFIG_ROTATION_TRIGGER_SENS_LEN+CONFIG_ROTATION_TRIGGER_SENS_ADDR+CONFIG_0X2C)//��б�仯�Ƕ�	Uchar	-90~90��

#define CONFIG_LEAN_TRIGGER_SENS_ADDR	        (CONFIG_LEAN_TRIGGER_ANGLE_LEN+CONFIG_LEAN_TRIGGER_ANGLE_ADDR+CONFIG_0X2C)//��б������	Uchar	0-99

#define CONFIG_PRESSURE_TRIGGER_VALUE_ADDR	(CONFIG_LEAN_TRIGGER_SENS_LEN+CONFIG_LEAN_TRIGGER_SENS_ADDR+CONFIG_0X2C)//ѹ���仯��ֵ	UINT	1-65535

#define CONFIG_PRESSURE_INTERVAL_ADDR	        (CONFIG_PRESSURE_TRIGGER_VALUE_LEN+CONFIG_PRESSURE_TRIGGER_VALUE_ADDR+CONFIG_0X2C)//ѹ�������	UINT	0-65535��

#define CONFIG_HEART_INTERVAL_ADDR	        (CONFIG_PRESSURE_INTERVAL_LEN+CONFIG_PRESSURE_INTERVAL_ADDR+CONFIG_0X2C)//�������ʱ��	UINT	0-65535ʱ

#define CONFIG_ALARM_INTERVAL_ADDR	        (CONFIG_HEART_INTERVAL_LEN+CONFIG_HEART_INTERVAL_ADDR+CONFIG_0X2C)//�澯���ʱ��	UINT	0-65535��

#define CONFIG_TRIGGER_INTERVAL_ADDR		(CONFIG_ALARM_INTERVAL_LEN+CONFIG_ALARM_INTERVAL_ADDR+CONFIG_0X2C)//�����ʱ��	UINT	0-65535��

#define CONFIG_CLOUD_SERVER_ADD_ADDR		(CONFIG_TRIGGER_INTERVAL_LEN+CONFIG_TRIGGER_INTERVAL_ADDR+CONFIG_0X2C)//����ɼ���ַ	�ַ���	�21

#define CONFIG_UPDATE_SERVER_ADDR 		(CONFIG_CLOUD_SERVER_ADD_LEN+CONFIG_CLOUD_SERVER_ADD_ADDR+CONFIG_0X2C)//������������ַ	�ַ���	�21

#define CONFIG_CARRIER_TYPE_ADDR 		(CONFIG_UPDATE_SERVER_LEN+CONFIG_UPDATE_SERVER_ADDR+CONFIG_0X2C)//��Ӫ��	Uchar	0���� 1�ƶ� 2��ͨ

#define CONFIG_MODULE_TYPE_ADDR 		(CONFIG_CARRIER_TYPE_LEN+CONFIG_CARRIER_TYPE_ADDR+CONFIG_0X2C)//������ʽ	Uchar	0:2G 1:3G 2:4G3:NB-IOT

#define CONFIG_CARD_ID_ADDR                     (CONFIG_MODULE_TYPE_LEN+CONFIG_MODULE_TYPE_ADDR+CONFIG_0X2C)  //���������š�                     

#define CONFIG_TIMESTAMP_ADDR                   (CONFIG_CARD_ID_LEN+CONFIG_CARD_ID_ADDR+CONFIG_0X2C)  //ʱ��� 4 1970��01��01��8��0��0�뵽���ڵ���������1498010100 

#define CONFIG_PRESSURE_UPLIMIT_ADDR            (CONFIG_TIMESTAMP_LEN+CONFIG_TIMESTAMP_ADDR+CONFIG_0X2C) //ѹ������	UINT	0~65535

#define CONFIG_PRESSURE_DOWNLIMIT_ADDR          (CONFIG_PRESSURE_UPLIMIT_LEN+CONFIG_PRESSURE_UPLIMIT_ADDR+CONFIG_0X2C) //ѹ������	UINT	0~65535

                               

#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
#define CONFIG_DATAINFO_LEN                     (CONFIG_PRESSURE_DOWNLIMIT_LEN+CONFIG_PRESSURE_DOWNLIMIT_ADDR+CONFIG_0X2C) //DATAINFO���ȡ�
#define CONFIG_LEN                              (CONFIG_DATAINFO_LEN+CONFIG_CHKSUM_LEN+CONFIG_EOI_LEN) //ͨѶ���������ܳ��ȡ�����˨276
#endif

#if APP_SELECT_COVER    //һ�´�����ˮѹ�������еĴ���
#define CONFIG_DATAINFO_LEN                     ((CONFIG_CARD_ID_LEN+CONFIG_CARD_ID_ADDR+CONFIG_0X2C)) //DATAINFO���ȡ� 
#define CONFIG_LEN                              (CONFIG_DATAINFO_LEN+CONFIG_CHKSUM_LEN+CONFIG_EOI_LEN) //ͨѶ���������ܳ��ȡ�����˨276
#endif

#define PROTOCOL_CONFIG_TOTAL_LEN (CONFIG_LEN)

#define BIT_1   ((u32)1)
#define BIT_CONFIG_ID 			              	(BIT_1<<0)  //ID��	�ַ���	16λ                     
#define BIT_CONFIG_SOFT_VER 		          	(BIT_1<<1)  //����汾	�ַ���	2λ���硰10��                    
#define BIT_CONFIG_HARDWARE_VER 	        	(BIT_1<<2)  //Ӳ���汾	�ַ���	2λ���硰10��              
#define BIT_CONFIG_STATUS 	              	        (BIT_1<<3)  //�豸״̬	Uchar	���״̬ ����״̬ ��װ״̬ ����״̬ 
#define BIT_CONFIG_SEND_TOTALCOUNT        	        (BIT_1<<4)  //Bit4	���ʹ���	UINT	0-65535           
#define BIT_CONFIG_ROTATION_TRIGGER_ANGLE 	        (BIT_1<<5)  //��ת�仯�Ƕ�	UINT	-360~360    
#define BIT_CONFIG_ROTATION_TRIGGER_SENS 		(BIT_1<<6)  //��ת������	Uchar	0-99      
#define BIT_CONFIG_LEAN_TRIGGER_ANGLE    	        (BIT_1<<7)  //��б�仯�Ƕ�	Uchar	-90~90
#define BIT_CONFIG_LEAN_TRIGGER_SENS 	    	        (BIT_1<<8)  //��б������	Uchar	0-99        
#define BIT_CONFIG_PRESSURE_TRIGGER_VALUE 	        (BIT_1<<9)  //ѹ���仯��ֵ	UINT	1-65535       
#define BIT_CONFIG_PRESSURE_INTERVAL 	    	        (BIT_1<<10) //ѹ�������	UINT	0-65535
#define BIT_CONFIG_HEART_INTERVAL 	      	        (BIT_1<<11) //�������ʱ��	UINT	0-65535ʱ    
#define BIT_CONFIG_ALARM_INTERVAL 	      	        (BIT_1<<12) //�澯���ʱ��	UINT	0-65535��        
#define BIT_CONFIG_TRIGGER_INTERVAL 		  	(BIT_1<<13) //�����ʱ��	UINT	0-65535��        
#define BIT_CONFIG_CLOUD_SERVER_ADD 		  	(BIT_1<<14) //����ɼ���ַ	�ַ���	�21            
#define BIT_CONFIG_UPDATE_SERVER 		    	(BIT_1<<15) //������������ַ	�ַ���	�21        
#define BIT_CONFIG_CARRIER_TYPE 		        (BIT_1<<16) //��Ӫ��	Uchar	0���� 1�ƶ� 2��ͨ           
#define BIT_CONFIG_MODULE_TYPE 		          	(BIT_1<<17) //������ʽ	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
#define BIT_CONFIG_CARD_ID                              (BIT_1<<18) //���������š�20  
#define BIT_TIMESTAMP_ID                                (BIT_1<<19) //ʱ��� 4 1970��01��01��8��0��0�뵽���ڵ���������1498010100  
#define BIT_PRESSURE_UPLIMIT_ID                         (BIT_1<<20) //ѹ������    UINT    0~65535
#define BIT_PRESSURE_DOWNLIMIT_ID                       (BIT_1<<21) //ѹ������    UINT    0~65535
#endif


#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
#define CONFIG_BEGIN_LEN                        (1) 
#define CONFIG_PROTOCOL_VER_LEN                 (2) 
#define CONFIG_DEVICE_ADD_LEN                   (2) 
#define CONFIG_DEVICE_TYPE_LEN                  (2) 
#define CONFIG_CMD_LEN                          (2) 
#define CONFIG_LEN_LEN                          (4) 
//CHKSUM 4
#define CONFIG_CHKSUM_LEN                        (4)
//EOI 1
#define CONFIG_EOI_LEN                           (1)
#define CONFIG_0X2C (2)

//#define CONFIG_BEGIN_ADDR	                (0)     
//#define CONFIG_PROTOCOL_VER_ADDR	        (CONFIG_BEGIN_LEN)  //
//#define CONFIG_DEVICE_ADD_ADDR		        (CONFIG_PROTOCOL_VER_LEN+CONFIG_PROTOCOL_VER_ADDR)
//#define CONFIG_DEVICE_TYPE_ADDR		        (CONFIG_DEVICE_ADD_LEN+CONFIG_DEVICE_ADD_ADDR)
//#define CONFIG_CMD_ADDR			        (CONFIG_DEVICE_TYPE_LEN+CONFIG_DEVICE_TYPE_ADDR)
//#define CONFIG_LEN_ADDR 		        (CONFIG_CMD_LEN+CONFIG_CMD_ADDR)
//#define CONFIG_CHANNEL_COUNT_ADDR 	        (CONFIG_LEN_LEN+CONFIG_LEN_ADDR)  //ͨ������	UINT��	�ַ������� 
//#define CONFIG_BIT_FLAG_ADDR		        (CONFIG_CHANNEL_COUNT_ADDR+8+CONFIG_0X2C)  //ͨ����Ч��־	Long��	��Ӧ��Bitx�����Ƿ���Ч 
//#define CONFIG_ID_ADDR 			        (CONFIG_BIT_FLAG_ADDR+32+CONFIG_0X2C)  //ID��	�ַ���	16λ 
//#define CONFIG_SOFT_VER_ADDR                    (CONFIG_ID_ADDR+4+CONFIG_0X2C)  //����汾    �ַ���    2λ���硰10��
//#define CONFIG_HARDWARE_VER_ADDR                (CONFIG_SOFT_VER_ADDR+4+CONFIG_0X2C)  //Ӳ���汾    �ַ���    2λ���硰10��
//#define CONFIG_STATUS_ADDR                      (CONFIG_HARDWARE_VER_ADDR+2+CONFIG_0X2C)//�豸״̬    Uchar    ���״̬ ����״̬ ��װ״̬ ����״̬ �̼����µȴ�״̬
//#define CONFIG_SEND_TOTALCOUNT_ADDR             (CONFIG_STATUS_ADDR+4+CONFIG_0X2C)//Bit4    ���ʹ���    UINT    0-65535
//#define CONFIG_PRESSURE_TRIGGER_VALUE_ADDR      (CONFIG_SEND_TOTALCOUNT_ADDR+4+CONFIG_0X2C)//ѹ���仯�ֱ���    UINT    1-65535
//#define CONFIG_HEART_INTERVAL_ADDR              (CONFIG_PRESSURE_TRIGGER_VALUE_ADDR+4+CONFIG_0X2C)//�������ʱ��    UINT    0-65535ʱ
//#define CONFIG_ALARM_ENABLE_BIT_ADDR            (CONFIG_HEART_INTERVAL_ADDR+4+CONFIG_0X2C) ////�澯ʹ��0ʧ�ܣ�1ʹ�� UINT
//#define CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL_ADDR       (CONFIG_ALARM_ENABLE_BIT_ADDR+4+CONFIG_0X2C) ///���޸澯���ʱ�� UINT
//#define CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL_ADDR    (CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //���޸澯���ʱ�� UINT
//#define CONFIG_NO_SENSOR_ALARM_INTERVAL_ADDR              (CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //δ��⵽�������澯��� UINT
//#define CONFIG_RESOLUTION_ALARM_INTERVAL_ADDR             (CONFIG_NO_SENSOR_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //�ֱ����ϱ��澯��� UINT
//#define CONFIG_TOUCHOFF_ALARM_INTERVAL_ADDR               (CONFIG_RESOLUTION_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //�˹������ϱ���� UINT
//#define CONFIG_TRIGGER_INTERVAL_ADDR            (CONFIG_TOUCHOFF_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C)//�����ʱ��    UINT    0-65535��
//#define CONFIG_CLOUD_SERVER_ADD_ADDR            (CONFIG_TRIGGER_INTERVAL_ADDR+42+CONFIG_0X2C)//����ɼ���ַ    �ַ���    �21
//#define CONFIG_UPDATE_SERVER_ADDR               (CONFIG_CLOUD_SERVER_ADD_ADDR+42+CONFIG_0X2C)//������������ַ    �ַ���    �21
//#define CONFIG_CARRIER_TYPE_ADDR                (CONFIG_UPDATE_SERVER_ADDR+2+CONFIG_0X2C)//��Ӫ��    Uchar    0���� 1�ƶ� 2��ͨ
//#define CONFIG_MODULE_TYPE_ADDR                 (CONFIG_CARRIER_TYPE_ADDR+2+CONFIG_0X2C)//������ʽ    Uchar    0:2G 1:3G 2:4G3:NB-IOT
//#define CONFIG_CARD_ID_ADDR                     (CONFIG_MODULE_TYPE_ADDR+40+CONFIG_0X2C)  //���������š�                    
//#define CONFIG_TIMESTAMP_ADDR                   (CONFIG_CARD_ID_ADDR+8+CONFIG_0X2C)  //ʱ��� 4 1970��01��01��8��0��0�뵽���ڵ���������1498010100
//#define CONFIG_PRESSURE_UPLIMIT_ADDR            (CONFIG_TIMESTAMP_ADDR+4+CONFIG_0X2C) //ѹ������    UINT    0~65535
//#define CONFIG_PRESSURE_DOWNLIMIT_ADDR          (CONFIG_PRESSURE_UPLIMIT_ADDR+4+CONFIG_0X2C) //ѹ������
//#define CONFIG_DATAINFO_LEN                     (CONFIG_PRESSURE_DOWNLIMIT_ADDR+4+CONFIG_0X2C) //DATAINFO���ȡ�
//#define CONFIG_LEN                              (CONFIG_DATAINFO_LEN+CONFIG_CHKSUM_LEN+CONFIG_EOI_LEN) //ͨѶ���������ܳ��ȡ�����˨276
#define CONFIG_BEGIN_ADDR	                (0)     
#define CONFIG_PROTOCOL_VER_ADDR	        (CONFIG_BEGIN_LEN)  //
#define CONFIG_DEVICE_ADD_ADDR		        (CONFIG_PROTOCOL_VER_LEN+CONFIG_PROTOCOL_VER_ADDR)
#define CONFIG_DEVICE_TYPE_ADDR		        (CONFIG_DEVICE_ADD_LEN+CONFIG_DEVICE_ADD_ADDR)
#define CONFIG_CMD_ADDR			        (CONFIG_DEVICE_TYPE_LEN+CONFIG_DEVICE_TYPE_ADDR)
#define CONFIG_LEN_ADDR 		        (CONFIG_CMD_LEN+CONFIG_CMD_ADDR)
#define CONFIG_CHANNEL_COUNT_ADDR 	        (CONFIG_LEN_LEN+CONFIG_LEN_ADDR)  //ͨ������	UINT��	�ַ������� 
#define CONFIG_BIT_FLAG_ADDR		        (CONFIG_CHANNEL_COUNT_ADDR+4+CONFIG_0X2C)  //ͨ����Ч��־	Long��	��Ӧ��Bitx�����Ƿ���Ч 
#define CONFIG_ID_ADDR 			        (CONFIG_BIT_FLAG_ADDR+8+CONFIG_0X2C)  //ID��	�ַ���	16λ 
#define CONFIG_SOFT_VER_ADDR                    (CONFIG_ID_ADDR+32+CONFIG_0X2C)  //����汾    �ַ���    2λ���硰10��
#define CONFIG_HARDWARE_VER_ADDR                (CONFIG_SOFT_VER_ADDR+4+CONFIG_0X2C)  //Ӳ���汾    �ַ���    2λ���硰10��
#define CONFIG_STATUS_ADDR                      (CONFIG_HARDWARE_VER_ADDR+4+CONFIG_0X2C)//�豸״̬    Uchar    ���״̬ ����״̬ ��װ״̬ ����״̬ �̼����µȴ�״̬
#define CONFIG_SEND_TOTALCOUNT_ADDR             (CONFIG_STATUS_ADDR+2+CONFIG_0X2C)//Bit4    ���ʹ���    UINT    0-65535
#define CONFIG_PRESSURE_TRIGGER_VALUE_ADDR      (CONFIG_SEND_TOTALCOUNT_ADDR+4+CONFIG_0X2C)//ѹ���仯�ֱ���    UINT    1-65535
#define CONFIG_HEART_INTERVAL_ADDR              (CONFIG_PRESSURE_TRIGGER_VALUE_ADDR+4+CONFIG_0X2C)//�������ʱ��    UINT    0-65535ʱ
#define CONFIG_ALARM_ENABLE_BIT_ADDR            (CONFIG_HEART_INTERVAL_ADDR+4+CONFIG_0X2C) ////�澯ʹ��0ʧ�ܣ�1ʹ�� UINT
#define CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL_ADDR       (CONFIG_ALARM_ENABLE_BIT_ADDR+4+CONFIG_0X2C) ///���޸澯���ʱ�� UINT
#define CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL_ADDR    (CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //���޸澯���ʱ�� UINT
#define CONFIG_NO_SENSOR_ALARM_INTERVAL_ADDR              (CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //δ��⵽�������澯��� UINT
#define CONFIG_RESOLUTION_ALARM_INTERVAL_ADDR             (CONFIG_NO_SENSOR_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //�ֱ����ϱ��澯��� UINT
#define CONFIG_TOUCHOFF_ALARM_INTERVAL_ADDR               (CONFIG_RESOLUTION_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //�˹������ϱ���� UINT
#define CONFIG_TRIGGER_INTERVAL_ADDR            (CONFIG_TOUCHOFF_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C)//�����ʱ��    UINT    0-65535��
#define CONFIG_CLOUD_SERVER_ADD_ADDR            (CONFIG_TRIGGER_INTERVAL_ADDR+4+CONFIG_0X2C)//����ɼ���ַ    �ַ���    �21
#define CONFIG_UPDATE_SERVER_ADDR               (CONFIG_CLOUD_SERVER_ADD_ADDR+42+CONFIG_0X2C)//������������ַ    �ַ���    �21
#define CONFIG_CARRIER_TYPE_ADDR                (CONFIG_UPDATE_SERVER_ADDR+42+CONFIG_0X2C)//��Ӫ��    Uchar    0���� 1�ƶ� 2��ͨ
#define CONFIG_MODULE_TYPE_ADDR                 (CONFIG_CARRIER_TYPE_ADDR+2+CONFIG_0X2C)//������ʽ    Uchar    0:2G 1:3G 2:4G3:NB-IOT
#define CONFIG_CARD_ID_ADDR                     (CONFIG_MODULE_TYPE_ADDR+2+CONFIG_0X2C)  //���������š�                    
#define CONFIG_TIMESTAMP_ADDR                   (CONFIG_CARD_ID_ADDR+40+CONFIG_0X2C)  //ʱ��� 4 1970��01��01��8��0��0�뵽���ڵ���������1498010100
#define CONFIG_PRESSURE_UPLIMIT_ADDR            (CONFIG_TIMESTAMP_ADDR+8+CONFIG_0X2C) //ѹ������    UINT    0~65535
#define CONFIG_PRESSURE_DOWNLIMIT_ADDR          (CONFIG_PRESSURE_UPLIMIT_ADDR+4+CONFIG_0X2C) //ѹ������
#define CONFIG_DATAINFO_LEN                     (CONFIG_PRESSURE_DOWNLIMIT_ADDR+4+CONFIG_0X2C) //DATAINFO���ȡ�
#define CONFIG_LEN                              (CONFIG_DATAINFO_LEN+CONFIG_CHKSUM_LEN+CONFIG_EOI_LEN) //ͨѶ���������ܳ��ȡ�����˨276

#define PROTOCOL_CONFIG_TOTAL_LEN (CONFIG_LEN)

#define BIT_1   ((u32)1)
#define BIT_CONFIG_ID 			              	(BIT_1<<0)  //ID��	�ַ���	16λ                     
#define BIT_CONFIG_SOFT_VER 		          	(BIT_1<<1)  //����汾	�ַ���	2λ���硰10��                    
#define BIT_CONFIG_HARDWARE_VER 	        	(BIT_1<<2)  //Ӳ���汾	�ַ���	2λ���硰10��              
#define BIT_CONFIG_STATUS 	              	        (BIT_1<<3)  //�豸״̬	Uchar	���״̬ ����״̬ ��װ״̬ ����״̬ 
#define BIT_CONFIG_SEND_TOTALCOUNT        	        (BIT_1<<4)  //Bit4	���ʹ���	UINT	0-65535           
#define BIT_CONFIG_PRESSURE_TRIGGER_VALUE 	        (BIT_1<<5)  //Bit5	ѹ���仯�ֱ���	UINT	1-65535 
#define BIT_CONFIG_HEART_INTERVAL        		(BIT_1<<6)  //Bit6	�������ʱ��	UINT	0-65535ʱ
#define BIT_CONFIG_ALARM_ENABLE_BIT          	        (BIT_1<<7)  //Bit7	�澯ʹ��	2
#define BIT_CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL	(BIT_1<<8)  //Bit8	���޸澯���ʱ��	2	1~65534 ����65535��ʾȡ��
#define BIT_CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL	(BIT_1<<9)  //Bit9	���޸澯���ʱ��	2	1~65534 ����65535��ʾȡ��   
#define BIT_CONFIG_NO_SENSOR_ALARM_INTERVAL	    	(BIT_1<<10) //Bit10	δ��⵽�������澯�����	2	1~65534 ����65535��ʾȡ��
#define BIT_CONFIG_RESOLUTION_ALARM_INTERVAL	      	(BIT_1<<11) //Bit11	�ֱ����ϱ��澯���	2	1~65534 ����65535��ʾȡ��
#define BIT_CONFIG_TOUCHOFF_ALARM_INTERVAL 	      	(BIT_1<<12) //Bit12	�˹������ϱ����	2	1~65534 ����65535��ʾȡ��
#define BIT_CONFIG_TRIGGER_INTERVAL 		  	(BIT_1<<13) //�����ʱ��	UINT	0-65535��        
#define BIT_CONFIG_CLOUD_SERVER_ADD 		  	(BIT_1<<14) //����ɼ���ַ	�ַ���	�21            
#define BIT_CONFIG_UPDATE_SERVER 		    	(BIT_1<<15) //������������ַ	�ַ���	�21        
#define BIT_CONFIG_CARRIER_TYPE 		        (BIT_1<<16) //��Ӫ��	Uchar	0���� 1�ƶ� 2��ͨ           
#define BIT_CONFIG_MODULE_TYPE 		          	(BIT_1<<17) //������ʽ	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
#define BIT_CONFIG_CARD_ID                              (BIT_1<<18) //���������š�20  
#define BIT_TIMESTAMP_ID                                (BIT_1<<19) //ʱ��� 4 1970��01��01��8��0��0�뵽���ڵ���������1498010100  
#define BIT_PRESSURE_UPLIMIT_ID                         (BIT_1<<20) //ѹ������    UINT    0~65535
#define BIT_PRESSURE_DOWNLIMIT_ID                       (BIT_1<<21) //ѹ������    UINT    0~65535
#endif

/*
#define CONFIG_BEGIN_ADDR			0
#define CONFIG_PROTOCOL_VER_ADDR	1
#define CONFIG_DEVICE_ADD_ADDR		3
#define CONFIG_DEVICE_TYPE_ADDR		5

#define CONFIG_CMD_ADDR				7
#define CONFIG_LEN_ADDR 			9

#define CONFIG_CHANNEL_COUNT_ADDR 	13
#define CONFIG_BIT_FLAG_ADDR		19
#define CONFIG_ID_ADDR 			29
#define CONFIG_SOFT_VER_ADDR		63
#define CONFIG_HARDWARE_VER_ADDR	69
#define CONFIG_TRIGGER_ANGLE_ADDR	75
#define CONFIG_TRIGGER_SEN_ADDR		79
#define CONFIG_HEART_S_COUNT_ADDR	83
#define CONFIG_ALARM_S_COUNT_ADDR	89 //�����������
#define CONFIG_SEND_TOTALCOUNT_ADDR	95
#define CONFIG_TEMP_ADJUST_VALUE_ADDR 101
#define CONFIG_UPDATE_SERVER_ADDR	105
#define CONFIG_CLOUD_SERVER_ADDR	149
#define CONFIG_DEFAULT_CLOUD_SERVER_ADDR	193
#define CONFIG_CARRIER_TYPE_ADDR	237
#define CONFIG_MODULE_TYPE_ADDR		241

#define CONFIG_CHECKSUM_ADDR		245
#define CONFIG_END_ADDR 			249
*/
//#define CONFIG_BEGIN_LEN                        (1) 
//#define CONFIG_PROTOCOL_VER_LEN                 (2) 
//#define CONFIG_DEVICE_ADD_LEN                   (2) 
//#define CONFIG_DEVICE_TYPE_LEN                  (2) 
//#define CONFIG_CMD_LEN                          (2) 
//#define CONFIG_LEN_LEN                          (4) 
//
//#define CONFIG_CHANNEL_COUNT_LEN                (4) //ͨ������	UINT��	�ַ������� 15
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C            //19
//#define CONFIG_BIT_FLAG_LEN                     (8) //ͨ����Ч��־	Long��	��Ӧ��Bitx�����Ƿ���Ч
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
//#define CONFIG_ID_LEN                           (32)//ID��	�ַ���	16λ 
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
//#define CONFIG_SOFT_VER_LEN                     (4) //����汾	�ַ���	2λ���硰10�� 
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
//#define CONFIG_HARDWARE_VER_LEN	                (4)//Ӳ���汾	�ַ���	2λ���硰10�� 
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C    //69
//#define CONFIG_STATUS_LEN	                (2)//�豸״̬	Uchar	���״̬ ����״̬ ��װ״̬ ����״̬ 
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C      73
//#define CONFIG_SEND_TOTALCOUNT_LEN              (4)//Bit4	���ʹ���	UINT	0-65535   
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     79
//#define CONFIG_ROTATION_TRIGGER_ANGLE_LEN       (2)//��ת�仯�Ƕ�	UINT	-360~360    	
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C      85
//#define CONFIG_ROTATION_TRIGGER_SENS_LEN        (2)//��ת������	Uchar	0-99      
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     89
//#define CONFIG_LEAN_TRIGGER_ANGLE_LEN           (2)//��б�仯�Ƕ�	Uchar	-90~90 	
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     93
//#define CONFIG_LEAN_TRIGGER_SENS_LEN	        (2)//��б������	Uchar	0-99       
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     97
//#define CONFIG_PRESSURE_TRIGGER_VALUE_LEN       (4)//ѹ���仯��ֵ	UINT	1-65535 
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     93
//#define CONFIG_PRESSURE_INTERVAL_LEN            (4)//ѹ�������	UINT	0-65535  
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     99 
//#define CONFIG_HEART_INTERVAL_LEN               (4)//�������ʱ��	UINT	0-65535ʱ  
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     105
//#define CONFIG_ALARM_INTERVAL_LEN               (4)//�澯���ʱ��	UINT	0-65535��  
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     110
//#define CONFIG_TRIGGER_INTERVAL_LEN             (4)//�����ʱ��	UINT	0-65535��
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     116
//#define CONFIG_CLOUD_SERVER_ADD_LEN             (42)//����ɼ���ַ	�ַ���	�21    
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     160
//#define CONFIG_UPDATE_SERVER_LEN                (42)//������������ַ	�ַ���	�21    
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     204
//#define CONFIG_CARRIER_TYPE_LEN                 (2)//��Ӫ��	Uchar	0���� 1�ƶ� 2��ͨ  
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C     
//#define CONFIG_MODULE_TYPE_LEN                  (2)//������ʽ	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C   
//#define CONFIG_CARD_ID_LEN                      (40)//���������š�20
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
//#define CONFIG_TIMESTAMP_LEN                    (8)//ʱ��� 4 1970��01��01��8��0��0�뵽���ڵ���������1498010100 
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
//#define CONFIG_PRESSURE_UPLIMIT_LEN             (4)//ѹ������	UINT	0~65535
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
//#define CONFIG_PRESSURE_DOWNLIMIT_LEN          (4)//ѹ������	UINT	0~65535
////�ֽ��1	Uchar	�ֽ��1�̶�Ϊ��0x2C
//
//#define CONFIG_AMD_PARA_FLAG_ADDR 46
//#define CONFIG_AMD_UPDATA_FLAG_ADDR 48
//
//
//#define CONFIG_BEGIN_ADDR     0
//#define CONFIG_PROTOCOL_VER_ADDR  CONFIG_BEGIN_ADDR+CONFIG_BEGIN_LEN
//#define CONFIG_DEVICE_ADD_ADDR    CONFIG_PROTOCOL_VER_ADDR+CONFIG_PROTOCOL_VER_LEN
//#define CONFIG_DEVICE_TYPE_ADDR   CONFIG_DEVICE_ADD_ADDR+CONFIG_DEVICE_ADD_LEN
//
//#define CONFIG_CMD_ADDR       CONFIG_DEVICE_TYPE_ADDR+CONFIG_DEVICE_TYPE_LEN
//#define CONFIG_LEN_ADDR       CONFIG_CMD_ADDR+CONFIG_CMD_LEN
//
//#define CONFIG_CHANNEL_COUNT_ADDR         CONFIG_LEN_ADDR+(1<<1)+2
//#define CONFIG_BIT_FLAG_ADDR              CONFIG_CHANNEL_COUNT_ADDR+(2<<1)+2
//#define CONFIG_ID_ADDR                    CONFIG_BIT_FLAG_ADDR+(4<<1)+2
//#define CONFIG_SOFT_VER_ADDR              CONFIG_ID_ADDR+(16<<1)+2
//#define CONFIG_HARDWARE_VER_ADDR          CONFIG_SOFT_VER_ADDR+(2<<1)+2
//#define CONFIG_TRIGGER_ANGLE_ADDR         CONFIG_HARDWARE_VER_ADDR+(2<<1)+2
//#define CONFIG_TRIGGER_SEN_ADDR           CONFIG_TRIGGER_ANGLE_ADDR+(1<<1)+2
//#define CONFIG_HEART_S_COUNT_ADDR         CONFIG_TRIGGER_SEN_ADDR+(1<<1)+2
//#define CONFIG_ALARM_S_COUNT_ADDR         CONFIG_HEART_S_COUNT_ADDR+(2<<1)+2 //�����������
//#define CONFIG_SQ_S_COUNT_ADDR            CONFIG_ALARM_S_COUNT_ADDR+(2<<1)+2 //����ȷ�ϼ��
//#define CONFIG_SEND_TOTALCOUNT_ADDR       CONFIG_SQ_S_COUNT_ADDR+(2<<1)+2
//#define CONFIG_TEMP_ADJUST_VALUE_ADDR     CONFIG_SEND_TOTALCOUNT_ADDR+(2<<1)+2
//#define CONFIG_UPDATE_SERVER_ADDR         CONFIG_TEMP_ADJUST_VALUE_ADDR+(1<<1)+2
//#define CONFIG_CLOUD_SERVER_ADDR          CONFIG_UPDATE_SERVER_ADDR+(21<<1)+2
//#define CONFIG_DEFAULT_CLOUD_SERVER_ADDR  CONFIG_CLOUD_SERVER_ADDR+(21<<1)+2
//#define CONFIG_CARRIER_TYPE_ADDR          CONFIG_DEFAULT_CLOUD_SERVER_ADDR+(21<<1)+2
//#define CONFIG_MODULE_TYPE_ADDR           CONFIG_CARRIER_TYPE_ADDR+(1<<1)+2
//
//#define CONFIG_CHECKSUM_ADDR              CONFIG_MODULE_TYPE_ADDR+(1<<1)+2
//#define CONFIG_END_ADDR                   CONFIG_CHECKSUM_ADDR+4
//
//#define BIT_1   ((u32)1)
//#define BIT_CONFIG_ID 			              	(BIT_1<<0)  //ID��	�ַ���	16λ                     
//#define BIT_CONFIG_SOFT_VER 		          	(BIT_1<<1)  //����汾	�ַ���	2λ���硰10��                    
//#define BIT_CONFIG_HARDWARE_VER 	        	(BIT_1<<2)  //Ӳ���汾	�ַ���	2λ���硰10��              
//#define BIT_CONFIG_STATUS 	              	        (BIT_1<<3)  //�豸״̬	Uchar	���״̬ ����״̬ ��װ״̬ ����״̬ 
//#define BIT_CONFIG_SEND_TOTALCOUNT        	        (BIT_1<<4)  //Bit4	���ʹ���	UINT	0-65535           
//#define BIT_CONFIG_ROTATION_TRIGGER_ANGLE 	        (BIT_1<<5)  //��ת�仯�Ƕ�	UINT	-360~360    
//#define BIT_CONFIG_ROTATION_TRIGGER_SENS 		(BIT_1<<6)  //��ת������	Uchar	0-99      
//#define BIT_CONFIG_LEAN_TRIGGER_ANGLE    	        (BIT_1<<7)  //��б�仯�Ƕ�	Uchar	-90~90
//#define BIT_CONFIG_LEAN_TRIGGER_SENS 	    	        (BIT_1<<8)  //��б������	Uchar	0-99        
//#define BIT_CONFIG_PRESSURE_TRIGGER_VALUE 	        (BIT_1<<9)  //ѹ���仯��ֵ	UINT	1-65535       
//#define BIT_CONFIG_PRESSURE_INTERVAL 	    	        (BIT_1<<10) //ѹ�������	UINT	0-65535
//#define BIT_CONFIG_HEART_INTERVAL 	      	        (BIT_1<<11) //�������ʱ��	UINT	0-65535ʱ    
//#define BIT_CONFIG_ALARM_INTERVAL 	      	        (BIT_1<<12) //�澯���ʱ��	UINT	0-65535��        
//#define BIT_CONFIG_TRIGGER_INTERVAL 		  	(BIT_1<<13) //�����ʱ��	UINT	0-65535��        
//#define BIT_CONFIG_CLOUD_SERVER_ADD 		  	(BIT_1<<14) //����ɼ���ַ	�ַ���	�21            
//#define BIT_CONFIG_UPDATE_SERVER 		    	(BIT_1<<15) //������������ַ	�ַ���	�21        
//#define BIT_CONFIG_CARRIER_TYPE 		        (BIT_1<<16) //��Ӫ��	Uchar	0���� 1�ƶ� 2��ͨ           
//#define BIT_CONFIG_MODULE_TYPE 		          	(BIT_1<<17) //������ʽ	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
//#define BIT_CONFIG_CARD_ID                              (BIT_1<<18) //���������š�20  
//#define BIT_TIMESTAMP_ID                                (BIT_1<<19) //ʱ��� 4 1970��01��01��8��0��0�뵽���ڵ���������1498010100  
//#define BIT_PRESSURE_UPLIMIT_ID                         (BIT_1<<20) //ѹ������    UINT    0~65535
//#define BIT_PRESSURE_DOWNLIMIT_ID                       (BIT_1<<21) //ѹ������    UINT    0~65535
//���ò�����Ч��־λ
//#define BIT_CONFIG_ID 				(1<<0)
//#define BIT_CONFIG_SOFT_VER			(1<<1)
//#define BIT_CONFIG_HARDWARE_VER		(1<<2)
//
//#define BIT_CONFIG_TRIGGER_ANGLE	(1<<3)
//#define BIT_CONFIG_TRIGGER_SEN		(1<<4)
//#define BIT_CONFIG_HEART_S_COUNT	(1<<5)
//#define BIT_CONFIG_ALARM_S_COUNT	(1<<6)
//#define BIT_CONFIG_SQ_S_COUNT  (1<<7)//����ȷ�ϼ��ʱ��
//#define BIT_CONFIG_SEND_TOTALCOUNT  (1<<8)
//#define BIT_CONFIG_TEMP_ADJUST_VALUE  (1<<9)
//#define BIT_CONFIG_UPDATE_SERVER  (1<<10)
//#define BIT_CONFIG_CLOUD_SERVER   (1<<11)
//#define BIT_CONFIG_DEFAULT_CLOUD_SERVER (1<<12)
//#define BIT_CONFIG_CARRIER_TYPE     (1<<13)
//#define BIT_CONFIG_MODULE_TYPE      (1<<14)
/*
#define BIT_CONFIG_SEND_TOTALCOUNT	(1<<7)
#define BIT_CONFIG_TEMP_ADJUST_VALUE	(1<<8)
#define BIT_CONFIG_UPDATE_SERVER	(1<<9)
#define BIT_CONFIG_CLOUD_SERVER		(1<<10)
#define BIT_CONFIG_DEFAULT_CLOUD_SERVER	(1<<11)
#define BIT_CONFIG_CARRIER_TYPE			(1<<12)
#define BIT_CONFIG_MODULE_TYPE			(1<<13)
*/


/*
u8 device_id[17];//�豸ID
  u8 soft_version[3];//����汾
  u8 harware_version[3];//Ӳ���汾
  u8 trigger_angle;//�����Ƕ�
  u8 trigger_sensitivity;//����������
  u16 heartdata_sent_interval_minute_count;//�������ʱ��
  u16 alarm_sent_interval_second_count;//�������ʱ��
  u16 send_totalcount;//��ط����ܴ���
  s8 temp_adjust_value;//�¶�������
  s16 temp_offset;//�¶�����ֵ
  u8 update_server_add[22];//������������ַ255.255.255.255,65535
  u8 cloud_server_add[22];//�ƶ˷�������ַ255.255.255.255,65535
  u8 default_cloud_server_add[22];//Ĭ���ƶ˷�������ַ255.255.255.255,65535
  u8 carrier_type;//��Ӫ��   �����޸� ��Ӳ���汾
  u8 module_type;//ģ����ʽ �����޸� ��Ӳ���汾
*/

typedef struct
{
        unsigned char Mode_Restart : 4; //ģ������׼����־
        unsigned char reconnect_count : 4; //�ظ����Ӵ���
        unsigned char send_count : 8;  //���ʹ���
        
	unsigned char initStep : 5;
        unsigned char ack_config : 1;//�ظ��������˱�־
        unsigned char send_success_flag : 1;//��Ϣ���ͳɹ���־
	unsigned char cardType : 1; //�ֻ���Ϊ1��������Ϊ0
} IOTMode_INFO;


typedef enum
{
  ACK_CONFIG_FAILED       = 0, /*!< Channel 00 */
//  ADC_Channel_1           = 1, /*!< Channel 01 */
//  ADC_Channel_2           = 2, /*!< Channel 02 */
//  ADC_Channel_3           = 3, /*!< Channel 03 */
//  ADC_Channel_4           = 4, /*!< Channel 04 */
}ACK_CONFIG_TypeDef;

typedef enum
{
  RTN_OK              = 0x00, /*!< ����*/
  RTN_ERR_VER         = 0x01, /*!< VER���� */
  RTN_ERR_CHKSUM      = 0x02, /*!< CHKSUM���� */
  RTN_ERR_LCHKSUM     = 0x03, /*!< LCHKSUM���� */
  RTN_ERR_CID2        = 0x04, /*!< CID2��Ч */
  RTN_ERR_FORMAT      = 0x05, /*!< ���ݸ�ʽ���� */
  RTN_ERR_INVALID     = 0x06, /*!< ��Ч���� */
  RTN_NULL            = 0x07, /*!< ���ظ� */
}RTN_TypeDef;



void sys_restart(void);
_Bool IsConfigConnect(USART_INFO *usartInfo);
_Bool IsUpdataConnect(void);
//void Test_protocol(void);
_Bool Select_protocol(USART_INFO *usartInfo);

//����Э�����
_Bool Para_config_protocol(char *input_protocol_string);

//�ж����ð�У����
_Bool CheckCheckSumFromConfigData(char *configdata);
//����������ûظ�����RTN
void Pack_Ack_Config_RTN(u8 rtn_value);
//�ظ���������
void Ack_config_cmd_2_pc(RTN_TypeDef rtn_value);




extern u8 ack_protocol_config_array[37];

HEX2STR_1 Hex2StrFun_1(u8 *addr);
HEX2STR_2 Hex2StrFun_2(u16 *addr);
//HEX2STR_13 Hex2StrFun_13(char *tempstr);
//HEX2STR_16 Hex2StrFun_16(char *tempstr);
HEX2STR_21 Hex2StrFun_21(char *tempstr);







//////////////���鿽��������////////////////////////////
void ArrayCopy(u8 *dest, u8 *src,u8 len);
//////////////�����Ƿ���ͬ����////////////////////////////
_Bool ArrayCmp(u8 *dest, u8 *src, u8 len);
//����������Ƿ����Э���׼
_Bool Isformatcorrect(char *protol_string);
///////////////////////Э���ڵ�ascii��תЭ����ת��////////////////
u8* Str2HexFun_16(char *tempstr, u8 addr);
u8* Str2HexFun(char *tempstr, u16 addr,u8 len);

char *num2string(u16 tempnum);
char *intnum2hex(u16 tempnum);
#endif