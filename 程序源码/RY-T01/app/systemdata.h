#ifndef __SYSTEMDATA_H__
#define __SYSTEMDATA_H__
#include "FreeRTOS.h"
#include "cmsis_os.h"

/*浮点数据小数位数*/
#define FLOAT_DECNUM 3

/*接收发送buf长度*/
#define UART_RXBUF_SIZE		100
#define UART_TXBUF_SIZE		200

/*ADC采集通道数  采集平均次数*/
#define ADC_CH_MAX  3
#define ADC_AVG_MAX 10

/*系统状态*/
typedef enum
{
    eSYSSTA_MEASU = 1,  //正常测量状态
    eSYSSTA_PITG_BLOW,  //皮托管反吹   
    eSYSSTA_HUMIT_BLOW, //湿度管反吹
    eSYSSTA_HAVE_BLOW_CALIB_ZERO,   //有反吹气校零
    eSYSSTA_NO_BLOW_CALIB_ZERO,     //无反吹气校零
    eSYSSTA_HUMIT_CALIB //湿度零点校准
}SYSTEM_STATE_E;

/*串口传输时使用的结构体*/
typedef struct
{
    uint8_t Num; /*主机时从机号*/
	uint8_t RxBuf[UART_RXBUF_SIZE];
	uint8_t RxCount;
	uint8_t RxNewFlag;
	uint8_t TxBuf[UART_TXBUF_SIZE];  
	uint8_t TxRspCode;   /*通讯异常时，需要发送的错误码*/
	uint8_t TxCount;
}UART_TRANS_T;

/*AD采样数据*/
typedef struct 
{
	uint32_t AdBuf[ADC_CH_MAX][ADC_AVG_MAX]; //AD采集裸数缓冲区
	uint32_t AdAver[ADC_CH_MAX]; //采集中间平均值
	uint8_t AdCnt;  //当前记录缓冲区位置
    uint8_t AdCh;   //当前接收的ADC通道
}AD_DATA_T;

/*实时采样数据，当有不是4字节时一定要强制单字节对齐*/
__packed typedef struct 
{
    uint32_t softVer;   //软件版本
    uint32_t MSN;       //MSN地址
    uint32_t sysSta;    //系统状态，1:测量状态 2:反吹状态
    float totalPress;   //全压(Pa)
    float dynPress;     //动压(Pa)
    float sticPress;    //静压(Pa)
    float blowGasPress; //反吹气压力(KPa)
    float speed;        //流速(m/s)
    float flow;         //流量(m³/s)
    float ptTem;        //环境温度
    float ptHeatTem;    //伴热温度
    float abshumit;     //绝对湿度(%RH)
    float relhumit;     //相对湿度(%V)
}SAMPLE_DATA_T;

/*掉电存储的参数，当有不是4字节时一定要强制单字节对齐*/
__packed typedef struct
{
    uint16_t devId;
    uint32_t baudRate;  //波特率
    uint16_t blowFreq;  //反吹频次
    uint16_t blowInterval;  //反吹间隔(min)
    float piTGRatioK;   //皮托管系数
    float speedRatioK;  //风速校准系数
    float density;      //空气密度
	float dynPRatioB;   //动压零点偏移量(Pa)
	float sticPRatioB;  //静压零点偏移量(Pa)
    float dynPRatioK;   //动压校准系数
	float sticPRatioK;  //静压校准系数
    uint16_t autoCalibZero; //自动校零，1=开启 0=关闭
    float ptTemRef;         //PT100温度参考值
    float ptTemRatioB;      //PT100温度偏移量
    float humitZero;        //湿度零点
    uint16_t humitZreoValib;//湿度零点有效，1=有效 0=无效
    uint16_t humitDispUnit; //湿度显示单位，1=相对 0=绝对
    float sectionArea;      //烟道截面积(㎡)
    uint16_t smoothTime;    //平滑时间(s),越大响应时间越长,平滑度越好，越小则相反，最大50，默认20
    uint16_t speedCalibZeroInterval; //流速自动校零间隔（h）
    uint16_t resv[4];
	
    uint16_t humitType;     //湿度传感器类型,0:A 1:B
    float temAOOffset;      //温度20mA输出偏移
    float temAOK;           //温度20mA输出系数
    float pressAOOffset;    //静压20mA输出偏移
    float pressAOK;         //静压20mA输出系数
    float speedAOOffset;    //流速20mA输出偏移
    float speedAOK;         //流速20mA输出系数
    float humitAOOffset;    //湿度20mA输出偏移
    float humitAOK;         //湿度20mA输出系数
    uint16_t humitFrepTab[11];//湿度-频率对应表
      
    uint16_t factoryFlg;    //恢复出厂标志，1恢复出厂
    uint16_t DO[4]; //电磁阀控制

    uint16_t speedCalibZeroFlg; //流速（压力）校零标志，0关闭 1开启校准 2完成校准
}PARA_DATA_T;


/*存入FLASH的参数*/
__packed typedef struct
{
    uint32_t InitFlag;
    PARA_DATA_T Para;
}PARA_FLASH_T;

/*系统数据*/
typedef union
{
    #define SAMPLE_DATA_LEN  (sizeof(SAMPLE_DATA_T) / 2)
    #define PARA_DATA_LEN    (sizeof(PARA_DATA_T) / 2)
    #define SYSTEM_DATA_LEN  (SAMPLE_DATA_LEN + PARA_DATA_LEN)
	struct
	{
		SAMPLE_DATA_T Sample;
		PARA_DATA_T Para;
	}Data;
	unsigned short regBuf[SYSTEM_DATA_LEN]; //2字节数据字节序为AB，4字节数据字节序为CDAB
}SYSTEM_DATA_UN;

#define HOLD_REGNUM PARA_DATA_LEN
#define INPUT_REGNUM SAMPLE_DATA_LEN

extern volatile SYSTEM_DATA_UN g_SysData;
extern xSemaphoreHandle CommSem;
extern xSemaphoreHandle AdSem;
extern xSemaphoreHandle FlashMutex;
extern xSemaphoreHandle MutexPrint;
extern EventGroupHandle_t EventGSpeedCal;
    
extern void SampleData_ToModbus(void);
extern void ParaData_Init(void);
extern void ParaData_Save(uint8_t opt);
extern void ParaData_Updata(void);
extern void ParaData_Factory(void);

#endif

	
	
	
	