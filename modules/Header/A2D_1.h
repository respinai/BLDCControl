//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
//------------------------------------------------------------------------------

#ifndef _SAADAT_A2D_EXT_H
#define _SAADAT_A2D_EXT_H

#include "main.h"



#define     STARTREADING_A2D_ALLCHANNELS()          A2D_Channel = 0
#define     CHECK_A2D_READINGCHANNEL()              (A2D_Channel < ENDOFA2D_CHANNEL ?   1:0)

#define     ADC_VREF        3300
#define     ADC_RESOLUTION  4095
#define     Vcc             5000
#define     Resistance_R88  4715    
/*===========================================================================|
 *                            A2DExt Output Data Structure
 *===========================================================================|
*/


/*===========================================================================|
 *                            A2DExt Output Data Structure
 *===========================================================================|
*/

typedef struct{
  u16		Temperature;
  u16		Curr_IRS1;
  u16		Curr_Valve1;
  //u16		Curr_IRS3;
  u16		VIN;
  u16		V12;
}Struct_ADC_Data;

/* A2D Channels Names For Switch:Case */
typedef enum 
{
  READ_TEMPERATURE      =0,
  READ_CURRENT_IRS1     =1,
  READ_CURRENT_VALVE1     =2,
  //READ_CURRENT_IRS3     =3,
  READ_VIN              =3,
  READ_V12              =4,

  ENDOFA2D_CHANNEL      =5  //End
    
}SWITCHCASE_A2D_CHANNEL;




//------------------------------------------------------------------------------
void ADC_Service(void);

#endif	// _SAADAT_A2D_EXT_H
