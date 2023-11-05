#include "includes.h"

volatile ADC_DATA_T g_ADCData;

void Sort(unsigned short *a, unsigned short int l)//a为数组地址，l为数组长度。  
{  
    int i, j;  
    int v;  
    //排序主体  
    for(i = 0; i < l - 1; i ++)  
    {
        for(j = i+1; j < l; j ++)  
        {  
            if(a[i] > a[j])//如前面的比后面的大，则交换。  
            {  
                v = a[i];  
                a[i] = a[j];  
                a[j] = v;  
            }  
        }  
    }
}

/**********************************************************************************************************
*	函 数 名: AD_Conver_Start
*	功能说明: ADC采集触发转换
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
static void AD_Conver_Start(void)
{   
//    LL_ADC_Enable(ADC1);
    
    switch(g_ADCData.ChCnt + 1)
    {
        case 1://PT100
            LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_0);
            break;
            
        case 2://大气压
            LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_9);
            break;                     
    }

    LL_ADC_REG_StartConversionSWStart(ADC1);  
}

/**********************************************************************************************************
*	函 数 名: AD_Data_Analysis
*	功能说明: ADC采集数据解析处理
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
static void AD_Data_Analysis(void)
{
    static unsigned char cChCnt=0,cAvgCnt=0;//均值滤波计数
    unsigned short buf[ADC_AVG_MAX]={0}, i=0,j=0;
    unsigned int sum=0;
	
    if(g_ADCData.ChCnt < ADC_CH_MAX && g_ADCData.Cnt < ADC_AVG_MAX)//防止越界
    {
        g_ADCData.Buf[g_ADCData.ChCnt][g_ADCData.Cnt] = (unsigned int)LL_ADC_REG_ReadConversionData12(ADC1);
    }
    
    /*数据采集*/
    if(g_ADCData.ChCnt < ADC_CH_MAX) 
    {
        g_ADCData.ChCnt ++;
        
        if(++cChCnt >= ADC_CH_MAX) cChCnt = ADC_CH_MAX;
    } 
    else
    {
        g_ADCData.ChCnt = 0;       
       
        if(++g_ADCData.Cnt >= ADC_AVG_MAX) g_ADCData.Cnt = 0;
        
        if(++cAvgCnt >= ADC_AVG_MAX) cAvgCnt = ADC_AVG_MAX;
    }
        
     //均值滤波
    if((cChCnt == ADC_CH_MAX) && (cAvgCnt == ADC_AVG_MAX))//满时排序
    {
        for(i = 0; i < cChCnt; i++)
        {
            for(j = 0; j < cAvgCnt; j++)
            {
                buf[j] = g_ADCData.Buf[i][j];
            }
            
            Sort(&buf[0], cAvgCnt);

            sum = 0;
            for(j = 3; j < 8; j++)
            {
                sum += buf[j];
            }
            
            g_ADCData.Aver[i] = (unsigned int)(sum / 5);
        }   
    }
    else//未满不排序
    {
        if(cAvgCnt)//至少采到了一组数据
        {
            for(i = 0; i < cChCnt; i++)
            {
                sum = 0;
                for(j = 0; j < cAvgCnt; j++)
                {
                    sum += g_ADCData.Buf[i][j];
                }
                
                g_ADCData.Aver[i] = (unsigned int)(sum / cAvgCnt);
            } 
        }
    }      
}

void App_AD(void *pvParameters)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(500);   
    
    while(1) 
    {
        AD_Conver_Start();
            
        if(xSemaphoreTake(AdSem,sMaxBlockTime) == pdTRUE)
        {
            AD_Data_Analysis();

            LOG_PRINT(DEBUG_TASK,"App_AD \r\n");
            
            vTaskDelay(sMaxBlockTime);
        }
    }
}


         