#ifndef _APP_PT100_
#define _APP_PT100_

typedef struct{
	
	unsigned short AD;     //��ȡ��ADֵ
	float           Vol;    //PT100��ѹֵ
	float           ResVal; //ת���ɵĵ���ֵ
	
	float           Temp;      //���ݵ����������¶�
	float           Tempavg;   //���ݵ����������¶�
	float           Tempsum;   //���ݵ����������¶�
	
	unsigned int   AD33;     //3.3V��ȡ��ADֵ
	float           Vol33;    //3.3V��ѹֵ
	
}PT100_DATA_T;


extern void  PT100_TempMeasure(void);
extern void  PT100_RefCal_Flag(unsigned char flag);

#endif


