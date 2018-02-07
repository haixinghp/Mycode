#ifndef _SA_PROTOCOL_H_
#define _SA_PROTOCOL_H_


#if APP_SELECT_COVER //一下代码是闷盖程序特有的代码
#define CONFIG_BEGIN_LEN                        (1) 
#define CONFIG_PROTOCOL_VER_LEN                 (2) 
#define CONFIG_DEVICE_ADD_LEN                   (2) 
#define CONFIG_DEVICE_TYPE_LEN                  (2) 
#define CONFIG_CMD_LEN                          (2) 
#define CONFIG_LEN_LEN                          (4) 

#define CONFIG_CHANNEL_COUNT_LEN                (4) //通道个数	UINT型	字符串个数 15
//分界符1	Uchar	分界符1固定为：0x2C            //19
#define CONFIG_BIT_FLAG_LEN                     (8) //通道有效标志	Long型	对应的Bitx数据是否有效
//分界符1	Uchar	分界符1固定为：0x2C
#define CONFIG_ID_LEN                           (32)//ID号	字符串	16位 
//分界符1	Uchar	分界符1固定为：0x2C
#define CONFIG_SOFT_VER_LEN                     (4) //软件版本	字符串	2位，如“10” 
//分界符1	Uchar	分界符1固定为：0x2C
#define CONFIG_HARDWARE_VER_LEN	                (4)//硬件版本	字符串	2位，如“10” 
//分界符1	Uchar	分界符1固定为：0x2C    //69
#define CONFIG_STATUS_LEN	                (2)//设备状态	Uchar	库存状态 运输状态 待装状态 正常状态 
//分界符1	Uchar	分界符1固定为：0x2C      73
#define CONFIG_SEND_TOTALCOUNT_LEN              (4)//Bit4	发送次数	UINT	0-65535   
//分界符1	Uchar	分界符1固定为：0x2C     79
#define CONFIG_ROTATION_TRIGGER_ANGLE_LEN       (2)//旋转变化角度	UINT	-360~360    	
//分界符1	Uchar	分界符1固定为：0x2C      85
#define CONFIG_ROTATION_TRIGGER_SENS_LEN        (2)//旋转灵敏度	Uchar	0-99      
//分界符1	Uchar	分界符1固定为：0x2C     89
#define CONFIG_LEAN_TRIGGER_ANGLE_LEN           (2)//倾斜变化角度	Uchar	-90~90 	
//分界符1	Uchar	分界符1固定为：0x2C     93
#define CONFIG_LEAN_TRIGGER_SENS_LEN	        (2)//倾斜灵敏度	Uchar	0-99       
//分界符1	Uchar	分界符1固定为：0x2C     97
#define CONFIG_PRESSURE_TRIGGER_VALUE_LEN       (4)//压力变化阈值	UINT	1-65535 
//分界符1	Uchar	分界符1固定为：0x2C     93
#define CONFIG_PRESSURE_INTERVAL_LEN            (4)//压力检测间隔	UINT	0-65535  
//分界符1	Uchar	分界符1固定为：0x2C     99 
#define CONFIG_HEART_INTERVAL_LEN               (4)//心跳间隔时间	UINT	0-65535时  
//分界符1	Uchar	分界符1固定为：0x2C     105
#define CONFIG_ALARM_INTERVAL_LEN               (4)//告警间隔时间	UINT	0-65535秒  
//分界符1	Uchar	分界符1固定为：0x2C     110
#define CONFIG_TRIGGER_INTERVAL_LEN             (4)//检测间隔时间	UINT	0-65535秒
//分界符1	Uchar	分界符1固定为：0x2C     116
#define CONFIG_CLOUD_SERVER_ADD_LEN             (42)//可配采集地址	字符串	最长21    
//分界符1	Uchar	分界符1固定为：0x2C     160
#define CONFIG_UPDATE_SERVER_LEN                (42)//升级服务器地址	字符串	最长21    
//分界符1	Uchar	分界符1固定为：0x2C     204
#define CONFIG_CARRIER_TYPE_LEN                 (2)//运营商	Uchar	0电信 1移动 2联通  
//分界符1	Uchar	分界符1固定为：0x2C     
#define CONFIG_MODULE_TYPE_LEN                  (2)//网络制式	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
//分界符1	Uchar	分界符1固定为：0x2C   
#define CONFIG_CARD_ID_LEN                      (40)//物联网卡号。20
//分界符1	Uchar	分界符1固定为：0x2C
#define CONFIG_TIMESTAMP_LEN                    (8)//时间戳 4 1970年01月01日8点0分0秒到现在的秒数，如1498010100 
//分界符1	Uchar	分界符1固定为：0x2C
#define CONFIG_PRESSURE_UPLIMIT_LEN             (4)//压力上限	UINT	0~65535
//分界符1	Uchar	分界符1固定为：0x2C
#define CONFIG_PRESSURE_DOWNLIMIT_LEN          (4)//压力下限	UINT	0~65535
//分界符1	Uchar	分界符1固定为：0x2C

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

#define CONFIG_CHANNEL_COUNT_ADDR 	        (CONFIG_LEN_LEN+CONFIG_LEN_ADDR)  //通道个数	UINT型	字符串个数 地址13

