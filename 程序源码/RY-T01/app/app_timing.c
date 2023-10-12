#include "includes.h"

#define BLOW_STA_START 0
#define BLOW_STA_ING   1
#define BLOW_STA_END   2

volatile unsigned int Tb = 0;//时钟基准
unsigned char Blow_Sta = 0;//反吹状态；开始、吹扫过程中、反吹结束

/**********************************************************************************************************
*	函 数 名: Timing_Clock_Cnt
*	功能说明: 时钟基准累加
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
void Timing_Clock_Cnt(void)
{
    Tb ++;
}

/**********************************************************************************************************
*	函 数 名: Timing_Blow_Proc
*	功能说明: 定时反吹处理 
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
static void Timing_Blow_Proc(void)
{
    unsigned int Bsd  = 2;//反吹开始延时时间
    unsigned int Bbsd = 5;//反吹吹扫开始延时时间
    unsigned int Bbed = 2;//反吹吹扫结束延时时间
    unsigned int Bed  = g_SysData.Data.Para.BlowIntvar * 60 * 60;//反吹结束延时时间    
//	unsigned int Bed = 5;
    static unsigned int  Ts=0,Te=0;
    static unsigned char Blow_Cnt=0;//吹扫计数

    switch (Blow_Sta)
    {
        case BLOW_STA_START /* 反吹开始 */:
        {
            if (Ts == 0)
            {
                Ts = Tb;
                g_SysData.Data.Sample.SysSta = SYS_STA_BLOW;
            }
            else
            {
                Te = Tb;
                if (Te >= Ts + Bsd)
                {
                    Ts = Te = 0;
                    Blow_Cnt ++;
                    Blow_Sta = BLOW_STA_ING;
                }
                else
                {
                    DRV_Pin_Write(epin_mes_do,1);
                }
            }
        }
        break;

        case BLOW_STA_ING  /* 反吹吹扫 */:
        {
            if (Ts == 0)
            {
                Ts = Tb;
            }
            else
            {
                Te = Tb;
                if (Te >= Ts + Bbsd)
                {
                    if (Te >= Ts + Bbsd + Bbed)
                    {
                        if (Blow_Cnt == 2)
                        {
                            Blow_Cnt = 0;
                            Ts = Te = 0;
                            Blow_Sta = BLOW_STA_END;
                        }
                        else
                        {
                            Ts = Tb;
                            Blow_Cnt ++;
                        }
                    }
                    else
                    {
                        DRV_Pin_Write(epin_blow_do,0);
                    }
                }
                else
                {
                    DRV_Pin_Write(epin_blow_do,1);
                }
            }
        }
        break;

        case BLOW_STA_END /* 反吹结束 */:
        {
            if (Ts == 0)
            {
                Ts = Tb;
                g_SysData.Data.Sample.SysSta = SYS_STA_MEASU;
            }
            else
            {
                Te = Tb;
                if (Te >= Ts + Bed)
                {
                    Tb = Ts = Te = 0;
                    Blow_Sta = BLOW_STA_START;
                }
                else
                {
                    DRV_Pin_Write(epin_mes_do,0);
                    DRV_Pin_Write(epin_blow_do,0);
                }
            }
        }
        break;
        
        default:
            break;
    }

    SampleData_ToModbus(); /* 保存系统状态 */
}

/**********************************************************************************************************
*	函 数 名: APP_Timing
*	功能说明: APP_Timing function
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void APP_Timing(void  * argument)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(500);
    
	while(1)
	{
        Timing_Blow_Proc();

        LOG_PRINT(DEBUG_TASK,"APP_Timing \r\n");
        
        vTaskDelay(sMaxBlockTime);
	}
}
