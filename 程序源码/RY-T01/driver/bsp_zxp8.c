#include "includes.h"

#define _I2C_SCL_SETH(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SCL.ePinName, PIN_HIGH)
#define _I2C_SCL_SETL(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SCL.ePinName, PIN_LOW)
#define _I2C_SDA_SETH(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SDA.ePinName, PIN_HIGH)
#define _I2C_SDA_SETL(I2C_Interface)      DRV_Pin_Write(I2C_Interface.SDA.ePinName, PIN_LOW)
#define _I2C_SDA_READ(I2C_Interface)      DRV_Pin_Read (I2C_Interface.SDA.ePinName)

static void _I2C_SCL_SETIN(_I2C_T  I2C_Interface)
{
    DRV_Pin_Mode(I2C_Interface.SCL.ePinName, PINMODE_IN_UP);
}

static void _I2C_SCL_SETOUT(_I2C_T  I2C_Interface)
{
    DRV_Pin_Mode(I2C_Interface.SCL.ePinName, PINMODE_OUT_OD_UP);
}


static void _I2C_SDA_SETIN(_I2C_T  I2C_Interface)
{     
    DRV_Pin_Mode(I2C_Interface.SDA.ePinName, PINMODE_IN_UP);
}


static void _I2C_SDA_SETOUT(_I2C_T  I2C_Interface)
{
    DRV_Pin_Mode(I2C_Interface.SDA.ePinName, PINMODE_OUT_OD_UP);
}


void _I2C_Delay_us(unsigned int _us)
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
void _I2C_Start(_I2C_T  I2C_Interface)
{
	_I2C_SCL_SETOUT(I2C_Interface);
	_I2C_SDA_SETOUT(I2C_Interface);
	
	_I2C_SCL_SETH(I2C_Interface);
	_I2C_SDA_SETH(I2C_Interface);
	_I2C_Delay_us(2);
	_I2C_SDA_SETL(I2C_Interface);
	_I2C_Delay_us(2);
	_I2C_SCL_SETL(I2C_Interface);//钳住I2C总线，准备发送或接收数据 
	_I2C_Delay_us(2);
}

/**********************************************************************************************************
*	函 数 名: _I2C_Stop
*	功能说明: I2C停止信号
*	形    参: I2C_Interface 
*	返 回 值: NONE
**********************************************************************************************************/
void _I2C_Stop(_I2C_T  I2C_Interface)
{
	_I2C_SDA_SETOUT(I2C_Interface);
	
	_I2C_SCL_SETL(I2C_Interface);
	_I2C_SDA_SETL(I2C_Interface);
	_I2C_Delay_us(2);
	_I2C_SCL_SETH(I2C_Interface);
	_I2C_Delay_us(2);
	_I2C_SDA_SETH(I2C_Interface);
	_I2C_Delay_us(2);
}


/**********************************************************************************************************
*	函 数 名: _I2C_WriteByte
*	功能说明: 发送1byte
*	形    参: _cByte 待发送的字节数据
*	返 回 值:  0，接收应答失败 1，接收应答成功
**********************************************************************************************************/
uint8_t _I2C_WriteByte(_I2C_T  I2C_Interface,uint8_t _cByte)
{
	uint8_t i = 0;
	
	for(i=0;i<8;i++)
	{
		if(_cByte & 0x80)
		{
			_I2C_SDA_SETH(I2C_Interface);
		}
		else
		{
			_I2C_SDA_SETL(I2C_Interface);
		}
		
		_I2C_Delay_us(2);
		_I2C_SCL_SETH(I2C_Interface);
		_I2C_Delay_us(2);
		_I2C_SCL_SETL(I2C_Interface);
		_cByte <<= 1;
	}

	_I2C_SDA_SETIN(I2C_Interface);
	
    _I2C_SCL_SETH(I2C_Interface);
    _I2C_Delay_us(2);
    _I2C_SCL_SETL(I2C_Interface);
    _I2C_Delay_us(2);

    _I2C_SDA_SETOUT(I2C_Interface);
    
	return 1;
}

