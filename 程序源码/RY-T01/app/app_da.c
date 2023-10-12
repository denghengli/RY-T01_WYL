#include "includes.h"

//#define CHIP_CODE      0x0C  //器件代码，默认为1100
//#define CHIP_ADDR      0X00  //i2c地址位，默认为000，可设
////#define MCP4728_ADDR   ((CHIP_CODE << 3) | CHIP_ADDR)
//#define MCP4728_ADDR   0xC0
//
//#define MCP4728_CMD_SETVREF 0X04  //设置电压基准，C2=1 C1=0 C0=0
//#define MCP4728_CMD_SETON   0X05  //关断选择,     C2=1 C1=0 C0=1
//#define MCP4728_CMD_SETG    0X06  //增益设置，    C2=1 C1=1 C0=0
//#define MCP4728_CMD_WRFAST  0X00  //快速写，      C2=0 C1=0 
//#define MCP4728_CMD_WRCONT  0X0A  //连续写，      C2=0 C1=1 C0=0 W1=1 W0=0
//
//#define MCP4728_RDY_READ()    DRV_Pin_Read (epin_dac_rdy)
//#define MCP4728_LDAC_SETH()   DRV_Pin_Write(epin_dac_ldac, PIN_HIGH)
//#define MCP4728_LDAC_SETL()   DRV_Pin_Write(epin_dac_ldac, PIN_LOW)

typedef struct 
{
    DRV_I2C_T  I2C;
    struct RegConfig
    {
        unsigned char  Vref; //电压基准选择 0 = VDD, 1 = 2.048
        unsigned char  Gain; //增益选择 0 = 1， 1 = 2
        unsigned char  ChSel;//通道选择 bit0:通道A，bit1:通道B，bit2:通道C，bit3:通道D

    }Config;

    unsigned short DAC[4]; //4通道DAC输入代码

}MCP4728_T;

