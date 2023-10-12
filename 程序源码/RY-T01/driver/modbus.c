#include "includes.h"

/*函数声明*/
static void Mods_Rd_Hold_Proc    (UART_TRANS_T* _ptTrans);
static void Mods_Rd_In_Proc      (UART_TRANS_T* _ptTrans);
static void Mods_Wr_SimpHold_Proc(UART_TRANS_T* _ptTrans);
static void Mods_Wr_MulHold_Proc (UART_TRANS_T* _ptTrans);
static void Mods_Wr_MulCoil_Proc (UART_TRANS_T* _ptTrans);
static void Mods_AddCRC      	  (UART_TRANS_T* _ptTrans);
static void Mods_SetAckErr       (UART_TRANS_T* _ptTrans,unsigned char _ucErrCode);

static unsigned short ModsReg_Hold [MODBUS_HOLDREG_LEN];
static unsigned short ModsReg_Input[MODBUS_INPUTREG_LEN];


///*
//*********************************************************************************************************
//*	函 数 名: UHUL_TO_U16
//*	功能说明: 字符转换
//*	形    参: _cPtr   数据指针
//*	返 回 值: short数据
//*********************************************************************************************************
//*/
//unsigned short UHUL_TO_U16(unsigned char *_cptr)
//{
//    unsigned short shTemp = 0;
//    
//    shTemp = _cptr[0];
//    shTemp = shTemp << 8 | _cptr[1];
//    
//    return shTemp;
//}
//
///*
//*********************************************************************************************************
//*	函 数 名: UHUL_TO_U16
//*	功能说明: 字符转换
//*	形    参: _cPtr   数据指针
//*	返 回 值: short数据
//*********************************************************************************************************
//*/
//unsigned short ULUH_TO_U16(unsigned char *_cptr)
//{
//    unsigned short shTemp = 0;
//    
//    shTemp = _cptr[1];
//    shTemp = shTemp << 8 | _cptr[0];
//    
//    return shTemp;
//}

/**********************************************************************************************************
*	函 数 名: ModsCRC16
*	功能说明: MODBUS CRC计算
*	形    参: _cPtr   数据指针
*           _shLen  数据长度
*	返 回 值: CRC
**********************************************************************************************************/
static unsigned short ModsCRC16(const unsigned char *_cPtr,unsigned short _shLen)
{
	unsigned short i,j;
	unsigned short shCRC = 0xFFFF;
	
	for(i=0;i<_shLen;i++)
	{
		shCRC ^= _cPtr[i];
        for(j=0;j<8;j++)
		{
			if(shCRC & 0x01)
			{ 
				shCRC = (shCRC>>1) ^ 0xA001;
			}
			else
			{
				shCRC = shCRC>>1;
			}	
		}
	}
	return shCRC;
}



/**********************************************************************************************************
*	函 数 名: IsModsRecvDone
*	功能说明: 在中断中接收一帧请求数据包
*     说明：这种方式没有采用定时3.5字符判别帧结束，不能边接收边解析
*           每种功能码接收方式不一样，只能在接收完整包后再解析
*	形    参: _ptTrans 缓存区
*           ：_cDat    数据
*	返 回 值: 1 接收完成，0 未接收完成
**********************************************************************************************************/
unsigned char IsModsRecvDone(UART_TRANS_T* _ptTrans,unsigned char _cDat)
{
	unsigned char cRet = 0;
	unsigned short shDevId = DevId_Get();
    
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
            if(_ptTrans->RxBuf[1] != MODBUS_COIL_REG_RD && _ptTrans->RxBuf[1] != MODBUS_HOLDING_REG_RD  && \
               _ptTrans->RxBuf[1] != MODBUS_IN_REG_RD   && _ptTrans->RxBuf[1] != MODBUS_SIMPCOIL_REG_WR && \
               _ptTrans->RxBuf[1] != MODBUS_SIMPHOLDING_REG_WR && _ptTrans->RxBuf[1] != MODBUS_MULHOLDING_REG_WR)
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
	if(_ptTrans->RxBuf[1] == MODBUS_COIL_REG_RD || _ptTrans->RxBuf[1] == MODBUS_HOLDING_REG_RD  || 
	   _ptTrans->RxBuf[1] == MODBUS_IN_REG_RD   || _ptTrans->RxBuf[1] == MODBUS_SIMPCOIL_REG_WR || 
	   _ptTrans->RxBuf[1] == MODBUS_SIMPHOLDING_REG_WR)
	{
		if(_ptTrans->RxCount == 8)
		{
			cRet = 1;
		}
	}
	/*功能码为10H时，接收到9字节 + stModsRxTx.RxBuf[6]时表示一帧接收完成*/
	else if(_ptTrans->RxBuf[1] == MODBUS_MULHOLDING_REG_WR)
	{
		if(_ptTrans->RxCount == (9 + _ptTrans->RxBuf[6]))
		{
			cRet = 1;
		}
	}
//	/*功能码为0FH时，接收到9字节 + stModsRxTx.RxBuf[6]时表示一帧接收完成*/
//	else if(_ptTrans->RxBuf[1] == MODBUS_MULCOIL_REG_WR)
//	{
//		if(_ptTrans->RxCount == (9 + _ptTrans->RxBuf[6]))
//		{
//			cRet = 1;
//		}
//	}
	
	return cRet;
}




