#ifndef MODULE_SELECT_H
#define MODULE_SELECT_H

#define 	BOOT_CL 		0
#define  	BOOT_HD 		1

/*--系统常用预定义变量--*/
#define 	CONF_WATCHDOG_EN		0			/*使能与否看门狗*/
#define		CONF_UART1_EN			1			/*是否使用串口1功能*/
#define		CONF_UART2_EN			0			/*是否使用串口2功能*/
#define		CONF_UART3_EN			1			/*是否使用串口3功能*/
#define		CONF_UART4_EN			0			/*是否使用串口4功能*/
#define		CONF_UART5_EN			0			/*是否使用串口5功能*/
#define		CONF_LED_EN			1			/*使能协议*/
#define		CONF_PROTOCOL_EN		1			/*使能协议*/
#define		CONF_TIMER_EN  			1			/*使能软定时器*/
#define		CONF_IAP_EN			1			/*程序更新使能*/
#define		CONF_CESHI_EN			0			/*测试代码宏定义*/
#define		CONF_QWL_NET_EN			1			/*测试代码宏定义*/

#define		UPLOAD_SPI_FLASH		0			/*用于固件升级的外存储器为FLASH*/
#define	        UPLOAD_EEPROM                   1                       /*用于固件升级的外存储器为EEPROM*/
#define         CONF_EX_MEMORY_EN               UPLOAD_SPI_FLASH        /*选择外部存储器*/
/*********************************************************************************************************************************************/
/*--井盖功能预定义变量--*/
#define         HARDWARE_VERSION_V105           0			/*硬件版本V1.05*/
#define         HARDWARE_VERSION_V205           1			/*硬件版本V2.05*/
#define         CONF_HARDWARE_VERSION		HARDWARE_VERSION_V205   /*选择硬件版本，决定IO配置*/		
#define         LiquidAlarm_EN                  0                       /*水位功能*/
#define         RADAR_EN                        0                       /*二次雷达功能*/
#define	        COMPANY_NAME			"ModouTech           "  /*公司名字*/






/////////////////////////此页参数为全局配置界面///////////////////////////////////
//
////软件版本
//#ifdef SYS_LDO_20V //2.0V 系统选项
//  #define SOFT_VERSION     "SOFT_VERSION---V1.04 DEMO 20170309 WDG 2.0V"
//  #define HARDWARE_VERSION "HARDWARE_VERSION---V1.02" 
//#else //非2.0V系统  目前都是这个系统
//    #if CONF_EX_MEMORY_EN ==UPLOAD_SPI_FLASH
//        #if RADAR_EN                   /*二次雷达*/
//              #define SOFT_VERSION     "SOFT_VERSION---V6.00 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V06.00"//上报平台的软件版本，上位机不能配置，写程序时注意修改
//        #elif RADAR_ENLiquidAlarm_EN   /*水位*/
//              #define SOFT_VERSION     "SOFT_VERSION---V5.00 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V05.00"//上报平台的软件版本，上位机不能配置，写程序时注意修改
//        #else                           /*普通*/
//              #define SOFT_VERSION     "SOFT_VERSION---V4.00 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V03.00"//上报平台的软件版本，上位机不能配置，写程序时注意修改
//        #endif
//
//        #define HARDWARE_VERSION "HARDWARE_VERSION---V2.07" 
//    #else
//        #if CONF_HARDWARE_VERSION == HARDWARE_VERSION_V105/*硬件版本V1.05*/
//              #define SOFT_VERSION     "SOFT_VERSION---V3.70 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V03.70"//上报平台的软件版本，上位机不能配置，写程序时注意修改
//              #define HARDWARE_VERSION "HARDWARE_VERSION---V1.05" 
//        #else                                             /*硬件版本V2.05*/
//              #define SOFT_VERSION     "SOFT_VERSION---V3.82 DEMO WDG SQ DB IAP" 
//              #define SOFTWARE_VER     "V03.83"//上报平台的软件版本，上位机不能配置，写程序时注意修改
//              #define HARDWARE_VERSION "HARDWARE_VERSION---V2.05" 
//        #endif
//    #endif
//#endif


