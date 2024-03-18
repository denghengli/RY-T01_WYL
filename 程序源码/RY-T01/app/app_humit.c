#include "includes.h"

#define FREQ_TAB_SCALE  10 /*   湿度传感器频率表刻度   */
#define FREQ_TAB_NUM    11 /*   湿度传感器频率表数目   */
#define NUM_ROWS 20
#define NUM_COLS 20
const static float relhumit_tem[NUM_ROWS][NUM_COLS] = 
{
	{0.34,0.68,1.02,1.36,1.70,2.04,2.38,2.72,3.06,3.40,3.73,4.07,4.41,4.75,5.09,5.43,5.77,6.11,6.45,6.79},
	{0.47,0.94,1.41,1.88,2.35,2.82,3.29,3.76,4.23,4.70,5.16,5.63,6.10,6.57,7.04,7.51,7.98,8.45,8.92,9.39},
	{0.64,1.28,1.92,2.56,3.21,3.85,4.49,5.13,5.77,6.41,7.05,7.69,8.33,8.97,9.62,10.26,10.90,11.54,12.18,12.82},
	{0.86,1.73,2.59,3.45,4.32,5.18,6.04,6.91,7.77,8.64,9.50,10.36,11.23,12.09,12.95,13.82,14.68,15.54,16.41,17.27},
	{1.15,2.30,3.45,4.60,5.75,6.90,8.05,9.20,10.35,11.51,12.66,13.81,14.96,16.11,17.26,18.41,19.56,20.71,21.86,23.01},
	{1.52,3.03,4.55,6.06,7.58,9.09,10.61,12.12,13.64,15.16,16.67,18.19,19.70,21.22,22.73,24.25,25.76,27.28,28.79,30.31},
	{1.98,3.95,5.93,7.90,9.88,11.85,13.83,15.80,17.78,19.76,21.73,23.71,25.68,27.66,29.63,31.61,33.58,35.56,37.53,39.51},
	{2.55,5.10,7.65,10.20,12.75,15.30,17.85,20.40,22.95,25.50,28.05,30.60,33.15,35.70,38.25,40.80,43.35,45.90,48.45,51.00},
	{3.26,6.52,9.78,13.04,16.30,19.56,22.82,26.08,29.34,32.61,35.87,39.13,42.39,45.65,48.91,52.17,55.43,58.69,61.95,65.21},
	{4.13,8.27,12.40,16.53,20.66,24.80,28.93,33.06,37.19,41.33,45.46,49.59,53.72,57.86,61.99,66.12,70.25,74.39,78.52,82.65},
	{5.19,10.39,15.58,20.78,25.97,31.17,36.36,41.56,46.75,51.95,57.14,62.33,67.53,72.72,77.92,83.11,88.31,93.50,98.70,103.89},
	{6.48,12.95,19.43,25.91,32.39,38.86,45.34,51.82,58.29,64.77,71.25,77.72,84.20,90.68,97.16,103.63,110.11,116.59,123.06,129.54},
	{8.02,16.03,24.05,32.06,40.08,48.09,56.11,64.12,72.14,80.15,88.17,96.18,104.20,112.21,120.23,128.24,136.26,144.27,152.29,160.30},
	{9.85,19.69,29.54,39.39,49.24,59.08,68.93,78.78,88.62,98.47,108.32,118.16,128.01,137.86,147.71,157.55,167.40,177.25,187.09,196.94},
	{12.02,24.03,36.05,48.06,60.08,72.09,84.11,96.12,108.14,120.16,132.17,144.19,156.20,168.22,180.23,192.25,204.26,216.28,228.29,240.31},
	{14.57,29.13,43.70,58.27,72.83,87.40,101.97,116.53,131.10,145.67,160.23,174.80,189.36,203.93,218.50,233.06,247.63,262.20,276.76,291.33},
	{17.55,35.10,52.65,70.20,87.75,105.29,122.84,140.39,157.94,175.49,193.04,210.59,228.14,245.69,263.24,280.78,298.33,315.88,333.43,350.98},
	{21.02,42.04,63.05,84.07,105.09,126.11,147.13,168.14,189.16,210.18,231.20,252.22,273.23,294.25,315.27,336.29,357.31,378.32,399.34,420.36},
	{25.03,50.06,75.09,100.12,125.15,150.18,175.21,200.24,225.27,250.30,275.33,300.36,325.39,350.42,375.45,400.48,425.51,450.54,475.57,500.60},
	{29.65,59.30,88.94,118.59,148.24,177.89,207.54,237.18,266.83,296.48,326.13,355.78,385.42,415.07,444.72,474.37,504.02,533.66,563.31,592.96}
};

