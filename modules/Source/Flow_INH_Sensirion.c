//-----------------------------------------------------------------------------|
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI ASHTIYANI
// VER 1.0
// This File contains the Set up the flow sensor SENSIRION company which contains :
// __the coefficients read from memory with I2C communication.
// __reading the analog value is output.
//-----------------------------------------------------------------------------|
#ifdef INH_FLOW_SENSIRION
//
#include "Flow_Sensirion_INH.h"
#include "Flow_TSI_1.h"
//--------------------------------------
extern          STRUCT_DATA_A2DSENSORS                  DATA_Sensors;
extern 	        Struct_ADCExt_Data                      VA2dExt_Data; 

//--------------------------------------
extern          u16  PWM_Table ,target_output;
//--------------------------
Struct_FlowSensor_EEPROM_Table      TSI_FlowROM[AIRO2];
//--------------------------------------
UNION_Sensirion_ROM_data            Sensirion_FlowROM[AIRO2];    
//-------------------------------------------------------------------------
u16     TM_I2C_Timeout;

u8      ADD_EErom = 0, BYTE_rom = 0, index1 = 0 , En_ADD = 0;
//----------------------------------------------------
/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0
#ifndef I2C_TIMEOUT
#define I2C_TIMEOUT          30000
#endif

//---
I2C_StatusTypeDefINH    I2C_Receive_INH(I2C_TypeDef* I2Cx, u8 address,u8* data, u16 count);
I2C_StatusTypeDefINH    I2C_Transmit_INH(I2C_TypeDef* I2Cx ,u8 address ,u8 data);

///---
//----------------------------------------------------------------------------------
u16 	Vf[2], Vt[2];
float fVf[2], fVt[2];

u16     Fo[2];
u32     Kf[2];


