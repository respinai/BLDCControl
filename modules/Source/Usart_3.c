/**
******************************************************************************
* File Name          : UART.c
* Description        : my program body
******************************************************************************
******************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "Usart_3.h"
#include "main.h"
//--------------------------------------
/* Extern Variables ---------------------------------------------------------*/

STRUCT_PACKET_PWM                   PWM;
UNION_PACKET_COM2_SERIAL            COM1;
extern  Struct_ADC_Data             ADC_Data;

/* Variables Definition  ----------------------------------------------------*/
UNION_PACKET_RCVPWM     RcvPWM;
UNION_PACKET_FRQ        SpeedMotor;
STRUCT_SEND_DATA        Send2MB;

u8     ReceiveData = 0;

//-----------------------------------------
//------------------------------------------------------------------------------
void    SendCOM2 (u8 mode)
{
#define         sizeBUFF        20
    static u8     BUFFCOM2[sizeBUFF],Ptr_Rd,Ptr_Wd;
    
    if(mode == 0)
    {
        Ptr_Wd=0;
        //-------------------
        COM1.Data.Header1 = 0xAA;
        COM1.Data.Header2 = 0xFF;
        COM1.Data.Header3 = 0xDC;
        COM1.Data.CRCS =0;
        for(Ptr_Wd=0;Ptr_Wd<(sizeBUFF - 1);Ptr_Wd++)
        {
            BUFFCOM2[Ptr_Wd] = COM1.BYTE[Ptr_Wd];
            COM1.Data.CRCS += BUFFCOM2[Ptr_Wd];
       }
        BUFFCOM2[sizeBUFF - 1] = COM1.Data.CRCS;
        USART_SendData(COMPORT, BUFFCOM2[0]);
        Ptr_Rd = 1;
        USART_ITConfig(COMPORT,USART_IT_TXE,ENABLE);  // 
        
    }
    if(mode == 1)
    {
        if( Ptr_Rd < sizeBUFF )
        {            
            //USART_SendData(COMPORT, 0xFF); 
            USART_SendData(COMPORT, BUFFCOM2[Ptr_Rd]); 

            if(Ptr_Rd==(sizeBUFF -1))
            {
               USART_ITConfig(COMPORT,USART_IT_TXE,DISABLE);
            }
            Ptr_Rd++;
        } 
        //    else
        //    {
        //      USART_ITConfig(SAMBOARD,USART_IT_TXE,DISABLE);
        //    }
    }
}
//-----------------------------------------
void    Send_Data(u8 Packet)
{
  u8        CS_t=0;
  if(Packet == 1)//speed
  {

    Send2MB.Buffer[Send2MB.Wp] = 0xAA;//aa
    CS_t = Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //
    Send2MB.Buffer[Send2MB.Wp] = 5;//len
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //
    Send2MB.Buffer[Send2MB.Wp] = 0xB1;//code-----------
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //
    Send2MB.Buffer[Send2MB.Wp] = SpeedMotor.BYTE[0];//s1
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //
    Send2MB.Buffer[Send2MB.Wp] = SpeedMotor.BYTE[1];//s2
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //
    Send2MB.Buffer[Send2MB.Wp] = SpeedMotor.BYTE[2];//s3
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //
    Send2MB.Buffer[Send2MB.Wp] = SpeedMotor.BYTE[3];//s4
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //--
    Send2MB.Buffer[Send2MB.Wp] = ADC_Data.Temperature;//temp1
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);    
    Send2MB.Buffer[Send2MB.Wp] = ADC_Data.Temperature>>8;//temp2
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //--
    Send2MB.Buffer[Send2MB.Wp] = ADC_Data.Curr_IRS1;//IRS11
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);    
    Send2MB.Buffer[Send2MB.Wp] = ADC_Data.Curr_IRS1>>8;//IRS12
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);    
    //--
    Send2MB.Buffer[Send2MB.Wp] = ADC_Data.Curr_Valve1;//VALVE1_Current
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    Send2MB.Buffer[Send2MB.Wp] = ADC_Data.Curr_Valve1>>8;//VALVE1_Current
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //--
    /*Send2MB.Buffer[Send2MB.Wp] = ADC_Data.Curr_IRS3;//IRS31
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    Send2MB.Buffer[Send2MB.Wp] = ADC_Data.Curr_IRS3>>8;//IRS32
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
   */
    Send2MB.Buffer[Send2MB.Wp] = PWM.TARGET;//ADC_Data.VIN;// ;//Vin1
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    Send2MB.Buffer[Send2MB.Wp] = PWM.TARGET>>8;//ADC_Data.VIN>>8;//Vin2
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //--
    Send2MB.Buffer[Send2MB.Wp] = PWM.CURRENT;//ADC_Data.V12;////V121
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    Send2MB.Buffer[Send2MB.Wp] = PWM.CURRENT>>8;//ADC_Data.V12>>8;//V122
    CS_t += Send2MB.Buffer[Send2MB.Wp];
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
    //--
    Send2MB.Buffer[Send2MB.Wp] = CS_t;
    INCREMENT_POINTER(Send2MB.Wp , LEN_BUFFER_SEND2MB);
