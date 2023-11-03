#include "includes.h"

#define SCL_SETH(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SCL.ePinName, PIN_HIGH)
#define SCL_SETL(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SCL.ePinName, PIN_LOW)
#define SDA_SETH(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SDA.ePinName, PIN_HIGH)
#define SDA_SETL(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SDA.ePinName, PIN_LOW)
#define SDA_READ(I2C_Interface)      DRV_Pin_Read (I2C_Interface.SDA.ePinName)

//IO方向设置 PA1
static void SDA_IN(DRV_I2C_T I2C_Interface)
{
    DRV_Pin_Mode(I2C_Interface.SDA.ePinName, PINMODE_IN_UP);
}
static void SDA_OUT(DRV_I2C_T I2C_Interface)
{
    DRV_Pin_Mode(I2C_Interface.SDA.ePinName, PINMODE_OUT_OD_UP);
}

/**********************************************************************************************************
*	函 数 名: SoftI2C_Init
*	功能说明: I2C初始化,配置成推挽输出后可同时输入输出
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
void DRV_SoftI2C_Init(DRV_I2C_T  I2C_Interface)
{
	DRV_Pin_Mode(I2C_Interface.SDA.ePinName, PINMODE_OUT_OD_UP);
	DRV_Pin_Mode(I2C_Interface.SCL.ePinName, PINMODE_OUT_OD_UP);
}

void SoftI2C_Delay_us(unsigned int _us)
{     
	unsigned int  i, j;
	for (i=0; i<_us; i++)
	{
		for (j=0; j<15; j++);
	}
}	 

/**********************************************************************************************************
*	函 数 名: DRV_SoftI2C_Start
*	功能说明: I2C起始信号
*	形    参: I2C_Interface
*	返 回 值: NONE
**********************************************************************************************************/
void DRV_SoftI2C_Start(DRV_I2C_T  I2C_Interface)
{
    SDA_OUT(I2C_Interface);
    
	SCL_SETH  (I2C_Interface);
	SDA_SETH  (I2C_Interface);
	SoftI2C_Delay_us(4);
	SDA_SETL  (I2C_Interface);
	SoftI2C_Delay_us(4);
	SCL_SETL  (I2C_Interface);//钳住I2C总线，准备发送或接收数据 
}

/**********************************************************************************************************
*	函 数 名: DRV_SoftI2C_Stop
*	功能说明: I2C停止信号
*	形    参: I2C_Interface 
*	返 回 值: NONE
**********************************************************************************************************/
void DRV_SoftI2C_Stop(DRV_I2C_T  I2C_Interface)
{
    SDA_OUT(I2C_Interface);
    
	SCL_SETH  (I2C_Interface);
	SDA_SETL  (I2C_Interface);
	SoftI2C_Delay_us(4);
	SDA_SETH  (I2C_Interface);
	SoftI2C_Delay_us(4);
}

/*
*********************************************************************************************************
*	函 数 名: DRV_SoftI2C_Wait_Ack
*	功能说明: 等待应答信号到来
*	形    参: NONE
*	返 回 值: 0，接收应答失败 1，接收应答成功
*********************************************************************************************************
*/
static uint8_t DRV_SoftI2C_Wait_Ack(DRV_I2C_T  I2C_Interface)
{
	unsigned int ucErrTime = 0;
	
    SDA_IN(I2C_Interface);

	SCL_SETH(I2C_Interface);
	SoftI2C_Delay_us(2);
	
	while(SDA_READ(I2C_Interface))//READ_SDA
	{
		ucErrTime ++;
		if(ucErrTime > 1000)
		{
			DRV_SoftI2C_Stop(I2C_Interface);
			return 0;
		}
	}
	
	SoftI2C_Delay_us(2);
	SCL_SETL(I2C_Interface);
	
	return 1;
}

