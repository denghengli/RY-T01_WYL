/*
 * File       : modbus_slave.c
 * Function   : ���ڽ���������յ�����������
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-17     denghengli   first version
 */

#include "includes.h"

/*��������*/
static void Slave_Rd_Hold_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Rd_IPut_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Rd_Coil_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Wr_SigHold_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Wr_MulHold_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Wr_SigCoil_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Wr_MulCoil_Proc(UART_TRANS_T* _ptTrans);
static void Slave_AddCRC(UART_TRANS_T* _ptTrans);
static void Slave_SetAckErr(UART_TRANS_T* _ptTrans,uint8_t _ucErrCode);

/*���ݸ�ʽΪAB��ABCD*/
static uint16_t SlaveReg_Hold[SLAVE_HOLDREG_NUM];
static uint16_t SlaveReg_Iput[SLAVE_IPUTREG_NUM];
static uint8_t SlaveReg_Coil[SLAVE_COILREG_NUM];


/**********************************************************************************************************
*	�� �� ��: Slave_IsRecvDone
*	����˵��: ���ж��н���һ֡�������ݰ�
*     ˵�������ַ�ʽû�в��ö�ʱ3.5�ַ��б�֡���������ܱ߽��ձ߽���
*           ÿ�ֹ�������շ�ʽ��һ����ֻ���ڽ������������ٽ���
*	��    ��: _ptTrans ������
*           ��_cDat    ����
*	�� �� ֵ: 1 ������ɣ�0 δ�������
**********************************************************************************************************/
uint8_t Slave_IsRecvDone(UART_TRANS_T* _ptTrans,uint8_t _cDat)
{
	uint8_t cRet = 0;
	uint16_t shRxCRC=0,shCalcCRC=0;
	uint16_t shDevId = SlaveId_Get();
    
	if(_ptTrans->RxCount < MODBUS_RECV_ELN_MAX)
	{
		_ptTrans->RxBuf[_ptTrans->RxCount ++] = _cDat;
		
        /* �豸ID��֤ */
        if(_ptTrans->RxCount == 1)
        {
            if((_ptTrans->RxBuf[0] != shDevId) && (_ptTrans->RxBuf[0] != MODBUS_ADDR_BROAD))
            {
                memset(_ptTrans,0,sizeof(UART_TRANS_T));
                _ptTrans->RxNewFlag = 1;
                return cRet;
            }
        }
        
        /* ��������֤ */
        if(_ptTrans->RxCount == 2)
        {
            if(_ptTrans->RxBuf[1] != MODBUS_COIL_REG_RD && _ptTrans->RxBuf[1] != MODBUS_HOLD_REG_RD  && \
               _ptTrans->RxBuf[1] != MODBUS_IPUT_REG_RD && _ptTrans->RxBuf[1] != MODBUS_SIMCOIL_REG_WR && \
               _ptTrans->RxBuf[1] != MODBUS_SIMHOLD_REG_WR && _ptTrans->RxBuf[1] != MODBUS_MULHOLD_REG_WR)
            {
                memset(_ptTrans,0,sizeof(UART_TRANS_T));
                _ptTrans->RxNewFlag = 1;
                return cRet;
            }
        }       
	} 
	else
	{
		memset(_ptTrans,0,sizeof(UART_TRANS_T));
        _ptTrans->RxNewFlag = 1;
        return cRet;
	}
	
	/*������Ϊ1 3 4 5 6ʱ�����յ�8�ֽ�ʱ��ʾһ֡�������*/
	if(_ptTrans->RxBuf[1] == MODBUS_COIL_REG_RD || _ptTrans->RxBuf[1] == MODBUS_HOLD_REG_RD  || 
	   _ptTrans->RxBuf[1] == MODBUS_IPUT_REG_RD || _ptTrans->RxBuf[1] == MODBUS_SIMCOIL_REG_WR || 
	   _ptTrans->RxBuf[1] == MODBUS_SIMHOLD_REG_WR)
	{
		if(_ptTrans->RxCount == 8)
		{
			cRet = 1;
		}
	}
	/*������Ϊ10Hʱ�����յ�9�ֽ� + stModsRxTx.RxBuf[6]ʱ��ʾһ֡�������*/
	else if(_ptTrans->RxBuf[1] == MODBUS_MULHOLD_REG_WR)
	{
		if(_ptTrans->RxCount == (9 + _ptTrans->RxBuf[6]))
		{
			cRet = 1;
		}
	}
	/*������Ϊ0FHʱ�����յ�9�ֽ� + stModsRxTx.RxBuf[6]ʱ��ʾһ֡�������*/
	else if(_ptTrans->RxBuf[1] == MODBUS_MULCOIL_REG_WR)
	{
		if(_ptTrans->RxCount == (9 + _ptTrans->RxBuf[6]))
		{
			cRet = 1;
		}
	}

    /*CRCУ�飻������ֱ�Ӷ���*/
    if (cRet == 1)
    {
    	shCalcCRC = ModsCRC16(_ptTrans->RxBuf,  _ptTrans->RxCount - 2);
    	shRxCRC   = Mods_ULUH_TO_U16(_ptTrans->RxBuf + _ptTrans->RxCount - 2);
    	if(shRxCRC != shCalcCRC)
    	{
    		memset(_ptTrans,0,sizeof(UART_TRANS_T));
    		_ptTrans->RxNewFlag = 1;
    		return 0;
    	}
    }
	
	return cRet;
}




