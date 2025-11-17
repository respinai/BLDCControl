//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
//------------------------------------------------------------------------------

#include "ISR_1.h"
#include "main.h"
#include "Algorithm.h"
//Define Variables ----------------------------------------------------
Staruct_TIMER                           Timer;
UNION_HALLDATA_READ                     HallSensor;
//---------------------------------------------------------------------
extern 	        u8	A2D_Channel;
extern          STRUCT_SEND_DATA        Send2MB; 
extern          UNION_PACKET_FRQ        SpeedMotor;
extern  Struct_ADC_Data             ADC_Data;
extern UNION_PACKET_COM2_SERIAL COM1;
//extern void    SendCOM2 (u8 mode);
//----------------------------------------------------------------------
extern void Flow_SENSIRION_EXH_ReadDATA(void);
u8  AllowCommand_FLG = 100;
u16 t1_temp = 1;
u8  Press_ISR_FLG = FALSE;
u8  Valve_CMD_FLG = FALSE;
//----------------------------------------------------------------------
/**
*   This function is called every 100 MicroSeconds.
*   
*/ 
void TIM2_IRQHandler(void)// 
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update))
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //  100 uSec
    //Timer.Flg_100us = TRUE;
    //-----------------------------------------------------
    if(CHECK_A2D_READINGCHANNEL())
    {
      ADC_Service();
    }//----------------------------------------------------
    if(++Timer.Cnt_1ms == 10)    // 10 * 100 = 1000 = 1 mSec
    {
      Timer.Cnt_1ms = 0;
      Timer.Flg_2ms5Sensor = TRUE;
      //Check_Current();
      STARTREADING_A2D_ALLCHANNELS();//------- Start ADC
      ReachToTarget_Saffar();
      Timer.Flg_1ms	= TRUE;//--------------------------
    }
    if(++Timer.Cnt_2ms5 == 25)            // 25 * 100 = 2.5 mSec
    {
      Timer.Cnt_2ms5 = 0;

      GPIO_ToggleBits(LED2_PORT, LED2_PIN);
      SendCOM2(0);
      Valve_CMD_FLG = TRUE;
      Algorithm_Service();
      
      
      Timer.Flg_2ms5 = TRUE;//---------------------------------
      if(++Timer.Cnt_10ms == 4)           // 4 * 2.5 = 10 mSec
      {
        Timer.Cnt_10ms = 0;
        
        Timer.Flg_10ms = TRUE;//-------------------------------
        if(++Timer.Cnt_20ms == 2)    // 2 * 10 = 20 mSec
        {                    
          Timer.Cnt_20ms = 0;
          
          Timer.Flg_20ms	= TRUE;//--------------------------
        }
        if(++Timer.Cnt_50ms == 5)         // 5 * 10 = 50 mSec
        {
          Timer.Cnt_50ms = 0;	
          Timer.Flg_50ms = TRUE;//-------------------------------
          if(++Timer.Cnt_100ms == 2)      // 2 * 50 = 100 mSec
          {
            Timer.Cnt_100ms = 0;
            Timer.Flg_100ms	= TRUE;//----------------------------
            if(++Timer.Cnt_200ms == 2)    // 2 * 100 = 200 mSec
            {
              Timer.Cnt_200ms = 0;
              Timer.Flg_200ms	= TRUE;//--------------------------
            }
            if(++Timer.Cnt_500ms == 5)    // 5 * 100 = 500 mSec
            {
              Timer.Cnt_500ms = 0;
              
              Timer.Flg_500ms	= TRUE;//--------------------------
              if(++Timer.Cnt_1sec == 2)   // 2 * 500 = 1000 mSec
              {																  //	1 	Sec			
                Timer.Cnt_1sec = 0;
                
                Timer.Flg_1sec	= TRUE;//------------------------
                if(++Timer.Cnt_2sec == 5)   // 5 * 1000 = 3000 mSec
                {
                  Timer.Cnt_2sec = 0; 
                  Timer.Flg_2sec = TRUE;
                  //Timer.Flg_2sec	= TRUE;//------------------------                           
                }//5Sec
              }//1sec
            }//500msec
          }//100msec
        }//50msec
      }//10msec
    }//2.5msec
  }//100usec
}//Tim2
//--------------------------------------------------------------------
void    EXTI9_5_IRQHandler(void)
{
  /* Make sure that interrupt flag is set */
  if (EXTI_GetITStatus(EXTI_Line6) != RESET) 
  {
    EXTI_ClearITPendingBit(EXTI_Line6);
  }
  if (EXTI_GetITStatus(EXTI_Line7) != RESET) 
  {
    EXTI_ClearITPendingBit(EXTI_Line7);
  }
  //
  HallSensor_Service();
}
//---------------
void    EXTI0_IRQHandler(void)
{
  EXTI_ClearITPendingBit(EXTI_Line0);
  //
  HallSensor_Service();
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
void USART1_IRQHandler(void)         /*  Connect to Mother Board  */
{
  //-------------------------< Recieve >----------------------------
 if(USART_GetITStatus(COMPORT,USART_IT_RXNE))
 {
 USART_ClearITPendingBit(COMPORT,USART_IT_RXNE); 	// clear INT Bit   
  RCV_data((u8)(COMPORT->DR & (u16)0x00FF));
  }
  //--------------------------< Send >-------  
  if(USART_GetITStatus(COMPORT,USART_IT_TXE))
  {
    USART_ClearITPendingBit(MAINBOARD,USART_IT_TXE); // clear INT Bit
    //----
    SendCOM2(1);
//    if( Send2MB.Wp != Send2MB.Rp )
//    {
//      USART_SendData(MAINBOARD, Send2MB.Buffer[Send2MB.Rp]);    // send   
//      INCREMENT_POINTER(Send2MB.Rp , LEN_BUFFER_SEND2MB);
//    }
//    else    
//    {
//      Send2MB.Flg_Sending = DISABLE;
//      USART_ITConfig(MAINBOARD,USART_IT_TXE,DISABLE);
//    }			
  } 
}
//-----------------------------------------------------------------------------
void SPI2_IRQHandler (void)	// // Press Sensor  INH
{ 
  if(SPI_I2S_GetITStatus(PRESS_INH,SPI_IT_RXNE))
  {
    SPI_I2S_ClearITPendingBit(PRESS_INH, SPI_IT_RXNE);// clear INT Bit
    Press_ISR_FLG = TRUE; 
    SPI_I2S_ITConfig(PRESS_INH , SPI_IT_RXNE , DISABLE );
  }  
}
//---------------------------------------------------------
void    HallSensor_Service (void)
{
  u16        t1= t1_temp;
  static    u8  CnT=0;
if(++CnT == 6)
  {
    SpeedMotor.TIMER_CNT = TIM_GetCounter(TIM5);
    TIM_SetCounter(TIM5,0);
    CnT=0;
    
  }
//  else if(GPIO_Pin == GPIO_PIN_6)
//  {
//    
//    Cnt_6++;
//  }
//  else if(GPIO_Pin == GPIO_PIN_7)
//  {
//    
//    Cnt_7++;
//  }

  
  //........
  HallSensor.BIT.Hall_1 = GPIO_ReadInputDataBit(Hall1_PORT,Hall1_PIN);
  HallSensor.BIT.Hall_2 = GPIO_ReadInputDataBit(Hall2_PORT,Hall2_PIN);
  HallSensor.BIT.Hall_3 = GPIO_ReadInputDataBit(Hall3_PORT,Hall3_PIN);

  if(AllowCommand_FLG)
  {
    MoveBLDC(0);
    while(t1){t1--;}
    MoveBLDC(10);
    t1  = t1_temp; 
    
  }
}
//---------------------------------------------------------
void Check_Current(void)
{
  static    u8 Cnt_100msSTOP=0,Cnt_100msNORMAL=0 ;
  

  if( (ADC_Data.Curr_IRS1 > 3000))//||(ADC_Data.Curr_IRS2 > 3000)||(ADC_Data.Curr_IRS3 > 3000))
  {
    Cnt_100msSTOP++;
    if(Cnt_100msSTOP >= 100)
    {
      Cnt_100msSTOP = 102;
      AllowCommand_FLG = 0;
      Cnt_100msNORMAL = 0;
      PWM_Timer1(M1_CH1,STOP);
      PWM_Timer1(M2_CH2,STOP);
      PWM_Timer1(M3_CH3,STOP);
      GPIO_WriteBit(M1_L_PORT,M1_L_PIN,Bit_RESET);
      GPIO_WriteBit(M2_L_PORT,M2_L_PIN,Bit_RESET);
      GPIO_WriteBit(M3_L_PORT,M3_L_PIN,Bit_RESET);
    }
  }
  else
  {
    Cnt_100msNORMAL++;
    Cnt_100msSTOP = 0;
    if(Cnt_100msNORMAL >= 100)
    {
      Cnt_100msNORMAL = 102;
      AllowCommand_FLG = 100;
    }
  }
}
//---------------------------------------------------------

