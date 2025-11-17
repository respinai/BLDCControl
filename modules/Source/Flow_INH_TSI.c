//-----------------------------------------------------------------------------|
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI ASHTIYANI
// VER 1.0
// This File contains the Set up the flow sensor SENSIRION company which contains :
// __the coefficients read from memory with I2C communication.
// __reading the analog value is output.
//-----------------------------------------------------------------------------|
#ifdef INH_FLOW_TSI
//-------------------
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

//---------------------------------------------------------------------------------
const u8 Flow_E2Prom_ADD[Flow_E2Prom_ADD_Len]=
{
    /*Address ,   Bytes	      */
    0		,	2,	//    CRC
    2		,	6,	//	SN
    8		,	4,	//	MN
    12	        ,	1,	//	Rev
    14	        ,	2,	//	Year
    16	        ,	1,	//	Month
    18	        ,	1,	//	Day
    20	        ,	4,	//	Tcal
    24	        ,	4,	//	S
    28	        ,	4,	//	Z	
    32	        ,	4,	//	S2
    36	        ,	4,	//	Z2
    40	        ,	4,	//	Tcorr
    
    44	        ,	1,	//	Cal Air
    46	        ,	2,	//	Location Air
    48	        ,	1,	//	Cal Oxygen (2A)
    50          ,	2,	//	Location	Oxygen
    52	        ,	1,	//	Cal	Heliox
    54	        ,	2,	//	Location Heliox
    56	        ,	1,	//	Cal Gas4	
    58	        ,	2,	//	Location	Gac4
    
    0		,	1,	//	offset (33)
    2		,	4,	//	K0			
    6		,	4,	//	K1			
    10	        ,	4,	//	K2			
    14	        ,	4,	//	K3
    18	        ,	4,	//	L0			
    22	        ,	4,	//	L1			
    26	        ,	4,	//	L2			
    30	        ,	4,	//	L3			
    
    34	        ,	16,	//	VABC[0]
    50	        ,	16,	//	VABC[1]
    66	        ,	16,	//	VABC[2]
    82	        ,	16,	//	VABC[3]
    98	        ,	16,	//	VABC[4]
    114	        ,	16,	//	VABC[5]
    130	        ,	16,	//	VABC[6]
    146	        ,	16,	//	VABC[7]
    162	        ,	16,	//	VABC[8]
    178	        ,	16,	//	VABC[9]
    194	        ,	16,	//	VABC[10]
    210	        ,	16,	//	VABC[11]
    226	        ,	16,	//	VABC[12]
};
//-------------------------------------------------------------------------------
const u16 TempTable[90] = {
    /*-20 ~ -11 */  22923,22804,22681,22552,22417,22277,22131,21979,21821,21658,
    /*-10 ~  -1 */  21488,21312,21130,20942,20748,20548,20342,20129,19911,19687,
    /*  0 ~   9 */  19457,19222,18981,18734,18483,18226,17934,17700,17430,17156,
    /* 10 ~  19 */  16879,16598,16315,16028,15739,15448,15156,14861,14566,14270,
    /* 20 ~  29 */  13974,13678,13382,13087,12793,12500,12209,11920,11633,11348,
    /* 30 ~  39 */  11066,10788,10512,10240, 9972, 9708, 9447, 9191, 8939, 8692,
    /* 40 ~  49 */   8449, 8211, 7977, 7749, 7525, 7306, 7092, 6883, 6679, 6480,
    /* 50 ~  59 */   6286, 6097, 5913, 5733, 5558, 5388, 5223, 5062, 4906, 4754,
    /* 60 ~  69 */   4607, 4464, 4325, 4190, 4060, 3933, 3810, 3691, 3576, 3464
}; // VOLT * 10000 

