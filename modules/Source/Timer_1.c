//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
//-----------------------------------------------------------------------------|
#include "Timer_1.h"
//-----------------------------------------------------------------------------|
TIM_TimeBaseInitTypeDef                 TIM_TimeBaseInitStruct;	
TIM_OCInitTypeDef 		        TIM_OCInitStruct;
static void ValvePWM_GPIO_Init(void);
//-----------------------------------------------------------------------------|
extern      NVIC_InitTypeDef		NVIC_InitStructure ;
extern        STRUCT_PACKET_PWM           PWM;
extern        STRUCT_PACKET_PWM           PWMS;
extern      UNION_HALLDATA_READ                     HallSensor;

#define VALVE_PWM_TIMER_PRESCALER    3U     /* 84MHz / (3+1) = 21MHz -> 20kHz with ARR=1049 */
#define VALVE_PWM_TIMER_PERIOD       (Timer4_PulseWidth - 1U)   /* Matches EXH PWM (≈20kHz) */
#define VALVE_PWM_TIMER_TICKS        (VALVE_PWM_TIMER_PERIOD + 1U)

//-----------------------------------------------------------------------------|
void Timer_INITIALIZE(void)
{
  return;
}
//-----------------------------------------------------------------------------|
void TIM_ConfigPort (void)
{
  //u16  temp_CNT;
  Timer_1_Initialize();   //,   APB2_FRQ = 168MHz     	
  Timer_2_Initialize();   //,   APB1_FRQ = 84 MHz     
  Timer_3_Initialize();   //,   APB1_FRQ = 84 MHz   
  Timer_4_Initialize();  
  Timer_5_Initialize();   //,   APB1_FRQ = 84 MHz     
  Timer_6_Initialize();   //,   APB1_FRQ = 84 MHz (100us free counter)

}
//-----------------------------------------------------------------------------|
void Timer_1_Initialize(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   ///APB2_FRQ = 168MHz									
  TIM_TimeBaseInitStruct.TIM_Prescaler = 1;  // 
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period =( Timer1_PulseWidth-1);		// Output Frq = 2KHz [41999]
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
  //  Enables the peripheral Outputs.
  TIM_CtrlPWMOutputs(TIM1 , ENABLE); 
  //  PWM Mode configuration Channel 1
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 0;  
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM1, &TIM_OCInitStruct);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  //  PWM Mode configuration Channel 2
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 0;  
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM1, &TIM_OCInitStruct);
  TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
  //  PWM Mode configuration Channel 3
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 0;  
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC3Init(TIM1, &TIM_OCInitStruct);
  TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
  //Start Timer 1
  TIM_Cmd(TIM1, ENABLE);

}
//-----------------------------------------------------------------------------|
void Timer_2_Initialize(void)           /*  Main Counter  :::  84MHz */ 
{
  // For 100 uSec
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  //  ///APB1-FRQ = Input Frq = 84MHz 
  TIM_TimeBaseInitStruct.TIM_Prescaler = 839;// // Input Frq  = 84MHz / (839+1) = 100KHz
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM_CounterMode_Down; //
  TIM_TimeBaseInitStruct.TIM_Period = 9;		// 100000 /(9+1) =10000 => T=100 uSec  		// between 0x0000 and 0xFFFF.
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
  //						  
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_GenerateEvent(TIM2, TIM_IT_Update);
  
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1 ;//
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
  NVIC_Init(&NVIC_InitStructure) ;//
  
    //  Start Timer 
  TIM_Cmd(TIM2, ENABLE);
}//-----------------------------------------------------------------------------|
void Timer_3_Initialize(void)
{
  ValvePWM_GPIO_Init();
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); ///APB1_FRQ = Input Frq = 84MHz 
  TIM_TimeBaseInitStruct.TIM_Prescaler = VALVE_PWM_TIMER_PRESCALER;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInitStruct.TIM_Period = VALVE_PWM_TIMER_PERIOD;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(VALVE_TIM3, &TIM_TimeBaseInitStruct);

  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 0;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC3Init(VALVE_TIM3, &TIM_OCInitStruct);
  TIM_OC3PreloadConfig(VALVE_TIM3, TIM_OCPreload_Enable);

  TIM_Cmd(VALVE_TIM3, ENABLE);
}
//-----------------------------------------------------------------------------|
void Timer_5_Initialize(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//APB1_FRQ = 84MHz 
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;  // 
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = (Timer5_PulseWidth-1);		// T = 11.904
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);
  
  //  
  TIM_Cmd(TIM5, ENABLE);
}
//-----------------------------------------------------------------------------|
void Timer_6_Initialize(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); ///APB1_FRQ = 84MHz 
  TIM_TimeBaseInitStruct.TIM_Prescaler = 8399; // Input Frq  = 84MHz / (8399+1) = 10,000Hz
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; 
  TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF;	// 10,000Hz => T = 100uSec
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
  TIM_Cmd(TIM6, ENABLE);
}
//-----------------------------------------------------------------------------|
//-----------------------------------------------------------------------------|
void Timer_4_Initialize(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//APB1_FRQ = 84MHz 
  TIM_TimeBaseInitStruct.TIM_Prescaler = 3;  // Input Frq  = 84MHz / (3+1) = 21MHz
  TIM_TimeBaseInitStruct.TIM_Period = (Timer4_PulseWidth-1);		// Output Frq =	21MHz / 52500 = 400Hz
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
  //  SET OUTPUT
  TIM_CtrlPWMOutputs(TIM4 , ENABLE);
  //  SET Interrupt
  //	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  //	TIM_GenerateEvent(TIM4, TIM_IT_Update);
  //  PWM1 Mode configuration Channel 1  For HEATER 
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 656;  // for 50% 
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM4, &TIM_OCInitStruct);
  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
  //  PWM1 Mode configuration Channel 2  For NEBULIZER 
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 328;  // for 50% 
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC2Init(TIM4, &TIM_OCInitStruct);
  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
  //  Start Timer 4
  TIM_Cmd(TIM4, ENABLE);
}
//-----------------------------------------------------------------------------|
void  Set_PWM(uint16_t DataIn)
{
  u16   Temp_PulseWidth = (DataIn * Timer1_PulseWidth)/10000;
  PWM_TIM->CCR1 = Temp_PulseWidth;	
  PWM_TIM->CCR2 = Temp_PulseWidth;	
  PWM_TIM->CCR3 = Temp_PulseWidth;	
  
}
//-----------------------------------------------------------
void SetPWM_EXH (u16 PulsePercent)
{
  u16		Temp_PulseWidth;
  Temp_PulseWidth = (u16) (((u32) EXH_PulseWidth * PulsePercent) / 10000);
  //* Set the Capture Compare Register value 
  //  Timer4_CH1 usd for EXH
  EXH_TIM->CCR1 = Temp_PulseWidth;
}