/**********************************************************************************************************
*	函 数 名: _I2C_ReadByte
*	功能说明: 接收1byte
*	形     参: NONE
*	返 回 值: 接收到的字节
**********************************************************************************************************/
uint8_t _I2C_ReadByte(_I2C_T  I2C_Interface, uint8_t ack)
{
	uint8_t i,receive=0;
	
	_I2C_SDA_SETIN(I2C_Interface);
	_I2C_Delay_us(2);
	for(i=0;i<8;i++)
	{
		_I2C_SCL_SETH(I2C_Interface);
		_I2C_Delay_us(2);
        receive <<= 1;
		if(_I2C_SDA_READ(I2C_Interface))
            receive |= 1;
        _I2C_SCL_SETL(I2C_Interface);    
		_I2C_Delay_us(2);
	}

	_I2C_SDA_SETOUT(I2C_Interface);
    if(ack == 1)
    {
        _I2C_SDA_SETL(I2C_Interface); 
    }
    else
    {
        _I2C_SDA_SETH(I2C_Interface); 
    }
    _I2C_Delay_us(2);
    _I2C_SCL_SETH(I2C_Interface);
    _I2C_Delay_us(2);
    _I2C_SCL_SETL(I2C_Interface);
	_I2C_Delay_us(2);

	return receive;
}

static uint8_t _I2C_WriteBytes(_I2C_T  I2C_Interface, uint8_t IIC_Address, uint8_t *buffer, uint16_t count)
{
	uint8_t i;
	uint8_t temp;

	temp = IIC_Address;

	_I2C_Start(I2C_Interface);
	_I2C_WriteByte(I2C_Interface, temp);

	for(i=0; i<count; i++)
	{
		_I2C_WriteByte(I2C_Interface, buffer[i]);
	}

	_I2C_Stop(I2C_Interface);
	
	return 0;
}

static uint8_t _I2C_ReadBytes(_I2C_T  I2C_Interface, uint8_t IIC_Address,uint8_t *buffer,uint16_t count)
{
	uint8_t i;
	uint8_t tag;
	uint8_t temp;

	temp = IIC_Address;
	_I2C_Start(I2C_Interface);
	_I2C_WriteByte(I2C_Interface, temp);
	_I2C_WriteByte(I2C_Interface, buffer[0]);
	
	_I2C_Start(I2C_Interface);
	temp = IIC_Address;
	temp |= 0x01;
	_I2C_WriteByte(I2C_Interface, temp);
	for(i=0; i<count; i++)
	{
		tag = (i==count-1) ? 0 : 1;
		buffer[i] = _I2C_ReadByte(I2C_Interface, tag);
	}

	_I2C_Stop(I2C_Interface);
	
	return 0;
}



#define ZXP3010D_Address                0xDA
#define ZXP3010D_ADC_LSB				0x08
#define ZXP3010D_ADC_LCB				0x07
#define ZXP3010D_ADC_MSB				0x06
#define ZXP3010D_TEMP_LSB				0x0A
#define ZXP3010D_TEMP_MSB				0x09
#define ZXP3010D_STATUS					0x02
#define ZXP3010D_CMD					0x30
#define ZXP3010D_DRDY					0x01
#define ZXP3010D_COMM_ERR_MAXCNT        20
static void DelayMs(uint16_t ms)
{
    uint32_t i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 7000; j++)
        {
        }
    }
}

//开启压力转换
void ZXP3010D_StartP(_I2C_T  I2C_Interface)
{
    uint8_t buf[4];

    buf[0] = 0xA5;
    //buf[1] = 0x13;输出原始ADC值
    buf[1] = 0x11; //输出校准数据
    _I2C_WriteBytes(I2C_Interface, ZXP3010D_Address, buf, 2);

    buf[0] = 0x30;
    buf[1] = 0x09;
    _I2C_WriteBytes(I2C_Interface, ZXP3010D_Address, buf, 2);
}

