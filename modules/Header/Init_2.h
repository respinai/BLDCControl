//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
//------------------------------------------------------------------------------

#ifndef _SAADAT_INIT_H
#define _SAADAT_INIT_H
//------------------------------------------------------------------------------
#define         BLINK_INDEX             9//9
//------------------------------------------------------------------------------
#include "main.h"
//------------------------------------------------------------------------------

const Pin_Struct STM32F4_PortConfig[STM32F4xx_PORTS][STM32F4xx_PIN_NUMBER]=
{
  //{    /*  PORT A CONFIG  */
  // Name,Default,Direction Mode, Output Type  ,  Output Speed    ,Pin_pullupdown   ,  GPIO_AF     , 	  Number  ,  Pin_sources   ,	    Comment
  //.............................................................................................................................................
  //	|  ,  1    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_2MHz   ,GPIO_PuPd_NOPULL ,		   ,            ,                ,                                 
  //	|  ,  0    ,GPIO_Mode_OUT ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_UP     ,		   ,            ,                ,
  //	|  , NuLL  ,GPIO_Mode_AF  ,--------      ,GPIO_Speed_50MHz  ,GPIO_PuPd_DOWN   ,		   ,            ,                ,
  //	|  , ----  ,GPIO_Mode_AN  ,--------      ,GPIO_Speed_100MHz ,  --------       ,		   ,            ,                ,
  //...............................................................................................................................................
  "PA0"  ,  0    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_2MHz   ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN ,GPIO_Pin_0  ,GPIO_PinSource0 ,  //NO USED
  "PA1"  ,  0    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN ,GPIO_Pin_1  ,GPIO_PinSource1 ,	//NO USED
  "PA2"  ,  0    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN ,GPIO_Pin_2  ,GPIO_PinSource2 ,	//NO USED
  
  "PA3"  ,  0    ,GPIO_Mode_OUT ,GPIO_OType_PP , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_OUT,GPIO_Pin_3  ,GPIO_PinSource3 ,	//M-P 
  
  "PA4"  ,  0    ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_2MHz   ,GPIO_PuPd_DOWN   ,GPIO_Mode_IN  ,GPIO_Pin_4  ,GPIO_PinSource4 ,	//NO USED
  "PA5"  ,  0    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_5  ,GPIO_PinSource5 ,	//NO USED
  
  "PA6"  ,  0    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_100MHz ,GPIO_PuPd_UP     ,GPIO_Mode_IN  ,GPIO_Pin_6  ,GPIO_PinSource6 ,	//EXIT-HALL 1
  "PA7"  ,  0    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_100MHz ,GPIO_PuPd_UP     ,GPIO_Mode_IN  ,GPIO_Pin_7  ,GPIO_PinSource7 ,	//EXIT-HALL 2
  
  "PA8"  ,  0    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_UP     ,GPIO_Mode_IN  ,GPIO_Pin_8  ,GPIO_PinSource8 ,	//NO USED
  
  "PA9"  ,  1    ,GPIO_Mode_AF  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_AF_USART1,GPIO_Pin_9  ,GPIO_PinSource9 ,	//TX1_MainBoard/USART1
  "PA10" ,  1    ,GPIO_Mode_AF  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_AF_USART1,GPIO_Pin_10 ,GPIO_PinSource10,	//RX1_MainBoard/USART1
  
  "PA11" ,  0    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_11 ,GPIO_PinSource11,	//NO USED
  "PA12" ,  0    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_12 ,GPIO_PinSource12,	//NO USED
  "PA13" ,NuLL   ,GPIO_Mode_AF  ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_AF_SWJ   ,GPIO_Pin_13 ,GPIO_PinSource13,	//JTDI/JTAG
  "PA14" ,NuLL   ,GPIO_Mode_AF  ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_AF_SWJ   ,GPIO_Pin_14 ,GPIO_PinSource14,	//JTCK/JTAG
  "PA15" ,NuLL   ,GPIO_Mode_AF  ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_AF_SWJ   ,GPIO_Pin_15 ,GPIO_PinSource15,	//JTMS/JTAG
  //}
  //{    /*  PORT B CONFIG  */
  
  // Name,Default,Direction Mode, Output Type  ,  Output Speed    ,Pin_pullupdown   ,  GPIO_AF     , 	  Number  ,  Pin_sources   ,	    Comment
  //.............................................................................................................................................
  //	|  ,  1    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_2MHz   ,GPIO_PuPd_NOPULL ,		   ,            ,                ,                                 
  //	|  ,  0    ,GPIO_Mode_OUT ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_UP     ,		   ,            ,                ,
  //	|  , NuLL  ,GPIO_Mode_AF  ,--------      ,GPIO_Speed_50MHz  ,GPIO_PuPd_DOWN   ,		   ,            ,                ,
  //	|  , ----  ,GPIO_Mode_AN  ,--------      ,GPIO_Speed_100MHz ,  --------       ,		   ,            ,                ,
  //...............................................................................................................................................
  "PB0"  , 0     ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_100MHz ,GPIO_PuPd_UP     ,GPIO_Mode_IN  ,GPIO_Pin_0  ,GPIO_PinSource0 ,	//EXIT-HALL 3
  
  "PB1"  , 0     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_1  ,GPIO_PinSource1 ,	//NO USED
  "PB2"  , 0     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_DOWN   ,GPIO_Mode_IN  ,GPIO_Pin_2  ,GPIO_PinSource2 ,	//NO USED
  "PB3"  , NuLL  ,GPIO_Mode_AF  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_SWJ   ,GPIO_Pin_3  ,GPIO_PinSource3 ,	//JTDO/JTAG
  "PB4"  , NuLL  ,GPIO_Mode_AF  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_SWJ   ,GPIO_Pin_4  ,GPIO_PinSource4 ,	//NTRST/JTAG
  "PB5"  , 0     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_DOWN   ,GPIO_Mode_IN  ,GPIO_Pin_5  ,GPIO_PinSource5 ,	//NO USED
  "PB6"  , 0     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_DOWN   ,GPIO_Mode_IN  ,GPIO_Pin_6  ,GPIO_PinSource6 ,	//NO USED
  "PB7"  , 0     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_DOWN   ,GPIO_Mode_IN  ,GPIO_Pin_7  ,GPIO_PinSource7 ,	//NO USED
  "PB8"  , 0     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_8  ,GPIO_PinSource8 ,	//NO USED
  "PB9"  , 0     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_9  ,GPIO_PinSource9 ,	//NO USED

  "PB10" , 1     ,GPIO_Mode_AF  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_I2C2  ,GPIO_Pin_10 ,GPIO_PinSource10,	//I2C SCL
  "PB11" , 1     ,GPIO_Mode_AF  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_I2C2  ,GPIO_Pin_11 ,GPIO_PinSource11,	//I2C SDA

  "PB12" , 1     ,GPIO_Mode_OUT ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_OUT ,GPIO_Pin_12 ,GPIO_PinSource12,	//CS SPI2
  "PB13" , 1     ,GPIO_Mode_AF  ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_SPI2  ,GPIO_Pin_13 ,GPIO_PinSource13,	//NO SPI2
  "PB14" , 1     ,GPIO_Mode_AF  ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_SPI2  ,GPIO_Pin_14 ,GPIO_PinSource14,	//NO SPI2
  "PB15" , 1     ,GPIO_Mode_AF  ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_SPI2  ,GPIO_Pin_15 ,GPIO_PinSource15,	//NO SPI2
  
   //}
  //{    /*  PORT C CONFIG  */

  // Name,Default,Direction Mode, Output Type  ,  Output Speed    ,Pin_pullupdown   ,  GPIO_AF     , 	  Number  ,  Pin_sources   ,	    Comment
  //.............................................................................................................................................
  //	|  ,  1    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_2MHz   ,GPIO_PuPd_NOPULL ,		   ,            ,                ,                                 
  //	|  ,  0    ,GPIO_Mode_OUT ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_UP     ,		   ,            ,                ,
  //	|  , NuLL  ,GPIO_Mode_AF  ,--------      ,GPIO_Speed_50MHz  ,GPIO_PuPd_DOWN   ,		   ,            ,                ,
  //	|  , ----  ,GPIO_Mode_AN  ,--------      ,GPIO_Speed_100MHz ,  --------       ,		   ,            ,                ,
  //...............................................................................................................................................
  "PC0"  , 1     ,GPIO_Mode_AN  ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_AN  ,GPIO_Pin_0  ,GPIO_PinSource0 ,	//ADC123-IN10
  "PC1"  , 1     ,GPIO_Mode_AN  ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_AN  ,GPIO_Pin_1  ,GPIO_PinSource1 ,	//ADC123-IN11
  "PC2"  , 1     ,GPIO_Mode_AN  ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_AN  ,GPIO_Pin_2  ,GPIO_PinSource2 ,	//ADC123-IN12
  "PC3"  , 1     ,GPIO_Mode_AN  ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_AN  ,GPIO_Pin_3  ,GPIO_PinSource3 ,	//ADC123-IN13
  "PC4"  , 1     ,GPIO_Mode_AN  ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_AN  ,GPIO_Pin_4  ,GPIO_PinSource4 ,	//ADC12 -IN14
  "PC5"  , 1     ,GPIO_Mode_AN  ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_AN  ,GPIO_Pin_5  ,GPIO_PinSource5 ,	//ADC12 -IN15
  
  "PC6"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_6  ,GPIO_PinSource6 ,	//NO USED
  "PC7"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_7  ,GPIO_PinSource7 ,	//NO USED
  "PC8"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_8  ,GPIO_PinSource8 ,	//NO USED
  "PC9"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_9  ,GPIO_PinSource9 ,	//NO USED
  "PC10" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_10 ,GPIO_PinSource10,	//NO USED
  "PC11" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_11 ,GPIO_PinSource11,	//NO USED
  "PC12" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_12 ,GPIO_PinSource12,	//NO USED
  "PC13" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_13 ,GPIO_PinSource13,	//NO USED
  "PC14" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_14 ,GPIO_PinSource14,	//NO USED
  "PC15" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_15 ,GPIO_PinSource15,	//NO USED
  
   //}
  //{    /*  PORT D CONFIG  */

  // Name,Default,Direction Mode, Output Type  ,  Output Speed    ,Pin_pullupdown   ,  GPIO_AF     , 	  Number  ,  Pin_sources   ,	    Comment
  //.............................................................................................................................................
  //	|  ,  1    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_2MHz   ,GPIO_PuPd_NOPULL ,		   ,            ,                ,                                 
  //	|  ,  0    ,GPIO_Mode_OUT ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_UP     ,		   ,            ,                ,
  //	|  , NuLL  ,GPIO_Mode_AF  ,--------      ,GPIO_Speed_50MHz  ,GPIO_PuPd_DOWN   ,		   ,            ,                ,
  //	|  , ----  ,GPIO_Mode_AN  ,--------      ,GPIO_Speed_100MHz ,  --------       ,		   ,            ,                ,
  //...............................................................................................................................................
  "PD0"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_0  ,GPIO_PinSource0 ,	//NO USED
  "PD1"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_1  ,GPIO_PinSource1 ,	//NO USED
  "PD2"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_2MHz   ,GPIO_PuPd_UP     ,GPIO_Mode_IN  ,GPIO_Pin_2  ,GPIO_PinSource2 ,	//NO USED
  "PD3"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_2MHz   ,GPIO_PuPd_UP     ,GPIO_Mode_IN  ,GPIO_Pin_3  ,GPIO_PinSource3 ,	//NO USED
  "PD4"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_2MHz   ,GPIO_PuPd_UP     ,GPIO_Mode_IN  ,GPIO_Pin_4  ,GPIO_PinSource4 ,	//NO USED  
  "PD5"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_5  ,GPIO_PinSource5 ,	//NO USED
  "PD6"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_6  ,GPIO_PinSource6 ,	//NO USED
  "PD7"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_2MHz   ,GPIO_PuPd_UP     ,GPIO_Mode_IN  ,GPIO_Pin_7  ,GPIO_PinSource7 ,	//NO USED
  "PD8"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_8  ,GPIO_PinSource8 ,	//NO USED
  "PD9"  , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_9  ,GPIO_PinSource9 ,	//NO USED
  "PD10" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_2MHz   ,GPIO_PuPd_DOWN   ,GPIO_Mode_IN  ,GPIO_Pin_10 ,GPIO_PinSource10,	//NO USED
  "PD11" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_2MHz   ,GPIO_PuPd_DOWN   ,GPIO_Mode_IN  ,GPIO_Pin_11 ,GPIO_PinSource11,	//NO USED
  "PD12" , 1     ,GPIO_Mode_AF  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_AF_TIM4  ,GPIO_Pin_12 ,GPIO_PinSource12,	//NO USED
  "PD13" , 1     ,GPIO_Mode_AF  ,GPIO_OType_PP ,GPIO_Speed_25MHz  ,GPIO_PuPd_NOPULL ,GPIO_AF_TIM4  ,GPIO_Pin_13 ,GPIO_PinSource13,	//NO USED
  "PD14" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_2MHz   ,GPIO_PuPd_DOWN   ,GPIO_Mode_IN  ,GPIO_Pin_14 ,GPIO_PinSource14,	//NO USED
  "PD15" , 1     ,GPIO_Mode_IN  ,GPIO_OType_OD ,GPIO_Speed_2MHz   ,GPIO_PuPd_DOWN   ,GPIO_Mode_IN  ,GPIO_Pin_15 ,GPIO_PinSource15,	//NO USED
 
   //}
  //{    /*  PORT E CONFIG  */

  // Name,Default,Direction Mode, Output Type  ,  Output Speed    ,Pin_pullupdown   ,  GPIO_AF     , 	  Number  ,  Pin_sources   ,	    Comment
  //.............................................................................................................................................
  //	|  ,  1    ,GPIO_Mode_IN  ,GPIO_OType_PP ,GPIO_Speed_2MHz   ,GPIO_PuPd_NOPULL ,		   ,            ,                ,                                 
  //	|  ,  0    ,GPIO_Mode_OUT ,GPIO_OType_OD ,GPIO_Speed_25MHz  ,GPIO_PuPd_UP     ,		   ,            ,                ,
  //	|  , NuLL  ,GPIO_Mode_AF  ,--------      ,GPIO_Speed_50MHz  ,GPIO_PuPd_DOWN   ,		   ,            ,                ,
  //	|  , ----  ,GPIO_Mode_AN  ,--------      ,GPIO_Speed_100MHz ,  --------       ,		   ,            ,                ,
  //...............................................................................................................................................
  "PE0"  , 0     ,GPIO_Mode_IN ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN ,GPIO_Pin_0  ,GPIO_PinSource0 ,	//NO USED
  "PE1"  , 0     ,GPIO_Mode_IN ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN ,GPIO_Pin_1  ,GPIO_PinSource1 ,	//NO USED
  
  "PE2"  , 0     ,GPIO_Mode_OUT ,GPIO_OType_PP , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_OUT ,GPIO_Pin_2  ,GPIO_PinSource2 ,	//LED1
  "PE3"  , 0     ,GPIO_Mode_OUT ,GPIO_OType_PP , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_OUT ,GPIO_Pin_3  ,GPIO_PinSource3 ,	//LED2
  "PE4"  , 0     ,GPIO_Mode_OUT ,GPIO_OType_PP , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_OUT ,GPIO_Pin_4  ,GPIO_PinSource4 ,	//LED3
  
  "PE5"  , 0     ,GPIO_Mode_IN ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_5  ,GPIO_PinSource5 ,	//NO USED
  "PE6"  , 0     ,GPIO_Mode_IN ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_6  ,GPIO_PinSource6 ,	//NO USED  
  "PE7"  , 0     ,GPIO_Mode_IN ,GPIO_OType_OD , GPIO_Speed_2MHz  ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN  ,GPIO_Pin_7  ,GPIO_PinSource7 ,	//NO USED
  
  "PE8"  , 1     ,GPIO_Mode_OUT,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_OUT,GPIO_Pin_8  ,GPIO_PinSource8 ,	//TIM1_CH1N
  "PE9"  , 1     ,GPIO_Mode_AF ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_TIM1 ,GPIO_Pin_9  ,GPIO_PinSource9 ,	//TIM1_CH1- PWM
  "PE10" , 1     ,GPIO_Mode_OUT,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_OUT,GPIO_Pin_10 ,GPIO_PinSource10,	//TIM1_CH2N
  "PE11" , 1     ,GPIO_Mode_AF ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_TIM1 ,GPIO_Pin_11 ,GPIO_PinSource11,	//TIM1_CH2- PWM
  "PE12" , 1     ,GPIO_Mode_OUT,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_OUT,GPIO_Pin_12 ,GPIO_PinSource12,	//TIM1_CH3N
  "PE13" , 1     ,GPIO_Mode_AF ,GPIO_OType_PP , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_AF_TIM1 ,GPIO_Pin_13 ,GPIO_PinSource13,	//TIM1_CH3- PWM
  
  "PE14" , 0     ,GPIO_Mode_IN ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN ,GPIO_Pin_14 ,GPIO_PinSource14,	//NO USED
  "PE15" , 0     ,GPIO_Mode_IN ,GPIO_OType_OD , GPIO_Speed_25MHz ,GPIO_PuPd_NOPULL ,GPIO_Mode_IN ,GPIO_Pin_15 ,GPIO_PinSource15	    //NO USED
   //}
};
//-----------------------------------------------------------------------------|
/*
|------------------------------------------------------------------------------|
|           ***		USART		***														   |
|------------------------------------------------------------------------------|
*/
const Usart_Struct STM32F4_USARTConfig[STM32F4xx_USART_NUMBER]=
{
  //	name ,default,  BaudRate ,    WordLength      ,     StopBits      ,     Parity      ,	  Mode       ,        HardwareFlowControl       ,Interrupt_EN,  Source_IT   ,    RCC_APB_Periph    ,	Comment
  //......................................................................................................................................................................................................
  //	 |   ,ENABLE ,	 9600	 ,USART_WordLength_8b ,USART_StopBits_1   ,USART_Parity_No  ,USART_Mode_Rx   ,USART_HardwareFlowControl_None    ,  ENABLE    ,USART_IT_TXE  ,RCC_APB2Periph_USART1 ,
  //	 |   ,DISABLE,	115200   ,USART_WordLength_9b ,USART_StopBits_0_5 ,USART_Parity_Even,USART_Mode_Tx   ,USART_HardwareFlowControl_RTS     ,  DISABLE   ,USART_IT_RXNE ,											 ,
  //     |   , ----  ,		 ,    --------        ,USART_StopBits_2   ,USART_Parity_Odd ,USART_Mode_RxTx ,USART_HardwareFlowControl_CTS     ,   ----     ,USART_IT_TC   ,											 ,
  //	 |   , ----  ,		 ,	  --------    ,USART_StopBits_1_5 ,    -------	    ,	  ----       ,USART_HardwareFlowControl_RTS_CTS ,   ----     ,  ....        ,											 ,
  //......................................................................................................................................................................................................
  "MainBoard",ENABLE ,	 230400  ,USART_WordLength_8b ,USART_StopBits_1   ,USART_Parity_No  ,USART_Mode_RxTx ,USART_HardwareFlowControl_None    ,  ENABLE    ,USART_IT_RXNE ,RCC_APB2Periph_USART1 ,
  //"EXH_Flow" ,ENABLE ,	 19200   ,USART_WordLength_8b ,USART_StopBits_1   ,USART_Parity_No  ,USART_Mode_RxTx ,USART_HardwareFlowControl_None    ,  ENABLE    ,USART_IT_RXNE ,RCC_APB1Periph_USART2 ,
  //"SAM_Board",ENABLE ,	 230400  ,USART_WordLength_8b ,USART_StopBits_1   ,USART_Parity_No  ,USART_Mode_Tx   ,USART_HardwareFlowControl_None    ,  ENABLE    ,USART_IT_TXE  ,RCC_APB1Periph_USART3    
};
/*
|------------------------------------------------------------------------------|
|           ***		SPI		***					    									   |
|------------------------------------------------------------------------------|
*/
/*
*     APB1 Frequency(Hz)      | 42000000  ,
*-----------------------------------------------------------------------------
*     APB2 Frequency(Hz)      | 84000000 => SPI1,
*/
const Spi_StrucT STM32F4_SPIConfig[STM32F4xx_SPI_NUMBER]=
{
  //	            name   ,default ,         BaudRate         ,      Mode      , 	    	Direction            ,    DataSize     ,	   CPOL     ,	 CPHA    ,	   NSS      ,     FirstBit    ,CRCPolynomial,Interrupt_EN,Source_IT ,  RCC_APB_Periph   ,	Comment
  //..................................................................................................................................................................................................................................
  //	                  ,ENABLE    ,SPI_BaudRatePrescaler_2   ,SPI_Mode_Master ,SPI_Direction_2Lines_FullDuplex ,SPI_DataSize_16b ,SPI_CPOL_Low  ,SPI_CPHA_1Edge ,SPI_NSS_Soft ,SPI_FirstBit_MSB ,   ......   ,ENABLE   ,SPI_IT_TXE  ,RCC_APB2Periph_SPI1,
  //	|	            ,DISABLE   ,SPI_BaudRatePrescaler_4   ,SPI_Mode_Slave  ,SPI_Direction_2Lines_RxOnly     ,SPI_DataSize_8b  ,SPI_CPOL_High ,SPI_CPHA_2Edge ,SPI_NSS_Hard ,SPI_FirstBit_LSB ,   ......   ,ENABLE   ,SPI_IT_RXNE ,RCC_APB1Periph_SPI2,
  //	|	            ,  ----    ,Prescaler :8,16,32,64,128 ,	--------  	 ,SPI_Direction_1Line_Rx          ,	   -------      ,	    ----     ,    -------    ,      ---- 	 ,   -------       ,   ......   ,ENABLE   ,    .....   ,RCC_APB1Periph_SPI3,
  //	|	            ,  ----    ,SPI_BaudRatePrescaler_256 ,	--------     ,SPI_Direction_1Line_Tx          ,	   -------      ,	    ----     ,    -------    ,	----   ,   -------       ,   ......   ,ENABLE   ,    .....   ,			------- 		 
  //..................................................................................................................................................................................................................................
/*Honywell*/    "P_INH"    ,ENABLE  ,SPI_BaudRatePrescaler_256  ,SPI_Mode_Master ,SPI_Direction_2Lines_FullDuplex ,SPI_DataSize_16b ,SPI_CPOL_Low  ,SPI_CPHA_2Edge ,SPI_NSS_Soft ,SPI_FirstBit_MSB,	5       ,ENABLE   ,SPI_IT_RXNE ,RCC_APB1Periph_SPI2 
///* A2D    */  "A2D_Ext"  ,ENABLE  ,SPI_BaudRatePrescaler_32   ,SPI_Mode_Master ,SPI_Direction_2Lines_FullDuplex ,SPI_DataSize_16b ,SPI_CPOL_Low  ,SPI_CPHA_2Edge ,SPI_NSS_Soft ,SPI_FirstBit_MSB,	5       ,ENABLE   ,SPI_IT_RXNE ,RCC_APB1Periph_SPI2,
///*allSensor*/ "P_O2EXH"  ,ENABLE  ,SPI_BaudRatePrescaler_256  ,SPI_Mode_Master ,SPI_Direction_2Lines_FullDuplex ,SPI_DataSize_16b ,SPI_CPOL_Low  ,SPI_CPHA_1Edge ,SPI_NSS_Soft ,SPI_FirstBit_MSB,	5       ,ENABLE   ,SPI_IT_RXNE ,RCC_APB1Periph_SPI3  
};

