#include "includes.h"

/* 这里的IO不包括 USART、Timer、ADC引脚 */
const  struct DRV_Pin   DRV_PINS[] =
{
    {epin_GAS_SDA1,     LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_11,  PIN_HIGH},
    {epin_GAS_SCL1,     LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_10,  PIN_HIGH},
    {epin_GAS_SDA2,     LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_11,  PIN_HIGH},
    {epin_GAS_SCL2,     LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_12,  PIN_HIGH},
    
    {epin_DAC_SDA,      LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_7,  PIN_LOW},
    {epin_DAC_SCL,      LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_6,  PIN_LOW},
    {epin_DAC_LDAC,     LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_5,  PIN_LOW},
    {epin_DAC_RDY,      LL_APB2_GRP1_PERIPH_GPIOD, GPIOD, LL_GPIO_PIN_2,  PIN_LOW},
    
    {epin_AT24_SDA,     LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_14,  PIN_LOW},
    {epin_AT24_SCL,     LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_13,  PIN_LOW},
    
    {epin_KEY0,         LL_APB2_GRP1_PERIPH_GPIOA, GPIOA, LL_GPIO_PIN_12,  PIN_LOW},
    {epin_KEY1,         LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_8,  PIN_LOW},
    {epin_KEY2,         LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_9,  PIN_LOW},
    {epin_KEY3,         LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_10,  PIN_LOW},
    {epin_KEY4,         LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_15,   PIN_LOW},
    
    {epin_VALVE1,       LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_1,  PIN_LOW},
    {epin_VALVE2,       LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_2,  PIN_LOW},
    {epin_VALVE3,       LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_13,  PIN_LOW},
    {epin_VALVE4,       LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_14,  PIN_LOW},

    {epin_RUN,          LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_6,  PIN_LOW},
    {epin_WDT,          LL_APB2_GRP1_PERIPH_GPIOA, GPIOA, LL_GPIO_PIN_8,  PIN_LOW},
    {epin_LCD_BLK,      LL_APB2_GRP1_PERIPH_GPIOB, GPIOB, LL_GPIO_PIN_12,  PIN_LOW},
    {epin_DO_OUT,       LL_APB2_GRP1_PERIPH_GPIOC, GPIOC, LL_GPIO_PIN_0,  PIN_LOW}
};


#define ITEM_NUM(items) sizeof(items)/sizeof(items[0])

/**********************************************************************************************************
*	函 数 名: DRV_Pin_Get
*	功能说明: 根据引脚名称获取引脚句柄
*	形    参: _ePin 引脚名称
*	返 回 值: NONE
**********************************************************************************************************/
const  struct DRV_Pin * DRV_Pin_Get(DRV_PIN_NAME_E  _ePin)
{
    const  struct DRV_Pin *Pin = NULL;
    unsigned char i = 0;
    
    for(i=0;i<ITEM_NUM(DRV_PINS);i++)
    {
        if(_ePin == DRV_PINS[i].ePinName)
        {
            Pin = &DRV_PINS[i];
            break;
        }
    }

    return Pin;
};


/**********************************************************************************************************
*	函 数 名: DRV_Pin_Mode
*	功能说明: IO引脚模式配置
*	形    参: _ePin 引脚名称；_cMode模式
*	返 回 值: NONE
**********************************************************************************************************/
void DRV_Pin_Mode (DRV_PIN_NAME_E  _ePin, unsigned char _cMode)
{
    const  struct DRV_Pin *Pin = NULL;
    LL_GPIO_InitTypeDef GPIO_InitStruct;
    char out_flg = 0;
    
    Pin = DRV_Pin_Get(_ePin);
    if (Pin == NULL)
    {
        return;
    }
	/* GPIO Ports Clock Enable */
	LL_APB2_GRP1_EnableClock(Pin->IORCC);
    
    if(_cMode == PINMODE_OUT_PP) /* 推挽输出*/
    {
        GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
        out_flg = 1;
    }
    else if(_cMode == PINMODE_OUT_OD_UP) /* 开漏输出，上拉*/
    {
        GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
        GPIO_InitStruct.Pull       = LL_GPIO_PULL_UP;
        out_flg = 1;
    }
    else if(_cMode == PINMODE_OUT_OD_DOWN) /* 开漏输出，下拉*/
    {
        GPIO_InitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
        GPIO_InitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
        GPIO_InitStruct.Pull       = LL_GPIO_PULL_DOWN;
        out_flg = 1;
    }
    else if(_cMode == PINMODE_IN_UP) /* 上拉输入*/
    {
        GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    }
    else if(_cMode == PINMODE_IN_DOWN) /* 下拉输入*/
    {
        GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    }
    else if(_cMode == PINMODE_IN_FLOATING) /* 浮空输入*/
    {
        GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
    }
    else if(_cMode == PINMODE_IN_ANALOG) /* 模拟量输入*/
    {
        GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    }
    
    GPIO_InitStruct.Pin  = Pin->PinMask;
    LL_GPIO_Init(Pin->GPIOx, &GPIO_InitStruct);
    
    /* default state */
	if(out_flg == 1 && Pin->DefSta == PIN_LOW)
    {
        LL_GPIO_ResetOutputPin(Pin->GPIOx, Pin->PinMask);
    }
    else if(out_flg == 1 && Pin->DefSta == PIN_HIGH)
    {
        LL_GPIO_SetOutputPin(Pin->GPIOx, Pin->PinMask);
    }
}