void SetPWM_TIM3_Valve (u16 PulsePercent)
{
  if(PulsePercent > VALVE_TIM3_PWM_SCALE)
  {
    PulsePercent = VALVE_TIM3_PWM_SCALE;
  }
  u32 pulse = ((u32)VALVE_PWM_TIMER_TICKS * PulsePercent) / VALVE_TIM3_PWM_SCALE;
  if(pulse > VALVE_PWM_TIMER_PERIOD)
  {
    pulse = VALVE_PWM_TIMER_PERIOD;
  }
  TIM_SetCompare3(VALVE_TIM3, (u16)pulse);
}
//-----------------------------------------------------------------
void    ReachToTarget(void)
{
  if(PWM.CURRENT != PWM.TARGET)
  {
    if(++PWM.CNT_DELAY >= PWM.DELAY)
    {
      PWM.CNT_DELAY = 0;
      if(PWM.STEP)
      {
        if(PWM.CURRENT < PWM.TARGET)
        {
          PWM.CURRENT += PWM.STEP;
          PWM.CURRENT = (PWM.CURRENT>PWM.TARGET)?PWM.TARGET:PWM.CURRENT; 
        }
        else if(PWM.CURRENT > PWM.TARGET)
        {
          if(PWM.CURRENT >= PWM.STEP)
            PWM.CURRENT -= PWM.STEP;
          else
            PWM.CURRENT = PWM.TARGET;
          PWM.CURRENT = (PWM.CURRENT<PWM.TARGET)?PWM.TARGET:PWM.CURRENT;
        }
        
      }
      else
        PWM.CURRENT = PWM.TARGET;
      Set_PWM(PWM.CURRENT);
    }
  }
}