/*                                84MHz /   8 = 10.5      MHz
                                  84MHz /  16 = 5.25      MHz
                                  84MHz /  32 = 2.625     MHz
                                  84MHz /  64 = 1.3125    KHz
                                  84MHz / 128 = 656.25    KHz
                                  84MHz / 256 = 328.125   KHz
|------------------------------------------------------------------------------|
|           ***		Include		***														   |
|------------------------------------------------------------------------------|
*/

//==================================================
typedef enum 
{
 BEGIN            =0x00,
 CLOCK            =0x02,
 TIMER            =0x03,
 INTERUPTS        =0x04,
 READY            =0x05
}ENUM_MICRO_CONTROL;

//====================================================
void GPIO_ConfigPort (GPIO_TypeDef* GPIOx);
void USART_ConfigPeriph(USART_TypeDef* USARTx);
void ExtIT_ConfigPort (void);
void ADC1_ConfigPeriph (void);
void SPI_ConfigPeriph(SPI_TypeDef* SPIx);
void init_I2C(I2C_TypeDef* I2Cx);
void GPIO_ConfigPort_EXHSENSIRION (void);
extern void    Algorithm_Init (void);
//extern      void TIM_ConfigPort (void);
//extern      void IWDG_INITIALIZE(void);

#endif // _SAADAT_INIT_H
//
