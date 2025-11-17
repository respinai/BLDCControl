//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI ASHTIYANI 
// VER 1.0
//------------------------------------------------------------------------------

#include "A2D_1.h"


//------------------------------------------------------------------------------
u8      A2D_Channel = ENDOFA2D_CHANNEL;
Struct_ADC_Data             ADC_Data; 
u16   Res_NTC = 0.0;
//
const   u16     Temperature_NTC[151]=
{
  28217,26988,25819,24707,23650,22643,21686,20774,19905,19078,18290,17539,16822,
  16139,15488,14867,14273,13707,13167,12650,12157,11686,11236,10805,10394,10000,
  9623,9263,8918,8588,8272,7969,7679,7401,7135,6879,6634,6400,6174,5958,5751,5552,
  5361,5177,5001,4832,4669,4513,4363,4218,4079,3946,3817,3694,3575,3460,3350,3244,
  3141,3043,2948,2856,2768,2683,2601,2522,2446,2373,2302,2233,2167,2103,2042,1982,
  1925,1870,1816,1764,1714,1666,1619,1574,1530,1488,1447,1407,1369,1332,1296,1261,
  1228,1195,1163,1133,1103,1074,1047,1020,994,968,944,920,897,874,853,831,811,791,
  772,753,735,717,700,683,667,651,636,621,607,593,579,566,553,540,528,516,505,494,
  483,472,462,452,442,432,423,414,405,396,388,380,372,364,356,349,342,335,328,321,
  315,309,302
};
//------------------------------------------------------------------------------
u8 Find_R (void)
{
  u8    i=0;
  
  if(Res_NTC >= Temperature_NTC[0])
  {
    return 0;
  }
  if(Res_NTC <= Temperature_NTC[150])
  {
    return 150;
  }
  for(i=1;i<150;i++)
  {
    if(Res_NTC >= Temperature_NTC[i])
      break;
  }
  return i;
}


//void ADC_Service(void)
//{
//  switch (A2D_Channel)  
//  {
//  case READ_TEMPERATURE://0
//    ADC_Data.Temperature = ((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION);
//    Res_NTC = (u16)((float)ADC_Data.Temperature / ((float)(Vcc - ADC_Data.Temperature)/Resistance_R88));
//    ADC_Data.Temperature = Find_R();
//    ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_15Cycles);
//    ADC_SoftwareStartConv(ADC1);
//    break;/*--------->>>><<<<<---------*/
//  case READ_CURRENT_IRS1://1
//    ADC_Data.Curr_IRS1 = (int)((((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION))*1.33);
//    ADC_RegularChannelConfig(ADC1,ADC_Channel_12,1,ADC_SampleTime_15Cycles);
//    ADC_SoftwareStartConv(ADC1);
//    break;/*--------->>>><<<<<---------*/
//  case READ_CURRENT_VALVE1://2
//    ADC_Data.Curr_Valve1 = (int)((((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION))*0.2);
//    ADC_RegularChannelConfig(ADC1,ADC_Channel_13,1,ADC_SampleTime_15Cycles);
//    ADC_SoftwareStartConv(ADC1);
//    break;/*--------->>>><<<<<---------*/
//  case READ_VIN://4
//    ADC_Data.VIN = ((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION)*11;
//    ADC_RegularChannelConfig(ADC1,ADC_Channel_15,1,ADC_SampleTime_15Cycles);
//    ADC_SoftwareStartConv(ADC1);
//    break;/*--------->>>><<<<<---------*/
//  case READ_V12://5
//    ADC_Data.V12 = ((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION)*5;
//    ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_15Cycles);
//    ADC_SoftwareStartConv(ADC1);
//    Send_Data(1);
//    break;/*--------->>>><<<<<---------*/
//    
//  default:
//    A2D_Channel = ENDOFA2D_CHANNEL;
//    break;
//  }//switch	
//  A2D_Channel++;    
//}


void ADC_Service(void)
{
 switch (A2D_Channel)  
 {
 case READ_TEMPERATURE:
    ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_15Cycles);
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    ADC_Data.Temperature = ((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION);
    Res_NTC = (u16)((float)ADC_Data.Temperature / ((float)(Vcc - ADC_Data.Temperature)/Resistance_R88));
    ADC_Data.Temperature = Find_R();
    break;
    
 case READ_CURRENT_IRS1:
    ADC_RegularChannelConfig(ADC1,ADC_Channel_12,1,ADC_SampleTime_15Cycles);
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    ADC_Data.Curr_IRS1 = (int)((((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION)*133)/100);
    break;
    
 case READ_CURRENT_VALVE1:
    ADC_RegularChannelConfig(ADC1,ADC_Channel_13,1,ADC_SampleTime_15Cycles);
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    ADC_Data.Curr_Valve1 = (int)((((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION)*2)/10);
    break;
    
 case READ_VIN:
    ADC_RegularChannelConfig(ADC1,ADC_Channel_15,1,ADC_SampleTime_15Cycles);
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    ADC_Data.VIN = ((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION)*11;
    break;
    
 case READ_V12:
    ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_15Cycles);
    ADC_SoftwareStartConv(ADC1);
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    ADC_Data.V12 = ((ADC_GetConversionValue(ADC1)*ADC_VREF)/ADC_RESOLUTION)*5;
    //Send_Data(1);
    break;
    
 default:
    A2D_Channel = ENDOFA2D_CHANNEL;
    break;
 }//switch
  
 A2D_Channel++;
}




//----------------------------------------------------------------------------------