//-----------------------------------------------------------------
void    ReachToTarget_Saffar(void)
{
  static int Cnt_Startup = 0;
  static u8 Is_Startup = 0;
  
  if ( (PWMS.CURRENT == 0) && (Is_Startup ==0) && (PWMS.TARGET>500) )
  {
    Is_Startup = 1;
    Cnt_Startup = 0;
  }
  
  if (Is_Startup == 1)
    
  {
    if (Cnt_Startup <500)
    {
      PWMS.CURRENT = Cnt_Startup; 
      Set_PWM(PWMS.CURRENT); 
      Cnt_Startup++;
    }
    else
    {
      Cnt_Startup = 0;
      Is_Startup = 0; 
    }
    
  }
  else
  {
    
    
    if(PWMS.CURRENT != PWMS.TARGET)
    {
      if(++PWMS.CNT_DELAY >= PWMS.DELAY)
      {
        PWMS.CNT_DELAY = 0;
        if(PWMS.STEP)
        {
          if(PWMS.CURRENT < PWMS.TARGET)
          {
            PWMS.CURRENT += PWMS.STEP;
            PWMS.CURRENT = (PWMS.CURRENT>PWMS.TARGET)?PWMS.TARGET:PWMS.CURRENT; 
          }
          else if(PWMS.CURRENT > PWMS.TARGET)
          {
            if(PWMS.CURRENT >= PWMS.STEP)
              PWMS.CURRENT -= PWMS.STEP;
            //PWMS.CURRENT = (PWMS.CURRENT<PWMS.TARGET)?PWMS.TARGET:PWMS.CURRENT;
            else
              PWMS.CURRENT = PWMS.TARGET;
            PWMS.CURRENT = (PWMS.CURRENT<PWMS.TARGET)?PWMS.TARGET:PWMS.CURRENT;
          }
          
        }
        else
          PWMS.CURRENT = PWMS.TARGET;
        Set_PWM(PWMS.CURRENT);
      }
    }
    
  }
}

//-----------------------------------------------------------------------------|

u16    Control(u16 SpeedSet,u32 SpeedMotor, float KP,float KI)
{
 float  Speed = 0; 
   
 float  Speederr = 0;
 float Output=0;
 if (SpeedMotor>1000)
 Speed = 60/(SpeedMotor*11.904*0.000000001);
 else
 Speed = 0; 
 Speederr = SpeedSet - Speed;
 
 static float KIterm=0;
 KIterm = KIterm + Speederr*0.0025*KI;
 if (KIterm>9500)
   KIterm = 9500;
 if(KIterm<0)
   KIterm=0;
 Output = Speederr*KP+KIterm;
  if (Output>9500)
   Output = 9500;
 if(Output<0)
   Output=0;
 if (SpeedSet==0)
   Output=0;   
 return (u16) Output;
 
}

