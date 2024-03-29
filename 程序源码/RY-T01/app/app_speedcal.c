#include "includes.h"

#define SPEEDCAL_AVG_NUM 5

static uint8_t Flag_Measover = 0;  //ȫѹ����ѹ������ѹ���¶Ȳ�����ɱ�־������������ʱ��Ҫ�������Ĳɼ����

/**********************************************************************************************************
*	�� �� ��: SetFlg_Measover
*	����˵��: ����ȫѹ����ѹ������ѹ���¶Ȳ�����ɱ�־
*	��    ��: _cflg ��ɱ�־λ
*	�� �� ֵ: NONE
**********************************************************************************************************/
void SetFlg_Measover(uint8_t _cflg)
{
    xEventGroupSetBits(EventGSpeedCal, _cflg);//�¼���־�鷽��
}

/**********************************************************************************************************
*	�� �� ��: GetFlg_Measover
*	����˵��: �������ټ��㴦��
*	��    ��: ��ȡȫѹ����ѹ������ѹ���¶Ȳ�����ɱ�־
*	�� �� ֵ: ��ɱ�־λ
**********************************************************************************************************/
uint8_t GetFlg_Measover(void)
{
	return Flag_Measover;
}


/**********************************************************************************************************
*	�� �� ��: FlueGasSpeedCal_Proc
*	����˵��: �������ټ��㴦��
*	��    ��: NONE
*	�� �� ֵ: NONE
**********************************************************************************************************/
void FlueGasSpeedCal_Proc(void)
{
    float fAdjFactor = g_SysData.Data.Para.piTGRatioK;
    float fAssistFactor = g_SysData.Data.Para.speedRatioK;
    float fDynPress = g_SysData.Data.Sample.dynPress;
    float fAirDensity = g_SysData.Data.Para.density;//�����ܶ�
	float fSpend,fSpeedavg,fFlow,sum=0.0;
	static float fSpeedBuf[SPEEDCAL_AVG_NUM] = {0.0};
	static uint8_t AvgCnt = 0;
	static uint8_t BufCnt = 0;
	uint8_t i = 0;
	int random_integer;
      
    /*�������*/
    if(fDynPress > 0.5)//��ѹ����0
    {
        if (fDynPress > 1)
        {
            /*�����ܶ�*/
            //fAirDensity = (1.34 * (1 - 0.5) + 0.804 * 0.5) * \
            //(273.0 /(273.0 + fPT100Tem)) * ((101 + fSticPress/1000.0)/101.325);
            fSpend = 1.414 * fAdjFactor * (sqrt(fDynPress / fAirDensity));
        }
        else
        {
            random_integer = rand();
            // ���������ӳ�䵽0��0.5�ķ�Χ��
            fSpend = (double)random_integer / ((double)RAND_MAX + 1) * 0.5;
        }
    }
    else
    {
        fSpend = 0;
    }

    /*�����ݴ��뻷��buf��*/
    if(BufCnt <= SPEEDCAL_AVG_NUM)
    {
        if(BufCnt == SPEEDCAL_AVG_NUM)BufCnt = 0;
        if(AvgCnt < SPEEDCAL_AVG_NUM)AvgCnt++;
    }
    fSpeedBuf[BufCnt++] = fSpend;

    /*�Ի���buf���������*/
    for(i=0;i<AvgCnt;i++)
    {
        sum += fSpeedBuf[i];
    }
    fSpeedavg = sum / (float)AvgCnt;/*���㻷��buf�����ݾ�ֵ*/
    
    fSpeedavg = fSpeedavg * fAssistFactor;//���Ը���ϵ��
    fFlow = fSpeedavg * g_SysData.Data.Para.sectionArea;
    
    /* �Ѳ������ݴ���ȫ�ֱ����� */
    FloatLimit(&fDynPress, FLOAT_DECNUM);
    FloatLimit(&fSpeedavg, FLOAT_DECNUM);
    FloatLimit(&fFlow, FLOAT_DECNUM);
    g_SysData.Data.Sample.dynPress = fDynPress;
    g_SysData.Data.Sample.speed = fSpeedavg;
    g_SysData.Data.Sample.flow = fFlow;
    
    SampleData_ToModbus();
}



/**********************************************************************************************************
*	�� �� ��: APP_SpeedCal
*	����˵��: APP_SpeedCal function
*	��    ��: argument   ����ָ��
*	�� �� ֵ: NONE
**********************************************************************************************************/
void APP_SpeedCal(void  * argument)
{
    TickType_t  sMaxBlockTime =	pdMS_TO_TICKS(2000);
    EventBits_t uxBits;
    
	while(1)
	{
        LOG_PRINT(DEBUG_TASK,"APP_SpeedCal \r\n");
         
        uxBits = xEventGroupWaitBits(EventGSpeedCal, /* �¼���־���� */
                                     FLG_MEASOVER,        /* �ȴ����Ա�־������ */
                                     pdTRUE,              /* �˳�ǰ���Ա�־����������������Ա�־�������òű�ʾ���˳���*/
                                     pdTRUE,              /* ����ΪpdTRUE��ʾ�ȴ�bit1��bit0��������*/
                                     sMaxBlockTime); 	  /* �ȴ��ӳ�ʱ��,portMAX_DELAYʱ���õȴ� */
        if((uxBits & FLG_MEASOVER) == FLG_MEASOVER)
        {
            FlueGasSpeedCal_Proc();
        }        
        
//        vTaskDelay(sMaxBlockTime);
	}
}




