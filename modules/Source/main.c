//--------------------------------------------------------------------------|
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// 
//--------------------------------------------------------------------------|
#include "main.h" 

extern  STRUCT_PACKET_PWM           PWM;

extern  Staruct_TIMER  Timer;
extern          UNION_PACKET_FRQ        SpeedMotor;
extern UNION_PACKET_COM2_SERIAL COM1;
extern  u8  Press_ISR_FLG,Valve_CMD_FLG;
extern UNION_PACKET_COM2_SERIAL COM1;
extern  Struct_ADC_Data             ADC_Data;
extern UNION_PACKET_RCVPWM          RcvPWM;
STRUCT_DATA_A2DSENSORS          DATA_Sensors;
u16 cntest = 0;
u16 SpeedSet=0;
u16 Flowset=0;
float KP=0.1;
float KI=0.3;
float KP_Flow =8;
float KI_Flow =250;
float speedtemp;
u8 control_mode=0;
u8 control_valve_mode = 0;
static int PWM_ind=0;
u8 iteration = 0;
static int cnt_valve = 0;
int Sample_arr[] = {809,798,677,785,842,865,746,734,994,595,951,963,914,621,689,720,865,637,882,624,854,787,907,880,959,954,720,873,663,592,892,790,781,959,836,839,940};
int PWM_arr[] = {2000,3000,4000,5000,6000,7000,8000,2000,3000,4000,5000,6000,7000,8000,2000,3000,4000,5000,6000,
 7000,8000,2000,3000,4000,5000,6000,7000,8000,2000,3000,4000,5000,6000,7000,8000,2000,3000};

