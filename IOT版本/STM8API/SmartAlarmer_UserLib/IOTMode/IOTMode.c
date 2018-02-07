//#include "stm8l15x.h"
#include "config.h"
#include <string.h>

char SigStrenValue[3] = "0";
IOTMode_INFO iotmodeInfo = {0, 0, 0, 0, 0, 0, 0};
static char temp_string[38] = "at+zipsetup=0,000.000.000.000,00000\r\n";
//static char temp_string1[38] = "AT+NSOST=0,120.77.205.98,20111,18,";
//static char temp_string[38] = "AT+NSOST=0,121.42.196.113,20008,204,";
static u8 temp_rtn_cmd = RTN_NULL;

u8 tmp_tab[USARTBUF_MAX];    //����һ����ʱ�Ļ�������

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
  
//        IOTModePoweroff();      //�ر�ģ���Դ
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
    uart1_init(9600);//��ʱ��Ӳ���
    iotmodeInfo.send_count = 0;
    
    //������ɺ���Ҫ�ж��ǲ��ǽ�����3�����ϵ��������ò����ʣ�����ǵĻ�����Ҫ��ʱ�˳�
    if (++iotmodeInfo.reconnect_count > MAX_RECONNECT_COUNT) { //������������3��
            UsartPrintf(USART3, "ERROR:iotmodeInfo.reconnect_count>3!!!\r\n");
            IOTModePoweroff();      //�ر�ģ���Դ
            iotmodeInfo.reconnect_count = 0;
            iotmodeInfo.send_count = 0;
            iotmodeInfo.Mode_Restart = 0;
            iotmodeInfo.initStep = 0;
            heart_send_flag = 0;
            return 0;
    } else
    {
            Increase_send_totalcount();//����һ�ε�ط��ʹ���  ����BUG  ����ۼƴ�����Ҫ��ÿ�γ���ʱ ������
            //���ò����ʺ󣬲��ܶ��豸����������������Ȼ�豸���ܱ����������
            //iotmodeInfo.initStep = 4;
            DelayT_ms(500);
            iotmodeInfo.initStep = 0;
            iotmodeInfo.send_count = 0;
    }
    break;
    
  case 1://9600������
    PrintString_("STA Tips:	AT\r\n");
    if (!IOTMode_SendCmd("AT\r\n", "OK", 1000)) //����AT
    {
      
      UsartPrintf(USART3, "AT OK\r\n");
    iotmodeInfo.Mode_Restart++;
      
    } else {
      if (++iotmodeInfo.send_count >= 5) {
        //����ģ��
        iotmodeInfo.Mode_Restart = 0;
        iotmodeInfo.send_count = 0;
      }
    }
    //////////////////////
