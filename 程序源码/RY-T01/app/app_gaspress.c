#include "includes.h"

#define SM9541_ADDR 0X6D
#define SAMP_FREQ_MAX  600 //采样次数 
#define AUTOADJ_FREQ   5//自动校准采样次数
#define PF 0
#define GROW_DIR_NUM  5

static _I2C_T s_tFGDynPresI2C;  //动压传感器I2C IO口
static _I2C_T s_tFGSticPresI2C; //静压传感器I2C IO口
static SM9541_DATA_T s_tFGDynPresDat; //动压传感器数据结构体
static SM9541_DATA_T s_tFGSticPresDat;//静压传感器数据结构体

struct soft_timer *timg_adj_timer; //定时自动校零定时器
uint8_t timing_adj_start_flag = 0; //定时自动校零开始标志，与手动校零区别开

/********************************************************************************************************
*	函 数 名: FlueGasPress_DataInit
*	功能说明: 动压静压数据初始化
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
void FlueGasPress_DataInit(void)
{
	/* U12 ZXP8_RANG_2KP */
    s_tFGDynPresI2C.SCL.ePinName = epin_GAS_SCL1;
    s_tFGDynPresI2C.SDA.ePinName = epin_GAS_SDA1;
	
	/* U11 ZXP8_RANG_10KP */
    s_tFGSticPresI2C.SCL.ePinName = epin_GAS_SCL2;
    s_tFGSticPresI2C.SDA.ePinName = epin_GAS_SDA2;
}


/********************************************************************************************************
*	函 数 名: FlueGasDynPress_Measure
*	功能说明: 动压传感器数据测量
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
static void FlueGasDynPress_Measure(void)
{
	uint8_t i;
    float sum = 0.0;
    float rp = 0, rt = 0;
    static float fDynPresBuf[SAMP_FREQ_MAX] = {0.0};
	static uint8_t AvgCnt = 0;
	static uint8_t BufCnt = 0;
	int PresCntMax = g_SysData.Data.Para.smoothTime;
	
    //设置超限
    if(PresCntMax > SAMP_FREQ_MAX)
    {
        PresCntMax = SAMP_FREQ_MAX;
        g_SysData.Data.Para.smoothTime = SAMP_FREQ_MAX;
        ParaData_Save(0);
    }
    
    //获取压力值
    getZXPValue(s_tFGDynPresI2C, ZXP8_RANG_2KP, &rp, &rt);
    s_tFGDynPresDat.Press = rp;
    s_tFGDynPresDat.Temp = rt;
    
    //压力值平滑处理
    if(BufCnt > PresCntMax) BufCnt = PresCntMax;
    if(BufCnt <= PresCntMax)
    {
        if(BufCnt == PresCntMax)BufCnt = 0;
        if(AvgCnt < PresCntMax)AvgCnt++;
        else AvgCnt = PresCntMax;
    }
    fDynPresBuf[BufCnt++] = s_tFGDynPresDat.Press;/*将数据存入环形buf中*/
    for(i=0;i<AvgCnt;i++)/*对环形buf中数据求和*/
    {
        sum += fDynPresBuf[i];
    }
    s_tFGDynPresDat.OffSet = g_SysData.Data.Para.dynPRatioB;
    s_tFGDynPresDat.AdjFactor = g_SysData.Data.Para.dynPRatioK;
    s_tFGDynPresDat.Pressavg = sum / (float)AvgCnt;
    s_tFGDynPresDat.PressavgAdj = s_tFGDynPresDat.Pressavg + s_tFGDynPresDat.OffSet;/*计算环形buf中数据均值*/
    if(s_tFGDynPresDat.AdjFactor > 0.0001)
    {
        s_tFGDynPresDat.PressavgAdj *= s_tFGDynPresDat.AdjFactor;
    }
    
    /* 把测量数据存入全局变量中 */
    FloatLimit(&s_tFGDynPresDat.PressavgAdj,FLOAT_DECNUM);
}

