#include "includes.h"

#define FREQ_TAB_SCALE  10 /*   湿度传感器频率表刻度   */
#define FREQ_TAB_NUM    11 /*   湿度传感器频率表数目   */

/* 湿度传感器频率与湿度对应表 */
const static uint32_t HUMIT_FREQ_TAB[] = {7351, 7224, 7100, 6976, 6853, 6728, 6600, 6468, 6330, 6186, 6033};

static uint32_t s_iFreqHumit = 0;/* 湿度传感器频率*/
static uint32_t s_iFreqOverCnt = 0;/*湿度传感器频率计数溢出次数*/

/**********************************************************************************************************
*	函 数 名: Humit_FreqOver_Set
*	功能说明: 在中断中设置1S内频率计数溢出次数
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
void Humit_FreqOver_Set(void)
{
    s_iFreqOverCnt += 1;
}

/**********************************************************************************************************
*	函 数 名: Humit_Freq_Set
*	功能说明: 在中断中设置湿度传感器脉冲计数
*	形    参: _ifreq   IS计数数据
*	返 回 值: NONE
**********************************************************************************************************/
void Humit_Freq_Set(uint32_t _ifreq)
{
    s_iFreqHumit = _ifreq;
}

/**********************************************************************************************************
*	函 数 名: Humit_Freq_Get
*	功能说明: 在任务中获取湿度传感器脉冲计数
*	形    参: NONE
*	返 回 值: IS计数数据
**********************************************************************************************************/
uint32_t Humit_Freq_Get(void)
{
    uint32_t iTemp = 0;
    
    iTemp = s_iFreqOverCnt * 65535 + s_iFreqHumit;
    
    s_iFreqOverCnt = 0;//清零计数
    
    return iTemp;
}

/**********************************************************************************************************
*	函 数 名: HumitMeasure
*	功能说明: 湿度测量
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
void HumitMeasure(void)
{
    unsigned char i = 0;
    float fTemp1 = 0.0,fTemp2 = 0.0; 
    float humit = 0;
    
    uint32_t iFreqHumit =  Humit_Freq_Get();  //s_iFreqHumit可能在任务执行过程中被中断改写，所以先赋值给一变量
    
    LOG_PRINT(DEBUG_HUMIT,"Humit->Freq = %d \r\n",iFreqHumit);
    
    if(iFreqHumit > HUMIT_FREQ_TAB[0] || iFreqHumit < HUMIT_FREQ_TAB[10])
    {
        humit = 0;
    }
    else
    {
        for(i=0;i<FREQ_TAB_NUM;i++)
        {
            if(iFreqHumit <= HUMIT_FREQ_TAB[i] && iFreqHumit >= HUMIT_FREQ_TAB[i+1])
            {
                fTemp1 = (float)(HUMIT_FREQ_TAB[i] - HUMIT_FREQ_TAB[i+1]) / FREQ_TAB_SCALE; //f/RH
                fTemp2 = (float)(HUMIT_FREQ_TAB[i] - iFreqHumit) / fTemp1;
                    
                humit = (float)FREQ_TAB_SCALE * i + fTemp2;
                break;
            }
        }       
    }

    //是否采用校准值
    if (g_SysData.Data.Para.humitZreoValib) {
        g_SysData.Data.Sample.humit = humit + g_SysData.Data.Para.humitZero;
    } else {
        g_SysData.Data.Sample.humit = humit;
    }

    SampleData_ToModbus();
}

/**********************************************************************************************************
*	函 数 名: APP_Humit
*	功能说明: 湿度传感器
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void APP_Humit(void *pvParameters)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
	
	while(1)
	{
        HumitMeasure();
        
        LOG_PRINT(DEBUG_TASK,"APP_Humit \r\n");
        
        vTaskDelay(sMaxBlockTime);
	}
}