/**********************************************************************************************************
*	函 数 名: DRV_SoftI2C_Send_Ack
*	功能说明: 产生ACK应答
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
static void DRV_SoftI2C_Send_Ack(DRV_I2C_T  I2C_Interface)
{
    SDA_OUT(I2C_Interface);
    
	SDA_SETL  (I2C_Interface);
	SoftI2C_Delay_us(2);
	SCL_SETH  (I2C_Interface);
	SoftI2C_Delay_us(2);
	SCL_SETL  (I2C_Interface);
	SoftI2C_Delay_us(2);
	
	SDA_SETH  (I2C_Interface);
}

/**********************************************************************************************************
*	函 数 名: DRV_SoftI2C_Send_NoAck
*	功能说明: 不产生ACK应答		
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
static void DRV_SoftI2C_Send_NoAck(DRV_I2C_T  I2C_Interface)
{
    SDA_OUT(I2C_Interface);
    
	SDA_SETH  (I2C_Interface);
	SoftI2C_Delay_us(2);
	SCL_SETH  (I2C_Interface);
	SoftI2C_Delay_us(2);
	SCL_SETL  (I2C_Interface);
	SoftI2C_Delay_us(2);
}
	
/**********************************************************************************************************
*	函 数 名: DRV_SoftI2C_WriteByteWaiteAck
*	功能说明: 发送1byte，并等待ACK
*	形    参: _cByte 待发送的字节数据
*	返 回 值:  0，接收应答失败 1，接收应答成功
**********************************************************************************************************/
uint8_t DRV_SoftI2C_WriteByteWaiteAck(DRV_I2C_T  I2C_Interface,uint8_t _cByte)
{
	uint8_t i = 0;
    
	SDA_OUT(I2C_Interface);
    
	for(i=0;i<8;i++)
	{
		if(_cByte & 0x80)
		{
			SDA_SETH(I2C_Interface);
		}
		else
		{
			SDA_SETL(I2C_Interface);
		}
		
		SoftI2C_Delay_us(2);
		SCL_SETH(I2C_Interface);
		SoftI2C_Delay_us(2);
		SCL_SETL(I2C_Interface);
		_cByte <<= 1;
	}
	
	return DRV_SoftI2C_Wait_Ack(I2C_Interface);
}

/**********************************************************************************************************
*	函 数 名: DRV_SoftI2C_ReadByteWithAck
*	功能说明: 接收1byte，并发送应答
*	形    参: NONE
*	返 回 值: 接收到的字节
**********************************************************************************************************/
uint8_t DRV_SoftI2C_ReadByteWithAck(DRV_I2C_T  I2C_Interface)
{
	uint8_t i,receive=0;
	
    SDA_IN(I2C_Interface);
    
	for(i=0;i<8;i++)
	{
		SCL_SETH(I2C_Interface);
		SoftI2C_Delay_us(2);
		receive <<= 1;
		if(SDA_READ(I2C_Interface)) receive++;
		SCL_SETL(I2C_Interface);
		SoftI2C_Delay_us(2);
	}
	
	DRV_SoftI2C_Send_Ack(I2C_Interface);
	
	return receive;
}

/**********************************************************************************************************
*	函 数 名: DRV_SoftI2C_ReadByteWithNoAck
*	功能说明: 接收1byte，不返回应答
*	形    参: NONE
*	返 回 值: 接收到的字节
**********************************************************************************************************/
uint8_t DRV_SoftI2C_ReadByteWithNoAck(DRV_I2C_T  I2C_Interface)
{
	uint8_t i,receive=0;
	
    SDA_IN(I2C_Interface);
    
	for(i=0;i<8;i++)
	{
		SCL_SETH(I2C_Interface);
		SoftI2C_Delay_us(2);
		receive <<= 1;
		if(SDA_READ(I2C_Interface)) receive++;
		SCL_SETL(I2C_Interface);
		SoftI2C_Delay_us(2);
	}
	
	DRV_SoftI2C_Send_NoAck(I2C_Interface);
	
	return receive;
}