/********************************************************************************************************
*	函 数 名: FlueGasSticPress_Measure
*	功能说明: 静压传感器数据测量
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
static void FlueGasSticPress_Measure(void)
{
	uint8_t i;
    float sum = 0.0;
    float rp = 0, rt = 0;
    static float fSticPresBuf[SAMP_FREQ_MAX] = {0.0};
	static uint8_t AvgCnt = 0;
	static uint8_t BufCnt = 0;
	int PresCntMax = g_SysData.Data.Para.smoothTime;
    
    //设置超限
    if(PresCntMax > SAMP_FREQ_MAX)
    {
        PresCntMax = SAMP_FREQ_MAX;
        g_SysData.Data.Para.smoothTime = SAMP_FREQ_MAX;
        ParaData_Save(0);
    }
    
    //获取压力值
    getZXPValue(s_tFGSticPresI2C, ZXP8_RANG_10KP, &rp, &rt);
    s_tFGSticPresDat.Press = rp;
    s_tFGSticPresDat.Temp = rt;
    
    //压力值平滑处理
    if(BufCnt > PresCntMax) BufCnt = PresCntMax;
    if(BufCnt <= PresCntMax)
    {
        if(BufCnt == PresCntMax)BufCnt = 0;
        if(AvgCnt < PresCntMax)AvgCnt++;
        else AvgCnt = PresCntMax;
    }
    fSticPresBuf[BufCnt++] = s_tFGSticPresDat.Press;/*将数据存入环形buf中单位为Pa*/ 
    for(i=0;i<AvgCnt;i++)/*对环形buf中数据求和*/
    {
        sum += fSticPresBuf[i];
    }
    s_tFGSticPresDat.OffSet = g_SysData.Data.Para.sticPRatioB;
    s_tFGSticPresDat.AdjFactor = g_SysData.Data.Para.sticPRatioK;
    s_tFGSticPresDat.Pressavg = sum / (float)AvgCnt;/*计算环形buf中数据均值*/
    s_tFGSticPresDat.PressavgAdj = s_tFGSticPresDat.Pressavg + s_tFGSticPresDat.OffSet;
    if(s_tFGSticPresDat.AdjFactor > 0.0001)
    {
        s_tFGSticPresDat.PressavgAdj *= s_tFGSticPresDat.AdjFactor;
    }
    
    /* 把测量数据存入全局变量中 */
    FloatLimit(&s_tFGSticPresDat.PressavgAdj,FLOAT_DECNUM);	
}

/********************************************************************************************************
*	函 数 名: FlueGasPress_AutoAdj
*	功能说明: 动压静压传感器自动校准
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
static void FlueGasPress_AutoAdj(void)
{
	uint8_t   i = 0;
	static float fAutoAdjDynPBuf [AUTOADJ_FREQ] = {0.0};//动压
	static float fAutoAdjSticPBuf[AUTOADJ_FREQ] = {0.0};//静压
	static uint8_t AdjBufCnt = 0, AdjAvgCnt = 0;
	float DynPsum=0.0, SticPsum = 0.0;

    //是否开启了自动校准 2完成，1启动，0关闭；默认关闭
	if(g_SysData.Data.Para.speedCalibZeroFlg == 0)
	{
	    AdjBufCnt = 0;
	    AdjAvgCnt = 0;
	}
	else if(g_SysData.Data.Para.speedCalibZeroFlg == 1)
	{
		fAutoAdjDynPBuf [AdjBufCnt] = s_tFGDynPresDat.Pressavg;/*将数据存入buf中*/
		fAutoAdjSticPBuf[AdjBufCnt] = s_tFGSticPresDat.Pressavg;

		AdjBufCnt = (AdjBufCnt + 1) % AUTOADJ_FREQ;
		AdjAvgCnt = (AdjAvgCnt < AUTOADJ_FREQ) ? AdjAvgCnt++ : AUTOADJ_FREQ;

        //自动定时校零，采集满后自动退出
		if (AdjAvgCnt == AUTOADJ_FREQ && timing_adj_start_flag)
		{
		    timing_adj_start_flag = 0;
		    g_SysData.Data.Para.speedCalibZeroFlg = 2;
		}
	}
	else if (g_SysData.Data.Para.speedCalibZeroFlg == 2)
	{
	    for(i=0; i<AdjAvgCnt; i++)
		{
			DynPsum += fAutoAdjDynPBuf[i];
			SticPsum += fAutoAdjSticPBuf[i];
		}
        /* 把测量数据存入全局变量中 */
        g_SysData.Data.Para.dynPRatioB = -(DynPsum / (float)AUTOADJ_FREQ);
        g_SysData.Data.Para.sticPRatioB = -(SticPsum  / (float)AUTOADJ_FREQ);
        g_SysData.Data.Para.dynPRatioB = (float)((int)(g_SysData.Data.Para.dynPRatioB * pow(10,FLOAT_DECNUM))) /pow(10,FLOAT_DECNUM);
        g_SysData.Data.Para.sticPRatioB  = (float)((int)(g_SysData.Data.Para.sticPRatioB * pow(10,FLOAT_DECNUM))) /pow(10,FLOAT_DECNUM);

        g_SysData.Data.Para.speedCalibZeroFlg = 0;
        AdjBufCnt = 0;
	    AdjAvgCnt = 0;
	    
		ParaData_Save(0);//将校准参数存入FLASH
	}
}

