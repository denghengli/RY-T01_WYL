#include "includes.h"

#define SM9541_ADDR 0X28
#define SAMP_FREQ_MAX  120 //采样次数 
#define AUTOADJ_FREQ   5//自动校准采样次数
#define PF 0
#define GROW_DIR_NUM  5

static DRV_I2C_T s_tFGDynPresI2C;  //动压传感器I2C IO口
static DRV_I2C_T s_tFGSticPresI2C; //静压传感器I2C IO口
static SM9541_DATA_T s_tFGDynPresDat; //动压传感器数据结构体
static SM9541_DATA_T s_tFGSticPresDat;//静压传感器数据结构体

/********************************************************************************************************
*	函 数 名: FlueGasPress_DataInit
*	功能说明: 动压静压数据初始化
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
void FlueGasPress_DataInit(void)
{
	/*U11 020*/
    s_tFGDynPresI2C.SCL.ePinName   = epin_gas_scl1;
    s_tFGDynPresI2C.SDA.ePinName   = epin_gas_sda1;
	
	/*U12 040*/
    s_tFGSticPresI2C.SCL.ePinName   = epin_gas_scl2;
    s_tFGSticPresI2C.SDA.ePinName   = epin_gas_sda2;
	
	/*SM9541_020C_D 动压传感器*/
	s_tFGDynPresDat.MaxCount = 14745;
	s_tFGDynPresDat.MinCount = 1638;
	s_tFGDynPresDat.MaxPress = 1961; //单位Pa,1.961KPa;cmH2O单位换算得来https://www.si-micro.com/
	s_tFGDynPresDat.MinPress = -1961;  //单位Pa,-1.961KPa;cmH2O单位换算得来https://www.si-micro.com/
//	s_tFGDynPresDat.MaxPress = 9806; //单位Pa,1.961KPa;cmH2O单位换算得来https://www.si-micro.com/
//	s_tFGDynPresDat.MinPress = -9806;//单位Pa,-1.961KPa;cmH2O单位换算得来https://www.si-micro.com/

	/*SM9541_040C_D 静压传感器*/
	s_tFGSticPresDat.MaxCount = 14745;
	s_tFGSticPresDat.MinCount = 1638;
	s_tFGSticPresDat.MaxPress = 3922; //单位Pa,9.806KPa;cmH2O单位换算得来https://www.si-micro.com/
	s_tFGSticPresDat.MinPress = -3922;//单位Pa,-9.806KPa;cmH2O单位换算得来https://www.si-micro.com/	
//    s_tFGSticPresDat.MaxPress = 1961; //单位Pa,9.806KPa;cmH2O单位换算得来https://www.si-micro.com/
//	s_tFGSticPresDat.MinPress = -1961;//单位Pa,-9.806KPa;cmH2O单位换算得来https://www.si-micro.com/

}



