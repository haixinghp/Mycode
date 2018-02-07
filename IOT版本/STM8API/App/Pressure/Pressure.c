#include "Pressure.h"
#include "ADC.h"
#include "FLASH_Queue.h"
#include "W25QXX.h"
bool sysResetUpdata=FALSE; //TRUE ���ͣ�FALSE��ֹ����
_struPressureOriginal struPressureAData;
_strPressureKeyUpdata strPressureKeyUpdata;
_strPressureControl strPressureControl=
{
  0,//u8 readFlag:1
  0,//u8 enableFlag:1;
  0,//u8 alarmFlag:1;
  0,//u8 alarmFlagbuckup:1;
  0,//u8 save:1;
};

/***************************************************************************************
**������:       getTimesTamp
**��������:     ��ȡ����
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-08,By Haixing.Huang
****************************************************************************************/
u32 *getTimesTamp()
{
  return &struPressureAData.TimesTamp;
}
/***************************************************************************************
**������:       getPressureData
**��������:     ��ȡ����
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureData()
{
  return &struPressureAData.PressureData;
}
/***************************************************************************************
**������:       getAlarmList
**��������:     ��ȡ����
**�������:     --
**�������:     --
**��ע:         YY.c,2017-12-08,By Haixing.Huang
****************************************************************************************/
u16 *getAlarmList()
{
  return &struPressureAData.AlarmList;
}
/***************************************************************************************
**������:       Pressure_GatValue
**��������:     ��ȡ������,���������Чֱ�Ӵ���洢���С�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
void Pressure_Init(_strPressureControl *parP)
{
    parP->alarmFlag = 0;
    parP->alarmFlagbuckup = 0;
    parP->enableFlag =0;
    parP->readFlag = 0;
    parP->save=0;
    parP->CurrentValue = 0;
    parP->CurrentValuePre = 0;
    parP->startupdata=0;
}
/***************************************************************************************
**������:       Pressure_GatValue
**��������:     ��ȡ������,���������Чֱ�Ӵ���洢���С�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
void Pressure_Start(_strPressureControl *parP)
{
  parP->enableFlag = 0;  //���ʹ�ܡ�
  parP->readFlag = 0;
}
/***************************************************************************************
**������:       u16 GetADCValue()
**��������:     ����ADC��ȡ
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u16 GetADCValue()
{
  return filter(ADC_PRESSURE);

}
#if APP_SELECT_PRESSURE
/***************************************************************************************
**������:       u16 GetADCValue()
**��������:     ����ADC��ȡ
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u16 GetPressureValue(u16 value)
{
  double tmp_db;
  tmp_db=(((value*3.3)/4096-0.4)/(1.6))*PRESSURE_MODE;	//1.0mp�汾��ѹ����������
  if(tmp_db < 0) //�����ָ�����
  {
    tmp_db =0;
  }
  return (u16)(tmp_db);
}
#endif


/***************************************************************************************
**������:       Pressure_GatValue
**��������:     ��ȡ������,���������Чֱ�Ӵ���洢���С�
**�������:     --
**�������:     --
**��ע:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
void Pressure_GatValue(_strPressureControl *parP)
{
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���
//  u16 tmp;
  if(0 == parP->enableFlag)  //���û�б�ʧ��
  {
    parP->enableFlag = 1; //ʧ�ܿ�����
    if(0 == parP->readFlag) //�����Ϣ��û�б���ȡ
    {
      parP->readFlag = 1; //֪ͨϵͳ�����Ѿ���ȡ
//      parP->CurrentValue = Pressure_GetValue(ADC_PRESSURE);  //��ȡѹ����ֵ���Ѿ������ݽ���ƽ���˲���
      debug_send_no_end("  ��������ǰ��ѹ��Ϊ��");
      PrintU16int2String_(parP->CurrentValue);
      struPressureAData.PressureData = parP->CurrentValue; //��ȡѹ��ֵ
      Sys_keydata.pressure_value = parP->CurrentValue; //��ȡѹ��ֵ��ϵͳ�ؼ�������
//      struPressureAData.TimesTamp = GetTick();  //��ȡʱ��
      struPressureAData.TimesTamp = TimeGetTick(TRUE);//��ȡʱ��
      Sys_keydata.TimesTamp = struPressureAData.TimesTamp;
//      EnQueue(&myqueue,&struPressureAData);  //���浱ǰ����FIFO��ȡ
//      EnFLASH_Queue(struPressureAData.TimesTamp,struPressureAData.PressureData);
      
      switch(Pressure_AnalyseState(&Sys_keydata,&struPressureAData))
      {
        case 1://����
          uniAlarm.struAlarmBit.pressureAlarmUpLimit = 1;
        break;
        case 2://����
            uniAlarm.struAlarmBit.pressureAlarmDownLimit = 1;
        break;
      default:
        uniAlarm.struAlarmBit.pressureAlarmUpLimit = 0;
        uniAlarm.struAlarmBit.pressureAlarmDownLimit =0;
        //������������еĸ澯 
        break;
      }
      
    }
  }
#endif
}
///***************************************************************************************
//**������:       Pressure_AnalyseState
//**��������:     ��ÿ���ɼ�����ѹ�����ݽ��з�����ĿǰΪ�����޹�����ʽ��
//��������Ϊ��ѹ��������������Ϊ��ѹ������
//���ݼ�¼����
//**�������:    SYS_KEYDATA *parP,_struPressureOriginal *parPa
//**�������:    1���ޱ�����2���ޱ���
//**��ע:         YY.c,2017-06-27,By Haixing.Huang
//****************************************************************************************/
u8 Pressure_AnalyseState(SYS_KEYDATA *parP,_struPressureOriginal *parPa)
{
#if APP_SELECT_PRESSURE   //һ�´�����ˮѹ�������еĴ���  
  if((parPa->PressureData) > (parP->pressure_upLimit ))  //
  {
    //���޸澯
    return 1;
  }
  if((parPa->PressureData) < (parP->pressure_downLimit )) 
  {
    //���ޱ���
    return 2;
  }
#endif
  return 0;  //����������û�з����澯��
}

