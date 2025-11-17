//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : AZIZZADE , MOJTABA REZAEI 
// Date First Issued  : 1392/11/06
// VER 1.0
//------------------------------------------------------------------------------

#ifndef _SAADAT_FLOW_SENSIRION_INH_H
#define _SAADAT_FLOW_SENSIRION_INH_H

#include "main.h"


#define         EEPROM_ADDRESS_TSI              0xA0	
#define	        Flow_E2Prom_ADD_Len  	        86	
#define         TEMP_REF                        ((float) 21.11)
#define	        SENSIRION_BUFFREADING_ROM_LEN	500
#define         CHECK_ADD		                60

#define         PVin                             5.00
#define         PVO                              3.4189
#define         PFixCOEFFICIENT1                 1033.228  // 
#define         PFixCOEFFICIENT2                 1.0539  // = (273.15 + 37) / (273.15 + 21.11)
#define         PTempCalib                       21.11

#pragma pack(1) 

//----------------------------
typedef struct{
    u8          Product_number_a;   /* 2        */  //  0xA002
    u8          Product_number_b[4];   /* 6,5,4,3  */  //  0XA006 0XA005 0XA004 0XA003 
    u8          Product_number_c;   /* 7        */  //  0XA007
    u8          Serial_number[8];   /* 10 .. 17 */  //  0XA00A 0XA00B 0XA00C 0XA00D 0XA00E 0XA00F 0XA010 0XA011
    s16         Alpha;              /* 257,256  */  //  0xA101 0xA100
    s16         Beta;               /* 259,258  */  //  0xA103 0xA102
    s16         Gama;               /* 261,260  */  //  0xA105 0xA104
    u16         SizeOf;             /* 263,262  */  //  0xA107 0xA106
    s16         Q_F_o2[40][2];      /* 264..422 */  //  0xA109 0xA108 ... 0xA1A7 0xA1A6
    s16         B;                  /* 425,424  */  //  0xA1A9 0xA1A8
    s16         A;                  /* 427,426  */  //  0xA1AB 0xA1AA
    s16         Offset;             /* 429,428  */  //  0xA1AD 0xA1AC 
}Struct_Sensirion_ROM_data;

typedef union{
   Struct_Sensirion_ROM_data    Data;
   u8                           BYTE[sizeof(Data)];
}UNION_Sensirion_ROM_data;
//---------------------------------------------------
#pragma pack()
//---------------------------------------------------

typedef enum 
{
  I2C__Emty          = 0x00U,   
  I2C__OK            = 0x10U,
  I2C__DisConnect    = 0x01U,
  I2C__BUSY          = 0x02U,
  I2C__TIMEout       = 0x03U
} I2C_StatusTypeDefINH;

//-------------------------------------------------------------------------------
void Flow_INH_Service(u8 type);
void Flow_INH_ReadCoeff (I2C_TypeDef* I2Cx);

FunctionalState      I2C_CheckConnect(I2C_TypeDef* I2Cx, u8 address);

void  ROM_init(void);
float  flow_calculation (u8 type);
u16 flow_calculation_Sensirion (u8 type);
float EXHflow_calculation (s16 DataFlow);
float temp_Convert(u16);
u8  INH_SensorDetection(u8* ReadData);
//-----
void CRC_Calculation (u8 NewData);
void I2C_EE_BufferRead(I2C_TypeDef* I2Cx, u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);
//-------------------------------------------------------------------------------
#endif	// _SAADAT_FLOW_SENSIRION_INH_H