/********************************************************************************************************
*	函 数 名: PiTG_Dir
*	功能说明: 皮托管安装方向识别
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
static void PiTG_Dir(void)
{
	/*不管皮托管在风机前还是后，动压大于0都是正向，小于0都是反向*/
	if (s_tFGDynPresDat.PressavgAdj >= 0)
	{
	  	g_SysData.Data.Sample.dynPress = s_tFGDynPresDat.PressavgAdj;
		g_SysData.Data.Sample.sticPress = s_tFGSticPresDat.PressavgAdj;
		g_SysData.Data.Sample.totalPress = s_tFGSticPresDat.PressavgAdj + s_tFGDynPresDat.PressavgAdj;
	}
	else
	{
	  	g_SysData.Data.Sample.dynPress = -s_tFGDynPresDat.PressavgAdj;
		g_SysData.Data.Sample.sticPress = s_tFGSticPresDat.PressavgAdj + s_tFGDynPresDat.PressavgAdj;
		g_SysData.Data.Sample.totalPress = s_tFGSticPresDat.PressavgAdj;
	}
	
//	if(s_tFGDynPresDat.Pressavg > 0.0001 || s_tFGDynPresDat.Pressavg < -0.0001)//置位测量完成标志,非0时说明有测量值
//	{
		SetFlg_Measover(FLG_MEASOVER_DYNP);
//	}
//	if(s_tFGSticPresDat.Pressavg > 0.0001 || s_tFGSticPresDat.Pressavg < -0.0001)//置位测量完成标志,非0时说明有测量值
//	{
		SetFlg_Measover(FLG_MEASOVER_STICP);
//	}	
}

/********************************************************************************************************
*	函 数 名: FlueGasPress_Measure
*	功能说明: 动压静压传感器数据测量
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
void FlueGasPress_Measure(void)
{
	FlueGasDynPress_Measure();
	FlueGasSticPress_Measure();
	PiTG_Dir();
	FlueGasPress_AutoAdj();
    
    SampleData_ToModbus();
}

void timg_adj_timer_cb(void *param)
{
    timing_adj_start_flag = 1;
    g_SysData.Data.Para.speedCalibZeroFlg = 1;
}

/**********************************************************************************************************
*	函 数 名: timing_adj_proc
*	功能说明: 识别定时自动校零 是否变化，需要启动或关闭定时
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
static void timing_adj_proc(void)
{
    static uint8_t last_autoCalibZero = 0;
    uint8_t cur_autoCalibZero = g_SysData.Data.Para.autoCalibZero;

    if (last_autoCalibZero != cur_autoCalibZero)
    {
        //1 --> 0关闭自动校零定时
        if (cur_autoCalibZero == 0)
        {
            soft_timer_stop(timg_adj_timer);
        }
        //0 --> 1开启自动校零定时
        else if (cur_autoCalibZero == 1)
        {
            soft_timer_config(timg_adj_timer, 24*60, 
                              SOFT_TIMER_MODE_RERIOD, SOFT_TIMER_UNIT_MIN,
                              NULL, timg_adj_timer_cb);
            soft_timer_start(timg_adj_timer);
        }
    }
}


/**********************************************************************************************************
*	函 数 名: APP_FlueGasP
*	功能说明: APP_FlueGasP function
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void APP_FlueGasP(void  * argument)
{
    TickType_t sMaxBlockTime = pdMS_TO_TICKS(1000);

    //定时自动校零定时器
    timg_adj_timer = creat_soft_timer();
    
	FlueGasPress_DataInit();
	
	while(1)
	{
//	    if (g_SysData.Data.Sample.sysSta == SYS_STA_MEASU)
//		{
			FlueGasPress_Measure();       
            timing_adj_proc();
            
			LOG_PRINT(DEBUG_TASK,"APP_FlueGasP \r\n");
//		}
        vTaskDelay(sMaxBlockTime);
	}
}



