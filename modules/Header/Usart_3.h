/**
  ******************************************************************************
  * File Name          : UART_H.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H
 /* Includes ------------------------------------------------------------------*/
//#include "string.h"
#include "main.h"
//-------------------------------------------
#define     LEN_BUFFER_SEND2MB     40
//-------------------------------------------
#pragma pack(push,1)
typedef struct{
  u16       KP_Speed;     
  u16       KI_Speed;
  u16       Speed_Setpoint;
  u16       KP_Flow;     
  u16       KI_Flow;
  u16        Flow_Setpoint;
  u16        Ti;
  u16        Te;
  u16        PEEP;
  u16        KP_Pressure;
  u16        KI_Pressure;
  u8        Control_mode;
}STRUCT_PACKET_RCVPWM;
typedef union{
  STRUCT_PACKET_RCVPWM     DATA;
  u8                       BYTE[sizeof(DATA)];
}UNION_PACKET_RCVPWM;
//----------------------
typedef struct{
  u8    Wp;
  u8    Rp;
  u8    Buffer[LEN_BUFFER_SEND2MB];
  u8  Flg_Sending;
}STRUCT_SEND_DATA;
//----------------------
#pragma pack(pop) //#pragma pack(pop)//#pragma pack(1)
//-------------------------------------------







//-------------------------------------------
void    RCV_data(u8 TempData);
void    Send_Data(u8 Packet);
//------------------------------------------------------------------------------
#endif /* __UART_H */
/*****************************END OF FILE**************************************/