/**********************************************************************************************************
*	函 数 名: Mods_AddCRC
*	功能说明: 追加2字节CRC至stModsRxTx.TxBuf;
*	形    参: _ptTrans 缓存区
*	返 回 值: NONE
**********************************************************************************************************/
static void Mods_AddCRC(UART_TRANS_T* _ptTrans)
{
	unsigned short shTxCRC;
	
	shTxCRC = ModsCRC16(_ptTrans->TxBuf,_ptTrans->TxCount);
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = shTxCRC; //先写低
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = shTxCRC >> 8;
	
}

/**********************************************************************************************************
*	函 数 名: Mods_SetAckErr
*	功能说明: 填充异常帧信息
*	形    参: _ptTrans 缓存区
*             _ucLen  数据长度
*	返 回 值: NONE
**********************************************************************************************************/
static void Mods_SetAckErr (UART_TRANS_T* _ptTrans,unsigned char _ucErrCode)
{
	_ptTrans->TxRspCode = _ucErrCode;
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1] | 0x80;
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->TxRspCode;

}

/**********************************************************************************************************
*	函 数 名: Mods_Recv_Analysis
*	功能说明: 在任务中解析MODBUS
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
**********************************************************************************************************/
void Mods_Recv_Analysis(UART_TRANS_T* _ptTrans)
{	
	unsigned short shRxCRC=0,shCalcCRC=0;
    
	shCalcCRC = ModsCRC16(_ptTrans->RxBuf,  _ptTrans->RxCount - 2);
	shRxCRC   = ULUH_TO_U16(_ptTrans->RxBuf + _ptTrans->RxCount - 2);
        
	/*地址，帧长度，CRC校验；不符合直接丢弃*/
	if(shRxCRC != shCalcCRC)
	{
		return; 
	}
	
	switch(_ptTrans->RxBuf[1])/*功能处理*/
	{
		case MODBUS_COIL_REG_RD:  /*0x1 读线圈寄存器 */
			break;      	
		
		case MODBUS_HOLDING_REG_RD: /* 0x3读保持寄存器 */
			Mods_Rd_Hold_Proc(_ptTrans); 
			break; 
		
		case MODBUS_IN_REG_RD:    	 /*0x4 读输入寄存器 */
			Mods_Rd_In_Proc(_ptTrans);
			break;
		
		case MODBUS_SIMPCOIL_REG_WR:/*0x5 写单个线圈寄存器 */
			break;
		
		case MODBUS_SIMPHOLDING_REG_WR: 	/*0x6写单个保持寄存器*/
			Mods_Wr_SimpHold_Proc(_ptTrans);
			break;
		
		case MODBUS_MULHOLDING_REG_WR:      /*0x10 写多个保持寄存器  */
			Mods_Wr_MulHold_Proc(_ptTrans);
			break;
		
		case MODBUS_MULCOIL_REG_WR: 	    /*0x0F 写多个线圈寄存器 */
			Mods_Wr_MulCoil_Proc(_ptTrans);
			break;
		
		default :
			Mods_SetAckErr(_ptTrans,RSP_ERR_CMD);
			Mods_AddCRC(_ptTrans);
			break;
    }
	
}



