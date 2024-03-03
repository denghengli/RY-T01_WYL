#include "includes.h"


/* ADC1 init function */
void MX_ADC1_Init(void)
{
	LL_ADC_InitTypeDef ADC_InitStruct = {0};
	LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};
	LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    
	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6);//ADC1的时钟频率
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    
    /**ADC1 GPIO Configuration
    PA2   ------> ADC1_IN2
    PB1   ------> ADC1_IN9
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
	/* ADC1 interrupt Init */
//	NVIC_SetPriority(ADC1_2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
	NVIC_EnableIRQ(ADC1_2_IRQn);

    /* ADC1 Init */
	ADC_InitStruct.DataAlignment 		= LL_ADC_DATA_ALIGN_RIGHT;
	ADC_InitStruct.SequencersScanMode 	= LL_ADC_SEQ_SCAN_ENABLE;
	LL_ADC_Init(ADC1, &ADC_InitStruct);
	ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
	LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
	ADC_REG_InitStruct.TriggerSource 	= LL_ADC_REG_TRIG_SOFTWARE;
	ADC_REG_InitStruct.SequencerLength 	= 1;
	ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;   //LL_ADC_REG_SEQ_DISCONT_4RANKS LL_ADC_REG_SEQ_DISCONT_DISABLE
	ADC_REG_InitStruct.ContinuousMode 	= LL_ADC_REG_CONV_SINGLE;          //LL_ADC_REG_CONV_CONTINUOUS LL_ADC_REG_CONV_SINGLE
	ADC_REG_InitStruct.DMATransfer 		= LL_ADC_REG_DMA_TRANSFER_NONE;   //LL_ADC_REG_DMA_TRANSFER_NONE  LL_ADC_REG_DMA_TRANSFER_UNLIMITED
	LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);

    /**Configure Regular Channel   PT100*/
	LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_2, LL_ADC_SAMPLINGTIME_239CYCLES_5);//LL_ADC_SAMPLINGTIME_13CYCLES_5
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_239CYCLES_5);//LL_ADC_SAMPLINGTIME_13CYCLES_5
    
    LL_ADC_Enable(ADC1);
    
    LL_ADC_StartCalibration(ADC1);
    while(LL_ADC_IsCalibrationOnGoing(ADC1) == 1)
    {};

    LL_ADC_EnableIT_EOS(ADC1);//使能转换结束后产生中断
}


