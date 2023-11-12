/*
 * File       : modbus_slave.c
 * Function   : 用于解析处理接收到的请求数据
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-17     denghengli   first version
 */

#include "includes.h"

/*函数声明*/
static void Slave_Rd_Hold_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Rd_IPut_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Rd_Coil_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Wr_SigHold_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Wr_MulHold_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Wr_SigCoil_Proc(UART_TRANS_T* _ptTrans);
static void Slave_Wr_MulCoil_Proc(UART_TRANS_T* _ptTrans);
static void Slave_AddCRC(UART_TRANS_T* _ptTrans);
static void Slave_SetAckErr(UART_TRANS_T* _ptTrans,uint8_t _ucErrCode);

/*数据格式为AB或ABCD*/
static uint16_t SlaveReg_Hold[SLAVE_HOLDREG_NUM];
static uint16_t SlaveReg_Iput[SLAVE_IPUTREG_NUM];
static uint8_t SlaveReg_Coil[SLAVE_COILREG_NUM];


/**********************************************************************************************************
*	函 数 名: Slave_IsRecvDone
*	功能说明: 在中断中接收一帧请求数据包
*     说明：这种方式没有采用定时3.5字符判别帧结束，不能边接收边解析
*           每种功能码接收方式不一样，只能在接收完整包后再解析
*	形    参: _ptTrans 缓存区
*           ：_cDat    数据
*	返 回 值: 1 接收完成，0 未接收完成
**********************************************************************************************************/
uint8_t Slave_IsRecvDone(UART_TRANS_T* _ptTrans,uint8_t _cDat)
{
	uint8_t cRet = 0;
	uint16_t shRxCRC=0,shCalcCRC=0;
	uint16_t shDevId = SlaveId_Get();
    
	if(_ptTrans->RxCount < MODBUS_RECV_ELN_MAX)
	{
		_ptTrans->RxBuf[_ptTrans->RxCount ++] = _cDat;
		
        /* 设备ID验证 */
        if(_ptTrans->RxCount == 1)
        {
            if((_ptTrans->RxBuf[0] != shDevId) && (_ptTrans->RxBuf[0] != MODBUS_ADDR_BROAD))
            {
                memset(_ptTrans,0,sizeof(UART_TRANS_T));
                _ptTrans->RxNewFlag = 1;
                return cRet;
            }
        }
        
        /* 功能码验证 */
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
	
	/*功能码为1 3 4 5 6时，接收到8字节时表示一帧接收完成*/
	if(_ptTrans->RxBuf[1] == MODBUS_COIL_REG_RD || _ptTrans->RxBuf[1] == MODBUS_HOLD_REG_RD  || 
	   _ptTrans->RxBuf[1] == MODBUS_IPUT_REG_RD || _ptTrans->RxBuf[1] == MODBUS_SIMCOIL_REG_WR || 
	   _ptTrans->RxBuf[1] == MODBUS_SIMHOLD_REG_WR)
	{
		if(_ptTrans->RxCount == 8)
		{
			cRet = 1;
		}
	}
	/*功能码为10H时，接收到9字节 + stModsRxTx.RxBuf[6]时表示一帧接收完成*/
	else if(_ptTrans->RxBuf[1] == MODBUS_MULHOLD_REG_WR)
	{
		if(_ptTrans->RxCount == (9 + _ptTrans->RxBuf[6]))
		{
			cRet = 1;
		}
	}
	/*功能码为0FH时，接收到9字节 + stModsRxTx.RxBuf[6]时表示一帧接收完成*/
	else if(_ptTrans->RxBuf[1] == MODBUS_MULCOIL_REG_WR)
	{
		if(_ptTrans->RxCount == (9 + _ptTrans->RxBuf[6]))
		{
			cRet = 1;
		}
	}

    /*CRC校验；不符合直接丢弃*/
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
*	函 数 名: Slave_AddCRC
*	功能说明: 追加2字节CRC至stModsRxTx.TxBuf;
*	形    参: _ptTrans 缓存区
*	返 回 值: NONE
**********************************************************************************************************/
static void Slave_AddCRC(UART_TRANS_T* _ptTrans)
{
	uint16_t shTxCRC;
	
	shTxCRC = ModsCRC16(_ptTrans->TxBuf,_ptTrans->TxCount);
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = shTxCRC; //先写低
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = shTxCRC >> 8;	
}

/**********************************************************************************************************
*	函 数 名: Slave_SetAckErr
*	功能说明: 填充异常帧信息
*	形    参: _ptTrans 缓存区
*             _ucLen  数据长度
*	返 回 值: NONE
**********************************************************************************************************/
static void Slave_SetAckErr (UART_TRANS_T* _ptTrans,uint8_t _ucErrCode)
{
	_ptTrans->TxRspCode = _ucErrCode;
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1] | 0x80;
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->TxRspCode;
}

/**********************************************************************************************************
*	函 数 名: Slave_Pack_Analysis
*	功能说明: 在任务中解析MODBUS,用于解析通过DMA方式接收的整包MODBUS请求
*	形    参: _ptTrans 缓存区
*	返 回 值: 1:解析成功  0:解析失败
**********************************************************************************************************/
uint8_t Slave_Pack_Analysis(UART_TRANS_T* _ptTrans)
{
    uint16_t shRxCRC=0,shCalcCRC=0;
    uint16_t shDevId = SlaveId_Get();
	
	/*地址校验；不符合直接丢弃*/
	if((_ptTrans->RxBuf[0] != shDevId) && (_ptTrans->RxBuf[0] != MODBUS_ADDR_BROAD))
	{
		memset(_ptTrans,0,sizeof(UART_TRANS_T));
		//_ptTrans->RxNewFlag = 1;
		return 0;
	}
	
	/*功能码校验；不符合直接丢弃*/
	if(_ptTrans->RxBuf[1] != MODBUS_COIL_REG_RD && _ptTrans->RxBuf[1] != MODBUS_HOLD_REG_RD  && \
	   _ptTrans->RxBuf[1] != MODBUS_IPUT_REG_RD && _ptTrans->RxBuf[1] != MODBUS_SIMCOIL_REG_WR && \
	   _ptTrans->RxBuf[1] != MODBUS_SIMHOLD_REG_WR && _ptTrans->RxBuf[1] != MODBUS_MULHOLD_REG_WR)
	{
		memset(_ptTrans,0,sizeof(UART_TRANS_T));
		//_ptTrans->RxNewFlag = 1;
		return 0;
	}
	
	/*数据帧长度校验；不符合直接丢弃；避免收到的是其他从机的应答包数据*/
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
	
	/*CRC校验；不符合直接丢弃*/
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
*	函 数 名: Slave_Recv_Proc
*	功能说明: 在任务中解析MODBUS,用于处理通过Slave_IsRecvDone校验过后正确MODBUS请求
*	形    参: _ptTrans 缓存区
*	返 回 值: 1:解析成功  0:解析失败
**********************************************************************************************************/
uint8_t Slave_Recv_Proc(UART_TRANS_T* _ptTrans)
{	
	switch(_ptTrans->RxBuf[1])/*功能处理*/
	{
		case MODBUS_COIL_REG_RD:  /*0x1 读线圈寄存器 */
		    Slave_Rd_Coil_Proc(_ptTrans);
			break;      	
		
		case MODBUS_HOLD_REG_RD: /* 0x3读保持寄存器 */
			Slave_Rd_Hold_Proc(_ptTrans); 
			break; 
		
		case MODBUS_IPUT_REG_RD: /*0x4 读输入寄存器 */
			Slave_Rd_IPut_Proc(_ptTrans);
			break;
		
		case MODBUS_SIMCOIL_REG_WR:/*0x5 写单个线圈寄存器 */
		    Slave_Wr_SigCoil_Proc(_ptTrans);
			break;

		case MODBUS_MULCOIL_REG_WR: /*0x0F 写多个线圈寄存器 */
			Slave_Wr_MulCoil_Proc(_ptTrans);
			break;
			
		case MODBUS_SIMHOLD_REG_WR: /*0x6写单个保持寄存器*/
			Slave_Wr_SigHold_Proc(_ptTrans);
			break;
		
		case MODBUS_MULHOLD_REG_WR: /*0x10 写多个保持寄存器  */
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
*	函 数 名: Slave_Rd_Hold_Proc
*	功能说明: 0x3读保持寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   请求    ：id 0x03 addr-h addr-l  regnum-h regnum-l crc-h crc-l
*   响应    ：id 0x03 bytes data1-h data1-l   ...     crc-h crc-l
**********************************************************************************************************/
void Slave_Rd_Hold_Proc(UART_TRANS_T* _ptTrans)
{
	uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);
	uint16_t shRegNum       = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);
	uint16_t cStFoot,cEndFoot,i = 0;
	uint8_t  cTxDatNum;

	/*地址校验*/
	if(shRegStartAddr + shRegNum > SLAVE_HOLDREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{
		cTxDatNum  = shRegNum * 2;//一个寄存器占2字节
		
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//功能码
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = cTxDatNum;//字节数
		
		/*帧数据*/
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0;i < cEndFoot-cStFoot;i++)
		{
            /*ABCD AB*/
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Hold[i+cStFoot] >> 8; //高8位
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Hold[i+cStFoot];      //低8位
		}

		Slave_AddCRC(_ptTrans);	
	}
}


/**********************************************************************************************************
*	函 数 名: Slave_Rd_IPut_Proc
*	功能说明: 0x4 读输入寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   请求    ：id 0x04 addr-h addr-l  regnum-h regnum-l crc-h crc-l
*   响应    ：id 0x04 bytes data1-h data1-l   ...     crc-h crc-l
*   说明    ：一组数据占2个寄存器，一个寄存器占2字节；地址从0x00开始的
**********************************************************************************************************/
void Slave_Rd_IPut_Proc(UART_TRANS_T* _ptTrans)
{
	uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);
	uint16_t shRegNum       = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);
	uint16_t cStFoot,cEndFoot,i = 0;
	uint8_t  cTxDatNum;

	/*地址校验*/
	if(shRegStartAddr + shRegNum > SLAVE_IPUTREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}                                                       
	else
	{
		cTxDatNum = shRegNum*2;//一个寄存器占2字节
		
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//功能码
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = cTxDatNum;//字节数
		
		/*帧数据*/
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0;i < cEndFoot-cStFoot;i++)
		{
			/*CDAB 或 AB*/
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Iput[i+cStFoot] >> 8; //高8位
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Iput[i+cStFoot];      //低8位
		}

		Slave_AddCRC(_ptTrans);	
	}
}

