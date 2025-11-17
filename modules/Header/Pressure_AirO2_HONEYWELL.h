//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
// 
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef _SAADAT_PRESS_HONEYWELL_H
#define _SAADAT_PRESS_HONEYWELL_H
//------------------------------------------------------------------------------
#include "main.h" 
#define     P_AIRO2_OUTPUTMAX      14745    // 90% of 2^14 counts or 0x3999 
#define     P_AIRO2_OUTPUTMIN      1638     // 10% of 2^14 counts or 0x3999
#define     AIRO2_Pressure_MAX     100      // max. value of pressure range [bar, psi, kPa, etc.] 
#define     AIRO2_Pressure_MIN     0        // min. value of pressure range [bar, psi, kPa, etc.] 


/*
|------------------------------------------------------------------------------|
|															***		Include		***														   |
|------------------------------------------------------------------------------|
*/

//------------------------------------------------------------------------------


void Read_Press_AIR_O2(void);
void Pressure_INHEXH_HoneyWell_INITIALIZE (void);
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#endif		//_SAADAT_PRESS_HONEYWELL_H
//