#define CONFIG_BIT_FLAG_ADDR		        (CONFIG_CHANNEL_COUNT_LEN+CONFIG_CHANNEL_COUNT_ADDR+CONFIG_0X2C)  //通道有效标志	Long型	对应的Bitx数据是否有效 地址19

#define CONFIG_ID_ADDR 			        (CONFIG_BIT_FLAG_LEN+CONFIG_BIT_FLAG_ADDR+CONFIG_0X2C)  //ID号	字符串	16位 地址29

#define CONFIG_SOFT_VER_ADDR		        (CONFIG_ID_LEN+CONFIG_ID_ADDR+CONFIG_0X2C)  //软件版本	字符串	2位，如“10”地址63

#define CONFIG_HARDWARE_VER_ADDR	        (CONFIG_SOFT_VER_LEN+CONFIG_SOFT_VER_ADDR+CONFIG_0X2C)  //硬件版本	字符串	2位，如“10”

#define CONFIG_STATUS_ADDR	                (CONFIG_HARDWARE_VER_LEN+CONFIG_HARDWARE_VER_ADDR+CONFIG_0X2C)//设备状态	Uchar	库存状态 运输状态 待装状态 正常状态 固件更新等待状态

#define CONFIG_SEND_TOTALCOUNT_ADDR             (CONFIG_STATUS_LEN+CONFIG_STATUS_ADDR+CONFIG_0X2C)//Bit4	发送次数	UINT	0-65535

#define CONFIG_ROTATION_TRIGGER_ANGLE_ADDR	(CONFIG_SEND_TOTALCOUNT_LEN+CONFIG_SEND_TOTALCOUNT_ADDR+CONFIG_0X2C)//旋转变化角度	UINT	-360~360

#define CONFIG_ROTATION_TRIGGER_SENS_ADDR	(CONFIG_ROTATION_TRIGGER_ANGLE_LEN+CONFIG_ROTATION_TRIGGER_ANGLE_ADDR+CONFIG_0X2C)//旋转灵敏度	Uchar	0-99

#define CONFIG_LEAN_TRIGGER_ANGLE_ADDR          (CONFIG_ROTATION_TRIGGER_SENS_LEN+CONFIG_ROTATION_TRIGGER_SENS_ADDR+CONFIG_0X2C)//倾斜变化角度	Uchar	-90~90度

#define CONFIG_LEAN_TRIGGER_SENS_ADDR	        (CONFIG_LEAN_TRIGGER_ANGLE_LEN+CONFIG_LEAN_TRIGGER_ANGLE_ADDR+CONFIG_0X2C)//倾斜灵敏度	Uchar	0-99

#define CONFIG_PRESSURE_TRIGGER_VALUE_ADDR	(CONFIG_LEAN_TRIGGER_SENS_LEN+CONFIG_LEAN_TRIGGER_SENS_ADDR+CONFIG_0X2C)//压力变化阈值	UINT	1-65535

#define CONFIG_PRESSURE_INTERVAL_ADDR	        (CONFIG_PRESSURE_TRIGGER_VALUE_LEN+CONFIG_PRESSURE_TRIGGER_VALUE_ADDR+CONFIG_0X2C)//压力检测间隔	UINT	0-65535秒

#define CONFIG_HEART_INTERVAL_ADDR	        (CONFIG_PRESSURE_INTERVAL_LEN+CONFIG_PRESSURE_INTERVAL_ADDR+CONFIG_0X2C)//心跳间隔时间	UINT	0-65535时

#define CONFIG_ALARM_INTERVAL_ADDR	        (CONFIG_HEART_INTERVAL_LEN+CONFIG_HEART_INTERVAL_ADDR+CONFIG_0X2C)//告警间隔时间	UINT	0-65535秒

#define CONFIG_TRIGGER_INTERVAL_ADDR		(CONFIG_ALARM_INTERVAL_LEN+CONFIG_ALARM_INTERVAL_ADDR+CONFIG_0X2C)//检测间隔时间	UINT	0-65535秒

#define CONFIG_CLOUD_SERVER_ADD_ADDR		(CONFIG_TRIGGER_INTERVAL_LEN+CONFIG_TRIGGER_INTERVAL_ADDR+CONFIG_0X2C)//可配采集地址	字符串	最长21

#define CONFIG_UPDATE_SERVER_ADDR 		(CONFIG_CLOUD_SERVER_ADD_LEN+CONFIG_CLOUD_SERVER_ADD_ADDR+CONFIG_0X2C)//升级服务器地址	字符串	最长21

#define CONFIG_CARRIER_TYPE_ADDR 		(CONFIG_UPDATE_SERVER_LEN+CONFIG_UPDATE_SERVER_ADDR+CONFIG_0X2C)//运营商	Uchar	0电信 1移动 2联通

#define CONFIG_MODULE_TYPE_ADDR 		(CONFIG_CARRIER_TYPE_LEN+CONFIG_CARRIER_TYPE_ADDR+CONFIG_0X2C)//网络制式	Uchar	0:2G 1:3G 2:4G3:NB-IOT

#define CONFIG_CARD_ID_ADDR                     (CONFIG_MODULE_TYPE_LEN+CONFIG_MODULE_TYPE_ADDR+CONFIG_0X2C)  //物联网卡号。                     

