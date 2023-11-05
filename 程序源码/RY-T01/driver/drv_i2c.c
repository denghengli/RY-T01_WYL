#include "includes.h"


#define DRV_I2C_SCL_SETH(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SCL.ePinName, PIN_HIGH)
#define DRV_I2C_SCL_SETL(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SCL.ePinName, PIN_LOW)
#define DRV_I2C_SDA_SETH(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SDA.ePinName, PIN_HIGH)
#define DRV_I2C_SDA_SETL(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SDA.ePinName, PIN_LOW)
#define DRV_I2C_SDA_READ(I2C_Interface)      DRV_Pin_Read (I2C_Interface.SDA.ePinName)


static void DRV_I2C_SCL_SETIN(DRV_I2C_T  I2C_Interface)
{
    DRV_Pin_Mode(I2C_Interface.SCL.ePinName, PINMODE_IN_UP);
}


static void DRV_I2C_SCL_SETOUT(DRV_I2C_T  I2C_Interface)
{
    DRV_Pin_Mode(I2C_Interface.SCL.ePinName, PINMODE_OUT_OD_UP);
}


static void DRV_I2C_SDA_SETIN(DRV_I2C_T  I2C_Interface)
{     
    DRV_Pin_Mode(I2C_Interface.SDA.ePinName, PINMODE_IN_UP);
}


static void DRV_I2C_SDA_SETOUT(DRV_I2C_T  I2C_Interface)
{
    DRV_Pin_Mode(I2C_Interface.SDA.ePinName, PINMODE_OUT_OD_UP);
}


void SM9541_Delay_us(unsigned int _us)
{     
	unsigned int  i, j;
	for (i=0; i<_us; i++)
	{
		for (j=0; j<15; j++);
	}
}	 

/**********************************************************************************************************
*	函 数 名: SHT21_Start
*	功能说明: I2C起始信号
*	形    参: I2C_Interface
*	返 回 值: NONE
**********************************************************************************************************/
void DRV_I2C_Start(DRV_I2C_T  I2C_Interface)
{

	DRV_I2C_SCL_SETOUT(I2C_Interface);
	DRV_I2C_SDA_SETOUT(I2C_Interface);
	
	DRV_I2C_SCL_SETH  (I2C_Interface);
	DRV_I2C_SDA_SETH  (I2C_Interface);
	SM9541_Delay_us(1);
	DRV_I2C_SDA_SETL  (I2C_Interface);
	SM9541_Delay_us(1);
	DRV_I2C_SCL_SETL  (I2C_Interface);//钳住I2C总线，准备发送或接收数据 
	SM9541_Delay_us(1);
}

/**********************************************************************************************************
*	函 数 名: DRV_I2C_Stop
*	功能说明: I2C停止信号
*	形    参: I2C_Interface 
*	返 回 值: NONE
**********************************************************************************************************/
void DRV_I2C_Stop(DRV_I2C_T  I2C_Interface)
{
	DRV_I2C_SDA_SETOUT(I2C_Interface);
	
	DRV_I2C_SCL_SETL  (I2C_Interface);
	DRV_I2C_SDA_SETL  (I2C_Interface);
	SM9541_Delay_us(1);
	DRV_I2C_SCL_SETH  (I2C_Interface);
	SM9541_Delay_us(1);
	DRV_I2C_SDA_SETH  (I2C_Interface);
	SM9541_Delay_us(1);
}

/*
*********************************************************************************************************
*	函 数 名: DRV_I2C_Wait_Ack
*	功能说明: 等待应答信号到来
*	形    参: NONE
*	返 回 值: 0，接收应答失败 1，接收应答成功
*********************************************************************************************************
*/
static uint8_t DRV_I2C_Wait_Ack(DRV_I2C_T  I2C_Interface)
{
	unsigned int ucErrTime = 0;
	
	SM9541_Delay_us(1);
	DRV_I2C_SDA_SETH(I2C_Interface);

	DRV_I2C_SDA_SETIN(I2C_Interface);

	while(DRV_I2C_SDA_READ(I2C_Interface))//READ_SDA
	{
		ucErrTime ++;
		if(ucErrTime > 1000)
		{
			DRV_I2C_Stop(I2C_Interface);
			return 0;
		}
	}
	
	DRV_I2C_SCL_SETH(I2C_Interface);
	SM9541_Delay_us(1);
	DRV_I2C_SCL_SETL(I2C_Interface);
	
	return 1;
}

