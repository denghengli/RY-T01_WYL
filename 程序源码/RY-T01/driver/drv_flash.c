#include  "includes.h"
#include  "stm32f1xx_hal_flash.h"
#include  "stm32f1xx_hal_flash_ex.h"



/**********************************************************************************************************
*                                          FlashReadHalfWord
*
* @Description : ����Ƭ��FLASH��ָ���ַ�İ���
* @Arguments   : _iStartAddr ָ���ĵ�ַ
* @Returns     : ��ȡ�İ���
**********************************************************************************************************/	
unsigned short FlashReadHalfWord (unsigned int _iStartAddr)
{
	return *(unsigned short*)_iStartAddr; 
}

/**********************************************************************************************************
*                                          FlashReadWord
*
* @Description : ����Ƭ��FLASH��ָ���ַ����
* @Arguments   : _iStartAddr ָ���ĵ�ַ
* @Returns     : ��ȡ����
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
* @Description : дFLASH
* @Arguments   : _iStartAddr д�����ʼ��ַ
*              : _piBuf      д�������buff���֣�
*              : _iBufSize   д������ݳ��ȣ��֣�
* @Returns     : SUCESS      д��ɹ�
*              : FAIL        д��ʧ��
**********************************************************************************************************/	
unsigned char FlashWrite(unsigned int _iStartAddr,  const unsigned int * _piBuf, unsigned int _iBufSize)
{
	volatile HAL_StatusTypeDef FLASHStatus = HAL_ERROR;
	unsigned char	cRet = 1;

	unsigned int	i	= 0;
	unsigned int	j	= 0;

	unsigned char	cRes = 0;
	unsigned int	iStartAddr    = _iStartAddr; //д�����ʼ��ַ
	unsigned int	iEndAddr      = iStartAddr + _iBufSize * SINGLE_WORD_LEN; //д��Ľ�����ַ

		
	HAL_FLASH_Unlock();
	
	//�жϵ�ַ�Ƿ�Ϸ�
	if(iStartAddr < STM32_FLASH_BASE || iStartAddr >= (STM32_FLASH_BASE + 1024*STM32_FLASH_SIZE))
	{
		cRet = 0;
		goto Finish;
	}
	
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	
	FLASHStatus  = HAL_OK; //�������
   
	//���Ƿ���Ҫ������ҳ���Է�FFFFFFFF�ĵط�,�Ȳ���
	while(iStartAddr < iEndAddr)
	{
		if(FlashReadWord(iStartAddr) != 0xFFFFFFFF)//�з�0XFFFFFFFF�ĵط�,Ҫ�����������
		{
			FLASHStatus = Flash_If_Erase(iStartAddr);
			if(FLASHStatus == HAL_OK)//д��ɹ�
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
	iStartAddr   =  _iStartAddr; //д�����ʼ��ַ,�������ʱ���ۼ���,���Եô��¸�ֵ
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
	iStartAddr   =  _iStartAddr; //д�����ʼ��ַ,�������ʱ���ۼ���,���Եô��¸�ֵ
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
	
	HAL_FLASH_Lock();//����
	return cRet;
}

/**********************************************************************************************************
*                                          FlashRead
*
* @Description : ��FLASH
* @Arguments   : _piBuf       ��ȡ��ŵ�buff
*              : _iStartAddr  ��ȡFLASH����ʼ��ַ
*              : _iBufSize    ��ȡ���ݵĳ���(��)
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








