#ifndef MODULE_SELECT_H
#define MODULE_SELECT_H

#define 	BOOT_CL 		0
#define  	BOOT_HD 		1

/*--ϵͳ����Ԥ�������--*/
#define 	CONF_WATCHDOG_EN		0			/*ʹ������Ź�*/
#define		CONF_UART1_EN			1			/*�Ƿ�ʹ�ô���1����*/
#define		CONF_UART2_EN			0			/*�Ƿ�ʹ�ô���2����*/
#define		CONF_UART3_EN			1			/*�Ƿ�ʹ�ô���3����*/
#define		CONF_UART4_EN			0			/*�Ƿ�ʹ�ô���4����*/
#define		CONF_UART5_EN			0			/*�Ƿ�ʹ�ô���5����*/
#define		CONF_LED_EN			1			/*ʹ��Э��*/
#define		CONF_PROTOCOL_EN		1			/*ʹ��Э��*/
#define		CONF_TIMER_EN  			1			/*ʹ����ʱ��*/
#define		CONF_IAP_EN			1			/*�������ʹ��*/
#define		CONF_CESHI_EN			0			/*���Դ���궨��*/
#define		CONF_QWL_NET_EN			1			/*���Դ���궨��*/

#define		UPLOAD_SPI_FLASH		0			/*���ڹ̼���������洢��ΪFLASH*/
#define	        UPLOAD_EEPROM                   1                       /*���ڹ̼���������洢��ΪEEPROM*/
#define         CONF_EX_MEMORY_EN               UPLOAD_SPI_FLASH        /*ѡ���ⲿ�洢��*/
/*********************************************************************************************************************************************/
/*--���ǹ���Ԥ�������--*/
#define         HARDWARE_VERSION_V105           0			/*Ӳ���汾V1.05*/
#define         HARDWARE_VERSION_V205           1			/*Ӳ���汾V2.05*/
#define         CONF_HARDWARE_VERSION		HARDWARE_VERSION_V205   /*ѡ��Ӳ���汾������IO����*/		
#define         LiquidAlarm_EN                  0                       /*ˮλ����*/
#define         RADAR_EN                        0                       /*�����״﹦��*/
#define	        COMPANY_NAME			"ModouTech           "  /*��˾����*/






/////////////////////////��ҳ����Ϊȫ�����ý���///////////////////////////////////
//
////����汾
//#ifdef SYS_LDO_20V //2.0V ϵͳѡ��
//  #define SOFT_VERSION     "SOFT_VERSION---V1.04 DEMO 20170309 WDG 2.0V"
//  #define HARDWARE_VERSION "HARDWARE_VERSION---V1.02" 
//#else //��2.0Vϵͳ  Ŀǰ�������ϵͳ
//    #if CONF_EX_MEMORY_EN ==UPLOAD_SPI_FLASH
//        #if RADAR_EN                   /*�����״�*/
//              #define SOFT_VERSION     "SOFT_VERSION---V6.00 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V06.00"//�ϱ�ƽ̨������汾����λ���������ã�д����ʱע���޸�
//        #elif RADAR_ENLiquidAlarm_EN   /*ˮλ*/
//              #define SOFT_VERSION     "SOFT_VERSION---V5.00 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V05.00"//�ϱ�ƽ̨������汾����λ���������ã�д����ʱע���޸�
//        #else                           /*��ͨ*/
//              #define SOFT_VERSION     "SOFT_VERSION---V4.00 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V03.00"//�ϱ�ƽ̨������汾����λ���������ã�д����ʱע���޸�
//        #endif
//
//        #define HARDWARE_VERSION "HARDWARE_VERSION---V2.07" 
//    #else
//        #if CONF_HARDWARE_VERSION == HARDWARE_VERSION_V105/*Ӳ���汾V1.05*/
//              #define SOFT_VERSION     "SOFT_VERSION---V3.70 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V03.70"//�ϱ�ƽ̨������汾����λ���������ã�д����ʱע���޸�
//              #define HARDWARE_VERSION "HARDWARE_VERSION---V1.05" 
//        #else                                             /*Ӳ���汾V2.05*/
//              #define SOFT_VERSION     "SOFT_VERSION---V3.82 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V03.83"//�ϱ�ƽ̨������汾����λ���������ã�д����ʱע���޸�
//              #define HARDWARE_VERSION "HARDWARE_VERSION---V2.05" 
//        #endif
//    #endif
//#endif


