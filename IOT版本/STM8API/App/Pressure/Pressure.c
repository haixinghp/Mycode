#include "Pressure.h"
#include "ADC.h"
#include "FLASH_Queue.h"
#include "W25QXX.h"
bool sysResetUpdata=FALSE; //TRUE 发送，FALSE禁止发送
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
**函数名:       getTimesTamp
**功能描述:     获取数据
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-08,By Haixing.Huang
****************************************************************************************/
u32 *getTimesTamp()
{
  return &struPressureAData.TimesTamp;
}
/***************************************************************************************
**函数名:       getPressureData
**功能描述:     获取数据
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-08,By Haixing.Huang
****************************************************************************************/
u16 *getPressureData()
{
  return &struPressureAData.PressureData;
}
/***************************************************************************************
**函数名:       getAlarmList
**功能描述:     获取数据
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-12-08,By Haixing.Huang
****************************************************************************************/
u16 *getAlarmList()
{
  return &struPressureAData.AlarmList;
}
/***************************************************************************************
**函数名:       Pressure_GatValue
**功能描述:     获取到数据,如果数据有效直接存入存储器中。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-27,By Haixing.Huang
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
**函数名:       Pressure_GatValue
**功能描述:     获取到数据,如果数据有效直接存入存储器中。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
void Pressure_Start(_strPressureControl *parP)
{
  parP->enableFlag = 0;  //清除使能。
  parP->readFlag = 0;
}
/***************************************************************************************
**函数名:       u16 GetADCValue()
**功能描述:     调用ADC获取
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u16 GetADCValue()
{
  return filter(ADC_PRESSURE);

}
#if APP_SELECT_PRESSURE
/***************************************************************************************
**函数名:       u16 GetADCValue()
**功能描述:     调用ADC获取
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
u16 GetPressureValue(u16 value)
{
  double tmp_db;
  tmp_db=(((value*3.3)/4096-0.4)/(1.6))*PRESSURE_MODE;	//1.0mp版本的压力传感器。
  if(tmp_db < 0) //不出现负数。
  {
    tmp_db =0;
  }
  return (u16)(tmp_db);
}
#endif


/***************************************************************************************
**函数名:       Pressure_GatValue
**功能描述:     获取到数据,如果数据有效直接存入存储器中。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-06-27,By Haixing.Huang
****************************************************************************************/
void Pressure_GatValue(_strPressureControl *parP)
{
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码
//  u16 tmp;
  if(0 == parP->enableFlag)  //如果没有被失能
  {
    parP->enableFlag = 1; //失能控制器
    if(0 == parP->readFlag) //如果消息还没有被读取
    {
      parP->readFlag = 1; //通知系统数据已经读取
//      parP->CurrentValue = Pressure_GetValue(ADC_PRESSURE);  //获取压力的值，已经对数据进行平均滤波。
      debug_send_no_end("  》》》当前的压力为：");
      PrintU16int2String_(parP->CurrentValue);
      struPressureAData.PressureData = parP->CurrentValue; //获取压力值
      Sys_keydata.pressure_value = parP->CurrentValue; //获取压力值到系统关键参数。
//      struPressureAData.TimesTamp = GetTick();  //获取时间
      struPressureAData.TimesTamp = TimeGetTick(TRUE);//获取时间
      Sys_keydata.TimesTamp = struPressureAData.TimesTamp;
//      EnQueue(&myqueue,&struPressureAData);  //保存当前数据FIFO存取
//      EnFLASH_Queue(struPressureAData.TimesTamp,struPressureAData.PressureData);
      
      switch(Pressure_AnalyseState(&Sys_keydata,&struPressureAData))
      {
        case 1://上限
          uniAlarm.struAlarmBit.pressureAlarmUpLimit = 1;
        break;
        case 2://下限
            uniAlarm.struAlarmBit.pressureAlarmDownLimit = 1;
        break;
      default:
        uniAlarm.struAlarmBit.pressureAlarmUpLimit = 0;
        uniAlarm.struAlarmBit.pressureAlarmDownLimit =0;
        //清除上下限所有的告警 
        break;
      }
      
    }
  }
#endif
}
///***************************************************************************************
//**函数名:       Pressure_AnalyseState
//**功能描述:     对每个采集到的压力数据进行分析，目前为上下限工作方式。
//低于下限为低压报警，高于上限为高压报警，
//数据记录采用
//**输入参数:    SYS_KEYDATA *parP,_struPressureOriginal *parPa
//**输出参数:    1上限报警，2下限报警
//**备注:         YY.c,2017-06-27,By Haixing.Huang
//****************************************************************************************/
u8 Pressure_AnalyseState(SYS_KEYDATA *parP,_struPressureOriginal *parPa)
{
#if APP_SELECT_PRESSURE   //一下代码是水压程序特有的代码  
  if((parPa->PressureData) > (parP->pressure_upLimit ))  //
  {
    //上限告警
    return 1;
  }
  if((parPa->PressureData) < (parP->pressure_downLimit )) 
  {
    //下限报警
    return 2;
  }
#endif
  return 0;  //数据正常，没有发生告警。
}

