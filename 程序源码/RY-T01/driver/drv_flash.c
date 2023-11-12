/*
 * File      : drv_flash.h
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-04-17     denghengli   first version
 */

#include  "includes.h"

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t addr)
{
    uint32_t page = 0;
    page = ALIGN_DOWN(addr, STM32_FLASH_PAGE_SIZE);
    return page;
}

/**
 * Read data from flash.
 * @note This operation's units is word.
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result 0:fial, >0:success
 */
int stm32_flash_read(uint32_t addr, uint8_t *buf, uint32_t size)
{
    uint32_t addrx,i;

    addrx = addr;
      
    if ((addrx + size) > STM32_FLASH_END)
    {
        return 0;
    }

    for (i = 0; i < size; i++, buf++, addrx++)
    {
        *buf = *(uint8_t *) addrx;
    }

    return size;
}


int stm32_flash_checksum(uint32_t addr, uint32_t size)
{
    uint32_t addrx,i;
    uint32_t checksum = 0;
    
    addrx = addr;
      
    if ((addrx + size) > STM32_FLASH_END)
    {
        return 0;
    }

    for (i = 0; i < size; i+=4, addrx+=4)
    {
        checksum += *(uint32_t *) addrx;
    }

    return checksum;
}

/**
 * Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result 0:success, 1:fail
 */
int stm32_flash_erase(uint32_t addr, uint32_t size)
{
    uint8_t  result = 0;
    uint32_t PAGEError = 0;
    
    /*Variable used for Erase procedure*/
    FLASH_EraseInitTypeDef EraseInitStruct;

    /* Clear OPTVERR bit set on virgin samples */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);
    
    if ((addr + size) > STM32_FLASH_END)
    {
        return 1;
    }

    __disable_interrupt();
    HAL_FLASH_Unlock();

    /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = GetPage(addr);
    EraseInitStruct.NbPages     = (size + STM32_FLASH_PAGE_SIZE - 1) / STM32_FLASH_PAGE_SIZE;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
    {
        result = 1;
    }
  
    HAL_FLASH_Lock();
    __enable_interrupt();

    return result;
}

/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result 0:success, 1:fail
 */
int stm32_flash_write(uint32_t addr, const uint8_t *buf, uint32_t size)
{
    uint8_t  result = 0;
    uint32_t addrx = addr;
    uint32_t end_addr = addr + size;
    uint32_t PAGEError = 0;
    /*Variable used for Erase procedure*/
    HAL_StatusTypeDef FlashStatus = HAL_OK;
    FLASH_EraseInitTypeDef EraseInitStruct;
    
    if (addrx % 4 != 0)
    {
        return 1;
    }

    if ((end_addr) > STM32_FLASH_END)
    {
        return 1;
    }

    __disable_interrupt();
    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
    //erase flash
    while (addrx < end_addr)
    {
        if (*(uint32_t *)addrx != 0xFFFFFFFF)
        {
            EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
            EraseInitStruct.PageAddress = GetPage(addrx);
            EraseInitStruct.NbPages     = (size + STM32_FLASH_PAGE_SIZE - 1) / STM32_FLASH_PAGE_SIZE;

            if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) == HAL_OK)
            {
                addrx += 4;
            }
        }
        else
        {
            addrx += 4;
        }
        FLASH_WaitForLastOperation(2);
    }
    
    //write and check flash
    addrx = addr;
    FlashStatus = FLASH_WaitForLastOperation(2);
	if(FlashStatus == HAL_OK)
    {
        while (addrx < end_addr)
        {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addrx, *((uint32_t *)buf)) == HAL_OK)
            {
                //FLASH_WaitForLastOperation(2);
                if (*(uint32_t *)addrx != *(uint32_t *)buf)
                {
                    result = 1;
                    goto __exit;
                }
                addrx += 4;
                buf  += 4;
            }
            else
            {
                result = 1;
                goto __exit;
            }
        }
    }
    else
    {
        result = 1;
        goto __exit;
    }
    
__exit:
  
    HAL_FLASH_Lock();
    __enable_interrupt();

    return result;
}

/**
 * Write data to flash.
 * @note This operation's units is word.
 * @note This operation must after erase. @see flash_erase.
 *
 * @param addr flash address
 * @param buf the write data buffer
 * @param size write bytes size
 *
 * @return result 0:success, 1:fail
 */
int stm32_flash_write_calchecksum(uint32_t addr, const uint8_t *buf, uint32_t size, uint32_t *checksum)
{
    uint8_t  result = 0;
    uint32_t addrx = addr;
    uint32_t end_addr = addr + size;
    uint32_t PAGEError = 0;
    uint32_t checksum_temp = 0;
    /*Variable used for Erase procedure*/
    HAL_StatusTypeDef FlashStatus = HAL_OK;
    FLASH_EraseInitTypeDef EraseInitStruct;
    
    if (addrx % 4 != 0)
    {
        return 1;
    }

    if ((end_addr) > STM32_FLASH_END)
    {
        return 1;
    }

    __disable_interrupt();
    HAL_FLASH_Unlock();

    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
    
    //erase flash
    while (addrx < end_addr)
    {
        if (*(uint32_t *)addrx != 0xFFFFFFFF)
        {
            EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
            EraseInitStruct.PageAddress = GetPage(addrx);
            EraseInitStruct.NbPages     = (size + STM32_FLASH_PAGE_SIZE - 1) / STM32_FLASH_PAGE_SIZE;

            if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) == HAL_OK)
            {
                addrx += 4;
            }
        }
        else
        {
            addrx += 4;
        }
        FLASH_WaitForLastOperation(2);
    }
    
    //write and check flash
    addrx = addr;
    FlashStatus = FLASH_WaitForLastOperation(2);
	if(FlashStatus == HAL_OK)
    {
        while (addrx < end_addr)
        {
            if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addrx, *((uint32_t *)buf)) == HAL_OK)
            {
                //FLASH_WaitForLastOperation(2);
                if (*(uint32_t *)addrx != *(uint32_t *)buf)
                {
                    result = 1;
                    goto __exit;
                }
                checksum_temp += *(uint32_t *)buf;
                addrx += 4;
                buf  += 4;
            }
            else
            {
                result = 1;
                goto __exit;
            }
        }
    }
    else
    {
        result = 1;
        goto __exit;
    }
    
__exit:
  
    HAL_FLASH_Lock();
    __enable_interrupt();

    if(!result) *checksum = checksum_temp;
    else *checksum = checksum_temp;
    
    return result;
}

