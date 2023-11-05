#ifndef _APP_ATPPRESS_
#define _APP_ATPPRESS_

typedef struct{
	unsigned short AD;  //通过ADC采集的AD值
	float Vol;          //电压值
	float TempFactor;   //计算时的温度系数
	float PressError;   //压力误差(KPa)
	float airPress;     //计算出的大气压强(KPa)
}MPXHZ_DATA_T;

extern void AtpPress_Measure(void);


#endif