float PWM_Array[] = {0,175,827,3340,85,409,600,408,0};
STRUCT_PACKET_PWM           PWMS = {
  .DELAY = 2,
  .STEP = 20
    
};
//--------------------------------------------------------------------------|
int main(void)
{
  u8      PWM_zero=0;
  Init();
  
  while(1)
  {  
    
    
    IWDG->KR = KR_KEY_RELOAD;                 /* Reset Watchdog */
    //--------------------
    
    if(PWMS.TARGET > 0)
    {
      if(PWM_zero == 0)
      {
        PWM_zero = 1;
        HallSensor_Service();
      }
    }
    else
    {
      PWM_zero = 0;
      PWM_Timer1(M1_CH1,STOP);
      PWM_Timer1(M2_CH2,STOP);
      PWM_Timer1(M3_CH3,STOP);
      GPIO_WriteBit(M1_L_PORT,M1_L_PIN,Bit_RESET);
      GPIO_WriteBit(M2_L_PORT,M2_L_PIN,Bit_RESET);
      GPIO_WriteBit(M3_L_PORT,M3_L_PIN,Bit_RESET);
    }
    //--------------------
    if(Timer.Flg_2ms5Sensor == TRUE)
    {
      if ( cntest < 60000)
      { cntest++;
      }
      else
      {
        cntest = 0;
      }
      COM1.Data.P01 = (u16) cntest;
      COM1.Data.P02 = (u16) DATA_Sensors.Flow_EXH;
      COM1.Data.P03 = (u16) DATA_Sensors.Valve_CMD;
      COM1.Data.P04 = (u16) speedtemp;
      COM1.Data.P05 = (u16) DATA_Sensors.Press_INH;
      COM1.Data.P06 = (u16) 0x0000;
      COM1.Data.P07 = (u16) 0x0000;
      COM1.Data.P08 = (u16) 0x0000;
      //GPIO_ToggleBits(LED2_PORT, LED2_PIN);
      //SendCOM2(0);
      
      Timer.Flg_2ms5Sensor = FALSE;
      //      //	  Read_Press_INH_EXH();
      GPIO_ResetBits(PRESSINHCS_PORT , PRESSINHCS_PIN);       // CS LOW
      SPI_I2S_SendData(PRESS_INH,0xAA55);      // 0XAA55 is temporary data for read from SPI
      SPI_I2S_ITConfig(PRESS_INH , SPI_IT_RXNE , ENABLE );
      //      //------
      Flow_SENSIRION_EXH_ReadDATA();
      //SendCOM2(0);

      
    }
    //---------------------
    if(Press_ISR_FLG == TRUE)
    {
      Press_ISR_FLG = FALSE;
      Read_Press_INH_EXH(); 
    }
    //---------------------
      if(Valve_CMD_FLG == TRUE)
      {
        SetPWM_EXH(DATA_Sensors.Valve_CMD);
        SetPWM_TIM3_Valve(DATA_Sensors.Valve3_CMD);

        Valve_CMD_FLG = FALSE;
      }
    //---------------------
    if(Timer.Flg_50ms)
    {
      Timer.Flg_50ms = FALSE;
      //SendCOM2(0);
      // Algorithm_50ms();
      //ReachToTarget();
    }
    if(Timer.Flg_2ms5)
    {
      //SendCOM2(0);
      Timer.Flg_2ms5 = FALSE;
      if(SpeedMotor.TIMER_CNT>1000)
        speedtemp = 60/(SpeedMotor.TIMER_CNT*11.904*0.000000001);
      else
        speedtemp =0;
      //COM1.Data.P01 = (u16) (PWMS.CURRENT);
      //COM1.Data.P02 = (u16) (speedtemp);
      if (RcvPWM.DATA.Control_mode==0)
      {
        PWMS.TARGET = Control(RcvPWM.DATA.Speed_Setpoint, SpeedMotor.TIMER_CNT, (float)(RcvPWM.DATA.KP_Speed)/100 ,(float)(RcvPWM.DATA.KI_Speed)/100);
        DATA_Sensors.Valve_CMD = Control_Flow( RcvPWM.DATA.Flow_Setpoint , ((float) (DATA_Sensors.Flow_EXH)/100) ,KP_Flow, KI_Flow);
          
      }
      
      if (RcvPWM.DATA.Control_mode==1)
      {
       PWMS.TARGET = Control(RcvPWM.DATA.Speed_Setpoint,SpeedMotor.TIMER_CNT,(float)(RcvPWM.DATA.KP_Speed)/100,(float)(RcvPWM.DATA.KI_Speed)/100); 
       DATA_Sensors.Valve_CMD =  RcvPWM.DATA.Flow_Setpoint;
      }
      if (RcvPWM.DATA.Control_mode==3)
      {
       PWMS.TARGET = Control(SpeedSet,SpeedMotor.TIMER_CNT,(float)(KP),(float)(KI)); 
       DATA_Sensors.Valve_CMD =  Flowset;
      }
      // Algorithm_50ms();
      //ReachToTarget();
      
      
      if (control_mode == 3)
      {
        if (iteration == 36)
        {
          PWMS.TARGET = 0; 
        }
        
        else
          
        {
          if (PWM_ind <Sample_arr[iteration])
          {
            PWMS.TARGET = PWM_arr[iteration];
            PWM_ind++;
          }
          else
          {
            PWM_ind = 0 ;
            iteration++;
            
          }
        }
        
        
        
      }
      if(RcvPWM.DATA.Control_mode== 4)
      {
        PWMS.TARGET = Control(RcvPWM.DATA.Speed_Setpoint,SpeedMotor.TIMER_CNT,(float)(RcvPWM.DATA.KP_Speed)/100,(float)(RcvPWM.DATA.KI_Speed)/100); 
        if (cnt_valve==12000)
        {cnt_valve = 0;}
        if (cnt_valve<12001)
        {
         cnt_valve++; 
        if (cnt_valve<5001)
        {
     
       DATA_Sensors.Valve_CMD++;
        }
        else
        {
          if (DATA_Sensors.Valve_CMD > 0 )
          {DATA_Sensors.Valve_CMD--; }
        }
        }
      }
    }
    
    //-----------
    if(Timer.Flg_1sec)
    {
      Timer.Flg_1sec = FALSE;
      GPIO_ToggleBits(LED1_PORT, LED1_PIN);
      
    } 
    if(Timer.Flg_2sec)
    {
      if (control_mode==2)
      {  
        if  (PWMS.TARGET<95)
        {PWMS.TARGET = PWMS.TARGET + 10; }
        else if  (PWMS.TARGET<195)
        {PWMS.TARGET = PWMS.TARGET + 25;} 
        else if  (PWMS.TARGET<395)
        {PWMS.TARGET = PWMS.TARGET + 50; }
        else if  (PWMS.TARGET<995)
        {PWMS.TARGET = PWMS.TARGET + 100; }
        else if  (PWMS.TARGET<1995)
        {PWMS.TARGET = PWMS.TARGET + 250;} 
        else if  (PWMS.TARGET<8005)
        {PWMS.TARGET = PWMS.TARGET + 1000; }
        else 
        {PWMS.TARGET = 0; }
      }
      

      //COM1.Data.P01 = (u16) (PWMS.CURRENT);
      Timer.Flg_2sec = FALSE;
    }
  }
}
//-------------------------------------------------------------------------------

//--------------------------------------------------------------------------|

//--------------------------------------------------------------------------|