/********************************************************************************************************
*	函 数 名: FlueGasDynPress_Measure
*	功能说明: 动压传感器数据测量
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
static void FlueGasDynPress_Measure(void)
{
	uint8_t i,cRdBuf[4];
	float sum=0.0,fTemp = 0.0;
    static float fDynPresBuf[SAMP_FREQ_MAX] = {0.0};
	static uint8_t AvgCnt = 0;
	static uint8_t BufCnt = 0;
	
	static int PresCntOld = 0, PresCntNew = 0;
	static int PresCntBuf[SAMP_FREQ_MAX] = {0};
	static uint8_t PresAvgCnt = 0;
	static uint8_t PresBufCnt = 0;  
	int Pressum = 0, PresTemp = 0;
	int PresCntMax = g_SysData.Data.Para.smoothTime;
	
    //设置超限
    if(PresCntMax > SAMP_FREQ_MAX)
    {
        PresCntMax = SAMP_FREQ_MAX;
        g_SysData.Data.Para.smoothTime = SAMP_FREQ_MAX;
        ParaData_Save(0);
    }
    
	DRV_I2C_Start(s_tFGDynPresI2C);
	
	if(DRV_I2C_WriteByteWaiteAck(s_tFGDynPresI2C,SM9541_ADDR<<1 | CMD_MODE_READ))
	{
		cRdBuf[0] = DRV_I2C_ReadByteWithAck  (s_tFGDynPresI2C);
		cRdBuf[1] = DRV_I2C_ReadByteWithAck  (s_tFGDynPresI2C);
		cRdBuf[2] = DRV_I2C_ReadByteWithAck  (s_tFGDynPresI2C);
		cRdBuf[3] = DRV_I2C_ReadByteWithNoAck(s_tFGDynPresI2C);
		DRV_I2C_Stop(s_tFGDynPresI2C);
		
		s_tFGDynPresDat.MsState   = cRdBuf[0]>>6;
		s_tFGDynPresDat.MsPresCnt = 0x3FFF & (cRdBuf[0]*256 + cRdBuf[1]);
		s_tFGDynPresDat.MsTempCnt = cRdBuf[2]*8 + (cRdBuf[3]>>5);
		
        if(s_tFGDynPresDat.MsPresCnt < s_tFGDynPresDat.MinCount || \
           s_tFGDynPresDat.MsPresCnt > s_tFGDynPresDat.MaxCount)
        {
            return;//防止出错，在调试的时候出现上电后第一次采集计数为0，导致数值很大
        }
        
		fTemp = (float)(s_tFGDynPresDat.MaxPress - s_tFGDynPresDat.MinPress) / \
		        (float)(s_tFGDynPresDat.MaxCount - s_tFGDynPresDat.MinCount);
		
		/*计数值平滑处理*/
		PresCntNew = (int) (PF * PresCntOld + (1 - PF) * s_tFGDynPresDat.MsPresCnt);
		PresCntOld = PresCntNew;
		if(PresBufCnt > PresCntMax) PresBufCnt = PresCntMax;
		if(PresBufCnt <= PresCntMax)
		{
			if(PresBufCnt == PresCntMax)PresBufCnt = 0;
			if(PresAvgCnt < PresCntMax)PresAvgCnt++;
			else PresAvgCnt = PresCntMax;
		}
		  
		PresCntBuf[PresBufCnt++] = PresCntNew;/*将数据存入环形buf中*/
		for(i=0;i<PresAvgCnt;i++)/*对环形buf中数据求和*/
		{
			Pressum += PresCntBuf[i];
		}
		PresTemp = Pressum / PresAvgCnt;
		
		/*压力值平滑处理*/
		s_tFGDynPresDat.Press = fTemp * (PresTemp - s_tFGDynPresDat.MinCount) + s_tFGDynPresDat.MinPress;
		s_tFGDynPresDat.Temp  = ((float)(s_tFGDynPresDat.MsTempCnt / 2048.0)) * 200 - 50;
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
}