////////////////////////////////////����Ҫ���á�/////////////////////////////////////////////
//#define DEBUG_JIASU_RTC_100 //����100��RTC����
#define APP_SELECT (0) //1��ˮѹ����0 �Ƹǳ���
#if APP_SELECT
  #define APP_SELECT_PRESSURE (1)  //ˮѹ����
  #define APP_SELECT_COVER    (0)  //�Ƹǳ���
  #define RTC_USE_LSE (1) //RTCʱ��ѡ��0�ڲ�����1�ⲿ����
  #define PRESSURESYS_MODULE_EN (1) //ģ��ʹ��
  #define INTERVAL_MODULE_EN (1) //ģ��ʹ��
  #define ALARMTIME_PRESSURE_EN (1)
  #define APP_CAN_UPDATA (1) //APP����������־��0,����ͨѶ��1������ͨѶ
  #define SPEED_UP (1) //���ٱ�־��0��������1���ٳ���
  #define WDG_EN  (1) //���Ź�ʹ��1��ʧ��0 
  #define PRESSURE_MODE_SELECT 1  //����������ѡ��1,1 1mPa�汾,0 1.6mPa�汾
  #define SD3078_WIRTE_INIT_FLAG_EN (1)  //sd3078д���־ʹ�ܣ�0ʧ�ܣ�1ʹ��
  #define ENTER_LPW_ENABLE (1) //����͹���ʹ�ܡ�
  #define USE_HUAWEI_IOT (1)
  #define SOFT_VERSION     "�̼��汾��V4.00 NB-IOT CDMA IAP" 
  #define SOFTWARE_VER     "V04.00"//�ϱ�ƽ̨������汾����λ���������ã�д����ʱע���޸�
  #define HARDWARE_VERSION "Ӳ���汾��V1.02" 
//  #define DEVICE_ID_PRESSURE    "1401000080026666" //16λID�š�
  #define DEVICE_ID_PRESSURE    "1401000000000000" //16λID�š�
  #define DEFAULT_CLOUD_SERVER_ADD_TEST (1) //Ĭ�ϲɼ���ַ���ԡ�1,ʹ�ò��Ե�ַ��0ʹ�ö�Ӧ��Ʒ�ĵ�ַ��
  #if DEFAULT_CLOUD_SERVER_ADD_TEST
    #define DEFAULT_CLOUD_SERVER_ADD 	                "123.159.193.22,8231"//���˵��Բ���ƽ̨��
//    #define DEFAULT_CLOUD_SERVER_ADD 	                "39.108.148.248,30007"  //ˮѹͳһƽ̨
  #else
    #define DEFAULT_CLOUD_SERVER_ADD 	                "39.108.91.208,20008"
  #endif
  #define    	POST_VER		   	0x13 //Э��汾
  #define    	SYS_DEVICE_TYPE		   	0xD0 //�豸����
  
   #if SPEED_UP
    #if RTC_USE_LSE
      #define RTC_MIN 192   //32.768K���ⲿ���񣬶�ʱ����6�뻽�ѣ�������Ϊ���ٰ汾��
    #else
     #define RTC_MIN (63750/300)
    #endif
  #else
    #if RTC_USE_LSE
      #define RTC_MIN 1920   //32.768K���ⲿ���񣬶�ʱ����60�뻽�ѣ�������Ϊ�����汾��
    #else
      #define RTC_MIN (63750/30)
    #endif
  #endif
 
  #if PRESSURE_MODE_SELECT
    #define PRESSURE_MODE (1000) //1mPa�汾
  #else
    #define PRESSURE_MODE (1600) //1.6mPa�汾
  #endif
  #define STATUS_NORMAL (0)       //�豸״̬����
  #define STATUS_CLEAR_FLASH (5)  //�豸״̬���豸��ʼ��״̬��
  #define RTC_HEART (60) 
  #define EEPROM_HISTORY_ADDR (2)