/**********************************************************************************************************
*	函 数 名: Slave_Rd_Coil_Proc
*	功能说明: 0x1 读线圈寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   send: id 01 regadd_h regadd_l regnum_h regnum_l crc-l crc-h
*   recv: id 01 bytes data1 data2 ... crc-l crc-h
**********************************************************************************************************/
void Slave_Rd_Coil_Proc(UART_TRANS_T* _ptTrans)
{
    uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2) / 8;//MODBUS寄存器起始地址
    uint16_t shCoilNum = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);//线圈数量
    uint8_t bytes = (shCoilNum + 7) / 8;
    uint8_t value, i;

	/*地址校验*/	
	if(shRegStartAddr > SLAVE_COILREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{			
        _ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//功能码
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = bytes;//字节数
        
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
*	函 数 名: Slave_Wr_SigHold_Proc
*	功能说明: 0x6写单个保持寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   请求    ：id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
*   响应    ：id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
**********************************************************************************************************/
void Slave_Wr_SigHold_Proc(UART_TRANS_T* _ptTrans)
{
	uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);
	uint16_t value=0;

	/*地址校验*/	
	if(shRegStartAddr > SLAVE_HOLDREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{
		/*ABCD AB*/
		value = _ptTrans->RxBuf[4]<<8 | _ptTrans->RxBuf[5];
        SlaveReg_Hold[shRegStartAddr] = value;//写入寄存器
        					
		/*响应组包*/
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//功能码
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//寄存器地址高位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//寄存器地址低位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Hold[shRegStartAddr] >> 8;//数据高位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = SlaveReg_Hold[shRegStartAddr];//数据低位

		Slave_AddCRC(_ptTrans);	     
	}
}


/**********************************************************************************************************
*	函 数 名: Slave_Wr_MulHold_Proc
*	功能说明: 0x10 写多个保持寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   请求    ：id 0x10 addr-h addr-l  regnum-h regnum-l bytes data1-h  data1-l ...  crc-h crc-l
*   响应    ：id 0x10 addr-h addr-l  regnum-h regnum-l crc-h crc-l
*   说明    ：因为改写一组数据，寄存器数量至少为 2;
**********************************************************************************************************/
void Slave_Wr_MulHold_Proc(UART_TRANS_T* _ptTrans)
{
    uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);
	uint16_t shRegNum       = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);
	uint16_t cStFoot,cEndFoot,i = 0;
	uint16_t temp = 0;
	
	/*地址校验*/	
	if(shRegStartAddr + shRegNum  > SLAVE_HOLDREG_NUM)
	{
		Slave_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Slave_AddCRC(_ptTrans);
	}
	else
	{
		/*写入保持寄存器*/		
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0; i < cEndFoot - cStFoot; i++)
		{
            /*接收到为ABCD，则按ABCD顺序存入*/
			temp =  _ptTrans->RxBuf[i*2 + 7] <<8 | _ptTrans->RxBuf[i*2 + 1 + 7];
            SlaveReg_Hold[cStFoot+i] = temp;
		}
		
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//功能码
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//地址高
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//地址低
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//寄存器数量高
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//寄存器数量低
		
		Slave_AddCRC(_ptTrans);	        
	}
}


