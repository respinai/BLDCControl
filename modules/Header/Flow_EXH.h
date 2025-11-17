//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : AZIZZADE , MOJTABA REZAEI 
// Date First Issued  : 1392/11/06
// VER 1.0
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef _SAADAT_FLOWEXH_H
#define _SAADAT_FLOWEXH_H
//------------------------------------------------------------------------------
//GENERAL DEFINE
#include "main.h"

#define         CLEAR0      0x00
#define         Delay_     5

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------
typedef struct{
   u8   ReqNormal       :1;     // BIT 0 1
   u8   Reqzero         :1;     // BIT 1 2
   u8   Reqstatus       :1;     // BIT 2 4
   u8   ReqClean        :1;     // BIT 3 8
   u8   b               :1;     // BIT 4
   u8   c               :1;     // BIT 5
   u8   d               :1;     // BIT 6
   u8   e               :1;     // BIT 7
}REQ_EXHFLOW_COMMUNICATION_FLG;
typedef struct{
   u8   RCVOK               :1;     // BIT 0
   u8   RCVErr              :1;     // BIT 1
   u8   BAD_CMD             :1;     // BIT 2
   u8   RcvStatOK           :1;     // BIT 3
   u8   RcvZeroOK           :1;     // BIT 4
   u8   RcvNormOK           :1;     // BIT 5
   u8   ReplyNorm2          :1;     // BIT 6
   u8   ReplyNorm1          :1;     // BIT 7
}REPLY_EXHFLOW_COMMUNICATION_FLG;
typedef union{
   REQ_EXHFLOW_COMMUNICATION_FLG        Data;   
   u8                                   BYTE;
}UNION_REQ_EXHFLOW_COMMUNICATION_FLG;
typedef union{
   REPLY_EXHFLOW_COMMUNICATION_FLG      Data;  
   u8                                   BYTE;
}UNION_REPLY_EXHFLOW_COMMUNICATION_FLG;

//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------
// *****************************    ****************************************|
//-----------------------------------------------------------------------------|
//------------------------------------------------------------------------------
/*
|------------------------------------------------------------------------------|
|           ***		DEFINE		***														   |
|------------------------------------------------------------------------------|
*/

void  Req_ZeroingMB(FunctionalState Tyep);
void  EXH_RCV_Data( u8 data);
void  Start_Read_EXHflow ( void );

//-------------------------------------------------------------------------------
#endif	// _SAADAT_FLOWEXH_H