static MCP4728_T       s_tMCP4728;  //MCP4728
//
//
///**********************************************************************************************************
//*	函 数 名: MCP4728_Set_Gain
//*	功能说明: 设置增益, 0 = 1， 1 = 2
//*	形    参: NONE
//*	返 回 值: 0失败 1成功
//**********************************************************************************************************/
//static unsigned char MCP4728_Set_Gain(void)
//{
//    unsigned char cWrData = 0;
//
//    if (!MCP4728_RDY_READ())//处于忙碌状态
//        return 0;
//
//	DRV_I2C_Start(s_tMCP4728.I2C);
//	
//	if (DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, MCP4728_ADDR | CMD_MODE_WRITE))
//	{
//        cWrData = s_tMCP4728.Config.Gain? (MCP4728_CMD_SETG << 5) | 0X0F : (MCP4728_CMD_SETG << 5) | 0;//0XC0
//        
//        DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, cWrData);
//    }
//	else return 0;
//
//    DRV_I2C_Stop(s_tMCP4728.I2C);
//	
//	return 1;
//}
//
//
///**********************************************************************************************************
//*	函 数 名: MCP4728_Set_ON
//*	功能说明: 关断选择，A B C D通道均设为正常模式
//*	形    参: NONE
//*	返 回 值: 0失败 1成功
//**********************************************************************************************************/
//static unsigned char MCP4728_Set_ON(void)
//{
//    unsigned char cWrData = 0;
//
//    if (!MCP4728_RDY_READ())//处于忙碌状态
//        return 0;
//
//	DRV_I2C_Start(s_tMCP4728.I2C);
//	
//	if (DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, MCP4728_ADDR | CMD_MODE_WRITE))
//	{
//        cWrData = MCP4728_CMD_SETON << 5;
//        
//        DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, cWrData);//0XA0
//        DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0);
//    }
//    else return 0;
//
//    DRV_I2C_Stop(s_tMCP4728.I2C);
//	
//	return 1;
//}
//
///**********************************************************************************************************
//*	函 数 名: MCP4728_Set_Vref
//*	功能说明: 设置电压基准 0 = VDD, 1 = 2.048
//*	形    参: NONE
//*	返 回 值: 0失败 1成功
//**********************************************************************************************************/
//static unsigned char MCP4728_Set_Vref(void)
//{
//    unsigned char cWrData = 0;
//
//    if (!MCP4728_RDY_READ())//处于忙碌状态
//        return 0;
//
//	DRV_I2C_Start(s_tMCP4728.I2C);
//	
//	if (DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, MCP4728_ADDR | CMD_MODE_WRITE))
//	{
//        cWrData = s_tMCP4728.Config.Vref? (MCP4728_CMD_SETVREF << 5) | 0X0F : (MCP4728_CMD_SETVREF << 5) | 0;//0X8F
//        
//        DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, cWrData);
//    }
//    else return 0;
//
//    DRV_I2C_Stop(s_tMCP4728.I2C);
//	
//	return 1;
//}
//
///**********************************************************************************************************
//*	函 数 名: MCP4728_Write_Fast
//*	功能说明: 快速写数据。按通道A至通道D顺序写DAC输入寄存器
//*	形    参: NONE
//*	返 回 值: 0失败 1成功
//**********************************************************************************************************/
//static unsigned char MCP4728_Write_Fast(void)
//{
//    unsigned char i = 0;
//    unsigned char cWrByte2 = 0;
//    unsigned char cWrByte3 = 0;
//
//    if (!MCP4728_RDY_READ())//处于忙碌状态
//        return 0;
//
//	DRV_I2C_Start(s_tMCP4728.I2C);
//	
//	if (DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, MCP4728_ADDR | CMD_MODE_WRITE))
//	{
//        for (i = 0; i < 4; i++)
//        {
//            cWrByte2 = (MCP4728_CMD_WRFAST << 6) | (s_tMCP4728.DAC[i] >> 8);//按通道A至通道D顺序写DAC输入寄存器
//            cWrByte3 = s_tMCP4728.DAC[i] & 0x0F;
//
//            DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, cWrByte2);
//            DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, cWrByte3);
//        }
//    }
//    else return 0;
//
//    DRV_I2C_Stop(s_tMCP4728.I2C);
//	
//	return 1;
//}
//
///**********************************************************************************************************
//*	函 数 名: MCP4728_Write_Cont
//*	功能说明: 连续写数据。从起始通道A至通道D连续写DAC输入寄存器和EEPROM
//*	形    参: NONE
//*	返 回 值: 0失败 1成功
//**********************************************************************************************************/
//static unsigned char MCP4728_Write_Cont(void)
//{
//    unsigned char i = 0;
//    unsigned char cWrByte2 = 0;
//    unsigned char cWrByte3 = 0;
//    unsigned char cWrByte4 = 0;
//
//    if (!MCP4728_RDY_READ())//处于忙碌状态
//        return 0;
//
////    s_tMCP4728.DAC[0] = 1024;
////    s_tMCP4728.DAC[1] = 1024;
////    s_tMCP4728.DAC[2] = 2048;
////    s_tMCP4728.DAC[3] = 3072;
//      
//	DRV_I2C_Start(s_tMCP4728.I2C);
//	
//	if (DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, MCP4728_ADDR | CMD_MODE_WRITE))
//	{
//        cWrByte2 =  (MCP4728_CMD_WRCONT << 3) | 0;//从起始通道A至通道D连续写DAC输入寄存器和EEPROM
//        DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, cWrByte2);
//
//        for (i = 0; i < 4; i++)
//        {
//            cWrByte3 = (s_tMCP4728.Config.Vref << 7) | (s_tMCP4728.Config.Gain << 4) | (s_tMCP4728.DAC[i] >> 8);
//            cWrByte4 = s_tMCP4728.DAC[i] & 0x0F;
//
//            DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, cWrByte3);
//            DRV_I2C_WriteByteWaiteAck(s_tMCP4728.I2C, cWrByte4);
//        }
//    }
//    else return 0;
//
//    DRV_I2C_Stop(s_tMCP4728.I2C);
//	
//	return 1;
//}
//
///**********************************************************************************************************
//*	函 数 名: MCP4728_Init
//*	功能说明: DAC芯片 I2C接口、MCP4728芯片配置
//*	形    参: NONE
//*	返 回 值: NONE
//**********************************************************************************************************/
//static void MCP4728_Init(void)
//{
//    s_tMCP4728.I2C.SCL.ePinName = epin_dac_scl;
//    s_tMCP4728.I2C.SDA.ePinName = epin_dac_sda;
//
//    s_tMCP4728.Config.Vref  = 1;//电压基准选择 0 = VDD, 1 = 2.048
//    s_tMCP4728.Config.Gain  = 0;//增益选择 0 = 1， 1 = 2
//    s_tMCP4728.Config.ChSel = 0x0F;//4个通道的选
//
////    MCP4728_LDAC_SETL();
//
//    MCP4728_Set_Vref();
//    MCP4728_Set_ON();
//    MCP4728_Set_Gain();
//}
//
///**********************************************************************************************************
//*	函 数 名: MCP4728_Output
//*	功能说明: DAC模拟量输出
//*	形    参: NONE
//*	返 回 值: NONE
//**********************************************************************************************************/
//static void MCP4728_Output(void)
//{
//    float temp=0;
//    float Vt=0.4, Vs=0.4, Vps=0.4, Vpt=0.4;//温度、流速、静压、全压 DAC输出电压
//    static float Vt_Old=0.4, Vs_Old=0.4, Vps_Old=0.4, Vpt_Old=0.4;//温度、流速、静压、全压 DAC输出电压
//
//    /* 根据电路DAC输出1V对应10mA输出可知，要输出4-20mA，DAC输出范围为0.4-2V */
//    if (g_SysData.Data.Sample.SysSta == SYS_STA_MEASU)
//    {
//        /* 温度 0-400℃*/
//        temp = (g_SysData.Data.Sample.PTTem / 400.0 * 16 + 4) / 10.0;
//        if (temp <= 0.4)    Vt_Old = Vt = 0.4;
//        else if (temp >= 2) Vt_Old = Vt = 2;
//        else Vt_Old = Vt = temp;
//
//        /* 压力（静压）-3922Pa - +3922Pa*/
//        temp = ((g_SysData.Data.Sample.SticP + 3922) / 7844.0 * 16 + 4) / 10.0;
//        if (temp <= 0.4)    Vps_Old = Vps = 0.4;
//        else if (temp >= 2) Vps_Old = Vps = 2;
//        else Vps_Old = Vps = temp;
//
//        /* 差压（全压） -1961Pa - +1961Pa*/
//        temp = ((g_SysData.Data.Sample.TotalP + 1961) / 3922.0 * 16 + 4) / 10.0;
//        if (temp <= 0.4)    Vpt_Old = Vpt = 0.4;
//        else if (temp >= 2) Vpt_Old = Vpt = 2;
//        else Vpt_Old = Vpt = temp;
//
//        /* 流速 0-40m/s*/
//        temp = (g_SysData.Data.Sample.Speed / 40.0 * 16 + 4) / 10.0;
//        if (temp <= 0.4)    Vs_Old = Vs = 0.4;
//        else if (temp >= 2) Vs_Old = Vs = 2;
//        else Vs_Old = Vs = temp;
//
//        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[0] = (unsigned short)((Vt * 4096) / (2.048 * 1));//1倍增益，内部基准
//        else s_tMCP4728.DAC[0] = (unsigned short)((Vt * 4096) / (2.048 * 2));//2倍增益，内部基准
//        
//        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[1] = (unsigned short)((Vps * 4096) / (2.048 * 1));//1倍增益，内部基准
//        else s_tMCP4728.DAC[1] = (unsigned short)((Vps * 4096) / (2.048 * 2));//2倍增益，内部基准
//
//        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[2] = (unsigned short)((Vpt * 4096) / (2.048 * 1));//1倍增益，内部基准
//        else s_tMCP4728.DAC[2] = (unsigned short)((Vpt * 4096) / (2.048 * 2));//2倍增益，内部基准
//
//        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[3] = (unsigned short)((Vs * 4096) / (2.048 * 1));//1倍增益，内部基准
//        else s_tMCP4728.DAC[3] = (unsigned short)((Vs * 4096) / (2.048 * 2));//2倍增益，内部基准
//    }
//    else if (g_SysData.Data.Sample.SysSta == SYS_STA_BLOW)
//    {
//        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[0] = (unsigned short)((Vt_Old * 4096) / (2.048 * 1));//1倍增益，内部基准
//        else s_tMCP4728.DAC[0] = (unsigned short)((Vt_Old * 4096) / (2.048 * 2));//2倍增益，内部基准
//        
//        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[1] = (unsigned short)((Vps_Old * 4096) / (2.048 * 1));//1倍增益，内部基准
//        else s_tMCP4728.DAC[1] = (unsigned short)((Vps_Old * 4096) / (2.048 * 2));//2倍增益，内部基准
//
//        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[2] = (unsigned short)((Vpt_Old * 4096) / (2.048 * 1));//1倍增益，内部基准
//        else s_tMCP4728.DAC[2] = (unsigned short)((Vpt_Old * 4096) / (2.048 * 2));//2倍增益，内部基准
//
//        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[3] = (unsigned short)((Vs_Old * 4096) / (2.048 * 1));//1倍增益，内部基准
//        else s_tMCP4728.DAC[3] = (unsigned short)((Vs_Old * 4096) / (2.048 * 2));//2倍增益，内部基准
//    }
//    
//    MCP4728_Write_Cont();
//}
//
///**********************************************************************************************************
//*	函 数 名: APP_DA
//*	功能说明: APP_DA function
//*	形    参: argument   数据指针
//*	返 回 值: NONE
//**********************************************************************************************************/
//void APP_DA(void  * argument)
//{
//    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
//    
//    MCP4728_Init();
//
//	while(1)
//	{
//        MCP4728_Output();
//
//        LOG_PRINT(DEBUG_TASK,"APP_DA \r\n");
//        
//        vTaskDelay(sMaxBlockTime);
//	}
//}



