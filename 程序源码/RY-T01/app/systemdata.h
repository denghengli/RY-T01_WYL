#ifndef __SYSTEMDATA_H__
#define __SYSTEMDATA_H__
#include "FreeRTOS.h"
#include "cmsis_os.h"

#define SOFTVER 100

/*系统状态*/
#define SYS_STA_MEASU  1
#define SYS_STA_BLOW   0

/*浮点数据小数位数*/
#define FLOAT_DECNUM 3

/*接收发送buf长度*/
#define UART_RXBUF_SIZE		100
#define UART_TXBUF_SIZE		200

/*ADC采集通道数  采集平均次数*/
#define ADC_CH_MAX  1
#define ADC_AVG_MAX 10

/*串口传输时使用的结构体*/
typedef struct
{
	unsigned char RxBuf[UART_RXBUF_SIZE];
	unsigned char RxCount;
	unsigned char RxNewFlag;
	unsigned char TxBuf[UART_TXBUF_SIZE];  
	unsigned char TxRspCode;   /*通讯异常时，需要发送的错误码*/
	unsigned char TxCount;
	
}UART_TRANS_T;

/*AD采样数据*/
typedef struct 
{
	unsigned int   AdBuf[ADC_CH_MAX][ADC_AVG_MAX];    //AD采集裸数缓冲区
	unsigned int   AdAver[ADC_CH_MAX]; //采集中间平均值
	unsigned char  AdCnt;		       //当前记录缓冲区位置
    unsigned char  AdCh;		       //当前接收的ADC通道
	
}AD_DATA_T;

/*实时采样数据，当有不是4字节时一定要强制单字节对齐*/
__packed typedef struct 
{
  	unsigned int SysSta; //系统状态 1：测量 0：反吹
	float  TotalP; 		//全压(Pa)
	float  SticP; 		//静压(Pa)
	float  DynP; 		//动压(Pa)
    float  Speed; 		//烟气流速(m/s)
    float  PTTem; 		//PT100温度
	
//	float DynMsTem;
//	unsigned int DynMsState;
//	unsigned int DynMsPresCnt;
//	unsigned int DynMsTempCnt;
//	float SticMsTem;
//	unsigned int SticMsState;
//	unsigned int SticMsPresCnt;
//	unsigned int SticMsTempCnt;
	
	float  NONE1;//预留
	float  NONE2;//预留
	float  NONE3;//预留
	float  NONE4;//预留
	float  NONE5;//预留
    float  NONE6;//预留
    float  NONE7;//预留
	float  NONE8;//预留
	float  NONE9;//预留
	
}SAMPLE_DATA_T;

/*掉电存储的参数，当有不是4字节时一定要强制单字节对齐*/
__packed typedef struct
{
	unsigned short 	DevId;	//设备ID
	unsigned short  SoftVer;//软件版本号
    unsigned short  SpeedAtAdjFlg;//风速计自动校准标志
    unsigned short  BlowIntvar;//反吹间隔时间(h)
	
    float  PiTGRatioK;      //皮托管风速计系数
    float  SpeedRatioK;     //风速校准系数
    float  Density;         //空气密度
	float  DynPRatioB; 	    //动压零点偏移量(Pa)
	float  SticPRatioB; 	//静压零点偏移量(Pa)
    float  DynPRatioK; 	    //动压校准系数
	float  SticPRatioK; 	//静压校准系数
	float  PTTemRef;        //PT100温度参考值
	float  PTTemRatioB; 	//PT100温度偏移量
	int    PresCnt;         //全压静压计数值,越大响应时间越长,平滑度越好，越小则相反，最大50，默认20
	
}PARA_DATA_T;

/*存入FLASH的参数*/
__packed typedef struct
{
    unsigned int InitFlag;
    PARA_DATA_T  Para;
	
}PARA_FLASH_T;


/*系统数据*/
typedef union
{
    #define SAMPLE_DATA_LEN  (sizeof(SAMPLE_DATA_T) / 2)
    #define PARA_DATA_LEN    (sizeof(PARA_DATA_T) / 2)
    #define SYSTEM_DATA_LEN  (SAMPLE_DATA_LEN + PARA_DATA_LEN)
    
	struct
	{
		SAMPLE_DATA_T	Sample;
		PARA_DATA_T		Para;
		
	}Data;
	
	unsigned short	RegBuf[SYSTEM_DATA_LEN]; //2字节数据字节序为AB，4字节数据字节序为CDAB
		
}SYSTEM_DATA_UN;


extern volatile SYSTEM_DATA_UN   g_SysData;
extern xSemaphoreHandle CommSem;
extern xSemaphoreHandle AdSem;
extern xSemaphoreHandle FlashMutex;
extern xSemaphoreHandle MutexPrint;
extern EventGroupHandle_t  EventGSpeedCal;
    
extern unsigned short DevId_Get(void);
extern void SampleData_ToModbus(void);
extern void ParaData_Init(void);
extern void ParaData_Save(unsigned char opt);
extern void ParaData_Updata(void);


#endif

	
	
	
	