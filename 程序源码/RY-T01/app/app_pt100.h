#ifndef _APP_PT100_
#define _APP_PT100_

typedef struct{
	unsigned short AD;  //读取的AD值
	float Vol;          //PT100电压值
	float ResVal;       //转换成的电阻值
	
	float Temp;      //根据电阻查表计算的温度
	float Tempavg;   //根据电阻查表计算的温度
	float Tempsum;   //根据电阻查表计算的温度
}PT100_DATA_T;

extern void  PT100_RefCal_Flag(unsigned char flag);

#endif