//设置IO口方向
#define SDA_IN()	{GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=0<<16;}	//PB4输入模式
#define SDA_OUT()	{GPIOB->CRL&=0x0FFFFFFF;GPIOB->CRL|=1<<16;}	//PB4输出模式

//设置IO口置高或置低
#define LDAC_H()	LL_GPIO_SetOutputPin  (GPIOB, LL_GPIO_PIN_3)
#define LDAC_L()	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_3)
#define SCL_H()		LL_GPIO_SetOutputPin  (GPIOB, LL_GPIO_PIN_5)
#define SCL_L()		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5)
#define SDA_H()		LL_GPIO_SetOutputPin  (GPIOB, LL_GPIO_PIN_4)
#define SDA_L()		LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4)

//读取IO口状态
#define READ_SDA()	LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_4)

//DAC芯片地址、指令、通道选择
#define DAC_ADDR	0xc0

#define DAC_READ	0x01
#define DAC_WRITE	0x00

void IIC_Delay(short time)
{
	short i = 0;
	while (time--)
	{
		i = 5;
		while(i--);
	}
}

void IIC_Start(void)	//开始信号
{
	SDA_OUT();
	SCL_H();
	SDA_H();
	IIC_Delay(4);
	SDA_L();
	IIC_Delay(4);
	SCL_L();
}