;
  case 2:
    iotmodeInfo.initStep = 0;
    PrintString_(">>>>>>>>>>��ѯͨѶģ��\r\n");
    if (!IOTMode_SendCmd("ATI0\r\n", "ZTE", 1000)) //����AT
    {
      PrintString_(">>ģ��ΪMC8618!!!!\r\n");
      iotmodeInfo.Mode_Restart = 4;
      
    }else if(!IOTMode_SendCmd("AT+NBAND?\r\n", "NB", 1000)){
      PrintString_(">>ģ��ΪNB95!!!!\r\n");
      iotmodeInfo.Mode_Restart = 3;
    }else{
      PrintString_(">>δ����ģ��!!!!\r\n");
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

/****************************************NB����*******************************/   

_Bool NB95_Report(void)
{
  switch (iotmodeInfo.initStep)
  {
    case 0://����Ƶ�� AT+NBAND=5 ����ģ�� AT+NBAND=8 �ƶ���ͨģ��
      PrintString_("AT+NBAND?\r\n");
      if(!Sys_keydata.carrier_type){
        if (!IOTMode_SendCmd("AT+NBAND?\r\n", "+NBAND:5", 1000)) //�����ֶ�����
        {
          iotmodeInfo.initStep++;
          PrintString_("NBAND IS SET FOR 5!\r\n");
          iotmodeInfo.send_count = 0;
        } else {//�������BAND5 ����ΪBAND5
          PrintString_("AT+NBAND=5\r\n");
          if (!IOTMode_SendCmd("AT+NBAND=5\r\n", "OK", 800)) //�����ֶ�����
          {
            iotmodeInfo.initStep++;
            PrintString_("NBAND IS SET FOR 5!\r\n");
            iotmodeInfo.send_count = 0;
          } else {//����ֶ����Ӳ��� ������ģ��
            if (++iotmodeInfo.send_count >= 3) {
              iotmodeInfo.initStep = 0;
              iotmodeInfo.Mode_Restart = 0;
              iotmodeInfo.send_count = 0;
            }
          }
        }
      }else{
        if (!IOTMode_SendCmd("AT+NBAND?\r\n", "+NBAND:8", 1000)) //�����ֶ�����
        {
          iotmodeInfo.initStep++;
          PrintString_("NBAND IS SET FOR 8!\r\n");
          iotmodeInfo.send_count = 0;
        } else {//�������BAND8 ����ΪBAND8
          PrintString_("AT+NBAND=8\r\n");
          if (!IOTMode_SendCmd("AT+NBAND=8\r\n", "OK", 800)) //�����ֶ�����
          {
            iotmodeInfo.initStep++;
            PrintString_("NBAND IS SET FOR 8!\r\n");
            iotmodeInfo.send_count = 0;
          } else {//����ֶ����Ӳ��� ������ģ��
            if (++iotmodeInfo.send_count >= 3) {
              iotmodeInfo.Mode_Restart = 0;
              iotmodeInfo.initStep = 0;
              iotmodeInfo.send_count = 0;
            }
          }
        }
      }
      
      break;
    case 1://��ѯ�Ƿ��Զ����� AT+NCONFIG?
      PrintString_("AT+NCONFIG?\r\n");
      if (!IOTMode_SendCmd("AT+NCONFIG?\r\n", "+NCONFIG:AUTOCONNECT,TRUE", 800)) //��ѯ�Ƿ��Զ�����
      {//������Զ�����
        iotmodeInfo.initStep++;
        PrintString_("AUTOCONNECT ENBALE!\r\n");
        iotmodeInfo.send_count = 0;
      } else {//��������Զ�����
        PrintString_("AT+NCONFIG=AUTOCONNECT,TRUE\r\n");
        if (!IOTMode_SendCmd("AT+NCONFIG=AUTOCONNECT,TRUE\r\n", "OK", 800)) //�����Զ�����
        {
          //����������AT����AT+NRB  ����ģ��
          PrintString_("AT+NRB\r\n");
          IOTMode_SendCmdWithoutResult("AT+NRB\r\n"); //����ģ��
          DelayT_ms(1000);
          iotmodeInfo.initStep = 0;
          iotmodeInfo.send_count = 0;
          
        }
      }
      break;
    case 2://����Socket AT+NSOCR=DGRAM,17,5683,1
      PrintString_("AT+NSOCR=DGRAM,17,1113,1\r\n");
      if (!IOTMode_SendCmd("AT+NSOCR=DGRAM,17,1113,1\r\n", "OK", 200)) //�����ֶ�����
      {
        iotmodeInfo.initStep++;
        PrintString_("AT+NSOCR SUCCESS!\r\n");
        iotmodeInfo.send_count = 0;
      } else {//����ֶ����Ӳ��� ������ģ��
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
    case 3://AT+CGATT?  ������Ҫ20�� ���ܶ���
      uart1_init(9600);//��ʱ��Ӳ���
      DelayT_ms(500);
      
      PrintString_("AT+CGATT?\r\n");
      //                if (!IOTMode_SendCmd("AT+CGATT?\r\n", "+CGATT:1", 3000)) //�����ֶ�����
      if (!IOTMode_SendCmd("AT+CGATT?\r\n", "+CGATT:1", 400)) //�����ֶ�����
      {
        iotmodeInfo.initStep++;
        PrintString_("AT+CGATT SUCCESS!\r\n");
        iotmodeInfo.send_count = 0;
      } else {//����ֶ����Ӳ��� ������ģ��
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
    case 4://��ѯ�ź�ǿ��
      if (!GetNBSigStenValue())
      {
        GetNBIotNum();
        iotmodeInfo.initStep++;
        iotmodeInfo.send_count = 0;
      } //��ѯ�ź�ǿ��
      else {
        if (++iotmodeInfo.send_count >= 3) {
          iotmodeInfo.Mode_Restart = 0;
          iotmodeInfo.initStep = 0;
          iotmodeInfo.send_count = 0;
        }
      }
      break;
    case 5://��װ����
      //Increase_send_totalcount();//����һ�ε�ط��ʹ���
      PackageMsg();//�ȷ�װ���ٷ���
      iotmodeInfo.initStep++;
      break;
    case 6://����UDP���� AT+NSOST=0,172.28.92.145,10001,2,AB40   IOTMode_SendCmd_with_length_nb_iot

      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //���Ϳ��ƿ�ָ� ���ԡ� 
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
      Usart_SendString(USART3, (unsigned char *)Sys_keydata.default_cloud_server_add, strlen(Sys_keydata.default_cloud_server_add));//�������// by hx 2017-06-15
      Usart_SendString(USART1,",250,", strlen(",250,"));
      Usart_SendString(USART3,",250,", strlen(",250,"));
      
      
      ////////////////////////////////////////////
      if (!IOTMode_SendCmd_with_length_nb_iot((char *)(&PacketData.senddata[17]), "0,250", 252)) //��������
        //if (!IOTMode_SendCmd_with_length_nb_iot("1234\r\n", "0,2",6)) //��������
      {
        //�в������� �ظ���������
        if (iotmodeInfo.ack_config) {
          iotmodeInfo.initStep++;
        } else {
          //û�в�������
          
          if(iotmodeInfo.send_success_flag){
            iotmodeInfo.initStep = 8;//��������
          }else{//UDP��Ϣû�лظ�  ��Ҫ�ط�
            if (++iotmodeInfo.send_count >= 5) {//�ط�����3��  ������������
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
      } else {//û�յ�����Ҫ�ط�
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //���Ϳ��ƿ�ָ� ���ԡ� 
      DelayT_ms(300);
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //���Ϳ��ƿ�ָ� ���ԡ� 
      DelayT_ms(300);      
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //���Ϳ��ƿ�ָ� ���ԡ� 
      DelayT_ms(300);
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //���Ϳ��ƿ�ָ� ���ԡ� 
      DelayT_ms(300);
      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //���Ϳ��ƿ�ָ� ���ԡ� 

        iotmodeInfo.initStep = 0;
        iotmodeInfo.Mode_Restart = 0;
        iotmodeInfo.send_count = 0;
      }
      break;
    case 7:

      Usart_SendString(USART1, "AT+NSORF=0,56\r\n", 15);
      Usart_SendString(USART3, "AT+NSORF=0,56\r\n", 15);  //���Ϳ��ƿ�ָ� ���ԡ� 
      
      DelayT_ms(1000);
      
      Usart_SendString(USART1,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
      Usart_SendString(USART3,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
      Usart_SendString(USART1, (unsigned char *)Sys_keydata.default_cloud_server_add, strlen(Sys_keydata.default_cloud_server_add));// by hx 2017-06-15
      Usart_SendString(USART3, (unsigned char *)Sys_keydata.default_cloud_server_add, strlen(Sys_keydata.default_cloud_server_add));//�������// by hx 2017-06-15
      Usart_SendString(USART3,",18,", strlen(",18,"));
      Usart_SendString(USART1,",18,", strlen(",18,"));
      
  //    IOTMode_SendCmdWithoutResult((char *)(&ack_protocol_config_array[16]));   
      if (!IOTMode_SendCmd_with_length_nb_iot(((char *)(&ack_protocol_config_array[16])), "0,18",20)) //��������// by hx 2017-06-15
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
          //�������� Ҳ�������
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
          //�������� Ҳ�������
          iotmodeInfo.initStep++;
          iotmodeInfo.Mode_Restart = 0;
          iotmodeInfo.send_count = 0;
        }
      }
      
      break;
    case 10://nb���ͳɹ�  �ر�ģ��
      iotmodeInfo.ack_config = 0;
      iotmodeInfo.reconnect_count = 0;
      iotmodeInfo.send_count = 0;
      iotmodeInfo.Mode_Restart = 0;
      PrintString_("send data finish!!!");
      IOTModePoweroff();//�ر�ģ���Դ
      iotmodeInfo.initStep = 0;//���ص͹��� �ر�ģ���Դ
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

  /**************************************CDMA����*******************************/   
_Bool MC8618_Report(void)
{
  switch (iotmodeInfo.initStep)
  {
        case 0:
          iotmodeInfo.initStep ++;
 	case 1://9600������
		PrintString_("AT+ZPKEEPALIVE=1\r\n");
		if (!IOTMode_SendCmd("AT+ZPKEEPALIVE=1\r\n", "OK", 200)) //����AT
		{

                  UsartPrintf(USART3, "AT+ZPKEEPALIVE=1 OK\r\n");
                  iotmodeInfo.initStep = 11;//��ȡ����
                  
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
		if (!IOTMode_SendCmd("AT+CTA=0\r\n", "OK", 200)) //����AT
		{

                    UsartPrintf(USART3, "AT+CTA=0 OK\r\n");
                    //iotmodeInfo.initStep = 4;
                    //iotmodeInfo.initStep = 6;
                    iotmodeInfo.initStep++;//��ȡ����

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
                IOTMode_SendCmdWithoutResult("at+zpppopen\r\n");//������
                iotmodeInfo.send_count = 0;
                iotmodeInfo.initStep = 4;
		break;
	case 2:
		UsartPrintf(USART3, "AT+IPR=9600\r\n");
		//if(!IOTMode_SendCmd("AT\r\n", "OK",200)){ //����AT{
		UsartPrintf(USART1, "AT+IPR=9600\r\n");//��115200�����������͸ı䲨����Ϊ9600,��������ȴ�
		uart1_init(9600);

		iotmodeInfo.initStep = 1;
		//}
		break;
	case 4:
		PrintString_("at+zpppstatus\r\n");
		if (!IOTMode_SendCmd("at+zpppstatus\r\n", "OPENED", 800)) //��PPP����
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
		//��װƽ̨��ַ��
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
		} //����ƽ̨
		else {
                  debug_send("[ERROR]");
                  iotmodeInfo.initStep = 0;
                  iotmodeInfo.Mode_Restart = 0;
                  iotmodeInfo.send_count = 0;
		}
		break;

	case 6://��ȡ�ź�ǿ�� ����������  �������
		//UsartPrintf(USART3, "AT+CSQ?\r\n");
		if (!GetSigStenValue())
		{
                  GetIotNum();
                  iotmodeInfo.initStep++;
                  iotmodeInfo.send_count = 0;
		} //��ѯ�ź�ǿ��
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
		//Increase_send_totalcount();//����һ�ε�ط��ʹ���
		PackageMsg();//�ȷ�װ���ٷ���
		if (!IOTMode_SendCmd_with_length((char *)(&PacketData.senddata[0]), "ZIPSEND: 250", 269)) //��������
		{
                  //�в������� �ظ���������
                  if (iotmodeInfo.ack_config) {
                          iotmodeInfo.initStep = 9;
                  } else {
                          //û�в�������
                          iotmodeInfo.initStep = 10;
                  }
                        
		} else {//û�յ�����Ҫ�ط�
			iotmodeInfo.initStep = 0;
			iotmodeInfo.send_count = 0;
                        iotmodeInfo.Mode_Restart = 0;
		}
		break;
	case 9://�ظ������� ����������Ŀ
		PrintString_("ack_protocol_config_array");
		if (!IOTMode_SendCmd_with_length((char *)(&ack_protocol_config_array[0]), "ZIPSEND: 18", 36)) {
                    iotmodeInfo.initStep = 10;
                    PrintSysParaList();
		}
		break;
          case 10:
                PrintString_("at+zipclose\r\n");
		if (!IOTMode_SendCmd("at+zipclose\r\n", "OK", 800)) //�Ͽ�����
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
		if (!IOTMode_SendCmd("at+zpppclose\r\n", "OK", 800)) //�Ͽ�����
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
	case 22://���ͳɹ�  �ر�ģ��
		iotmodeInfo.ack_config = 0;
		iotmodeInfo.reconnect_count = 0;
		iotmodeInfo.send_count = 0;
                iotmodeInfo.Mode_Restart = 0;
		PrintString_("send data finish!!!");
                PrintString_(">>>>���ͳɹ��������˳����͹���");
		IOTModePoweroff();//�ر�ģ���Դ
		iotmodeInfo.initStep = 0;//���ص͹��� �ر�ģ���Դ
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
//�ж��ַ��������Ƿ�Ϸ�
_Bool IsStrNumValid(char *str_num_in) {
  //�ж������ַ����Ƿ�Ϸ�
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
      //u8_result = (Sting_in[1] - '0');//BUG 20170306 ǿ��С��10 ����ʾ
      u8_result = (Sting_in[0] - 0x30);
    }
    
  }
  return u8_result;
}

///��ȡ���������ź���
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
///��ȡ���������ź��� //NB��λ��6s����
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
/////��ȡģ���ź�ǿ��
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
					Pack_SigStren(Str2u8(SigStrenValue));//����ź�ǿ��

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
/////��ȡģ���ź�ǿ��
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
          Pack_SigStren(Str2u8(SigStrenValue));//����ź�ǿ��
          
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



//�ж�������У����
_Bool CheckCheckSumFromHeartData(char *heartdata) {
  HEX2STR_2 get_checksum = {'0', '0', '0', '0'};
  //u8 get_heartdata_len = 0;
  //get_heartdata_len = strlen(heartdata);
  
  
  CalcChecksum(&get_checksum, heartdata, 50); //����У����
  PrintString("CHESUM :");
  PrintString_((char *)get_checksum.str2 + 0);
  
  //PrintString_(get_checksum.str2+1);
  //PrintString_(get_checksum.str2+2);
  //PrintString_(get_checksum.str2+3);
  //ArrayCmp �Ա�У����
  if (ArrayCmp((u8 *)(&get_checksum), (u8 *)heartdata + 50, 4)) {
    PrintString_("CHESUM CORRECT");
    return 1;
  }
  else {
    PrintString_("CHESUM ERROR");
    return 0;
  }
  
  /*if(get_heartdata_len == 54){//����Э��������װ��
  
}else if(get_heartdata_len == 16){//�豸δ��װ
  
}else if(get_heartdata_len == 100){//�½ӿ� ������ ��������������ַ
  
}*/
  
}

//������Ϣ  ��ȡ����
RTN_TypeDef GetStrFromStr(char *source_str) {
  char *tmpStr=NULL;  //��ʱָ�롣
  //��ȡУ����
  if (CheckCheckSumFromHeartData(source_str)) {
    
    //��ȡ�������±�־
    if (*(Str2HexFun(source_str, CONFIG_AMD_PARA_FLAG_ADDR, 1)) == 1) {
      //����в������£����������Э��
           Usart1_RcvClr();//�����������
      for(u16 i=0;i<USARTBUF_MAX;i++) //��ջ�����������
      {
        tmp_tab[i] = 0;  
      }
      Usart_SendString(USART1, "AT+NSORF=0,274\r\n", 16);
      Usart_SendString(USART3, "AT+NSORF=0,274\r\n", 16);  //���Ͷ�ȡָ�
      UsartReciveFlag(&usart1Info);
      tmpStr = strstr((const char *)usart1Info.usartBuf, "7E");
      if(NULL != NBiotToPosts(tmpStr,tmp_tab))   //��ȷ��ת��������//by hx 2017-06-10//NBͨѶ��ʽת�ʵ粿Э�顣
      {
        //            //���NB-IOTת�ʵ粿Э�����
        PrintString_((char *)tmp_tab);
        for(u16 i=0;i<USARTBUF_MAX;i++) //��ջ����������ݣ����������λ
        {
          usart1Info.usartBuf[i] = 0;  
        }
        strcpy(usart1Info.usartBuf,tmp_tab); //���ݿ��봮�ڻ�����ģ��һ�����չ��̡�
//        temp_rtn_cmd = GetRebackMsg((char *)tmp_tab);//������Ϣ
      }
      //����һ�ε�ʹ�õ�IP��ַ����,ռ��Ĭ�ϵ�IP��ַ��
      
      
//      usart1Info.usartReceiveFlag = REV_OK;    
      if (Select_protocol(&usart1Info)) { //ѡ��Э�����
        return RTN_OK;
      }
      else {
        return RTN_ERR_INVALID;
      }
    } else {
      
    }
  }
  //��ȡID
  //��ȡ�豸״̬
  //��ȡ�������¿���
  //��ȡ�̼����¿���
  //��ȡ�̼����·�������ַ
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

//��ȡ���ص���Ϣ
RTN_TypeDef GetRebackMsg(char *rck_msg) {
  char *rec_msg = "~1001D000802679696E676A69616E30323031000000000001010000";
  //strlen(strchr(rck_msg,'~'))
  char *end_str;
  //u8 msg_len = 0;//������Ϣ�ĳ���
  
  //��ȡ
  rec_msg = strchr(rck_msg, '~') + 1;
  end_str = strchr(rec_msg, 0x0d);
  *end_str = '\0';
  PrintString_(rec_msg);
  msg_len = strlen(rec_msg);
  PrintU8int2String_(msg_len);
  
  //��ȡ�����ݰ� �������Ѿ�ȥ���˿�ʼ�ַ�0x7e������ַ�0x0d  ��������֤(�ж�У����)
  if (msg_len == 54) { //�豸�Ѿ�������װ ������������
    //��ȡ����
    if (!GetStrFromStr(rec_msg)) {
      return RTN_OK;
    } 
    else 
    {
      return RTN_NULL;
    }
  } else if (msg_len == 16) { //�豸��δ��װ ����06
    
  } else if (msg_len == 100) { //����������ISP��������ַ������Э��
    
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
**������:        NBiotToPosts
**��������:     ��NB IOTģ��ʹ�õ��ַ�����ת��Ϊ�ʵ粿Э�顣
��7E313030314430303030303030464441410D
ת��Ϊ~1001D0000000FDAD

**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
u8* NBiotToPosts(char *parPin,u8 *parPout)
{
  u16 slen=0;
  u16 i;
  u8 tmp=0;
  slen = strlen(parPin);
  //  if(slen> USARTBUF_MAX)      //���ݳ��������Ļ��档��������
  //  {
  //    return NULL;
  //  }
  for(i=0;i<slen;i=i+2)       //�����ַ��ϳ�һ���ַ���
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
  char *tmpStr=NULL;  //��ʱָ�롣
  u16 strLen = 0;
  u8 tmp_tab[USARTBUF_MAX];    //����һ����ʱ�Ļ�������
  
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
        //////////////��ʱ��������/////////////////////
        PrintString_((char *)usart1Info.usartBuf);
        Usart1_RcvClr();
        DelayT_ms(5000);
        PrintString("BEGIN PRINT:");
        PrintString_((char *)usart1Info.usartBuf);
        //				if (strstr((const char *)usart1Info.usartBuf, "+NSONMI:") != NULL) { //����Ϣ����
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
        //					temp_rtn_cmd = GetRebackMsg((char *)usart1Info.usartBuf);//������Ϣ
        //					//��װRTN
        //					if (temp_rtn_cmd == RTN_OK) {
        //						//��ת���ظ�״̬
        //						Pack_Ack_Config_RTN(temp_rtn_cmd);//��װRTN
        //						CalcChecksum((HEX2STR_2 *)(&ack_protocol_config_array[29]), (char *)(&ack_protocol_config_array[17]), 12); //����У����
        //                                                
        //						ack_protocol_config_array[36] = '\0';
        //						iotmodeInfo.ack_config = 1;//�ظ���־��һ
        //					}
        //				} 
        if(strstr((const char *)usart1Info.usartBuf, "+NSONMI:") != NULL) //����Ϣ����
        { 
          tmpStr = strstr((const char *)usart1Info.usartBuf, ",");
          if( tmpStr!= NULL) //��ȡ�������ݵĳ��ȡ�
          { 
            strLen = strlen(tmpStr)-1;    //
            tmpStr++;     //+NSONMI:0,18  ��18��ʼȡ����
            for(int i=0;i<strLen;i++)
            {
              tmp_tab[i]= *tmpStr;
              tmpStr++; 
            }
          }
          //                                        PrintString_((char *)tmp_tab);   //��ӡ���ݳ���         
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
          if(NULL != NBiotToPosts(tmpStr,tmp_tab))   //��ȷ��ת��������//by hx 2017-06-10
          {
            //            //���NB-IOTת�ʵ粿Э�����
            PrintString_((char *)tmp_tab);
            temp_rtn_cmd = GetRebackMsg((char *)tmp_tab);//������Ϣ          
          //��װRTN
          if (temp_rtn_cmd == RTN_OK) 
          {
            //��ת���ظ�״̬
            Pack_Ack_Config_RTN(temp_rtn_cmd);//��װRTN
            CalcChecksum((HEX2STR_2 *)(&ack_protocol_config_array[29]), (char *)(&ack_protocol_config_array[17]), 12); //����У����
            
            ack_protocol_config_array[36] = '\0';
            iotmodeInfo.ack_config = 1;//�ظ���־��һ
          }
          }

        }
        
        else { //���û��UDP��Ϣ �˳�
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
        //////////////��ʱ��������/////////////////////
        PrintString_((char *)usart1Info.usartBuf);
        Usart1_RcvClr();
        DelayT_ms(2000);
        PrintString("BEGIN PRINT:");
        PrintString_((char *)usart1Info.usartBuf);
        
        temp_rtn_cmd = GetRebackMsg((char *)usart1Info.usartBuf);//������Ϣ
        //��װRTN
        if (temp_rtn_cmd == RTN_OK) {
          //��ת���ظ�״̬
          Pack_Ack_Config_RTN(temp_rtn_cmd);//��װRTN
          CalcChecksum((HEX2STR_2 *)(&ack_protocol_config_array[29]), (char *)(&ack_protocol_config_array[17]), 12); //����У����
          
          ack_protocol_config_array[36] = '\0';
          iotmodeInfo.ack_config = 1;//�ظ���־��һ
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

void IOTMode_SendCmdWithoutResult(char *cmd) //�ܻ�ȡ�����ص�����
{
  Usart_SendString(USART1, (unsigned char *)cmd, strlen((const char *)cmd));
}



char *IOTMode_GetStatus(void)
{
  
  char *status = "CONNECT OK";
  
  //IOTMode_QuitTrans();
  
  if (IOTMode_SendCmd("AT\r\n", "OK", 200)) //���������
  {
    status = "No Device";
    //faultTypeReport = faultType = FAULT_NODEVICE;
  }
  
  if (IOTMode_SendCmd("AT+SIM1\r\n", "OK", 200)) //���sim����ʧ
  {
    status = "No Sim Card";
    //faultTypeReport = faultType = FAULT_NODEVICE;
  }
  
  if (IOTMode_SendCmd("AT+CMSTAT\r\n", "OK", 200)) //�������״̬
  {
    status = "Lost Connect";
    //faultTypeReport = faultType = FAULT_EDP;
  }
  
  //IOTMode_EnterTrans();
  
  return status;
  
}



