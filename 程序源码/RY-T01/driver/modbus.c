#include "includes.h"

/*��������*/
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
//*	�� �� ��: UHUL_TO_U16
//*	����˵��: �ַ�ת��
//*	��    ��: _cPtr   ����ָ��
//*	�� �� ֵ: short����
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
//*	�� �� ��: UHUL_TO_U16
//*	����˵��: �ַ�ת��
//*	��    ��: _cPtr   ����ָ��
//*	�� �� ֵ: short����
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
*	�� �� ��: ModsCRC16
*	����˵��: MODBUS CRC����
*	��    ��: _cPtr   ����ָ��
*           _shLen  ���ݳ���
*	�� �� ֵ: CRC
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
*	�� �� ��: IsModsRecvDone
*	����˵��: ���ж��н���һ֡�������ݰ�
*     ˵�������ַ�ʽû�в��ö�ʱ3.5�ַ��б�֡���������ܱ߽��ձ߽���
*           ÿ�ֹ�������շ�ʽ��һ����ֻ���ڽ������������ٽ���
*	��    ��: _ptTrans ������
*           ��_cDat    ����
*	�� �� ֵ: 1 ������ɣ�0 δ�������
**********************************************************************************************************/
unsigned char IsModsRecvDone(UART_TRANS_T* _ptTrans,unsigned char _cDat)
{
	unsigned char cRet = 0;
	unsigned short shDevId = DevId_Get();
    
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
	
	/*������Ϊ1 3 4 5 6ʱ�����յ�8�ֽ�ʱ��ʾһ֡�������*/
	if(_ptTrans->RxBuf[1] == MODBUS_COIL_REG_RD || _ptTrans->RxBuf[1] == MODBUS_HOLDING_REG_RD  || 
	   _ptTrans->RxBuf[1] == MODBUS_IN_REG_RD   || _ptTrans->RxBuf[1] == MODBUS_SIMPCOIL_REG_WR || 
	   _ptTrans->RxBuf[1] == MODBUS_SIMPHOLDING_REG_WR)
	{
		if(_ptTrans->RxCount == 8)
		{
			cRet = 1;
		}
	}
	/*������Ϊ10Hʱ�����յ�9�ֽ� + stModsRxTx.RxBuf[6]ʱ��ʾһ֡�������*/
	else if(_ptTrans->RxBuf[1] == MODBUS_MULHOLDING_REG_WR)
	{
		if(_ptTrans->RxCount == (9 + _ptTrans->RxBuf[6]))
		{
			cRet = 1;
		}
	}
//	/*������Ϊ0FHʱ�����յ�9�ֽ� + stModsRxTx.RxBuf[6]ʱ��ʾһ֡�������*/
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
*	�� �� ��: Mods_AddCRC
*	����˵��: ׷��2�ֽ�CRC��stModsRxTx.TxBuf;
*	��    ��: _ptTrans ������
*	�� �� ֵ: NONE
**********************************************************************************************************/
static void Mods_AddCRC(UART_TRANS_T* _ptTrans)
{
	unsigned short shTxCRC;
	
	shTxCRC = ModsCRC16(_ptTrans->TxBuf,_ptTrans->TxCount);
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = shTxCRC; //��д��
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = shTxCRC >> 8;
	
}

/**********************************************************************************************************
*	�� �� ��: Mods_SetAckErr
*	����˵��: ����쳣֡��Ϣ
*	��    ��: _ptTrans ������
*             _ucLen  ���ݳ���
*	�� �� ֵ: NONE
**********************************************************************************************************/
static void Mods_SetAckErr (UART_TRANS_T* _ptTrans,unsigned char _ucErrCode)
{
	_ptTrans->TxRspCode = _ucErrCode;
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1] | 0x80;
	_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->TxRspCode;

}

