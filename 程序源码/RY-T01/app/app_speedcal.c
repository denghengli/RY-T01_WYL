#include "includes.h"

#define SPEEDCAL_AVG_NUM 5

static uint8_t Flag_Measover = 0;  //全压、静压、大气压、温度测量完成标志；这里计算风速时需要各参数的采集完成

/**********************************************************************************************************
*	函 数 名: SetFlg_Measover
*	功能说明: 设置全压、静压、大气压、温度测量完成标志
*	形    参: _cflg 完成标志位
*	返 回 值: NONE
**********************************************************************************************************/
void SetFlg_Measover(uint8_t _cflg)
{
    xEventGroupSetBits(EventGSpeedCal, _cflg);//事件标志组方法
}

/**********************************************************************************************************
*	函 数 名: GetFlg_Measover
*	功能说明: 烟气流速计算处理
*	形    参: 获取全压、静压、大气压、温度测量完成标志
*	返 回 值: 完成标志位
**********************************************************************************************************/
uint8_t GetFlg_Measover(void)
{
	return Flag_Measover;
}


/**********************************************************************************************************
*	函 数 名: FlueGasSpeedCal_Proc
*	功能说明: 烟气流速计算处理
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
void FlueGasSpeedCal_Proc(void)
{
    float fAdjFactor = g_SysData.Data.Para.piTGRatioK;
    float fAssistFactor = g_SysData.Data.Para.speedRatioK;
    float fDynPress = g_SysData.Data.Sample.dynPress;
    float fAirDensity = g_SysData.Data.Para.density;//空气密度
	float fSpend,fSpeedavg,fFlow,sum=0.0;
	static float fSpeedBuf[SPEEDCAL_AVG_NUM] = {0.0};
	static uint8_t AvgCnt = 0;
	static uint8_t BufCnt = 0;
	uint8_t i = 0;
	int random_integer;
      
    /*计算风速*/
    if(fDynPress > 0.5)//动压大于0
    {
        if (fDynPress > 1)
        {
            /*计算密度*/
            //fAirDensity = (1.34 * (1 - 0.5) + 0.804 * 0.5) * \
            //(273.0 /(273.0 + fPT100Tem)) * ((101 + fSticPress/1000.0)/101.325);
            fSpend = 1.414 * fAdjFactor * (sqrt(fDynPress / fAirDensity));
        }
        else
        {
            random_integer = rand();
            // 将随机整数映射到0到0.5的范围内
            fSpend = (double)random_integer / ((double)RAND_MAX + 1) * 0.5;
        }
    }
    else
    {
        fSpend = 0;
    }

    /*将数据存入环形buf中*/
    if(BufCnt <= SPEEDCAL_AVG_NUM)
    {
        if(BufCnt == SPEEDCAL_AVG_NUM)BufCnt = 0;
        if(AvgCnt < SPEEDCAL_AVG_NUM)AvgCnt++;
    }
    fSpeedBuf[BufCnt++] = fSpend;

    /*对环形buf中数据求和*/
    for(i=0;i<AvgCnt;i++)
    {
        sum += fSpeedBuf[i];
    }
    fSpeedavg = sum / (float)AvgCnt;/*计算环形buf中数据均值*/
    
    fSpeedavg = fSpeedavg * fAssistFactor;//乘以辅助系数
    fFlow = fSpeedavg * g_SysData.Data.Para.sectionArea;
    
    /* 把测量数据存入全局变量中 */
    FloatLimit(&fDynPress, FLOAT_DECNUM);
    FloatLimit(&fSpeedavg, FLOAT_DECNUM);
    FloatLimit(&fFlow, FLOAT_DECNUM);
    g_SysData.Data.Sample.dynPress = fDynPress;
    g_SysData.Data.Sample.speed = fSpeedavg;
    g_SysData.Data.Sample.flow = fFlow;
    
    SampleData_ToModbus();
}



/**********************************************************************************************************
*	函 数 名: APP_SpeedCal
*	功能说明: APP_SpeedCal function
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void APP_SpeedCal(void  * argument)
{
    TickType_t  sMaxBlockTime =	pdMS_TO_TICKS(2000);
    EventBits_t uxBits;
    
	while(1)
	{
        LOG_PRINT(DEBUG_TASK,"APP_SpeedCal \r\n");
         
        uxBits = xEventGroupWaitBits(EventGSpeedCal, /* 事件标志组句柄 */
                                     FLG_MEASOVER,        /* 等待所以标志被设置 */
                                     pdTRUE,              /* 退出前所以标志被清除，这里是所以标志都被设置才表示“退出”*/
                                     pdTRUE,              /* 设置为pdTRUE表示等待bit1和bit0都被设置*/
                                     sMaxBlockTime); 	  /* 等待延迟时间,portMAX_DELAY时永久等待 */
        if((uxBits & FLG_MEASOVER) == FLG_MEASOVER)
        {
            FlueGasSpeedCal_Proc();
        }        
        
//        vTaskDelay(sMaxBlockTime);
	}
}