/**********************************************************************************************************
*	�� �� ��: Slave_AddCRC
*	����˵��: ׷��2�ֽ�CRC��stModsRxTx.TxBuf;
*	��    ��: _ptTrans ������
*	�� �� ֵ: NONE
**********************************************************************************************************/
static void Slave_AddCRC(UART_TRANS_T* _ptTrans)
{
	uint16_t shTxCRC;
	
	shTxCRC = ModsCRC16(_ptTrans->TxBuf,_ptTrans->TxCount);
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = shTxCRC; //��д��
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = shTxCRC >> 8;	
}

/**********************************************************************************************************
*	�� �� ��: Slave_SetAckErr
*	����˵��: ����쳣֡��Ϣ
*	��    ��: _ptTrans ������
*             _ucLen  ���ݳ���
*	�� �� ֵ: NONE
**********************************************************************************************************/
static void Slave_SetAckErr (UART_TRANS_T* _ptTrans,uint8_t _ucErrCode)
{
	_ptTrans->TxRspCode = _ucErrCode;
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1] | 0x80;
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->TxRspCode;
}

/**********************************************************************************************************
*	�� �� ��: Slave_Pack_Analysis
*	����˵��: �������н���MODBUS,���ڽ���ͨ��DMA��ʽ���յ�����MODBUS����
*	��    ��: _ptTrans ������
*	�� �� ֵ: 1:�����ɹ�  0:����ʧ��
**********************************************************************************************************/
uint8_t Slave_Pack_Analysis(UART_TRANS_T* _ptTrans)
{
    uint16_t shRxCRC=0,shCalcCRC=0;
    uint16_t shDevId = SlaveId_Get();
	
	/*��ַУ�飻������ֱ�Ӷ���*/
	if((_ptTrans->RxBuf[0] != shDevId) && (_ptTrans->RxBuf[0] != MODBUS_ADDR_BROAD))
	{
		memset(_ptTrans,0,sizeof(UART_TRANS_T));
		//_ptTrans->RxNewFlag = 1;
		return 0;
	}
	
	/*������У�飻������ֱ�Ӷ���*/
	if(_ptTrans->RxBuf[1] != MODBUS_COIL_REG_RD && _ptTrans->RxBuf[1] != MODBUS_HOLD_REG_RD  && \
	   _ptTrans->RxBuf[1] != MODBUS_IPUT_REG_RD && _ptTrans->RxBuf[1] != MODBUS_SIMCOIL_REG_WR && \
	   _ptTrans->RxBuf[1] != MODBUS_SIMHOLD_REG_WR && _ptTrans->RxBuf[1] != MODBUS_MULHOLD_REG_WR)
	{
		memset(_ptTrans,0,sizeof(UART_TRANS_T));
		//_ptTrans->RxNewFlag = 1;
		return 0;
	}
	
	/*����֡����У�飻������ֱ�Ӷ����������յ����������ӻ���Ӧ�������*/
	if(_ptTrans->RxBuf[1] == MODBUS_COIL_REG_RD || _ptTrans->RxBuf[1] == MODBUS_HOLD_REG_RD  || 
	   _ptTrans->RxBuf[1] == MODBUS_IPUT_REG_RD || _ptTrans->RxBuf[1] == MODBUS_SIMCOIL_REG_WR || 
	   _ptTrans->RxBuf[1] == MODBUS_SIMHOLD_REG_WR)
	{
		if(_ptTrans->RxCount != 8)
		{
			memset(_ptTrans,0,sizeof(UART_TRANS_T));
			//_ptTrans->RxNewFlag = 1;
			return 0;
		}
	}
	if(_ptTrans->RxBuf[1] == MODBUS_MULHOLD_REG_WR)
	{
		if(_ptTrans->RxCount != (9 + _ptTrans->RxBuf[6]))
		{
			memset(_ptTrans,0,sizeof(UART_TRANS_T));
			//_ptTrans->RxNewFlag = 1;
			return 0;
		}
	}
	
	/*CRCУ�飻������ֱ�Ӷ���*/
	shCalcCRC = ModsCRC16(_ptTrans->RxBuf,  _ptTrans->RxCount - 2);
	shRxCRC   = Mods_ULUH_TO_U16(_ptTrans->RxBuf + _ptTrans->RxCount - 2);
	if(shRxCRC != shCalcCRC)
	{
		memset(_ptTrans,0,sizeof(UART_TRANS_T));
		//_ptTrans->RxNewFlag = 1;
		return 0;
	}

    Slave_Recv_Proc(_ptTrans);
    
	return 1;
}

