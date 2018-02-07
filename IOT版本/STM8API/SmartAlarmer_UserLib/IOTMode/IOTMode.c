//#include "stm8l15x.h"
#include "config.h"
#include <string.h>

char SigStrenValue[3] = "0";
IOTMode_INFO iotmodeInfo = {0, 0, 0, 0, 0, 0, 0};
static char temp_string[38] = "at+zipsetup=0,000.000.000.000,00000\r\n";
//static char temp_string1[38] = "AT+NSOST=0,120.77.205.98,20111,18,";
//static char temp_string[38] = "AT+NSOST=0,121.42.196.113,20008,204,";
static u8 temp_rtn_cmd = RTN_NULL;

u8 tmp_tab[USARTBUF_MAX];    //创建一个临时的缓冲区。

void IOTModeRestart(void) {

//	POWERON_MC;
//	RESET_MC_RESET;
//	DelayT_ms(1000);
//	SET_MC_POWER_ONOFF;
//	DelayT_ms(40);
//	SET_MC_RESET;
//	DelayT_ms(20);
//	RESET_MC_POWER_ONOFF;
//	DelayT_ms(500);
//	SET_MC_POWER_ONOFF;
  
//        IOTModePoweroff();      //关闭模块电源
//        DelayT_ms(500);
	POWERON_MC;
	DelayT_ms(20);
  	RESET_MC_RESET;
	SET_MC_POWER_ONOFF;
	DelayT_ms(1000);
	SET_MC_RESET;
	DelayT_ms(20);
	RESET_MC_POWER_ONOFF;
	DelayT_ms(1000);
	SET_MC_POWER_ONOFF;
	//IOTModeRset();
}
void IOTModePoweroff(void) {
	POWEROFF_MC;
	RESET_MC_RESET;
	SET_MC_POWER_ONOFF;
}

void IOTModeRset(void) {
	POWERON_MC;
	RESET_MC_POWER_ONOFF;

	DelayT_ms(500);
	SET_MC_RESET;
}




_Bool Report_Data(void)
{
  switch (iotmodeInfo.Mode_Restart)
  {  
  case 0:
    IOTModeRestart();
    iotmodeInfo.Mode_Restart++;
    uart1_init(9600);//临时添加测试
    iotmodeInfo.send_count = 0;
    
    //设置完成后，需要判断是不是进行了3次以上的重新设置波特率，如果是的话，需要超时退出
    if (++iotmodeInfo.reconnect_count > MAX_RECONNECT_COUNT) { //重启次数大于3次
            UsartPrintf(USART3, "ERROR:iotmodeInfo.reconnect_count>3!!!\r\n");
            IOTModePoweroff();      //关闭模块电源
            iotmodeInfo.reconnect_count = 0;
            iotmodeInfo.send_count = 0;
            iotmodeInfo.Mode_Restart = 0;
            iotmodeInfo.initStep = 0;
            heart_send_flag = 0;
            return 0;
    } else
    {
            Increase_send_totalcount();//增加一次电池发送次数  修正BUG  电池累计次数需要在每次尝试时 都新增
            //设置波特率后，不能对设备进行马上重启，不然设备不能保存这个参数
            //iotmodeInfo.initStep = 4;
            DelayT_ms(500);
            iotmodeInfo.initStep = 0;
            iotmodeInfo.send_count = 0;
    }
    break;
    
  case 1://9600波特率
    PrintString_("STA Tips:	AT\r\n");
    if (!IOTMode_SendCmd("AT\r\n", "OK", 1000)) //测试AT
    {
      
      UsartPrintf(USART3, "AT OK\r\n");
    iotmodeInfo.Mode_Restart++;
      
    } else {
      if (++iotmodeInfo.send_count >= 5) {
        //重启模块
        iotmodeInfo.Mode_Restart = 0;
        iotmodeInfo.send_count = 0;
      }
    }
    //////////////////////
;
  case 2:
    iotmodeInfo.initStep = 0;
    PrintString_(">>>>>>>>>>查询通讯模块\r\n");
    if (!IOTMode_SendCmd("ATI0\r\n", "ZTE", 1000)) //测试AT
    {
      PrintString_(">>模块为MC8618!!!!\r\n");
      iotmodeInfo.Mode_Restart = 4;
      
    }else if(!IOTMode_SendCmd("AT+NBAND?\r\n", "NB", 1000)){
      PrintString_(">>模块为NB95!!!!\r\n");
      iotmodeInfo.Mode_Restart = 3;
    }else{
      PrintString_(">>未检测出模块!!!!\r\n");
            if (++iotmodeInfo.send_count >= 3) {
              iotmodeInfo.initStep = 0;
              iotmodeInfo.Mode_Restart = 0;
              iotmodeInfo.send_count = 0;
            }
    }
    break;
  case 3:
    return NB95_Report();
    break;
  case 4:
    return MC8618_Report();
    break;
  case 5:
  default:
    break;
  }
	return 1;
}

/****************************************NB发送*******************************/   