////////////////////////////////////【重要配置】/////////////////////////////////////////////
//#define DEBUG_JIASU_RTC_100 //加速100倍RTC测试
#define APP_SELECT (0) //1，水压程序，0 闷盖程序。
#if APP_SELECT
  #define APP_SELECT_PRESSURE (1)  //水压程序。
  #define APP_SELECT_COVER    (0)  //闷盖程序。
  #define RTC_USE_LSE (1) //RTC时钟选择，0内部晶振，1外部晶振
  #define PRESSURESYS_MODULE_EN (1) //模块使能
  #define INTERVAL_MODULE_EN (1) //模块使能
  #define ALARMTIME_PRESSURE_EN (1)
  #define APP_CAN_UPDATA (1) //APP程序上网标志。0,不能通讯，1，可以通讯
  #define SPEED_UP (1) //加速标志。0正常程序，1加速程序。
  #define WDG_EN  (1) //看门狗使能1，失能0 
  #define PRESSURE_MODE_SELECT 1  //传感器类型选择1,1 1mPa版本,0 1.6mPa版本
  #define SD3078_WIRTE_INIT_FLAG_EN (1)  //sd3078写入标志使能，0失能，1使能
  #define ENTER_LPW_ENABLE (1) //进入低功耗使能。
  #define USE_HUAWEI_IOT (1)
  #define SOFT_VERSION     "固件版本：V4.00 NB-IOT CDMA IAP" 
  #define SOFTWARE_VER     "V04.00"//上报平台的软件版本，上位机不能配置，写程序时注意修改
  #define HARDWARE_VERSION "硬件版本：V1.02" 
//  #define DEVICE_ID_PRESSURE    "1401000080026666" //16位ID号。
  #define DEVICE_ID_PRESSURE    "1401000000000000" //16位ID号。
  #define DEFAULT_CLOUD_SERVER_ADD_TEST (1) //默认采集地址测试。1,使用测试地址。0使用对应产品的地址。
  #if DEFAULT_CLOUD_SERVER_ADD_TEST
    #define DEFAULT_CLOUD_SERVER_ADD 	                "123.159.193.22,8231"//个人电脑测试平台。
//    #define DEFAULT_CLOUD_SERVER_ADD 	                "39.108.148.248,30007"  //水压统一平台
  #else
    #define DEFAULT_CLOUD_SERVER_ADD 	                "39.108.91.208,20008"
  #endif
  #define    	POST_VER		   	0x13 //协议版本
  #define    	SYS_DEVICE_TYPE		   	0xD0 //设备类型
  
   #if SPEED_UP
    #if RTC_USE_LSE
      #define RTC_MIN 192   //32.768K的外部晶振，定时唤醒6秒唤醒，这里作为加速版本。
    #else
     #define RTC_MIN (63750/300)
    #endif
  #else
    #if RTC_USE_LSE
      #define RTC_MIN 1920   //32.768K的外部晶振，定时唤醒60秒唤醒，这里作为正常版本。
    #else
      #define RTC_MIN (63750/30)
    #endif
  #endif
 
  #if PRESSURE_MODE_SELECT
    #define PRESSURE_MODE (1000) //1mPa版本
  #else
    #define PRESSURE_MODE (1600) //1.6mPa版本
  #endif
  #define STATUS_NORMAL (0)       //设备状态正常
  #define STATUS_CLEAR_FLASH (5)  //设备状态，设备初始化状态。
  #define RTC_HEART (60) 
  #define EEPROM_HISTORY_ADDR (2)