/**********************************************************************************************************
*	�� �� ��: Slave_Recv_Proc
*	����˵��: �������н���MODBUS,���ڴ���ͨ��Slave_IsRecvDoneУ�������ȷMODBUS����
*	��    ��: _ptTrans ������
*	�� �� ֵ: 1:�����ɹ�  0:����ʧ��
**********************************************************************************************************/
uint8_t Slave_Recv_Proc(UART_TRANS_T* _ptTrans)
{	
	switch(_ptTrans->RxBuf[1])/*���ܴ���*/
	{
		case MODBUS_COIL_REG_RD:  /*0x1 ����Ȧ�Ĵ��� */
		    Slave_Rd_Coil_Proc(_ptTrans);
			break;      	
		
		case MODBUS_HOLD_REG_RD: /* 0x3�����ּĴ��� */
			Slave_Rd_Hold_Proc(_ptTrans); 
			break; 
		
		case MODBUS_IPUT_REG_RD: /*0x4 ������Ĵ��� */
			Slave_Rd_IPut_Proc(_ptTrans);
			break;
		
		case MODBUS_SIMCOIL_REG_WR:/*0x5 д������Ȧ�Ĵ��� */
		    Slave_Wr_SigCoil_Proc(_ptTrans);
			break;

		case MODBUS_MULCOIL_REG_WR: /*0x0F д�����Ȧ�Ĵ��� */
			Slave_Wr_MulCoil_Proc(_ptTrans);
			break;
			
		case MODBUS_SIMHOLD_REG_WR: /*0x6д�������ּĴ���*/
			Slave_Wr_SigHold_Proc(_ptTrans);
			break;
		
		case MODBUS_MULHOLD_REG_WR: /*0x10 д������ּĴ���  */
			Slave_Wr_MulHold_Proc(_ptTrans);
			break;
		
		default :
			Slave_SetAckErr(_ptTrans,RSP_ERR_CMD);
			Slave_AddCRC(_ptTrans);
			break;
    }
	
	return 1;
}



