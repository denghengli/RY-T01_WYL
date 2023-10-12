#ifndef __AD_H__
#define __AD_H__

#include "systemdata.h"

typedef struct ADC_DATA
{
    unsigned short   Buf [ADC_CH_MAX][ADC_AVG_MAX];//AD�ɼ�����������
    unsigned short   Aver[ADC_CH_MAX];   //�ɼ��м�ƽ��ֵ
    unsigned short   Cnt;  //��ǰ��¼������λ��
    unsigned short   ChCnt;  

}ADC_DATA_T;


extern volatile ADC_DATA_T g_ADCData;



#endif