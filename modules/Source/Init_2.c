//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
//------------------------------------------------------------------------------|
#include "Init_2.h"
#include "main.h"
//------------------------------------------------------------------------------|
GPIO_InitTypeDef			GPIO_InitStructure;
USART_InitTypeDef			USART_InitStruct;
NVIC_InitTypeDef			NVIC_InitStructure ;
EXTI_InitTypeDef            EXTI_InitStruct;
ADC_InitTypeDef             ADC_InitStruct;
SPI_InitTypeDef			    SPI_InitStruct;
I2C_InitTypeDef 			I2C_InitStruct;


extern void Flow_SENSIRION_EXH_ReadSN(void);
//------------------------------------------------------------------------------
/*   Configuration  Priority Interrupt      
  ______________________________________________________________________________
  InitType              PreemptionPriority      SubPriority     Comment        
  ------------------------------------------------------------------------------
1)EXTI                          0                    0             External IT [0,6,7]       
2)Timer2                        0                    1             Main Timer                                                                                   
3)USART1                        0                    1             Serial MotherBoard   

-)ADC                           -                    -             A2D [ch10 ~ ch15]        
-)Timer5                        -                    -             Speed Time [11.904 nsec] 



*/
//------------------------------------------------------------------------------
void Init (void)
{
  u32   TimeDelayCNT = 0;
  
  for(TimeDelayCNT=0;TimeDelayCNT<600000;TimeDelayCNT++){};// ~ 25ms
  //
  u16 Blink_CNT, Blink_DataTim[BLINK_INDEX] = {1500, 1000, 1500, 1000 , 1500, 6000, 1200, 1000, 1500 };
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);  
  //    USART 
  USART_ConfigPeriph(MAINBOARD);
  //	SPI
  SPI_ConfigPeriph(PRESS_INH);
  //    GPIO
  GPIO_ConfigPort(GPIOA);
  GPIO_ConfigPort(GPIOB);
  GPIO_ConfigPort(GPIOC);
  GPIO_ConfigPort(GPIOD);
  GPIO_ConfigPort(GPIOE);
//  
  ExtIT_ConfigPort();
  //
  init_I2C(SENSIRION_EXH);
  Flow_SENSIRION_EXH_ReadSN();
//
  ADC1_ConfigPeriph();
  //    Algorithm
  Algorithm_Init();     
  //    TIMER
  TIM_ConfigPort();
  
  //    LED Blink  
  for(u8 bi=0;bi<BLINK_INDEX-4;bi++)
  {
    Blink_CNT = FREE_CNT_100us;
    if(bi%2)
    {
      GPIO_ResetBits(LED2_PORT, LED2_PIN);
      GPIO_ToggleBits(LED1_PORT, LED1_PIN);
      GPIO_ToggleBits(LED3_PORT, LED3_PIN);
    }
    else
    {
      GPIO_SetBits(LED2_PORT, LED2_PIN);
      GPIO_ToggleBits(LED1_PORT, LED1_PIN);
      GPIO_ToggleBits(LED3_PORT, LED3_PIN);
    }
    while(( Blink_CNT + Blink_DataTim[bi] )>= FREE_CNT_100us );//200ms = 200000uSec
  } 
  //---------------------------------
  GPIO_ResetBits(LED1_PORT, LED1_PIN);
  GPIO_ResetBits(LED2_PORT, LED2_PIN);
  GPIO_ResetBits(LED3_PORT, LED3_PIN);
  //--------------------------------------------------------------------------------  
  //    Watchdog
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(0x0ABC);     //must be a number between 0 and 0x0FFF.
  IWDG_Enable();              //write 0xCCCC to KR register bit  
}
//------------------------------------------------------------------------------
/*================================ Source Code Functions ===========================*/
//----------------
  //----------------------------
