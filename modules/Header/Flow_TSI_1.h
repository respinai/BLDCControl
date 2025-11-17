//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : AZIZZADE , MOJTABA REZAEI 
// Date First Issued  : 1392/11/06
// VER 1.0
//------------------------------------------------------------------------------

#ifndef _SAADAT_FLOW_TSI_H
#define _SAADAT_FLOW_TSI_H

#include "main.h"


#define         EEPROM_ADDRESS_TSI              0xA0	
#define	        Flow_E2Prom_ADD_Len  	        86	
#define         TEMP_REF                        ((float) 21.11)
#define	        TSI_BUFFREADING_ROM_LEN		600
#define         CHECK_ADD		        60

#define         PVin                             5.00
#define         PVO                              3.4189
#define         PFixCOEFFICIENT1                 1033.228  // 
#define         PFixCOEFFICIENT2                 1.0539  // = (273.15 + 37) / (273.15 + 21.11)
#define         PTempCalib                       21.11

#pragma pack(1) 
typedef struct{
  float		C;
  float		B;
  float		A;
  float		V;
}Struct_VABC;

typedef struct{
  u16             Crc;
  u8              SN[6];
  u32             MN;
  u8              Rev;	
  //u8              Rev_;	
  u16             Year;
  u8              Month;
  //u8              Month_;
  u8              Day;
  //u8              Day_;
  float           Tcal;
  float           S;
  float           Z;
  float           S2;
  float           Z2;
  float           Tcorr;
  u8              CalAIR;
  //u8              CalAIR_;
  u16             LocationAIR;
  u8              CalO2;
  //u8              CalO2_;
  u16             LocationO2;
  u8              CalHLX;
  //u8              CalHLX_;
  u16             LocationHLX;
  u8              CalGAS;
  //u8              CalGAS_;
  u16             LocationGAS;
  
  u8              CoeffNUM;
  float           K0;
  float           K1;
  float           K2;
  float           K3;
  float           L0;
  float           L1;
  float           L2;
  float           L3;
  Struct_VABC     Coeff[13];
}Struct_Flow_ROM_data;

typedef struct{
  u16             CRC_calculation;
  u16             Index;
  u16             Len;
  u16             Len_cpy;
  u8              Flg_loop;
  u16		      cnt;
  u16		      Offset;
}Struct_Flow_ROM_Control;

typedef struct {
  Struct_Flow_ROM_data			Data;
  Struct_Flow_ROM_Control           Control;
}Struct_FlowSensor_EEPROM_Table;
  
    
#pragma pack()


//-------------------------------------------------------------------------------
void Flow_INH_Service(u8 type);
void Flow_TSI_Initialize (I2C_TypeDef* I2Cx);

FunctionalState      I2C_CheckConnect(I2C_TypeDef* I2Cx, u8 address);

void  ROM_init(void);
float  flow_calculation (u8 type);
float EXHflow_calculation (s16 DataFlow);
float temp_Convert(u16);
//-----
void CRC_Calculation (u8 NewData);
void I2C_EE_BufferRead(I2C_TypeDef* I2Cx, u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);
//-------------------------------------------------------------------------------
#endif	// _SAADAT_FLOW_TSI_H