/**********************************************************************************************************
*	函 数 名: Master_Wr_SigCoil_Proc
*	功能说明: 05 写单个线圈寄存器  
*	形    参: NULL
*	返 回 值: NULL
*   send: id 05 regadd_h regadd_l data_h data_l crc-l crc-h
*   recv: id 05 regadd_h regadd_l data_h data_l crc-l crc-h
*   说明    ：低地址对应低位
**********************************************************************************************************/
void Slave_Wr_SigCoil_Proc(UART_TRANS_T* _ptTrans)
{
    uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2) / 8;//MODBUS寄存器起始地址
	uint16_t shCoilStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2);//线圈地址,8个线圈用1个MODBUS寄存器
    uint16_t value=0;

	/*地址校验*/	
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
        					
		/*响应组包*/
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//功能码
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//寄存器地址高位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//寄存器地址低位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//数据高位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//数据低位

		Slave_AddCRC(_ptTrans);	     
	}
}


/**********************************************************************************************************
*	函 数 名: Master_Wr_MulCoil_Proc
*	功能说明: 0x0F 写多个线圈寄存器  
*	形    参: NULL
*	返 回 值: NULL
*   请求    ：id 0x0f addr-h addr-l regnum-h regnum-l bytes data1 ... crc-h crc-l
*   响应    ：id 0x0f addr-h addr-l regnum-h regnum-l bytes crc-h crc-l
*   说明    ：低地址对应低位
**********************************************************************************************************/
void Slave_Wr_MulCoil_Proc(UART_TRANS_T* _ptTrans)
{
    uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 2) / 8;//MODBUS寄存器起始地址
    uint16_t shCoilNum = Mods_UHUL_TO_U16(_ptTrans->RxBuf + 4);//线圈数量
    uint8_t bytes = _ptTrans->RxBuf[6];
    uint8_t value, i;

	/*地址校验*/	
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
        
		/*响应组包*/
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//功能码
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//寄存器地址高位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//寄存器地址低位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//数量高位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//数量低位
        _ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//字节数

		Slave_AddCRC(_ptTrans);	     
	}
}