//--------------------
/*
|------------------------------------------------------------------------------|
|		***		EXTERNAL IT 		***															   |
|------------------------------------------------------------------------------|
*/
void ADC1_ConfigPeriph (void)    //(ADC_TypeDef* ADCx) // ADC1
{
  ADC_CommonInitTypeDef     ADC_CommonInitStruct;
  /* Enable the ADC interface clock using */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 

  /* Set common ADC settings */
  ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_8Cycles;
  ADC_CommonInit(&ADC_CommonInitStruct);
  /* Init ADC settings */
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;//ADC_ExternalTrigConv_T2_CC2;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_NbrOfConversion = 5; 
  ADC_Init(ADC1, &ADC_InitStruct);
  /* Enable ADC */
  ADC_Cmd(ADC1,ENABLE);
  
  
  
  
  
  
  ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_15Cycles);
  ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_15Cycles);
  ADC_RegularChannelConfig(ADC1,ADC_Channel_12,1,ADC_SampleTime_15Cycles);
  ADC_RegularChannelConfig(ADC1,ADC_Channel_13,1,ADC_SampleTime_15Cycles);
  //ADC_RegularChannelConfig(ADC1,ADC_Channel_14,1,ADC_SampleTime_15Cycles);
  ADC_RegularChannelConfig(ADC1,ADC_Channel_15,1,ADC_SampleTime_15Cycles);
    
    //ADC_SoftwareStartConv(ADC1);
  //ADC_GetConversionValue(ADC1);
}
/*
|------------------------------------------------------------------------------|
|		***		EXTERNAL IT 		***															   |
|------------------------------------------------------------------------------|
*/
void ExtIT_ConfigPort (void) //GPIO_TypeDef* GPIOx)
{
  /* Enable clock for GPIOD */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  /* Enable clock for SYSCFG */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Tell system that you will use PA6,7 for EXTI_Line6,7 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6); 
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);
    
    /* PA6,7 is connected to EXTI_Line6,7 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line6 | EXTI_Line7;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on rising and falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);
 
    /* Add IRQ vector to NVIC */
    /* PA6,7 is connected to EXTI_Line6,7 , which has EXTI9_5_IRQn vector */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    /* Set priority */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    /* Enable interrupt */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStructure);
  //-----------------------------------
    /* Tell system that you will use PB0 for EXTI_Line0 */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0); 
    
    /* PB0 is connected to EXTI_Line0 */
    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    /* Enable interrupt */
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    /* Interrupt mode */
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    /* Triggers on rising and falling edge */
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* Add to EXTI */
    EXTI_Init(&EXTI_InitStruct);
 
    /* Add IRQ vector to NVIC */
    /* PB0 is connected to EXTI_Line0 , which has EXTI0_IRQn vector */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    /* Set priority */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    /* Set sub priority */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    /* Enable interrupt */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* Add to NVIC */
    NVIC_Init(&NVIC_InitStructure);
}
  

/*
|------------------------------------------------------------------------------|
|		***		GPIO		***															   |
|------------------------------------------------------------------------------|
*/
void GPIO_ConfigPort (GPIO_TypeDef* GPIOx)
{
  u8 	Port_Num,Pin_Num,Temp;
  if(GPIOx==GPIOA)
  {
    Port_Num=0;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  }
  else if(GPIOx==GPIOB)
  {
    Port_Num=1;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  }
  else if(GPIOx==GPIOC)
  {
    Port_Num=2;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  }
  else if(GPIOx==GPIOD)
  {
    Port_Num=3;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  }
  else if(GPIOx==GPIOE)
  {
    Port_Num=4;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  }
  else
    return;
  for(Pin_Num=0; Pin_Num < STM32F4xx_PIN_NUMBER; Pin_Num++)
  {
    Temp = STM32F4_PortConfig[Port_Num][Pin_Num].Pin_default;
    if(Temp != NuLL)
    {
      GPIO_InitStructure.GPIO_Pin 	= STM32F4_PortConfig[Port_Num][Pin_Num].Pin_number;
      GPIO_InitStructure.GPIO_Mode 	= STM32F4_PortConfig[Port_Num][Pin_Num].Pin_directionmode;
      GPIO_InitStructure.GPIO_OType = STM32F4_PortConfig[Port_Num][Pin_Num].Pin_outputtype;
      GPIO_InitStructure.GPIO_Speed = STM32F4_PortConfig[Port_Num][Pin_Num].Pin_outputspeed;
      GPIO_InitStructure.GPIO_PuPd 	= STM32F4_PortConfig[Port_Num][Pin_Num].Pin_pullupdown;
      GPIO_Init(GPIOx, &GPIO_InitStructure);
      
      if(GPIO_InitStructure.GPIO_Mode == GPIO_Mode_AF)
      {
        GPIO_PinAFConfig(GPIOx , STM32F4_PortConfig[Port_Num][Pin_Num].Pin_sources , STM32F4_PortConfig[Port_Num][Pin_Num].GPIO_AF);
      }
      else 
      {
        if(Temp==1)
          GPIOx->BSRRL |=((uint16_t)1<< Pin_Num);       //Set   Pin
        else
          GPIOx->BSRRH |=((uint16_t)1<< Pin_Num);	//Reset Pin
      }
    }
  }
}
/*
|------------------------------------------------------------------------------|
|		***		USART		***															   |
|------------------------------------------------------------------------------|
*/
void USART_ConfigPeriph(USART_TypeDef* USARTx)
{	
  uint8_t 	Usart_Num;
  if(USARTx==USART1)
    Usart_Num=0;
  else if(USARTx==USART2)
    Usart_Num=1;
  else if(USARTx==USART3)
    Usart_Num=2;
  else
    return;
  if( STM32F4_USARTConfig[Usart_Num].uart_default == ENABLE)
  {
    //Enable peripheral clock
    if(Usart_Num==0)
      RCC_APB2PeriphClockCmd(STM32F4_USARTConfig[Usart_Num].RCC_APB_Periph , ENABLE);
    else
      RCC_APB1PeriphClockCmd(STM32F4_USARTConfig[Usart_Num].RCC_APB_Periph , ENABLE);
    // Programm Configure USART
    USART_InitStruct.USART_BaudRate = STM32F4_USARTConfig[Usart_Num].BaudRate ;
    USART_InitStruct.USART_WordLength = STM32F4_USARTConfig[Usart_Num].WordLength ;
    USART_InitStruct.USART_StopBits = STM32F4_USARTConfig[Usart_Num].StopBits ;
    USART_InitStruct.USART_Parity = STM32F4_USARTConfig[Usart_Num].Parity ;
    USART_InitStruct.USART_Mode = STM32F4_USARTConfig[Usart_Num].Mode ;
    USART_InitStruct.USART_HardwareFlowControl = STM32F4_USARTConfig[Usart_Num].HardwareFlowControl ;
    USART_Init(USARTx , &USART_InitStruct);
    //Enable interrupt
    if(STM32F4_USARTConfig[Usart_Num].Interrupt_EN == ENABLE)
    {
      USART_ITConfig(USARTx , STM32F4_USARTConfig[Usart_Num].Source_IT , ENABLE );
      //Set Interrupt Routine
      if(Usart_Num==0)
      {                         /*--- Serial MB ---*/
         NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn ;	
         NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5 ;
         NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 ;
         NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
         NVIC_Init(&NVIC_InitStructure) ;//
      }
      else if(Usart_Num==1)
      {                         /*--- Flow EXH ---*/
         NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn ;	
         NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6 ;
         NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 ;
         NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
         NVIC_Init(&NVIC_InitStructure) ;//
      }
      else if(Usart_Num==2)
      {                         /*--- Viewer  ---*/
         NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn ;	
         NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7 ;
         NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0 ;
         NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
         NVIC_Init(&NVIC_InitStructure) ;//
      }
    }
    // ACTIVE USART 
    USART_Cmd(USARTx , ENABLE);
  }
}

