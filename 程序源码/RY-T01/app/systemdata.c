#include "includes.h"

#define SOFTWARE_VERSION 104

//初次上点判断标识,不要变!!!!
#define FLASH_INITFLG 0x22223355 
//每次修改或新增参数后新增如下格式宏
#define PARA_MODIFY_ADDR_20231112 STM32_FLASH_ADDR_PARA_MODFYFLG
#define PARA_MODIFY_ADDR_20231113 (PARA_MODIFY_ADDR_20231112 + 4)

volatile SYSTEM_DATA_UN g_SysData;
volatile PARA_FLASH_T s_FactoryParaData;
static PARA_FLASH_T s_ParaData;

/**********************************************************************************************************
*	函 数 名: FlashSave
*	功能说明: FLASH存储，加入互斥量保护
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void FlashSave(unsigned int _iStartAddr,  const uint8_t * _pucBuf, unsigned int _iBufSize)
{
    if(xSemaphoreTake(FlashMutex , 0) == pdTRUE)
    {
        stm32_flash_write(_iStartAddr, (uint8_t*)_pucBuf, _iBufSize);
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
    uint32_t ModifyFlag;
    
	//从flash中读出校准系数，如果第一次的话，则把默认值放入到flash中。如果flash中已经有校准系数的话，直接读出来放入数据结构中
	stm32_flash_read(STM32_FLASH_ADDR_PARA, (uint8_t*)&s_ParaData, sizeof(s_ParaData));
	if(s_ParaData.InitFlag != FLASH_INITFLG)
	{
		/*给全局变量赋初始值*/
        s_ParaData.InitFlag = FLASH_INITFLG;
        s_ParaData.Para.devId = 1;
        s_ParaData.Para.baudRate = 9600;
        s_ParaData.Para.blowFreq = 5;
        s_ParaData.Para.blowInterval = 240;
        s_ParaData.Para.piTGRatioK = 1;
        s_ParaData.Para.speedRatioK = 1;
        s_ParaData.Para.density = 1.34;
        s_ParaData.Para.dynPRatioB = 0;
        s_ParaData.Para.sticPRatioB = 0;
        s_ParaData.Para.dynPRatioK = 1;
        s_ParaData.Para.sticPRatioK = 1;
        s_ParaData.Para.autoCalibZero = 0;
        s_ParaData.Para.ptTemRef = 0;
        s_ParaData.Para.ptTemRatioB = 0;
        s_ParaData.Para.humitZero = 0;
        s_ParaData.Para.humitZreoValib = 0;
        s_ParaData.Para.humitDispUnit = 1;
        s_ParaData.Para.sectionArea = 50;
        s_ParaData.Para.smoothTime = 5;
        s_ParaData.Para.humitType = 0;
        s_ParaData.Para.temAOOffset = 0;
        s_ParaData.Para.temAOK = 1;
        s_ParaData.Para.pressAOOffset = 0;
        s_ParaData.Para.pressAOK = 1;
        s_ParaData.Para.speedAOOffset = 0;
        s_ParaData.Para.speedAOK = 1;
        s_ParaData.Para.humitAOOffset = 0;
        s_ParaData.Para.humitAOK = 1;
        memset(s_ParaData.Para.humitFrepTab, 0, sizeof(s_ParaData.Para.humitFrepTab));
		FlashSave(STM32_FLASH_ADDR_PARA, (uint8_t*)&s_ParaData, sizeof(s_ParaData));
	}
	  
    /*备份参数*/
    stm32_flash_read(STM32_FLASH_ADDR_PARA_FACTORY, (uint8_t*)&s_FactoryParaData, sizeof(s_FactoryParaData));
	if(s_FactoryParaData.InitFlag != FLASH_INITFLG)
    {
        s_FactoryParaData.InitFlag = FLASH_INITFLG;
        s_FactoryParaData.Para.devId = 1;
        s_FactoryParaData.Para.baudRate = 9600;
        s_FactoryParaData.Para.blowFreq = 5;
        s_FactoryParaData.Para.blowInterval = 240;
        s_FactoryParaData.Para.piTGRatioK = 1;
        s_FactoryParaData.Para.speedRatioK = 1;
        s_FactoryParaData.Para.density = 1.34;
        s_FactoryParaData.Para.dynPRatioB = 0;
        s_FactoryParaData.Para.sticPRatioB = 0;
        s_FactoryParaData.Para.dynPRatioK = 1;
        s_FactoryParaData.Para.sticPRatioK = 1;
        s_FactoryParaData.Para.autoCalibZero = 0;
        s_FactoryParaData.Para.ptTemRef = 0;
        s_FactoryParaData.Para.ptTemRatioB = 0;
        s_FactoryParaData.Para.humitZero = 0;
        s_FactoryParaData.Para.humitZreoValib = 0;
        s_FactoryParaData.Para.humitDispUnit = 1;
        s_FactoryParaData.Para.sectionArea = 50;
        s_FactoryParaData.Para.smoothTime = 5;
        s_FactoryParaData.Para.humitType = 0;
        s_FactoryParaData.Para.temAOOffset = 0;
        s_FactoryParaData.Para.temAOK = 1;
        s_FactoryParaData.Para.pressAOOffset = 0;
        s_FactoryParaData.Para.pressAOK = 1;
        s_FactoryParaData.Para.speedAOOffset = 0;
        s_FactoryParaData.Para.speedAOK = 1;
        s_FactoryParaData.Para.humitAOOffset = 0;
        s_FactoryParaData.Para.humitAOK = 1;
        memset((void*)s_FactoryParaData.Para.humitFrepTab, 0, sizeof(s_FactoryParaData.Para.humitFrepTab));
        FlashSave(STM32_FLASH_ADDR_PARA_FACTORY, (uint8_t*)&s_FactoryParaData, sizeof(s_FactoryParaData));
    }
    
    /*判断参数是否需要修改或新增*/
    stm32_flash_read(PARA_MODIFY_ADDR_20231112, (uint8_t*)&ModifyFlag, 4);
	if(ModifyFlag != 20231112)
    {
        ModifyFlag = 20231112;
        FlashSave(PARA_MODIFY_ADDR_20231112, (uint8_t*)&ModifyFlag, 4);
    }
    
	/*不管是不是第一次赋值，的需要写到g_SysData中*/
    g_SysData.Data.Para = s_ParaData.Para;
    g_SysData.Data.Para.factoryFlg = 0;
    memset((void*)&g_SysData.Data.Para.DO[0], 0, sizeof(g_SysData.Data.Para.DO));
    
    g_SysData.Data.Sample.softVer = SOFTWARE_VERSION;
    /*写入MODBUS寄存器*/
    Slave_Write_Reg(EMB_HOLE, 0, PARA_DATA_LEN, (unsigned char *)&g_SysData.regBuf[SAMPLE_DATA_LEN]);
}