/* ----->>  Starte sending */  // ============================== */           
  //  if( Send2MB.Flg_Sending == DISABLE )
    //{
     // Send2MB.Flg_Sending = ENABLE;
      //USART_SendData(MAINBOARD, Send2MB.Buffer[Send2MB.Rp]); 
      //INCREMENT_POINTER(Send2MB.Rp , LEN_BUFFER_SEND2MB);
      //USART_ITConfig(MAINBOARD,USART_IT_TXE,ENABLE);  // INT ersal faal  mishe  
   // }  
  }
}
//-----------------------------------------
void    RCV_data(u8 TempData)
{ 
  static    u8          Packet_State = 0 ;
  static    u8          LEN_=0,CS_=0,Cnt_=0;
  
  switch(Packet_State)
    {
    case 0: /* AA */
        if(TempData == 0xAA)
            Packet_State = 1;         // Goto LEN
        break;
    case 1: /* LEN */ 
        if (TempData == (u8)sizeof(STRUCT_PACKET_RCVPWM) )//|| TempData == 7 || TempData == 1  )
        {
            LEN_ = TempData+1;
            CS_ = 0xAA + LEN_;                   // Start Calculation CS     
            Packet_State = 2;      // Goto CODE
        }
        else if(TempData == 0xAA)
        {
            Packet_State = 1;       // Goto LEN
        }
        else  
        {
            Packet_State = 0;       //Return AA
        }
        break;         
        //-------------........................................---------------  
    case 2: /*   Code */ 
        if (TempData == 0xA1 )//|| TempData == 7 || TempData == 1  )
        {
            CS_ += TempData;       // AA+len+code     
            Packet_State = 10;      // Goto data
            LEN_ --;
            Cnt_ = 0;
        }
        else
        {
            Packet_State = 0; // Goto aa
        }  
        break;
    case 10: /* DATA   *///........................................................
        RcvPWM.BYTE[Cnt_] = TempData;
        Cnt_++;
        CS_ += TempData;
        LEN_ --;        
        if(!LEN_)
        {
          Packet_State = 0; // Goto cs
        }
        break;
    case 11:  /* CRC   *///........................................................
    //    if(CS_ == TempData)
     //   {   
     //     PWM.TARGET = RcvPWM.DATA.TARGET;
     //     PWM.DELAY = RcvPWM.DATA.DELAY;
     //     PWM.STEP = RcvPWM.DATA.STEP;
     //   }
        Packet_State = 0; // Goto aa
        break;
    default:
        Packet_State = 0; //return AA
        break;
    }//switch

    
}

//-----------------------------------------
//-----------------------------------------

//-----------------------------------------

//-----------------------------------------

//-----------------------------------------

//================================================================================

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
