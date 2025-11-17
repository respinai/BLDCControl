//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
// 
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef _SAADAT_I2C_H
#define _SAADAT_I2C_H

#include "main.h"

//------------------------------------------------
/* Private defines */
#define I2C_TRANSMITTER_MODE   0
#define I2C_RECEIVER_MODE      1
#define I2C_ACK_ENABLE         1
#define I2C_ACK_DISABLE        0
#ifndef I2C_TIMEOUT
#define I2C_TIMEOUT          3000 // ~ 400uSec
#endif
//
#define ADDRES_SENSIRION_READ          0x81
#define ADDRES_SENSIRION_WRITE         0x80

#define ERROR10         10
#define ERROR20         20
#define CONNECTOK50     50

//-------------------------------------------------
typedef enum 
{
  I2C_OK            = 0x00U,
  I2C_DisConnect    = 0x01U,
  I2C_BUSY          = 0x02U,
  I2C_TIMEout       = 0x03U
} I2C_StatusTypeDef;
//---------------------
typedef enum 
{
  READ_DATA             = 0x01U,
  READ_SCALE            = 0x02U,
  READ_OFFSET           = 0x03U,
  READ_SN               = 0x04U,
  SENDCMD_SCALE         = 0x05U,
  SENDCMD_OFFSET        = 0x06U,
  SENDCMD_SN            = 0x07U,
  SENDCMD_DATA          = 0x08U,
  SENDCMD_TEMP          = 0x09U,
  SENSOR_DETECT         = 0x0AU,
  SENSOR_DELAYON        = 0x0BU, 
  
  //---------
  Endenumexh            
} STATE_READ_StatusTypeDef;

//------------------------------------------------

extern FunctionalState         I2C_Stop(I2C_TypeDef* I2Cx);
//--------------------------------------------------

#pragma pack(1) 

typedef struct{
  u8                LOWBYT;
  u8                HIGHBYT;
}Struct_PACKET_U16_BYTE;
typedef union
{
  Struct_PACKET_U16_BYTE    Data;
  u16                       ALLBYTE;
}UNION_PACKET_U16_BYTE;
//----------------------------------------------------
typedef struct{
  u8                BYTE_L;
  u8                BYTE_2;
  u8                BYTE_3;
  u8                BYTE_H;
}Struct_PACKET_U32_BYTE;
typedef union
{
  Struct_PACKET_U32_BYTE    Data;
  u32                       ALLBYTE;
}UNION_PACKET_U32_BYTE;
//--------------------------------------------------------

    
#pragma pack()

//---------------------------------------------
u8    Check_Connect (I2C_TypeDef* I2Cx, u8 address);
void Flow_SENSIRION_EXH_ReadSN(void);
void Flow_SENSIRION_EXH_ReadDATA(void);
I2C_StatusTypeDef    I2C_Receive(I2C_TypeDef* I2Cx, u8 address,u8* data, u16 count);
I2C_StatusTypeDef    I2C_Transmit(I2C_TypeDef* I2Cx, u8 address,u8* data, u16 count);
//-------------------------------------------------------------------------------
#endif	// _SAADAT_I2C_H