/**********************************************************************************************************
*	�� �� ��: Slave_Rd_Hold_Proc
*	����˵��: 0x3�����ּĴ���
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
*   ����    ��id 0x03 addr-h addr-l  regnum-h regnum-l crc-h crc-l
*   ��Ӧ    ��id 0x03 bytes data1-h data1-l   ...     crc-h crc-l
**********************************************************************************************************/
void Slave_Rd_Hold_Proc(UART_TRANS_T* _ptTrans)
{
	uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);
	uint16_t shRegNum       = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);
	uint16_t cStFoot,cEndFoot,i = 0;
	uint8_t  cTxDatNum;

	/*��ַУ��*/
	if(shRegStartAddr + shRegNum > SLAVE_HOLDREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{
		cTxDatNum  = shRegNum * 2;//һ���Ĵ���ռ2�ֽ�
		
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = cTxDatNum;//�ֽ���
		
		/*֡����*/
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0;i < cEndFoot-cStFoot;i++)
		{
            /*ABCD AB*/
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Hold[i+cStFoot] >> 8; //��8λ
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Hold[i+cStFoot];      //��8λ
		}

		Slave_AddCRC(_ptTrans);	
	}
}


/**********************************************************************************************************
*	�� �� ��: Slave_Rd_IPut_Proc
*	����˵��: 0x4 ������Ĵ���
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
*   ����    ��id 0x04 addr-h addr-l  regnum-h regnum-l crc-h crc-l
*   ��Ӧ    ��id 0x04 bytes data1-h data1-l   ...     crc-h crc-l
*   ˵��    ��һ������ռ2���Ĵ�����һ���Ĵ���ռ2�ֽڣ���ַ��0x00��ʼ��
**********************************************************************************************************/
void Slave_Rd_IPut_Proc(UART_TRANS_T* _ptTrans)
{
	uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);
	uint16_t shRegNum       = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);
	uint16_t cStFoot,cEndFoot,i = 0;
	uint8_t  cTxDatNum;

	/*��ַУ��*/
	if(shRegStartAddr + shRegNum > SLAVE_IPUTREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}                                                       
	else
	{
		cTxDatNum = shRegNum*2;//һ���Ĵ���ռ2�ֽ�
		
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = cTxDatNum;//�ֽ���
		
		/*֡����*/
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0;i < cEndFoot-cStFoot;i++)
		{
			/*CDAB �� AB*/
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Iput[i+cStFoot] >> 8; //��8λ
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Iput[i+cStFoot];      //��8λ
		}

		Slave_AddCRC(_ptTrans);	
	}
}

/**********************************************************************************************************
*	�� �� ��: Slave_Rd_Coil_Proc
*	����˵��: 0x1 ����Ȧ�Ĵ���
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
*   send: id 01 regadd_h regadd_l regnum_h regnum_l crc-l crc-h
*   recv: id 01 bytes data1 data2 ... crc-l crc-h
**********************************************************************************************************/
void Slave_Rd_Coil_Proc(UART_TRANS_T* _ptTrans)
{
    uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2) / 8;//MODBUS�Ĵ�����ʼ��ַ
    uint16_t shCoilNum = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);//��Ȧ����
    uint8_t bytes = (shCoilNum + 7) / 8;
    uint8_t value, i;

	/*��ַУ��*/	
	if(shRegStartAddr > SLAVE_COILREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{			
        _ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = bytes;//�ֽ���
        
		for(i=0; i < bytes; i++)
		{
			value = SlaveReg_Coil[i + shRegStartAddr];
            if (shCoilNum >= 8)
            {
                _ptTrans->TxBuf[_ptTrans->TxCount ++] = value;
                shCoilNum -= 8;
            }
            else
            {
				value = value << (8 - shCoilNum);
				value = value >> (8 - shCoilNum);
                _ptTrans->TxBuf[_ptTrans->TxCount ++] = value;
				shCoilNum = 0;
            }
		}

		Slave_AddCRC(_ptTrans);	     
	}
}

/**********************************************************************************************************
*	�� �� ��: Slave_Wr_SigHold_Proc
*	����˵��: 0x6д�������ּĴ���
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
*   ����    ��id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
*   ��Ӧ    ��id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
**********************************************************************************************************/
void Slave_Wr_SigHold_Proc(UART_TRANS_T* _ptTrans)
{
	uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);
	uint16_t value=0;

	/*��ַУ��*/	
	if(shRegStartAddr > SLAVE_HOLDREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{
		/*ABCD AB*/
		value = _ptTrans->RxBuf[4]<<8 | _ptTrans->RxBuf[5];
        SlaveReg_Hold[shRegStartAddr] = value;//д��Ĵ���
        					
		/*��Ӧ���*/
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//�Ĵ�����ַ��λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//�Ĵ�����ַ��λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Hold[shRegStartAddr] >> 8;//���ݸ�λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Hold[shRegStartAddr];//���ݵ�λ

		Slave_AddCRC(_ptTrans);	     
	}
}


