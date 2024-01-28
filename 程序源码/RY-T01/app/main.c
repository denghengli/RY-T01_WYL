#include "includes.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN PV */

/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
extern void App_Comm(void *pvParameters);
extern void App_AD(void *pvParameters);
extern void App_WDT(void *pvParameters);
extern void APP_AtspP(void *pvParameters);
extern void APP_FlueGasP(void *pvParameters);
extern void APP_PT100(void *pvParameters);
extern void APP_SpeedCal(void *pvParameters);
extern void APP_DA(void *pvParameters);
extern void APP_Timing(void *pvParameters);
extern void APP_Humit(void *pvParameters);
extern void APP_GUI(void  * argument);

xSemaphoreHandle CommSem;
xSemaphoreHandle AdSem;
xSemaphoreHandle FlashMutex;
xSemaphoreHandle MutexPrint;
EventGroupHandle_t  EventGSpeedCal = NULL;

void SystemClock_Config(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_3);

    if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_3)
    {
        Error_Handler();  
    }
    LL_RCC_HSE_Enable();

    /* Wait till HSE is ready */
    while(LL_RCC_HSE_IsReady() != 1)
    {

    }
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while(LL_RCC_PLL_IsReady() != 1)
    {

    }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);

    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {

    }
    LL_Init1msTick(72000000);

    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

    LL_SetSystemCoreClock(72000000);

    LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_FLASH);
}


/* IWDG init function */
static void MX_IWDG_Init(void)
{
  LL_IWDG_Enable(IWDG);

  LL_IWDG_EnableWriteAccess(IWDG);

  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_16);

  LL_IWDG_SetReloadCounter(IWDG, 4095);

  while (LL_IWDG_IsReady(IWDG) != 1)
  {
  }

  LL_IWDG_ReloadCounter(IWDG);
}


static void LL_Init(void)
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    /* BusFault_IRQn interrupt configuration */
    NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    /* UsageFault_IRQn interrupt configuration */
    NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    /* SVCall_IRQn interrupt configuration */
    NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    /* DebugMonitor_IRQn interrupt configuration */
    NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    /* PendSV_IRQn interrupt configuration */
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));
    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

    NVIC_SetPriority(USART1_IRQn,  NVIC_EncodePriority(NVIC_GetPriorityGrouping(),8, 0));
    NVIC_SetPriority(USART2_IRQn,  NVIC_EncodePriority(NVIC_GetPriorityGrouping(),7, 0));
    NVIC_SetPriority(USART3_IRQn,  NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
	NVIC_SetPriority(ADC1_2_IRQn,  NVIC_EncodePriority(NVIC_GetPriorityGrouping(),6, 0));
    
	NVIC_SetPriority(TIM1_UP_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2, 0));//TIM1用作湿度传感器定时1S
    NVIC_SetPriority(TIM2_IRQn,    NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 0));//TIM2用作湿度传感器外部脉冲计数
    /**NOJTAG: JTAG-DP Disabled and SW-DP Enabled 
    */
    LL_GPIO_AF_Remap_SWJ_NOJTAG();

}
/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* Init with LL driver */
  /* DMA controller clock enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */

}

int main()
{
    __set_PRIMASK(1);  

    CommSem    = xSemaphoreCreateBinary();
    AdSem      = xSemaphoreCreateBinary();
    FlashMutex = xSemaphoreCreateMutex();   
    MutexPrint = xSemaphoreCreateMutex();    
    EventGSpeedCal = xEventGroupCreate();/* 创建事件标志组 */

    LL_Init();
    SystemClock_Config();
    MX_GPIO_Init();     
    MX_ADC1_Init();
    MX_USART2_UART_Init();//485通讯
    MX_USART1_UART_Init();
    MX_IWDG_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    ParaData_Init();
    LCD_Init();
    
    xTaskCreate(App_Comm ,   "App_Comm",     configMINIMAL_STACK_SIZE*2,  NULL, 	tskIDLE_PRIORITY + 10, 	NULL);//n通讯
    xTaskCreate(App_AD,      "App_AD",       configMINIMAL_STACK_SIZE*2,  NULL, 	tskIDLE_PRIORITY + 9, 	NULL);//n
    xTaskCreate(APP_FlueGasP,"APP_FlueGasP", configMINIMAL_STACK_SIZE*2,  NULL, 	tskIDLE_PRIORITY + 8, 	NULL);//y
    xTaskCreate(APP_PT100,   "APP_PT100",    configMINIMAL_STACK_SIZE*2,  NULL, 	tskIDLE_PRIORITY + 7, 	NULL);
    xTaskCreate(APP_AtspP,   "APP_AtspP",    configMINIMAL_STACK_SIZE*2,  NULL, 	tskIDLE_PRIORITY + 7, 	NULL);
    xTaskCreate(APP_Humit,   "APP_Humit",    configMINIMAL_STACK_SIZE*4,  NULL, 	tskIDLE_PRIORITY + 7, 	NULL);
    xTaskCreate(APP_SpeedCal,"APP_SpeedCal", configMINIMAL_STACK_SIZE*2,  NULL, 	tskIDLE_PRIORITY + 6, 	NULL);
    xTaskCreate(APP_DA,      "APP_DA",       configMINIMAL_STACK_SIZE*2,  NULL, 	tskIDLE_PRIORITY + 5, 	NULL);
    xTaskCreate(APP_Timing,  "APP_Timing",   configMINIMAL_STACK_SIZE,    NULL, 	tskIDLE_PRIORITY + 4, 	NULL);
    xTaskCreate(App_WDT,     "App_WDT",      configMINIMAL_STACK_SIZE,    NULL, 	tskIDLE_PRIORITY + 4, 	NULL);//y
    xTaskCreate(APP_GUI,     "APP_GUI",      configMINIMAL_STACK_SIZE*4,  NULL, 	tskIDLE_PRIORITY + 3, 	NULL);
    
    vTaskStartScheduler();   

    while (1)
    {
        
    }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
	//IWDG_Clear();
	/* Called on each iteration of the idle task.  In this case the idle task
	just enters a low(ish) power mode. */
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues or
	semaphores. */
	taskDISABLE_INTERRUPTS();
	for (;;)
		;
}
/*-----------------------------------------------------------*/  
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	(void)pxTask;
	(void)pcTaskName;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for (;;)
		;
}
/*-----------------------------------------------------------*/