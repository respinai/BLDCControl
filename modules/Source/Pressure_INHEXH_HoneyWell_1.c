//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
// 
/*
|------------------------------------------------------------------------------|
|     		***		Include		***														   |
|------------------------------------------------------------------------------|
*/
#include "Pressure_INHEXH_HoneyWell.h"
#include "main.h"
/*
|------------------------------------------------------------------------------|
|			***		Defines		***														   |
|------------------------------------------------------------------------------|
*/
extern  STRUCT_DATA_A2DSENSORS          DATA_Sensors;

/*
|------------------------------------------------------------------------------|
|                 ***		Fanctions 		***												   |
|------------------------------------------------------------------------------|
*/
//-----------------------------------------------------------------------------|
void Pressure_AllSensors_INITIALIZE(void)
{
  return;
}
//-----------------------------------------------------------------------------|
void Read_Press_INH_EXH(void) /* HoneyWell */ //  Call Every 2.5 ms
{
  //static    u8    State_Process = 0;
  float     Temp_Press;
  //...................
  
  Temp_Press = (SPI_I2S_ReceiveData(PRESS_INH) & 0x3FFF); //=> Delet Status Bits [31:30]
  Temp_Press = (((Temp_Press - P_INHEXH_OUTPUTMIN)*(INHEXH_Pressure_MAX - INHEXH_Pressure_MIN))/(P_INHEXH_OUTPUTMAX - P_INHEXH_OUTPUTMIN)) + INHEXH_Pressure_MIN ;
  /*  Temp_Press = ((   Pout    - OSdig) / 2^14 ) * 150  */
  DATA_Sensors.Press_INH = (s16)(Temp_Press * 100 * 1.019716); /* 100=>2Decimal Place & 1.0197162 =>>Convert mbar TO cmH2O */     
  GPIO_SetBits(PRESSINHCS_PORT , PRESSINHCS_PIN);        // CS High
  //---------------------------------------------------
  SPI_I2S_ITConfig(PRESS_INH , SPI_IT_RXNE , DISABLE );
  
//    
//  switch(State_Process)
//  {
//   case 0://=====>> Start Read Pressure INH
//    
//    State_Process = 1; // Goto Calculate Pressure INH
//    break; 
//   case 1 ://=====>> Calculate Pressure INH
//   
//    State_Process = 0; // Return to Start Process 
//    break; 
//  }
}
/*
|------------------------------------------------------------------------------|
|                             ***         ***															   |
|------------------------------------------------------------------------------|
*/

