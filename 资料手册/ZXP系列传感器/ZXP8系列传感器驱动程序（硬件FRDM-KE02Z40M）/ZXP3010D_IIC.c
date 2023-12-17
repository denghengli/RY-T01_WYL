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
* ZXP3010D_IIC.c
* Date: 2020/06/17
* Revision: 1.0.0
*
* Usage: IIC read and write interface
*
**************************************************************************/
#include "gpio.h"
#include "sys.h"
#include "ZXP3010D.h"

void _BSP_IIC_Start(void);
void _BSP_IIC_Stop(void);
void _BSP_IIC_ByteWrite(uint8_t dat);
uint8_t _BSP_IIC_ByteRead(uint8_t ack);
void _Delay_us(int32_t dly);

u8 BSP_IIC_Write(u8 IIC_Address, u8 *buffer, u16 count)
{
	//add your own code which send data via iic
	uint8_t i;
	uint8_t temp;

	temp = IIC_Address;

	_BSP_IIC_Start();
	_BSP_IIC_ByteWrite(temp);

	for(i=0; i<count; i++)
	{
		_BSP_IIC_ByteWrite(buffer[i]);
	}

	_BSP_IIC_Stop();
	
	return 0;
}

u8 BSP_IIC_Read(u8 IIC_Address,u8 *buffer,u16 count)
{
	//add your own code which receive data via iic
	uint8_t i;
	uint8_t tag;
	uint8_t temp;

	temp = IIC_Address;
	_BSP_IIC_Start();
	_BSP_IIC_ByteWrite(temp);
	_BSP_IIC_ByteWrite(buffer[0]);
	
	_BSP_IIC_Start();
	temp = IIC_Address;
	temp |= 0x01;
	_BSP_IIC_ByteWrite(temp);
	for(i=0; i<count; i++)
	{
		tag = (i==count-1) ? 0 : 1;
		buffer[i] = _BSP_IIC_ByteRead(tag);
	}

	_BSP_IIC_Stop();
	
	return 0;
}

void BSP_IIC_Initial(void)
{
	SDA_SetOutput();
	SCL_SetOutput();
	BPS_PwrSetOutput();
	SDA_SetVal();
	SCL_SetVal();
	BPS_PwrSetVal();
}

void _BSP_IIC_Start(void)
{
	SCL_SetVal();
	SDA_SetVal();
	_Delay_us(2);
	SDA_ClrVal();
	_Delay_us(2);
	SCL_ClrVal();
	_Delay_us(2);
}

void _BSP_IIC_Stop(void)
{
	SCL_ClrVal();
	SDA_ClrVal();
	_Delay_us(2);
	SCL_SetVal();
	_Delay_us(2);
	SDA_SetVal();
	_Delay_us(2);
}

void _BSP_IIC_ByteWrite(uint8_t dat)
{
	uint8_t i;

	for(i=0; i<8; i++)
	{
		if(dat & 0x80)
		{
			SDA_SetVal();
		}
		else
		{
			SDA_ClrVal();
		}
		dat <<= 1;
		_Delay_us(2);
		SCL_SetVal();
		_Delay_us(2);
		SCL_ClrVal();
		_Delay_us(2);
	}

	SDA_SetInput();
	SCL_SetVal();
	_Delay_us(2);
	SCL_ClrVal();
	_Delay_us(2);
	SDA_SetOutput();
}

uint8_t _BSP_IIC_ByteRead(uint8_t ack)
{
	uint8_t i;
	uint8_t dat;

	dat = 0;
	SDA_SetInput();
	for(i=0; i<8; i++)
	{
		SCL_SetVal();
		_Delay_us(2);
		dat <<= 1;
		if(SDA_GetVal())
			dat |= 1;
		SCL_ClrVal();
		_Delay_us(2);
	}

	SDA_SetOutput();
	if(ack == 1)
	{
		SDA_ClrVal();
	}
	else
	{
		SDA_SetVal();
	}
	_Delay_us(2);
	SCL_SetVal();
	_Delay_us(2);
	SCL_ClrVal();
	_Delay_us(2);

	return(dat);
}

void _Delay_us(int32_t dly)
{
	int32_t i;
	
	if(dly > 0)
	{
		i = dly;
		dly <<= 2;
		dly += i;
	}
	do
	{
		dly--;
	}while(dly > 0);
}