#else
  #define APP_SELECT_PRESSURE (0)  //ˮѹ����
  #define APP_SELECT_COVER    (1)  //�Ƹǳ���
  #define RTC_USE_LSE (0) //RTCʱ��ѡ��0�ڲ�����1�ⲿ����
  #define RTC_CAN_BE_SET (0)
  #define ALARMTIME_EN (1)
  #define ENTER_LPW_ENABLE (1) //����͹���ʹ�ܡ�
  #define SOFT_VERSION     "�̼��汾��V3.00 NB-IOT CDMA IAP" 
  #define SOFTWARE_VER     "V03.00"//�ϱ�ƽ̨������汾����λ���������ã�д����ʱע���޸�
  #define HARDWARE_VERSION "Ӳ���汾��V1.05" 
  #define    	POST_VER		   	0x12 //Э��汾
  #define    	SYS_DEVICE_TYPE		   	0xD0 //�豸����
  #define APP_CAN_UPDATA (1) //APP����������־��0,����ͨѶ��1������ͨѶ
  #define SPEED_UP (0) //���ٱ�־��0��������1���ٳ���
  #if SPEED_UP
  #define RTC_MIN (63750/30)
  //    #if RTC_USE_LSE
  //      #define RTC_MIN 192   //32.768K���ⲿ���񣬶�ʱ����6�뻽�ѣ�������Ϊ���ٰ汾��
  //    #else
  //     #define RTC_MIN (63750/300)
  //    #endif
  #else
  #define RTC_MIN (63750)
  //    #if RTC_USE_LSE
  //      #define RTC_MIN 1920   //32.768K���ⲿ���񣬶�ʱ����60�뻽�ѣ�������Ϊ�����汾��
  //    #else
  //      #define RTC_MIN (63750/30)
  //    #endif
  #endif
  #define WDG_EN  (1) //���Ź�ʹ��1��ʧ��0 
  #define DEVICE_ID                                       "1400000000000000" //16λID�š�
  #define DEFAULT_CLOUD_SERVER_ADD_TEST (0) //Ĭ�ϲɼ���ַ���ԡ�1,ʹ�ò��Ե�ַ��0ʹ�ö�Ӧ��Ʒ�ĵ�ַ��
  #if DEFAULT_CLOUD_SERVER_ADD_TEST
    #define DEFAULT_CLOUD_SERVER_ADD 	                "123.159.193.22,8231"
  #else
    #define DEFAULT_CLOUD_SERVER_ADD 	                "39.108.91.208,20008"
  #endif
  #define STATUS_NORMAL (0)       //�豸״̬����
  #define STATUS_CLEAR_FLASH (5)  //�豸״̬���豸��ʼ��״̬��
  #define RTC_HEART (60) 
  #define EEPROM_HISTORY_ADDR (2)
#endif

#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
//#define SOFT_VERSION     "�̼��汾��V4.00 NB-IOT CDMA IAP" 
//#define SOFTWARE_VER     "V04.00"//�ϱ�ƽ̨������汾����λ���������ã�д����ʱע���޸�
//#define HARDWARE_VERSION "Ӳ���汾��V1.02" 
//#define DEVICE_ID_PRESSURE    "1401000000000000" //16λID�š�
//#define    	POST_VER		   	0x13 //Э��汾
//#define    	SYS_DEVICE_TYPE		   	0xD0 //�豸����
#endif

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
//#define SOFT_VERSION     "�̼��汾��V3.00 NB-IOT CDMA IAP" 
//#define SOFTWARE_VER     "V03.00"//�ϱ�ƽ̨������汾����λ���������ã�д����ʱע���޸�
//#define HARDWARE_VERSION "Ӳ���汾��V1.05" 
//#define    	POST_VER		   	0x12 //Э��汾
//#define    	SYS_DEVICE_TYPE		   	0xD0 //�豸����
#endif

//#define APP_CAN_UPDATA (1) //APP����������־��0,����ͨѶ��1������ͨѶ
//#define STATUS_NORMAL (0)       //�豸״̬����
//#define STATUS_CLEAR_FLASH (5)  //�豸״̬���豸��ʼ��״̬��

//#define SPEED_UP (1) //���ٱ�־��0��������1���ٳ���
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
//  #if SPEED_UP
//    #if RTC_USE_LSE
//      #define RTC_MIN 192   //32.768K���ⲿ���񣬶�ʱ����6�뻽�ѣ�������Ϊ���ٰ汾��
//    #else
//     #define RTC_MIN (63750/300)
//    #endif
//  #else
//    #if RTC_USE_LSE
//      #define RTC_MIN 1920   //32.768K���ⲿ���񣬶�ʱ����60�뻽�ѣ�������Ϊ�����汾��
//    #else
//      #define RTC_MIN (63750/30)
//    #endif
//  #endif
#endif

#if APP_SELECT_COVER //һ�´������Ƹǳ������еĴ���
//  #if SPEED_UP
//    #define RTC_MIN (63750/30)
////    #if RTC_USE_LSE
////      #define RTC_MIN 192   //32.768K���ⲿ���񣬶�ʱ����6�뻽�ѣ�������Ϊ���ٰ汾��
////    #else
////     #define RTC_MIN (63750/300)
////    #endif
//  #else
//    #define RTC_MIN (63750)
////    #if RTC_USE_LSE
////      #define RTC_MIN 1920   //32.768K���ⲿ���񣬶�ʱ����60�뻽�ѣ�������Ϊ�����汾��
////    #else
////      #define RTC_MIN (63750/30)
////    #endif
//  #endif
#endif