/***************************************************************************************
**函数名:       Pressure_Updata
**功能描述:     压力上传数据。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
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
//  if(0 == strPressureControl.refreshupdata)  //每次发送完成数据成才能清除掉这个FLAG
  {
//    strPressureControl.refreshupdata = 1; //刷新数据。
    if(1 == IsFLASH_QueueEmpty()) //检查队列是否为空
    {
      debug_send("QueueEmpty1!");
      strPressureControl.startupdata = 0; //清除发送标志。
      strPressureControl.enableFlag =1; //置位结束标志。
      uni_u16_tmp.tmp = 0;  //清除缓冲中的数据
//      FM24CLXX_Read(CONST_QUEUE_SENDBUFF_CNT_ADDR,//保存到FLASH
//                    uni_u16_tmp.tmp_tab,
//                    CONST_QUEUE_SENDBUFF_CNT_LEN);  //获取到缓冲区的大小。
      //清空缓冲区 2017-09-21 
      return 0; //直接退出数据发送。
    }
    else  //读取数据到缓冲区。每次最大读取38组。需要写入FLASH缓存。
    {
      for(i=0;i<(CONST_PRESSUREDATA_MAX_LEN);i++) //一次最多缓存
      {
//        if(1 == IsQueueEmpty(&myqueue)) //检查队列是否为空。
        Watchdog_Feed();  //测试发现，会有重启的现象，现在加入喂狗，进行测试。
        if(1 == IsFLASH_QueueEmpty()) //检查队列是否为空。
        {
          debug_send("QueueEmpty2!");
          strPressureControl.startupdata = 0;  //清除数据包上传。
          break;  //数据列为空，强制退出
        }
        else
        {
//          DeQueue(&myqueue,&struPressureAData);//从表中获取数据
//          DeFLASH_Queue(&struPressureAData.TimesTamp,&struPressureAData.PressureData,&struPressureAData.AlarmList);
          struPressureDataOriginal.struDataOriginal[i].TimesTamp=struPressureAData.TimesTamp;
          struPressureDataOriginal.struDataOriginal[i].PressureData = struPressureAData.PressureData;
          struPressureDataOriginal.struDataOriginal[i].AlarmList = struPressureAData.AlarmList;
        }
      }
      struPressureDataOriginal.len = i;  //填写数据个数
//      uni_u16_tmp.tmp = i;  //填写数据个数
//      FM24CLXX_Write(CONST_QUEUE_SENDBUFF_CNT_ADDR,
//                    uni_u16_tmp.tmp_tab,
//                    CONST_QUEUE_SENDBUFF_CNT_LEN);  //写入缓存的数据个数。
    }
//    Pack_PressureUploadData(&uniSendBuf,struPressureDataOriginal.len);  //把数据打包
    return 1;
  }
}
/***************************************************************************************
**函数名:       Pressure_Updata
**功能描述:     压力上传数据。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2015/3/11,By Haixing.Huang
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
//                  CONST_QUEUE_SENDBUFF_CNT_LEN);  //获取到缓冲区的大小。
// parP->len = uni_u16_tmp.tmp;  //从缓冲区获取数据长度。
// if((0xFFFF == uni_u16_tmp.tmp) || (0x0000 == uni_u16_tmp.tmp))  //说明数据缓冲区的数据还没有存入。或者发送完成
// {
//   //数据空
//   return 0;
// }
// else //
// {
//   //把数据填充填充到RAM区域。
//   if(uni_u16_tmp.tmp >(CONST_QUEUE_SENDBUFF_LEN/6))
//   {
//     struPressureDataOriginal.len = 0; //清除长度。
//     return 0; //数据有误
//   }
//   else
//   {
////     FM24CLXX_Read(CONST_QUEUE_SENDBUFF_ADDR,
////                  (u8 *)(&struPressureDataOriginal.struDataOriginal[0].TimesTamp), 
////                  uni_u16_tmp.tmp);  //获取FLASH到缓冲区的
//     struPressureDataOriginal.len = uni_u16_tmp.tmp;
//     FM24CLXX_Read(CONST_QUEUE_SENDBUFF_ADDR,
//                  (u8 *)(&struPressureDataOriginal.struDataOriginal[0].TimesTamp), 
//                  uni_u16_tmp.tmp*6);  //获取FLASH到缓冲区的     
//   }
////   Pack_PressureUploadData(&uniSendBuf,struPressureDataOriginal.len);  //把数据打包
//   return 1;
// }
  
  if(0 != parP->len) //如果有数据
  {
    return 1;
  }
  else
  {
    return 0;
  }
}
/***************************************************************************************
**函数名:       Pressure_SendBufClear
**功能描述:     清除发送数据FLASH和RAM缓冲。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-07-10,By Haixing.Huang
****************************************************************************************/
//u8 Pressure_RamBuf(_struPressureDataOriginal *parP,u16 parNum)
//{
//  uint i;
//  parP->len =0;
//  for(i=0;i<parNum;i++)
//  {
//    parP->struDataOriginal[i].TimesTamp=0;
//    parP->struDataOriginal[i].PressureData=0;
//  } //清空RAM
//}

