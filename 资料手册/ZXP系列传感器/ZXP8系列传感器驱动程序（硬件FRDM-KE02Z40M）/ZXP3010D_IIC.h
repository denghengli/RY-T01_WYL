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
* ZXP3010D_IIC.h
* Date: 2020/06/17
* Revision: 1.0.0
*
* Usage: IIC read and write interface
*
**************************************************************************/
#ifndef __ZXP3010D_IIC_H_
#define __ZXP3010D_IIC_H_

#include "ZXP3010D.h"

void BSP_IIC_Initial(void);
u8 BSP_IIC_Write(u8 IIC_Address, u8 *buffer, u16 count);
u8 BSP_IIC_Read(u8 IIC_Address, u8 *buffer, u16 count);

#endif
