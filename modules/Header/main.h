//******************************************************************************
//   This file contains the general headers for all files.
//******************************************************************************
#ifndef __MAIN_H
#define __MAIN_H
//------------------------------------------------------------------------------
/*/ GENERAL INCLUDES */
//-----------------------------|
#include "math.h"                   // flow.C
//-----------------------------|
#include "misc.h"                   // init.C
#include "stm32f4xx.h"              // 
#include "stm32f4xx_gpio.h"         
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_iwdg.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_i2c.h"


//-----------------------------|
#include "Init_Def_2.h"
#include "A2D_1.h"
#include "Timer_1.h"
#include "Usart_3.h"
//-----------------------------|
#define KR_KEY_RELOAD    ((uint16_t)0xAAAA)
/*================================================================*/
/* Actuator Names  */

//==================================================
//#define         CALCULATE_FREE_TIME(T1,T2)              ((u16)((T2+0x10000) - T1))  // Output is uSec 
#define         FREE_CNT_100us                          TIM6->CNT     // Timer6 is Free Counter
#define         FREE_CNT_6ns                            TIM9->CNT     // Timer9 is Free Counter
//================================================================================

//==========================================================================
#pragma pack(push,1)

typedef struct{
   u8  Hall_1          :1;     // BIT 0
   u8  Hall_2          :1;     // BIT 1     
   u8  Hall_3          :1;     // BIT 2     
   u8  Packet_03       :1;     // BIT 3     
   u8  Packet_04       :1;     // BIT 4     
   u8  Packet_05       :1;     // BIT 5     
   u8  Packet_06       :1;     // BIT 6 
   u8  Packet_07       :1;     // BIT 7 
}STRUCT_PACKET_Flg;
typedef union{
   STRUCT_PACKET_Flg    BIT;
   u8                   BYTE;
}UNION_HALLDATA_READ;
//.................................
typedef struct{
   //1 

   //2
   u8   EXHHARDWARE     :1;     // BIT 0 
   u8   EXHErr          :1;     // BIT 1 
   u8   EXHZEROOK       :1;     // BIT 2
   u8   EXHSENSOR       :1;     // BIT 3
   u8   RESERVE4        :1;     // BIT 4
   u8   RESERVE5        :1;     // BIT 5
   u8   RESERVE6        :1;     // BIT 6
   u8   RESERVE7        :1;     // BIT 7
   
}STRUCT_PACKETBIT_HW_ERROR;
//----------------------------------
typedef struct{
  u16       TARGET;     // PWM*100 => 0~10000
  u16       CURRENT;     // PWM*100 => 0~10000
  u16       DELAY;      // mSec  0~1000 ( 0 = 1 mSec)
  u16       CNT_DELAY;    // 
  u8        STEP;       // 0~255  ( 0 = Maximum)
}STRUCT_PACKET_PWM;
//...................................
typedef union{
  u32     TIMER_CNT;
  u8      BYTE[4];
}UNION_PACKET_FRQ;
//...................................
typedef struct{
  s16   Press_INH;
  u16   Flow_EXH;
  u16   Valve_CMD;
  u16   Valve3_CMD;
  STRUCT_PACKETBIT_HW_ERROR  HWErr;


//-----------------
  
//-----------------------
}STRUCT_DATA_A2DSENSORS;
typedef struct{
   u8          Header1;
   u8          Header2;
   u8          Header3;
   u16          P01;
   u16          P02;
   u16          P03;
   u16          P04;
   u16          P05;
   u16          P06;
   u16          P07;
   u16          P08;
 //  u16          P09;
 // u16          P10;
   u8           CRCS;
}STRUCT_PACKET_COM2_SERIAL;
typedef union{
   STRUCT_PACKET_COM2_SERIAL  Data;
   u8                           BYTE[sizeof(Data)];
}UNION_PACKET_COM2_SERIAL;

//------------------------------------
//==========================================================================
#pragma pack(1)
//==========================================================================
extern  void    HallSensor_Service (void);
extern  void Algorithm_50ms (void);
extern  void Flow_SENSIRION_EXH_ReadDATA(void);
extern  void    SendCOM2 (u8 mode);
extern  void Read_Press_INH_EXH(void);
extern  void SetPWM_EXH (u16 PulsePercent);
extern  void SetPWM_TIM3_Valve (u16 PulsePercent);

//------------------------------------------------------------------------------|
#endif /* __MAIN_H */



//timeandpwmstructure
//=============================================================