///F= -8.0295*X^5 + 53.128*X^4 - 144.94*X^3 + 205.1*X^2 - 185.02*X+ 113.71
//----------------------------------------------------------------------------------
//--------------------------------------------
void Flow_INH_Service(u8 type)
{
    if(type == AIR)
    {
        Vf[AIR] = VA2dExt_Data.AirVf;
        Vt[AIR] = VA2dExt_Data.AirVt;
        fVf[AIR] = ((float) (((u32) (VA2dExt_Data.AirVf * A2DEXT_VREFmv)) / A2DEXT_MAXVAL)) / 1000;//Convert mVolt TO Volt
        fVt[AIR] = ((float) (((u32) (VA2dExt_Data.AirVt * A2DEXT_VREFmv)) / A2DEXT_MAXVAL)) / 1000;
        DATA_Sensors.Flow_AIR = (u16)(flow_calculation(AIR)*100 );
        
    }
    else if(type == O2)
    {
        Vf[O2]  = VA2dExt_Data.O2Vf;
        Vt[O2]  = VA2dExt_Data.O2Vt;
        fVf[O2]  = ((float) (((u32) (VA2dExt_Data.O2Vf  * A2DEXT_VREFmv)) / A2DEXT_MAXVAL)) / 1000;
        fVt[O2]  = ((float) (((u32) (VA2dExt_Data.O2Vt  * A2DEXT_VREFmv)) / A2DEXT_MAXVAL)) / 1000;
        DATA_Sensors.Flow_O2 = (u16)(flow_calculation(O2)*100 );
    }
}
/*************************** flow Service *********************************/
float flow_calculation (u8 type)
{
    float T_temp, T_over, V_fStd,Q,K,PressAmIn; 
    u8 i = 0;
    
    //----Step 4
    T_temp = TSI_FlowROM[type].Data.Tcorr + temp_Convert(Vt[type]);
    if(type == AIR) 
       DATA_Sensors.Temp_Gas_Air = (s16)(T_temp * 10);
    else if(type == O2)
    DATA_Sensors.Temp_Gas_O2 = (s16)(T_temp * 10);
    //----Step 5	
    if(T_temp >= TEMP_REF)// Use K coefficient if T >= 21.11
    {
        T_over = (TSI_FlowROM[type].Data.K0 +(TSI_FlowROM[type].Data.K1 * T_temp)+(TSI_FlowROM[type].Data.K2 * (float)(pow(T_temp,2)))+(TSI_FlowROM[type].Data.K3 * (float)(pow(T_temp,3))));
    }
    else // Use L coefficient
    {
        T_over = (TSI_FlowROM[type].Data.L0 +(TSI_FlowROM[type].Data.L1 * T_temp)+(TSI_FlowROM[type].Data.L2 * (float)(pow(T_temp,2)))+(TSI_FlowROM[type].Data.L3 * (float)(pow(T_temp,3))));
    }
    //----Step 6
    V_fStd = ((fVf[type] + TSI_FlowROM[type].Data.Z)*((float)(sqrt((T_over - TEMP_REF)/(T_over - T_temp))))) - TSI_FlowROM[type].Data.Z;
    //----Step 7	
    while((V_fStd > TSI_FlowROM[type].Data.Coeff[i].V)&&(i < TSI_FlowROM[type].Data.CoeffNUM ))
    {
        i++;
    }
    if(i)
        i --;	
    //----Step 8	
    Q = TSI_FlowROM[type].Data.Coeff[i].A +(TSI_FlowROM[type].Data.Coeff[i].B * (float)(pow(V_fStd , 2)))+(TSI_FlowROM[type].Data.Coeff[i].C * (float)(pow(V_fStd , 5)));
    
    PressAmIn = ((float)DATA_Sensors.A2D_AmbientPressure / 10) + ((float)DATA_Sensors.Press_INH / 100 );
    K = ( PFixCOEFFICIENT1 / (PressAmIn - 64.0) ) * PFixCOEFFICIENT2;

    Q *= K;  //  convert STPD TO BTPS
    if(Q < 0)
        Q = 0;
    return Q ;
}
//-------------------------------------------------------------------------------
float temp_Convert(u16 Val)
{
    s8          i;
    u16         Vt,tmp;
    float       Temp;
    static  u8  TempTableState;
    //	// -8.0295*F49^5 + 53.128*F49^4 - 144.94*F49^3 + 205.1*F49^2 - 185.02*F49+ 113.71
    //	Temp = ((-8.0295 * pow(Vt,5))+(53.128*pow(Vt,4))-(144.94*pow(Vt,3))+(205.1*pow(Vt,2))-(185.02* Vt)+(113.71));
    Vt = (u16) (((u32) Val * A2DEXT_VREFmv * 10) / A2DEXT_MAXVAL);
    //                 28753
    if(Vt <= TempTable[89])
    {
        Temp = 70.00;
    }
    else if(Vt < TempTable[0]) // 22923,3464
    {
        TempTableState = (u8)(Vt / 1000);
        switch(TempTableState)
        {
        case 22:
            i = -20;
            break;
        case 21:
            i = -13;
            break;
        case 20:
            i = -7;
            break;
        case 19:
            i = -2;
            break;
        case 18:
            i = 2;
            break;
        case 17:
            i = 6;
            break;
        case 16:
            i = 10;
            break;
        case 15:
            i = 14;
            break;
        case 14:
            i = 17;
            break;
        case 13:
            i = 20;
            break;
        case 12:
            i = 24;
            break;
        case 11:
            i = 27;
            break;
        case 10:
            i = 31;
            break;
        case 9:
            i = 34;
            break;
        case 8:
            i = 38;
            break;
        case 7:      
            i = 42;
            break;
        case 6:
            i = 47;
            break;
        case 5:
            i = 52;
            break;
        case 4:
            i = 58;
            break;
        case 3:
            i = 65;
            break;      
        }
        //
        while ((i < 70) & (Vt < TempTable[i+20]))  // i will be -20~70
        {
            i++;
        }   
        i --;  // i is the integer part of TEMP 			
        tmp = ((TempTable[i+20] - Vt)*100) / (TempTable[i+20] - TempTable[i+21]); // rational part 
        Temp = (float) tmp / 100;
        Temp += i;	
    }
    //--------------------------------------
    else
    {
        Temp = -20.00;
    }
    return Temp;
}
//----------------------------------------------------------------------------------
void Flow_INH_ReadCoeff (I2C_TypeDef* I2Cx)
{
  u32       Dl = 0;
  u8	  	ReadData[SENSIRION_BUFFREADING_ROM_LEN] = {0};
  
  u8		TypeAirO2 = 0,SensorType = 0,CNTErr=0;
  //--
  u16       Index_CNT = 0;
  u16       TmepData = 0;
  u8		*ptr_eeprom;
  u16       Len_ReadBuff = SENSIRION_BUFFREADING_ROM_LEN;
  I2C_StatusTypeDefINH   StatusFu = I2C__Emty;
  //----
  
  if(I2Cx == FLOWSENSOR_AIR )
    TypeAirO2 = AIR;
  else
    TypeAirO2 = O2;
  
  ptr_eeprom = (u8*)&TSI_FlowROM[TypeAirO2].Data;
  
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
  if(SensorType == TSI_INHSENSOR)
  {
    while(Len_ReadBuff)  
    {
      TmepData = ReadData[Index_CNT++];
      /* Calculation CRC */			
      
      if(((Flow_E2Prom_ADD[TSI_FlowROM[TypeAirO2].Control.Index ] + TSI_FlowROM[TypeAirO2].Control.Offset) == TSI_FlowROM[TypeAirO2].Control.cnt) || (TSI_FlowROM[TypeAirO2].Control.Flg_loop == TRUE))
      {
        if(!TSI_FlowROM[TypeAirO2].Control.Flg_loop)
        {
          TSI_FlowROM[TypeAirO2].Control.Flg_loop = TRUE;
          TSI_FlowROM[TypeAirO2].Control.Index++;
          TSI_FlowROM[TypeAirO2].Control.Len = Flow_E2Prom_ADD[TSI_FlowROM[TypeAirO2].Control.Index];
          TSI_FlowROM[TypeAirO2].Control.Len_cpy = TSI_FlowROM[TypeAirO2].Control.Len;
        }
        *(ptr_eeprom + TSI_FlowROM[TypeAirO2].Control.Len - 1) = TmepData;
        //CRC_Calculation(TmepData);
        TSI_FlowROM[TypeAirO2].Control.Len--;
        if(TSI_FlowROM[TypeAirO2].Control.Len == 0)
        {
          TSI_FlowROM[TypeAirO2].Control.Flg_loop =FALSE;
          TSI_FlowROM[TypeAirO2].Control.Index++;
          ptr_eeprom += TSI_FlowROM[TypeAirO2].Control.Len_cpy;
        }
      }	
      TSI_FlowROM[TypeAirO2].Control.cnt ++;
      if(TSI_FlowROM[TypeAirO2].Control.cnt == CHECK_ADD)
      {
        if(TSI_FlowROM[TypeAirO2].Data.CalAIR == 0x01)
        {
          TSI_FlowROM[TypeAirO2].Control.Offset = TSI_FlowROM[TypeAirO2].Data.LocationAIR;
        }
        else if(TSI_FlowROM[TypeAirO2].Data.CalO2 == 0x01)
        {
          TSI_FlowROM[TypeAirO2].Control.Offset = TSI_FlowROM[TypeAirO2].Data.LocationO2;
        }
        else if(TSI_FlowROM[TypeAirO2].Data.CalHLX == 0x01)
        {
          TSI_FlowROM[TypeAirO2].Control.Offset = TSI_FlowROM[TypeAirO2].Data.LocationHLX;
        }
        else if(TSI_FlowROM[TypeAirO2].Data.CalGAS == 0x01)
        {
          TSI_FlowROM[TypeAirO2].Control.Offset = TSI_FlowROM[TypeAirO2].Data.LocationGAS;
        }
        else
        {
          // Error & SET  coefficiont
        }
      }    
      
      Len_ReadBuff--; //  
      
    } // While
    
    if( TypeAirO2 == AIR)
    {
        DATA_Sensors.Air_TSISN[0]=TSI_FlowROM[AIR].Data.SN[0];
        DATA_Sensors.Air_TSISN[1]=TSI_FlowROM[AIR].Data.SN[1];
        DATA_Sensors.Air_TSISN[2]=TSI_FlowROM[AIR].Data.SN[2];
        DATA_Sensors.Air_TSISN[3]=TSI_FlowROM[AIR].Data.SN[3];
        DATA_Sensors.Air_TSISN[4]=TSI_FlowROM[AIR].Data.SN[4];
        DATA_Sensors.Air_TSISN[5]=TSI_FlowROM[AIR].Data.SN[5]; 
        
        if(TSI_FlowROM[TypeAirO2].Data.MN == 840521)
          DATA_Sensors.HWErr.TSIASSEMAIR = FALSE; 
        else
          DATA_Sensors.HWErr.TSIASSEMAIR = TRUE; 
    }
    else if( TypeAirO2 == O2)
    {
        DATA_Sensors.O2_TSISN[0]=TSI_FlowROM[O2].Data.SN[0];
        DATA_Sensors.O2_TSISN[1]=TSI_FlowROM[O2].Data.SN[1];
        DATA_Sensors.O2_TSISN[2]=TSI_FlowROM[O2].Data.SN[2];
        DATA_Sensors.O2_TSISN[3]=TSI_FlowROM[O2].Data.SN[3];
        DATA_Sensors.O2_TSISN[4]=TSI_FlowROM[O2].Data.SN[4];
        DATA_Sensors.O2_TSISN[5]=TSI_FlowROM[O2].Data.SN[5]; 
        
        if(TSI_FlowROM[TypeAirO2].Data.MN == 840522)
          DATA_Sensors.HWErr.TSIASSEMO2 = FALSE;
        else
          DATA_Sensors.HWErr.TSIASSEMO2 = TRUE; 
    }
  }
  //-------------------------------------
  else
  {
    if(TypeAirO2 == AIR)
    {
      DATA_Sensors.HWErr.TSIROMAIR = TRUE;
    }
    if( TypeAirO2 == O2)
    {
      DATA_Sensors.HWErr.TSIROMO2 = TRUE;
    }
    //Err
  }
  
}

