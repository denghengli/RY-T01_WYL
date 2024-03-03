#include "includes.h"

struct soft_timer *timg_blow_timer; //定时反吹定时器
struct soft_timer *blow_seq_timer; //反吹时序定时器
uint8_t timing_blow_start_flag = 0; //定时自动反吹开始标志，与手动反吹区别开
uint8_t blow_cnt = 0; //反吹次数，定时反吹需要达到反吹频次；手动反吹无次数限制，直到时间结束
uint8_t blow_sta = 0; //反吹状态

void timg_blow_timer_cb(void *param)
{
    timing_blow_start_flag = 1;
    valve_ctrl(eSYSSTA_PITG_BLOW, 1);
}

//反吹时序控制 全压管吹 1 秒，暂停 1 秒，静压管吹 1 秒，暂停 1 秒
void blow_seq_timer_cb(void *param)
{
    switch(blow_sta)
    {
        case 0: //全压管吹 1 秒
            DRV_Pin_Write(epin_VALVE1, PIN_HIGH);
            DRV_Pin_Write(epin_VALVE2, PIN_LOW);
            DRV_Pin_Write(epin_VALVE3, PIN_LOW);
            DRV_Pin_Write(epin_VALVE4, PIN_LOW);
            blow_sta++;
            break;

        case 1: //暂停 1 秒
            DRV_Pin_Write(epin_VALVE1, PIN_LOW);
            DRV_Pin_Write(epin_VALVE2, PIN_LOW);
            DRV_Pin_Write(epin_VALVE3, PIN_LOW);
            if (timing_blow_start_flag)
                DRV_Pin_Write(epin_VALVE4, PIN_HIGH); //进行湿度管反吹
            blow_sta++;
            break;    

        case 2: //静压管吹 1 秒
            DRV_Pin_Write(epin_VALVE1, PIN_LOW);
            DRV_Pin_Write(epin_VALVE2, PIN_LOW);
            DRV_Pin_Write(epin_VALVE3, PIN_HIGH);
            DRV_Pin_Write(epin_VALVE4, PIN_LOW);
            blow_sta++;
            break;

        case 3: //暂停 1 秒
            DRV_Pin_Write(epin_VALVE1, PIN_LOW);
            DRV_Pin_Write(epin_VALVE2, PIN_LOW);
            DRV_Pin_Write(epin_VALVE3, PIN_LOW);
            if (timing_blow_start_flag)
                DRV_Pin_Write(epin_VALVE4, PIN_HIGH); //进行湿度管反吹
            blow_cnt++; //完成一个周期，反吹次数累加
            blow_sta = 0; //重新开始下一个周期
            break;    
    }

    //定时反吹需要达到反吹频次；手动反吹无次数限制，直到时间结束
    if (timing_blow_start_flag)
    {
        if (blow_cnt < g_SysData.Data.Para.blowFreq)
        {
            soft_timer_start(blow_seq_timer);
        }
        else
        {
            timing_blow_start_flag = 0;
            valve_ctrl(eSYSSTA_PITG_BLOW, 0);
        }
    }
    else
    {
        soft_timer_start(blow_seq_timer);
    }
}


