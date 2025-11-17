//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
//------------------------------------------------------------------------------

#ifndef _SAADAT_TIMERS_H
#define _SAADAT_TIMERS_H

#include "main.h"

//#define     APB2_FRQ    168000000

#define     Timer1_PulseWidth       (84000000 / 80000) //42000
#define     Timer5_PulseWidth       4200000000  // ~50sec
#define     Timer4_PulseWidth       1050 //20KHz
//#define     Timer8_PulseWidth       52500
//#define     Timer8_FinalFreq        400
#define     Timer9_PulseWidth       5127
//-------------
#define     Case1   4//1//2//5//4//6//2//3//1
#define     Case2   6//3//3//4//6//2//3//1//5
#define     Case3   2//7//1//6//2//3//1//5//4
#define     Case4   3//6//5//2//3//1//5//4//6
#define     Case5   1//4//4//3//1//5//4//6//2
#define     Case6   5//0//6//1//5//4//6//2//3
//----------

typedef struct {
  u8        Cnt_1ms;
  u8        Cnt_2ms5;
  u8        Cnt_10ms;
  u8        Cnt_20ms;
  u8        Cnt_50ms;
  u8        Cnt_100ms;
  u8        Cnt_200ms;  
  u8        Cnt_500ms;
  u8        Cnt_1sec;
  u8        Cnt_2sec;
  u8        Cnt_Buzzer;
//  u8       Enable_Buz;
//.....................   
  u8        Flg_100us;
  u8        Flg_1ms;
  u8        Flg_2ms5;
  u8        Flg_10ms;
  u8        Flg_20ms;
  u8        Flg_50ms;
  u8        Flg_100ms;
  u8        Flg_200ms;
  u8        Flg_500ms;
  u8        Flg_1sec;
  u8        Flg_2sec;
  u8        Flg_2ms5Sensor;
}Staruct_TIMER;

//------------------------------------------------------------------------------
void TIM_ConfigPort(void);
void  Set_PWM(uint16_t DataIn);
void Timer_1_Initialize(void);
void Timer_2_Initialize(void);
void Timer_3_Initialize(void);
void Timer_6_Initialize(void);
void Timer_5_Initialize(void);
void Timer_4_Initialize(void);
void    ReachToTarget(void);
void    ReachToTarget_Saffar(void);
void  MoveBLDC(u8  Flg_state );
void PWM_Timer1(u8  ChanneL ,u8 NewState);
void SetPWM_TIM3_Valve(u16 PulsePercent);
u16    Control(u16 SpeedSet,u32 SpeedMotor, float KP,float KI);
u16    Control_Flow(u16 Setpoint,float Flow_Exh , float KP,float KI);
u16    Control_Pressure(u16 Setpoint,float Pressure , float KP,float KI);
//------------------------------------------------------------------------------
#endif      // _SAADAT_TIMERS_H