/**********************************************************************************************************
*	�� �� ��: Slave_Wr_MulHold_Proc
*	����˵��: 0x10 д������ּĴ���
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
*   ����    ��id 0x10 addr-h addr-l  regnum-h regnum-l bytes data1-h  data1-l ...  crc-h crc-l
*   ��Ӧ    ��id 0x10 addr-h addr-l  regnum-h regnum-l crc-h crc-l
*   ˵��    ����Ϊ��дһ�����ݣ��Ĵ�����������Ϊ 2;
**********************************************************************************************************/
void Slave_Wr_MulHold_Proc(UART_TRANS_T* _ptTrans)
{
    uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);
	uint16_t shRegNum       = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);
	uint16_t cStFoot,cEndFoot,i = 0;
	uint16_t temp = 0;
	
	/*��ַУ��*/	
	if(shRegStartAddr + shRegNum  > SLAVE_HOLDREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{
		/*д�뱣�ּĴ���*/		
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0; i < cEndFoot - cStFoot; i++)
		{
            /*���յ�ΪABCD����ABCD˳�����*/
			temp =  _ptTrans->RxBuf[i*2 + 7] <<8 | _ptTrans->RxBuf[i*2 + 1 + 7];
            SlaveReg_Hold[cStFoot+i] = temp;
		}
		
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//��ַ��
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//��ַ��
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//�Ĵ���������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//�Ĵ���������
		
		Slave_AddCRC(_ptTrans);	        
	}
}


/**********************************************************************************************************
*	�� �� ��: Master_Wr_SigCoil_Proc
*	����˵��: 05 д������Ȧ�Ĵ���  
*	��    ��: NULL
*	�� �� ֵ: NULL
*   send: id 05 regadd_h regadd_l data_h data_l crc-l crc-h
*   recv: id 05 regadd_h regadd_l data_h data_l crc-l crc-h
*   ˵��    ���͵�ַ��Ӧ��λ
**********************************************************************************************************/
void Slave_Wr_SigCoil_Proc(UART_TRANS_T* _ptTrans)
{
    uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2) / 8;//MODBUS�Ĵ�����ʼ��ַ
	uint16_t shCoilStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);//��Ȧ��ַ,8����Ȧ��1��MODBUS�Ĵ���
    uint16_t value=0;

	/*��ַУ��*/	
	if(shRegStartAddr > SLAVE_COILREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{
		/*FF00 0000*/
		value = _ptTrans->RxBuf[4]<<8 | _ptTrans->RxBuf[5];
        if (value == 0xFF00)
        {
            SlaveReg_Coil[shRegStartAddr] |= (1<<(shCoilStartAddr%8));
        }
        else if (value == 0x0000)
        {
            SlaveReg_Coil[shRegStartAddr] &= ~(1<<(shCoilStartAddr%8));
        }
        					
		/*��Ӧ���*/
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//�Ĵ�����ַ��λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//�Ĵ�����ַ��λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//���ݸ�λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//���ݵ�λ

		Slave_AddCRC(_ptTrans);	     
	}
}