/**********************************************************************************************************
*	�� �� ��: Mods_Recv_Analysis
*	����˵��: �������н���MODBUS
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
**********************************************************************************************************/
void Mods_Recv_Analysis(UART_TRANS_T* _ptTrans)
{	
	unsigned short shRxCRC=0,shCalcCRC=0;
    
	shCalcCRC = ModsCRC16(_ptTrans->RxBuf,  _ptTrans->RxCount - 2);
	shRxCRC   = ULUH_TO_U16(_ptTrans->RxBuf + _ptTrans->RxCount - 2);
        
	/*��ַ��֡���ȣ�CRCУ�飻������ֱ�Ӷ���*/
	if(shRxCRC != shCalcCRC)
	{
		return; 
	}
	
	switch(_ptTrans->RxBuf[1])/*���ܴ���*/
	{
		case MODBUS_COIL_REG_RD:  /*0x1 ����Ȧ�Ĵ��� */
			break;      	
		
		case MODBUS_HOLDING_REG_RD: /* 0x3�����ּĴ��� */
			Mods_Rd_Hold_Proc(_ptTrans); 
			break; 
		
		case MODBUS_IN_REG_RD:    	 /*0x4 ������Ĵ��� */
			Mods_Rd_In_Proc(_ptTrans);
			break;
		
		case MODBUS_SIMPCOIL_REG_WR:/*0x5 д������Ȧ�Ĵ��� */
			break;
		
		case MODBUS_SIMPHOLDING_REG_WR: 	/*0x6д�������ּĴ���*/
			Mods_Wr_SimpHold_Proc(_ptTrans);
			break;
		
		case MODBUS_MULHOLDING_REG_WR:      /*0x10 д������ּĴ���  */
			Mods_Wr_MulHold_Proc(_ptTrans);
			break;
		
		case MODBUS_MULCOIL_REG_WR: 	    /*0x0F д�����Ȧ�Ĵ��� */
			Mods_Wr_MulCoil_Proc(_ptTrans);
			break;
		
		default :
			Mods_SetAckErr(_ptTrans,RSP_ERR_CMD);
			Mods_AddCRC(_ptTrans);
			break;
    }
	
}



/**********************************************************************************************************
*	�� �� ��: Mods_Rd_Hold_Proc
*	����˵��: 0x3�����ּĴ���
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
* ����    ��id 0x03 addr-h addr-l  regnum-h regnum-l crc-h crc-l
* ��Ӧ    ��id 0x03 bytes data1-h data1-l   ...     crc-h crc-l
**********************************************************************************************************/
void Mods_Rd_Hold_Proc(UART_TRANS_T* _ptTrans)
{
	unsigned short shRegStartAddr = UHUL_TO_U16(_ptTrans->RxBuf + 2);
	unsigned short shRegNum       = UHUL_TO_U16(_ptTrans->RxBuf + 4);
	unsigned short cStFoot,cEndFoot,i = 0;
	unsigned char  cTxDatNum;

	/*��ַУ��*/
	if(shRegStartAddr + shRegNum > MODBUS_HOLDREG_LEN)
	{
		Mods_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Mods_AddCRC(_ptTrans);
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
            /*CDAB AB*/
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = ModsReg_Hold[i+cStFoot] >> 8; //��8λ
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = ModsReg_Hold[i+cStFoot];      //��8λ

		}

		Mods_AddCRC(_ptTrans);	
	}
}


