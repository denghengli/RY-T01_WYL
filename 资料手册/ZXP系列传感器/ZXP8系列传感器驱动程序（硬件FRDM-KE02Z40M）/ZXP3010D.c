***************************************************************************
*
* BEIJING ISENSING TECHNOLOGY CO.,LTD.01060974500
* (c) Copyright 2020 BEIJING ISENSING TECHNOLOGY CO.,LTD.
* ALL RIGHTS RESERVED.
*
***************************************************************************
/*
****************************************************************************
*
* File : ZXP3010D.c
*
* Date : 2020/06/17
*
* Revision : 1.0.0
*
* Usage: Sensor Driver for ZXP3010D sensor
*
**************************************************************************/
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include  "ZXP3010D.h"
#include "gpio.h"
#include "sys.h"


void _DelayMs(uint16_t count);
void ZXP3010D_StartP(void)
{
	uint8_t buf[4];
	
	buf[0] = 0xA5;
	//buf[1] = 0x13;输出原始ADC值
        buf[1] = 0x11; //输出校准数据
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);

	buf[0] = 0x30;
	buf[1] = 0x09;
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);
}

void ZXP3010D_StartT(void)
{
	uint8_t buf[4];
	
	buf[0] = 0xA5;
	buf[1] = 0x01;
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);

	buf[0] = 0x30;
	buf[1] = 0x08;
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);
}

int32_t ZXP3010D_ResultP(void)
{
	int32_t ltemp;

	uint8_t buf[4];
	
	buf[0] = 0x06;

	BSP_IIC_Read(ZXP3010D_Address, buf, 3);

	ltemp = buf[0] << 8;
	ltemp |= buf[1];
	ltemp <<= 8;
	ltemp |= buf[2];
        
	return(ltemp);
}

int32_t ZXP3010D_ResultT(void)
{
	int32_t ltemp;

	uint8_t buf[4];
	
	buf[0] = 0x09;

	BSP_IIC_Read(ZXP3010D_Address, buf, 2);

	ltemp = buf[0] << 8;
	ltemp |= buf[1];

	return(ltemp);
}

uint8_t ZXP3010D_ConStatus(void)
{
	u8 status;

	uint8_t buf[4];

	buf[0] = ZXP3010D_CMD;
	BSP_IIC_Read(ZXP3010D_Address, buf, 1);
	status= (buf[0]>>3)&0x01;
	return status;
}

void ZXP3010D_Caculate(int32_t up, int32_t ut, float *rp, float *rt)
{
	float fp, ft, ftemp;
	
///////////////////////////////////////////////////  温度值正负判断       
        ft = ut ;
        if(ft >= pow(2.0, 15.0))
	
           ft =ft - pow(2.0, 16.0);
              
 ///////////////////////////////////////////////////       
       
 /////////////////////////////////////////////////    压力值正负判断   
         ftemp = up;
         if(ftemp >= pow(2.0, 23.0))
	
           ftemp =ftemp - pow(2.0, 24.0);
 //////////////////////////////////////////////////    
     ftemp= ftemp/pow(2.0, 12.0); //n=13  1Kpa// n=14  500pa//12 2kpa  //11 4kpa; // 9 10kpa //  6  100kpa            
        
        ft =  ft / 256.0;
        fp = ftemp;                                    
	*rp = fp;
	*rt = ft;
}

void _DelayMs(uint16_t count)
{
	int32_t i;
	
	for(;count>0;count--)
	{
		//add your own code which can delay 1ms
		for(i=0; i<3400; i++);
	}
}

