#include "includes.h"

#define MPXHZ_VCC 5.0f

static MPXHZ_DATA_T  s_tAtpPressDat;//����ѹǿ����ȫ�ֱ���

/********************************************************************************************************
*	�� �� ��: MPX_GetTempFactor
*	����˵��: ��ȡ��������ѹ���������¶�ϵ��
*	��    ��: NONE
*	�� �� ֵ: �¶�ϵ��
********************************************************************************************************/
static float MPX_GetTempFactor()
{
	float fRes,fTemp = 0.0f;
	
	fTemp = g_SysData.Data.Sample.ptTem;//��ȡ�����¶� -40�� -- 125��
    
	if(fTemp < -40 || fTemp > 125)//�¶ȳ�������ѹ������������Χ���п�����PT100�¶Ȳ���������
	{
		return 0;
	}
	
	if(fTemp < 0)/*-40��  --  0�� y=-1/20x + 1*/
	{
		fRes = -0.05 * fTemp + 1;
	}
	else if(fTemp > 85)/*85��  --  125��  y=1/40x - 45/40*/
	{
		fRes = 0.025 * fTemp - 1.125;
	}
	else/*0��  --  85��*/
	{
		fRes = 1;
	}

	return fRes;
}


/********************************************************************************************************
*	�� �� ��: AtpPress_GetAD
*	����˵��: ��ȡ��������ѹ��������ADֵ
*	��    ��: NONE
*	�� �� ֵ: ��������ѹ��������ADֵ
********************************************************************************************************/
static unsigned short AtpPress_GetAD()
{
	return g_ADCData.Aver[2];
}

/********************************************************************************************************
*	�� �� ��: AtpPress_Measure
*	����˵��: ����ѹǿ����
*	��    ��: NONE
*	�� �� ֵ: NONE
********************************************************************************************************/
void AtpPress_Measure(void)
{
	float temp;
	
	s_tAtpPressDat.AD  = AtpPress_GetAD();//�ɼ����ĵ�ѹADֵ

	temp = ((float)3.0 / 4096.0) * s_tAtpPressDat.AD;
	s_tAtpPressDat.Vol = 1.5 * temp;//ͨ����ѹ�ȼ�������յĴ�����������ѹ
	 
    if (s_tAtpPressDat.Vol < 0.2)
    {
        s_tAtpPressDat.airPress = 20;
    }
    else
    {
        s_tAtpPressDat.airPress = (s_tAtpPressDat.Vol - 0.2) / (4.6/380.0) + 20;
    }
//	s_tAtpPressDat.TempFactor = MPX_GetTempFactor();
//	s_tAtpPressDat.PressError = 0; //-1.5KPa - 1.5KPa
//
//	temp = s_tAtpPressDat.TempFactor * s_tAtpPressDat.PressError * 0.009 * MPXHZ_VCC;
//	s_tAtpPressDat.airPress = (((float)s_tAtpPressDat.Vol/MPXHZ_VCC + 0.095) / 0.009) + temp;
	
    /* �Ѳ������ݴ���ȫ�ֱ����� */
    FloatLimit(&s_tAtpPressDat.airPress,FLOAT_DECNUM);
	g_SysData.Data.Sample.blowGasPress = s_tAtpPressDat.airPress;
    
    SampleData_ToModbus();
    
    LOG_PRINT(DEBUG_ATPPRESS,"MPXHZ.AD = %d\r\n",s_tAtpPressDat.AD);
	LOG_PRINT(DEBUG_ATPPRESS,"MPXHZ.Vol = %f\r\n",s_tAtpPressDat.Vol);
	LOG_PRINT(DEBUG_ATPPRESS,"MPXHZ.TempFactor = %f\r\n",s_tAtpPressDat.TempFactor);
	LOG_PRINT(DEBUG_ATPPRESS,"MPXHZ.airPress = %f\r\n\r\n",s_tAtpPressDat.airPress);
}


/**********************************************************************************************************
*	�� �� ��: APP_AtspP
*	����˵��: APP_AtspP function
*	��    ��: argument   ����ָ��
*	�� �� ֵ: NONE
**********************************************************************************************************/
void APP_AtspP(void  * argument)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
    
	while(1)
	{
		AtpPress_Measure();
        
        LOG_PRINT(DEBUG_TASK,"APP_AtspP \r\n");
        
        vTaskDelay(sMaxBlockTime);
	}
}
