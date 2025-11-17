//-----------------------------------------------------------------------------|
// SAADAT Co 
// VENTILATOR 
// DEVELOPER : MOJTABA REZAEI ASHTIYANI
// VER 1.0
// This File contains the Set up the flow sensor TSI company which contains :
// __the coefficients read from memory with I2C communication.
// __reading the analog value is output.
//-----------------------------------------------------------------------------|

#include "Flow_EXH.h"
//----------------------------------------------------------------------
UNION_REQ_EXHFLOW_COMMUNICATION_FLG             EXHFLOW0_REQ_FLG;
UNION_REPLY_EXHFLOW_COMMUNICATION_FLG           EXHFLOW0_Reply_FLG;

extern          STRUCT_DATA_A2DSENSORS          DATA_Sensors;
//----------------------------------------------------------------------
u8 isFlowStable(void);
//----------------------------------------------------------------------
u8  A1_Packet_Cnt = 0;
//----------------------------------------------------------------------
FunctionalState         ZeroingFromMB = DISABLE;
u16     LOG_Err_Normal = 0, LOG_Err_BADCMD = 0,HsmCnt=0;  
u8      ZeroState = 0, ReqNormalState = 0, BADCMD_CNT = 0;      
u8      Zeroing_Repeat,Status_Repeat;
u16      delay_errEXH=0;
//-----------------------------------------------------------------------------|
#define FLOWWIN 100
float   FlowWin[FLOWWIN]={0};
u8      FlowIdx=0;
u8      FlowTimeOut=0;
//-----------------------------------------------------------------------------|
void EXH_RCV_Data(u8 data)
{  
    static    u16     EXH_DataSensor = 0;
    static    u8      Flg_1 = FALSE;
    
    switch(EXHFLOW0_REQ_FLG.BYTE)
    {
    case 1://REQNormal
        switch(ReqNormalState)
        {
        case 0:
            if(data == 'B')
            {
                EXHFLOW0_REQ_FLG.BYTE = 8;
            }
            else
            {
                if(data & 0x30)
                    DATA_Sensors.HWErr.EXHSENSOR = TRUE;
                else
                    DATA_Sensors.HWErr.EXHSENSOR = FALSE;
                if(data & 0x04)
                    DATA_Sensors.HWErr.EXHHARDWARE = TRUE;
                else
                    DATA_Sensors.HWErr.EXHHARDWARE = FALSE;
                if(data & 0x80)
                    Flg_1 = TRUE;
                else
                    Flg_1 = FALSE;
                
                EXHFLOW0_Reply_FLG.Data.ReplyNorm1 = TRUE;
                ReqNormalState = 1;
            }
            break;
        case 1:
            EXH_DataSensor = (data << 8);
            EXHFLOW0_Reply_FLG.Data.ReplyNorm2 = TRUE;
            ReqNormalState = 2;
            break;
        case 2:
          
          if(A1_Packet_Cnt > 5)
            DATA_Sensors.Detect_EXH_Sensor = A1BOARD_EXH; //A1 Board OK
          else
            A1_Packet_Cnt++;
          
          EXH_DataSensor |= data;
          if(Flg_1)
          {
            DATA_Sensors.Flow_EXH = EXH_DataSensor;
          }
          EXHFLOW0_Reply_FLG.Data.RcvNormOK = TRUE;     
          EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//rcv
          ReqNormalState = 0;
          break;
        }
        break;   
    case 2://REQzero ----------------------------------------
        if(data == 'B')
        {
            EXHFLOW0_REQ_FLG.BYTE = 8;
        }
        else
        {    
            EXHFLOW0_REQ_FLG.BYTE = CLEAR0; //rcv
            if(data == 0x02)
                EXHFLOW0_Reply_FLG.Data.RcvZeroOK = TRUE;
            else
                EXHFLOW0_Reply_FLG.Data.RcvZeroOK = FALSE;
        }
        break; 
    case 4://REQstatus --------------------------------------
        if(data == 'B')
        {
            EXHFLOW0_REQ_FLG.BYTE = 8;
        }
        else
        {
            EXHFLOW0_Reply_FLG.Data.RcvStatOK = TRUE;    
            if(data & 0x30)
            {
                DATA_Sensors.HWErr.EXHSENSOR = TRUE;
                EXHFLOW0_Reply_FLG.Data.RcvStatOK = FALSE;
            }
            else
            {
                DATA_Sensors.HWErr.EXHSENSOR = FALSE;
            }
            if(data & 0x04)
            {
                DATA_Sensors.HWErr.EXHHARDWARE = TRUE;
                EXHFLOW0_Reply_FLG.Data.RcvStatOK = FALSE;
            }
            else
            {
                DATA_Sensors.HWErr.EXHHARDWARE = FALSE;
            }
            EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//rcv
            if(!(data & 0x80))
            {
                EXHFLOW0_Reply_FLG.Data.RcvStatOK = FALSE;
            }
        }
        break;     
    case 8: //-----------------------------------------------
        if(++BADCMD_CNT >= 6)   
        {
            EXHFLOW0_Reply_FLG.Data.BAD_CMD = TRUE;
            EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//rcv
            LOG_Err_BADCMD++;
            BADCMD_CNT = 0;
        }
        break;
    }//switch 
    //-----------
}
//-----------------------------------------------------------------------------|
void    Req_ZeroingMB(FunctionalState Tyep)
{
    if(~ZeroingFromMB)
    {
        ZeroingFromMB = Tyep;
        ZeroState = 0;
        Status_Repeat = 4;
        Zeroing_Repeat = 3;
        EXHFLOW0_Reply_FLG.BYTE = CLEAR0;
        EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//req_zero
        DATA_Sensors.HWErr.EXHZEROOK = FALSE;
        DATA_Sensors.HWErr.EXHErr = FALSE;
    }
}
//-----------------------------------------------------------------------------|
void  Start_Read_EXHflow ( void )//10mSec
{
    if(delay_errEXH == 0)
    {
        if(ZeroingFromMB == DISABLE) // normal
        {
            if( EXHFLOW0_Reply_FLG.Data.RcvNormOK == FALSE )
            {
                LOG_Err_Normal++;
            }           
            ReqNormalState = 0;
            EXHFLOW0_Reply_FLG.BYTE = CLEAR0;
            EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//read
            EXHFLOW0_REQ_FLG.Data.ReqNormal = TRUE;   
            USART_SendData(EXHFLOW, (u16)REQUEST_EXH_STATUSDATA);          
        }
        else //--------------- Zeroing------
        {
            switch(ZeroState)
            {
            case 0:
                delay_errEXH = 200;//2sec
                ZeroState = 10;//1;
                break;     
            case 1:
                if(EXHFLOW0_Reply_FLG.Data.RcvStatOK == TRUE)
                {
                    ZeroState = 20;
                    FlowIdx=0;
                    ReqNormalState = 0;
                    EXHFLOW0_Reply_FLG.BYTE = CLEAR0;
                    EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//read
                    EXHFLOW0_REQ_FLG.Data.ReqNormal = TRUE;   
                    USART_SendData(EXHFLOW, (u16)REQUEST_EXH_STATUSDATA);
                }
                else
                {
                    if(--Status_Repeat == 0)
                    {
                        DATA_Sensors.HWErr.EXHZEROOK = FALSE;//err status
                        DATA_Sensors.HWErr.EXHErr = TRUE;
                        EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//read
                        ZeroingFromMB = DISABLE; //End
                        ZeroState = 0;
                        FlowTimeOut=0;
                    }
                    else
                    {
                        ZeroState = 10;//0;
                    }
                }
                break;
            case 2:
                if(EXHFLOW0_Reply_FLG.Data.RcvZeroOK == TRUE)
                {
                    DATA_Sensors.HWErr.EXHZEROOK = TRUE;
                    DATA_Sensors.HWErr.EXHErr = FALSE;
                    ZeroingFromMB = DISABLE; //End
                    EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//read
                    ZeroState = 0;
                    FlowTimeOut=0;
                    delay_errEXH = Delay_;
                }
                else if(FlowTimeOut>15-1)//30 sec timeout
                {
                    FlowTimeOut=0;
                    DATA_Sensors.HWErr.EXHZEROOK = FALSE;
                    DATA_Sensors.HWErr.EXHErr = TRUE;
                    EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//read
                    ZeroingFromMB = DISABLE; //End
                    ZeroState = 0;
                    delay_errEXH = Delay_;
                }
                else
                {                       
                    FlowTimeOut++;
                    ZeroState = 20;
                    FlowIdx=0;
                    ReqNormalState = 0;
                    EXHFLOW0_Reply_FLG.BYTE = CLEAR0;
                    EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//read
                    EXHFLOW0_REQ_FLG.Data.ReqNormal = TRUE;   
                    USART_SendData(EXHFLOW, (u16)REQUEST_EXH_STATUSDATA);
                }
                break;  
            case 10:
                EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//read
                EXHFLOW0_REQ_FLG.Data.Reqstatus = TRUE;
                EXHFLOW0_Reply_FLG.BYTE = CLEAR0;
                USART_SendData(EXHFLOW, (u16)REQUEST_EXH_STATUS);
                ZeroState = 1;
                delay_errEXH = 50;//.5sec
                break; 
            case 20:
                FlowWin[FlowIdx++]=DATA_Sensors.Flow_EXH;
                if(FlowIdx>=FLOWWIN)//1 sec
                {
                    if(isFlowStable())
                    {
                        ZeroState = 21;
                        delay_errEXH = 50;//.5sec
                        FlowIdx=0;
                    }
                    else
                    {
                        ZeroState = 2;
                        FlowIdx=0;
                        delay_errEXH = 100;//1sec
                    }
                }
                else
                {
                    ReqNormalState = 0;
                    EXHFLOW0_Reply_FLG.BYTE = CLEAR0;
                    EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//read
                    EXHFLOW0_REQ_FLG.Data.ReqNormal = TRUE;   
                    USART_SendData(EXHFLOW, (u16)REQUEST_EXH_STATUSDATA);
                }
                break;
            case 21:
                EXHFLOW0_REQ_FLG.BYTE = CLEAR0;//read
                EXHFLOW0_REQ_FLG.Data.Reqzero = TRUE;//ok status
                EXHFLOW0_Reply_FLG.BYTE = CLEAR0;
                USART_SendData(EXHFLOW, (u16)REQUEST_EXH_AUTOZERO);
                ZeroState = 2;
                delay_errEXH = 50;//
                break;     
            }//switch  
        }//zero
    }
    else
    {
        delay_errEXH --;
    }
}
//-----------------------------------------------------------------------------|
u8 isFlowStable(void)
{
#define TOLERANCE 0.15f
#define THRESHOLD 0.9f
    
    float ave=0.0f,perc=0;
    u8 idx=0,Count=0;
    
    for(;idx<FLOWWIN;idx++)
    {
        ave+= FlowWin[idx]/(float)FLOWWIN;
    }
    perc = (ave * TOLERANCE<1?1:ave * TOLERANCE);
    for(idx=0;idx<FLOWWIN;idx++)
    {
        if(FlowWin[idx] < ave+perc && FlowWin[idx] > ave-perc)
        {
            Count++;
        }
    }
    return (Count>=THRESHOLD*FLOWWIN);
}
//------------------------------------------------------------------------------
float EXHflow_calculation (s16 DataFlow)
{
    float temp;
    temp = ((2.0675 * pow((float)DataFlow,3))/10000000)+((1.203 * pow((float)DataFlow,2)) / 100000)+((8.563 * DataFlow) / 1000);
    return temp;
}
//------------------------------------------------------------------------------


//USART_SendData(MAINBOARD, SENDData_2MB.Buffer[SENDData_2MB.Rd]);
/*************************** END **************************************/