_Bool NB95_Report(void)
{
  switch (iotmodeInfo.initStep)
  {
    case 0://设置频段 AT+NBAND=5 电信模块 AT+NBAND=8 移动联通模块
      PrintString_("AT+NBAND?\r\n");
      if(!Sys_keydata.carrier_type){
        if (!IOTMode_SendCmd("AT+NBAND?\r\n", "+NBAND:5", 1000)) //设置手动连接
        {
          iotmodeInfo.initStep++;
          PrintString_("NBAND IS SET FOR 5!\r\n");
          iotmodeInfo.send_count = 0;
        } else {//如果不是BAND5 设置为BAND5
          PrintString_("AT+NBAND=5\r\n");
          if (!IOTMode_SendCmd("AT+NBAND=5\r\n", "OK", 800)) //设置手动连接
          {
            iotmodeInfo.initStep++;
            PrintString_("NBAND IS SET FOR 5!\r\n");
            iotmodeInfo.send_count = 0;
          } else {//如果手动连接不上 会重启模块
            if (++iotmodeInfo.send_count >= 3) {
              iotmodeInfo.initStep = 0;
              iotmodeInfo.Mode_Restart = 0;
              iotmodeInfo.send_count = 0;
            }
          }
        }
      }else{
        if (!IOTMode_SendCmd("AT+NBAND?\r\n", "+NBAND:8", 1000)) //设置手动连接
        {
          iotmodeInfo.initStep++;
          PrintString_("NBAND IS SET FOR 8!\r\n");
          iotmodeInfo.send_count = 0;
        } else {//如果不是BAND8 设置为BAND8
          PrintString_("AT+NBAND=8\r\n");
          if (!IOTMode_SendCmd("AT+NBAND=8\r\n", "OK", 800)) //设置手动连接
          {
            iotmodeInfo.initStep++;
            PrintString_("NBAND IS SET FOR 8!\r\n");
            iotmodeInfo.send_count = 0;
          } else {//如果手动连接不上 会重启模块
            if (++iotmodeInfo.send_count >= 3) {
              iotmodeInfo.Mode_Restart = 0;
              iotmodeInfo.initStep = 0;
              iotmodeInfo.send_count = 0;
            }
          }
        }
      }
      
      break;
    case 1://查询是否自动连接 AT+NCONFIG?
      PrintString_("AT+NCONFIG?\r\n");
      if (!IOTMode_SendCmd("AT+NCONFIG?\r\n", "+NCONFIG:AUTOCONNECT,TRUE", 800)) //查询是否自动连接
      {//如果是自动连接
        iotmodeInfo.initStep++;
        PrintString_("AUTOCONNECT ENBALE!\r\n");
        iotmodeInfo.send_count = 0;
      } else {//如果不是自动连接
        PrintString_("AT+NCONFIG=AUTOCONNECT,TRUE\r\n");
        if (!IOTMode_SendCmd("AT+NCONFIG=AUTOCONNECT,TRUE\r\n", "OK", 800)) //设置自动连接
        {
          //发送重启的AT命令AT+NRB  重启模块
          PrintString_("AT+NRB\r\n");
          IOTMode_SendCmdWithoutResult("AT+NRB\r\n"); //重启模块
          DelayT_ms(1000);
          iotmodeInfo.initStep = 0;
          iotmodeInfo.send_count = 0;
          
        }
      }
      break;
    case 2://建立Socket AT+NSOCR=DGRAM,17,5683,1
      PrintString_("AT+NSOCR=DGRAM,17,1113,1\r\n");
      if (!IOTMode_SendCmd("AT+NSOCR=DGRAM,17,1113,1\r\n", "OK", 200)) //设置手动连接
      {
        iotmodeInfo.initStep++;
        PrintString_("AT+NSOCR SUCCESS!\r\n");
        iotmodeInfo.send_count = 0;
      } else {//如果手动连接不上 会重启模块
        if (++iotmodeInfo.send_count >= 3) {
          iotmodeInfo.initStep++;
          iotmodeInfo.send_count = 0;
        }
      }
      break;
//    case 3:  
		//GetIotNum();
		//iotmodeInfo.initStep++;
		//break;
    case 3://AT+CGATT?  可能需要20秒 才能读到
      uart1_init(9600);//临时添加测试
      DelayT_ms(500);
      
      PrintString_("AT+CGATT?\r\n");
      //                if (!IOTMode_SendCmd("AT+CGATT?\r\n", "+CGATT:1", 3000)) //设置手动连接
      if (!IOTMode_SendCmd("AT+CGATT?\r\n", "+CGATT:1", 400)) //设置手动连接
      {
        iotmodeInfo.initStep++;
        PrintString_("AT+CGATT SUCCESS!\r\n");
        iotmodeInfo.send_count = 0;
      } else {//如果手动连接不上 会重启模块
        //                        if (++iotmodeInfo.send_count >= 15) {
        if (++iotmodeInfo.send_count >= 35) {
          UsartPrintf(USART3, "AT+NRB\r\n");
          UsartPrintf(USART1, "AT+NRB\r\n");
          iotmodeInfo.Mode_Restart = 0;
          iotmodeInfo.initStep = 0;
          iotmodeInfo.send_count = 0;
        }
      }
      break;
    case 4://查询信号强度
      if (!GetNBSigStenValue())
      {
        GetNBIotNum();
        iotmodeInfo.initStep++;
        iotmodeInfo.send_count = 0;
      } //查询信号强度
      else {
        if (++iotmodeInfo.send_count >= 3) {
          iotmodeInfo.Mode_Restart = 0;
          iotmodeInfo.initStep = 0;
          iotmodeInfo.send_count = 0;
        }
      }
      break;
    case 5://封装数据
      //Increase_send_totalcount();//增加一次电池发送次数
      PackageMsg();//先封装，再发送
      iotmodeInfo.initStep++;
      break;
    case 6://发送UDP数据 AT+NSOST=0,172.28.92.145,10001,2,AB40   IOTMode_SendCmd_with_length_nb_iot

      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //发送控制空指令。 测试。 
      DelayT_ms(300);
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);
      DelayT_ms(300);      
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);
      DelayT_ms(400);
      
      UsartPrintf(USART3, "connecting...");
      
      Usart_SendString(USART1,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
      Usart_SendString(USART3,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
      Usart_SendString(USART1, (unsigned char *)Sys_keydata.default_cloud_server_add, strlen(Sys_keydata.default_cloud_server_add));// by hx 2017-06-15
      Usart_SendString(USART3, (unsigned char *)Sys_keydata.default_cloud_server_add, strlen(Sys_keydata.default_cloud_server_add));//调试输出// by hx 2017-06-15
      Usart_SendString(USART1,",250,", strlen(",250,"));
      Usart_SendString(USART3,",250,", strlen(",250,"));
      
      
      ////////////////////////////////////////////
      if (!IOTMode_SendCmd_with_length_nb_iot((char *)(&PacketData.senddata[17]), "0,250", 252)) //发送数据
        //if (!IOTMode_SendCmd_with_length_nb_iot("1234\r\n", "0,2",6)) //发送数据
      {
        //有参数更新 回复服务器端
        if (iotmodeInfo.ack_config) {
          iotmodeInfo.initStep++;
        } else {
          //没有参数更新
          
          if(iotmodeInfo.send_success_flag){
            iotmodeInfo.initStep = 8;//结束发送
          }else{//UDP消息没有回复  需要重发
            if (++iotmodeInfo.send_count >= 5) {//重发超过3次  结束整个流程
              PrintString_("ERROR:UDP MSG SENDING TIMEOUT_COUNT>5!!!\r\n");
              PrintString_("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n");
              			iotmodeInfo.reconnect_count = 0;
              			iotmodeInfo.send_count = 0;
              			iotmodeInfo.initStep = 0;
              			heart_send_flag = 0;
                                iotmodeInfo.Mode_Restart = 0;
              			iotmodeInfo.ack_config = 0;
                                iotmodeInfo.send_success_flag = 0;
              			return 0;
              break;
            }
          }
        }
        //}
      } else {//没收到，需要重发
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //发送控制空指令。 测试。 
      DelayT_ms(300);
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //发送控制空指令。 测试。 
      DelayT_ms(300);      
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //发送控制空指令。 测试。 
      DelayT_ms(300);
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //发送控制空指令。 测试。 
      DelayT_ms(300);
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //发送控制空指令。 测试。 

        iotmodeInfo.initStep = 0;
        iotmodeInfo.Mode_Restart = 0;
        iotmodeInfo.send_count = 0;
      }
      break;
    case 7:

      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //发送控制空指令。 测试。 
      
      DelayT_ms(1000);
      
      Usart_SendString(USART1,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
      Usart_SendString(USART3,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
      Usart_SendString(USART1, (unsigned char *)Sys_keydata.default_cloud_server_add, strlen(Sys_keydata.default_cloud_server_add));// by hx 2017-06-15
      Usart_SendString(USART3, (unsigned char *)Sys_keydata.default_cloud_server_add, strlen(Sys_keydata.default_cloud_server_add));//调试输出// by hx 2017-06-15
      Usart_SendString(USART3,",18,", strlen(",18,"));
      Usart_SendString(USART1,",18,", strlen(",18,"));
      
  //    IOTMode_SendCmdWithoutResult((char *)(&ack_protocol_config_array[16]));   
      if (!IOTMode_SendCmd_with_length_nb_iot(((char *)(&ack_protocol_config_array[16])), "0,18",20)) //发送数据// by hx 2017-06-15
      {
        
      iotmodeInfo.initStep ++;
        PrintSysParaList();
      }
      
      break;
    case 8:
      PrintString_("STA Tips:	AT+NSOCL=0\r\n");
      if (!IOTMode_SendCmd("AT+NSOCL=0\r\n", "OK", 500)) //
      {
        UsartPrintf(USART3, "Close Socket Done!\r\n");
        iotmodeInfo.initStep++;
        
      } else {
        if (++iotmodeInfo.send_count >= 2) {
          //超过次数 也会结束掉
          iotmodeInfo.initStep++;
          iotmodeInfo.Mode_Restart = 0;
          iotmodeInfo.send_count = 0;
        }
      }
      break;
    case 9:
      PrintString_("STA Tips:	AT+CGATT=0\r\n");
      if (!IOTMode_SendCmd("AT+CGATT=0\r\n", "OK", 500)) //
      {
        UsartPrintf(USART3, "Detach Done!\r\n");
        iotmodeInfo.initStep++;
        
      } else {
        if (++iotmodeInfo.send_count >= 2) {
          //超过次数 也会结束掉
          iotmodeInfo.initStep++;
          iotmodeInfo.Mode_Restart = 0;
          iotmodeInfo.send_count = 0;
        }
      }
      
      break;
    case 10://nb发送成功  关闭模块
      iotmodeInfo.ack_config = 0;
      iotmodeInfo.reconnect_count = 0;
      iotmodeInfo.send_count = 0;
      iotmodeInfo.Mode_Restart = 0;
      PrintString_("send data finish!!!");
      IOTModePoweroff();//关闭模块电源
      iotmodeInfo.initStep = 0;//返回低功耗 关闭模块电源
      iotmodeInfo.reconnect_count = 0;
      iotmodeInfo.send_count = 0;
      heart_send_flag = 0;
      return 0;
      break;
    default:
      break;
  }
	return 1;
}

  /**************************************CDMA发送*******************************/   
_Bool MC8618_Report(void)
{
  switch (iotmodeInfo.initStep)
  {
        case 0:
          iotmodeInfo.initStep ++;
 	case 1://9600波特率
		PrintString_("AT+ZPKEEPALIVE=1\r\n");
		if (!IOTMode_SendCmd("AT+ZPKEEPALIVE=1\r\n", "OK", 200)) //测试AT
		{

                  UsartPrintf(USART3, "AT+ZPKEEPALIVE=1 OK\r\n");
                  iotmodeInfo.initStep = 11;//获取卡号
                  
		} else {
                  if (++iotmodeInfo.send_count >= 15) {
                    UsartPrintf(USART3, "set 115200\r\n");
                    uart1_init(115200);
                    //UsartPrintf(USART1, "AT\r\n");
                    //DelayT_ms(100);
                    iotmodeInfo.initStep = 2;
                  }
		}
		break;//
                //at+zpppclose
        case 11:
                PrintString_("AT+CTA=0\r\n");
		if (!IOTMode_SendCmd("AT+CTA=0\r\n", "OK", 200)) //测试AT
		{

                    UsartPrintf(USART3, "AT+CTA=0 OK\r\n");
                    //iotmodeInfo.initStep = 4;
                    //iotmodeInfo.initStep = 6;
                    iotmodeInfo.initStep++;//获取卡号

		} else {
                  if (++iotmodeInfo.send_count >= 15) {
                    UsartPrintf(USART3, "set 115200\r\n");
                    uart1_init(115200);
                    //UsartPrintf(USART1, "AT\r\n");
                    //DelayT_ms(100);
                    iotmodeInfo.initStep = 2;
                  }
		}
		break;
	case 12:
                GetIotNum();
                PrintString_("at+zpppopen\r\n");
                IOTMode_SendCmdWithoutResult("at+zpppopen\r\n");//打开连接
                iotmodeInfo.send_count = 0;
                iotmodeInfo.initStep = 4;
		break;
	case 2:
		UsartPrintf(USART3, "AT+IPR=9600\r\n");
		//if(!IOTMode_SendCmd("AT\r\n", "OK",200)){ //测试AT{
		UsartPrintf(USART1, "AT+IPR=9600\r\n");//以115200波特率来发送改变波特率为9600,此条命令不等待
		uart1_init(9600);

		iotmodeInfo.initStep = 1;
		//}
		break;
	case 4:
		PrintString_("at+zpppstatus\r\n");
		if (!IOTMode_SendCmd("at+zpppstatus\r\n", "OPENED", 800)) //打开PPP连接
		{
                  iotmodeInfo.initStep++;
                  PrintString_("ZPPPSTATUS: OPENED\r\n");
                  iotmodeInfo.send_count = 0;
                  //iotmodeInfo.initStep=0;
		} else {
                  IOTMode_SendCmdWithoutResult("at+zpppopen\r\n");
                  PrintString_("at+zpppopen\r\n");
                  if (++iotmodeInfo.send_count >= 10) {
                          iotmodeInfo.initStep = 0;
                          iotmodeInfo.Mode_Restart = 0;
                          iotmodeInfo.send_count = 0;
                  }
		}

		break;
	case 5:
		//封装平台地址包
		for (u8 i = 0; i < 22; i++)
		{
                  if (Sys_keydata.default_cloud_server_add[i] != '\0') {
                          temp_string[i + 14] = Sys_keydata.default_cloud_server_add[i];
                  } else {
                          break;
                  }
		}
		temp_string[14 + strlen(Sys_keydata.default_cloud_server_add)] = '\r';
		temp_string[14 + strlen(Sys_keydata.default_cloud_server_add) + 1] = '\n';
		temp_string[14 + strlen(Sys_keydata.default_cloud_server_add) + 2] = '\0';
		PrintString_(temp_string);
		UsartPrintf(USART3, "connecting...");
		if (!IOTMode_SendCmd(temp_string, "ZTCPESTABLISHED", 8000))
		{
                  UsartPrintf(USART3, "connected to cloud!\r\n");
                  iotmodeInfo.initStep++;
		} //连接平台
		else {
                  debug_send("[ERROR]");
                  iotmodeInfo.initStep = 0;
                  iotmodeInfo.Mode_Restart = 0;
                  iotmodeInfo.send_count = 0;
		}
		break;

	case 6://获取信号强度 物联网卡号  打包数据
		//UsartPrintf(USART3, "AT+CSQ?\r\n");
		if (!GetSigStenValue())
		{
                  GetIotNum();
                  iotmodeInfo.initStep++;
                  iotmodeInfo.send_count = 0;
		} //查询信号强度
		else {
                  if (++iotmodeInfo.send_count >= 2) {
                    iotmodeInfo.initStep = 0;
                    iotmodeInfo.Mode_Restart = 0;
                    iotmodeInfo.send_count = 0;
                  }
		}
		break;
	case 7:
		//GetIotNum();
		//iotmodeInfo.initStep++;
		//break;
	case 8:
		//UsartPrintf(USART3, "at+zipsend=0,204 PacketData\r\n");
		PrintString_("at+zipsend=0,250 PacketData\r\n");
		DelayT_ms(100);
		//Increase_send_totalcount();//增加一次电池发送次数
		PackageMsg();//先封装，再发送
		if (!IOTMode_SendCmd_with_length((char *)(&PacketData.senddata[0]), "ZIPSEND: 250", 269)) //发送数据
		{
                  //有参数更新 回复服务器端
                  if (iotmodeInfo.ack_config) {
                          iotmodeInfo.initStep = 9;
                  } else {
                          //没有参数更新
                          iotmodeInfo.initStep = 10;
                  }
                        
		} else {//没收到，需要重发
			iotmodeInfo.initStep = 0;
			iotmodeInfo.send_count = 0;
                        iotmodeInfo.Mode_Restart = 0;
		}
		break;
	case 9://回复服务器 参数配置项目
		PrintString_("ack_protocol_config_array");
		if (!IOTMode_SendCmd_with_length((char *)(&ack_protocol_config_array[0]), "ZIPSEND: 18", 36)) {
                    iotmodeInfo.initStep = 10;
                    PrintSysParaList();
		}
		break;
          case 10:
                PrintString_("at+zipclose\r\n");
		if (!IOTMode_SendCmd("at+zipclose\r\n", "OK", 800)) //断开连接
		{
                    UsartPrintf(USART3, "at+zipclose OKOKOKOKOK\r\n");
                    iotmodeInfo.initStep=21;

		} else {
                    if (++iotmodeInfo.send_count >= 2) {
                            iotmodeInfo.initStep=21;
                    }
		}
		break;
          case 21:
                PrintString_("at+zpppclose\r\n");
		if (!IOTMode_SendCmd("at+zpppclose\r\n", "OK", 800)) //断开连接
		{

                  UsartPrintf(USART3, "at+zpppclose OKOKOKOKOK\r\n");
                  iotmodeInfo.initStep++;

		} else {
                  if (++iotmodeInfo.send_count >= 2) {
                      iotmodeInfo.initStep++;
                  }
		}
		break;
          break;
	case 22://发送成功  关闭模块
		iotmodeInfo.ack_config = 0;
		iotmodeInfo.reconnect_count = 0;
		iotmodeInfo.send_count = 0;
                iotmodeInfo.Mode_Restart = 0;
		PrintString_("send data finish!!!");
                PrintString_(">>>>发送成功，结束退出到低功耗");
		IOTModePoweroff();//关闭模块电源
		iotmodeInfo.initStep = 0;//返回低功耗 关闭模块电源
		iotmodeInfo.reconnect_count = 0;
		iotmodeInfo.send_count = 0;
		heart_send_flag = 0;
		return 0;
		break;
	default:
		break;
	}
	return 1;
}
//判读字符串数字是否合法
_Bool IsStrNumValid(char *str_num_in) {
  //判断数字字符串是否合法
  if (strlen(str_num_in) == 2) {
    if ( ((str_num_in[0] >= '0') && (str_num_in[0] <= '9')) && ((str_num_in[1] >= '0') && (str_num_in[1] <= '9')) ) {
      return 1;
    } else {
      return 0;
    }
    
  } else if (strlen(str_num_in) == 1) {
    if ((str_num_in[0] >= '0') && (str_num_in[0] <= '9')) {
      return 1;
    } else {
      return 0;
    }
  }
  return 1;
}
u8 Str2u8(char *Sting_in) {
  u8 u8_result = 0;
  if (IsStrNumValid(Sting_in)) {
    if (strlen(Sting_in) == 2) {
      u8_result = (Sting_in[0] - 0x30) * 10 + (Sting_in[1] - 0x30);
    } else if (strlen(Sting_in) == 1) {
      //u8_result = (Sting_in[1] - '0');//BUG 20170306 强度小于10 不显示
      u8_result = (Sting_in[0] - 0x30);
    }
    
  }
  return u8_result;
}

///获取物联网卡号号码
_Bool GetIotNum(void) {
  unsigned int timeOut = 200;
  //u16 front_count = 0;
  //u16 after_count = 0;
  static char *temp_iot_num_string = "0000000000000";
  static char iot_num_string[14] = "0000000000000";
  Usart_SendString(USART1, "AT+GETICCID\r\n", 13);
  DelayT_ms(200);
  while (timeOut--)
  {
    UsartReciveFlag(&usart1Info);
    if (usart1Info.usartReceiveFlag == REV_OK)
    {
      usart1Info.usartReceiveFlag = REV_WAIT;
      
      if (strstr((const char *)usart1Info.usartBuf, "GETICCID:0x") != NULL)
      {
        temp_iot_num_string = strstr((const char *)usart1Info.usartBuf, "GETICCID:0x");
        
        //PrintString_("usart1Info.usartBuf is :");
        //PrintString_((char*)usart1Info.usartBuf);
        
        //PrintString_(temp_iot_num_string);
        
        //strncpy(iot_num_string, temp_iot_num_string + 17, 13);
        for (u8 i = 0; i < 13; i++) {
          iot_num_string[i] = temp_iot_num_string[i + 17];
        }
        iot_num_string[13] = '\0';
        //memcpy(iot_num_string, (usart1Info.usartBuf + front_count), 13);
        
        PrintString_("iot_num_string is :");
        PrintString_(iot_num_string);
        
        Pack_IotNum(iot_num_string);
        
        Usart1_RcvClr();
        return 1;
      }
    }
    
    DelayT_ms(2);
  }
  
  Usart1_RcvClr();
  
  return 0;
}
///获取物联网卡号号码 //NB复位后6s读卡
_Bool GetNBIotNum(void) {
  unsigned int timeOut = 200;
  //u16 front_count = 0;
  //u16 after_count = 0;
  static char *temp_iot_num_string = "0000000000000";
  static char iot_num_string[20] = "0000000000000";
  Usart_SendString(USART1, "AT+NCCID\r\n", 10);
  DelayT_ms(200);
  while (timeOut--)
  {
    UsartReciveFlag(&usart1Info);
    if (usart1Info.usartReceiveFlag == REV_OK)
    {
      usart1Info.usartReceiveFlag = REV_WAIT;
      
      if (strstr((const char *)usart1Info.usartBuf, "+NCCID:") != NULL)
      {
        temp_iot_num_string = strstr((const char *)usart1Info.usartBuf, "+NCCID:");
        
        //PrintString_("usart1Info.usartBuf is :");
        //PrintString_((char*)usart1Info.usartBuf);
        
        //PrintString_(temp_iot_num_string);
        
        //strncpy(iot_num_string, temp_iot_num_string + 17, 13);
        for (u8 i = 0; i < 13; i++) {
          iot_num_string[i] = temp_iot_num_string[i + 13];
        }
        iot_num_string[13] = '\0';
        //memcpy(iot_num_string, (usart1Info.usartBuf + front_count), 13);
        
        PrintString_("iot_num_string is :");
        PrintString_(iot_num_string);
        
        Pack_IotNum(iot_num_string);
        
        Usart1_RcvClr();
        return 1;
      }
    }
    
    DelayT_ms(2);
  }
  
  Usart1_RcvClr();
  
  return 0;
}
/////获取模块信号强度
_Bool GetSigStenValue(void) {
	unsigned int timeOut = 200;
	//u16 front_count = 0;
	u16 after_count = 0;
	Usart_SendString(USART1, "AT+CSQ?\r\n", 9);
	while (timeOut--)
	{
		UsartReciveFlag(&usart1Info);
		if (usart1Info.usartReceiveFlag == REV_OK)
		{
			usart1Info.usartReceiveFlag = REV_WAIT;
			if (strstr((const char *)usart1Info.usartBuf, "CSQ: ") != NULL)
			{
				//Usart1_RcvClr()
				//front_count = strcspn((const char *)usart1Info.usartBuf, "+CSQ: ");
				if (strstr((const char *)usart1Info.usartBuf, ", 99") != NULL) {
					after_count = strcspn((const char *)usart1Info.usartBuf, ", 99");
					if (usart1Info.usartBuf[after_count - 2] != ' ') {
						SigStrenValue[0] = usart1Info.usartBuf[after_count + 1];
						SigStrenValue[1] = usart1Info.usartBuf[after_count + 2];
						SigStrenValue[2] = '\0';
					} else {
						SigStrenValue[0] = usart1Info.usartBuf[after_count + 2];
						SigStrenValue[1] = '\0';
					}

					//SigStrenValue[0] = '9';
					//SigStrenValue[1] = '\0';
					PrintString("SigStren: ");
					PrintString_(SigStrenValue);
					Pack_SigStren(Str2u8(SigStrenValue));//打包信号强度

					Usart1_RcvClr();

					return 0;
				}
			}
		}

		DelayT_ms(2);
	}

	Usart1_RcvClr();

	return 1;
}
/////获取模块信号强度
_Bool GetNBSigStenValue(void) {
  unsigned int timeOut = 200;
  //u16 front_count = 0;
  u16 after_count = 0;
  Usart1_RcvClr();
  Usart_SendString(USART1, "AT+CSQ\r\n", 9);
  Usart_SendString(USART3, "AT+CSQ\r\n", 9);
  while (timeOut--)
  {
    UsartReciveFlag(&usart1Info);
    if (usart1Info.usartReceiveFlag == REV_OK)
    {
      usart1Info.usartReceiveFlag = REV_WAIT;
      if (strstr((const char *)usart1Info.usartBuf, "+CSQ:") != NULL)
      {
        //Usart1_RcvClr()
        //front_count = strcspn((const char *)usart1Info.usartBuf, "+CSQ: ");
        if (strstr((const char *)usart1Info.usartBuf, ",99") != NULL) {
          after_count = strcspn((const char *)usart1Info.usartBuf, ",99");
          if (usart1Info.usartBuf[after_count - 2] != ':') {
            SigStrenValue[0] = usart1Info.usartBuf[after_count - 2];
            SigStrenValue[1] = usart1Info.usartBuf[after_count - 1];
            SigStrenValue[2] = '\0';
          } else {
            SigStrenValue[0] = usart1Info.usartBuf[after_count - 1];
            SigStrenValue[1] = '\0';
          }
          
          //SigStrenValue[0] = '9';
          //SigStrenValue[1] = '\0';
          debug_send("SigStrenValue is :");
          debug_send(SigStrenValue);
          if (Str2u8(SigStrenValue) > 31) {
            Usart1_RcvClr();
            return 1;
          }
          Pack_SigStren(Str2u8(SigStrenValue));//打包信号强度
          
          Usart1_RcvClr();
          
          return 0;
        }
      }
    }
    
    DelayT_ms(2);
  }
  
  Usart1_RcvClr();
  
  return 1;
}



//判断心跳包校验码
_Bool CheckCheckSumFromHeartData(char *heartdata) {
  HEX2STR_2 get_checksum = {'0', '0', '0', '0'};
  //u8 get_heartdata_len = 0;
  //get_heartdata_len = strlen(heartdata);
  
  
  CalcChecksum(&get_checksum, heartdata, 50); //计算校验码
  PrintString("CHESUM :");
  PrintString_((char *)get_checksum.str2 + 0);
  
  //PrintString_(get_checksum.str2+1);
  //PrintString_(get_checksum.str2+2);
  //PrintString_(get_checksum.str2+3);
  //ArrayCmp 对比校验码
  if (ArrayCmp((u8 *)(&get_checksum), (u8 *)heartdata + 50, 4)) {
    PrintString_("CHESUM CORRECT");
    return 1;
  }
  else {
    PrintString_("CHESUM ERROR");
    return 0;
  }
  
  /*if(get_heartdata_len == 54){//现在协议正常报装的
  
}else if(get_heartdata_len == 16){//设备未报装
  
}else if(get_heartdata_len == 100){//新接口 带参数 带升级服务器地址
  
}*/
  
}

//解析消息  读取参数
RTN_TypeDef GetStrFromStr(char *source_str) {
  char *tmpStr=NULL;  //临时指针。
  //获取校验码
  if (CheckCheckSumFromHeartData(source_str)) {
    
    //获取参数更新标志
    if (*(Str2HexFun(source_str, CONFIG_AMD_PARA_FLAG_ADDR, 1)) == 1) {
      //如果有参数更新，则继续分析协议
           Usart1_RcvClr();//清除缓冲区。
      for(u16 i=0;i<USARTBUF_MAX;i++) //清空缓冲区的数据
      {
        tmp_tab[i] = 0;  
      }
      Usart_SendString(USART1, "AT+NSORF=0,274\r\n", 16);
      Usart_SendString(USART3, "AT+NSORF=0,274\r\n", 16);  //发送读取指令。
      UsartReciveFlag(&usart1Info);
      tmpStr = strstr((const char *)usart1Info.usartBuf, "7E");
      if(NULL != NBiotToPosts(tmpStr,tmp_tab))   //正确地转换了数据//by hx 2017-06-10//NB通讯格式转邮电部协议。
      {
        //            //添加NB-IOT转邮电部协议程序。
        PrintString_((char *)tmp_tab);
        for(u16 i=0;i<USARTBUF_MAX;i++) //清空缓冲区的数据，不清除控制位
        {
          usart1Info.usartBuf[i] = 0;  
        }
        strcpy(usart1Info.usartBuf,tmp_tab); //数据考入串口缓冲区模拟一个接收过程。
//        temp_rtn_cmd = GetRebackMsg((char *)tmp_tab);//解析消息
      }
      //对上一次的使用的IP地址备份,占用默认的IP地址。
      
      
//      usart1Info.usartReceiveFlag = REV_OK;    
      if (Select_protocol(&usart1Info)) { //选择协议分析
        return RTN_OK;
      }
      else {
        return RTN_ERR_INVALID;
      }
    } else {
      
    }
  }
  //获取ID
  //获取设备状态
  //获取参数更新开关
  //获取固件更新开关
  //获取固件更新服务器地址
  return RTN_NULL;
}
/*
HEX2STR_2 CalcCheckOfString(char *input_string) {

}


_Bool GetParaFromHeartData(char *heartdata) {


return 0;
}
*/
u8 msg_len = 0;

//读取返回的消息
RTN_TypeDef GetRebackMsg(char *rck_msg) {
  char *rec_msg = "~1001D000802679696E676A69616E30323031000000000001010000";
  //strlen(strchr(rck_msg,'~'))
  char *end_str;
  //u8 msg_len = 0;//返回消息的长度
  
  //截取
  rec_msg = strchr(rck_msg, '~') + 1;
  end_str = strchr(rec_msg, 0x0d);
  *end_str = '\0';
  PrintString_(rec_msg);
  msg_len = strlen(rec_msg);
  PrintU8int2String_(msg_len);
  
  //提取出数据包 长度上已经去掉了开始字符0x7e与结束字符0x0d  并解析验证(判断校验码)
  if (msg_len == 54) { //设备已经正常报装 返回正常数据
    //获取参数
    if (!GetStrFromStr(rec_msg)) {
      return RTN_OK;
    } 
    else 
    {
      return RTN_NULL;
    }
  } else if (msg_len == 16) { //设备还未报装 返回06
    
  } else if (msg_len == 100) { //兼容增加了ISP服务器地址参数的协议
    
  }
  return RTN_NULL;
}

void SendDataWithUdp_NB(USART_TypeDef *USARTx, char *cmd, u8 len) {
  for (u8 i = 0; i < len; i++) {
    Usart_SendString(USARTx, Hex2StrFun_1(cmd + i).str1, 2);
  }
  Usart_SendString(USARTx, "\r\n", 2);
}


const unsigned char HEX_AF[]=
{
  0xA,
  0XB,
  0XC,
  0XD,
  0XE,
  0XF,
};
/***************************************************************************************
**函数名:        NBiotToPosts
**功能描述:     把NB IOT模块使用的字符类型转换为邮电部协议。
如7E313030314430303030303030464441410D
转换为~1001D0000000FDAD

**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
u8* NBiotToPosts(char *parPin,u8 *parPout)
{
  u16 slen=0;
  u16 i;
  u8 tmp=0;
  slen = strlen(parPin);
  //  if(slen> USARTBUF_MAX)      //数据超出了最大的缓存。发生错误。
  //  {
  //    return NULL;
  //  }
  for(i=0;i<slen;i=i+2)       //两个字符合成一个字符。
  {
    tmp = 0;
    if((*parPin>='0')&&(*parPin<='9'))
    {
      tmp =(*parPin-'0')<<4;
    }
    else if((*parPin>='A')&&(*parPin<='F'))
    {
      tmp = (HEX_AF[*parPin-'A'])<<4;
    }
    else
    {
      tmp='\0';
    }
    parPin++;
    if((*parPin>='0')&&(*parPin<='9'))
    {
      tmp =tmp|((*parPin-'0')&0x0F);
    }
    else if((*parPin>='A')&&(*parPin<='F'))
    {
      tmp = tmp|((HEX_AF[*parPin-'A'])&0x0F);
    }
    else
    {
      tmp='\0';
    }
    parPin++;
    *parPout =tmp;
    parPout++;
  }
  *parPout='\0';
  return parPout;
}



_Bool IOTMode_SendCmd_with_length_nb_iot(char *cmd, char *res, u8 len) //
{
  char *tmpStr=NULL;  //临时指针。
  u16 strLen = 0;
  u8 tmp_tab[USARTBUF_MAX];    //创建一个临时的缓冲区。
  
  Watchdog_Feed();
  Usart1_RcvClr();
  unsigned int timeOut = 5000;
  SendDataWithUdp_NB(USART3, cmd, len - 2);
  SendDataWithUdp_NB(USART1, cmd, len - 2);
  Watchdog_Feed();
  Usart_SendString(USART3, (unsigned char *)cmd, len);
  
  //Usart_SendString(USART1, (unsigned char *)cmd, len);
  while (timeOut--)
  {
    UsartReciveFlag(&usart1Info);
    if (usart1Info.usartReceiveFlag == REV_OK)
    {
      usart1Info.usartReceiveFlag = REV_WAIT;
      
      if (strstr((const char *)usart1Info.usartBuf, res) != NULL)
      {
        //////////////临时调试新增/////////////////////
        PrintString_((char *)usart1Info.usartBuf);
        Usart1_RcvClr();
        DelayT_ms(5000);
        PrintString("BEGIN PRINT:");
        PrintString_((char *)usart1Info.usartBuf);
        //				if (strstr((const char *)usart1Info.usartBuf, "+NSONMI:") != NULL) { //有消息接收
        //					Usart1_RcvClr();
        //                                        /*
        //					Usart_SendString(USART1, "AT+NSORF=", 9);
        //					Usart_SendString(USART1, strstr((const char *)usart1Info.usartBuf, "+NSONMI:") + 8, 4);
        //					Usart_SendString(USART1, "\r\n", 2);
        //                                        
        //                                        Usart_SendString(USART3, "AT+NSORF=", 9);
        //					Usart_SendString(USART3, strstr((const char *)usart1Info.usartBuf, "+NSONMI:") + 8, 4);
        //					Usart_SendString(USART3, "\r\n", 2);
        //                                        */
        //                                        iotmodeInfo.send_success_flag = 1;
        //                                        Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
        //                                        Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);
        //					DelayT_ms(1000);
        //					PrintString_((char *)usart1Info.usartBuf);
        //					temp_rtn_cmd = GetRebackMsg((char *)usart1Info.usartBuf);//解析消息
        //					//封装RTN
        //					if (temp_rtn_cmd == RTN_OK) {
        //						//跳转到回复状态
        //						Pack_Ack_Config_RTN(temp_rtn_cmd);//封装RTN
        //						CalcChecksum((HEX2STR_2 *)(&ack_protocol_config_array[29]), (char *)(&ack_protocol_config_array[17]), 12); //计算校验码
        //                                                
        //						ack_protocol_config_array[36] = '\0';
        //						iotmodeInfo.ack_config = 1;//回复标志置一
        //					}
        //				} 
        if(strstr((const char *)usart1Info.usartBuf, "+NSONMI:") != NULL) //有消息接收
        { 
          tmpStr = strstr((const char *)usart1Info.usartBuf, ",");
          if( tmpStr!= NULL) //获取接收数据的长度。
          { 
            strLen = strlen(tmpStr)-1;    //
            tmpStr++;     //+NSONMI:0,18  从18开始取出。
            for(int i=0;i<strLen;i++)
            {
              tmp_tab[i]= *tmpStr;
              tmpStr++; 
            }
          }
          //                                        PrintString_((char *)tmp_tab);   //打印数据长度         
          Usart1_RcvClr();
          iotmodeInfo.send_success_flag = 1;
          
          Usart_SendString(USART1, "AT+NSORF=0,", 11);
          Usart_SendString(USART1,tmp_tab,strLen);        
          Usart_SendString(USART1,"\r\n",2);
          Usart_SendString(USART3, "AT+NSORF=0,", 11); 					
          Usart_SendString(USART3,tmp_tab,strLen);
          Usart_SendString(USART3,"\r\n",2);
          PrintString_((char *)usart1Info.usartBuf);
          DelayT_ms(1000);
          tmpStr = strstr((const char *)usart1Info.usartBuf, "7E");
          if(NULL != NBiotToPosts(tmpStr,tmp_tab))   //正确地转换了数据//by hx 2017-06-10
          {
            //            //添加NB-IOT转邮电部协议程序。
            PrintString_((char *)tmp_tab);
            temp_rtn_cmd = GetRebackMsg((char *)tmp_tab);//解析消息          
          //封装RTN
          if (temp_rtn_cmd == RTN_OK) 
          {
            //跳转到回复状态
            Pack_Ack_Config_RTN(temp_rtn_cmd);//封装RTN
            CalcChecksum((HEX2STR_2 *)(&ack_protocol_config_array[29]), (char *)(&ack_protocol_config_array[17]), 12); //计算校验码
            
            ack_protocol_config_array[36] = '\0';
            iotmodeInfo.ack_config = 1;//回复标志置一
          }
          }

        }
        
        else { //如果没有UDP消息 退出
          iotmodeInfo.send_success_flag = 0;
          PrintString_("NONE ACK");
        }
        
        
        
        PrintString_("END PRINT");
        ///////////////////////////////////
        
        Usart1_RcvClr();
        return 0;
      }
    }
    
    Watchdog_Feed();
    DelayT_ms(2);
  }
  
  Usart1_RcvClr();
  
  return 1;
  
}


