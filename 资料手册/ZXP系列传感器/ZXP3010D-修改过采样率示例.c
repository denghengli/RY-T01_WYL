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
	buf[1] = 0x11; //���У׼����
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);

	//�������ù�������ѡ��һ������
	//���ù�������A6�Ĵ�����Bit0-Bit2λ
	//000��1024x;(Ĭ��ֵ)
	//001��2048x;
	//010: 4096x;
	//011: 8192x;
	//110: 16384x;
	//111: 327678x
	buf[0] = 0xA6;
	BSP_IIC_Read(ZXP3010D_Address, buf, 1);         //��A6�Ĵ���������ֵ��Buf[0]��


	buf[0]=buf[0]&0xF8; 							//������5λ
	
	//���Ĳ�����Ϊ32768x
	buf[1]=buf[0]|0x07;								//A6����λ���ù�������Ϊ32768x
	
	//���Ĳ�����Ϊ16384x
	//buf[1]=buf[0]|0x06;							////A6����λ���ù�������Ϊ16384x

	//���Ĳ�����Ϊ2048x
	//buf[1]=buf[0]|0x01;							////A6����λ���ù�������Ϊ2048x
	
	buf[0]=0xA6;									//����A6�Ĵ�����ַ
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);        //д��A6ֵ

    
	

	buf[0] = 0x30;
	buf[1] = 0x09;
	BSP_IIC_Write(ZXP3010D_Address, buf, 2);
}


