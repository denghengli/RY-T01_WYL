/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "includes.h"
#include "cmsis_os.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/



/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/
/**
* @brief This function handles DMA1 channel1 global interrupt.
*/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
//void SVC_Handler(void)
//{
//  /* USER CODE BEGIN SVCall_IRQn 0 */
//
//  /* USER CODE END SVCall_IRQn 0 */
//  /* USER CODE BEGIN SVCall_IRQn 1 */
//
//  /* USER CODE END SVCall_IRQn 1 */
//}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
//void PendSV_Handler(void)
//{
//  /* USER CODE BEGIN PendSV_IRQn 0 */
//
//  /* USER CODE END PendSV_IRQn 0 */
//  /* USER CODE BEGIN PendSV_IRQn 1 */
//
//  /* USER CODE END PendSV_IRQn 1 */
//}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
    /* USER CODE BEGIN SysTick_IRQn 0 */

    /* USER CODE END SysTick_IRQn 0 */
    osSystickHandler();
    HAL_IncTick();
    /* USER CODE BEGIN SysTick_IRQn 1 */

    /* USER CODE END SysTick_IRQn 1 */
}

void ADC1_2_IRQHandler(void)
{
    static BaseType_t xHigherPriorityTaskWoken;
    
    if(LL_ADC_IsActiveFlag_EOS(ADC1) == 1)
    {
        xSemaphoreGiveFromISR(AdSem, &xHigherPriorityTaskWoken );

        LL_ADC_ClearFlag_EOS(ADC1);
    }
}

/**
  * @brief This function handles DMA1 channel3 global interrupt.
  */
void DMA1_Channel3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel3_IRQn 0 */

  /* USER CODE END DMA1_Channel3_IRQn 0 */
  
  /* USER CODE BEGIN DMA1_Channel3_IRQn 1 */

  /* USER CODE END DMA1_Channel3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

  /* USER CODE END DMA1_Channel5_IRQn 0 */
  
  /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

  /* USER CODE END DMA1_Channel5_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles USART1 global interrupt.
*/
void USART1_IRQHandler(void)
{   
    if(LL_USART_IsActiveFlag_RXNE(USART1)) //检测是否接收中断
	{
        LL_USART_ClearFlag_RXNE(USART1);
	}	
    
    if(LL_USART_IsActiveFlag_ORE(USART1))
    {
        LL_USART_ReceiveData8(USART1);
      	LL_USART_ClearFlag_ORE(USART1);
    }
    
    if(LL_USART_IsActiveFlag_FE(USART1))
    {
      	LL_USART_ClearFlag_FE(USART1);
    }     
}

/**
* @brief This function handles USART2 global interrupt.
*/
void USART2_IRQHandler(void)
{   
    static BaseType_t xHigherPriorityTaskWoken;
    
    if(LL_USART_IsActiveFlag_RXNE(USART2)) //检测是否接收中断
    {
        if(IsComRcvNew())
		{
			/*接收处理*/
			if(IsComRcvDone(LL_USART_ReceiveData8(USART2)))     
			{
				SetComRcvNewFlg(0);//解析时，不能接收新的帧，直至解析完成
				xSemaphoreGiveFromISR(CommSem, &xHigherPriorityTaskWoken);//发送信号量
			}
		}
        
        LL_USART_ClearFlag_RXNE(USART2);
    }
    
    if(LL_USART_IsActiveFlag_ORE(USART2))
    {
        LL_USART_ReceiveData8(USART2);
      	LL_USART_ClearFlag_ORE(USART2);
    }    
    
    if(LL_USART_IsActiveFlag_FE(USART2))
    {
      	LL_USART_ClearFlag_FE(USART2);
    }  
}

/**
* @brief This function handles USART3 global interrupt.
*/
void USART3_IRQHandler(void)
{   
    if(LL_USART_IsActiveFlag_RXNE(USART3)) //检测是否接收中断
    {
        LL_USART_ClearFlag_RXNE(USART3);
    }
    
    if(LL_USART_IsActiveFlag_ORE(USART3))
    {
      	LL_USART_ClearFlag_ORE(USART3);
    }    
    
    if(LL_USART_IsActiveFlag_FE(USART3))
    {
      	LL_USART_ClearFlag_FE(USART3);
    }     
}

/*
  * @brief This function handles TIM1 update interrupt.
  * TIM1用作时钟基准定时1S
  * 中断优先级大于configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY，所以在中断中不可调用FreeRTOS函数
*/
void TIM1_UP_IRQHandler(void)
{
    unsigned int iFreq = 0;
    
    if(LL_TIM_IsActiveFlag_UPDATE(TIM1))
	{
	    ex_sec_signal = !ex_sec_signal; //界面S变信号
	    
        soft_timer_exe(); //定时反吹
        
        iFreq = LL_TIM_GetCounter(TIM2); //获取TIM2湿度传感器脉冲计数
        Humit_Freq_Set(iFreq);
        LL_TIM_SetCounter(TIM2,0);
        
		LL_TIM_SetCounter(TIM1,0);   //清空计数
        LL_TIM_ClearFlag_UPDATE(TIM1);	
	}
}


/*
  * @brief This function handles TIM2-CH2 update interrupt.
  * 湿度传感器脉冲计数溢出中断
  * 中断优先级大于configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY，所以在中断中不可调用FreeRTOS函数
  * 湿度传感器频率大概为6K-7K，而TIM1溢出计数为65536，溢出视为错误
*/
void TIM2_IRQHandler(void)
{
    if(LL_TIM_IsActiveFlag_UPDATE(TIM2))
	{
        Humit_FreqOver_Set();
		LL_TIM_ClearFlag_UPDATE(TIM2);	       
	}
}
