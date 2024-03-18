#include "includes.h"

#define MPXHZ_VCC 5.0f

static MPXHZ_DATA_T  s_tAtpPressDat;//大气压强数据全局变量

/********************************************************************************************************
*	函 数 名: MPX_GetTempFactor
*	功能说明: 获取烟气大气压传感器的温度系数
*	形    参: NONE
*	返 回 值: 温度系数
********************************************************************************************************/
static float MPX_GetTempFactor()
{
	float fRes,fTemp = 0.0f;
	
	fTemp = g_SysData.Data.Sample.ptTem;//读取烟气温度 -40° -- 125°
    
	if(fTemp < -40 || fTemp > 125)//温度超出大气压传感器工作范围，有可能是PT100温度测量有问题
	{
		return 0;
	}
	
	if(fTemp < 0)/*-40°  --  0° y=-1/20x + 1*/
	{
		fRes = -0.05 * fTemp + 1;
	}
	else if(fTemp > 85)/*85°  --  125°  y=1/40x - 45/40*/
	{
		fRes = 0.025 * fTemp - 1.125;
	}
	else/*0°  --  85°*/
	{
		fRes = 1;
	}

	return fRes;
}


/********************************************************************************************************
*	函 数 名: AtpPress_GetAD
*	功能说明: 获取烟气大气压传感器的AD值
*	形    参: NONE
*	返 回 值: 烟气大气压传感器的AD值
********************************************************************************************************/
static unsigned short AtpPress_GetAD()
{
	return g_ADCData.Aver[2];
}

/********************************************************************************************************
*	函 数 名: AtpPress_Measure
*	功能说明: 大气压强测量
*	形    参: NONE
*	返 回 值: NONE
********************************************************************************************************/
void AtpPress_Measure(void)
{
	float temp;
	
	s_tAtpPressDat.AD  = AtpPress_GetAD();//采集到的电压AD值

	temp = ((float)3.0 / 4096.0) * s_tAtpPressDat.AD;
	s_tAtpPressDat.Vol = 1.5 * temp;//通过分压比计算出最终的传感器测量电压
	 
    if (s_tAtpPressDat.Vol < 0.2)
    {
        s_tAtpPressDat.airPress = 20;
    }
    else
    {
        s_tAtpPressDat.airPress = (s_tAtpPressDat.Vol - 0.2) / (4.6/380.0) + 20;
    }
//	s_tAtpPressDat.TempFactor = MPX_GetTempFactor();
//	s_tAtpPressDat.PressError = 0; //-1.5KPa - 1.5KPa
//
//	temp = s_tAtpPressDat.TempFactor * s_tAtpPressDat.PressError * 0.009 * MPXHZ_VCC;
//	s_tAtpPressDat.airPress = (((float)s_tAtpPressDat.Vol/MPXHZ_VCC + 0.095) / 0.009) + temp;
	
    /* 把测量数据存入全局变量中 */
    FloatLimit(&s_tAtpPressDat.airPress,FLOAT_DECNUM);
	g_SysData.Data.Sample.blowGasPress = s_tAtpPressDat.airPress;
    
    SampleData_ToModbus();
    
    LOG_PRINT(DEBUG_ATPPRESS,"MPXHZ.AD = %d\r\n",s_tAtpPressDat.AD);
	LOG_PRINT(DEBUG_ATPPRESS,"MPXHZ.Vol = %f\r\n",s_tAtpPressDat.Vol);
	LOG_PRINT(DEBUG_ATPPRESS,"MPXHZ.TempFactor = %f\r\n",s_tAtpPressDat.TempFactor);
	LOG_PRINT(DEBUG_ATPPRESS,"MPXHZ.airPress = %f\r\n\r\n",s_tAtpPressDat.airPress);
}


/**********************************************************************************************************
*	函 数 名: APP_AtspP
*	功能说明: APP_AtspP function
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void APP_AtspP(void  * argument)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
    
	while(1)
	{
		AtpPress_Measure();
        
        LOG_PRINT(DEBUG_TASK,"APP_AtspP \r\n");
        
        vTaskDelay(sMaxBlockTime);
	}
}