/* 湿度传感器频率与湿度对应表 */
const static uint32_t HUMIT_FREQ_TAB[] = {7351, 7224, 7100, 6976, 6853, 6728, 6600, 6468, 6330, 6186, 6033};

static uint32_t s_iFreqHumit = 0;/* 湿度传感器频率*/
static uint32_t s_iFreqOverCnt = 0;/*湿度传感器频率计数溢出次数*/
float real_humit;

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

float findAbsoluteHumidity(float relativeHumidity, float temperature) 
{
    
    int row,col; // 将相对湿度和温度转换为数组索引
    
    //范围限制
    if (relativeHumidity < 5) relativeHumidity = 5;
    else if (relativeHumidity > 100) relativeHumidity = 100;
    if (temperature < 5) temperature = 5;
    else if (temperature > 100) temperature = 100;
    
    row = (int)((temperature - 5.0) / 5.0);
    col = (int)((relativeHumidity - 5.0) / 5.0);
    
    // 边界检查
    if (row < 0) row = 0;
    if (col < 0) col = 0;
    if (row >= NUM_ROWS - 1) row = NUM_ROWS - 2;
    if (col >= NUM_COLS - 1) col = NUM_COLS - 2;

    // 简单的线性插值，计算相对湿度和温度对应的绝对湿度值
    float relHumidity1 = col * 5.0 + 5.0; // 较小的相对湿度值
    float relHumidity2 = (col + 1) * 5.0 + 5.0; // 较大的相对湿度值
    float temp1 = row * 5.0 + 5.0; // 较低的温度值
    float temp2 = (row + 1) * 5.0 + 5.0; // 较高的温度值

    // 线性插值计算绝对湿度值
    float absoluteHumidity1 = relhumit_tem[row][col];
    float absoluteHumidity2 = relhumit_tem[row + 1][col];
    float absoluteHumidity3 = relhumit_tem[row][col + 1];
    float absoluteHumidity4 = relhumit_tem[row + 1][col + 1];

    float interpolatedHumidity1 = ((temp2 - temperature) / (temp2 - temp1)) * absoluteHumidity1 +
                                  ((temperature - temp1) / (temp2 - temp1)) * absoluteHumidity2;

    float interpolatedHumidity2 = ((temp2 - temperature) / (temp2 - temp1)) * absoluteHumidity3 +
                                  ((temperature - temp1) / (temp2 - temp1)) * absoluteHumidity4;

    float finalHumidity = ((relHumidity2 - relativeHumidity) / (relHumidity2 - relHumidity1)) * interpolatedHumidity1 +
                          ((relativeHumidity - relHumidity1) / (relHumidity2 - relHumidity1)) * interpolatedHumidity2;

    return finalHumidity;
}

/**********************************************************************************************************
*	函 数 名: humitCollect
*	功能说明: 湿度测量
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
void humitCollect(void)
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
    real_humit = humit;
    
    //是否采用校准值
    if (g_SysData.Data.Para.humitZreoValib) {
        if (humit > g_SysData.Data.Para.humitZero)
            g_SysData.Data.Sample.relhumit = humit - g_SysData.Data.Para.humitZero;
        else 
            g_SysData.Data.Sample.relhumit = 0;
    } else {
        g_SysData.Data.Sample.relhumit = humit;
    }

    humit = findAbsoluteHumidity(g_SysData.Data.Sample.relhumit, g_SysData.Data.Sample.ptTem); //g/m3
    humit = humit * 1000.0 * 22.4 / 18.0; //ppm
    humit = humit / 10000.0; //%vol
    if (humit < 40)
        g_SysData.Data.Sample.abshumit = humit;
    else
        g_SysData.Data.Sample.abshumit = 40;
    
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
        humitCollect();
        
        LOG_PRINT(DEBUG_TASK,"APP_Humit \r\n");
        
        vTaskDelay(sMaxBlockTime);
	}
}
