#include "includes.h"

#define FREQ_TAB_SCALE  10 /*   ʪ�ȴ�����Ƶ�ʱ�̶�   */
#define FREQ_TAB_NUM    11 /*   ʪ�ȴ�����Ƶ�ʱ���Ŀ   */

/* ʪ�ȴ�����Ƶ����ʪ�ȶ�Ӧ�� */
const static uint32_t HUMIT_FREQ_TAB[] = {7351, 7224, 7100, 6976, 6853, 6728, 6600, 6468, 6330, 6186, 6033};

static uint32_t s_iFreqHumit = 0;/* ʪ�ȴ�����Ƶ��*/
static uint32_t s_iFreqOverCnt = 0;/*ʪ�ȴ�����Ƶ�ʼ����������*/

/**********************************************************************************************************
*	�� �� ��: Humit_FreqOver_Set
*	����˵��: ���ж�������1S��Ƶ�ʼ����������
*	��    ��: NONE
*	�� �� ֵ: NONE
**********************************************************************************************************/
void Humit_FreqOver_Set(void)
{
    s_iFreqOverCnt += 1;
}

/**********************************************************************************************************
*	�� �� ��: Humit_Freq_Set
*	����˵��: ���ж�������ʪ�ȴ������������
*	��    ��: _ifreq   IS��������
*	�� �� ֵ: NONE
**********************************************************************************************************/
void Humit_Freq_Set(uint32_t _ifreq)
{
    s_iFreqHumit = _ifreq;
}

/**********************************************************************************************************
*	�� �� ��: Humit_Freq_Get
*	����˵��: �������л�ȡʪ�ȴ������������
*	��    ��: NONE
*	�� �� ֵ: IS��������
**********************************************************************************************************/
uint32_t Humit_Freq_Get(void)
{
    uint32_t iTemp = 0;
    
    iTemp = s_iFreqOverCnt * 65535 + s_iFreqHumit;
    
    s_iFreqOverCnt = 0;//�������
    
    return iTemp;
}

/**********************************************************************************************************
*	�� �� ��: HumitMeasure
*	����˵��: ʪ�Ȳ���
*	��    ��: NONE
*	�� �� ֵ: NONE
**********************************************************************************************************/
void HumitMeasure(void)
{
    unsigned char i = 0;
    float fTemp1 = 0.0,fTemp2 = 0.0; 
    float humit = 0;
    
    uint32_t iFreqHumit =  Humit_Freq_Get();  //s_iFreqHumit����������ִ�й����б��жϸ�д�������ȸ�ֵ��һ����
    
    LOG_PRINT(DEBUG_HUMIT,"Humit->Freq = %d \r\n",iFreqHumit);
    
    if(iFreqHumit > HUMIT_FREQ_TAB[0] || iFreqHumit < HUMIT_FREQ_TAB[10])
    {
        humit = 0;
    }
    else
    {
        for(i=0;i<FREQ_TAB_NUM;i++)
        {
            if(iFreqHumit <= HUMIT_FREQ_TAB[i] && iFreqHumit >= HUMIT_FREQ_TAB[i+1])
            {
                fTemp1 = (float)(HUMIT_FREQ_TAB[i] - HUMIT_FREQ_TAB[i+1]) / FREQ_TAB_SCALE; //f/RH
                fTemp2 = (float)(HUMIT_FREQ_TAB[i] - iFreqHumit) / fTemp1;
                    
                humit = (float)FREQ_TAB_SCALE * i + fTemp2;
                break;
            }
        }       
    }

    //�Ƿ����У׼ֵ
    if (g_SysData.Data.Para.humitZreoValib) {
        g_SysData.Data.Sample.humit = humit + g_SysData.Data.Para.humitZero;
    } else {
        g_SysData.Data.Sample.humit = humit;
    }

    SampleData_ToModbus();
}

/**********************************************************************************************************
*	�� �� ��: APP_Humit
*	����˵��: ʪ�ȴ�����
*	��    ��: argument   ����ָ��
*	�� �� ֵ: NONE
**********************************************************************************************************/
void APP_Humit(void *pvParameters)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
	
	while(1)
	{
        HumitMeasure();
        
        LOG_PRINT(DEBUG_TASK,"APP_Humit \r\n");
        
        vTaskDelay(sMaxBlockTime);
	}
}