/**********************************************************************************************************
*	函 数 名: DRV_Pin_Write
*	功能说明: 写IO引脚高低电平
*	形    参: _ePin 引脚名称；_cValue 电平
*	返 回 值: NONE
**********************************************************************************************************/
void DRV_Pin_Write(DRV_PIN_NAME_E  _ePin, unsigned char _cValue)
{
    const  struct DRV_Pin *Pin = NULL;
    
    Pin = DRV_Pin_Get(_ePin);
    if (Pin == NULL)
    {
        return;
    }
    
    if(_cValue == PIN_LOW)
    {
        LL_GPIO_ResetOutputPin(Pin->GPIOx, Pin->PinMask);
    }
    else if(_cValue == PIN_HIGH)
    {
        LL_GPIO_SetOutputPin(Pin->GPIOx, Pin->PinMask);
    }
    
}


/**********************************************************************************************************
*	函 数 名: DRV_Pin_Read
*	功能说明: 读取IO引脚电平信号
*	形    参: _ePin 引脚名称
*	返 回 值: NONE
**********************************************************************************************************/
unsigned char DRV_Pin_Read(DRV_PIN_NAME_E  _ePin)
{
    const  struct DRV_Pin *Pin = NULL;
    
    Pin = DRV_Pin_Get(_ePin);
    if (Pin == NULL)
    {
        return 2;
    }
    
    if (LL_GPIO_IsInputPinSet(Pin->GPIOx, Pin->PinMask))
    {
        return PIN_HIGH;
    }
    else
    {
        return PIN_LOW;
    }
}



/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{
    DRV_Pin_Mode(epin_WDT,  PINMODE_OUT_PP);
    DRV_Pin_Mode(epin_RUN,  PINMODE_OUT_PP);
    DRV_Pin_Mode(epin_LCD_BLK,  PINMODE_OUT_PP);
    DRV_Pin_Mode(epin_DO_OUT,  PINMODE_OUT_PP);
      
    DRV_Pin_Mode(epin_DAC_RDY, PINMODE_IN_UP);
    DRV_Pin_Mode(epin_DAC_LDAC,PINMODE_OUT_OD_UP);
    DRV_Pin_Mode(epin_DAC_SDA, PINMODE_OUT_OD_UP);
    DRV_Pin_Mode(epin_DAC_SCL, PINMODE_OUT_OD_UP);
    
    DRV_Pin_Mode(epin_GAS_SDA1,PINMODE_OUT_OD_UP);
    DRV_Pin_Mode(epin_GAS_SCL1,PINMODE_OUT_OD_UP);
    DRV_Pin_Mode(epin_GAS_SDA2,PINMODE_OUT_OD_UP);
    DRV_Pin_Mode(epin_GAS_SCL2,PINMODE_OUT_OD_UP);

    DRV_Pin_Mode(epin_AT24_SDA,PINMODE_OUT_OD_UP);
    DRV_Pin_Mode(epin_AT24_SCL,PINMODE_OUT_OD_UP);
    
    DRV_Pin_Mode(epin_VALVE1,   PINMODE_OUT_PP);
    DRV_Pin_Mode(epin_VALVE2,   PINMODE_OUT_PP);
    DRV_Pin_Mode(epin_VALVE3,   PINMODE_OUT_PP);
    DRV_Pin_Mode(epin_VALVE4,   PINMODE_OUT_PP);

    DRV_Pin_Mode(epin_KEY0,   PINMODE_IN_FLOATING);
    DRV_Pin_Mode(epin_KEY1,   PINMODE_IN_FLOATING);
    DRV_Pin_Mode(epin_KEY2,   PINMODE_IN_FLOATING);
    DRV_Pin_Mode(epin_KEY3,   PINMODE_IN_FLOATING);
    DRV_Pin_Mode(epin_KEY4,   PINMODE_IN_FLOATING);
}
   