///***************************************************************************************
//**函数名:       Pressure_SendBufClear
//**功能描述:     清除发送数据FLASH和RAM缓冲。
//**输入参数:     --
//**输出参数:     --
//**备注:         YY.c,2017-07-10,By Haixing.Huang
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
//  } //清空RAM
//  parP->len = 0;        //清除数据长度
////  FM24CLXX_Write(CONST_QUEUE_SENDBUFF_ADDR,
////                    (u8 *)(parP->struDataOriginal),
////                    tmp_u16);  //写入缓存的数据个数。
////  FM24CLXX_Write(CONST_QUEUE_SENDBUFF_CNT_ADDR,
////                    uni_u16_tmp.tmp_tab,
////                    CONST_QUEUE_SENDBUFF_CNT_LEN);  //写入缓存的数据个数。
//  return 0;
//}
/***************************************************************************************
**函数名:       Pressure_SendBufClear
**功能描述:     清除发送数据FLASH和RAM缓冲。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-07-10,By Haixing.Huang
****************************************************************************************/
u8 Pressure_SendBufClear(_struPressureDataOriginal *parP)
{
  u16 i;
  for(i=0;i<CONST_PRESSUREDATA_MAX_LEN;i++)
  {
    parP->struDataOriginal[i].TimesTamp=0;
    parP->struDataOriginal[i].PressureData=0;
  } //清空RAM
  parP->len = 0;        //清除数据长度
  return 0;
}
/***************************************************************************************
**函数名:       Pressure_Send()
**功能描述:     根据真实数据直接打包上传，不使用缓冲。
**输入参数:     --
**输出参数:     --
**备注:         YY.c,2017-07-22,By Haixing.Huang
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
//  tmp_u16 = POST_CalcLen(((u16)(parP->len)*6));  //获取数据长度。
//  hex2str2_tmp = Hex2StrFun_2(&tmp_u16);
//  for(i=0;i<4;i++)
//  {
//    sumnum += hex2str2_tmp.str2[i];
//  }
//  Usart_SendString(USART3, hex2str2_tmp.str2,4);
//  Usart_SendString(USART1, hex2str2_tmp.str2,4); //发送邮电部长度。
//
//  hex2str16_tmp = Hex2StrFun_16((char *)Sys_keydata.device_id);//封装设备ID
//  for(i=0;i<32;i++)
//  {
//    sumnum += hex2str16_tmp.str16[i];
//  }
//  Usart_SendString(USART3, hex2str16_tmp.str16,32);
//  Usart_SendString(USART1, hex2str16_tmp.str16,32); //发送
//  
//  
//  hex2str1_tmp = Hex2StrFun_1(&(parP->len));
//  for(i=0;i<2;i++)
//  {
//    sumnum += hex2str1_tmp.str1[i];
//  }
//  Usart_SendString(USART3, hex2str1_tmp.str1,2);
//  Usart_SendString(USART1, hex2str1_tmp.str1,2); //发送
//  for(i=0;i<parP->len;i++)  //填充数据。
//  {
//    hex2str4_tmp = Hex2StrFun_4(&struPressureDataOriginal.struDataOriginal[i].TimesTamp);
//    Usart_SendString(USART3, hex2str4_tmp.str4,8);
//    Usart_SendString(USART1, hex2str4_tmp.str4,8); //发送
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
//    Usart_SendString(USART1, hex2str2_tmp.str2,4); //发送
//  }
//   sumnum = (~(sumnum % 65535)) + 1;
//  hex2str2_tmp = Hex2StrFun_2(&sumnum);
//  Usart_SendString(USART3, hex2str2_tmp.str2,4);
//  Usart_SendString(USART1, hex2str2_tmp.str2,4); //发送
////  tmp_u8 = 0X0D;
////  Usart_SendString(USART3, (char *)&tmp_u8,1);
////  Usart_SendString(USART1, (char *)&tmp_u8,1); //发送 
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
**函数名:       PressureUpDataDeal()
**功能描述:     上传数据处理程序，包含了发送函数。
**输入参数:     --
**输出参数:     0，处理结束，1继续处理。 
**备注:         YY.c,2017-07-22,By Haixing.Huang
****************************************************************************************/
u8 PressureUpDataDeal()
{
//  switch(strPressureControl.updataStatus)
//  {
//  case 0: //发送缓冲状态
//    //检查缓冲器中的数据的个数
//    if(Pressure_CheckBuff(&struPressureDataOriginal)) //如果缓冲器有数据
//    {
//      strPressureControl.updataStatus =3; //发送过程
//    }
//    else //没有数据
//    {
//      strPressureControl.updataStatus = 1; //从FIFO获取数据系统缓存.
//    }
//    return 1;
//    break;
//  case 1://从FIFO获取数据系统缓存.
//    if(Pressure_Updata())
//    {
//      strPressureControl.updataStatus =3; //发送过程
//    }
//    else
//    {
//      strPressureControl.updataStatus =4;
//    }
//    return 1;
//    break;
//  case 2://重复发送
//    strPressureControl.failCnt ++;
//    if( strPressureControl.failCnt > 3)
//    {
//      PrintString_("updata failCnt>3"); 
//      strPressureControl.updataStatus = 4; //退出发送数据串状态
//    }
//    else
//    {
//      strPressureControl.updataStatus =3; //发送过程
//    }
//    break;  
//  case 3://发送过程
//    PrintString_("start updata!");
//    /****************************************NB发送函数************************************************************/
//    //    Usart_SendString(USART1,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
//    //    Usart_SendString(USART3,"AT+NSOST=0,", strlen("AT+NSOST=0,"));
//    //    Usart_SendString(USART1, (unsigned char *)Sys_keydata.cloud_server_add, strlen((char *)(Sys_keydata.cloud_server_add)));// by hx 2017-06-15
//    //    Usart_SendString(USART3, (unsigned char *)Sys_keydata.cloud_server_add, strlen((char *)(Sys_keydata.cloud_server_add)));//调试输出// by hx 2017-06-15
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
//    //    Usart_SendString(USART3, (unsigned char *)Sys_keydata.cloud_server_add, strlen((char *)(Sys_keydata.cloud_server_add)));//调试输出// by hx 2017-06-15
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
//    Pressure_Send(&struPressureDataOriginal);  //真实发送数据。
//    if (!MC8618_SendCmd_with_length((char *)(str),//模拟发送，实际没有发送任何的数据，只是用于接收。
//                                    "OK",
//                                    2)) 
//    {
//      PrintString_("updata send Succsss!");//发送成功。
//      strPressureControl.updataStatus = 1; //继续从FIFO获取数据。
//      struPressureDataOriginal.len = 0;      //清空数据长度。
////      Pressure_SendBufClear(&struPressureDataOriginal,CONST_QUEUE_SENDBUFF_LEN,struPressureDataOriginal.len);//清除FLASH中的发送缓冲
//      Pressure_SendBufClear(&struPressureDataOriginal);
//      
//    }
//    else  //发送失败
//    {
//      PrintString_("updata send fail!"); 
//      strPressureControl.updataStatus = 2; //进入数据重发过程
//      //                  strPressureControl.SendRepeatedlyFlag = 1; //重新开始标志。
//    }
//    return 1;
//    break;  
//  case 4://退出发送数据串状态
//    strPressureControl.updataStatus = 0; //默认为发送缓存状态。
//    strPressureControl.failCnt =0; //清除失败计数器
//    uniAlarm.AlarmAllData = 0X00; //清除所有的告警
//    return 0;
//  default:
//    break;
//    break;  
//  }
  return 1;
}

/***************************************************************************************
**函数名:       PressureKeyUpdataIntterSetFlag()
**功能描述:     按键中断服务程序。
**输入参数:     --
**输出参数:      
**备注:         YY.c,2017-11-16,By Haixing.Huang
****************************************************************************************/
void PressureKeyUpdataIntterSetFlag()
{
  strPressureKeyUpdata.haveKey = TRUE;
}
/***************************************************************************************
**函数名:       PressureKeyUpdataConfirm()
**功能描述:     按键中断服务程序。
**输入参数:     --
**输出参数:      
**备注:         YY.c,2017-11-16,By Haixing.Huang
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
//        debug_send("按键触发上报数据!");
//        uniAlarm.struAlarmBit.KeyUpdata = 1; 
//        ENTER_SENDALARM_STATUS; //进入发射流程; 自动打包。
      }
    }
  }
  return 0;
}


