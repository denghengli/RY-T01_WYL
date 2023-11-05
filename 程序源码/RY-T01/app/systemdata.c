#include "includes.h"

#define FLASH_INITFLG 20200331

volatile SYSTEM_DATA_UN g_SysData;
static PARA_FLASH_T s_ParaData;

/**********************************************************************************************************
*	函 数 名: FlashSave
*	功能说明: FLASH存储，加入互斥量保护
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void FlashSave(unsigned int _iStartAddr,  const unsigned int * _piBuf, unsigned int _iBufSize)
{
    if(xSemaphoreTake(FlashMutex , 0) == pdTRUE)
    {
        FlashWrite(_iStartAddr,_piBuf,_iBufSize);
        xSemaphoreGive(FlashMutex);
    }
}

/**********************************************************************************************************
*	函 数 名: ParaData_Init
*	功能说明: 参数设置数据缓存，接收标志初始化
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void ParaData_Init(void)
{	
	//从flash中读出校准系数，如果第一次的话，则把默认值放入到flash中。如果flash中已经有校准系数的话，直接读出来放入数据结构中
	FlashRead((unsigned int*)&s_ParaData,FLASH_ADDR_UASER,sizeof(s_ParaData)/4);
	if(s_ParaData.InitFlag != FLASH_INITFLG)
	{
		/*给全局变量赋初始值*/
        s_ParaData.InitFlag = FLASH_INITFLG;
        s_ParaData.Para.devId = 1;
//        s_ParaData.Para.BlowIntvar     = 4;
//        s_ParaData.Para.SpeedAtAdjFlg  = 0;
//		s_ParaData.Para.Density        = 1.34;
//        s_ParaData.Para.PiTGRatioK     = 0.83;
//        s_ParaData.Para.SpeedRatioK    = 1;
//        s_ParaData.Para.DynPRatioB     = 0;
//        s_ParaData.Para.SticPRatioB    = 0;
//        s_ParaData.Para.DynPRatioK     = 1;
//        s_ParaData.Para.SticPRatioK    = 1;
//		s_ParaData.Para.PTTemRef       = 0;
//		s_ParaData.Para.PTTemRatioB    = 0;
//		s_ParaData.Para.PresCnt        = 25;
        
		FlashSave(FLASH_ADDR_UASER,(unsigned int*)&s_ParaData,sizeof(s_ParaData)/4);
	}
	  
	/*不管是不是第一次赋值，的需要写到g_SysData中*/
    g_SysData.Data.Para = s_ParaData.Para;
    g_SysData.Data.Para.speedCalibZeroFlg = 0;
    
    /*写入MODBUS寄存器*/
    Write_Hold_Reg(SAMPLE_DATA_LEN,PARA_DATA_LEN,(unsigned char *)&g_SysData.regBuf[SAMPLE_DATA_LEN]);
}

/**********************************************************************************************************
*	函 数 名: ParaData_Save
*	功能说明: 将系数写入FLASH
*	形    参: opt 1：在MODBUS中更改 0：在程序中自动更改
*	返 回 值: NULL
**********************************************************************************************************/
void ParaData_Save(unsigned char opt)
{
    if(opt == 1)
    {
        /*更新g_SysData.Para*/
        Read_Hold_Reg(SAMPLE_DATA_LEN,PARA_DATA_LEN,(unsigned char *)&g_SysData.regBuf[SAMPLE_DATA_LEN]);
    }
    else
    {
        /*更新MODBUS寄存器*/
        Write_Hold_Reg(SAMPLE_DATA_LEN,PARA_DATA_LEN,(unsigned char *)&g_SysData.regBuf[SAMPLE_DATA_LEN]);
    }
        
    s_ParaData.InitFlag = FLASH_INITFLG;
    s_ParaData.Para = g_SysData.Data.Para;

    FlashSave(FLASH_ADDR_UASER,(unsigned int*)&s_ParaData,sizeof(s_ParaData)/4);
}

/**********************************************************************************************************
*	函 数 名: ParaData_Updata
*	功能说明: 参数更新，从MODBUS读取设置的参数置g_SysData.Data.Para
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void ParaData_Updata(void)
{
    /*更新g_SysData.Para*/
    Read_Hold_Reg(SAMPLE_DATA_LEN,PARA_DATA_LEN,(unsigned char *)&g_SysData.regBuf[SAMPLE_DATA_LEN]);
}

/**********************************************************************************************************
*	函 数 名: DevId_Get
*	功能说明: 获取设备ID
*	形    参: NULL
*	返 回 值: ID
**********************************************************************************************************/
unsigned short  DevId_Get(void)
{
    return g_SysData.Data.Para.devId;
}

/**********************************************************************************************************
*	函 数 名: SampleData_ToModbus
*	功能说明: 将采样数据写入Modbus，一次性全部写入
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void SampleData_ToModbus(void)
{
	Write_Hold_Reg(0,SAMPLE_DATA_LEN,(unsigned char *)&g_SysData.regBuf[0]);
}


