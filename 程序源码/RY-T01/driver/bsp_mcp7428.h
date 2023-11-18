#ifndef _BSP_MCP7428_
#define _BSP_MCP7428_

typedef struct 
{
    DRV_I2C_T  I2C;
    struct RegConfig
    {
        unsigned char  Vref; //电压基准选择 0 = VDD, 1 = 2.048
        unsigned char  Gain; //增益选择 1 = 1， 2 = 2
        unsigned char  ChSel;//通道选择 bit0:通道A，bit1:通道B，bit2:通道C，bit3:通道D
    }Config; 
    
    unsigned short DACValue[4]; //4通道DAC输入代码

}MCP4728_T;

extern void MCP7428_Init(void);
extern void MCP4728_Output(float A_V1, float B_V2, float C_V3, float D_V4);

#endif