u16    Control_Flow(u16 Setpoint,float Flow_Exh , float KP,float KI)
{
 float  Flowerr = 0; 
 u16 Outputvalve = 0;  

 Flowerr = Setpoint - Flow_Exh;
 static float KIterm1=0;
 
 KIterm1 = KIterm1 + Flowerr*0.0025*KI;
 if (KIterm1>5700)
   KIterm1 = 5700;
 if(KIterm1<0)
   KIterm1=0;
 Outputvalve = Flowerr*KP+KIterm1;
  if (Outputvalve>9000)
   Outputvalve = 9000;
 if(Outputvalve<0)
   Outputvalve=0;
 if (Setpoint==0.0f)
   Outputvalve=0;   
 return Outputvalve;
 
}
void PWM_Timer1(u8  ChanneL ,u8 NewState)
{
  if(ChanneL == M1_CH1)
  {
    if(NewState == START)
    {
      TIM_CCxCmd(PWM_TIM,TIM_Channel_1,TIM_CCx_Enable);
      TIM_OC1PreloadConfig(PWM_TIM,TIM_OCPreload_Enable);
    }
    else if(NewState == STOP)
    {
      TIM_CCxCmd(PWM_TIM,TIM_Channel_1,TIM_CCx_Disable);
      TIM_OC1PreloadConfig(PWM_TIM,TIM_OCPreload_Disable);
    }
  }
  else if(ChanneL == M2_CH2)
  {
    if(NewState == START)
    {
      TIM_CCxCmd(PWM_TIM,TIM_Channel_2,TIM_CCx_Enable);
      TIM_OC2PreloadConfig(PWM_TIM,TIM_OCPreload_Enable);
    }
    else if(NewState == STOP)
    {
      TIM_CCxCmd(PWM_TIM,TIM_Channel_2,TIM_CCx_Disable);
      TIM_OC2PreloadConfig(PWM_TIM,TIM_OCPreload_Disable);
    }
  } 
  else if(ChanneL == M3_CH3)
  {
    if(NewState == START)
    {
      TIM_CCxCmd(PWM_TIM,TIM_Channel_3,TIM_CCx_Enable);
      TIM_OC3PreloadConfig(PWM_TIM,TIM_OCPreload_Enable);
    }
    else if(NewState == STOP)
    {
      TIM_CCxCmd(PWM_TIM,TIM_Channel_3,TIM_CCx_Disable);
      TIM_OC3PreloadConfig(PWM_TIM,TIM_OCPreload_Disable);    
    }
  } 
}
//-----------------------------------------------------------------------------|
void  MoveBLDC(u8  Flg_state )
{
  switch(HallSensor.BYTE)
  {
  case Case1: 
    if(Flg_state==0)//Delay
    {
      
      PWM_Timer1(M1_CH1,STOP);//H1
      PWM_Timer1(M2_CH2,START);//H2
      PWM_Timer1(M3_CH3,STOP);//H3     
      GPIO_WriteBit(M1_L_PORT,M1_L_PIN,Bit_RESET);//L1
      GPIO_WriteBit(M2_L_PORT,M2_L_PIN,Bit_RESET);//L2
      GPIO_WriteBit(M3_L_PORT,M3_L_PIN,Bit_RESET);//L3
      //
    }
    else if(Flg_state==10)
    {
      GPIO_WriteBit(M3_L_PORT,M3_L_PIN,Bit_SET);//L3
    }
    
    break;
  case Case2://----------------------------------
    if(Flg_state==0)//Delay
    {
      
      
      PWM_Timer1(M1_CH1,STOP);//
      PWM_Timer1(M2_CH2,STOP);//
      PWM_Timer1(M3_CH3,STOP);//
      GPIO_WriteBit(M1_L_PORT,M1_L_PIN,Bit_RESET);//
      GPIO_WriteBit(M2_L_PORT,M2_L_PIN,Bit_RESET);//
      GPIO_WriteBit(M3_L_PORT,M3_L_PIN,Bit_SET);//
      //
    }
    else if(Flg_state==10)
    {  
      TIM_SetCounter(PWM_TIM,0);
      PWM_Timer1(M1_CH1,START);//
    }
   
    break;
  case Case3://----------------------------------
    if(Flg_state==0)//Delay
    {
      
      
      PWM_Timer1(M1_CH1,START);//
      PWM_Timer1(M2_CH2,STOP);//
      PWM_Timer1(M3_CH3,STOP);//
      GPIO_WriteBit(M1_L_PORT,M1_L_PIN,Bit_RESET);//
      GPIO_WriteBit(M2_L_PORT,M2_L_PIN,Bit_RESET);//
      GPIO_WriteBit(M3_L_PORT,M3_L_PIN,Bit_RESET);//
    }
    else if(Flg_state==10)
    {

      GPIO_WriteBit(M2_L_PORT,M2_L_PIN,Bit_SET);//
    }

    break;
  case Case4://----------------------------------
    if(Flg_state==0)//Delay
    {

      PWM_Timer1(M1_CH1,STOP);//
      PWM_Timer1(M2_CH2,STOP);//
      PWM_Timer1(M3_CH3,STOP);//
      GPIO_WriteBit(M1_L_PORT,M1_L_PIN,Bit_RESET);//
      GPIO_WriteBit(M2_L_PORT,M2_L_PIN,Bit_SET);//
      GPIO_WriteBit(M3_L_PORT,M3_L_PIN,Bit_RESET);//
      //
    }
    else if(Flg_state==10)
    {
      TIM_SetCounter(PWM_TIM,0);
      PWM_Timer1(M3_CH3,START);//
    }

      
    break;
  case Case5://----------------------------------
    if(Flg_state==0)//Delay
    {
      PWM_Timer1(M1_CH1,STOP);//
      PWM_Timer1(M2_CH2,STOP);//
      PWM_Timer1(M3_CH3,START);//
      GPIO_WriteBit(M1_L_PORT,M1_L_PIN,Bit_RESET);//
      GPIO_WriteBit(M2_L_PORT,M2_L_PIN,Bit_RESET);//
      GPIO_WriteBit(M3_L_PORT,M3_L_PIN,Bit_RESET);//
    }
    else if(Flg_state==10)
    {
      GPIO_WriteBit(M1_L_PORT,M1_L_PIN,Bit_SET);//
    }

    break;
  case Case6://----------------------------------
    if(Flg_state==0)//Delay
    {
      PWM_Timer1(M1_CH1,STOP);//
      PWM_Timer1(M2_CH2,STOP);//
      PWM_Timer1(M3_CH3,STOP);//
      GPIO_WriteBit(M1_L_PORT,M1_L_PIN,Bit_SET);//
      GPIO_WriteBit(M2_L_PORT,M2_L_PIN,Bit_RESET);//
      GPIO_WriteBit(M3_L_PORT,M3_L_PIN,Bit_RESET);//
      //
    }
    else if(Flg_state==10)
    {
      TIM_SetCounter(PWM_TIM,0);
      PWM_Timer1(M2_CH2,START);//
    }
    break;
    
  }//switch
  
}
//-----------------------------------------------------------------------------|

static void ValvePWM_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(VALVE_TIM3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin = VALVE_TIM3_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(VALVE_TIM3_GPIO_PORT, &GPIO_InitStructure);
  GPIO_PinAFConfig(VALVE_TIM3_GPIO_PORT, VALVE_TIM3_GPIO_SOURCE, VALVE_TIM3_AF);
}
//-----------------------------------------------------------------------------|