/**********************************************************************************************************
*	�� �� ��: Master_Wr_MulCoil_Proc
*	����˵��: 0x0F д�����Ȧ�Ĵ���  
*	��    ��: NULL
*	�� �� ֵ: NULL
*   ����    ��id 0x0f addr-h addr-l regnum-h regnum-l bytes data1 ... crc-h crc-l
*   ��Ӧ    ��id 0x0f addr-h addr-l regnum-h regnum-l bytes crc-h crc-l
*   ˵��    ���͵�ַ��Ӧ��λ
**********************************************************************************************************/
void Slave_Wr_MulCoil_Proc(UART_TRANS_T* _ptTrans)
{
    uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2) / 8;//MODBUS�Ĵ�����ʼ��ַ
    uint16_t shCoilNum = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);//��Ȧ����
    uint8_t bytes = _ptTrans->RxBuf[6];
    uint8_t value, i;

	/*��ַУ��*/	
	if(shRegStartAddr > SLAVE_COILREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{			
		for(i=0; i < bytes; i++)
		{
			value = _ptTrans->RxBuf[i + 7];
            if (shCoilNum >= 8)
            {
                SlaveReg_Coil[shRegStartAddr+i] = value;
                shCoilNum -= 8;
            }
            else
            {
                SlaveReg_Coil[shRegStartAddr+i] = SlaveReg_Coil[shRegStartAddr+i] >> shCoilNum;
                SlaveReg_Coil[shRegStartAddr+i] = SlaveReg_Coil[shRegStartAddr+i] << shCoilNum;
                SlaveReg_Coil[shRegStartAddr+i] = SlaveReg_Coil[shRegStartAddr+i] + value;
				shCoilNum = 0;
            }
		}
        
		/*��Ӧ���*/
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//�Ĵ�����ַ��λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//�Ĵ�����ַ��λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//������λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//������λ
        _ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//�ֽ���

		Slave_AddCRC(_ptTrans);	     
	}
}


/**********************************************************************************************************
*	�� �� ��: Master_Read_Hold_Reg
*	����˵��: �����ּĴ������� 
*	��     ��: type �Ĵ�������
*	        : addr ��ַ
*	        : nums �Ĵ�������(���ָ�������uint16_t���͵�����,��Ȧ��uint8_t���͵�����)
*           ��data ����
*	�� �� ֵ: NULL
*   ˵     ���������2�ֽڸ�ʽû����,�����4�ֽڸ�ʽ���ﷵ�ص���ABCD,ʵ����Ҫת��CDAB!!!
**********************************************************************************************************/
void Slave_Read_Reg(MODBUS_REG_TYPE type, uint32_t addr,uint32_t nums,uint8_t* data)
{
    uint16_t *ptemp = NULL;
    uint16_t i=0;
    
    ptemp = (uint16_t *)data;
     
    taskENTER_CRITICAL();//�����ٽ�������

    switch (type)
    {
        case EMB_HOLE:
            if(addr < SLAVE_HOLDREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    ptemp[i] = SlaveReg_Hold[addr + i];
                }
            }
            break;

        case EMB_IPUT:
            if(addr < SLAVE_IPUTREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    ptemp[i] = SlaveReg_Iput[addr + i];
                }
            }
            break;

        case EMB_COIL:
            if(addr < SLAVE_COILREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    data[i] = SlaveReg_Coil[addr + i];
                }
            }
            break;
    }
    
    taskEXIT_CRITICAL();//�˳��ٽ�������
}

/**********************************************************************************************************
*	�� �� ��: Master_Write_Hold_Reg
*	����˵��: д���ּĴ������� 
*	��     ��: type �Ĵ�������
*	        : addr ��ַ
*	        : nums �Ĵ�������(���ָ�������uint16_t���͵�����,��Ȧ��uint8_t���͵�����)
*           ��data ����
*	�� �� ֵ: NULL
*   ˵     ���������2�ֽڸ�ʽû����,�����4�ֽڸ�ʽ���ﷵ�ص���ABCD,ʵ����Ҫת��CDAB!!!
**********************************************************************************************************/
void Slave_Write_Reg(MODBUS_REG_TYPE type, uint32_t addr,uint32_t nums,uint8_t* data)
{
    uint16_t *ptemp = NULL;
    uint16_t i=0;
    
    ptemp = (uint16_t *)data;
    
    taskENTER_CRITICAL();//�����ٽ�������

    switch (type)
    {
        case EMB_HOLE:
            if(addr < SLAVE_HOLDREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    SlaveReg_Hold[addr + i] = ptemp[i];
                }
            }
            break;

        case EMB_IPUT:
            if(addr < SLAVE_IPUTREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    SlaveReg_Iput[addr + i] = ptemp[i];
                }
            }
            break;

        case EMB_COIL:
            if(addr < SLAVE_COILREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    SlaveReg_Coil[addr + i] = data[i];
                }
            }
            break;
    }
    
    taskEXIT_CRITICAL();//�˳��ٽ�������
}



