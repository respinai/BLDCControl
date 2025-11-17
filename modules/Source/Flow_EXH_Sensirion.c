//-----------------------------------------------------------------------------|
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI ASHTIYANI
// VER 1.0
// This File contains the Set up the flow sensor TSI company which contains :
// __the coefficients read from memory with I2C communication.
// __reading the analog value is output.
//-----------------------------------------------------------------------------|

#include "FLOW_Sensirion.h"
#include "Flow_TSI_1.h"

extern  void init_I2C(I2C_TypeDef* I2Cx);
//--------------------------------------
extern          STRUCT_DATA_A2DSENSORS          DATA_Sensors;
//--------------------------------------------------

u32                         I2C_Timeout;
//--------------------------------------------------

UNION_PACKET_U32_BYTE   Sensirion_SN;
UNION_PACKET_U16_BYTE   Sensirion_OFFset, Sensirion_SCALE;

//--------------------------------------------------

u8 EXH_DATACMD_Buffer[] = {0x10,0x00 ,0x30,0xDE, 0x30,0xDF,0x31,0xAE,0x20,0x00};
//                           datda      scale      offset      SN      Reset                
u8 EXH_DMARCV_Buffer_Data[2];
float EXHData;


#define     DISCONNET_EXH_CNTV        100
#define     CONNET_EXH_CNTV           20