#define CONFIG_TIMESTAMP_ADDR                   (CONFIG_CARD_ID_LEN+CONFIG_CARD_ID_ADDR+CONFIG_0X2C)  //时间戳 4 1970年01月01日8点0分0秒到现在的秒数，如1498010100 

#define CONFIG_PRESSURE_UPLIMIT_ADDR            (CONFIG_TIMESTAMP_LEN+CONFIG_TIMESTAMP_ADDR+CONFIG_0X2C) //压力上限	UINT	0~65535

#define CONFIG_PRESSURE_DOWNLIMIT_ADDR          (CONFIG_PRESSURE_UPLIMIT_LEN+CONFIG_PRESSURE_UPLIMIT_ADDR+CONFIG_0X2C) //压力上限	UINT	0~65535

                               

#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
#define CONFIG_DATAINFO_LEN                     (CONFIG_PRESSURE_DOWNLIMIT_LEN+CONFIG_PRESSURE_DOWNLIMIT_ADDR+CONFIG_0X2C) //DATAINFO长度。
#define CONFIG_LEN                              (CONFIG_DATAINFO_LEN+CONFIG_CHKSUM_LEN+CONFIG_EOI_LEN) //通讯数据数据总长度。消防栓276
#endif

#if APP_SELECT_COVER    //一下代码是水压程序特有的代码
#define CONFIG_DATAINFO_LEN                     ((CONFIG_CARD_ID_LEN+CONFIG_CARD_ID_ADDR+CONFIG_0X2C)) //DATAINFO长度。 
#define CONFIG_LEN                              (CONFIG_DATAINFO_LEN+CONFIG_CHKSUM_LEN+CONFIG_EOI_LEN) //通讯数据数据总长度。消防栓276
#endif

#define PROTOCOL_CONFIG_TOTAL_LEN (CONFIG_LEN)

#define BIT_1   ((u32)1)
#define BIT_CONFIG_ID 			              	(BIT_1<<0)  //ID号	字符串	16位                     
#define BIT_CONFIG_SOFT_VER 		          	(BIT_1<<1)  //软件版本	字符串	2位，如“10”                    
#define BIT_CONFIG_HARDWARE_VER 	        	(BIT_1<<2)  //硬件版本	字符串	2位，如“10”              
#define BIT_CONFIG_STATUS 	              	        (BIT_1<<3)  //设备状态	Uchar	库存状态 运输状态 待装状态 正常状态 
#define BIT_CONFIG_SEND_TOTALCOUNT        	        (BIT_1<<4)  //Bit4	发送次数	UINT	0-65535           
#define BIT_CONFIG_ROTATION_TRIGGER_ANGLE 	        (BIT_1<<5)  //旋转变化角度	UINT	-360~360    
#define BIT_CONFIG_ROTATION_TRIGGER_SENS 		(BIT_1<<6)  //旋转灵敏度	Uchar	0-99      
#define BIT_CONFIG_LEAN_TRIGGER_ANGLE    	        (BIT_1<<7)  //倾斜变化角度	Uchar	-90~90
#define BIT_CONFIG_LEAN_TRIGGER_SENS 	    	        (BIT_1<<8)  //倾斜灵敏度	Uchar	0-99        
#define BIT_CONFIG_PRESSURE_TRIGGER_VALUE 	        (BIT_1<<9)  //压力变化阈值	UINT	1-65535       
#define BIT_CONFIG_PRESSURE_INTERVAL 	    	        (BIT_1<<10) //压力检测间隔	UINT	0-65535
#define BIT_CONFIG_HEART_INTERVAL 	      	        (BIT_1<<11) //心跳间隔时间	UINT	0-65535时    
#define BIT_CONFIG_ALARM_INTERVAL 	      	        (BIT_1<<12) //告警间隔时间	UINT	0-65535秒        
#define BIT_CONFIG_TRIGGER_INTERVAL 		  	(BIT_1<<13) //检测间隔时间	UINT	0-65535秒        
#define BIT_CONFIG_CLOUD_SERVER_ADD 		  	(BIT_1<<14) //可配采集地址	字符串	最长21            
#define BIT_CONFIG_UPDATE_SERVER 		    	(BIT_1<<15) //升级服务器地址	字符串	最长21        
#define BIT_CONFIG_CARRIER_TYPE 		        (BIT_1<<16) //运营商	Uchar	0电信 1移动 2联通           
#define BIT_CONFIG_MODULE_TYPE 		          	(BIT_1<<17) //网络制式	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
#define BIT_CONFIG_CARD_ID                              (BIT_1<<18) //物联网卡号。20  
#define BIT_TIMESTAMP_ID                                (BIT_1<<19) //时间戳 4 1970年01月01日8点0分0秒到现在的秒数，如1498010100  
#define BIT_PRESSURE_UPLIMIT_ID                         (BIT_1<<20) //压力上限    UINT    0~65535
#define BIT_PRESSURE_DOWNLIMIT_ID                       (BIT_1<<21) //压力下限    UINT    0~65535
#endif


