
/***************************************************************************
*
* BEIJING ISENSING TECHNOLOGY CO.,LTD.01060974500
* (c) Copyright 2020 BEIJING ISENSING TECHNOLOGY CO.,LTD.
* ALL RIGHTS RESERVED.
*
***************************************************************************

*
* @file GPIO_demo.c
*
* @author 
*
* @version 0.0.1
*
* @date Jun. 17, 2020
*
* @brief providing GPIO demo 
*
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <math.h>
#include "common.h"
#include "ics.h"
#include "rtc.h"
#include "uart.h"
#include "gpio.h"
#include "pit.h"
#include "sysinit.h"
#include "start.h"

#include "sys.h"
#include "ZXP3010D.h"
#include "ZXP3010D_IIC.h"

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define C_BMP180_ONE_U8X			1

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
uint8_t cInitState = 0;
int16_t ntimecount = 0;
//double temp;
//double press;
//double press_filt;
//double altitude;

int32_t temp;
int32_t press;

float fTemp;
float fTemp_old;
float fPress;

uint8_t cBuffer[128];

/******************************************************************************
* Local functions
******************************************************************************/
int main (void);
void PIT_Config(void);
void PIT_Task(void);
void RTC_Task(void);


void DelayMs(u16 count);

/******************************************************************************
* Global functions
******************************************************************************/

/********************************************************************/
int main (void)
{
	/* Perform processor initialization */
	sysinit();
	cpu_identify();

	RTC_ConfigType  sRTCConfig;
	RTC_ConfigType  *pRTCConfig = &sRTCConfig;  

	printf("\nRunning the ISENSING ZXP3010D test project.\n");

	/* configure RTC to 1Hz interrupt frequency */
	pRTCConfig->u16ModuloValue = 9;                                      
	pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     /* enable interrupt */
	pRTCConfig->bClockSource   = RTC_CLKSRC_1KHZ;          /*clock source is 1khz*/
	pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    /*prescaler is 100*/

	RTC_SetCallback(RTC_Task);
	RTC_Init(pRTCConfig);

	PIT_Config();

	/* way 1. GPIO initialize by multiple pin mask */
	GPIO_Init(GPIOB, GPIO_PTH1_MASK, GPIO_PinOutput);
	GPIO_Init(GPIOB, GPIO_PTH2_MASK, GPIO_PinOutput);

	/*** End of Processor Expert internal initialization.                    ***/

	BSP_IIC_Initial();

	do
	{

	}while(ntimecount < 2);

	cInitState = 1;
	ntimecount = 0;

	while (1)
	{
		if(ntimecount >= 1)
		{
			ntimecount = 0;

			ZXP3010D_StartT();
			DelayMs(4);
			do{
				DelayMs(1);
			}while(ZXP3010D_ConStatus());
			temp = ZXP3010D_ResultT();

			ZXP3010D_StartP();
			DelayMs(12);
			do{
				DelayMs(1);
			}while(ZXP3010D_ConStatus());
			press = ZXP3010D_ResultP();
                        
			ZXP3010D_Caculate(press, temp, &fPress, &fTemp);

                        (void)sprintf(cBuffer, "%0.2f, %0.1f, \n", fTemp, fPress);
			printf(cBuffer);
		}
	}
}

void PIT_Config(void)
{
	uint32_t        u32LoadValue0, u32LoadValue1;     
	PIT_ConfigType  sPITConfig0, sPITConfig1;    
	PIT_ConfigType  *pPIT_Config1   =&sPITConfig1;
	PIT_ConfigType  *pPIT_Config0   =&sPITConfig0; 

	/* configure PIT module in chain mode */
	/* PIT clock source is bus clock,20MHz */
//	/* PIT channel 0 load value = (1000000-1), channel 1 load value = (20-1) */
//	u32LoadValue0   = 0xF423F;                  /*!< PIT ch0 timer load value  */ 
	/* PIT channel 0 load value = (20000-1), channel 1 load value = (20-1) 50Hz*/
	u32LoadValue0   = 0x4E1F;                  /*!< PIT ch0 timer load value  */ 
	u32LoadValue1   = 0x13;                     /*!< PIT ch1 timer load value  */  
	/* PIT channel 0 load value = (20000-1), channel 1 load value = (5-1) 200Hz*/
//	u32LoadValue0   = 0x4E1F;                  /*!< PIT ch0 timer load value  */ 
//	u32LoadValue1   = 0x4;                     /*!< PIT ch1 timer load value  */ 
//	u32LoadValue1   = 0x63;                     /*!< PIT ch1 timer load value  */    
//	/* PIT channel 0 load value = (100000-1), channel 1 load value = (20-1) 10Hz*/
//	u32LoadValue0   = 0x1869F;                  /*!< PIT ch0 timer load value  */ 
//	u32LoadValue1   = 0x13;                     /*!< PIT ch1 timer load value  */    
//	/* PIT channel 0 load value = (100000-1), channel 1 load value = (20-1) 10Hz*/
//	u32LoadValue0   = 0x3869F;                  /*!< PIT ch0 timer load value  */ 
//	u32LoadValue1   = 0x13;                     /*!< PIT ch1 timer load value  */ 
        
	/* configure PIT channel 1 in chain mode, enable interrupt and timer */
	pPIT_Config1->u32LoadValue      = u32LoadValue1;
	pPIT_Config1->bFreeze           = FALSE;
	pPIT_Config1->bModuleDis        = FALSE;    /*!< enable PIT module */ 
	pPIT_Config1->bInterruptEn      = TRUE;
	pPIT_Config1->bChainMode        = TRUE;
	pPIT_Config1->bTimerEn          = TRUE; 

	/* configure PIT channel 0, only enable timer */    
	pPIT_Config0->u32LoadValue      = u32LoadValue0;
	pPIT_Config0->bFreeze           = FALSE;    
	pPIT_Config0->bModuleDis        = FALSE;    /*!< enable PIT module */     
	pPIT_Config0->bInterruptEn      = FALSE;
	pPIT_Config0->bChainMode        = FALSE; 
	pPIT_Config0->bTimerEn          = TRUE;

	PIT_Init(PIT_CHANNEL0, pPIT_Config0);               
	PIT_Init(PIT_CHANNEL1, pPIT_Config1);

	PIT_SetCallback(PIT_CHANNEL1, PIT_Task);  
}

/*****************************************************************************//*!
*
* @brief PIT module task
*        
* @param none  
*
* @return none 
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void PIT_Task(void)
{
//	GPIO_Toggle(GPIOB, GPIO_PTH2_MASK);

//	if(cInitState == 1)
//	{
//		switch(ntimecount)
//		{
//			case 0;
//				press = ZXP3010D_ResultP();
//				ZXP3010D_StartT();
//				break;

//			case 1:
//				temp = ZXP3010D_ResultT();
//				ZXP3010D_StartP();
//				break;

//			case 2:
//				break;
//			case 3:
//				break;

//			default:
//				
//		}
//	}

	ntimecount ++;
//	if(ntimecount >= 4)
//		ntimecount = 0;
}

/*****************************************************************************//*!
*
* @brief callback routine of RTC driver which does what you want to do at 
*        every RTC period.
*        
* @param  none
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void RTC_Task(void)
{
	/* toggle LED1 */
	/* way 1. toggle LED1 by multiple pin mask */
//	GPIO_Toggle(GPIOB, GPIO_PTH1_MASK);

//	ntimecount ++;
}

/********************************************************************/

void DelayMs(u16 count)
{
	s32 i;
	
	for(;count>0;count--)
	{
		//add your own code which can delay 1ms
		for(i=0; i<3400; i++);
	}
}

