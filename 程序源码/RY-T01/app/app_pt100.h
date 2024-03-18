#ifndef _APP_PT100_
#define _APP_PT100_

typedef struct{
	unsigned short AD;  //��ȡ��ADֵ
	float Vol;          //PT100��ѹֵ
	float ResVal;       //ת���ɵĵ���ֵ
	
	float Temp;      //���ݵ����������¶�
	float Tempavg;   //���ݵ����������¶�
	float Tempsum;   //���ݵ����������¶�
}PT100_DATA_T;

extern void  PT100_RefCal_Flag(unsigned char flag);

#endif