//---------------------------------------------------------------------------------
void FoKf_Calculate (u8 TypeAirO2)
{
  Fo[TypeAirO2] = ((u32) Sensirion_FlowROM[TypeAirO2].Data.Offset * A2DEXT_MAXVAL) / ((u16) A2DEXT_VREFv * Sensirion_FlowROM[TypeAirO2].Data.B);
  Kf[TypeAirO2] = (((u32)A2DEXT_MAXVAL*A2DEXT_MAXVAL) / ((u32)A2DEXT_VREFv*A2DEXT_VREFv)) / (((u16)Sensirion_FlowROM[TypeAirO2].Data.A) / Sensirion_FlowROM[TypeAirO2].Data.B);
}
//---------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
void Flow_INH_ReadCoeff (I2C_TypeDef* I2Cx)
{
  u32       Sn = 0,Dl = 0;
  u8	  	ReadData[SENSIRION_BUFFREADING_ROM_LEN] = {0};
  u16       ADD_buff = 0;
  u8		TypeAirO2 = 0,ADD_Rom = 0,RomCNT = 0,SensorType = 0,CNTErr=0;
  I2C_StatusTypeDefINH   StatusFu = I2C__Emty;
  //----
  
  if(I2Cx == FLOWSENSOR_AIR )
    TypeAirO2 = AIR;
  else
    TypeAirO2 = O2;
  
  //---
  do
  {
    StatusFu = I2C_Transmit_INH( I2Cx, 0xA0, 0 );
    if(StatusFu != I2C__OK)
    {
      for(Dl=0;Dl<50000;Dl++){};
      StatusFu = I2C_Transmit_INH( I2Cx, 0xA0, 0 );
    }
    
    StatusFu = I2C_Receive_INH ( I2Cx, 0xA1, ReadData, SENSIRION_BUFFREADING_ROM_LEN); 
    SensorType = INH_SensorDetection(ReadData);
    CNTErr++;
  }while((SensorType == INHSENSOR_ERROR)&&(CNTErr < 5));
  //-----------------------
  if(SensorType == SENSIRION_INHSENSOR)
  {
    
    Sensirion_FlowROM[TypeAirO2].BYTE[ADD_Rom] = ReadData[2];
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[6];
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[5];
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[4];
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[3];
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[7];
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[10] - 0x30;// serial num
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[11] - 0x30;        
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[12] - 0x30;
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[13] - 0x30;        
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[14] - 0x30;
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[15] - 0x30;       
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[16] - 0x30;
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[17] - 0x30;
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[257];//Alpha
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[256];
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[259];//Beta
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[258];
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[261];//Gama
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[260];
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[263];//Size of
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[262];
    
    ADD_buff = 263;  // 0xA107
    for(RomCNT =0; RomCNT<40; RomCNT++)
    {
      ADD_buff += 2;
      Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[ADD_buff];
      Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[ADD_buff - 1];
      ADD_buff += 2;
      Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[ADD_buff];
      Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[ADD_buff - 1];
    }
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[425];//B
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[424];
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[427];//A
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[426];
    
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[429];//Offset
    Sensirion_FlowROM[TypeAirO2].BYTE[++ADD_Rom] = ReadData[428];
    //-----------------
    Sn  = ((u32)ReadData[17]-0x30);
    Sn += ((u32)ReadData[16]-0x30)*10;
    Sn += ((u32)ReadData[15]-0x30)*100;
    Sn += ((u32)ReadData[14]-0x30)*1000;
    Sn += ((u32)ReadData[13]-0x30)*10000;
    Sn += ((u32)ReadData[12]-0x30)*100000;
    Sn += ((u32)ReadData[11]-0x30)*1000000;
    Sn += ((u32)ReadData[10]-0x30)*10000000; 
    
    if(I2Cx == FLOWSENSOR_AIR )
    {
      DATA_Sensors.Air_TSISN[0] = (u8) (Sn & 0xFF);
      DATA_Sensors.Air_TSISN[1] = (u8) ((Sn >> 8) & 0xFF);
      DATA_Sensors.Air_TSISN[2] = (u8) ((Sn >> 16) & 0xFF);
      DATA_Sensors.Air_TSISN[3] = (u8) ((Sn >> 24) & 0xFF);
      DATA_Sensors.Air_TSISN[4] = 0;
      DATA_Sensors.Air_TSISN[5] = 0;
    }
    else if(I2Cx == FLOWSENSOR_O2 )
    { 
      DATA_Sensors.O2_TSISN[0] = (u8) (Sn & 0xFF);
      DATA_Sensors.O2_TSISN[1] = (u8) ((Sn >> 8) & 0xFF);
      DATA_Sensors.O2_TSISN[2] = (u8) ((Sn >> 16) & 0xFF);
      DATA_Sensors.O2_TSISN[3] = (u8) ((Sn >> 24) & 0xFF);
      DATA_Sensors.O2_TSISN[4] = 0;
      DATA_Sensors.O2_TSISN[5] = 0;
    }
    
    //------------------------------------
    /* 
    Q = (Fv - Fov)^2 * K2
    Fv = Fadc * K1  ;  K1 = (VREFv / MAXVAL)  ;  Fov = (ROM_offset / ROM_B)  ;  K2 = (ROM_A / ROM_B)
    Q = [K1 * (Fadc - Fo)]^2 * K2
    Q = (Fadc - Fo)^2 / Kf
    Fo = Fov / K1 = (ROM_offset * MAXVAL) / (VREFv * ROM_B)
    Kf = 1 / (K1^2 * K2) = (MAXVAL^2 / VREFv^2) / (ROM_A / ROM_B)
    */
    FoKf_Calculate(TypeAirO2);
  }
  else
  {
    if(TypeAirO2 == AIR)
    {
      DATA_Sensors.HWErr.TSIROMAIR = TRUE;
    }
    if(TypeAirO2 == O2)
    {
      DATA_Sensors.HWErr.TSIROMO2 = TRUE;
    }
    //Err
  }
  
}

//---------------------------------------------
u8  INH_SensorDetection(u8* ReadDatA)
{
  u8    INH_TypeS = INHSENSOR_ERROR;
  union MD_
  {
    u8  DataROM[4];
    u32 AllDataROM;
  };
  union MD_ MD_Sensor;
  MD_Sensor.AllDataROM = 0;
  //------
  MD_Sensor.DataROM[0] = ReadDatA[6];// 
  MD_Sensor.DataROM[1] = ReadDatA[5];        
  MD_Sensor.DataROM[2] = ReadDatA[4];
  MD_Sensor.DataROM[3] = ReadDatA[3];          
  if(MD_Sensor.AllDataROM == 101290)
    INH_TypeS = SENSIRION_INHSENSOR;  //Sensirion 
  //------
  return INH_TypeS;
}

//---------------------------------------------------------------------------------------

