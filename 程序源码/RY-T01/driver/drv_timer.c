#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_gpio.h"

/* TIM1-CH1 init function */
/*
  TIM1-CH1用作湿度传感器外部计数
*/
void MX_TIM1_Init(void)
{
    LL_TIM_InitTypeDef     TIM_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
    /**TIM1 GPIO Configuration  
    PA8   ------> TIM1_CH1 湿度传感器脉冲计数
    */
    
    TIM_InitStruct.Prescaler     = 0;
    TIM_InitStruct.CounterMode   = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload    = 0xffff;
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    LL_TIM_Init(TIM1, &TIM_InitStruct);
    
    LL_TIM_DisableARRPreload     (TIM1);
    LL_TIM_SetTriggerInput       (TIM1, LL_TIM_TS_TI1FP1);
    LL_TIM_SetClockSource        (TIM1, LL_TIM_CLOCKSOURCE_EXT_MODE1);
    LL_TIM_IC_SetFilter          (TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_IC_FILTER_FDIV1);
    LL_TIM_IC_SetPolarity        (TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_IC_POLARITY_FALLING);   
    LL_TIM_DisableIT_TRIG        (TIM1);
    LL_TIM_DisableDMAReq_TRIG    (TIM1);   
    LL_TIM_SetTriggerOutput      (TIM1, LL_TIM_TRGO_RESET);
    LL_TIM_DisableMasterSlaveMode(TIM1);

    /* TIM1 interrupt Init */
    NVIC_EnableIRQ(TIM1_UP_IRQn);
    
    LL_TIM_SetCounter      (TIM1, 0);
    LL_TIM_EnableIT_UPDATE (TIM1);
    LL_TIM_ClearFlag_UPDATE(TIM1);
    LL_TIM_EnableCounter   (TIM1);   
}


/* TIM2 init function */
/*
  TIM2用作时钟基准定时1S
  溢出时间 = （Autoreload + 1) * (Prescaler + 1)/TCCLK72M 
*/
void MX_TIM2_Init(void)
{
    LL_TIM_InitTypeDef TIM_InitStruct;

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

    TIM_InitStruct.Prescaler         = 9999;
    TIM_InitStruct.CounterMode       = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload        = 7199;
    TIM_InitStruct.ClockDivision     = LL_TIM_CLOCKDIVISION_DIV1;
    TIM_InitStruct.RepetitionCounter = 0;  //重复计数器
    LL_TIM_Init(TIM2, &TIM_InitStruct);
    LL_TIM_EnableARRPreload(TIM2);   //使能自动重装载预装载，ARR寄存器有缓冲区

    LL_TIM_SetClockSource  (TIM2, LL_TIM_CLOCKSOURCE_INTERNAL);
    LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
    LL_TIM_DisableMasterSlaveMode(TIM2);
    
    /* TIM2 interrupt Init */
    NVIC_EnableIRQ(TIM2_IRQn);
    
    LL_TIM_EnableIT_UPDATE (TIM2);//使能更新中断
    LL_TIM_ClearFlag_UPDATE(TIM2);
    LL_TIM_EnableCounter   (TIM2);
}


