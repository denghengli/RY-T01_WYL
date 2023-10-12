#include "includes.h"

#define RXBUF_SIZE 20
#define TXBUF_SIZE 20

#define PM100_COMM_HEAD   0XAA
#define PM100_COMM_CMD    0XCF
#define PM100_COMM_TAIL   0XAB
#define PM100_COMM_LEN    0X0A

/*PM100 串口传输时使用的结构体*/
typedef struct
{
	unsigned char RxBuf[RXBUF_SIZE];
	volatile unsigned char RxCount;
	volatile unsigned char RxNewFlag;

	unsigned char TxBuf[TXBUF_SIZE];  
	volatile unsigned char TxCount;
	
}PM100_RXTX_T;

PM100_RXTX_T    stPM100RxTx; //PM100通讯数据结构


/**********************************************************************************************************
*	函 数 名: IsPM100RcvDone
*	功能说明: 判断PM100一帧是否接收完成
*	形    参: _cDat 中断接收数据
*	返 回 值: 1 接收完成，0 未接收完成
**********************************************************************************************************/
unsigned char IsPM100RcvDone(unsigned char _cDat)
{
    unsigned char cs = 0;
    
	if(stPM100RxTx.RxCount < RXBUF_SIZE)
    {
        stPM100RxTx.RxBuf[stPM100RxTx.RxCount++] = _cDat;
        
        if(stPM100RxTx.RxBuf[0] != PM100_COMM_HEAD)
        {
            memset(&stPM100RxTx,0,sizeof(PM100_RXTX_T));
            stPM100RxTx.RxNewFlag = 1;
			return 0;
        }
    }
    else
    {
        memset(&stPM100RxTx,0,sizeof(PM100_RXTX_T));
        stPM100RxTx.RxNewFlag = 1;
        return 0;
    }
    
    if(stPM100RxTx.RxCount == PM100_COMM_LEN)
    {
        cs = stPM100RxTx.RxBuf[2] + stPM100RxTx.RxBuf[3] + stPM100RxTx.RxBuf[4] + stPM100RxTx.RxBuf[5] + 
             stPM100RxTx.RxBuf[6] + stPM100RxTx.RxBuf[7];
        
        if(stPM100RxTx.RxBuf[1] != PM100_COMM_CMD || stPM100RxTx.RxBuf[9] != PM100_COMM_TAIL || cs != stPM100RxTx.RxBuf[8])
        {
            memset(&stPM100RxTx,0,sizeof(PM100_RXTX_T));
            stPM100RxTx.RxNewFlag = 1;
            return 0;
        }
        else return 1;  
    }

    return 0;
}


/**********************************************************************************************************
*	函 数 名: IsPM100RcvNew
*	功能说明: 在中断中判断是否可以接收下一帧
*	形    参: NULL
*	返 回 值: 1 or 0
**********************************************************************************************************/
unsigned char IsPM100RcvNew (void)
{
	
	return stPM100RxTx.RxNewFlag;
	
}

/**********************************************************************************************************
*	函 数 名: SetPM100RcvNewFlg
*	功能说明: 设置接收标志
*	形    参: 1 or 0
*	返 回 值: NULL
**********************************************************************************************************/
void SetPM100RcvNewFlg (unsigned char _cFlg)
{
	
	stPM100RxTx.RxNewFlag = _cFlg;
	
}

/**********************************************************************************************************
*	函 数 名: PM100_DataInit
*	功能说明: 接收标志初始化
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void PM100_DataInit(void)
{
	unsigned int i = 0;
	
	stPM100RxTx.RxCount   = 0;
	stPM100RxTx.TxCount   = 0;
    
	for(i=0;i<RXBUF_SIZE;i++)
	{
		stPM100RxTx.RxBuf[i] = 0;
	}
	for(i=0;i<TXBUF_SIZE;i++)
	{
		stPM100RxTx.TxBuf[i] = 0;
	}
	
	stPM100RxTx.RxNewFlag = 1;
}


/**********************************************************************************************************
*	函 数 名: PM100Ctl_Set
*	功能说明: PM100电压控制
*	形    参: _cSw 开关状态 1启动，0关闭；默认开启
*	返 回 值: NULL
**********************************************************************************************************/
void  PM100Ctl_Set(unsigned char _cSw)
{
    g_tSystemData.PM100Ctr1 = _cSw;
    Write_Holding_Reg((void*)&g_tSystemData.PM100Ctr1,eholdreg_pm100ctr1);
    
    if(_cSw == 1) //1开0关
    {
        BSP_Pin_Write(epin_pm100ctl,1);
    }
    else/* 关闭电源时需要清空数据*/
    {
        BSP_Pin_Write(epin_pm100ctl,0);
        g_tSystemData.PM100 = 0.0;
        Write_Holding_Reg((void*)&g_tSystemData.PM100,eholdreg_pm1001);
    }
    
}


/**********************************************************************************************************
*	函 数 名: PM100_Recv_Analysis
*	功能说明: 接收解析
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void PM100_Recv_Analysis()
{
    unsigned char cCtl = g_tSystemData.PM100Ctr1;
    
    if(cCtl == 1) //1启动，0关闭
    {
        Read_Holding_Reg((void*)&g_tSystemData.PM100Factor,eholdreg_pm100factor1);
        
        g_tSystemData.PM100 = stPM100RxTx.RxBuf[5] * 256 + stPM100RxTx.RxBuf[4];   //ug/m3 把测量数据存入全局变量中 */
        if(g_tSystemData.PM100Factor > 0.1) g_tSystemData.PM100 *= g_tSystemData.PM100Factor;
            
        /* 把测量数据存入保持寄存器中 */
        if(g_tSystemData.PM100 < 40000)
        {
            Write_Holding_Reg((void*)&g_tSystemData.PM100, eholdreg_pm1001);
        }
    }
    
    PM100_DataInit();
}

/**********************************************************************************************************
*	函 数 名: osPM100Task
*	功能说明: PM100颗粒物
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void osPM100Task(void  * argument)
{
    unsigned portBASE_TYPE uxHighWaterMark;
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(2000);
    
	PM100_DataInit();
    PM100Ctl_Set(1);//电源默认开启
    
	while(1)
	{
        if(xSemaphoreTake(xSemPM100,sMaxBlockTime) == pdTRUE)
        {
            PM100_Recv_Analysis();//接收解析
            
            uxHighWaterMark = uxTaskGetStackHighWaterMark(xHandle_PM100); 
            LOG_PRINT(DEBUG_TASK,"task9--->PM100，FreeStack: %d bytes\r\n",uxHighWaterMark*4);
            vTaskDelay(sMaxBlockTime);
            
        }
	}
}

