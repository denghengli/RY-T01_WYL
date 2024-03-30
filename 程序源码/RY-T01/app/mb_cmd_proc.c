#include "includes.h"

/**
 * @brief Modbus命令处理函数类型
 *
 * @param beginAddr 请求命令寄存器地址
 * @param regNum 请求命令寄存器数量
 *
 * @return 返回：0成功;其他：modbus错误码
 */

typedef unsigned char (* MB_REG_PROC_FUNC_T)(unsigned char cmd, int beginAddr, int regNum);

//寄存器区定义
typedef struct MB_REG_REGION
{
	int beginAddr;  //起始寄存器地址
	int endAddr;    //结束寄存器地址
	MB_REG_PROC_FUNC_T procFunc;
	const char *desc;
}MB_REG_REGION_T;

/**********************************************************************************************************
*	                                        modbus保持寄存器反控
**********************************************************************************************************/
static uint8_t setBaudRateProc(unsigned char cmd, int beginAddr, int regNum)
{
    UART_CONFIG config;
    
    memset(&config, 0, sizeof(UART_CONFIG));
    config.BaudRate = g_SysData.Data.Para.baudRate;
    config.Parity = PARITY_NONE;
    config.DataBits = DATA_BITS_8;
    config.StopBits = STOP_BITS_1;
    Modify_UART_BaudRate(UARTPORT_COM, &config);
    
    return 1;
}

static uint8_t tempTargetCalibProc(unsigned char cmd, int beginAddr, int regNum)
{
    PT100_RefCal_Flag(1);
    return 1;
}

static uint8_t restoreFactory(unsigned char cmd, int beginAddr, int regNum)
{
    ParaData_Factory();
    return 1;
}

static uint8_t do1Ctrl(unsigned char cmd, int beginAddr, int regNum)
{
    g_SysData.Data.Para.DO[0] == 1 ? DRV_Pin_Write(epin_VALVE1,PIN_HIGH) : 
                                     DRV_Pin_Write(epin_VALVE1,PIN_LOW);
    return 1;
}

static uint8_t do2Ctrl(unsigned char cmd, int beginAddr, int regNum)
{
    g_SysData.Data.Para.DO[1] == 1 ? DRV_Pin_Write(epin_VALVE2,PIN_HIGH) : 
                                     DRV_Pin_Write(epin_VALVE2,PIN_LOW);
    return 1;
}

static uint8_t do3Ctrl(unsigned char cmd, int beginAddr, int regNum)
{
    g_SysData.Data.Para.DO[2] == 1 ? DRV_Pin_Write(epin_VALVE3,PIN_HIGH) : 
                                     DRV_Pin_Write(epin_VALVE3,PIN_LOW);
    return 1;
}

static uint8_t do4Ctrl(unsigned char cmd, int beginAddr, int regNum)
{
    g_SysData.Data.Para.DO[3] == 1 ? DRV_Pin_Write(epin_VALVE4,PIN_HIGH) : 
                                     DRV_Pin_Write(epin_VALVE4,PIN_LOW);
    return 1;
}

//保持寄存器区信息列表
static MB_REG_REGION_T MbRegRegionList[] = 
{
    //线圈寄存器

    //保持寄存器--命令
    {40001, 40002,  setBaudRateProc,        "tempTargetCalib"}, //设置波特率
    {40020, 40021,  tempTargetCalibProc,    "tempTargetCalib"}, //温度目标值校准
    {40064, 40064,  restoreFactory,         "restoreFactory"},  //恢复出厂设置
    
    {40065, 40065,  do1Ctrl,                 "do1Ctrl"},  //电磁阀控制
    {40066, 40066,  do2Ctrl,                 "do2Ctrl"},  //电磁阀控制
    {40067, 40067,  do3Ctrl,                 "do3Ctrl"},  //电磁阀控制
    {40068, 40068,  do4Ctrl,                 "do4Ctrl"},  //电磁阀控制
};

/**
 * @brief 处理Modbus请求命令
 *
 * @param beginAddr 请求命令寄存器地址
 * @param regNum 请求命令寄存器数量
 *
 * @return 返回：1成功;
 */
uint8_t dealMbCmd(unsigned char cmd, int beginAddr, int regNum)
{
	unsigned char errCode = 0;
	int i;

	do
	{
		if (regNum <= 0)
		{
			return 0;
		}

		//寄存器匹配
		for (i = 0; i < sizeof(MbRegRegionList)/sizeof(MB_REG_REGION_T); i++)
		{
			if (beginAddr >= MbRegRegionList[i].beginAddr && (beginAddr + regNum - 1) <= MbRegRegionList[i].endAddr)
			{
				errCode = MbRegRegionList[i].procFunc(cmd, beginAddr, regNum);
				return errCode;
			}
		}

		//无效寄存器范围
		if (i >= sizeof(MbRegRegionList)/sizeof(MB_REG_REGION_T))
		{
			return 0;
		}
		
	}while(0);
    
    return 0;
}

