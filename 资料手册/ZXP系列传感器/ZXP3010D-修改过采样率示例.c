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


void ZXP3010D_StartP(void)
{
	uint8_t buf[4];
	
	buf[0] = 0xA5;
	buf[1] = 0x11; //输出校准数据
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);

	//以下设置过采样率选择一种设置
	//设置过采样率A6寄存器的Bit0-Bit2位
	//000：1024x;(默认值)
	//001：2048x;
	//010: 4096x;
	//011: 8192x;
	//110: 16384x;
	//111: 327678x
	buf[0] = 0xA6;
	BSP_IIC_Read(ZXP3010D_Address, buf, 1);         //读A6寄存器，返回值到Buf[0]中


	buf[0]=buf[0]&0xF8; 							//保留高5位
	
	//更改采样率为32768x
	buf[1]=buf[0]|0x07;								//A6低三位设置过采样率为32768x
	
	//更改采样率为16384x
	//buf[1]=buf[0]|0x06;							////A6低三位设置过采样率为16384x

	//更改采样率为2048x
	//buf[1]=buf[0]|0x01;							////A6低三位设置过采样率为2048x
	
	buf[0]=0xA6;									//设置A6寄存器地址
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);        //写入A6值

    
	

	buf[0] = 0x30;
	buf[1] = 0x09;
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);
}


