#include "includes.h"

UART_TRANS_T  stComTrans;

/**********************************************************************************************************
*	函 数 名: SetComRcvNewFlg
*	功能说明: 设置接收标志
*	形    参: 1 or 0
*	返 回 值: NULL
**********************************************************************************************************/
void SetComRcvNewFlg (unsigned char _cFlg)
{
    stComTrans.RxNewFlag = _cFlg;
}

/**********************************************************************************************************
*	函 数 名: IsComRcvNew
*	功能说明: 在中断中判断是否可以接收下一帧
*	形    参: NULL
*	返 回 值: 1 or 0
**********************************************************************************************************/
unsigned char IsComRcvNew (void)
{
    return stComTrans.RxNewFlag;
}


/**********************************************************************************************************
*	函 数 名: IsComRcvDone
*	功能说明: 判断通讯请求帧一帧是否接收完成
*	形    参: _cDat 中断接收数据
*	返 回 值: 1 接收完成，0 未接收完成
**********************************************************************************************************/
unsigned char IsComRcvDone(unsigned char _cDat)
{
    return Slave_IsRecvDone(&stComTrans,_cDat);
}


/**********************************************************************************************************
*	函 数 名: Com_Recv_Analysis
*	功能说明: 通讯请求解析处理
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
static void Com_Recv_Analysis(void)
{
    Slave_Recv_Proc(&stComTrans);
}

/**********************************************************************************************************
*	函 数 名: Com_Resp_Send
*	功能说明: 通讯请求应答处理
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
static void Com_Resp_Send(void)
{
    Uart_SendData(UARTPORT_COM,stComTrans.TxBuf,stComTrans.TxCount);
}

/**********************************************************************************************************
*	函 数 名: Com_DataInit
*	功能说明: 通讯数据缓存，接收标志初始化
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void Com_DataInit(void)
{
    memset(&stComTrans,0,sizeof(stComTrans));
	stComTrans.RxNewFlag = 1;
}

//1、将数据从modbus缓存中读取到g_sysData
//2、处理反控命令
void dealMbWriteCmd(UART_TRANS_T *_pTrans)
{
    uint16_t addr, reg_addr = (_pTrans->RxBuf[2] << 8) | _pTrans->RxBuf[3];
    uint16_t reg_num = 0;
    
    if (_pTrans->RxBuf[1] == MODBUS_SIMCOIL_REG_WR)/*0x5 写单个线圈寄存器 */
    {
        addr = reg_addr / 8;
    }
    else if (_pTrans->RxBuf[1] == MODBUS_MULCOIL_REG_WR)/*0x0F 写多个线圈寄存器 */
    {
        reg_num = (_pTrans->RxBuf[4] << 8) | _pTrans->RxBuf[5];
        reg_num = (reg_num + 7) / 8;
        reg_addr = reg_addr / 8;
    }
    else if (_pTrans->RxBuf[1] == MODBUS_SIMHOLD_REG_WR)/*0x6写单个保持寄存器*/
    {
        //控制类
        if(reg_addr >= 50)
        {
            ParaData_Updata();
            dealMbCmd(MODBUS_SIMHOLD_REG_WR, reg_addr + 40000, 1);
        }
        //参数类需要存入FLASH
        else
        {
            ParaData_Updata();
            dealMbCmd(MODBUS_SIMHOLD_REG_WR, reg_addr + 40000, 1);
            ParaData_Save(1);
        }
    }
    else if (_pTrans->RxBuf[1] == MODBUS_MULHOLD_REG_WR)/*0x10 写多个保持寄存器  */ 
    {
        reg_num = (_pTrans->RxBuf[4] << 8) | _pTrans->RxBuf[5];

        //控制类
        if(reg_addr >= 50)
        {
            ParaData_Updata();
            dealMbCmd(MODBUS_MULHOLD_REG_WR, reg_addr + 40000, reg_num);
        }
        //参数类需要存入FLASH
        else
        {
            ParaData_Updata();
            dealMbCmd(MODBUS_MULHOLD_REG_WR, reg_addr + 40000, reg_num);
            ParaData_Save(1);
        }
    }
}

/**********************************************************************************************************
*	函 数 名: App_Comm
*	功能说明: App_Comm function
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void App_Comm(void *pvParameters)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
    
    Com_DataInit();
    
    while(1) 
    {      
        if(xSemaphoreTake(CommSem,sMaxBlockTime) == pdTRUE)
        {
            Com_Recv_Analysis();//请求解析
            Com_Resp_Send();//应答发送
            dealMbWriteCmd(&stComTrans); //处理modbus写指令
            Com_DataInit();//解析完后清空缓冲区

            LOG_PRINT(DEBUG_TASK,"App_Comm \r\n");
            vTaskDelay(sMaxBlockTime);
        }
    }
}