void IIC_Stop(void)	//停止信号
{
	SDA_OUT();
	SCL_L();
	SDA_L();
	IIC_Delay(4);
	SCL_H();
	SDA_H();
	IIC_Delay(4);
}

void IIC_Ack(void)	//产生应答
{
	SCL_L();
	SDA_OUT();
	SDA_L();
	IIC_Delay(2);
	SCL_H();
	IIC_Delay(2);
	SCL_L();
}

void IIC_NAck(void)	//不产生应答
{
	SCL_L();
	
	SDA_OUT();
	SDA_H();
	IIC_Delay(2);
	
	SCL_H();
	IIC_Delay(2);
	SCL_L();
}

void IIC_SendByte(unsigned char data)	//发送一个字节
{
	unsigned char i = 0;
	SDA_OUT();
	SCL_L();
	
	for(i = 0;i < 8;i++)
	{
		if((data & 0x80) >> 7)
			SDA_H();
		else
			SDA_L();
		data <<= 1;

		IIC_Delay(2);
		SCL_H();
		IIC_Delay(2);
		SCL_L();
		IIC_Delay(2);
    }
    IIC_Ack();
}

unsigned char IIC_ReadByte(unsigned char ack)	//读一个字节
{
	unsigned char i = 0,receive = 0;
	SDA_IN();
	for(i = 0;i < 8;i++ )
	{
		SCL_L();
		IIC_Delay(2);
		SCL_H();
		receive <<= 1;
		if(READ_SDA())
			receive++;
		IIC_Delay(2);
	}
	if (!ack)
		IIC_NAck();
	else
		IIC_Ack();
	return receive;
}


