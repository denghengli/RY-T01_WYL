#ifndef _BSP_MCP7428_
#define _BSP_MCP7428_

typedef struct 
{
    DRV_I2C_T  I2C;
    struct RegConfig
    {
        unsigned char  Vref; //��ѹ��׼ѡ�� 0 = VDD, 1 = 2.048
        unsigned char  Gain; //����ѡ�� 1 = 1�� 2 = 2
        unsigned char  ChSel;//ͨ��ѡ�� bit0:ͨ��A��bit1:ͨ��B��bit2:ͨ��C��bit3:ͨ��D
    }Config; 
    
    unsigned short DACValue[4]; //4ͨ��DAC�������

}MCP4728_T;

extern void MCP7428_Init(void);
extern void MCP4728_Output(float A_V1, float B_V2, float C_V3, float D_V4);

#endif

