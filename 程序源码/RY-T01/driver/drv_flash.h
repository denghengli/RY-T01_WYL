#ifndef _DRIVER_FLASH_
#define _DRIVER_FLASH_

//#define FLASH_PAGE_SIZE   0x400         //每页FLASH的大小 1K
#define STM32_FLASH_SIZE  128 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_BASE  0x08000000 	//STM32 FLASH的起始地址

#define SINGLE_WORD_LEN 		4
#define SINGLE_WORD_OFFSET   	4

#define FLASH_ADDR_UASER        (STM32_FLASH_BASE + 0x18000) //从FLASH起始位置偏移0x18000开始用于存放用户数据,表示程序空间为96K(BOOT:32K APP:64K)

extern unsigned char  FlashWrite        (unsigned int _iStartAddr,  const unsigned int * _piBuf, unsigned int _iBufSize); 
extern void            FlashRead         (unsigned int * _piBuf, unsigned int _iStartAddr, unsigned int _iBufSize);     
extern unsigned short FlashReadHalfWord (unsigned int _iStartAddr);      
extern unsigned int   FlashReadWord     (unsigned int _iStartAddr);  


#endif