void Vref_Select(void)
{
	IIC_Start();
	IIC_SendByte(DAC_ADDR | DAC_WRITE);
	IIC_SendByte(0x8F);		//选择内部参考电压
	IIC_Stop();
}

void PowerDown_Select(void)
{
	IIC_Start();
	IIC_SendByte(DAC_ADDR | DAC_WRITE);
	IIC_SendByte(0xa0);		//正常工作模式
	IIC_SendByte(0x00);
	IIC_Stop();
}

void Gain_Select(void)		//选择外部参考电压时忽略此设置项，增益为x1
{
	IIC_Start();
	IIC_SendByte(DAC_ADDR | DAC_WRITE);
	IIC_SendByte(0xc0);		//选择增益为x1
	IIC_Stop();
}

void Fast_Write(uint16_t A_input,uint16_t B_input,uint16_t C_input,uint16_t D_input)		//快速写
{
//	IIC_Start();
//	IIC_SendByte(DAC_ADDR | DAC_WRITE);
//	
//	IIC_SendByte(0x00 | (value2module(A_input) >> 8));
//	IIC_SendByte(value2module(A_input) & 0x00ff);
//	
//	IIC_SendByte(0x00 | (value2module(B_input) >> 8));
//	IIC_SendByte(value2module(B_input) & 0x00ff);
//	
//	IIC_SendByte(0x00 | (value2module(C_input) >> 8));
//	IIC_SendByte(value2module(C_input) & 0x00ff);
//	
//	IIC_SendByte(0x00 | (value2module(D_input) >> 8));
//	IIC_SendByte(value2module(D_input) & 0x00ff);
//	
//	IIC_Stop();
//	
//	LDAC_L();
}

void Sequential_Write(uint16_t A_input,uint16_t B_input,uint16_t C_input,uint16_t D_input)		//连续写
{
	IIC_Start();
	IIC_SendByte(DAC_ADDR | DAC_WRITE);
	IIC_SendByte(0x50);
	
	IIC_SendByte(0x80 | (A_input >> 8));
	IIC_SendByte(A_input & 0x00ff);
	
	IIC_SendByte(0x80 | (B_input >> 8));
	IIC_SendByte(B_input & 0x00ff);
	
	IIC_SendByte(0x80 | (C_input >> 8));
	IIC_SendByte(C_input & 0x00ff);
	
	IIC_SendByte(0x80 | (D_input >> 8));
	IIC_SendByte(D_input & 0x00ff);
	
	IIC_Stop();
	
	LDAC_L();
}

void DAC_Init(void)
{
	Vref_Select();
	PowerDown_Select();
	Gain_Select();
}