/**********************************************************************************************************
*	函 数 名: Mods_Rd_Hold_Proc
*	功能说明: 0x3读保持寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
* 请求    ：id 0x03 addr-h addr-l  regnum-h regnum-l crc-h crc-l
* 响应    ：id 0x03 bytes data1-h data1-l   ...     crc-h crc-l
**********************************************************************************************************/
void Mods_Rd_Hold_Proc(UART_TRANS_T* _ptTrans)
{
	unsigned short shRegStartAddr = UHUL_TO_U16(_ptTrans->RxBuf + 2);
	unsigned short shRegNum       = UHUL_TO_U16(_ptTrans->RxBuf + 4);
	unsigned short cStFoot,cEndFoot,i = 0;
	unsigned char  cTxDatNum;

	/*地址校验*/
	if(shRegStartAddr + shRegNum > MODBUS_HOLDREG_LEN)
	{
		Mods_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Mods_AddCRC(_ptTrans);
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
            /*CDAB AB*/
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = ModsReg_Hold[i+cStFoot] >> 8; //高8位
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = ModsReg_Hold[i+cStFoot];      //低8位

		}

		Mods_AddCRC(_ptTrans);	
	}
}


/**********************************************************************************************************
*	函 数 名: Mods_Rd_In_Proc
*	功能说明: 0x4 读输入寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
* 请求    ：id 0x04 addr-h addr-l  regnum-h regnum-l crc-h crc-l
* 响应    ：id 0x04 bytes data1-h data1-l   ...     crc-h crc-l
* 说明    ：一组数据占2个寄存器，一个寄存器占2字节；地址从0x00开始的
**********************************************************************************************************/
void Mods_Rd_In_Proc(UART_TRANS_T* _ptTrans)
{
	unsigned short shRegStartAddr = UHUL_TO_U16(_ptTrans->RxBuf + 2);
	unsigned short shRegNum       = UHUL_TO_U16(_ptTrans->RxBuf + 4);
	unsigned short cStFoot,cEndFoot,i = 0;
	unsigned char  cTxDatNum;

	/*地址校验*/
	if(shRegStartAddr + shRegNum > MODBUS_INPUTREG_LEN)
	{
		Mods_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Mods_AddCRC(_ptTrans);
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
			/*CDAB AB*/
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = ModsReg_Input[i+cStFoot] >> 8; //高8位
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = ModsReg_Input[i+cStFoot];      //低8位
		}

		Mods_AddCRC(_ptTrans);	
	}
}


/**********************************************************************************************************
*	函 数 名: Mods_Wr_SimpHold_Proc
*	功能说明: 0x6写单个保持寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
* 请求    ：id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
* 响应    ：id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
**********************************************************************************************************/
void Mods_Wr_SimpHold_Proc(UART_TRANS_T* _ptTrans)
{
	unsigned short shRegStartAddr = UHUL_TO_U16(_ptTrans->RxBuf + 2);
	unsigned short value=0;

	/*地址校验*/	
	if(shRegStartAddr > MODBUS_HOLDREG_LEN)
	{
		Mods_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Mods_AddCRC(_ptTrans);
	}
	else
	{
        if(shRegStartAddr == 30)//设备ID
        {
            /*CDAB AB*/
            value = _ptTrans->RxBuf[4]<<8 | _ptTrans->RxBuf[5];
            ModsReg_Hold[shRegStartAddr] = value;

            /* 参数存储 */
            ParaData_Save(1);
        }
        else if(shRegStartAddr == 32)//风速计自动校准标志
        {
            /*CDAB AB*/
            value = _ptTrans->RxBuf[4]<<8 | _ptTrans->RxBuf[5];
            ModsReg_Hold[shRegStartAddr] = value;     
            
            ParaData_Updata();
        }
        else
        {
            /* 添加新的字段 */
        }
        
		/*响应组包*/
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//功能码
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//寄存器地址高位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//寄存器地址低位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//数据高位
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//数据低位

		Mods_AddCRC(_ptTrans);	     
                
	}
}