//---------------------------------------------
u8  INH_SensorDetection(u8* ReadDatA)
{
  u8    INH_TypeT = INHSENSOR_ERROR;
  union MD_
  {
    u8  DataROM[4];
    u32 AllDataROM;
  };
  union MD_ MD_Sensor;
  //------
  MD_Sensor.DataROM[3] = ReadDatA[8];// 
  MD_Sensor.DataROM[2] = ReadDatA[9];        
  MD_Sensor.DataROM[1] = ReadDatA[10];
  MD_Sensor.DataROM[0] = ReadDatA[11];        
  
  if((MD_Sensor.AllDataROM == 840521)||(MD_Sensor.AllDataROM == 840522))
    INH_TypeT = TSI_INHSENSOR; //TSI
    //------
    return INH_TypeT;
}



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
    /* Wait until BTF flag is set */
    I2C__Timeout = I2C_TIMEOUT;
    while (!(I2Cx->SR1 & I2C_SR1_BTF)) 
    {
      if (--I2C__Timeout == 0x00) 
      {
        /* Generate Stop */
        I2Cx->CR1 |= I2C_CR1_STOP;
        return I2C__TIMEout;
      }
    }    
  
  //-------------- 
  /* Generate Stop */
  I2Cx->CR1 |= I2C_CR1_STOP;
  return I2C__OK;
}
//---------------------
#endif /* INH_FLOW_TSI */

/*************************** temp Convert **************************************/



