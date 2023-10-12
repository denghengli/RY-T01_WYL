#ifndef _APP_GASPRESS_
#define _APP_GASPRESS_

typedef struct{
	
	int   MinPress; //传感器可测量的最小压力值(Pa)
	int   MaxPress; //传感器可测量的最大压力值(Pa)
	int   MinCount; //传感器可计数的最小压力计数值
	int   MaxCount; //传感器可计数的最大压力计数值
	
	int   MsPresCnt;  //传感器测量的压力计数值
	int   MsTempCnt;  //传感器测量的温度计数值
	char  MsState;    //传感器状态
	
	float Press;        //通过公式换算出的传感器测量的压力值实时值
	float Pressavg;     //通过公式换算出的传感器测量的压力值平均值
	float PressavgAdj;  //通过公式换算出的传感器测量的压力值平均值加上校准值
	float Temp;         //通过公式换算出的传感器测量的温度
	
    float OffSet;       //偏移量
    float AdjFactor;   //校准系数
    
}SM9541_DATA_T;

extern void FlueGasPress_DataInit(void);
extern void FlueGasPress_Measure(void);

#endif


