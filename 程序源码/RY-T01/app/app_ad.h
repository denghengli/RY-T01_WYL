#ifndef __AD_H__
#define __AD_H__

#include "systemdata.h"

typedef struct ADC_DATA
{
    unsigned short   Buf [ADC_CH_MAX][ADC_AVG_MAX];//AD采集裸数缓冲区
    unsigned short   Aver[ADC_CH_MAX];   //采集中间平均值
    unsigned short   Cnt;  //当前记录缓冲区位置
    unsigned short   ChCnt;  

}ADC_DATA_T;


extern volatile ADC_DATA_T g_ADCData;



#endif