/**********************************************************************************************************
*	函 数 名: DRV_I2C_Send_Ack
*	功能说明: 产生ACK应答
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
static void DRV_I2C_Send_Ack(DRV_I2C_T  I2C_Interface)
{
	DRV_I2C_SCL_SETL  (I2C_Interface);
	
	DRV_I2C_SDA_SETOUT(I2C_Interface);
	
	DRV_I2C_SDA_SETL  (I2C_Interface);
	SM9541_Delay_us(1);
	DRV_I2C_SCL_SETH  (I2C_Interface);
	SM9541_Delay_us(1);
	DRV_I2C_SCL_SETL  (I2C_Interface);
}

/**********************************************************************************************************
*	函 数 名: DRV_I2C_Send_NoAck
*	功能说明: 不产生ACK应答		
*	形    参: NONE
*	返 回 值: NONE
**********************************************************************************************************/
static void DRV_I2C_Send_NoAck(DRV_I2C_T  I2C_Interface)
{
	DRV_I2C_SCL_SETL(I2C_Interface);
	
	DRV_I2C_SDA_SETOUT(I2C_Interface);
	
	DRV_I2C_SDA_SETH  (I2C_Interface);
	SM9541_Delay_us(1);
	DRV_I2C_SCL_SETH  (I2C_Interface);
	SM9541_Delay_us(1);
	DRV_I2C_SCL_SETL  (I2C_Interface);
}
	
/**********************************************************************************************************
*	函 数 名: DRV_I2C_WriteByteWaiteAck
*	功能说明: 发送1byte，并等待ACK
*	形    参: _cByte 待发送的字节数据
*	返 回 值:  0，接收应答失败 1，接收应答成功
**********************************************************************************************************/
uint8_t DRV_I2C_WriteByteWaiteAck(DRV_I2C_T  I2C_Interface,uint8_t _cByte)
{
	uint8_t i = 0;
	
	//DRV_I2C_SDA_SETOUT(I2C_Interface);//经调试这里不能有这个
	
	for(i=0;i<8;i++)
	{
		if(_cByte & 0x80)
		{
			DRV_I2C_SDA_SETH(I2C_Interface);
		}
		else
		{
			DRV_I2C_SDA_SETL(I2C_Interface);
		}
		
		SM9541_Delay_us(1);
		DRV_I2C_SCL_SETH(I2C_Interface);
		SM9541_Delay_us(1);
		DRV_I2C_SCL_SETL(I2C_Interface);
		_cByte <<= 1;
	}
	
	return DRV_I2C_Wait_Ack(I2C_Interface);
}

/**********************************************************************************************************
*	函 数 名: DRV_I2C_ReadByteWithAck
*	功能说明: 接收1byte，并发送应答
*	形    参: NONE
*	返 回 值: 接收到的字节
**********************************************************************************************************/
uint8_t DRV_I2C_ReadByteWithAck(DRV_I2C_T  I2C_Interface)
{
	uint8_t i,receive=0;
	
	DRV_I2C_SDA_SETIN(I2C_Interface);
	SM9541_Delay_us(1);
	for(i=0;i<8;i++)
	{
		DRV_I2C_SCL_SETL(I2C_Interface);
		SM9541_Delay_us(1);
		DRV_I2C_SCL_SETH(I2C_Interface);
		SM9541_Delay_us(1);
		receive <<= 1;
		if(DRV_I2C_SDA_READ(I2C_Interface)) receive++;
		//DRV_I2C_SCL_SETL(I2C_Interface);
	}
	
	DRV_I2C_Send_Ack(I2C_Interface);
	SM9541_Delay_us(1);
	return receive;
}

/**********************************************************************************************************
*	函 数 名: DRV_I2C_ReadByteWithNoAck
*	功能说明: 接收1byte，不返回应答
*	形    参: NONE
*	返 回 值: 接收到的字节
**********************************************************************************************************/
uint8_t DRV_I2C_ReadByteWithNoAck(DRV_I2C_T  I2C_Interface)
{
	uint8_t i,receive=0;
	
	DRV_I2C_SDA_SETIN(I2C_Interface);
	
	for(i=0;i<8;i++)
	{
		DRV_I2C_SCL_SETL(I2C_Interface);
		SM9541_Delay_us(1);
		DRV_I2C_SCL_SETH(I2C_Interface);
		SM9541_Delay_us(1);
		receive <<= 1;
		if(DRV_I2C_SDA_READ(I2C_Interface)) receive++;
	//	DRV_I2C_SCL_SETL(I2C_Interface);
	}
	
	DRV_I2C_Send_NoAck(I2C_Interface);
	SM9541_Delay_us(1);
	return receive;
}

