/*
 * File       : modbus_master.c
 * Function   : 用于解析处理接收到的应答数据
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-17     denghengli   first version
 */

#include "includes.h"

/*函数声明*/
static void Master_Rd_Hold_Proc(UART_TRANS_T* _ptTrans);
static void Master_Rd_IPut_Proc(UART_TRANS_T* _ptTrans);
static void Master_Rd_Coil_Proc(UART_TRANS_T* _ptTrans);
static void Master_Wr_SigHold_Proc(UART_TRANS_T* _ptTrans);
static void Master_Wr_MulHold_Proc(UART_TRANS_T* _ptTrans);
static void Master_Wr_SigCoil_Proc(UART_TRANS_T* _ptTrans);
static void Master_Wr_MulCoil_Proc(UART_TRANS_T* _ptTrans);

/*数据格式为AB或CDAB*/
static uint16_t MasterReg_Hold[1][MASTER_HOLDREG_NUM];
static uint8_t  MasterReg_Coil[1][MASTER_COILREG_NUM];

/**********************************************************************************************************
*	函 数 名: Master_IsRecvDone
*	功能说明: 在中断中接收一帧请求数据包
*     说明：这种方式没有采用定时3.5字符判别帧结束，不能边接收边解析
*           每种功能码接收方式不一样，只能在接收完整包后再解析
*	形    参: _ptTrans 缓存区
*           ：_cDat    数据
*	返 回 值: 1 接收完成，0 未接收完成
*	id 06 regadd_h regadd_l data_h data_l crc-l crc-h 写单个保持  8
*	id 10 addr-h addr-l  regnum-h regnum-l crc-l crc-h 写多个保持 8
*	id 05 regadd_h regadd_l data_h data_l crc-l crc-h 写单个线圈 8
*	id 0F regadd_h regadd_l regnum_h regnum_l bytes crc-l crc-h 写多个线圈 9
*	id 01 bytes data1 data2 ... crc-l crc-h 读线圈 5+[2]
*	id 03 bytes data1_h data1_l ... crc-l crc-h 读多个保持  5+[2]
**********************************************************************************************************/
uint8_t Master_IsRecvDone(UART_TRANS_T* _ptTrans,uint8_t _cDat)
{
	uint8_t cRet = 0;
	uint16_t shRxCRC=0,shCalcCRC=0;
	uint16_t shDevId = MasterId_Get();
    
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
	
	/*功能码为5 6 10时，接收到8字节时表示一帧接收完成*/
	if(_ptTrans->RxBuf[1] == MODBUS_SIMCOIL_REG_WR || _ptTrans->RxBuf[1] == MODBUS_SIMHOLD_REG_WR  || 
	   _ptTrans->RxBuf[1] == MODBUS_MULHOLD_REG_WR)
	{
		if(_ptTrans->RxCount == 8)
		{
			cRet = 1;
		}
	}
	/*功能码为1 3时，接收到5字节 + stModsRxTx.RxBuf[2]时表示一帧接收完成*/
	else if(_ptTrans->RxBuf[1] == MODBUS_COIL_REG_RD ||  _ptTrans->RxBuf[1] == MODBUS_HOLD_REG_RD)
	{
        if ((_ptTrans->TxBuf[4]<<8 | _ptTrans->TxBuf[5]) > 128)
        {
            if(_ptTrans->RxCount == (5 + _ptTrans->RxBuf[2] + 256))
            {
                cRet = 1;
            }
        }
        else
        {
            if(_ptTrans->RxCount == (5 + _ptTrans->RxBuf[2]))
            {
                cRet = 1;
            }
        }
	}
	/*功能码为0F时，接收到9字节时表示一帧接收完成*/
	else if(_ptTrans->RxBuf[1] == MODBUS_MULCOIL_REG_WR)
	{
		if(_ptTrans->RxCount == 9)
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
*	函 数 名: Master_Pack_Analysis
*	功能说明: 在任务中解析MODBUS,用于解析通过DMA方式接收的整包MODBUS应答
*	形    参: _ptTrans 缓存区
*	返 回 值: 1:解析成功  0:解析失败
**********************************************************************************************************/
uint8_t Master_Pack_Analysis(UART_TRANS_T* _ptTrans)
{
    uint16_t shRxCRC=0,shCalcCRC=0;
    uint16_t shDevId = MasterId_Get();
	
	/*地址校验；不符合直接丢弃*/
	if((_ptTrans->RxBuf[0] != shDevId) && (_ptTrans->RxBuf[0] != MODBUS_ADDR_BROAD))
	{
		memset(_ptTrans,0,sizeof(UART_TRANS_T));
		_ptTrans->RxNewFlag = 1;
		return 0;
	}
	
	/*功能码校验；不符合直接丢弃*/
	if(_ptTrans->RxBuf[1] != MODBUS_COIL_REG_RD && _ptTrans->RxBuf[1] != MODBUS_HOLD_REG_RD  && \
	   _ptTrans->RxBuf[1] != MODBUS_IPUT_REG_RD && _ptTrans->RxBuf[1] != MODBUS_SIMCOIL_REG_WR && \
	   _ptTrans->RxBuf[1] != MODBUS_SIMHOLD_REG_WR && _ptTrans->RxBuf[1] != MODBUS_MULHOLD_REG_WR)
	{
		memset(_ptTrans,0,sizeof(UART_TRANS_T));
		_ptTrans->RxNewFlag = 1;
		return 0;
	}
	
	/*数据帧长度校验；不符合直接丢弃*/
	if(_ptTrans->RxBuf[1] == MODBUS_SIMCOIL_REG_WR || _ptTrans->RxBuf[1] == MODBUS_SIMHOLD_REG_WR  || 
	   _ptTrans->RxBuf[1] == MODBUS_MULHOLD_REG_WR)
	{
		if(_ptTrans->RxCount != 8)
		{
			memset(_ptTrans,0,sizeof(UART_TRANS_T));
			_ptTrans->RxNewFlag = 1;
			return 0;
		}
	}
	if(_ptTrans->RxBuf[1] == MODBUS_COIL_REG_RD ||  _ptTrans->RxBuf[1] == MODBUS_HOLD_REG_RD)
	{
		if(_ptTrans->RxCount != (5 + _ptTrans->RxBuf[2]))
		{
			memset(_ptTrans,0,sizeof(UART_TRANS_T));
			_ptTrans->RxNewFlag = 1;
			return 0;
		}
	}
	if(_ptTrans->RxBuf[1] == MODBUS_MULCOIL_REG_WR)
	{
		if(_ptTrans->RxCount != 9)
		{
			memset(_ptTrans,0,sizeof(UART_TRANS_T));
			_ptTrans->RxNewFlag = 1;
			return 0;
		}
	}
	
	/*CRC校验；不符合直接丢弃*/
	shCalcCRC = ModsCRC16(_ptTrans->RxBuf,  _ptTrans->RxCount - 2);
	shRxCRC   = Mods_ULUH_TO_U16(_ptTrans->RxBuf + _ptTrans->RxCount - 2);
	if(shRxCRC != shCalcCRC)
	{
		memset(_ptTrans,0,sizeof(UART_TRANS_T));
		_ptTrans->RxNewFlag = 1;
		return 0;
	}

    Master_Recv_Proc(_ptTrans);
    
	return 1;
}


/**********************************************************************************************************
*	函 数 名: Master_Recv_Analysis
*	功能说明: 在任务中解析MODBUS,可以用于直接解析整包MODBUS请求
*	形    参: _ptTrans 缓存区 _cNum 模块序号
*	返 回 值: 1:解析成功  0:解析失败
**********************************************************************************************************/
uint8_t Master_Recv_Proc(UART_TRANS_T* _ptTrans)
{	
	switch(_ptTrans->RxBuf[1])/*功能处理*/
	{
		case MODBUS_COIL_REG_RD:  /*0x1 读线圈寄存器 */
		    Master_Rd_Coil_Proc(_ptTrans);
			break;      	
		
		case MODBUS_HOLD_REG_RD: /* 0x3读保持寄存器 */
			Master_Rd_Hold_Proc(_ptTrans); 
			break; 
		
		case MODBUS_IPUT_REG_RD: /*0x4 读输入寄存器 */
			Master_Rd_IPut_Proc(_ptTrans);
			break;
		
		case MODBUS_SIMCOIL_REG_WR:/*0x5 写单个线圈寄存器 */
		    Master_Wr_SigCoil_Proc(_ptTrans);
			break;

		case MODBUS_MULCOIL_REG_WR: /*0x0F 写多个线圈寄存器 */
			Master_Wr_MulCoil_Proc(_ptTrans);
			break;
			
		case MODBUS_SIMHOLD_REG_WR: /*0x6写单个保持寄存器*/
			Master_Wr_SigHold_Proc(_ptTrans);
			break;
		
		case MODBUS_MULHOLD_REG_WR: /*0x10 写多个保持寄存器  */
			Master_Wr_MulHold_Proc(_ptTrans);
			break;
		
		default :
			break;
    }
	
	return 1;
}



/**********************************************************************************************************
*	函 数 名: Master_Rd_Hold_Proc
*	功能说明: 0x3读保持寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   请求    ：id 0x03 addr-h addr-l  regnum-h regnum-l crc-h crc-l
*   响应    ：id 0x03 bytes data1-h data1-l   ...     crc-h crc-l
**********************************************************************************************************/
void Master_Rd_Hold_Proc(UART_TRANS_T* _ptTrans)
{
	uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->TxBuf + 2);
	uint16_t shRegNum       = Mods_UHUL_TO_U16(_ptTrans->TxBuf + 4);
	uint16_t cStFoot,cEndFoot,i = 0;

	/*地址校验*/
	if(shRegStartAddr + shRegNum <= MASTER_HOLDREG_NUM)
	{
		/*帧数据*/
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0;i < cEndFoot-cStFoot;i++)
		{
            /*ABCD AB*/
            MasterReg_Hold[_ptTrans->Num][cStFoot+i] = (_ptTrans->RxBuf[3+2*i] << 8) | _ptTrans->RxBuf[3+2*i+1];
		}
	}
}