_Bool IOTMode_SendCmd_with_length(char *cmd, char *res, u16 len) //
{
  Usart1_RcvClr();
  unsigned int timeOut = 2000;
  Usart_SendString(USART3, (unsigned char *)cmd, len);
  Usart_SendString(USART1, (unsigned char *)cmd, len);
  while (timeOut--)
  {
    UsartReciveFlag(&usart1Info);
    if (usart1Info.usartReceiveFlag == REV_OK)
    {
      usart1Info.usartReceiveFlag = REV_WAIT;
      
      if (strstr((const char *)usart1Info.usartBuf, res) != NULL)
      {
        //////////////临时调试新增/////////////////////
        PrintString_((char *)usart1Info.usartBuf);
        Usart1_RcvClr();
        DelayT_ms(2000);
        PrintString("BEGIN PRINT:");
        PrintString_((char *)usart1Info.usartBuf);
        
        temp_rtn_cmd = GetRebackMsg((char *)usart1Info.usartBuf);//解析消息
        //封装RTN
        if (temp_rtn_cmd == RTN_OK) {
          //跳转到回复状态
          Pack_Ack_Config_RTN(temp_rtn_cmd);//封装RTN
          CalcChecksum((HEX2STR_2 *)(&ack_protocol_config_array[29]), (char *)(&ack_protocol_config_array[17]), 12); //计算校验码
          
          ack_protocol_config_array[36] = '\0';
          iotmodeInfo.ack_config = 1;//回复标志置一
          //PrintString_("ack_protocol_config_array");
          //PrintString_(ack_protocol_config_array);
        }
        
        PrintString_("END PRINT");
        ///////////////////////////////////
        
        Usart1_RcvClr();
        return 0;
      }
    }
    
    DelayT_ms(2);
  }
  
  Usart1_RcvClr();
  
  return 1;
  

}



