#include "includes.h"

UART_TRANS_T  stComTrans;

/**********************************************************************************************************
*	�� �� ��: SetComRcvNewFlg
*	����˵��: ���ý��ձ�־
*	��    ��: 1 or 0
*	�� �� ֵ: NULL
**********************************************************************************************************/
void SetComRcvNewFlg (unsigned char _cFlg)
{
    stComTrans.RxNewFlag = _cFlg;
}

/**********************************************************************************************************
*	�� �� ��: IsComRcvNew
*	����˵��: ���ж����ж��Ƿ���Խ�����һ֡
*	��    ��: NULL
*	�� �� ֵ: 1 or 0
**********************************************************************************************************/
unsigned char IsComRcvNew (void)
{
    return stComTrans.RxNewFlag;
}


/**********************************************************************************************************
*	�� �� ��: IsComRcvDone
*	����˵��: �ж�ͨѶ����֡һ֡�Ƿ�������
*	��    ��: _cDat �жϽ�������
*	�� �� ֵ: 1 ������ɣ�0 δ�������
**********************************************************************************************************/
unsigned char IsComRcvDone(unsigned char _cDat)
{
    return Slave_IsRecvDone(&stComTrans,_cDat);
}


/**********************************************************************************************************
*	�� �� ��: Com_Recv_Analysis
*	����˵��: ͨѶ�����������
*	��    ��: NULL
*	�� �� ֵ: NULL
**********************************************************************************************************/
static void Com_Recv_Analysis(void)
{
    Slave_Recv_Proc(&stComTrans);
}

/**********************************************************************************************************
*	�� �� ��: Com_Resp_Send
*	����˵��: ͨѶ����Ӧ����
*	��    ��: NULL
*	�� �� ֵ: NULL
**********************************************************************************************************/
static void Com_Resp_Send(void)
{
    Uart_SendData(UARTPORT_COM,stComTrans.TxBuf,stComTrans.TxCount);
}

/**********************************************************************************************************
*	�� �� ��: Com_DataInit
*	����˵��: ͨѶ���ݻ��棬���ձ�־��ʼ��
*	��    ��: NULL
*	�� �� ֵ: NULL
**********************************************************************************************************/
void Com_DataInit(void)
{
    memset(&stComTrans,0,sizeof(stComTrans));
	stComTrans.RxNewFlag = 1;
}

//1�������ݴ�modbus�����ж�ȡ��g_sysData
//2������������
void dealMbWriteCmd(UART_TRANS_T *_pTrans)
{
    uint16_t addr, reg_addr = (_pTrans->RxBuf[2] << 8) | _pTrans->RxBuf[3];
    uint16_t reg_num = 0;
    
    if (_pTrans->RxBuf[1] == MODBUS_SIMCOIL_REG_WR)/*0x5 д������Ȧ�Ĵ��� */
    {
        addr = reg_addr / 8;
    }
    else if (_pTrans->RxBuf[1] == MODBUS_MULCOIL_REG_WR)/*0x0F д�����Ȧ�Ĵ��� */
    {
        reg_num = (_pTrans->RxBuf[4] << 8) | _pTrans->RxBuf[5];
        reg_num = (reg_num + 7) / 8;
        reg_addr = reg_addr / 8;
    }
    else if (_pTrans->RxBuf[1] == MODBUS_SIMHOLD_REG_WR)/*0x6д�������ּĴ���*/
    {
        //������
        if(reg_addr >= 50)
        {
            ParaData_Updata();
            dealMbCmd(MODBUS_SIMHOLD_REG_WR, reg_addr + 40000, 1);
        }
        //��������Ҫ����FLASH
        else
        {
            ParaData_Updata();
            dealMbCmd(MODBUS_SIMHOLD_REG_WR, reg_addr + 40000, 1);
            ParaData_Save(1);
        }
    }
    else if (_pTrans->RxBuf[1] == MODBUS_MULHOLD_REG_WR)/*0x10 д������ּĴ���  */ 
    {
        reg_num = (_pTrans->RxBuf[4] << 8) | _pTrans->RxBuf[5];

        //������
        if(reg_addr >= 50)
        {
            ParaData_Updata();
            dealMbCmd(MODBUS_MULHOLD_REG_WR, reg_addr + 40000, reg_num);
        }
        //��������Ҫ����FLASH
        else
        {
            ParaData_Updata();
            dealMbCmd(MODBUS_MULHOLD_REG_WR, reg_addr + 40000, reg_num);
            ParaData_Save(1);
        }
    }
}

/**********************************************************************************************************
*	�� �� ��: App_Comm
*	����˵��: App_Comm function
*	��    ��: argument   ����ָ��
*	�� �� ֵ: NONE
**********************************************************************************************************/
void App_Comm(void *pvParameters)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
    
    Com_DataInit();
    
    while(1) 
    {      
        if(xSemaphoreTake(CommSem,sMaxBlockTime) == pdTRUE)
        {
            Com_Recv_Analysis();//�������
            Com_Resp_Send();//Ӧ����
            dealMbWriteCmd(&stComTrans); //����modbusдָ��
            Com_DataInit();//���������ջ�����

            LOG_PRINT(DEBUG_TASK,"App_Comm \r\n");
            vTaskDelay(sMaxBlockTime);
        }
    }
}

