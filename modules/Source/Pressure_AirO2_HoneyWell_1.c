//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
// 
/*
|------------------------------------------------------------------------------|
|			***		Include		***														   |
|------------------------------------------------------------------------------|
*/
#include "Pressure_AirO2_HONEYWELL.h"
/*
|------------------------------------------------------------------------------|
|     		***		Defines		***														   |
|------------------------------------------------------------------------------|
*/
extern  STRUCT_DATA_A2DSENSORS          DATA_Sensors;

/*
|------------------------------------------------------------------------------|
|															***		Variables 		***												   |
|------------------------------------------------------------------------------|
*/
//-----------------------------------------------------------------------------|
void Pressure_INHEXH_HoneyWell_INITIALIZE(void)
{
  return;
}
//-----------------------------------------------------------------------------|
void Read_Press_AIR_O2(void) /* HoneyWell */ //  Call Every 10 ms
{  
  static    u8    State_Process = 0;
  float     Temp_Press;
  //...................
  switch(State_Process)
  {
    case 0://=====>> Start Read Pressure AIR
    GPIO_ResetBits(PRESSAIRCS_PORT, PRESSAIRCS_PIN); // CS LOW , AIR
    SPI_I2S_ITConfig(PRESS_AIRO2 , SPI_IT_RXNE , ENABLE );
    SPI_I2S_SendData(PRESS_AIRO2,0xAA55);      // 0XAA55 is temporary data for read from SPI
    State_Process = 1; // Goto Calculate Pressure AIR
    break;
  case 1 ://=====>> Calculate Pressure AIR
    Temp_Press = (SPI_I2S_ReceiveData(PRESS_AIRO2) & 0x3FFF); //=> Delet Status Bits [31:30]
    Temp_Press = (((Temp_Press - P_AIRO2_OUTPUTMIN)*(AIRO2_Pressure_MAX - AIRO2_Pressure_MIN))/(P_AIRO2_OUTPUTMAX - P_AIRO2_OUTPUTMIN)) + AIRO2_Pressure_MIN ;
    Temp_Press=(Temp_Press<0?0:Temp_Press);
    /*  Temp_Press = ((   Pout    - OSdig) / 2^14 ) * 150  */
    DATA_Sensors.Press_AIR = (u16)(Temp_Press * 68.947572); /* 100=>2Decimal Place & 68.947572=>>Convert PSI TO mbar */ 
    GPIO_SetBits(PRESSAIRCS_PORT , PRESSAIRCS_PIN);         // CS High
    //................ End Read AIR & Start Read O2
    GPIO_ResetBits(PRESSO2CS_PORT, PRESSO2CS_PIN); // CS LOW
    SPI_I2S_SendData(PRESS_AIRO2,0xAA55);
    State_Process = 2; // Goto Calculate Pressure O2
    break;
  case 2 ://=====>> Calculate Pressure O2
    Temp_Press = (SPI_I2S_ReceiveData(PRESS_AIRO2) & 0x3FFF); //=> Delet Status Bits [31:30]
    Temp_Press = (((Temp_Press - P_AIRO2_OUTPUTMIN)*(AIRO2_Pressure_MAX - AIRO2_Pressure_MIN))/(P_AIRO2_OUTPUTMAX - P_AIRO2_OUTPUTMIN)) + AIRO2_Pressure_MIN ;
    Temp_Press=(Temp_Press<0?0:Temp_Press);
    /*  Temp_Press = ((   Pout    - OSdig) / 2^14 ) * 150  */
    DATA_Sensors.Press_O2 = (u16)(Temp_Press * 68.947572); /* 100=>2Decimal Place & 68.947572=>>Convert PSI TO mbar */  
    SPI_I2S_ITConfig(PRESS_INHEXH , SPI_IT_RXNE , DISABLE );
    GPIO_SetBits(PRESSO2CS_PORT, PRESSO2CS_PIN);         // CS High
    State_Process = 0; // Return to Start Process 
 
    break;
  }
}
/*
|------------------------------------------------------------------------------|
|                             ***         ***															   |
|------------------------------------------------------------------------------|
*/