/**********************************************************************************************************
*	函 数 名: ParaData_Save
*	功能说明: 将系数写入FLASH
*	形    参: opt 1:在MODBUS中更改 0:在程序中自动更改
*	返 回 值: NULL
**********************************************************************************************************/
void ParaData_Save(unsigned char opt)
{
    if(opt == 1)
    {
        /*更新g_SysData.Para*/
        Slave_Read_Reg(EMB_HOLE, 0, PARA_DATA_LEN, (unsigned char *)&g_SysData.regBuf[SAMPLE_DATA_LEN]);
    }
    else
    {
        /*更新MODBUS寄存器*/
        Slave_Write_Reg(EMB_HOLE, 0, PARA_DATA_LEN, (unsigned char *)&g_SysData.regBuf[SAMPLE_DATA_LEN]);
    }
        
    s_ParaData.InitFlag = FLASH_INITFLG;
    s_ParaData.Para = g_SysData.Data.Para;

    FlashSave(STM32_FLASH_ADDR_PARA, (uint8_t*)&s_ParaData, sizeof(s_ParaData));
}

/**********************************************************************************************************
*	函 数 名: ParaData_Factory
*	功能说明: 恢复出厂参数
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void ParaData_Factory(void)
{
    UART_CONFIG config;
    
    //修改串口波特率
    if(g_SysData.Data.Para.baudRate != s_FactoryParaData.Para.baudRate)
    {
        memset(&config, 0, sizeof(UART_CONFIG));
        config.BaudRate = s_FactoryParaData.Para.baudRate;
        config.Parity = PARITY_NONE;
        config.DataBits = DATA_BITS_8;
        config.StopBits = STOP_BITS_1;
        Modify_UART_BaudRate(UARTPORT_COM, &config);
    }
    
    g_SysData.Data.Para = s_FactoryParaData.Para;
    ParaData_Save(0);
}

/**********************************************************************************************************
*	函 数 名: ParaData_Updata
*	功能说明: 参数更新，从MODBUS读取设置的参数置g_SysData.Data.Para
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void ParaData_Updata(void)
{
    Slave_Read_Reg(EMB_HOLE, 0, PARA_DATA_LEN, (unsigned char *)&g_SysData.regBuf[SAMPLE_DATA_LEN]);
}

/**********************************************************************************************************
*	函 数 名: SampleData_ToModbus
*	功能说明: 将采样数据写入Modbus，一次性全部写入
*	形    参: NULL
*	返 回 值: NULL
**********************************************************************************************************/
void SampleData_ToModbus(void)
{
    Slave_Write_Reg(EMB_IPUT, 0, SAMPLE_DATA_LEN, (unsigned char *)&g_SysData.regBuf[0]);
}

//获取modbus地址
uint16_t SlaveId_Get(void)
{
    return g_SysData.Data.Para.devId;
}

uint16_t MasterId_Get(void)
{
    return 1;
}
