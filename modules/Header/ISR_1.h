//-----------------------------------------------------------------------------|
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
//-----------------------------------------------------------------------------|
#ifndef _SAADAT_ISR_H
#define _SAADAT_ISR_H
//-----------------------------------------------------------------------------|
#include "main.h"
//-----------------------------------------------------------------------------|
#ifdef __cplusplus
extern "C" {
#endif 
   void TIM2_IRQHandler(void);
   void TIM1_UP_TIM10_IRQHandler(void);
   //void TIM8_UP_TIM13_IRQHandler(void);
   
   void USART1_IRQHandler(void);
   //void USART2_IRQHandler(void);
   //void USART3_IRQHandler(void);
   //void SPI1_IRQHandler(void);
   void SPI2_IRQHandler(void);
   //void SPI3_IRQHandler (void);
//----------
extern          void Read_Press_INH_EXH(void);

//------------
   
#ifdef __cplusplus
}
#endif
//-----------------------------------------------------------------------------|
void    HallSensor_Service (void);
void Check_Current(void);
//-----------------------------------------------------------------------------|
#endif		// _SAADAT_ISR_H

