#ifndef _APP_GASPRESS_
#define _APP_GASPRESS_

typedef struct{
	
	int   MinPress; //�������ɲ�������Сѹ��ֵ(Pa)
	int   MaxPress; //�������ɲ��������ѹ��ֵ(Pa)
	int   MinCount; //�������ɼ�������Сѹ������ֵ
	int   MaxCount; //�������ɼ��������ѹ������ֵ
	
	int   MsPresCnt;  //������������ѹ������ֵ
	int   MsTempCnt;  //�������������¶ȼ���ֵ
	char  MsState;    //������״̬
	
	float Press;        //ͨ����ʽ������Ĵ�����������ѹ��ֵʵʱֵ
	float Pressavg;     //ͨ����ʽ������Ĵ�����������ѹ��ֵƽ��ֵ
	float PressavgAdj;  //ͨ����ʽ������Ĵ�����������ѹ��ֵƽ��ֵ����У׼ֵ
	float Temp;         //ͨ����ʽ������Ĵ������������¶�
	
    float OffSet;       //ƫ����
    float AdjFactor;   //У׼ϵ��
    
}SM9541_DATA_T;

extern void FlueGasPress_DataInit(void);
extern void FlueGasPress_Measure(void);

#endif