#else
  #define APP_SELECT_PRESSURE (0)  //水压程序。
  #define APP_SELECT_COVER    (1)  //闷盖程序。
  #define RTC_USE_LSE (0) //RTC时钟选择，0内部晶振，1外部晶振
  #define RTC_CAN_BE_SET (0)
  #define ALARMTIME_EN (1)
  #define ENTER_LPW_ENABLE (1) //进入低功耗使能。
  #define SOFT_VERSION     "固件版本：V3.00 NB-IOT CDMA IAP" 
  #define SOFTWARE_VER     "V03.00"//上报平台的软件版本，上位机不能配置，写程序时注意修改
  #define HARDWARE_VERSION "硬件版本：V1.05" 
  #define    	POST_VER		   	0x12 //协议版本
  #define    	SYS_DEVICE_TYPE		   	0xD0 //设备类型
  #define APP_CAN_UPDATA (1) //APP程序上网标志。0,不能通讯，1，可以通讯
  #define SPEED_UP (0) //加速标志。0正常程序，1加速程序。
  #if SPEED_UP
  #define RTC_MIN (63750/30)
  //    #if RTC_USE_LSE
  //      #define RTC_MIN 192   //32.768K的外部晶振，定时唤醒6秒唤醒，这里作为加速版本。
  //    #else
  //     #define RTC_MIN (63750/300)
  //    #endif
  #else
  #define RTC_MIN (63750)
  //    #if RTC_USE_LSE
  //      #define RTC_MIN 1920   //32.768K的外部晶振，定时唤醒60秒唤醒，这里作为正常版本。
  //    #else
  //      #define RTC_MIN (63750/30)
  //    #endif
  #endif
  #define WDG_EN  (1) //看门狗使能1，失能0 
  #define DEVICE_ID                                       "1400000000000000" //16位ID号。
  #define DEFAULT_CLOUD_SERVER_ADD_TEST (0) //默认采集地址测试。1,使用测试地址。0使用对应产品的地址。
  #if DEFAULT_CLOUD_SERVER_ADD_TEST
    #define DEFAULT_CLOUD_SERVER_ADD 	                "123.159.193.22,8231"
  #else
    #define DEFAULT_CLOUD_SERVER_ADD 	                "39.108.91.208,20008"
  #endif
  #define STATUS_NORMAL (0)       //设备状态正常
  #define STATUS_CLEAR_FLASH (5)  //设备状态，设备初始化状态。
  #define RTC_HEART (60) 
  #define EEPROM_HISTORY_ADDR (2)
#endif

#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
//#define SOFT_VERSION     "固件版本：V4.00 NB-IOT CDMA IAP" 
//#define SOFTWARE_VER     "V04.00"//上报平台的软件版本，上位机不能配置，写程序时注意修改
//#define HARDWARE_VERSION "硬件版本：V1.02" 
//#define DEVICE_ID_PRESSURE    "1401000000000000" //16位ID号。
//#define    	POST_VER		   	0x13 //协议版本
//#define    	SYS_DEVICE_TYPE		   	0xD0 //设备类型
#endif

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
//#define SOFT_VERSION     "固件版本：V3.00 NB-IOT CDMA IAP" 
//#define SOFTWARE_VER     "V03.00"//上报平台的软件版本，上位机不能配置，写程序时注意修改
//#define HARDWARE_VERSION "硬件版本：V1.05" 
//#define    	POST_VER		   	0x12 //协议版本
//#define    	SYS_DEVICE_TYPE		   	0xD0 //设备类型
#endif

//#define APP_CAN_UPDATA (1) //APP程序上网标志。0,不能通讯，1，可以通讯
//#define STATUS_NORMAL (0)       //设备状态正常
//#define STATUS_CLEAR_FLASH (5)  //设备状态，设备初始化状态。

//#define SPEED_UP (1) //加速标志。0正常程序，1加速程序。
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
//  #if SPEED_UP
//    #if RTC_USE_LSE
//      #define RTC_MIN 192   //32.768K的外部晶振，定时唤醒6秒唤醒，这里作为加速版本。
//    #else
//     #define RTC_MIN (63750/300)
//    #endif
//  #else
//    #if RTC_USE_LSE
//      #define RTC_MIN 1920   //32.768K的外部晶振，定时唤醒60秒唤醒，这里作为正常版本。
//    #else
//      #define RTC_MIN (63750/30)
//    #endif
//  #endif
#endif

