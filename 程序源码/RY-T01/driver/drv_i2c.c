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
*	�� �� ��: SHT21_Start
*	����˵��: I2C��ʼ�ź�
*	��    ��: I2C_Interface
*	�� �� ֵ: NONE
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
	DRV_I2C_SCL_SETL  (I2C_Interface);//ǯסI2C���ߣ�׼�����ͻ�������� 
	SM9541_Delay_us(1);
}

/**********************************************************************************************************
*	�� �� ��: DRV_I2C_Stop
*	����˵��: I2Cֹͣ�ź�
*	��    ��: I2C_Interface 
*	�� �� ֵ: NONE
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
*	�� �� ��: DRV_I2C_Wait_Ack
*	����˵��: �ȴ�Ӧ���źŵ���
*	��    ��: NONE
*	�� �� ֵ: 0������Ӧ��ʧ�� 1������Ӧ��ɹ�
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
*	�� �� ��: DRV_I2C_Send_Ack
*	����˵��: ����ACKӦ��
*	��    ��: NONE
*	�� �� ֵ: NONE
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
*	�� �� ��: DRV_I2C_Send_NoAck
*	����˵��: ������ACKӦ��		
*	��    ��: NONE
*	�� �� ֵ: NONE
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
*	�� �� ��: DRV_I2C_WriteByteWaiteAck
*	����˵��: ����1byte�����ȴ�ACK
*	��    ��: _cByte �����͵��ֽ�����
*	�� �� ֵ:  0������Ӧ��ʧ�� 1������Ӧ��ɹ�
**********************************************************************************************************/
uint8_t DRV_I2C_WriteByteWaiteAck(DRV_I2C_T  I2C_Interface,uint8_t _cByte)
{
	uint8_t i = 0;
	
	//DRV_I2C_SDA_SETOUT(I2C_Interface);//���������ﲻ�������
	
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
*	�� �� ��: DRV_I2C_ReadByteWithAck
*	����˵��: ����1byte��������Ӧ��
*	��    ��: NONE
*	�� �� ֵ: ���յ����ֽ�
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
*	�� �� ��: DRV_I2C_ReadByteWithNoAck
*	����˵��: ����1byte��������Ӧ��
*	��    ��: NONE
*	�� �� ֵ: ���յ����ֽ�
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

