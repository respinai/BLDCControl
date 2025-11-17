//------------------------------------------------------------------------------
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI 
// VER 1.0
//------------------------------------------------------------------------------
#ifndef _SAADAT_USART_LIMIT_H
#define _SAADAT_USART_LIMIT_H
//================================================================================|

//================================================================================|
//================================================================================|
#define         NUMBERMODE      6       // StartUP,Init,Sleep,Direct,Ventilation,Old
#define         MAXPACKET       5       // maximum valid packet 
#define         NUMPACKETSEND   14      // tedade kole pakethaye ersali az CB.
const   u8      PACKETCodeLimit_MB2CB[NUMBERMODE][MAXPACKET]=
{
   0x00,   0x00,   0x00,   0x00,   0x00,        // StartUP Mode         00
   0x03,   0x00,   0x00,   0x00,   0x00,        // Sleep Mode           01
   0x03,   0x0A,   0x0B,   0x00,   0x00,        // Sensors Init Mode    02
   0x04,   0x05,   0x06,   0x07,   0x00,        // Ventilation Mode     03
   0x09,   0x00,   0x00,   0x00,   0x00,        // Direct Command Mode  04
   0x01,   0x02,   0x00,   0x00,   0x00         // Old Mode             05
};   
const   u8      PACKETCodeLimit_CB2MB[NUMBERMODE][MAXPACKET]=
{
   0x00,   0x00,   0x00,   0x00,   0x00,        // StartUP Mode         00
   0x05,   0x0C,   0x00,   0x00,   0x00,        // Sleep Mode           01
   0x05,   0x0A,   0x0C,   0x00,   0x00,        // Sensors Init Mode    02 
   0x06,   0x07,   0x09,   0x0A,   0x0C,        // Ventilation Mode     03
   0x0B,   0x0A,   0x0C,   0x00,   0x00,        // Direct Command Mode  04  
   0x01,   0x02,   0x03,   0x04,   0x00         // Old Mode             05
};

//const   u8      MODE_LEDBLINK[NUMBERMODE][2]= 
//{  /*LED1       LED2  */   
//    0,          0,              // StartUP Mode         00
//    30,         250,            // Sleep Mode           01    
//    5,          100,            // Sensors Init Mode    02 
//    30,         30,             // Ventilation Mode     03
//    5,          50,             // Direct Command Mode  04 
//    100,        100,            // Old Mode             05
//};

const   u8      LEN_PACKETSENDING[NUMPACKETSEND]=
{
               0,               // 0    -----
              11,               // 1    fast sensor 
              13,               // 2    slow sensor
              13,               // 3    pwm
              13,               // 4    current
             101,               // 5    introduction
              26,               // 6    signal
              60,               // 7    parameter
               2,               // 8    state control
              11,               // 9    alarms
               5,               // 10   hardware error
              22,               // 11   low level data
               2,                // 12  receive acknowledge
              25                // 13   Extend low level data
}; 

const   u8      LEN_PACKETSENDINGVIEWER[16]=
{
                0,                  // 0    --
                0,                  // 1    --  
                0,                  // 2    --  
                0,                  // 3    --
                0,                  // 4    --
                101,                // 5    introduction
                26,                 // 6    signal
                60,                 // 7    parameter
                0,                  // 8    --
                11,                 // 9    Alarms
                5,                  // 10   Hardware Errors
                25,                 // 11   Algorithm Settings S1
                17,                 // 12   Algorithm Settings S2
                21,                 // 13   Alarm Setting
                6,                  // 14   Apnea Setting
                5,                  // 15   Reserved
};
//================================================================================|


#endif  //_SAADAT_USART_LIMIT_H