const u16 TempTable_Sensirion3100[90] = {
  /*-20 ~ -11 */  23050,22940,22830,22720,22600,22480,22350,22220,22080,21930,
  /*-10 ~  -1 */  21790,21630,21470,21310,21140,20970,20790,20610,20420,20220,
  /*  0 ~   9 */  20020,19820,19610,19400,19180,18960,18730,18500,18260,18030,
  /* 10 ~  19 */  17780,17540,17290,17040,16780,16520,16260,16000,15740,15470,
  /* 20 ~  29 */  15200,14930,14670,14400,14130,13860,13590,13320,13050,12780,
  /* 30 ~  39 */  12520,12250,11990,11730,11470,11220,10960,10710,10460,10220,
  /* 40 ~  49 */   9980, 9740, 9500, 9270, 9040, 8820, 8590, 8380, 8160, 7950,
  /* 50 ~  59 */   7750, 7550, 7350, 7160, 6970, 6780, 6600, 6430, 6250, 6080,
  /* 60 ~  69 */   5920, 5760, 5600, 5450, 5300, 5150, 5010, 4870, 4740, 4610
}; // VOLT * 10000 

/*************************** flow Service *********************************/
// Calculate teprature * 10
s16 temp_Convert_Sensirion3100(u16 Val)
{
  u8          i, k;
  u16         Vt;
  s16         Temp;
  
  Vt = (u16) (((u32) Val * A2DEXT_VREFmv * 10) / A2DEXT_MAXVAL);  //mV * 10
  
  if(Vt <= TempTable_Sensirion3100[89])
  {
    Temp = 700;  // +70 degree
  }
  else if(Vt >= TempTable_Sensirion3100[0])
  {
    Temp = -200; // -20 degree
  }
  else
  {
    i = 1;
    while (Vt < TempTable_Sensirion3100[i])
      i++;
    i--;
    k = ((TempTable_Sensirion3100[i] - Vt) * 10) / (TempTable_Sensirion3100[i] - TempTable_Sensirion3100[i+1]); // rational part
    if(k > 9)
      k = 9;
    Temp = ((s16)i - 20) * 10 + k;
  }
  //--------------------------------------
  return Temp;
}
//----------------------------------------------------------------------------------

//--------------------------------------------
void Flow_INH_Service(u8 type)
{
  if(type == AIR)
  {
    Vf[AIR] = VA2dExt_Data.AirVf;
    Vt[AIR] = VA2dExt_Data.AirVt;
    //
    DATA_Sensors.Flow_AIR = flow_calculation_Sensirion(AIR);
  }
  else if(type == O2)
  {
    Vf[O2]  = VA2dExt_Data.O2Vf;
    Vt[O2]  = VA2dExt_Data.O2Vt;
    DATA_Sensors.Flow_O2 = flow_calculation_Sensirion(O2);
    //------
  }
}

/*************************** flow Service Sensirion *********************************/
u16 flow_calculation_Sensirion (u8 type)  // *100
{
  float K,PressAmIn; 
  s32   Qs;
  u8    sizebuf, di;
  
  if(type == AIR) 
    DATA_Sensors.Temp_Gas_Air = (s16)temp_Convert_Sensirion3100(Vt[type]);
  else if(type == O2)
    DATA_Sensors.Temp_Gas_O2  = (s16)temp_Convert_Sensirion3100(Vt[type]);
  
  
  Qs = 0;
  if(Vf[type] > Fo[type])  //  Qs = (Vf - Fo)^2 / (Kf/100)
  {
    Qs = (u32)Vf[type] - Fo[type];
    Qs *= Qs;
    Qs /= (Kf[type]/100);
  }
  
  if( type == O2)
  {
    sizebuf =(u8) Sensirion_FlowROM[type].Data.SizeOf;
    if(sizebuf > 40)
      sizebuf = 40;
    di = 1;
    while((Qs >= Sensirion_FlowROM[type].Data.Q_F_o2[di][0]) && (di < sizebuf))
      di++;      
    if((Qs - Sensirion_FlowROM[type].Data.Q_F_o2[di-1][0]) < (Sensirion_FlowROM[type].Data.Q_F_o2[di][0] - Qs))
      di--;
    Qs = (Qs * (((s32)Sensirion_FlowROM[type].Data.Beta) - Sensirion_FlowROM[type].Data.Q_F_o2[di][1])) / Sensirion_FlowROM[type].Data.Beta;
  }
  //----Step 8	
  
  PressAmIn = ((float)DATA_Sensors.A2D_AmbientPressure / 10) + ((float)DATA_Sensors.Press_INH / 100 );
  K = ( PFixCOEFFICIENT1 / (PressAmIn - 64.0) ) * PFixCOEFFICIENT2;
  
  Qs = (s16)((float)Qs * K);  //  convert STPD TO BTPS
  if(Qs < 0)
    Qs = 0;
  
  return (u16)Qs;
}
//-------------
/*************************** flow Service TSI *********************************/