/**********************************************************************************************************
*	函 数 名: Master_Rd_IPut_Proc
*	功能说明: 0x4 读输入寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   请求    ：id 0x04 addr-h addr-l  regnum-h regnum-l crc-h crc-l
*   响应    ：id 0x04 bytes data1-h data1-l   ...     crc-h crc-l
**********************************************************************************************************/
void Master_Rd_IPut_Proc(UART_TRANS_T* _ptTrans)
{
	uint16_t shRegStartAddr = Mods_UHUL_TO_U16(_ptTrans->TxBuf + 2);
	uint16_t shRegNum       = Mods_UHUL_TO_U16(_ptTrans->TxBuf + 4);
	uint16_t cStFoot,cEndFoot,i = 0;

	/*地址校验*/
	if(shRegStartAddr + shRegNum <= MASTER_IPUTREG_NUM)
	{
		/*帧数据*/
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0;i < cEndFoot-cStFoot;i++)
		{
            /*ABCD AB*/
            MasterReg_Hold[_ptTrans->Num][cStFoot+i] = (_ptTrans->RxBuf[3+2*i] << 8) | _ptTrans->RxBuf[3+2*i+1];
		}
	}
}

/**********************************************************************************************************
*	函 数 名: Master_Rd_Coil_Proc
*	功能说明: 0x1 读线圈寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   send: id 01 regadd_h regadd_l regnum_h regnum_l crc-l crc-h
*   recv: id 01 bytes data1 data2 ... crc-l crc-h
**********************************************************************************************************/
void Master_Rd_Coil_Proc(UART_TRANS_T* _ptTrans)
{
	uint16_t shCoilStartAddr = Mods_UHUL_TO_U16(_ptTrans->TxBuf + 2);//线圈起始地址,8个线圈用一个MODBUS
    uint16_t shRegStartAddr = shCoilStartAddr / 8;//MODBUS寄存器起始地址
	uint16_t shCoilNum = Mods_UHUL_TO_U16(_ptTrans->TxBuf + 4);//线圈数量
	uint8_t  cDatas = _ptTrans->RxBuf[2], value;
	uint16_t i = 0;

	/*地址校验*/
	if(shRegStartAddr + cDatas <= MASTER_COILREG_NUM)
	{
		/*帧数据*/
		for(i=0; i<cDatas; i++)
		{
			value = _ptTrans->RxBuf[3+i];
			if (shCoilNum >= 8)
			{
				MasterReg_Coil[_ptTrans->Num][shRegStartAddr+i] = value;
				shCoilNum -= 8;
			}
			else
			{
				MasterReg_Coil[_ptTrans->Num][shRegStartAddr+i] += value;
				shCoilNum = 0;
			}
		}
	}
}