_Bool WaitForReturnCmd(char *res, u16 delaytime) //
{
  unsigned int timeOut = delaytime;
  while (timeOut--)
  {
    UsartReciveFlag(&usart1Info);
    if (usart1Info.usartReceiveFlag == REV_OK)
    {
      usart1Info.usartReceiveFlag = REV_WAIT;
      
      if (strstr((const char *)usart1Info.usartBuf, res) != NULL)
      {
        
        Usart1_RcvClr();
        
        return 0;
      }
    }
    
    DelayT_ms(2);
  }
  
  Usart1_RcvClr();
  
  return 1;
  
}

_Bool IOTMode_SendCmd(char *cmd, char *res, u16 delaytime) //
{
  
  unsigned int timeOut = delaytime;
  
  Usart_SendString(USART1, (unsigned char *)cmd, strlen((const char *)cmd));
  
  while (timeOut--)
  {
    UsartReciveFlag(&usart1Info);
    if (usart1Info.usartReceiveFlag == REV_OK)
    {
      usart1Info.usartReceiveFlag = REV_WAIT;
      
      if (strstr((const char *)usart1Info.usartBuf, res) != NULL)
      {
        //PrintString_("uart1 buff is:");
        PrintString_(usart1Info.usartBuf);
        Usart1_RcvClr();
        
        return 0;
      }
    }
    
    DelayT_ms(2);
  }
  
  Usart1_RcvClr();
  
  return 1;
  
}

void IOTMode_SendCmdWithoutResult(char *cmd) //能获取到返回的数据
{
  Usart_SendString(USART1, (unsigned char *)cmd, strlen((const char *)cmd));
}



char *IOTMode_GetStatus(void)
{
  
  char *status = "CONNECT OK";
  
  //IOTMode_QuitTrans();
  
  if (IOTMode_SendCmd("AT\r\n", "OK", 200)) //如果不存在
  {
    status = "No Device";
    //faultTypeReport = faultType = FAULT_NODEVICE;
  }
  
  if (IOTMode_SendCmd("AT+SIM1\r\n", "OK", 200)) //如果sim卡丢失
  {
    status = "No Sim Card";
    //faultTypeReport = faultType = FAULT_NODEVICE;
  }
  
  if (IOTMode_SendCmd("AT+CMSTAT\r\n", "OK", 200)) //检查连接状态
  {
    status = "Lost Connect";
    //faultTypeReport = faultType = FAULT_EDP;
  }
  
  //IOTMode_EnterTrans();
  
  return status;
  
}



