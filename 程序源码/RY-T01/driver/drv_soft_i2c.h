#ifndef _DRIVER_IIC_
#define _DRIVER_IIC_
#include "drv_gpio.h"

//#define CMD_MODE_READ   0x01
//#define CMD_MODE_WRITE  0x00
//
//typedef struct
//{
//    struct DRV_Pin  SCL;
//    struct DRV_Pin  SDA;
//	
//}DRV_I2C_T;

extern void    DRV_SoftI2C_Init(DRV_I2C_T  I2C_Interface);
extern void    DRV_SoftI2C_Start(DRV_I2C_T  I2C_Interface);
extern void    DRV_SoftI2C_Stop (DRV_I2C_T  I2C_Interface);
extern uint8_t DRV_SoftI2C_WriteByteWaiteAck(DRV_I2C_T  I2C_Interface,uint8_t _cByte);
extern uint8_t DRV_SoftI2C_ReadByteWithAck  (DRV_I2C_T  I2C_Interface);
extern uint8_t DRV_SoftI2C_ReadByteWithNoAck(DRV_I2C_T  I2C_Interface);
	

#endif


