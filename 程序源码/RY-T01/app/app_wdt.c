#include "includes.h"

/**********************************************************************************************************
*	函 数 名: App_WDT
*	功能说明: 
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void App_WDT(void *pvParameters)
{
    static unsigned char flag = 1;
        
    while(1) 
    {
        if(flag)
        {
            flag = 0;
            DRV_Pin_Write(epin_RUN,0);
            DRV_Pin_Write(epin_WDT,0);
        }
        else
        {
            flag = 1;
            DRV_Pin_Write(epin_RUN,1);
            DRV_Pin_Write(epin_WDT,1);
        }       
        
        LL_IWDG_ReloadCounter(IWDG);
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

