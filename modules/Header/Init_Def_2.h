//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifndef _SAADAT_INIT_DEF_H
#define _SAADAT_INIT_DEF_H
//------------------------------------------------------------------------------
//GENERAL DEFINE

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------
#include "stm32f4xx_gpio.h"
#include "main.h"
#include "stm32f4xx.h" 

//------------------------------------------------------------------------------
//         Internal definitions
//------------------------------------------------------------------------------
/*
|------------------------------------------------------------------------------|
|           ***		DEFINE		***														   |
|------------------------------------------------------------------------------|
*/
/* Define CLK ---------------------------------------------------------------*/
/* Define GPIO ---------------------------------------------------------------*/
#define     STM32F4xx_PORTS         5           //  PORT A ~ E
#define     STM32F4xx_PIN_NUMBER    16          //  PIN  0 ~ 15
#define     NuLL                    0x0F        //  
/* Define USART ---------------------------------------------------------------*/
#define     STM32F4xx_USART_NUMBER  1           // USART1 , USART2 , USART3
#define     USART_Mode_RxTx         (USART_Mode_Rx | USART_Mode_Tx) 
/* Define SPI ---------------------------------------------------------------*/
#define     STM32F4xx_SPI_NUMBER    1//3			
/* Define TIMER ---------------------------------------------------------------*/
/* Define I2C ---------------------------------------------------------------*/
/* Define xx ---------------------------------------------------------------*/
/*
|------------------------------------------------------------------------------|
|           ***		DEFINE STRUCT		***											   |
|------------------------------------------------------------------------------|
*/
#define     INCREMENT_POINTER(Ptr,Len)  Ptr++; (Ptr == Len ?	Ptr=0 : 0)   //INCREMENT_POINTER that increments a pointer and resets it to zero if it reaches a specified length.
/*           
|------------------------------------------------------------------------------|
|           ***		TYPE DEFINE		***												   |
|------------------------------------------------------------------------------|
*/
/* Define General
---------------------------------------------------------------*/
#define     A2DEXT_VREFmv         	((u16) 5000)
#define     A2DEXT_VREFv         	((u8) 5)
#define     A2DEXT_RESOLUTION		16
#define     A2DEXT_MAXVAL         	((u16)65535)//((u16) (((u32) 1 << (A2DEXT_RESOLUTION - 0)) - 1))//FFFF
#define     A2DEXT_HALFVAL        	((u16)32767)//((u16) (((u32) 1 << (A2DEXT_RESOLUTION - 1)) - 1))//7FFF
#define     A2DEXT_MINVAL         	0

#define     FALSE	                  0        
#define     TRUE	                  !FALSE

#define     PWM_TIM                 TIM1
#define     M1_CH1                  1
#define     M2_CH2                  2
#define     M3_CH3                  3
//
#define     M1_L_PIN                GPIO_Pin_8
#define     M1_L_PORT               GPIOE

#define     M2_L_PIN                GPIO_Pin_10
#define     M2_L_PORT               GPIOE

#define     M3_L_PIN                GPIO_Pin_12
#define     M3_L_PORT               GPIOE


//
#define     Hall1_PIN               GPIO_Pin_6
#define     Hall1_PORT              GPIOA

#define     Hall2_PIN               GPIO_Pin_7
#define     Hall2_PORT              GPIOA

#define     Hall3_PIN               GPIO_Pin_0
#define     Hall3_PORT              GPIOB

#define     LED1_PORT               GPIOE
#define     LED1_PIN                GPIO_Pin_2

#define     LED2_PORT               GPIOE
#define     LED2_PIN                GPIO_Pin_3

#define     LED3_PORT               GPIOE
#define     LED3_PIN                GPIO_Pin_4
//---
#define     PROGRAM_PORT            GPIOE
#define     PROGRAM_PIN             GPIO_Pin_6

//#define     A2DCS_PORT              GPIOB
//#define     A2DCS_PIN               GPIO_Pin_12

#define     MAINBOARD               USART1
#define     COMPORT                 USART1

#define     START                   10
#define     STOP                    20
//---
#define     PRESS_INH               SPI2

#define     PRESSINHCS_PORT        GPIOB
#define     PRESSINHCS_PIN         GPIO_Pin_12

#define     SENSIRION_EXH          I2C2