u8      TimeDel , delaycnt;
//---------------------------------------------------
void Flow_SENSIRION_EXH_ReadDATA(void)
{
  static    u8  StateExhSensi = READ_DATA, CNT_ERR_Disconnect = 0, CNT_ERR_Connect = 0;
  u8        EXH_DATACMD_Buffer_scaleoffset[2] = {0};
  //float     Ksn;  
  
  //----.......
  switch(StateExhSensi)
  {
  case READ_DATA:   /*  01  */
    if((I2C_Receive(SENSIRION_EXH,0x81,EXH_DMARCV_Buffer_Data ,2))!= I2C_OK)
    {
      CNT_ERR_Disconnect++;
      if(CNT_ERR_Disconnect >= DISCONNET_EXH_CNTV)//100
      {
        DATA_Sensors.HWErr.EXHSENSOR = 1;
        DATA_Sensors.Flow_EXH = 0; // Reset data
        StateExhSensi = SENSOR_DETECT; //Go to CMD SCALE / Error Disconnection
        CNT_ERR_Disconnect = 0;
        CNT_ERR_Connect = 0;
      }
    }
    else
    {
      EXHData = (float)(EXH_DMARCV_Buffer_Data[0] *256 + EXH_DMARCV_Buffer_Data[1]);
      EXHData = (EXHData - Sensirion_OFFset.ALLBYTE)/Sensirion_SCALE.ALLBYTE;
      // Convert to BTPS
      //Ksn = ((float)DATA_Sensors.A2D_AmbientPressure / 10) + ((float)DATA_Sensors.Press_INH / 100 );
      //Ksn = ( PFixCOEFFICIENT1 / (Ksn - 64.0) ) * PFixCOEFFICIENT2;    
      //EXHData *= Ksn;
      if(EXHData < 0)
      {
        if(EXHData < -50)
          StateExhSensi = SENDCMD_DATA;
      }
      
      EXHData = EXHData <= 0 ? 0:EXHData;
      DATA_Sensors.Flow_EXH = (u16) (EXHData*100);
      
      if(DATA_Sensors.HWErr.EXHSENSOR == 1)
      {
        if(++CNT_ERR_Connect >= CONNET_EXH_CNTV)
        {
          DATA_Sensors.HWErr.EXHSENSOR = 0;
          DATA_Sensors.HWErr.EXHHARDWARE = 0;
          CNT_ERR_Connect = 0;
        }
      }
    }
    break;
    //----------------------
  case SENSOR_DETECT:   /*  0A  */
    TimeDel++;
    DATA_Sensors.HWErr.EXHSENSOR = 1;
    if(TimeDel == 10)
    {
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,ENABLE);
    }
    else if(TimeDel == 11)
    {
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1,DISABLE);
    }
    else if(TimeDel == 200)
    {       
      init_I2C(SENSIRION_EXH);
    }
    else if(TimeDel >= 250)
    {
      TimeDel=0;
      if(Check_Connect(SENSIRION_EXH, 0x80) == CONNECTOK50)
        StateExhSensi = SENDCMD_SCALE; //Go to CMD SCALE 
      else
      {
        I2C_Transmit(SENSIRION_EXH, 0x80, &EXH_DATACMD_Buffer[8], 2);
      }
    }    
    break;
    //----------------------
  case SENDCMD_SCALE: /*  5  */
    if((I2C_Transmit(SENSIRION_EXH, 0x80, &EXH_DATACMD_Buffer[2], 2)) != I2C_OK) // Scale
    {
      //Error  DISConnection / go to timer
      StateExhSensi = SENSOR_DETECT; // Back to DETECT 
    }
    else
    {
      StateExhSensi = READ_SCALE; //Go to Read SCALE
    }
    break;
    //----------------------
  case READ_SCALE:  /*  02  */
    if((I2C_Receive(SENSIRION_EXH,0x81,EXH_DATACMD_Buffer_scaleoffset ,2)) != I2C_OK)
    {
      StateExhSensi = SENSOR_DETECT; //Back to DETECT  
    }
    else
    {
      Sensirion_SCALE.Data.HIGHBYT = EXH_DATACMD_Buffer_scaleoffset[0];
      Sensirion_SCALE.Data.LOWBYT = EXH_DATACMD_Buffer_scaleoffset[1];
      StateExhSensi = SENDCMD_OFFSET; //Go to CMD OFFSET
    }
    break;
    //----------------------
  case SENDCMD_OFFSET:  /*  06  */
    if((I2C_Transmit(SENSIRION_EXH, 0x80, &EXH_DATACMD_Buffer[4], 2)) != I2C_OK) // OFFSET
    {
      //Error / go to timer
      StateExhSensi = SENSOR_DETECT; //Back to DETECT  
    }
    else
    {
      StateExhSensi = READ_OFFSET; //Go to Read OFFSET
    }
    break;
    //----------------------
  case READ_OFFSET: /*  03  */
    if((I2C_Receive(SENSIRION_EXH,0x81,EXH_DATACMD_Buffer_scaleoffset ,2)) != I2C_OK)
    {
      StateExhSensi = SENSOR_DETECT; //Back to DETECT 
    }
    else
    {
      Sensirion_OFFset.Data.HIGHBYT = EXH_DATACMD_Buffer_scaleoffset[0];
      Sensirion_OFFset.Data.LOWBYT = EXH_DATACMD_Buffer_scaleoffset[1];
      StateExhSensi = SENDCMD_DATA; //Go to CMD DATA
    }
    break;
    //----------------------
    //SN
    //----------------------
    //----------------------
  case SENDCMD_DATA:    /*  08  */
    if((I2C_Transmit(SENSIRION_EXH, 0x80, EXH_DATACMD_Buffer, 2)) != I2C_OK) // Request Data - 0x1000) 
    {
      //Error / go to timer
      StateExhSensi = SENSOR_DETECT; //Back to DETECT 
    }
    else
    {
      StateExhSensi = READ_DATA; //Go to Read OFFSET
    }
    break;  
  }//switch
  //----- 
  
  //DATA_Sensors.Flow_EXH =  150;
}
//---------------------------------------------------
void Flow_SENSIRION_EXH_ReadSN(void)
{
  u8 EXH_DATACMD_Buffer_scale[2] = {0};
  u8 EXH_DATACMD_Buffer_offset[2] = {0};
  u8 EXH_DATACMD_Buffer_SN[6] = {0};
  
  u8    ReRead = 0, i=0;
  u8    CheckData=0;
  for(ReRead=0;ReRead<10;ReRead++)
  {
    for(u8 wa=0;wa<250;wa++){}; // delay for WarmUp Time
    if((I2C_Transmit(SENSIRION_EXH, ADDRES_SENSIRION_WRITE, &EXH_DATACMD_Buffer[2], 2)) == I2C_OK) // Scale
    {
      ReRead = 20;
    }
    
  }
  DATA_Sensors.HWErr.EXHHARDWARE = FALSE;
  DATA_Sensors.HWErr.EXHSENSOR = FALSE;
  if(ReRead >= 20)//OK
  {
    CheckData=0;
    while(CheckData < 30)
    {
      //........1      
      if((I2C_Transmit(SENSIRION_EXH, ADDRES_SENSIRION_WRITE, &EXH_DATACMD_Buffer[2], 2)) == I2C_OK) // Scale
      {
        if((I2C_Receive(SENSIRION_EXH,ADDRES_SENSIRION_READ,EXH_DATACMD_Buffer_scale ,2)) == I2C_OK)
        {
          Sensirion_SCALE.Data.HIGHBYT = EXH_DATACMD_Buffer_scale[0];
          Sensirion_SCALE.Data.LOWBYT = EXH_DATACMD_Buffer_scale[1];
          if(Sensirion_SCALE.ALLBYTE > 10 && Sensirion_SCALE.ALLBYTE < 1000)
          {
            if((I2C_Transmit(SENSIRION_EXH, ADDRES_SENSIRION_WRITE, &EXH_DATACMD_Buffer[4], 2)) == I2C_OK) // Offset
            {
              if((I2C_Receive(SENSIRION_EXH,ADDRES_SENSIRION_READ,EXH_DATACMD_Buffer_offset ,2)) == I2C_OK)
              {
                Sensirion_OFFset.Data.HIGHBYT = EXH_DATACMD_Buffer_offset[0];
                Sensirion_OFFset.Data.LOWBYT = EXH_DATACMD_Buffer_offset[1];
                if(Sensirion_OFFset.ALLBYTE > 25000 && Sensirion_OFFset.ALLBYTE < 45000)
                {
                  if((I2C_Transmit(SENSIRION_EXH, ADDRES_SENSIRION_WRITE, &EXH_DATACMD_Buffer[6], 2)) == I2C_OK) // SN
                  {
                    if((I2C_Receive(SENSIRION_EXH,ADDRES_SENSIRION_READ,EXH_DATACMD_Buffer_SN ,6)) == I2C_OK)
                    {
                      Sensirion_SN.Data.BYTE_H = EXH_DATACMD_Buffer_SN[0];
                      Sensirion_SN.Data.BYTE_3 = EXH_DATACMD_Buffer_SN[1];
                      Sensirion_SN.Data.BYTE_2 = EXH_DATACMD_Buffer_SN[3];
                      Sensirion_SN.Data.BYTE_L = EXH_DATACMD_Buffer_SN[4];
                      for(i=0;i<5;i++)
                      {
                        I2C_Transmit(SENSIRION_EXH, ADDRES_SENSIRION_WRITE, EXH_DATACMD_Buffer, 2);// Request Data - 0x1000
                        for(u8 ag=0;ag<250;ag++){};//For Delay
                      }
                      CheckData = 100;
                    }
                  }
                }
              }
            }
          }
        }
      }
      for(u8 ag=0;ag<250;ag++){};//For Delay
      CheckData++;
    }//while 
    if(CheckData == 30)
      DATA_Sensors.HWErr.EXHHARDWARE = TRUE;//Error
  }
}
//----------------------------------------------------
I2C_StatusTypeDef    I2C_Receive(I2C_TypeDef* I2Cx, u8 address,u8* data, u16 count) 
{
  I2C_Timeout = I2C_TIMEOUT;
  /* Wait until BUSY flag is reset */
  while(I2Cx->SR2 & I2C_SR2_BUSY)
  {
    if(--I2C_Timeout == 0x00)
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C_TIMEout;  
    }
  }
  //--------------------
  /* Enable Acknowledge */
  I2Cx->CR1 |= I2C_CR1_ACK;   
  //------------
  /* Generate Start */
  I2Cx->CR1 |= I2C_CR1_START; 
  /* Wait until SB flag is set */
  I2C_Timeout = I2C_TIMEOUT;
  while(!(I2Cx->SR1 & I2C_SR1_SB)) 
  {
    if (--I2C_Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C_TIMEout;
    }      
  }
  ///-------- 
  /* Send address with zero last bit */
  I2Cx->DR = address;   
  /* Wait till finished */
  while (!(I2Cx->SR1 & I2C_SR1_ADDR)) 
  {
    if (--I2C_Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C_TIMEout;
    } 
  }
  /* Read status register to clear ADDR flag */
  I2Cx->SR1;
  I2Cx->SR2;
  //----------------------
  
  
  for(;count>0;count--)
  {
    if(count == 1)//end data
    {
      I2Cx->CR1 &= ~I2C_CR1_ACK;
    }
    
    /* Wait till not received */
    I2C_Timeout = I2C_TIMEOUT;
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)) 
    {
      if (--I2C_Timeout == 0x00) 
      {
        /* Generate Stop */
        I2Cx->CR1 |= I2C_CR1_STOP;
        return I2C_TIMEout;
      }
    }    
    /* Read data */
    *data = I2Cx->DR;  
    data++;
  }
  /* Generate Stop */
  I2Cx->CR1 |= I2C_CR1_STOP;
  /* Read status register to clear ADDR flag */
  I2Cx->SR1;
  I2Cx->SR2;
  //-----------------
  return I2C_OK;  
}
//----------------------------------------------------
I2C_StatusTypeDef    I2C_Transmit(I2C_TypeDef* I2Cx, u8 address,u8* data, u16 count) 
{
  I2C_Timeout = I2C_TIMEOUT;
  /* Wait until BUSY flag is reset */
  while(I2Cx->SR2 & I2C_SR2_BUSY)
  {
    if(--I2C_Timeout == 0x00)
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C_TIMEout;  
    }
  }
  //--------------------
  /* Generate Start */
  I2Cx->CR1 |= I2C_CR1_START; 
  /* Wait until SB flag is set */
  I2C_Timeout = I2C_TIMEOUT;
  while(!(I2Cx->SR1 & I2C_SR1_SB)) 
  {
    if (--I2C_Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C_TIMEout;
    }      
  }
  //--------------------
  I2C_Timeout = I2C_TIMEOUT;
  /* Send address with zero last bit */
  I2Cx->DR = address;   // address & write Bit
  /* Wait till finished */
  while (!(I2Cx->SR1 & I2C_SR1_ADDR)) 
  {
    if (--I2C_Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return I2C_TIMEout;
    } 
  }
  /* Read status register to clear ADDR flag */
  I2Cx->SR1;
  I2Cx->SR2;
  //----------------------
  /* Send data */
  for(u16 i=0;i<count;i++)
  {
    /* Wait until TXE flag is set */
    I2C_Timeout = I2C_TIMEOUT;
    while (!(I2Cx->SR1 & I2C_SR1_TXE)) 
    {
      if (--I2C_Timeout == 0x00) 
      {
        /* Generate Stop */
        I2Cx->CR1 |= I2C_CR1_STOP;
        return I2C_TIMEout;
      }
    }    
    /* Send I2C data */
    I2Cx->DR = *data;
    data++;
    /* Wait until BTF flag is set */
    I2C_Timeout = I2C_TIMEOUT;
    while (!(I2Cx->SR1 & I2C_SR1_BTF)) 
    {
      if (--I2C_Timeout == 0x00) 
      {
        /* Generate Stop */
        I2Cx->CR1 |= I2C_CR1_STOP;
        return I2C_TIMEout;
      }
    }    
  }
  //-------------- 
  /* Generate Stop */
  I2Cx->CR1 |= I2C_CR1_STOP;
  return I2C_OK;
}

