#ifndef _APP_ATPPRESS_
#define _APP_ATPPRESS_

typedef struct{
	unsigned short AD;  //ͨ��ADC�ɼ���ADֵ
	float Vol;          //��ѹֵ
	float TempFactor;   //����ʱ���¶�ϵ��
	float PressError;   //ѹ�����(KPa)
	float airPress;     //������Ĵ���ѹǿ(KPa)
}MPXHZ_DATA_T;

extern void AtpPress_Measure(void);


#endif

