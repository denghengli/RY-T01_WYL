#include  "includes.h"
#include  "stm32f1xx_hal_flash.h"
#include  "stm32f1xx_hal_flash_ex.h"



/**********************************************************************************************************
*                                          FlashReadHalfWord
*
* @Description : 读单片机FLASH中指点地址的半字
* @Arguments   : _iStartAddr 指定的地址
* @Returns     : 读取的半字
**********************************************************************************************************/	
unsigned short FlashReadHalfWord (unsigned int _iStartAddr)
{
	return *(unsigned short*)_iStartAddr; 
}

/**********************************************************************************************************
*                                          FlashReadWord
*
* @Description : 读单片机FLASH中指点地址的字
* @Arguments   : _iStartAddr 指定的地址
* @Returns     : 读取的字
**********************************************************************************************************/	
unsigned int FlashReadWord (unsigned int _iStartAddr)
{
	return *(unsigned int*)_iStartAddr; 
}


/**
  * @brief  Erases sector.
  * @param  Add: Address of sector to be erased.
  * @retval 0 if operation is successeful, MAL_FAIL else.
  */
HAL_StatusTypeDef Flash_If_Erase(unsigned int Add)
{
  uint32_t PageError = 0;
  /* Variable contains Flash operation status */
  HAL_StatusTypeDef status;
  FLASH_EraseInitTypeDef eraseinitstruct;

  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR);

   /* Get the number of sector to erase from 1st sector*/
  eraseinitstruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  eraseinitstruct.PageAddress = Add;
  eraseinitstruct.NbPages = 1;
  status = HAL_FLASHEx_Erase(&eraseinitstruct, &PageError);

  if (status != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}


/**********************************************************************************************************
*                                          FlashWrite
*
* @Description : 写FLASH
* @Arguments   : _iStartAddr 写入的起始地址
*              : _piBuf      写入的数据buff（字）
*              : _iBufSize   写入的数据长度（字）
* @Returns     : SUCESS      写入成功
*              : FAIL        写入失败
**********************************************************************************************************/	
unsigned char FlashWrite(unsigned int _iStartAddr,  const unsigned int * _piBuf, unsigned int _iBufSize)
{
	volatile HAL_StatusTypeDef FLASHStatus = HAL_ERROR;
	unsigned char	cRet = 1;

	unsigned int	i	= 0;
	unsigned int	j	= 0;

	unsigned char	cRes = 0;
	unsigned int	iStartAddr    = _iStartAddr; //写入的起始地址
	unsigned int	iEndAddr      = iStartAddr + _iBufSize * SINGLE_WORD_LEN; //写入的结束地址

		
	HAL_FLASH_Unlock();
	
	//判断地址是否合法
	if(iStartAddr < STM32_FLASH_BASE || iStartAddr >= (STM32_FLASH_BASE + 1024*STM32_FLASH_SIZE))
	{
		cRet = 0;
		goto Finish;
	}
	
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	
	FLASHStatus  = HAL_OK; //操作完成
   
	//看是否需要擦除此页，对非FFFFFFFF的地方,先擦除
	while(iStartAddr < iEndAddr)
	{
		if(FlashReadWord(iStartAddr) != 0xFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
		{
			FLASHStatus = Flash_If_Erase(iStartAddr);
			if(FLASHStatus == HAL_OK)//写入成功
			{
				iStartAddr += SINGLE_WORD_OFFSET;
			}
		}
		else
		{
			iStartAddr += SINGLE_WORD_OFFSET;
		}
		FLASH_WaitForLastOperation(2);
	}

	// Wr flash.
	FLASH_WaitForLastOperation(2);
	iStartAddr   =  _iStartAddr; //写入的起始地址,因擦除的时候累加了,所以得从新赋值
	while ((iStartAddr < iEndAddr) && (FLASHStatus == HAL_OK))
	{
		FLASHStatus = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, iStartAddr,_piBuf[i++]);
		if(FLASHStatus == HAL_OK)
		{
			iStartAddr	 += SINGLE_WORD_OFFSET;
		}
		else
		{
			cRet	=  0;
			goto Finish;
		}
	}

	// Chk wr result.
	iStartAddr   =  _iStartAddr; //写入的起始地址,因擦除的时候累加了,所以得从新赋值
	while ((iStartAddr < iEndAddr) && (cRes == 0))
	{
		if (FlashReadWord(iStartAddr) != _piBuf[j++])
		{
			cRes				= 1;
			cRet				= 0;
			goto Finish;
		}
		iStartAddr	 += SINGLE_WORD_OFFSET;
	}
	
	Finish:
	
	HAL_FLASH_Lock();//上锁
	return cRet;
}

/**********************************************************************************************************
*                                          FlashRead
*
* @Description : 读FLASH
* @Arguments   : _piBuf       读取存放的buff
*              : _iStartAddr  读取FLASH的起始地址
*              : _iBufSize    读取数据的长度(字)
* @Returns     : none
**********************************************************************************************************/	
void FlashRead(unsigned int * _piBuf, unsigned int _iStartAddr, unsigned int _iBufSize)
{
	unsigned int	i	= 0;

	unsigned int	iStartAddr = _iStartAddr;
	unsigned int	iEndAddr = iStartAddr + _iBufSize * SINGLE_WORD_LEN;

	while ((iStartAddr < iEndAddr) && (i < _iBufSize))
	{
		_piBuf[i++] 		= FlashReadWord (iStartAddr);
		iStartAddr			+= SINGLE_WORD_OFFSET;
	}
}