/**********************************************************************************************************
*	�� �� ��: Mods_Rd_In_Proc
*	����˵��: 0x4 ������Ĵ���
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
* ����    ��id 0x04 addr-h addr-l  regnum-h regnum-l crc-h crc-l
* ��Ӧ    ��id 0x04 bytes data1-h data1-l   ...     crc-h crc-l
* ˵��    ��һ������ռ2���Ĵ�����һ���Ĵ���ռ2�ֽڣ���ַ��0x00��ʼ��
**********************************************************************************************************/
void Mods_Rd_In_Proc(UART_TRANS_T* _ptTrans)
{
	unsigned short shRegStartAddr = UHUL_TO_U16(_ptTrans->RxBuf + 2);
	unsigned short shRegNum       = UHUL_TO_U16(_ptTrans->RxBuf + 4);
	unsigned short cStFoot,cEndFoot,i = 0;
	unsigned char  cTxDatNum;

	/*��ַУ��*/
	if(shRegStartAddr + shRegNum > MODBUS_INPUTREG_LEN)
	{
		Mods_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Mods_AddCRC(_ptTrans);
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
			/*CDAB AB*/
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = ModsReg_Input[i+cStFoot] >> 8; //��8λ
            _ptTrans->TxBuf[_ptTrans->TxCount ++] = ModsReg_Input[i+cStFoot];      //��8λ
		}

		Mods_AddCRC(_ptTrans);	
	}
}


/**********************************************************************************************************
*	�� �� ��: Mods_Wr_SimpHold_Proc
*	����˵��: 0x6д�������ּĴ���
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
* ����    ��id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
* ��Ӧ    ��id 0x06 addr-h addr-l  data-h  data-l   crc-h crc-l
**********************************************************************************************************/
void Mods_Wr_SimpHold_Proc(UART_TRANS_T* _ptTrans)
{
	unsigned short shRegStartAddr = UHUL_TO_U16(_ptTrans->RxBuf + 2);
	unsigned short value=0;

	/*��ַУ��*/	
	if(shRegStartAddr > MODBUS_HOLDREG_LEN)
	{
		Mods_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Mods_AddCRC(_ptTrans);
	}
	else
	{
        if(shRegStartAddr == 30)//�豸ID
        {
            /*CDAB AB*/
            value = _ptTrans->RxBuf[4]<<8 | _ptTrans->RxBuf[5];
            ModsReg_Hold[shRegStartAddr] = value;

            /* �����洢 */
            ParaData_Save(1);
        }
        else if(shRegStartAddr == 32)//���ټ��Զ�У׼��־
        {
            /*CDAB AB*/
            value = _ptTrans->RxBuf[4]<<8 | _ptTrans->RxBuf[5];
            ModsReg_Hold[shRegStartAddr] = value;     
            
            ParaData_Updata();
        }
        else
        {
            /* ����µ��ֶ� */
        }
        
		/*��Ӧ���*/
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//�Ĵ�����ַ��λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//�Ĵ�����ַ��λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//���ݸ�λ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//���ݵ�λ

		Mods_AddCRC(_ptTrans);	     
                
	}
}