#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
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
//#define CONFIG_CHANNEL_COUNT_ADDR 	        (CONFIG_LEN_LEN+CONFIG_LEN_ADDR)  //通道个数	UINT型	字符串个数 
//#define CONFIG_BIT_FLAG_ADDR		        (CONFIG_CHANNEL_COUNT_ADDR+8+CONFIG_0X2C)  //通道有效标志	Long型	对应的Bitx数据是否有效 
//#define CONFIG_ID_ADDR 			        (CONFIG_BIT_FLAG_ADDR+32+CONFIG_0X2C)  //ID号	字符串	16位 
//#define CONFIG_SOFT_VER_ADDR                    (CONFIG_ID_ADDR+4+CONFIG_0X2C)  //软件版本    字符串    2位，如“10”
//#define CONFIG_HARDWARE_VER_ADDR                (CONFIG_SOFT_VER_ADDR+4+CONFIG_0X2C)  //硬件版本    字符串    2位，如“10”
//#define CONFIG_STATUS_ADDR                      (CONFIG_HARDWARE_VER_ADDR+2+CONFIG_0X2C)//设备状态    Uchar    库存状态 运输状态 待装状态 正常状态 固件更新等待状态
//#define CONFIG_SEND_TOTALCOUNT_ADDR             (CONFIG_STATUS_ADDR+4+CONFIG_0X2C)//Bit4    发送次数    UINT    0-65535
//#define CONFIG_PRESSURE_TRIGGER_VALUE_ADDR      (CONFIG_SEND_TOTALCOUNT_ADDR+4+CONFIG_0X2C)//压力变化分辨率    UINT    1-65535
//#define CONFIG_HEART_INTERVAL_ADDR              (CONFIG_PRESSURE_TRIGGER_VALUE_ADDR+4+CONFIG_0X2C)//心跳间隔时间    UINT    0-65535时
//#define CONFIG_ALARM_ENABLE_BIT_ADDR            (CONFIG_HEART_INTERVAL_ADDR+4+CONFIG_0X2C) ////告警使能0失能，1使能 UINT
//#define CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL_ADDR       (CONFIG_ALARM_ENABLE_BIT_ADDR+4+CONFIG_0X2C) ///上限告警间隔时间 UINT
//#define CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL_ADDR    (CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //下限告警间隔时间 UINT
//#define CONFIG_NO_SENSOR_ALARM_INTERVAL_ADDR              (CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //未检测到传感器告警间隔 UINT
//#define CONFIG_RESOLUTION_ALARM_INTERVAL_ADDR             (CONFIG_NO_SENSOR_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //分辨率上报告警间隔 UINT
//#define CONFIG_TOUCHOFF_ALARM_INTERVAL_ADDR               (CONFIG_RESOLUTION_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //人工触发上报间隔 UINT
//#define CONFIG_TRIGGER_INTERVAL_ADDR            (CONFIG_TOUCHOFF_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C)//检测间隔时间    UINT    0-65535秒
//#define CONFIG_CLOUD_SERVER_ADD_ADDR            (CONFIG_TRIGGER_INTERVAL_ADDR+42+CONFIG_0X2C)//可配采集地址    字符串    最长21
//#define CONFIG_UPDATE_SERVER_ADDR               (CONFIG_CLOUD_SERVER_ADD_ADDR+42+CONFIG_0X2C)//升级服务器地址    字符串    最长21
//#define CONFIG_CARRIER_TYPE_ADDR                (CONFIG_UPDATE_SERVER_ADDR+2+CONFIG_0X2C)//运营商    Uchar    0电信 1移动 2联通
//#define CONFIG_MODULE_TYPE_ADDR                 (CONFIG_CARRIER_TYPE_ADDR+2+CONFIG_0X2C)//网络制式    Uchar    0:2G 1:3G 2:4G3:NB-IOT
//#define CONFIG_CARD_ID_ADDR                     (CONFIG_MODULE_TYPE_ADDR+40+CONFIG_0X2C)  //物联网卡号。                    
//#define CONFIG_TIMESTAMP_ADDR                   (CONFIG_CARD_ID_ADDR+8+CONFIG_0X2C)  //时间戳 4 1970年01月01日8点0分0秒到现在的秒数，如1498010100
//#define CONFIG_PRESSURE_UPLIMIT_ADDR            (CONFIG_TIMESTAMP_ADDR+4+CONFIG_0X2C) //压力上限    UINT    0~65535
//#define CONFIG_PRESSURE_DOWNLIMIT_ADDR          (CONFIG_PRESSURE_UPLIMIT_ADDR+4+CONFIG_0X2C) //压力下限
//#define CONFIG_DATAINFO_LEN                     (CONFIG_PRESSURE_DOWNLIMIT_ADDR+4+CONFIG_0X2C) //DATAINFO长度。
//#define CONFIG_LEN                              (CONFIG_DATAINFO_LEN+CONFIG_CHKSUM_LEN+CONFIG_EOI_LEN) //通讯数据数据总长度。消防栓276
#define CONFIG_BEGIN_ADDR	                (0)     
#define CONFIG_PROTOCOL_VER_ADDR	        (CONFIG_BEGIN_LEN)  //
#define CONFIG_DEVICE_ADD_ADDR		        (CONFIG_PROTOCOL_VER_LEN+CONFIG_PROTOCOL_VER_ADDR)
#define CONFIG_DEVICE_TYPE_ADDR		        (CONFIG_DEVICE_ADD_LEN+CONFIG_DEVICE_ADD_ADDR)
#define CONFIG_CMD_ADDR			        (CONFIG_DEVICE_TYPE_LEN+CONFIG_DEVICE_TYPE_ADDR)
#define CONFIG_LEN_ADDR 		        (CONFIG_CMD_LEN+CONFIG_CMD_ADDR)
#define CONFIG_CHANNEL_COUNT_ADDR 	        (CONFIG_LEN_LEN+CONFIG_LEN_ADDR)  //通道个数	UINT型	字符串个数 
#define CONFIG_BIT_FLAG_ADDR		        (CONFIG_CHANNEL_COUNT_ADDR+4+CONFIG_0X2C)  //通道有效标志	Long型	对应的Bitx数据是否有效 
#define CONFIG_ID_ADDR 			        (CONFIG_BIT_FLAG_ADDR+8+CONFIG_0X2C)  //ID号	字符串	16位 
#define CONFIG_SOFT_VER_ADDR                    (CONFIG_ID_ADDR+32+CONFIG_0X2C)  //软件版本    字符串    2位，如“10”
#define CONFIG_HARDWARE_VER_ADDR                (CONFIG_SOFT_VER_ADDR+4+CONFIG_0X2C)  //硬件版本    字符串    2位，如“10”
#define CONFIG_STATUS_ADDR                      (CONFIG_HARDWARE_VER_ADDR+4+CONFIG_0X2C)//设备状态    Uchar    库存状态 运输状态 待装状态 正常状态 固件更新等待状态
#define CONFIG_SEND_TOTALCOUNT_ADDR             (CONFIG_STATUS_ADDR+2+CONFIG_0X2C)//Bit4    发送次数    UINT    0-65535
#define CONFIG_PRESSURE_TRIGGER_VALUE_ADDR      (CONFIG_SEND_TOTALCOUNT_ADDR+4+CONFIG_0X2C)//压力变化分辨率    UINT    1-65535
#define CONFIG_HEART_INTERVAL_ADDR              (CONFIG_PRESSURE_TRIGGER_VALUE_ADDR+4+CONFIG_0X2C)//心跳间隔时间    UINT    0-65535时
#define CONFIG_ALARM_ENABLE_BIT_ADDR            (CONFIG_HEART_INTERVAL_ADDR+4+CONFIG_0X2C) ////告警使能0失能，1使能 UINT
#define CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL_ADDR       (CONFIG_ALARM_ENABLE_BIT_ADDR+4+CONFIG_0X2C) ///上限告警间隔时间 UINT
#define CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL_ADDR    (CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //下限告警间隔时间 UINT
#define CONFIG_NO_SENSOR_ALARM_INTERVAL_ADDR              (CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //未检测到传感器告警间隔 UINT
#define CONFIG_RESOLUTION_ALARM_INTERVAL_ADDR             (CONFIG_NO_SENSOR_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //分辨率上报告警间隔 UINT
#define CONFIG_TOUCHOFF_ALARM_INTERVAL_ADDR               (CONFIG_RESOLUTION_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C) //人工触发上报间隔 UINT
#define CONFIG_TRIGGER_INTERVAL_ADDR            (CONFIG_TOUCHOFF_ALARM_INTERVAL_ADDR+4+CONFIG_0X2C)//检测间隔时间    UINT    0-65535秒
#define CONFIG_CLOUD_SERVER_ADD_ADDR            (CONFIG_TRIGGER_INTERVAL_ADDR+4+CONFIG_0X2C)//可配采集地址    字符串    最长21
#define CONFIG_UPDATE_SERVER_ADDR               (CONFIG_CLOUD_SERVER_ADD_ADDR+42+CONFIG_0X2C)//升级服务器地址    字符串    最长21
#define CONFIG_CARRIER_TYPE_ADDR                (CONFIG_UPDATE_SERVER_ADDR+42+CONFIG_0X2C)//运营商    Uchar    0电信 1移动 2联通
#define CONFIG_MODULE_TYPE_ADDR                 (CONFIG_CARRIER_TYPE_ADDR+2+CONFIG_0X2C)//网络制式    Uchar    0:2G 1:3G 2:4G3:NB-IOT
#define CONFIG_CARD_ID_ADDR                     (CONFIG_MODULE_TYPE_ADDR+2+CONFIG_0X2C)  //物联网卡号。                    
#define CONFIG_TIMESTAMP_ADDR                   (CONFIG_CARD_ID_ADDR+40+CONFIG_0X2C)  //时间戳 4 1970年01月01日8点0分0秒到现在的秒数，如1498010100
#define CONFIG_PRESSURE_UPLIMIT_ADDR            (CONFIG_TIMESTAMP_ADDR+8+CONFIG_0X2C) //压力上限    UINT    0~65535
#define CONFIG_PRESSURE_DOWNLIMIT_ADDR          (CONFIG_PRESSURE_UPLIMIT_ADDR+4+CONFIG_0X2C) //压力下限
#define CONFIG_DATAINFO_LEN                     (CONFIG_PRESSURE_DOWNLIMIT_ADDR+4+CONFIG_0X2C) //DATAINFO长度。
#define CONFIG_LEN                              (CONFIG_DATAINFO_LEN+CONFIG_CHKSUM_LEN+CONFIG_EOI_LEN) //通讯数据数据总长度。消防栓276

