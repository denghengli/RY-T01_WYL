#include "includes.h"

/**********************************************************************************************************
*	函 数 名: MCP4728_Output
*	功能说明: DAC模拟量输出 Vout = (Vref*DAC)/4096 * GAIN
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
static void DAC_Output(void)
{
    float temp=0;
    float Vt=0.4, Vs=0.4, Vps=0.4, Vh=0.4;//温度、流速、静压、湿度 DAC输出电压
    static float Vt_Old=0.4, Vs_Old=0.4, Vps_Old=0.4, Vh_Old=0.4;//温度、流速、静压、湿度 DAC输出电压

    /* 根据电路DAC输出1V对应10mA输出可知，要输出4-20mA，DAC输出范围为0.4-2V */
    if (g_SysData.Data.Sample.sysSta == eSYSSTA_MEASU)
    {
        /* 温度 0-500℃,分辨率为 0.032mA/1℃*/
        temp = g_SysData.Data.Sample.ptTem / 500.0 * 16 + 4;
        temp = temp + g_SysData.Data.Para.temAOOffset;
        temp = (temp * g_SysData.Data.Para.temAOK) / 10.0;
        if (temp <= 0.4) Vt_Old = Vt = 0.4;
        else if (temp >= 2) Vt_Old = Vt = 2;
        else Vt_Old = Vt = temp;
        
        /* 流速 0-40m/s, 分辨率为 0.4mA/1m/s*/
        temp = g_SysData.Data.Sample.speed / 40.0 * 16 + 4;
        temp = temp + g_SysData.Data.Para.speedAOOffset;
        temp = (temp * g_SysData.Data.Para.speedAOK) / 10.0;
        if (temp <= 0.4) Vs_Old = Vs = 0.4;
        else if (temp >= 2) Vs_Old = Vs = 2;
        else Vs_Old = Vs = temp;
        
        /* 压力（静压）-5KPa - +5KPa,分辨率为 0.0016mA/1Pa*/
        temp = (g_SysData.Data.Sample.sticPress + 5000) / 10000.0 * 16 + 4;
        temp = temp + g_SysData.Data.Para.pressAOOffset;
        temp = (temp * g_SysData.Data.Para.pressAOK) / 10.0;
        if (temp <= 0.4) Vps_Old = Vps = 0.4;
        else if (temp >= 2) Vps_Old = Vps = 2;
        else Vps_Old = Vps = temp;
        
        /* 湿度 0-40%V,分辨率为 0.4mA/1%V*/
        temp = g_SysData.Data.Sample.abshumit / 40.0 * 16 + 4;
        temp = temp + g_SysData.Data.Para.humitAOOffset;
        temp = (temp * g_SysData.Data.Para.humitAOK) / 10.0;
        if (temp <= 0.4) Vh_Old = Vh = 0.4;
        else if (temp >= 2) Vh_Old = Vh = 2;
        else Vh_Old = Vh = temp;
        
        // 0  1  2  3分别对应 温度 静压 流速 湿度
        MCP4728_Output(Vt, Vps, Vs, Vh);
        //MCP4728_Output(1, 1, 1, 1);
    }
    else
    {
        // 0  1  2  3分别对应 温度 静压 流速 湿度
        MCP4728_Output(Vt_Old, Vps_Old, Vs_Old, Vh_Old);
    }
}

/**********************************************************************************************************
*	函 数 名: APP_DA
*	功能说明: APP_DA function
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void APP_DA(void  * argument)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
    
    MCP7428_Init();

	while(1)
	{
        DAC_Output();

        LOG_PRINT(DEBUG_TASK,"APP_DA \r\n");
        vTaskDelay(sMaxBlockTime);
	}
}