#define     EXH_TIM                 TIM4
#define     EXH_CH                  1
#define     EXH_PulseWidth          Timer4_PulseWidth

#define     VALVE_TIM3_GPIO_PORT     GPIOC
#define     VALVE_TIM3_GPIO_PIN      GPIO_Pin_8
#define     VALVE_TIM3_GPIO_SOURCE   GPIO_PinSource8
#define     VALVE_TIM3_GPIO_CLK      RCC_AHB1Periph_GPIOC
#define     VALVE_TIM3_AF            GPIO_AF_TIM3
#define     VALVE_TIM3               TIM3
#define     VALVE_TIM3_CHANNEL       TIM_Channel_3
#define     VALVE_TIM3_PWM_SCALE     10000U

//---------------------------------------------
/* Actuator Names  */
typedef enum 
{
  _AIR            =0,
  _O2             =1,
  _INH            =2,
  _EXH            =3,
  _NEBULIZER      =4,
  _END_ACTUATOR   =5 
}Actuator_Names;

/* Define CLK ---------------------------------------------------------------*/
/* Define GPIO ---------------------------------------------------------------*/
typedef struct 
{
  uint8_t               Pin_name [5];
  uint8_t               Pin_default;
  GPIOMode_TypeDef  	Pin_directionmode;
  GPIOOType_TypeDef     Pin_outputtype;
  GPIOSpeed_TypeDef 	Pin_outputspeed;
  GPIOPuPd_TypeDef  	Pin_pullupdown;
  uint8_t               GPIO_AF;	
  uint32_t              Pin_number;	 
  uint16_t              Pin_sources;
  
}Pin_Struct;
//  
/* Define USART ---------------------------------------------------------------*/
typedef struct 
{
  uint8_t               uart_name [10];
  FunctionalState       uart_default;
  uint32_t              BaudRate;
  uint16_t              WordLength;
  uint16_t              StopBits;
  uint16_t              Parity;
  uint16_t              Mode;	
  uint16_t              HardwareFlowControl;
  FunctionalState       Interrupt_EN;
  uint16_t              Source_IT;
  uint32_t              RCC_APB_Periph;
  
}Usart_Struct;
//
/* Define SPI ---------------------------------------------------------------*/
typedef struct 
{
  uint8_t               spi_name [9];
  FunctionalState       spi_default;
  uint16_t              BaudRate;
  uint16_t              Mode;
  uint16_t              Direction;
  uint16_t              DataSize;
  uint16_t              CPOL;
  uint16_t              CPHA;	
  uint16_t              NSS;
  uint16_t              FirstBit;
  uint16_t              CRCPolynomial;
  FunctionalState       Interrupt_EN;
  uint16_t              Source_IT;
  uint32_t              RCC_APB_Periph;
  
}Spi_Struct;
//
/* Define TIMER ---------------------------------------------------------------*/
/* Define I2C ---------------------------------------------------------------*/
/* Define SPI ---------------------------------------------------------------*/
typedef struct 
{
  uint8_t               spi_name [9];
  FunctionalState       spi_default;
  uint16_t              BaudRate;
  uint16_t              Mode;
  uint16_t              Direction;
  uint16_t              DataSize;
  uint16_t              CPOL;
  uint16_t              CPHA;	
  uint16_t              NSS;
  uint16_t              FirstBit;
  uint16_t              CRCPolynomial;
  FunctionalState       Interrupt_EN;
  uint16_t              Source_IT;
  uint32_t              RCC_APB_Periph;
  
}Spi_StrucT;
//

/* Define xx ---------------------------------------------------------------*/

/*
|------------------------------------------------------------------------------|
|															***		Prototype		***												   |
|------------------------------------------------------------------------------|
*/
void Init (void);
//void SetPWMPulse (TIM_TypeDef* TIMx, u16 TimePulse ,u8 Channel);
//void Read_I2C(void);
//void SetPWM_PAIR (u16 PulsePercent);
//void SetPWM_PO2 (u16 PulsePercent);
//void SetPWM_INH (u16 PulsePercent);
//void SetPWM_EXH (u16 PulsePercent);
//void SetPWM_NEBULIZER (u16 PulsePercent);

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

#endif  //_SAADAT_INIT_DEF_H
//---------------------------------------------------------------

