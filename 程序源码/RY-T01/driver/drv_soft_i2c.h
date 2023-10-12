/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-03     Denghengli   the first version
 */
#ifndef _DRIVER_SOFTI2C_
#define _DRIVER_SOFTI2C_
#include "drv_gpio.h"

#define CMD_MODE_READ   0x01
#define CMD_MODE_WRITE  0x00

struct soft_i2c_config
{
	struct DRV_Pin  SCL;
	struct DRV_Pin  SDA;
}









#endif
