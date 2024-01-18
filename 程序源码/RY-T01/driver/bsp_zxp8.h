#ifndef _BSP_ZXP8_
#define _BSP_ZXP8_
#include "drv_gpio.h"

typedef struct
{
	struct DRV_Pin  SCL;
	struct DRV_Pin  SDA;
	
}_I2C_T;

#define ZXP8_RANG_2KP 1
#define ZXP8_RANG_10KP 2

void getZXPValue(_I2C_T  I2C_Interface, uint8_t rang, float *rp, float *rt);

#endif