/**********************************************************************************************************
*	�� �� ��: Mods_Wr_MulHold_Proc
*	����˵��: 0x10 д������ּĴ���
*	��    ��: _ptTrans ������
*	�� �� ֵ: NULL
* ����    ��0x11 0x10 addr-h addr-l  regnum-h regnum-l bytes data1-h  data1-l ...  crc-h crc-l
* ��Ӧ    ��0x11 0x10 addr-h addr-l  regnum-h regnum-l crc-h crc-l
* ˵��    ����Ϊ��дһ�����ݣ��Ĵ�����������Ϊ 2;
*���ݸ�ʽΪ  float DCBA  ��u16 BA
**********************************************************************************************************/
void Mods_Wr_MulHold_Proc(UART_TRANS_T* _ptTrans)
{
    unsigned short shRegStartAddr = UHUL_TO_U16(_ptTrans->RxBuf + 2);
	unsigned short shRegNum       = UHUL_TO_U16(_ptTrans->RxBuf + 4);
	unsigned short cStFoot,cEndFoot,i = 0;
	unsigned short temp = 0;
	
	/*��ַУ��*/	
	if(shRegStartAddr + shRegNum  > MODBUS_HOLDREG_LEN)
	{
		Mods_SetAckErr(_ptTrans,RSP_ERR_REG_ADDR);
		Mods_AddCRC(_ptTrans);
	}
	else
	{		
		/*д�뱣�ּĴ���*/		
		cStFoot  = shRegStartAddr;
		cEndFoot = shRegStartAddr + shRegNum;
		for(i=0; i < cEndFoot - cStFoot; i++)
		{
		  	/*���յ�ΪCDAB����CDAB˳�����*/
			temp =  _ptTrans->RxBuf[i*2 + 7] <<8 | _ptTrans->RxBuf[i*2 + 1 + 7];
			
			if ( (i+cStFoot) == 48 )//PT100�¶Ȳο�ֵ
			{
				PT100_RefCal_Flag(1);
			}
			else if ( (i+cStFoot) == 52 )//ȫѹ��ѹ����ֵ,Խ����Ӧʱ��Խ��,ƽ����Խ�ã�ԽС���෴�����50��Ĭ��20
			{
			  	if (temp < 1) temp = 1;
				else if (temp > 50) temp = 50;
			}
            ModsReg_Hold[cStFoot+i] = temp;
		}
		
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[0];//��ַ
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[1];//������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[2];//��ַ��
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[3];//��ַ��
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[4];//�Ĵ���������
		_ptTrans->TxBuf[_ptTrans->TxCount ++] = _ptTrans->RxBuf[5];//�Ĵ���������
		
		Mods_AddCRC(_ptTrans);	
			
        /*ϵ����ƫ������������ú��������Ч*/
        ParaData_Save(1);
	}
}


/**********************************************************************************************************
*	�� �� ��: Mods_Wr_MulCoil_Proc
*	����˵��: 0x0F д�����Ȧ�Ĵ���  
*	��    ��: NULL
*	�� �� ֵ: NULL
* ����    ��id 0x0f addr-h addr-l regnum-h regnum-l bytes data1 ... crc-h crc-l
* ��Ӧ    ��id 0x0f addr-h addr-l regnum-h regnum-l bytes crc-h crc-l
* ˵��    ���͵�ַ��Ӧ��λ
**********************************************************************************************************/
void Mods_Wr_MulCoil_Proc(UART_TRANS_T* _ptTrans)
{
    ;
}

/**********************************************************************************************************
*	�� �� ��: Read_Hold_Reg
*	����˵��: �����ּĴ������� 
*	��    ��: addr ��ַ
*	        : regs �Ĵ�������
*           ��data ����
*	�� �� ֵ: NULL
**********************************************************************************************************/
void Read_Hold_Reg (unsigned int addr,unsigned int regs,unsigned char* data)
{
    unsigned short *ptemp = NULL;
    unsigned short i=0;
    
    ptemp = (unsigned short *)data;
     
    taskENTER_CRITICAL();//�����ٽ�������
    
    if(addr < MODBUS_HOLDREG_LEN)
    {
        for(i=0;i<regs;i++)
        {
            ptemp[i] = ModsReg_Hold[addr + i];
        }
    }
    
    taskEXIT_CRITICAL();//�˳��ٽ�������
}

/**********************************************************************************************************
*	�� �� ��: Write_Hold_Reg
*	����˵��: д���ּĴ������� 
*	��    ��: addr ��ַ
*	        : regs �Ĵ�������
*           ��data ����
*	�� �� ֵ: NULL
**********************************************************************************************************/
void Write_Hold_Reg(unsigned int addr,unsigned int regs,unsigned char* data)
{
    unsigned short *ptemp = NULL;
    unsigned short i=0;
    
    ptemp = (unsigned short *)data;
    
    taskENTER_CRITICAL();//�����ٽ�������
    
    if(addr < MODBUS_HOLDREG_LEN)
    {
        for(i=0;i<regs;i++)
        {
            ModsReg_Hold[addr + i] = ptemp[i];
        }
    }
    
    taskEXIT_CRITICAL();//�˳��ٽ�������
}