/**********************************************************************************************************
*	函 数 名: Master_Wr_SigHold_Proc
*	功能说明: 0x6写单个保持寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   请求    ：id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
*   响应    ：id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
**********************************************************************************************************/
void Master_Wr_SigHold_Proc(UART_TRANS_T* _ptTrans)
{
    ;
}


/**********************************************************************************************************
*	函 数 名: Master_Wr_MulHold_Proc
*	功能说明: 0x10 写多个保持寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
*   请求    ：id 0x10 addr-h addr-l  regnum-h regnum-l bytes data1-h  data1-l ...  crc-h crc-l
*   响应    ：id 0x10 addr-h addr-l  regnum-h regnum-l crc-h crc-l
**********************************************************************************************************/
void Master_Wr_MulHold_Proc(UART_TRANS_T* _ptTrans)
{
    ;
}


/**********************************************************************************************************
*	函 数 名: Master_Wr_SigCoil_Proc
*	功能说明: 05 写单个线圈寄存器  
*	形    参: NULL
*	返 回 值: NULL
*   send: id 05 regadd_h regadd_l data_h data_l crc-l crc-h
*   recv: id 05 regadd_h regadd_l data_h data_l crc-l crc-h
**********************************************************************************************************/
void Master_Wr_SigCoil_Proc(UART_TRANS_T* _ptTrans)
{
    ;
}