//开启温度转换
void ZXP3010D_StartT(_I2C_T  I2C_Interface)
{
	uint8_t buf[4];
	
	buf[0] = 0xA5;
	buf[1] = 0x01;
	_I2C_WriteBytes(I2C_Interface, ZXP3010D_Address, buf, 2);

	buf[0] = 0x30;
	buf[1] = 0x08;
	_I2C_WriteBytes(I2C_Interface, ZXP3010D_Address, buf, 2);
}

//获取压力结果
int32_t ZXP3010D_ResultP(_I2C_T  I2C_Interface)
{
	int32_t ltemp;

	uint8_t buf[4];
	
	buf[0] = 0x06;

	_I2C_ReadBytes(I2C_Interface, ZXP3010D_Address, buf, 3);

	ltemp = buf[0] << 8;
	ltemp |= buf[1];
	ltemp <<= 8;
	ltemp |= buf[2];
        
	return(ltemp);
}

//获取温度结果
int32_t ZXP3010D_ResultT(_I2C_T  I2C_Interface)
{
	int32_t ltemp;

	uint8_t buf[4];
	
	buf[0] = 0x09;

	_I2C_ReadBytes(I2C_Interface, ZXP3010D_Address, buf, 2);

	ltemp = buf[0] << 8;
	ltemp |= buf[1];

	return(ltemp);
}

//获取状态
uint8_t ZXP3010D_ConStatus(_I2C_T  I2C_Interface)
{
	uint8_t status;

	uint8_t buf[4];

	buf[0] = ZXP3010D_CMD;
	_I2C_ReadBytes(I2C_Interface, ZXP3010D_Address, buf, 1);
	status= (buf[0]>>3)&0x01;
    
	return status;
}

//温度、压力正负值转换
void ZXP3010D_Caculate(uint8_t rang, int32_t up, int32_t ut, float *rp, float *rt)
{
    float fp, ft, ftemp;

    //温度值正负判断       
    ft = ut ;
    if(ft >= pow(2.0, 15.0))
        ft = ft - pow(2.0, 16.0);

    //压力值正负判断   
    ftemp = up;
    if(ftemp >= pow(2.0, 23.0))
        ftemp = ftemp - pow(2.0, 24.0);
    //n=13  1Kpa
    //n=14  500pa
    //12    2kpa  
    //11    4kpa
    //9     10kpa 
    //6     100kpa
    if (rang == ZXP8_RANG_2KP)
        ftemp= ftemp / pow(2.0, 12.0);           
    else if (rang == ZXP8_RANG_10KP)
        ftemp= ftemp / pow(2.0, 9.0);
    else 
      ftemp= ftemp / pow(2.0, 12.0);
        
    ft =  ft / 256.0;
    fp = ftemp;                                    
    *rp = fp;
    *rt = ft;
}

void getZXPValue(_I2C_T  I2C_Interface, uint8_t rang, float *rp, float *rt)
{
    uint8_t msCnt = 0;
    uint32_t temp, press;
    
    ZXP3010D_StartT(I2C_Interface);
	DelayMs(4);
	do{
		DelayMs(1);
        msCnt++;
	}while(ZXP3010D_ConStatus(I2C_Interface) && msCnt < ZXP3010D_COMM_ERR_MAXCNT);
	temp = ZXP3010D_ResultT(I2C_Interface);
    if(msCnt == ZXP3010D_COMM_ERR_MAXCNT)
      return;
      
	ZXP3010D_StartP(I2C_Interface);
	DelayMs(12);
    msCnt = 0;
	do{
		DelayMs(1);
		msCnt++;
	}while(ZXP3010D_ConStatus(I2C_Interface) && msCnt < ZXP3010D_COMM_ERR_MAXCNT);
	press = ZXP3010D_ResultP(I2C_Interface);
    if(msCnt == ZXP3010D_COMM_ERR_MAXCNT)
      return;
    
	ZXP3010D_Caculate(rang, press, temp, rp, rt);
}

