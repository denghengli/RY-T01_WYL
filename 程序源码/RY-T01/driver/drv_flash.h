#ifndef _DRIVER_FLASH_
#define _DRIVER_FLASH_

//#define FLASH_PAGE_SIZE   0x400         //ÿҳFLASH�Ĵ�С 1K
#define STM32_FLASH_SIZE  128 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_BASE  0x08000000 	//STM32 FLASH����ʼ��ַ

#define SINGLE_WORD_LEN 		4
#define SINGLE_WORD_OFFSET   	4

#define FLASH_ADDR_UASER        (STM32_FLASH_BASE + 0x18000) //��FLASH��ʼλ��ƫ��0x18000��ʼ���ڴ���û�����,��ʾ����ռ�Ϊ96K(BOOT:32K APP:64K)

extern unsigned char  FlashWrite        (unsigned int _iStartAddr,  const unsigned int * _piBuf, unsigned int _iBufSize); 
extern void            FlashRead         (unsigned int * _piBuf, unsigned int _iStartAddr, unsigned int _iBufSize);     
extern unsigned short FlashReadHalfWord (unsigned int _iStartAddr);      
extern unsigned int   FlashReadWord     (unsigned int _iStartAddr);  


#endif