#define PROTOCOL_CONFIG_TOTAL_LEN (CONFIG_LEN)

#define BIT_1   ((u32)1)
#define BIT_CONFIG_ID 			              	(BIT_1<<0)  //ID号	字符串	16位                     
#define BIT_CONFIG_SOFT_VER 		          	(BIT_1<<1)  //软件版本	字符串	2位，如“10”                    
#define BIT_CONFIG_HARDWARE_VER 	        	(BIT_1<<2)  //硬件版本	字符串	2位，如“10”              
#define BIT_CONFIG_STATUS 	              	        (BIT_1<<3)  //设备状态	Uchar	库存状态 运输状态 待装状态 正常状态 
#define BIT_CONFIG_SEND_TOTALCOUNT        	        (BIT_1<<4)  //Bit4	发送次数	UINT	0-65535           
#define BIT_CONFIG_PRESSURE_TRIGGER_VALUE 	        (BIT_1<<5)  //Bit5	压力变化分辨率	UINT	1-65535 
#define BIT_CONFIG_HEART_INTERVAL        		(BIT_1<<6)  //Bit6	心跳间隔时间	UINT	0-65535时
#define BIT_CONFIG_ALARM_ENABLE_BIT          	        (BIT_1<<7)  //Bit7	告警使能	2
#define BIT_CONFIG_PRESSURE_UPLIMIT_ALARM_INTERVAL	(BIT_1<<8)  //Bit8	上限告警间隔时间	2	1~65534 分钟65535表示取消
#define BIT_CONFIG_PRESSURE_DOWN_LIMIT_ALARM_INTERVAL	(BIT_1<<9)  //Bit9	下限告警间隔时间	2	1~65534 分钟65535表示取消   
#define BIT_CONFIG_NO_SENSOR_ALARM_INTERVAL	    	(BIT_1<<10) //Bit10	未检测到传感器告警间隔。	2	1~65534 分钟65535表示取消
#define BIT_CONFIG_RESOLUTION_ALARM_INTERVAL	      	(BIT_1<<11) //Bit11	分辨率上报告警间隔	2	1~65534 分钟65535表示取消
#define BIT_CONFIG_TOUCHOFF_ALARM_INTERVAL 	      	(BIT_1<<12) //Bit12	人工触发上报间隔	2	1~65534 分钟65535表示取消
#define BIT_CONFIG_TRIGGER_INTERVAL 		  	(BIT_1<<13) //检测间隔时间	UINT	0-65535秒        
#define BIT_CONFIG_CLOUD_SERVER_ADD 		  	(BIT_1<<14) //可配采集地址	字符串	最长21            
#define BIT_CONFIG_UPDATE_SERVER 		    	(BIT_1<<15) //升级服务器地址	字符串	最长21        
#define BIT_CONFIG_CARRIER_TYPE 		        (BIT_1<<16) //运营商	Uchar	0电信 1移动 2联通           
#define BIT_CONFIG_MODULE_TYPE 		          	(BIT_1<<17) //网络制式	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
#define BIT_CONFIG_CARD_ID                              (BIT_1<<18) //物联网卡号。20  
#define BIT_TIMESTAMP_ID                                (BIT_1<<19) //时间戳 4 1970年01月01日8点0分0秒到现在的秒数，如1498010100  
#define BIT_PRESSURE_UPLIMIT_ID                         (BIT_1<<20) //压力上限    UINT    0~65535
#define BIT_PRESSURE_DOWNLIMIT_ID                       (BIT_1<<21) //压力下限    UINT    0~65535
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
#define CONFIG_ALARM_S_COUNT_ADDR	89 //触发报警间隔
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
//#define CONFIG_CHANNEL_COUNT_LEN                (4) //通道个数	UINT型	字符串个数 15
////分界符1	Uchar	分界符1固定为：0x2C            //19
//#define CONFIG_BIT_FLAG_LEN                     (8) //通道有效标志	Long型	对应的Bitx数据是否有效
////分界符1	Uchar	分界符1固定为：0x2C
//#define CONFIG_ID_LEN                           (32)//ID号	字符串	16位 
////分界符1	Uchar	分界符1固定为：0x2C
//#define CONFIG_SOFT_VER_LEN                     (4) //软件版本	字符串	2位，如“10” 
////分界符1	Uchar	分界符1固定为：0x2C
//#define CONFIG_HARDWARE_VER_LEN	                (4)//硬件版本	字符串	2位，如“10” 
////分界符1	Uchar	分界符1固定为：0x2C    //69
//#define CONFIG_STATUS_LEN	                (2)//设备状态	Uchar	库存状态 运输状态 待装状态 正常状态 
////分界符1	Uchar	分界符1固定为：0x2C      73
//#define CONFIG_SEND_TOTALCOUNT_LEN              (4)//Bit4	发送次数	UINT	0-65535   
////分界符1	Uchar	分界符1固定为：0x2C     79
//#define CONFIG_ROTATION_TRIGGER_ANGLE_LEN       (2)//旋转变化角度	UINT	-360~360    	
////分界符1	Uchar	分界符1固定为：0x2C      85
//#define CONFIG_ROTATION_TRIGGER_SENS_LEN        (2)//旋转灵敏度	Uchar	0-99      
////分界符1	Uchar	分界符1固定为：0x2C     89
//#define CONFIG_LEAN_TRIGGER_ANGLE_LEN           (2)//倾斜变化角度	Uchar	-90~90 	
////分界符1	Uchar	分界符1固定为：0x2C     93
//#define CONFIG_LEAN_TRIGGER_SENS_LEN	        (2)//倾斜灵敏度	Uchar	0-99       
////分界符1	Uchar	分界符1固定为：0x2C     97
//#define CONFIG_PRESSURE_TRIGGER_VALUE_LEN       (4)//压力变化阈值	UINT	1-65535 
////分界符1	Uchar	分界符1固定为：0x2C     93
//#define CONFIG_PRESSURE_INTERVAL_LEN            (4)//压力检测间隔	UINT	0-65535  
////分界符1	Uchar	分界符1固定为：0x2C     99 
//#define CONFIG_HEART_INTERVAL_LEN               (4)//心跳间隔时间	UINT	0-65535时  
////分界符1	Uchar	分界符1固定为：0x2C     105
//#define CONFIG_ALARM_INTERVAL_LEN               (4)//告警间隔时间	UINT	0-65535秒  
////分界符1	Uchar	分界符1固定为：0x2C     110
//#define CONFIG_TRIGGER_INTERVAL_LEN             (4)//检测间隔时间	UINT	0-65535秒
////分界符1	Uchar	分界符1固定为：0x2C     116
//#define CONFIG_CLOUD_SERVER_ADD_LEN             (42)//可配采集地址	字符串	最长21    
////分界符1	Uchar	分界符1固定为：0x2C     160
//#define CONFIG_UPDATE_SERVER_LEN                (42)//升级服务器地址	字符串	最长21    
////分界符1	Uchar	分界符1固定为：0x2C     204
//#define CONFIG_CARRIER_TYPE_LEN                 (2)//运营商	Uchar	0电信 1移动 2联通  
////分界符1	Uchar	分界符1固定为：0x2C     
//#define CONFIG_MODULE_TYPE_LEN                  (2)//网络制式	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
////分界符1	Uchar	分界符1固定为：0x2C   
//#define CONFIG_CARD_ID_LEN                      (40)//物联网卡号。20
////分界符1	Uchar	分界符1固定为：0x2C
//#define CONFIG_TIMESTAMP_LEN                    (8)//时间戳 4 1970年01月01日8点0分0秒到现在的秒数，如1498010100 
////分界符1	Uchar	分界符1固定为：0x2C
//#define CONFIG_PRESSURE_UPLIMIT_LEN             (4)//压力上限	UINT	0~65535
////分界符1	Uchar	分界符1固定为：0x2C
//#define CONFIG_PRESSURE_DOWNLIMIT_LEN          (4)//压力下限	UINT	0~65535
////分界符1	Uchar	分界符1固定为：0x2C
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
//#define CONFIG_ALARM_S_COUNT_ADDR         CONFIG_HEART_S_COUNT_ADDR+(2<<1)+2 //触发报警间隔
//#define CONFIG_SQ_S_COUNT_ADDR            CONFIG_ALARM_S_COUNT_ADDR+(2<<1)+2 //二次确认间隔
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
//#define BIT_CONFIG_ID 			              	(BIT_1<<0)  //ID号	字符串	16位                     
//#define BIT_CONFIG_SOFT_VER 		          	(BIT_1<<1)  //软件版本	字符串	2位，如“10”                    
//#define BIT_CONFIG_HARDWARE_VER 	        	(BIT_1<<2)  //硬件版本	字符串	2位，如“10”              
//#define BIT_CONFIG_STATUS 	              	        (BIT_1<<3)  //设备状态	Uchar	库存状态 运输状态 待装状态 正常状态 
//#define BIT_CONFIG_SEND_TOTALCOUNT        	        (BIT_1<<4)  //Bit4	发送次数	UINT	0-65535           
//#define BIT_CONFIG_ROTATION_TRIGGER_ANGLE 	        (BIT_1<<5)  //旋转变化角度	UINT	-360~360    
//#define BIT_CONFIG_ROTATION_TRIGGER_SENS 		(BIT_1<<6)  //旋转灵敏度	Uchar	0-99      
//#define BIT_CONFIG_LEAN_TRIGGER_ANGLE    	        (BIT_1<<7)  //倾斜变化角度	Uchar	-90~90
//#define BIT_CONFIG_LEAN_TRIGGER_SENS 	    	        (BIT_1<<8)  //倾斜灵敏度	Uchar	0-99        
//#define BIT_CONFIG_PRESSURE_TRIGGER_VALUE 	        (BIT_1<<9)  //压力变化阈值	UINT	1-65535       
//#define BIT_CONFIG_PRESSURE_INTERVAL 	    	        (BIT_1<<10) //压力检测间隔	UINT	0-65535
//#define BIT_CONFIG_HEART_INTERVAL 	      	        (BIT_1<<11) //心跳间隔时间	UINT	0-65535时    
//#define BIT_CONFIG_ALARM_INTERVAL 	      	        (BIT_1<<12) //告警间隔时间	UINT	0-65535秒        
//#define BIT_CONFIG_TRIGGER_INTERVAL 		  	(BIT_1<<13) //检测间隔时间	UINT	0-65535秒        
//#define BIT_CONFIG_CLOUD_SERVER_ADD 		  	(BIT_1<<14) //可配采集地址	字符串	最长21            
//#define BIT_CONFIG_UPDATE_SERVER 		    	(BIT_1<<15) //升级服务器地址	字符串	最长21        
//#define BIT_CONFIG_CARRIER_TYPE 		        (BIT_1<<16) //运营商	Uchar	0电信 1移动 2联通           
//#define BIT_CONFIG_MODULE_TYPE 		          	(BIT_1<<17) //网络制式	Uchar	0:2G 1:3G 2:4G3:NB-IOT   
//#define BIT_CONFIG_CARD_ID                              (BIT_1<<18) //物联网卡号。20  
//#define BIT_TIMESTAMP_ID                                (BIT_1<<19) //时间戳 4 1970年01月01日8点0分0秒到现在的秒数，如1498010100  
//#define BIT_PRESSURE_UPLIMIT_ID                         (BIT_1<<20) //压力上限    UINT    0~65535
//#define BIT_PRESSURE_DOWNLIMIT_ID                       (BIT_1<<21) //压力下限    UINT    0~65535
//配置参数有效标志位
//#define BIT_CONFIG_ID 				(1<<0)
//#define BIT_CONFIG_SOFT_VER			(1<<1)
//#define BIT_CONFIG_HARDWARE_VER		(1<<2)
//
//#define BIT_CONFIG_TRIGGER_ANGLE	(1<<3)
//#define BIT_CONFIG_TRIGGER_SEN		(1<<4)
//#define BIT_CONFIG_HEART_S_COUNT	(1<<5)
//#define BIT_CONFIG_ALARM_S_COUNT	(1<<6)
//#define BIT_CONFIG_SQ_S_COUNT  (1<<7)//二次确认间隔时间
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
u8 device_id[17];//设备ID
  u8 soft_version[3];//软件版本
  u8 harware_version[3];//硬件版本
  u8 trigger_angle;//触发角度
  u8 trigger_sensitivity;//触发灵敏度
  u16 heartdata_sent_interval_minute_count;//心跳间隔时间
  u16 alarm_sent_interval_second_count;//报警间隔时间
  u16 send_totalcount;//电池发送总次数
  s8 temp_adjust_value;//温度修正点
  s16 temp_offset;//温度修正值
  u8 update_server_add[22];//升级服务器地址255.255.255.255,65535
  u8 cloud_server_add[22];//云端服务器地址255.255.255.255,65535
  u8 default_cloud_server_add[22];//默认云端服务器地址255.255.255.255,65535
  u8 carrier_type;//运营商   不可修改 绑定硬件版本
  u8 module_type;//模块制式 不可修改 绑定硬件版本