#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
//  #if SPEED_UP
//    #define RTC_MIN (63750/30)
////    #if RTC_USE_LSE
////      #define RTC_MIN 192   //32.768K的外部晶振，定时唤醒6秒唤醒，这里作为加速版本。
////    #else
////     #define RTC_MIN (63750/300)
////    #endif
//  #else
//    #define RTC_MIN (63750)
////    #if RTC_USE_LSE
////      #define RTC_MIN 1920   //32.768K的外部晶振，定时唤醒60秒唤醒，这里作为正常版本。
////    #else
////      #define RTC_MIN (63750/30)
////    #endif
//  #endif
#endif


//#define RTC_HEART (60) 
//#define SD3078_WIRTE_INIT_FLAG_EN (1)  //sd3078写入标志使能，0失能，1使能

//#define PRESSURE_MODE_SELECT 1

//#if PRESSURE_MODE_SELECT
//  #define PRESSURE_MODE (1000) //1mPa版本
//#else
//  #define PRESSURE_MODE (1600) //1mPa版本
//#endif



//#define WDG_EN  (1) //看门狗使能1，失能0 

//#define DEFAULT_CLOUD_SERVER_ADD_TEST (1) //默认采集地址测试。1,使用测试地址。0使用对应产品的地址。

#define DEFAULT_QECOND_QUERY_INTERVAL_SENCOND_COUNT     0
#define IOT_NUM                                         "00000000000000"  //?

//#if DEFAULT_CLOUD_SERVER_ADD_TEST
//#define DEFAULT_CLOUD_SERVER_ADD 	                "123.159.193.22,8231"
//#else
//#define DEFAULT_CLOUD_SERVER_ADD 	                "39.108.91.208,20008"
//#endif

//#define DEVICE_ID                                       "0000000000000000" //16位ID号。
/////////////////////////////////////////////////////////////////////////////////
#define SYS_OVERTIME_COUNT 60000 	//系统超时退出时间 1000 为2秒
//#define HEART_TIMECOUNT_SET 3000 //3000为24小时  125为1个小时左右 1为28.8秒左右
//#define HEART_TIMECOUNT_SET 3000 //临时测试使用

#define HEART_REPORT_EN //心跳包使能

///////////////////////发送平台选择///////////////////////////////////
#define MODOU_REAPORT_EN 
//#define TLINK_REAPORT_EN 
//#define REPORT_SEND_COUNT_EN //纯测试心跳包模式选择 
//#define REPOERT_TEMPDATA_EN //临时数据包打包上传
//////////////////////手动烧写不同ID需要修改的点///////////////////////////////
//#define DEVICE_ID               "20009"		//原测试平台 20001-20010


#define HEART_TIMECOUNT_SET 48 //30分钟1个



///////////////////南昌服务器////////////////
//#define DEVICE_ID            		"nchg004"    //内部测试用
//#define SERVER_ADD_PORT  		  	"182.106.128.246,2017" //南昌部署测试平台

//////////////////测试部服务器////////////////
//#define DEVICE_ID               "test01"    //手动修改ID号位置 APP和平台测试专用
//#define SERVER_ADD_PORT  		  "121.42.196.113,20007" //测试部平台

//////////////////测试部服务器////////////////
//#define DEVICE_ID               "80003"    //手动修改ID号位置 上海商务演示专用
//#define DEVICE_ID               "gaodiwen04"    //手动修改ID号位置 硬件测试专用 高低温测试专用段
//#define DEVICE_ID               "yingjian0202"    //手动修改ID号位置 硬件测试专用
//#define DEVICE_ID               "nchg001"    //南昌部署3台测试  nchg001-nchg003
//#define DEVICE_ID               "gaowen01"    //高温测试
//#define SERVER_ADD_PORT  		  "121.42.196.113,20008" //商务演示平台 专用 20170112带到上海的就是这个端口
					//cover.modoutech.com									//同时用于硬件测试
            
//#define SERVER_ADD_PORT         "123.159.193.22,7601"  //原测试服务器
//#define SERVER_ADD_PORT         "121.42.196.113,20009"  //原会议室测试服务器

//上海 180.169.9.90:6080

///////////////////////////////////////////////////////////////////////////
//////////////////////打印显示相关设置///////////////////////////////
//#define DISPLAY_DEBUG_EN 1    //打开调试信息输出
///////////////////////////////////////////////////////////////////////////








#endif
