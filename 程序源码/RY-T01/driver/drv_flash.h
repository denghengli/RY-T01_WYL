/*
 * File      : dr_flash.h
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-04-17     denghengli   first version
 */

#ifndef _DRIVER_FLASH_
#define _DRIVER_FLASH_

	 
/**
 * @ingroup BasicDef
 *
 * @def ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. ALIGN(13, 4)
 * would return 16.
 */
#define ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

#define STM32_FLASH_SIZE       512 	 		/*��ѡSTM32��FLASH������С(��λΪK)*/
#define STM32_FLASH_BASE       0x08000000 	/*STM32 FLASH����ʼ��ַ*/
#define STM32_FLASH_END        (STM32_FLASH_BASE + STM32_FLASH_SIZE * 1024) 
#define STM32_FLASH_PAGE_SIZE  0x800       /* 2 Kbytes */
#define STM32_FLASH_BOOT_SIZE  (32 * 1024) /*BOOT��С*/	
#define STM32_FLASH_APP_SIZE   (96 * 1024) /*Ӧ�ó����С*/	 
#define STM32_FLASH_ADDR_BOOT  (STM32_FLASH_BASE)  /*BOOT�����ַ,��СΪ32K*/
#define STM32_FLASH_ADDR_APP   (STM32_FLASH_BASE + STM32_FLASH_BOOT_SIZE)  /*Ӧ�ó����ַ,��СΪ96K*/
#define STM32_FLASH_ADDR_OTA   (STM32_FLASH_BASE + STM32_FLASH_BOOT_SIZE + STM32_FLASH_APP_SIZE) /*�����̼���ַ,��СΪ96K*/
#define STM32_FLASH_ADDR_PARA_FACTORY  (STM32_FLASH_BASE + 496 * 1024) /*�������ݴ洢��ַ,��СΪ4K*/
#define STM32_FLASH_ADDR_PARA          (STM32_FLASH_BASE + 500 * 1024) /*�����洢��ַ,��СΪ4K*/
#define STM32_FLASH_ADDR_PARA_MODFYFLG (STM32_FLASH_BASE + 504 * 1024) /*�����޸ı�־�洢��ַ,��СΪ2K*/
	 
extern int stm32_flash_erase(uint32_t addr, uint32_t size);
extern int stm32_flash_write(uint32_t addr, const uint8_t *buf, uint32_t size);
extern int stm32_flash_write_calchecksum(uint32_t addr, const uint8_t *buf, uint32_t size, uint32_t *checksum);
extern int stm32_flash_read (uint32_t addr, uint8_t *buf, uint32_t size);
extern int stm32_flash_checksum(uint32_t addr, uint32_t size);


#endif