*/

typedef struct
{
        unsigned char Mode_Restart : 4; //模块重启准备标志
        unsigned char reconnect_count : 4; //重复连接次数
        unsigned char send_count : 8;  //发送次数
        
	unsigned char initStep : 5;
        unsigned char ack_config : 1;//回复服务器端标志
        unsigned char send_success_flag : 1;//消息发送成功标志
	unsigned char cardType : 1; //手机卡为1，物联卡为0
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
  RTN_OK              = 0x00, /*!< 正常*/
  RTN_ERR_VER         = 0x01, /*!< VER错误 */
  RTN_ERR_CHKSUM      = 0x02, /*!< CHKSUM错误 */
  RTN_ERR_LCHKSUM     = 0x03, /*!< LCHKSUM错误 */
  RTN_ERR_CID2        = 0x04, /*!< CID2无效 */
  RTN_ERR_FORMAT      = 0x05, /*!< 数据格式错误 */
  RTN_ERR_INVALID     = 0x06, /*!< 无效数据 */
  RTN_NULL            = 0x07, /*!< 不回复 */
}RTN_TypeDef;



void sys_restart(void);
_Bool IsConfigConnect(USART_INFO *usartInfo);
_Bool IsUpdataConnect(void);
//void Test_protocol(void);
_Bool Select_protocol(USART_INFO *usartInfo);