//-------------------------------------------------------------------------
u8    Check_Connect (I2C_TypeDef* I2Cx , u8 address)
{
  I2C_Timeout = I2C_TIMEOUT;
  //--------------------
  /* Generate Start */
  I2Cx->CR1 |= I2C_CR1_START; 
  /* Wait until SB flag is set */
  I2C_Timeout = I2C_TIMEOUT;
  while(!(I2Cx->SR1 & I2C_SR1_SB)) 
  {
    if (--I2C_Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      return ERROR10;
    }      
  }
  //--------------------
  I2C_Timeout = I2C_TIMEOUT;
  /* Send address with zero last bit */
  I2Cx->DR = address;   // address & write Bit
  /* Wait till finished */
  while (!(I2Cx->SR1 & I2C_SR1_ADDR)) 
  {
    if (--I2C_Timeout == 0x00) 
    {
      /* Generate Stop */
      I2Cx->CR1 |= I2C_CR1_STOP;
      //data_timeout = 0xE1;
      return ERROR20;
    } 
  }
  /* Read status register to clear ADDR flag */
  I2Cx->SR1;
  I2Cx->SR2;
  
  /* Generate Stop */
  I2Cx->CR1 |= I2C_CR1_STOP;
  //----------------------
  return CONNECTOK50;
}
////----------------------------------------------------
//
//