/**********************************************************************************************************
*	函 数 名: timing_blow_proc
*	功能说明: 识别定时自动反吹间隔 是否变化，需要启动或关闭定时
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
static void timing_blow_proc(void)
{
    static uint8_t last_blowInterval = 0;
    uint8_t cur_blowInterval = g_SysData.Data.Para.blowInterval;

    if (last_blowInterval != cur_blowInterval)
    {
        //1 --> 0关闭自动反吹定时
        if (cur_blowInterval == 0)
        {
            soft_timer_stop(timg_blow_timer);
        }
        //0 --> 1开启自动反吹定时
        else if (cur_blowInterval == 1)
        {
            soft_timer_config(timg_blow_timer, cur_blowInterval, 
                              SOFT_TIMER_MODE_RERIOD, SOFT_TIMER_UNIT_MIN,
                              NULL, timg_blow_timer_cb);
            soft_timer_start(timg_blow_timer);
        }
    }
}

//电磁阀控制
void valve_ctrl(SYSTEM_STATE_E sysSta, int start)
{
    if (start) //开始
    {
        switch(sysSta)
        {
            case eSYSSTA_MEASU: //正常测量状态
                g_SysData.Data.Sample.sysSta = eSYSSTA_MEASU;
                DRV_Pin_Write(epin_VALVE1, PIN_LOW);
                DRV_Pin_Write(epin_VALVE2, PIN_LOW);
                DRV_Pin_Write(epin_VALVE3, PIN_LOW);
                DRV_Pin_Write(epin_VALVE4, PIN_LOW);
                break;
            case eSYSSTA_PITG_BLOW: //皮托管反吹
                g_SysData.Data.Sample.sysSta = eSYSSTA_PITG_BLOW;
                //启动皮托管反吹时序控制
                blow_sta = 0;
                blow_cnt = 0;
                soft_timer_start(blow_seq_timer);
                break;
            case eSYSSTA_HUMIT_BLOW: //湿度管反吹
                g_SysData.Data.Sample.sysSta = eSYSSTA_HUMIT_BLOW;
                DRV_Pin_Write(epin_VALVE1, PIN_LOW);
                DRV_Pin_Write(epin_VALVE2, PIN_LOW);
                DRV_Pin_Write(epin_VALVE3, PIN_LOW);
                DRV_Pin_Write(epin_VALVE4, PIN_HIGH);
                break;
            case eSYSSTA_HAVE_BLOW_CALIB_ZERO: //有反吹气校零
                g_SysData.Data.Sample.sysSta = eSYSSTA_HAVE_BLOW_CALIB_ZERO;
                DRV_Pin_Write(epin_VALVE1, PIN_HIGH);
                DRV_Pin_Write(epin_VALVE2, PIN_HIGH);
                DRV_Pin_Write(epin_VALVE3, PIN_HIGH);
                DRV_Pin_Write(epin_VALVE4, PIN_LOW);
                break;
            case eSYSSTA_NO_BLOW_CALIB_ZERO: //无反吹气校零
                g_SysData.Data.Sample.sysSta = eSYSSTA_NO_BLOW_CALIB_ZERO;
                DRV_Pin_Write(epin_VALVE1, PIN_LOW);
                DRV_Pin_Write(epin_VALVE2, PIN_HIGH);
                DRV_Pin_Write(epin_VALVE3, PIN_LOW);
                DRV_Pin_Write(epin_VALVE4, PIN_LOW);
                break; 
            case eSYSSTA_HUMIT_CALIB: //湿度零点校准
                g_SysData.Data.Sample.sysSta = eSYSSTA_HUMIT_CALIB;
                DRV_Pin_Write(epin_VALVE1, PIN_LOW);
                DRV_Pin_Write(epin_VALVE2, PIN_LOW);
                DRV_Pin_Write(epin_VALVE3, PIN_LOW);
                DRV_Pin_Write(epin_VALVE4, PIN_LOW);
                break;    
        }
    }
    else //结束后退出到测量状态
    {
        //如果皮托管反吹还在进行中需要先停止
        if (g_SysData.Data.Sample.sysSta == eSYSSTA_PITG_BLOW)
        {
            if (get_soft_timer_status(blow_seq_timer) != SOFT_TIMER_STATUS_STOP)
            {
                blow_sta = 0;
                blow_cnt = 0;
                soft_timer_stop(blow_seq_timer);
            }
        }

        //开始测量
        valve_ctrl(eSYSSTA_MEASU, 1);
    }
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
    
    //上电默认为测量状态
    valve_ctrl(eSYSSTA_MEASU, 1);

    //定时反吹定时器
    timg_blow_timer = creat_soft_timer();
    blow_seq_timer = creat_soft_timer();
    soft_timer_config(blow_seq_timer, 1, SOFT_TIMER_MODE_SIGLE, SOFT_TIMER_UNIT_S,
                              NULL, blow_seq_timer_cb);
	while(1)
	{
        timing_blow_proc();

        LOG_PRINT(DEBUG_TASK,"APP_Timing \r\n");
        vTaskDelay(sMaxBlockTime);
	}
}