/********************************************************************************************************
*	函 数 名: FlueGasSticPress_Measure
*	功能说明: 静压传感器数据测量
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
static void FlueGasSticPress_Measure(void)
{
	uint8_t i,cRdBuf[4];
	float sum=0.0,fTemp = 0.0;
    static float fSticPresBuf[SAMP_FREQ_MAX] = {0.0};
	static uint8_t AvgCnt = 0;
	static uint8_t BufCnt = 0;
	
	static int PresCntOld = 0, PresCntNew = 0;
	static int PresCntBuf[SAMP_FREQ_MAX] = {0};
	static uint8_t PresAvgCnt = 0;
	static uint8_t PresBufCnt = 0;  
	int Pressum = 0, PresTemp = 0;
	int PresCntMax = g_SysData.Data.Para.smoothTime;
		
    //设置超限
    if(PresCntMax > SAMP_FREQ_MAX)
    {
        PresCntMax = SAMP_FREQ_MAX;
        g_SysData.Data.Para.smoothTime = SAMP_FREQ_MAX;
        ParaData_Save(0);
    }
    
	DRV_I2C_Start(s_tFGSticPresI2C);
	
	if(DRV_I2C_WriteByteWaiteAck(s_tFGSticPresI2C,SM9541_ADDR<<1 | CMD_MODE_READ))
	{
		cRdBuf[0] = DRV_I2C_ReadByteWithAck  (s_tFGSticPresI2C);
		cRdBuf[1] = DRV_I2C_ReadByteWithAck  (s_tFGSticPresI2C);
		cRdBuf[2] = DRV_I2C_ReadByteWithAck  (s_tFGSticPresI2C);
		cRdBuf[3] = DRV_I2C_ReadByteWithNoAck(s_tFGSticPresI2C);
		DRV_I2C_Stop(s_tFGSticPresI2C);
		
		s_tFGSticPresDat.MsState   = cRdBuf[0]>>6;
		s_tFGSticPresDat.MsPresCnt = 0x3FFF & (cRdBuf[0]*256 + cRdBuf[1]);
		s_tFGSticPresDat.MsTempCnt = cRdBuf[2]*8 + (cRdBuf[3]>>5);

        if(s_tFGSticPresDat.MsPresCnt < s_tFGSticPresDat.MinCount || \
           s_tFGSticPresDat.MsPresCnt > s_tFGSticPresDat.MaxCount)
        {
            return;//防止出错，在调试的时候出现上电后第一次采集计数为0，导致数值很大
        }
        
		fTemp = (float)(s_tFGSticPresDat.MaxPress - s_tFGSticPresDat.MinPress) / \
		        (float)(s_tFGSticPresDat.MaxCount - s_tFGSticPresDat.MinCount);
		
		/*计数值平滑处理*/
		PresCntNew = (int) (PF * PresCntOld + (1 - PF) * s_tFGSticPresDat.MsPresCnt);
		PresCntOld = PresCntNew;
		if(PresBufCnt > PresCntMax) PresBufCnt = PresCntMax;
		if(PresBufCnt <= PresCntMax)
		{
			if(PresBufCnt == PresCntMax)PresBufCnt = 0;
			if(PresAvgCnt < PresCntMax)PresAvgCnt++;
			else PresAvgCnt = PresCntMax;
		}
		PresCntBuf[PresBufCnt++] = PresCntNew;/*将数据存入环形buf中*/
		for(i=0;i<PresAvgCnt;i++)/*对环形buf中数据求和*/
		{
			Pressum += PresCntBuf[i];
		}
		PresTemp = Pressum / PresAvgCnt;
		
		/*压力值平滑处理*/
		s_tFGSticPresDat.Press = fTemp * (PresTemp - s_tFGSticPresDat.MinCount) + s_tFGSticPresDat.MinPress;
		s_tFGSticPresDat.Temp  = ((float)s_tFGSticPresDat.MsTempCnt / 2048.0) * 200 - 50;
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
	static float fAutoAdjDynPBuf [AUTOADJ_FREQ] = {0.0};//全压
	static float fAutoAdjSticPBuf[AUTOADJ_FREQ] = {0.0};//静压
	static uint8_t AdjBufCnt = 0;
	float DynPsum=0.0, SticPsum = 0.0;
	
	if(g_SysData.Data.Para.speedCalibZeroFlg == 1)//是否开启了自动校准 1启动，0关闭；默认关闭
	{
		fAutoAdjDynPBuf [AdjBufCnt] = s_tFGDynPresDat.Pressavg;/*将数据存入buf中*/
		fAutoAdjSticPBuf[AdjBufCnt] = s_tFGSticPresDat.Pressavg;
		
		if(++AdjBufCnt >= AUTOADJ_FREQ)//校准数据采集完成
		{
			for(i=0;i<AUTOADJ_FREQ;i++)
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
                
			ParaData_Save(0);//将校准参数存入FLASH
			
			AdjBufCnt = 0;
		}
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
	
	if(s_tFGDynPresDat.Pressavg > 0.0001 || s_tFGDynPresDat.Pressavg < -0.0001)//置位测量完成标志,非0时说明有测量值
	{
		SetFlg_Measover(FLG_MEASOVER_DYNP);
	}
	if(s_tFGSticPresDat.Pressavg > 0.0001 || s_tFGSticPresDat.Pressavg < -0.0001)//置位测量完成标志,非0时说明有测量值
	{
		SetFlg_Measover(FLG_MEASOVER_STICP);
	}	
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



/**********************************************************************************************************
*	函 数 名: APP_FlueGasP
*	功能说明: APP_FlueGasP function
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void APP_FlueGasP(void  * argument)
{
    TickType_t sMaxBlockTime = pdMS_TO_TICKS(1000);
    
	FlueGasPress_DataInit();
	
	while(1)
	{
	    if (g_SysData.Data.Sample.sysSta == SYS_STA_MEASU)
		{
			FlueGasPress_Measure();       

			LOG_PRINT(DEBUG_TASK,"APP_FlueGasP \r\n");
		}
        vTaskDelay(sMaxBlockTime);
	}
}