//配置协议分析
_Bool Para_config_protocol(char *input_protocol_string);

//判断配置包校验码
_Bool CheckCheckSumFromConfigData(char *configdata);
//打包参数配置回复包的RTN
void Pack_Ack_Config_RTN(u8 rtn_value);
//回复串口配置
void Ack_config_cmd_2_pc(RTN_TypeDef rtn_value);




extern u8 ack_protocol_config_array[37];

HEX2STR_1 Hex2StrFun_1(u8 *addr);
HEX2STR_2 Hex2StrFun_2(u16 *addr);
//HEX2STR_13 Hex2StrFun_13(char *tempstr);
//HEX2STR_16 Hex2StrFun_16(char *tempstr);
HEX2STR_21 Hex2StrFun_21(char *tempstr);







//////////////数组拷贝给数组////////////////////////////
void ArrayCopy(u8 *dest, u8 *src,u8 len);
//////////////数组是否相同数组////////////////////////////
_Bool ArrayCmp(u8 *dest, u8 *src, u8 len);
//检查所有码是否符合协议标准
_Bool Isformatcorrect(char *protol_string);
///////////////////////协议内的ascii码转协议码转换////////////////
u8* Str2HexFun_16(char *tempstr, u8 addr);
u8* Str2HexFun(char *tempstr, u16 addr,u8 len);

char *num2string(u16 tempnum);
char *intnum2hex(u16 tempnum);
#endif