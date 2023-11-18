#include "includes.h"
#include "drv_soft_i2c.h"

MCP4728_T s_tMCP4728; 

//DAC芯片地址、指令、通道选择
#define DAC_ADDR	0xC0
#define DAC_READ	0x01
#define DAC_WRITE	0x00

//电压基准选择 0 = VDD, 1 = 2.048
void Vref_Select(uint8_t vref) 
{
	DRV_SoftI2C_Start(s_tMCP4728.I2C);
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, DAC_ADDR | DAC_WRITE);
    if(vref == 0) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x80); //选择参考电压 0x8F=内部参考电压2.048, 0x80=VDD=5V 
	if(vref == 1) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x8F); //选择参考电压 0x8F=内部参考电压2.048, 0x80=VDD=5V 
	DRV_SoftI2C_Stop(s_tMCP4728.I2C);
}

void PowerDown_Select(void)
{
	DRV_SoftI2C_Start(s_tMCP4728.I2C);
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, DAC_ADDR | DAC_WRITE);
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0xA0);//正常工作模式
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x00);
	DRV_SoftI2C_Stop(s_tMCP4728.I2C);
}

//选择外部参考电压 1 = x1， 2 = x2
void Gain_Select(uint8_t gain)		
{
	DRV_SoftI2C_Start(s_tMCP4728.I2C);
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, DAC_ADDR | DAC_WRITE);
    if(gain == 1) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0xC0); //选择增益 0XC0=1 0XCF=2
	if(gain == 2) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0xCF); //选择增益 0XC0=1 0XCF=2
	DRV_SoftI2C_Stop(s_tMCP4728.I2C);
}

void Sequential_Write(uint16_t A_input,uint16_t B_input,uint16_t C_input,uint16_t D_input)		//连续写
{
	DRV_SoftI2C_Start(s_tMCP4728.I2C);
	
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, DAC_ADDR | DAC_WRITE);
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x50);
	
    //0x80=内部参考1倍增益, 0x90=内部参考2倍增益, 0x00=外部参考
    if(s_tMCP4728.Config.Vref == 0) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x00 | (A_input >> 8));
    else if(s_tMCP4728.Config.Vref == 1)
    {
        if(s_tMCP4728.Config.Gain == 1) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x80 | (A_input >> 8));
        else if(s_tMCP4728.Config.Gain == 2) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x90 | (A_input >> 8));
    }
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, A_input & 0x00ff);
	
    //0x80=内部参考1倍增益, 0x90=内部参考2倍增益, 0x00=外部参考
    if(s_tMCP4728.Config.Vref == 0) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x00 | (B_input >> 8));
    else if(s_tMCP4728.Config.Vref == 1)
    {
        if(s_tMCP4728.Config.Gain == 1) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x80 | (B_input >> 8));
        else if(s_tMCP4728.Config.Gain == 2) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x90 | (B_input >> 8));
    }
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, B_input & 0x00ff);
	
    //0x80=内部参考1倍增益, 0x90=内部参考2倍增益, 0x00=外部参考
    if(s_tMCP4728.Config.Vref == 0) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x00 | (C_input >> 8));
    else if(s_tMCP4728.Config.Vref == 1)
    {
        if(s_tMCP4728.Config.Gain == 1) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x80 | (C_input >> 8));
        else if(s_tMCP4728.Config.Gain == 2) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x90 | (C_input >> 8));
    }
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, C_input & 0x00ff);
	
    //0x80=内部参考1倍增益, 0x90=内部参考2倍增益, 0x00=外部参考
    if(s_tMCP4728.Config.Vref == 0) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x00 | (D_input >> 8));
    else if(s_tMCP4728.Config.Vref == 1)
    {
        if(s_tMCP4728.Config.Gain == 1) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x80 | (D_input >> 8));
        else if(s_tMCP4728.Config.Gain == 2) DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, 0x90 | (D_input >> 8));
    }
	DRV_SoftI2C_WriteByteWaiteAck(s_tMCP4728.I2C, D_input & 0x00ff);
	
	DRV_SoftI2C_Stop(s_tMCP4728.I2C);
}

void MCP7428_Init(void)
{
//    s_tMCP4728.I2C.SCL.ePinName = epin_I2C_SCL;
//    s_tMCP4728.I2C.SDA.ePinName = epin_I2C_SDA;
    DRV_SoftI2C_Init(s_tMCP4728.I2C);
    
    s_tMCP4728.Config.Vref = 1; //电压基准选择 0 = VDD, 1 = 2.048
    s_tMCP4728.Config.Gain = 2; //增益选择 1 = 1， 2 = 2, 外部参考电压时忽略此配置
    
	Vref_Select(s_tMCP4728.Config.Vref);
	PowerDown_Select();
	Gain_Select(s_tMCP4728.Config.Gain);
}

/**********************************************************************************************************
*	函 数 名: MCP4728_Output
*	功能说明: DAC模拟量输出 Vout = (VDD*DACValue)/4096, VDD=5
*	形     参: A_V1:0-5V VAO01, B_V2:0-5V VAO02, C_V3:0-5V VAO03, D_V4:0-5V VAO04
*	返 回 值: NONE
**********************************************************************************************************/
void MCP4728_Output(float A_V1, float B_V2, float C_V3, float D_V4)
{
    if(s_tMCP4728.Config.Vref == 0) s_tMCP4728.DACValue[0] = (uint16_t)(A_V1 * 4096.0 / 5.0); 
    if(s_tMCP4728.Config.Vref == 1) s_tMCP4728.DACValue[0] = (uint16_t)(A_V1 * 4096.0 / (2.048 * s_tMCP4728.Config.Gain));
    
    if(s_tMCP4728.Config.Vref == 0) s_tMCP4728.DACValue[1] = (uint16_t)(B_V2 * 4096.0 / 5.0); 
    if(s_tMCP4728.Config.Vref == 1) s_tMCP4728.DACValue[1] = (uint16_t)(B_V2 * 4096.0 / (2.048 * s_tMCP4728.Config.Gain)); 

    if(s_tMCP4728.Config.Vref == 0) s_tMCP4728.DACValue[2] = (uint16_t)(C_V3 * 4096.0 / 5.0); 
    if(s_tMCP4728.Config.Vref == 1) s_tMCP4728.DACValue[2] = (uint16_t)(C_V3 * 4096.0 / (2.048 * s_tMCP4728.Config.Gain));
    
    if(s_tMCP4728.Config.Vref == 0) s_tMCP4728.DACValue[3] = (uint16_t)(D_V4 * 4096.0 / 5.0); 
    if(s_tMCP4728.Config.Vref == 1) s_tMCP4728.DACValue[3] = (uint16_t)(D_V4 * 4096.0 / (2.048 * s_tMCP4728.Config.Gain));
    
    Sequential_Write(s_tMCP4728.DACValue[0], s_tMCP4728.DACValue[1], s_tMCP4728.DACValue[2], s_tMCP4728.DACValue[3]);
}