/**********************************************************************************************************
*	函 数 名: MCP4728_Output
*	功能说明: DAC模拟量输出 Vout = (Vref*DAC)/4096 * GAIN
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
static void MCP4728_Output(void)
{
    float temp=0;
    float Vt=0.4, Vs=0.4, Vps=0.4, Vpt=0.4;//温度、流速、静压、全压 DAC输出电压
    static float Vt_Old=0.4, Vs_Old=0.4, Vps_Old=0.4, Vpt_Old=0.4;//温度、流速、静压、全压 DAC输出电压

    /* 根据电路DAC输出1V对应10mA输出可知，要输出4-20mA，DAC输出范围为0.4-2V */
    if (g_SysData.Data.Sample.SysSta == SYS_STA_MEASU)
    {
        /* 温度 0-400℃*/
        temp = (g_SysData.Data.Sample.PTTem / 400.0 * 16 + 4) / 10.0;
        if (temp <= 0.4)    Vt_Old = Vt = 0.4;
        else if (temp >= 2) Vt_Old = Vt = 2;
        else Vt_Old = Vt = temp;

        /* 压力（静压）-3922Pa - +3922Pa*/
        temp = ((g_SysData.Data.Sample.SticP + 3922) / 7844.0 * 16 + 4) / 10.0;
        if (temp <= 0.4)    Vps_Old = Vps = 0.4;
        else if (temp >= 2) Vps_Old = Vps = 2;
        else Vps_Old = Vps = temp;

        /* 差压（全压） -1961Pa - +1961Pa*/
        temp = ((g_SysData.Data.Sample.TotalP + 1961) / 3922.0 * 16 + 4) / 10.0;
        if (temp <= 0.4)    Vpt_Old = Vpt = 0.4;
        else if (temp >= 2) Vpt_Old = Vpt = 2;
        else Vpt_Old = Vpt = temp;

        /* 流速 0-40m/s*/
        temp = (g_SysData.Data.Sample.Speed / 40.0 * 16 + 4) / 10.0;
        if (temp <= 0.4)    Vs_Old = Vs = 0.4;
        else if (temp >= 2) Vs_Old = Vs = 2;
        else Vs_Old = Vs = temp;

        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[0] = (unsigned short)((Vt * 4096) / (2.048 * 1));//1倍增益，内部基准
        else s_tMCP4728.DAC[0] = (unsigned short)((Vt * 4096) / (2.048 * 2));//2倍增益，内部基准
        
        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[1] = (unsigned short)((Vps * 4096) / (2.048 * 1));//1倍增益，内部基准
        else s_tMCP4728.DAC[1] = (unsigned short)((Vps * 4096) / (2.048 * 2));//2倍增益，内部基准

        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[2] = (unsigned short)((Vpt * 4096) / (2.048 * 1));//1倍增益，内部基准
        else s_tMCP4728.DAC[2] = (unsigned short)((Vpt * 4096) / (2.048 * 2));//2倍增益，内部基准

        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[3] = (unsigned short)((Vs * 4096) / (2.048 * 1));//1倍增益，内部基准
        else s_tMCP4728.DAC[3] = (unsigned short)((Vs * 4096) / (2.048 * 2));//2倍增益，内部基准
    }
    else if (g_SysData.Data.Sample.SysSta == SYS_STA_BLOW)
    {
        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[0] = (unsigned short)((Vt_Old * 4096) / (2.048 * 1));//1倍增益，内部基准
        else s_tMCP4728.DAC[0] = (unsigned short)((Vt_Old * 4096) / (2.048 * 2));//2倍增益，内部基准
        
        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[1] = (unsigned short)((Vps_Old * 4096) / (2.048 * 1));//1倍增益，内部基准
        else s_tMCP4728.DAC[1] = (unsigned short)((Vps_Old * 4096) / (2.048 * 2));//2倍增益，内部基准

        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[2] = (unsigned short)((Vpt_Old * 4096) / (2.048 * 1));//1倍增益，内部基准
        else s_tMCP4728.DAC[2] = (unsigned short)((Vpt_Old * 4096) / (2.048 * 2));//2倍增益，内部基准

        if (!s_tMCP4728.Config.Gain) s_tMCP4728.DAC[3] = (unsigned short)((Vs_Old * 4096) / (2.048 * 1));//1倍增益，内部基准
        else s_tMCP4728.DAC[3] = (unsigned short)((Vs_Old * 4096) / (2.048 * 2));//2倍增益，内部基准
    }
    // 0  1  2  3分别对应 温度 静压 全压 流速
	s_tMCP4728.DAC[0] -= 10;
	s_tMCP4728.DAC[1] -= 10;
	s_tMCP4728.DAC[2] -= 10;
//	s_tMCP4728.DAC[3] -= 10;
    Sequential_Write(s_tMCP4728.DAC[0], s_tMCP4728.DAC[1], s_tMCP4728.DAC[2], s_tMCP4728.DAC[3]);
}

/**********************************************************************************************************
*	函 数 名: APP_DA
*	功能说明: APP_DA function
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void APP_DA(void  * argument)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
    
    DAC_Init();

	while(1)
	{
        MCP4728_Output();

        LOG_PRINT(DEBUG_TASK,"APP_DA \r\n");
        
        vTaskDelay(sMaxBlockTime);
	}

}