/***************************************************************************************
**������:       Pressure_Updata
**��������:     ѹ���ϴ����ݡ�
**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
u8 Pressure_Updata(void)
{
  int i;
  union _uni_u16_tmp
  {
    u16 tmp;
    u8 tmp_tab[2];
  };
  union _uni_u16_tmp uni_u16_tmp;
//  if(0 == strPressureControl.refreshupdata)  //ÿ�η���������ݳɲ�����������FLAG
  {
//    strPressureControl.refreshupdata = 1; //ˢ�����ݡ�
    if(1 == IsFLASH_QueueEmpty()) //�������Ƿ�Ϊ��
    {
      debug_send("QueueEmpty1!");
      strPressureControl.startupdata = 0; //������ͱ�־��
      strPressureControl.enableFlag =1; //��λ������־��
      uni_u16_tmp.tmp = 0;  //��������е�����
//      FM24CLXX_Read(CONST_QUEUE_SENDBUFF_CNT_ADDR,//���浽FLASH
//                    uni_u16_tmp.tmp_tab,
//                    CONST_QUEUE_SENDBUFF_CNT_LEN);  //��ȡ���������Ĵ�С��
      //��ջ����� 2017-09-21 
      return 0; //ֱ���˳����ݷ��͡�
    }
    else  //��ȡ���ݵ���������ÿ������ȡ38�顣��Ҫд��FLASH���档
    {
      for(i=0;i<(CONST_PRESSUREDATA_MAX_LEN);i++) //һ����໺��
      {
//        if(1 == IsQueueEmpty(&myqueue)) //�������Ƿ�Ϊ�ա�
        Watchdog_Feed();  //���Է��֣������������������ڼ���ι�������в��ԡ�
        if(1 == IsFLASH_QueueEmpty()) //�������Ƿ�Ϊ�ա�
        {
          debug_send("QueueEmpty2!");
          strPressureControl.startupdata = 0;  //������ݰ��ϴ���
          break;  //������Ϊ�գ�ǿ���˳�
        }
        else
        {
//          DeQueue(&myqueue,&struPressureAData);//�ӱ��л�ȡ����
//          DeFLASH_Queue(&struPressureAData.TimesTamp,&struPressureAData.PressureData,&struPressureAData.AlarmList);
          struPressureDataOriginal.struDataOriginal[i].TimesTamp=struPressureAData.TimesTamp;
          struPressureDataOriginal.struDataOriginal[i].PressureData = struPressureAData.PressureData;
          struPressureDataOriginal.struDataOriginal[i].AlarmList = struPressureAData.AlarmList;
        }
      }
      struPressureDataOriginal.len = i;  //��д���ݸ���
//      uni_u16_tmp.tmp = i;  //��д���ݸ���
//      FM24CLXX_Write(CONST_QUEUE_SENDBUFF_CNT_ADDR,
//                    uni_u16_tmp.tmp_tab,
//                    CONST_QUEUE_SENDBUFF_CNT_LEN);  //д�뻺������ݸ�����
    }
//    Pack_PressureUploadData(&uniSendBuf,struPressureDataOriginal.len);  //�����ݴ��
    return 1;
  }
}
/***************************************************************************************
**������:       Pressure_Updata
**��������:     ѹ���ϴ����ݡ�
**�������:     --
**�������:     --
**��ע:         YY.c,2015/3/11,By Haixing.Huang
****************************************************************************************/
u8 Pressure_CheckBuff(_struPressureDataOriginal *parP)
{
//  union _uni_u16_tmp
//  {
//    u16 tmp;
//    u8 tmp_tab[2];
//  };
//  union _uni_u16_tmp uni_u16_tmp;
//  FM24CLXX_Read(CONST_QUEUE_SENDBUFF_CNT_ADDR,
//                  uni_u16_tmp.tmp_tab,
//                  CONST_QUEUE_SENDBUFF_CNT_LEN);  //��ȡ���������Ĵ�С��
// parP->len = uni_u16_tmp.tmp;  //�ӻ�������ȡ���ݳ��ȡ�
// if((0xFFFF == uni_u16_tmp.tmp) || (0x0000 == uni_u16_tmp.tmp))  //˵�����ݻ����������ݻ�û�д��롣���߷������
// {
//   //���ݿ�
//   return 0;
// }
// else //
// {
//   //�����������䵽RAM����
//   if(uni_u16_tmp.tmp >(CONST_QUEUE_SENDBUFF_LEN/6))
//   {
//     struPressureDataOriginal.len = 0; //������ȡ�
//     return 0; //��������
//   }
//   else
//   {
////     FM24CLXX_Read(CONST_QUEUE_SENDBUFF_ADDR,
////                  (u8 *)(&struPressureDataOriginal.struDataOriginal[0].TimesTamp), 
////                  uni_u16_tmp.tmp);  //��ȡFLASH����������
//     struPressureDataOriginal.len = uni_u16_tmp.tmp;
//     FM24CLXX_Read(CONST_QUEUE_SENDBUFF_ADDR,
//                  (u8 *)(&struPressureDataOriginal.struDataOriginal[0].TimesTamp), 
//                  uni_u16_tmp.tmp*6);  //��ȡFLASH����������     
//   }
////   Pack_PressureUploadData(&uniSendBuf,struPressureDataOriginal.len);  //�����ݴ��
//   return 1;
// }
  
  if(0 != parP->len) //���������
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
/***************************************************************************************
**������:       Pressure_SendBufClear
**��������:     �����������FLASH��RAM���塣
**�������:     --
**�������:     --
**��ע:         YY.c,2017-07-10,By Haixing.Huang
****************************************************************************************/
//u8 Pressure_RamBuf(_struPressureDataOriginal *parP,u16 parNum)
//{
//  uint i;
//  parP->len =0;
//  for(i=0;i<parNum;i++)
//  {
//    parP->struDataOriginal[i].TimesTamp=0;
//    parP->struDataOriginal[i].PressureData=0;
//  } //���RAM
//}

///***************************************************************************************
//**������:       Pressure_SendBufClear
//**��������:     �����������FLASH��RAM���塣
//**�������:     --
//**�������:     --
//**��ע:         YY.c,2017-07-10,By Haixing.Huang
//****************************************************************************************/
//u8 Pressure_SendBufClear(_struPressureDataOriginal *parP,u16 parNum,u16 parLen)
//{
//  u16 i;
//  u16 tmp_u16;
//  tmp_u16 = parNum/6;
//  union _uni_u16_tmp
//  {
//    u16 tmp;
//    u8 tmp_tab[2];
//  };
//  union _uni_u16_tmp uni_u16_tmp;
//  uni_u16_tmp.tmp = parLen;
//  for(i=0;i<tmp_u16;i++)
//  {
//    parP->struDataOriginal[i].TimesTamp=0;
//    parP->struDataOriginal[i].PressureData=0;
//  } //���RAM
//  parP->len = 0;        //������ݳ���
////  FM24CLXX_Write(CONST_QUEUE_SENDBUFF_ADDR,
////                    (u8 *)(parP->struDataOriginal),
////                    tmp_u16);  //д�뻺������ݸ�����
////  FM24CLXX_Write(CONST_QUEUE_SENDBUFF_CNT_ADDR,
////                    uni_u16_tmp.tmp_tab,
////                    CONST_QUEUE_SENDBUFF_CNT_LEN);  //д�뻺������ݸ�����
//  return 0;
//}
/***************************************************************************************
**������:       Pressure_SendBufClear
**��������:     �����������FLASH��RAM���塣
**�������:     --
**�������:     --
**��ע:         YY.c,2017-07-10,By Haixing.Huang
****************************************************************************************/
u8 Pressure_SendBufClear(_struPressureDataOriginal *parP)
{
  u16 i;
  for(i=0;i<CONST_PRESSUREDATA_MAX_LEN;i++)
  {
    parP->struDataOriginal[i].TimesTamp=0;
    parP->struDataOriginal[i].PressureData=0;
  } //���RAM
  parP->len = 0;        //������ݳ���
  return 0;
}
/***************************************************************************************
**������:       Pressure_Send()
**��������:     ������ʵ����ֱ�Ӵ���ϴ�����ʹ�û��塣
**�������:     --
**�������:     --
**��ע:         YY.c,2017-07-22,By Haixing.Huang
****************************************************************************************/
u8 Pressure_Send(_struPressureDataOriginal *parP)
{

//  u8 send_buftmp[40];
//  HEX2STR_1  hex2str1_tmp; 
//  HEX2STR_2  hex2str2_tmp;
//  HEX2STR_4  hex2str4_tmp;
//  HEX2STR_16 hex2str16_tmp;
//  u16 i,j;
//  u16 sumnum = 0;
//  u16 tmp_u16 =0;
//  tmp_u16 = strlen((const char *)("~1001D01B"));
//  strcpy((char *)send_buftmp,(const char *)"~1001D01B");                  
//  for(i=1;i<tmp_u16;i++)
//  {
//    sumnum += send_buftmp[i];
//  }
//  Usart_SendString(USART3, send_buftmp, 9);
//  Usart_SendString(USART1, send_buftmp, 9);
//
//  tmp_u16 = POST_CalcLen(((u16)(parP->len)*6));  //��ȡ���ݳ��ȡ�
//  hex2str2_tmp = Hex2StrFun_2(&tmp_u16);
//  for(i=0;i<4;i++)
//  {
//    sumnum += hex2str2_tmp.str2[i];
//  }
//  Usart_SendString(USART3, hex2str2_tmp.str2,4);
//  Usart_SendString(USART1, hex2str2_tmp.str2,4); //�����ʵ粿���ȡ�
//
//  hex2str16_tmp = Hex2StrFun_16((char *)Sys_keydata.device_id);//��װ�豸ID
//  for(i=0;i<32;i++)
//  {
//    sumnum += hex2str16_tmp.str16[i];
//  }
//  Usart_SendString(USART3, hex2str16_tmp.str16,32);
//  Usart_SendString(USART1, hex2str16_tmp.str16,32); //����
//  
//  
//  hex2str1_tmp = Hex2StrFun_1(&(parP->len));
//  for(i=0;i<2;i++)
//  {
//    sumnum += hex2str1_tmp.str1[i];
//  }
//  Usart_SendString(USART3, hex2str1_tmp.str1,2);
//  Usart_SendString(USART1, hex2str1_tmp.str1,2); //����
//  for(i=0;i<parP->len;i++)  //������ݡ�
//  {
//    hex2str4_tmp = Hex2StrFun_4(&struPressureDataOriginal.struDataOriginal[i].TimesTamp);
//    Usart_SendString(USART3, hex2str4_tmp.str4,8);
//    Usart_SendString(USART1, hex2str4_tmp.str4,8); //����
//    for(j=0;j<8;j++)
//    {
//      sumnum += hex2str4_tmp.str4[j];
//    }
//    hex2str2_tmp = Hex2StrFun_2(&struPressureDataOriginal.struDataOriginal[i].PressureData);
//    for(j=0;j<4;j++)
//    {
//      sumnum += hex2str2_tmp.str2[j];
//    }
//    Usart_SendString(USART3, hex2str2_tmp.str2,4);
//    Usart_SendString(USART1, hex2str2_tmp.str2,4); //����
//  }
//   sumnum = (~(sumnum % 65535)) + 1;
//  hex2str2_tmp = Hex2StrFun_2(&sumnum);
//  Usart_SendString(USART3, hex2str2_tmp.str2,4);
//  Usart_SendString(USART1, hex2str2_tmp.str2,4); //����
////  tmp_u8 = 0X0D;
////  Usart_SendString(USART3, (char *)&tmp_u8,1);
////  Usart_SendString(USART1, (char *)&tmp_u8,1); //���� 
//  Usart_SendString(USART3,"\r\n",2);
//  Usart_SendString(USART1,"\r\n",2);
//  sprintf(str, " sumnum:%d", sumnum);
//  debug_send(str); 
//  PrintString("CHESUM :");  
//  for(i=0;i<4;i++)
//  {
//    send_buftmp[i] = hex2str2_tmp.str2[i];
//  }
//  send_buftmp[i] = 0;
//  PrintString_((char *)send_buftmp);
//  
  return 0;
}
/***************************************************************************************
**������:       PressureUpDataDeal()
**��������:     �ϴ����ݴ�����򣬰����˷��ͺ�����
**�������:     --
**�������:     0�����������1�������� 
**��ע:         YY.c,2017-07-22,By Haixing.Huang
****************************************************************************************/
u8 PressureUpDataDeal()
{
//  switch(strPressureControl.updataStatus)
//  {
//  case 0: //���ͻ���״̬
//    //��黺�����е����ݵĸ���
//    if(Pressure_CheckBuff(&struPressureDataOriginal)) //���������������
//    {
//      strPressureControl.updataStatus =3; //���͹���
//    }
//    else //û������
//    {
//      strPressureControl.updataStatus = 1; //��FIFO��ȡ����ϵͳ����.
//    }
//    return 1;
//    break;
//  case 1://��FIFO��ȡ����ϵͳ����.
//    if(Pressure_Updata())
//    {
//      strPressureControl.updataStatus =3; //���͹���
//    }
//    else
//    {
//      strPressureControl.updataStatus =4;
//    }
//    return 1;
//    break;
//  case 2://�ظ�����
//    strPressureControl.failCnt ++;
//    if( strPressureControl.failCnt > 3)
//    {
//      PrintString_("updata failCnt>3"); 
//      strPressureControl.updataStatus = 4; //�˳��������ݴ�״̬
//    }
//    else
//    {
//      strPressureControl.updataStatus =3; //���͹���
//    }
//    break;  
//  case 3://���͹���
//    PrintString_("start updata!");
//    /****************************************NB���ͺ���************************************************************/
//    //    Usart_SendString(USART1,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
//    //    Usart_SendString(USART3,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
//    //    Usart_SendString(USART1, (unsigned char *)Sys_keydata.cloud_server_add, strlen((char *)(Sys_keydata.cloud_server_add)));// by hx 2017-06-15
//    //    Usart_SendString(USART3, (unsigned char *)Sys_keydata.cloud_server_add, strlen((char *)(Sys_keydata.cloud_server_add)));//�������// by hx 2017-06-15
//    //    sprintf(str, "%d",(struPressureDataOriginal.len*12+52)); 
//    //    Usart_SendString(USART3,",", strlen(","));
//    //    Usart_SendString(USART1,",", strlen(","));
//    //    Usart_SendString(USART3, (unsigned char *)str, strlen(str));
//    //    Usart_SendString(USART1, (unsigned char *)str, strlen(str)); 
//    //    Usart_SendString(USART3,",", strlen(","));
//    //    Usart_SendString(USART1,",", strlen(","));
//    
//    Usart_SendString(USART1,"at+zipsend=0,", strlen("at+zipsend=0,"));
//    Usart_SendString(USART3,"at+zipsend=0,", strlen("at+zipsend=0,"));
//    //    Usart_SendString(USART1, (unsigned char *)Sys_keydata.cloud_server_add, strlen((char *)(Sys_keydata.cloud_server_add)));// by hx 2017-06-15
//    //    Usart_SendString(USART3, (unsigned char *)Sys_keydata.cloud_server_add, strlen((char *)(Sys_keydata.cloud_server_add)));//�������// by hx 2017-06-15
//    sprintf(str, "%d",(struPressureDataOriginal.len*12+52)); 
//    //    Usart_SendString(USART3,",", strlen(","));
//    //    Usart_SendString(USART1,",", strlen(","));
//    Usart_SendString(USART3, (unsigned char *)str, strlen(str));
//    Usart_SendString(USART1, (unsigned char *)str, strlen(str)); 
//    Usart_SendString(USART3,"\r", strlen("\r"));
//    Usart_SendString(USART1,"\r", strlen("\r"));
////    Usart_SendString(USART3,",", strlen(","));
////    Usart_SendString(USART1,",", strlen(","));
//    
//    //    DelayT_ms(100);
//    //    PrintString_("start updata!");
//    Pressure_Send(&struPressureDataOriginal);  //��ʵ�������ݡ�
//    if (!MC8618_SendCmd_with_length((char *)(str),//ģ�ⷢ�ͣ�ʵ��û�з����κε����ݣ�ֻ�����ڽ��ա�
//                                    "OK",
//                                    2)) 
//    {
//      PrintString_("updata send Succsss!");//���ͳɹ���
//      strPressureControl.updataStatus = 1; //������FIFO��ȡ���ݡ�
//      struPressureDataOriginal.len = 0;      //������ݳ��ȡ�
////      Pressure_SendBufClear(&struPressureDataOriginal,CONST_QUEUE_SENDBUFF_LEN,struPressureDataOriginal.len);//���FLASH�еķ��ͻ���
//      Pressure_SendBufClear(&struPressureDataOriginal);
//      
//    }
//    else  //����ʧ��
//    {
//      PrintString_("updata send fail!"); 
//      strPressureControl.updataStatus = 2; //���������ط�����
//      //                  strPressureControl.SendRepeatedlyFlag = 1; //���¿�ʼ��־��
//    }
//    return 1;
//    break;  
//  case 4://�˳��������ݴ�״̬
//    strPressureControl.updataStatus = 0; //Ĭ��Ϊ���ͻ���״̬��
//    strPressureControl.failCnt =0; //���ʧ�ܼ�����
//    uniAlarm.AlarmAllData = 0X00; //������еĸ澯
//    return 0;
//  default:
//    break;
//    break;  
//  }
  return 1;
}

/***************************************************************************************
**������:       PressureKeyUpdataIntterSetFlag()
**��������:     �����жϷ������
**�������:     --
**�������:      
**��ע:         YY.c,2017-11-16,By Haixing.Huang
****************************************************************************************/
void PressureKeyUpdataIntterSetFlag()
{
  strPressureKeyUpdata.haveKey = TRUE;
}
/***************************************************************************************
**������:       PressureKeyUpdataConfirm()
**��������:     �����жϷ������
**�������:     --
**�������:      
**��ע:         YY.c,2017-11-16,By Haixing.Huang
****************************************************************************************/
s8 PressureKeyUpdataConfirm()
{
  if(TRUE == strPressureKeyUpdata.haveKey)
  {
    strPressureKeyUpdata.haveKey =  FALSE; 
//    if((0x01<<5) != GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5))
    if((0x00) == GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5))
    {
      DelayT_ms(20);
//      if((0x01<<5) != GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5))
      if((0x00) == GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5))
      {   
        return 1;
//        debug_send("���������ϱ�����!");
//        uniAlarm.struAlarmBit.KeyUpdata = 1; 
//        ENTER_SENDALARM_STATUS; //���뷢������; �Զ������
      }
    }
  }
  return 0;
}


