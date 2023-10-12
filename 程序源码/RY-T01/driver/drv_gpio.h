#ifndef _DRIVER_GPIO_
#define _DRIVER_GPIO_

//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

#define PINMODE_OUT_PP          0x10
#define PINMODE_OUT_OD_UP       0x11
#define PINMODE_OUT_OD_DOWN     0x12
#define PINMODE_IN_UP           0x20
#define PINMODE_IN_DOWN         0x21
#define PINMODE_IN_FLOATING     0x22
#define PINMODE_IN_ANALOG       0x23

/*这里的IO不包括 USART引脚*/
typedef enum 
{
    epin_pt100 = 0,  
    epin_gas_sda1,
    epin_gas_scl1,
    epin_gas_sda2,
    epin_gas_scl2,
    epin_dac_sda,
    epin_dac_scl,
    epin_dac_ldac,
    epin_dac_rdy,
    epin_mes_do,      //测量电磁阀
    epin_blow_do,     //反吹电磁阀
	epin_run,	      //系统运行状态指示灯
    epin_wdt,         //看门狗
    epin_boot1,       //BOOT1
    
}DRV_PIN_NAME_E;


struct DRV_Pin
{
    DRV_PIN_NAME_E     ePinName;
    unsigned int       IORCC;
	GPIO_TypeDef       *GPIOx;
	unsigned int      PinMask;
	
};

extern void MX_GPIO_Init(void);

extern void DRV_Pin_Mode (DRV_PIN_NAME_E  _ePin, unsigned char _cMode);
extern void DRV_Pin_Write(DRV_PIN_NAME_E  _ePin, unsigned char _cValue);
extern unsigned char DRV_Pin_Read(DRV_PIN_NAME_E  _ePin);


#endif	


