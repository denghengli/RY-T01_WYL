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
* File : ZXP3010D.h
*
* Date : 2020/05/25
*
* Revision : 1.0.0
*
* Usage: Sensor Driver for ZXP3010D sensor
*
**************************************************************************/

#ifndef __ZXP3010D_H__
#define __ZXP3010D_H__

#define ZXP3010D_Address					0xDA

#define ZXP3010D_ADC_LSB				0x08
#define ZXP3010D_ADC_LCB				0x07
#define ZXP3010D_ADC_MSB				0x06

#define ZXP3010D_TEMP_LSB				0x0A
#define ZXP3010D_TEMP_MSB				0x09

#define ZXP3010D_STATUS					0x02
#define ZXP3010D_CMD					0x30

#define ZXP3010D_DRDY					0x01

typedef signed int							s32;
typedef unsigned int						u32;
typedef signed short						s16;
typedef unsigned short						u16;
typedef	signed char						s8;
typedef unsigned char						u8;
#ifdef CPU_64Bits
typedef long								s64;
#elif defined(CPU_32Bits)
typedef long long							s64;
#endif

uint8_t ZXP3010D_Initial(void);
void ZXP3010D_StartP(void);
void ZXP3010D_StartT(void);
int32_t ZXP3010D_ResultP(void);
int32_t ZXP3010D_ResultT(void);
uint8_t ZXP3010D_ConStatus(void);
void ZXP3010D_Caculate(int32_t up, int32_t ut, float *rp, float *rt);
void ZXP3010D_Caculate(int32_t up, int32_t ut, float *rp, float *rt);

#endif