/**********************************************************************************************************
*	函 数 名: Mods_Wr_MulHold_Proc
*	功能说明: 0x10 写多个保持寄存器
*	形    参: _ptTrans 缓存区
*	返 回 值: NULL
* 请求    ：0x11 0x10 addr-h addr-l  regnum-h regnum-l bytes data1-h  data1-l ...  crc-h crc-l
* 响应    ：0x11 0x10 addr-h addr-l  regnum-h regnum-l crc-h crc-l
* 说明    ：因为改写一组数据，寄存器数量至少为 2;
*数据格式为  float DCBA  ；u16 BA
**********************************************************************************************************/
void Mods_Wr_MulHold_Proc(UART_TRANS_T* _ptTrans)
{
    unsigned short shRegStartAddr = UHUL_TO_U16(_ptTrans->RxBuf + 2);
	unsigned short shRegNum       = UHUL_TO_U16(_ptTrans->RxBuf + 4);
	unsigned short cStFoot,cEndFoot,i = 0;
	unsigned short temp = 0;
	
	/*地址校验*/	
	if(shRegStartAddr + shRegNum  > MODBUS_HOLDREG_LEN)
	{
		Mods_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Mods_AddCRC(_ptTrans);
	}
	else
	{		
		/*写入保持寄存器*/		
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0; i < cEndFoot - cStFoot; i++)
		{
		  	/*接收到为CDAB，则按CDAB顺序存入*/
			temp =  _ptTrans->RxBuf[i*2 + 7] <<8 | _ptTrans->RxBuf[i*2 + 1 + 7];
			
			if ( (i+cStFoot) == 48 )//PT100温度参考值
			{
				PT100_RefCal_Flag(1);
			}
			else if ( (i+cStFoot) == 52 )//全压静压计数值,越大响应时间越长,平滑度越好，越小则相反，最大50，默认20
			{
			  	if (temp < 1) temp = 1;
				else if (temp > 50) temp = 50;
			}
            ModsReg_Hold[cStFoot+i] = temp;
		}
		
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//地址
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//功能码
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//地址高
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//地址低
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//寄存器数量高
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//寄存器数量低
		
		Mods_AddCRC(_ptTrans);	
			
        /*系数和偏移量类参数设置后会立即生效*/
        ParaData_Save(1);
	}
}


/**********************************************************************************************************
*	函 数 名: Mods_Wr_MulCoil_Proc
*	功能说明: 0x0F 写多个线圈寄存器  
*	形    参: NULL
*	返 回 值: NULL
* 请求    ：id 0x0f addr-h addr-l regnum-h regnum-l bytes data1 ... crc-h crc-l
* 响应    ：id 0x0f addr-h addr-l regnum-h regnum-l bytes crc-h crc-l
* 说明    ：低地址对应低位
**********************************************************************************************************/
void Mods_Wr_MulCoil_Proc(UART_TRANS_T* _ptTrans)
{
    ;
}

/**********************************************************************************************************
*	函 数 名: Read_Hold_Reg
*	功能说明: 读保持寄存器数据 
*	形    参: addr 地址
*	        : regs 寄存器数量
*           ：data 数据
*	返 回 值: NULL
**********************************************************************************************************/
void Read_Hold_Reg (unsigned int addr,unsigned int regs,unsigned char* data)
{
    unsigned short *ptemp = NULL;
    unsigned short i=0;
    
    ptemp = (unsigned short *)data;
     
    taskENTER_CRITICAL();//进入临界区保护
    
    if(addr < MODBUS_HOLDREG_LEN)
    {
        for(i=0;i<regs;i++)
        {
            ptemp[i] = ModsReg_Hold[addr + i];
        }
    }
    
    taskEXIT_CRITICAL();//退出临界区保护
}

/**********************************************************************************************************
*	函 数 名: Write_Hold_Reg
*	功能说明: 写保持寄存器数据 
*	形    参: addr 地址
*	        : regs 寄存器数量
*           ：data 数据
*	返 回 值: NULL
**********************************************************************************************************/
void Write_Hold_Reg(unsigned int addr,unsigned int regs,unsigned char* data)
{
    unsigned short *ptemp = NULL;
    unsigned short i=0;
    
    ptemp = (unsigned short *)data;
    
    taskENTER_CRITICAL();//进入临界区保护
    
    if(addr < MODBUS_HOLDREG_LEN)
    {
        for(i=0;i<regs;i++)
        {
            ModsReg_Hold[addr + i] = ptemp[i];
        }
    }
    
    taskEXIT_CRITICAL();//退出临界区保护
}