/**********************************************************************************************************
*	函 数 名: Master_Wr_MulCoil_Proc
*	功能说明: 0x0F 写多个线圈寄存器  
*	形    参: NULL
*	返 回 值: NULL
*   请求    ：id 0x0f addr-h addr-l regnum-h regnum-l bytes data1 ... crc-h crc-l
*   响应    ：id 0x0f addr-h addr-l regnum-h regnum-l bytes crc-h crc-l
**********************************************************************************************************/
void Master_Wr_MulCoil_Proc(UART_TRANS_T* _ptTrans)
{
    ;
}

/**********************************************************************************************************
*	函 数 名: Master_Read_Hold_Reg
*	功能说明: 读保持寄存器数据 
*	形    参: type 寄存器类型
*	        : mod 模块号
*	        : addr 地址
*	        : nums 寄存器数量(保持跟输入是uint16_t类型的数量,线圈是uint8_t类型的数量)
*           : data 数据
*	返 回 值: NULL
*   说     明：如果是2字节格式没问题,如果是4字节格式这里返回的是ABCD,实际需要转成CDAB!!!
**********************************************************************************************************/
void Master_Read_Reg (MODBUS_REG_TYPE type, uint8_t mod, uint32_t addr,uint32_t nums,uint8_t* data)
{
    uint16_t *ptemp = NULL;
    uint16_t i=0;
    
    ptemp = (uint16_t *)data;
     
    taskENTER_CRITICAL();//进入临界区保护

    switch (type)
    {
        case EMB_HOLE:
            if(addr < MASTER_HOLDREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    ptemp[i] = MasterReg_Hold[mod][addr + i];
                }
            }
            break;

        case EMB_IPUT:
            if(addr < MASTER_IPUTREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    ptemp[i] = MasterReg_Hold[mod][addr + i];
                }
            }
            break;

        case EMB_COIL:
            if(addr < MASTER_COILREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    data[i] = MasterReg_Coil[mod][addr + i];
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
void Master_Write_Reg(MODBUS_REG_TYPE type, uint8_t mod, uint32_t addr,uint32_t nums,uint8_t* data)
{
    uint16_t *ptemp = NULL;
    uint16_t i=0;
    
    ptemp = (uint16_t *)data;
    
    taskENTER_CRITICAL();//进入临界区保护

    switch (type)
    {
        case EMB_HOLE:
            if(addr < MASTER_HOLDREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    MasterReg_Hold[mod][addr + i] = ptemp[i];
                }
            }
            break;

        case EMB_IPUT:
            if(addr < MASTER_IPUTREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    MasterReg_Hold[mod][addr + i] = ptemp[i];
                }
            }
            break;

        case EMB_COIL:
            if(addr < MASTER_COILREG_NUM)
            {
                for(i=0;i<nums;i++)
                {
                    MasterReg_Coil[mod][addr + i] = data[i];
                }
            }
            break;
    }
    
    taskEXIT_CRITICAL();//退出临界区保护
}



