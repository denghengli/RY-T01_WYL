#ifndef _SYS_H
#define _SYS_H

//#define SDA_SetOutput()					GPIO_PinInit(GPIO_PTA2, GPIO_PinOutput)
//#define SDA_SetInput()					GPIO_PinInit(GPIO_PTA2, GPIO_PinInput)

//#define SDA_SetVal()						GPIO_PinSet(GPIO_PTA2)
//#define SDA_ClrVal()						GPIO_PinClear(GPIO_PTA2)

//#define SDA_GetVal()						(GPIO_Read(GPIOA) & GPIO_PTA2_MASK)

//#define SCL_SetOutput()					GPIO_PinInit(GPIO_PTA3, GPIO_PinOutput)
//#define SCL_SetInput()					GPIO_PinInit(GPIO_PTA3, GPIO_PinInput)

//#define SCL_SetVal()						GPIO_PinSet(GPIO_PTA3)
//#define SCL_ClrVal()						GPIO_PinClear(GPIO_PTA3)

//#define BPS_PwrSetOutput()				GPIO_PinInit(GPIO_PTB2, GPIO_PinOutput)
//#define BPS_PwrSetInput()				GPIO_PinInit(GPIO_PTB2, GPIO_PinInput)

//#define BPS_PwrSetVal()					GPIO_PinSet(GPIO_PTB2)
//#define BPS_PwrClrVal()					GPIO_PinClear(GPIO_PTB2)

#define SDA_SetOutput()					GPIO_PinInit(GPIO_PTE1, GPIO_PinOutput)
#define SDA_SetInput()					GPIO_PinInit(GPIO_PTE1, GPIO_PinInput)

#define SDA_SetVal()						GPIO_PinSet(GPIO_PTE1)
#define SDA_ClrVal()						GPIO_PinClear(GPIO_PTE1)

#define SDA_GetVal()						(GPIO_Read(GPIOB) & GPIO_PTE1_MASK)

#define SCL_SetOutput()					GPIO_PinInit(GPIO_PTE0, GPIO_PinOutput)
#define SCL_SetInput()					GPIO_PinInit(GPIO_PTE0, GPIO_PinInput)

#define SCL_SetVal()						GPIO_PinSet(GPIO_PTE0)
#define SCL_ClrVal()						GPIO_PinClear(GPIO_PTE0)

#define BPS_PwrSetOutput()				GPIO_PinInit(GPIO_PTE2, GPIO_PinOutput)
#define BPS_PwrSetInput()				GPIO_PinInit(GPIO_PTE2, GPIO_PinInput)

#define BPS_PwrSetVal()					GPIO_PinSet(GPIO_PTE2)
#define BPS_PwrClrVal()					GPIO_PinClear(GPIO_PTE2)

#endif