/**********************************************************************************************************
*	函 数 名: Master_Read_Hold_Reg
*	功能说明: 读保持寄存器数据 
*	形     参: type 寄存器类型
*	        : addr 地址
*	        : nums 寄存器数量(保持跟输入是uint16_t类型的数量,线圈是uint8_t类型的数量)
*           ：data 数据
*	返 回 值: NULL
*   说     明：如果是2字节格式没问题,如果是4字节格式这里返回的是ABCD,实际需要转成CDAB!!!
**********************************************************************************************************/
void Slave_Read_Reg(MODBUS_REG_TYPE type, uint32_t addr,uint32_t nums,uint8_t* data)
{
    uint16_t *ptemp = NULL;
    uint16_t i=0;
    
    ptemp = (uint16_t *)data;
     
    taskENTER_CRITICAL();//进入临界区保护

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
    
    taskEXIT_CRITICAL();//退出临界区保护
}

/**********************************************************************************************************
*	函 数 名: Master_Write_Hold_Reg
*	功能说明: 写保持寄存器数据 
*	形     参: type 寄存器类型
*	        : addr 地址
*	        : nums 寄存器数量(保持跟输入是uint16_t类型的数量,线圈是uint8_t类型的数量)
*           ：data 数据
*	返 回 值: NULL
*   说     明：如果是2字节格式没问题,如果是4字节格式这里返回的是ABCD,实际需要转成CDAB!!!
**********************************************************************************************************/
void Slave_Write_Reg(MODBUS_REG_TYPE type, uint32_t addr,uint32_t nums,uint8_t* data)
{
    uint16_t *ptemp = NULL;
    uint16_t i=0;
    
    ptemp = (uint16_t *)data;
    
    taskENTER_CRITICAL();//进入临界区保护

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
    
    taskEXIT_CRITICAL();//退出临界区保护
}