//#define RTC_HEART (60) 
//#define SD3078_WIRTE_INIT_FLAG_EN (1)  //sd3078д���־ʹ�ܣ�0ʧ�ܣ�1ʹ��

//#define PRESSURE_MODE_SELECT 1

//#if PRESSURE_MODE_SELECT
//  #define PRESSURE_MODE (1000) //1mPa�汾
//#else
//  #define PRESSURE_MODE (1600) //1mPa�汾
//#endif



//#define WDG_EN  (1) //���Ź�ʹ��1��ʧ��0 

//#define DEFAULT_CLOUD_SERVER_ADD_TEST (1) //Ĭ�ϲɼ���ַ���ԡ�1,ʹ�ò��Ե�ַ��0ʹ�ö�Ӧ��Ʒ�ĵ�ַ��

#define DEFAULT_QECOND_QUERY_INTERVAL_SENCOND_COUNT     0
#define IOT_NUM                                         "00000000000000"  //?

//#if DEFAULT_CLOUD_SERVER_ADD_TEST
//#define DEFAULT_CLOUD_SERVER_ADD 	                "123.159.193.22,8231"
//#else
//#define DEFAULT_CLOUD_SERVER_ADD 	                "39.108.91.208,20008"
//#endif

//#define DEVICE_ID                                       "0000000000000000" //16λID�š�
/////////////////////////////////////////////////////////////////////////////////
#define SYS_OVERTIME_COUNT 60000 	//ϵͳ��ʱ�˳�ʱ�� 1000 Ϊ2��
//#define HEART_TIMECOUNT_SET 3000 //3000Ϊ24Сʱ  125Ϊ1��Сʱ���� 1Ϊ28.8������
//#define HEART_TIMECOUNT_SET 3000 //��ʱ����ʹ��

#define HEART_REPORT_EN //������ʹ��

///////////////////////����ƽ̨ѡ��///////////////////////////////////
#define MODOU_REAPORT_EN 
//#define TLINK_REAPORT_EN 
//#define REPORT_SEND_COUNT_EN //������������ģʽѡ�� 
//#define REPOERT_TEMPDATA_EN //��ʱ���ݰ�����ϴ�
//////////////////////�ֶ���д��ͬID��Ҫ�޸ĵĵ�///////////////////////////////
//#define DEVICE_ID               "20009"		//ԭ����ƽ̨ 20001-20010


#define HEART_TIMECOUNT_SET 48 //30����1��



///////////////////�ϲ�������////////////////
//#define DEVICE_ID            		"nchg004"    //�ڲ�������
//#define SERVER_ADD_PORT  		  	"182.106.128.246,2017" //�ϲ��������ƽ̨

//////////////////���Բ�������////////////////
//#define DEVICE_ID               "test01"    //�ֶ��޸�ID��λ�� APP��ƽ̨����ר��
//#define SERVER_ADD_PORT  		  "121.42.196.113,20007" //���Բ�ƽ̨

//////////////////���Բ�������////////////////
//#define DEVICE_ID               "80003"    //�ֶ��޸�ID��λ�� �Ϻ�������ʾר��
//#define DEVICE_ID               "gaodiwen04"    //�ֶ��޸�ID��λ�� Ӳ������ר�� �ߵ��²���ר�ö�
//#define DEVICE_ID               "yingjian0202"    //�ֶ��޸�ID��λ�� Ӳ������ר��
//#define DEVICE_ID               "nchg001"    //�ϲ�����3̨����  nchg001-nchg003
//#define DEVICE_ID               "gaowen01"    //���²���
//#define SERVER_ADD_PORT  		  "121.42.196.113,20008" //������ʾƽ̨ ר�� 20170112�����Ϻ��ľ�������˿�
					//cover.modoutech.com									//ͬʱ����Ӳ������
            
//#define SERVER_ADD_PORT         "123.159.193.22,7601"  //ԭ���Է�����
//#define SERVER_ADD_PORT         "121.42.196.113,20009"  //ԭ�����Ҳ��Է�����

//�Ϻ� 180.169.9.90:6080

///////////////////////////////////////////////////////////////////////////
//////////////////////��ӡ��ʾ�������///////////////////////////////
//#define DISPLAY_DEBUG_EN 1    //�򿪵�����Ϣ���
///////////////////////////////////////////////////////////////////////////








#endif