//-------------------------------------------------------------------------------


/*************************** temp Convert **************************************/
u32 I2C__Timeout;
//----------------------------------------------------
I2C_StatusTypeDefINH    I2C_Receive_INH(I2C_TypeDef* I2Cx, u8 address,u8* data, u16 count) 
{
  I2C__Timeout = I2C_TIMEOUT;
  /* Wait until BUSY flag is reset */
  while(I2Cx->SR2 & I2C_SR2_BUSY)
  {
    if(--I2C__Timeout == 0x00)
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C__TIMEout;  
    }
  }
  //--------------------
  /* Enable Acknowledge */
  I2Cx->CR1 |= I2C_CR1_ACK;   
  //------------
  /* Generate Start */
  I2Cx->CR1 |= I2C_CR1_START; 
  /* Wait until SB flag is set */
  I2C__Timeout = I2C_TIMEOUT;
  while(!(I2Cx->SR1 & I2C_SR1_SB)) 
  {
    if (--I2C__Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C__TIMEout;
    }      
  }
  ///-------- 
  /* Send address with zero last bit */
  I2Cx->DR = address;   
  /* Wait till finished */
  while (!(I2Cx->SR1 & I2C_SR1_ADDR)) 
  {
    if (--I2C__Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C__TIMEout;
    } 
  }
  /* Read status register to clear ADDR flag */
  I2Cx->SR1;
  I2Cx->SR2;
  //----------------------
  
  
  for(;count>0;count--)
  {
    if(count == 1)//end data
    {
      I2Cx->CR1 &= ~I2C_CR1_ACK;
    }
    
    /* Wait till not received */
    I2C__Timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) 
    {
      if (--I2C__Timeout == 0x00) 
      {
        /* Generate Stop */
        I2Cx->CR1 |= I2C_CR1_STOP;
        return I2C__TIMEout;
      }
    }    
    /* Read data */
    *data = I2Cx->DR;  
    data++;
  }
  /* Generate Stop */
  I2Cx->CR1 |= I2C_CR1_STOP;
  /* Read status register to clear ADDR flag */
  I2Cx->SR1;
  I2Cx->SR2;
  //-----------------
  return I2C__OK;  
}

//--------------------------------------------
I2C_StatusTypeDefINH    I2C_Transmit_INH(I2C_TypeDef* I2Cx ,u8 address ,u8 data) 
{
  I2C__Timeout = I2C_TIMEOUT;
  /* Wait until BUSY flag is reset */
  while(I2Cx->SR2 & I2C_SR2_BUSY)
  {
    if(--I2C__Timeout == 0x00)
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C__TIMEout;  
    }
  }
  //--------------------
  /* Generate Start */
  I2Cx->CR1 |= I2C_CR1_START; 
  /* Wait until SB flag is set */
  I2C__Timeout = I2C_TIMEOUT;
  while(!(I2Cx->SR1 & I2C_SR1_SB)) 
  {
    if (--I2C__Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C__TIMEout;
    }      
  }
  //--------------------
  I2C__Timeout = I2C_TIMEOUT;
  /* Send address with zero last bit */
  I2Cx->DR = address;   // address & write Bit
  /* Wait till finished */
  while (!(I2Cx->SR1 & I2C_SR1_ADDR)) 
  {
    if (--I2C__Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C__TIMEout;
    } 
  }
  /* Read status register to clear ADDR flag */
  I2Cx->SR1;
  I2Cx->SR2;
  //----------------------
  /* Send data */
  
  /* Wait until TXE flag is set */
  I2C__Timeout = I2C_TIMEOUT;
  while (!(I2Cx->SR1 & I2C_SR1_TXE)) 
  {
    if (--I2C__Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C__TIMEout;
    }
  }    
  /* Send I2C data */
  I2Cx->DR = data;
  //  /* Wait until BTF flag is set */
  //  I2C_Timeout = I2C_TIMEOUT;
  //  while (!(I2Cx->SR1 & I2C_SR1_BTF)) 
  //  {
  //    if (--I2C_Timeout == 0x00) 
  //    {
  //      /* Generate Stop */
  //      I2Cx->CR1 |= I2C_CR1_STOP;
  //      return I2C_TIMEout;
  //    }
  //  }    
  
  //-------------- 
  /* Generate Stop */
  I2Cx->CR1 |= I2C_CR1_STOP;
  return I2C__OK;
}
//
#endif /*INH_FLOW_SENSIRION*/
/*************************** temp Convert **************************************/