//-----------------------------------------
/*
|------------------------------------------------------------------------------|
|		***		 SPI  	***		  												   |
|------------------------------------------------------------------------------|
*/
void SPI_ConfigPeriph(SPI_TypeDef* SPIx)
{
  uint8_t 	Spi_Num;
  if(SPIx==SPI2)
    Spi_Num=0;
  else
    return;
  if( STM32F4_SPIConfig[Spi_Num].spi_default == ENABLE)
  {
    //Enable peripheral clock

    RCC_APB1PeriphClockCmd(STM32F4_SPIConfig[Spi_Num].RCC_APB_Periph , ENABLE);
    // Programm Configure SPI
    SPI_InitStruct.SPI_BaudRatePrescaler = STM32F4_SPIConfig[Spi_Num].BaudRate ;
    SPI_InitStruct.SPI_Mode = STM32F4_SPIConfig[Spi_Num].Mode ;
    SPI_InitStruct.SPI_Direction = STM32F4_SPIConfig[Spi_Num].Direction ;
    SPI_InitStruct.SPI_DataSize = STM32F4_SPIConfig[Spi_Num].DataSize ;
    SPI_InitStruct.SPI_CPOL= STM32F4_SPIConfig[Spi_Num].CPOL ;
    SPI_InitStruct.SPI_CPHA = STM32F4_SPIConfig[Spi_Num].CPHA ;
    SPI_InitStruct.SPI_NSS = STM32F4_SPIConfig[Spi_Num].NSS ;
    SPI_InitStruct.SPI_FirstBit = STM32F4_SPIConfig[Spi_Num].FirstBit ;
    SPI_InitStruct.SPI_CRCPolynomial = STM32F4_SPIConfig[Spi_Num].CRCPolynomial ;
    SPI_Init(SPIx , &SPI_InitStruct);
    //Enable interrupt
    if(STM32F4_SPIConfig[Spi_Num].Interrupt_EN == ENABLE)
    {
      SPI_I2S_ITConfig(SPIx , STM32F4_SPIConfig[Spi_Num].Source_IT , ENABLE );
      //Set Interrupt Routine
                         /*--- A2D ---*/
         NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn ;	
         NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
         NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2 ;
         NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
         NVIC_Init(&NVIC_InitStructure) ;//
          
    }
    // ACTIVE SPI 
    SPI_Cmd(SPIx , ENABLE);
  }
}
//-------------------------------------------------------------------
/*
|------------------------------------------------------------------------------|
|     	***		 I2C		***															   |
|------------------------------------------------------------------------------|
*/
/* This funcion initializes the I2C1 peripheral */
void init_I2C(I2C_TypeDef* I2Cx)
{
  if(I2Cx == I2C1)	
    /* enable APB1 peripheral clock for I2C1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  else if (I2Cx == I2C2)	
    /* enable APB1 peripheral clock for I2C1 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  else
    return; // Error Input 
  /* Set the I2C structure parameters */
  I2C_InitStruct.I2C_ClockSpeed = 200000;		// 200kHz
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;       // 50% duty cycle --> standard
  I2C_InitStruct.I2C_OwnAddress1 = 0;//0x40A0;							// 1010 0000
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable; //I2C_Ack_Disable;  I2C_Ack_Enable
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  
  /* Initialize the I2C peripheral w/ selected parameters */
  I2C_Init(I2Cx, &I2C_InitStruct);
  
  /* Enable the I2C peripheral */
  I2C_Cmd(I2Cx, ENABLE);
}
//-----------------------------------------

