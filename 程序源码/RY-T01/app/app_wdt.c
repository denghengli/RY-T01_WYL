#include "includes.h"

/**********************************************************************************************************
*	�� �� ��: App_WDT
*	����˵��: 
*	��    ��: NULL
*	�� �� ֵ: NULL
**********************************************************************************************************/
void App_WDT(void *pvParameters)
{
    static unsigned char flag = 1;
        
    while(1) 
    {
        if(flag)
        {
            flag = 0;
            DRV_Pin_Write(epin_run,0);
            DRV_Pin_Write(epin_wdt,0);
        }
        else
        {
            flag = 1;
            DRV_Pin_Write(epin_run,1);
            DRV_Pin_Write(epin_wdt,1);
        }       
        
        LL_IWDG_ReloadCounter(IWDG);
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

