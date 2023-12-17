#include "includes.h"

#define UI_STATE_ARR_MAX 25

#define KEY_NULL    0x00
#define KEY_UP      0x01
#define KEY_DOWN    0x02
#define KEY_RIGHT   0x03
#define KEY_OK      0x04
#define KEY_RETURN  0x05

volatile uint8_t ui_pre_state = STATE_NULL;
volatile uint8_t ui_cur_state = START_LOGO;
uint8_t cur_main_pagenum = 1; //������ҳ��

const UI_STATE_TRANS UIStateArray[UI_STATE_ARR_MAX] =
{
    /*��ǰ      �ϼ�      �¼�       �Ҽ�       OK��   ���ؼ�*/
    {START_LOGO, START_LOGO, START_LOGO, START_LOGO, START_LOGO, START_LOGO}, 
    {MAIN_WIND,  MAIN_WIND, MAIN_WIND,  MAIN_WIND_RIGHT,  MAIN_WIND,  MAIN_WIND},
};

const state_fun MenuFun[] = 
{
    NULL,

    start_logo,
    main_wind,
    main_wind_right,
};

/*���� */
void clean_sercen(void)
{
	FNC_LCD_DISP_DRAW_PARA lcd_para;
	char tmp_str[24] = {0};

	lcd_para.cmd = FCN_LCD_DISP_CLEAR;
	lcd_para.bc = DARKBLUE;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

/*���� ��ɫ*/
void clean_sercen_white(void)
{
	FNC_LCD_DISP_DRAW_PARA lcd_para;
	char tmp_str[24] = {0};

	lcd_para.cmd = FCN_LCD_DISP_CLEAR;
	lcd_para.bc = WHITE;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

/*�ϵ���ʼ����*/
void start_logo(void *para)
{
	FNC_LCD_DISP_DRAW_PARA lcd_para;
	char tmp_str[32] = {0};
	uint8_t buf[8] = {0};

	lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
	lcd_para.bc = WHITE;
	lcd_para.fc = DARKBLUE;
	lcd_para.mode = 24;
	lcd_para.x = 32;
	lcd_para.y = LCD_H / 2 - 60 - 30;
	strncpy(tmp_str, "RY-T01", sizeof(tmp_str));
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

	lcd_para.cmd = FNC_LCD_DISP_DRAW_LINE;
	lcd_para.bc = DARKBLUE;
	lcd_para.fc = DARKBLUE;
	lcd_para.mode = 24;
	lcd_para.x = 32;
	lcd_para.y = 85 - 25;
	buf[0] = 32 + 24 * 4;
	buf[1] = 0;
	buf[2] = 85 - 25;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);

	lcd_para.x = 32;
	lcd_para.y = 87 - 25;
	buf[0] = 32 + 24 * 4;
	buf[1] = 0;
	buf[2] = 87 - 25;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);

	lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
	lcd_para.bc = WHITE;
	lcd_para.fc = DARKBLUE;
	lcd_para.mode = 24;
	lcd_para.x = 32;
	lcd_para.y = LCD_H / 2 - 30 - 20;
	strncpy(tmp_str, "��ѹ��ʪһ������", sizeof(tmp_str));
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
	
    vTaskDelay(1000 / portTICK_RATE_MS);
	ui_cur_state = MAIN_WIND;
	clean_sercen();
}

void main_wind(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;

    if (cur_main_pagenum == 1)
    {
        /*�¶�*/
        lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
        lcd_para.x = 20;
        lcd_para.y = 20;
        lcd_para.bc = DARKBLUE;
        lcd_para.fc = WHITE;
        lcd_para.mode = 24;
        snprintf(tmp_str, sizeof(tmp_str), "�¶ȣ�%7.2f ��", g_SysData.Data.Sample.ptTem);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

        /*ʪ��*/
        lcd_para.y = 60;
        snprintf(tmp_str, sizeof(tmp_str), "ʪ�ȣ�%7.2f %%V", g_SysData.Data.Sample.humit);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

        /*����*/
        lcd_para.y = 100;
        snprintf(tmp_str, sizeof(tmp_str), "���٣�%7.2f m/s", g_SysData.Data.Sample.speed);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

        /*����*/
        lcd_para.y = 140;
        snprintf(tmp_str, sizeof(tmp_str), "������%7.2f m3/s", g_SysData.Data.Sample.flow);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
    }
    else if (cur_main_pagenum == 2)
    {
        /*��ѹ*/
        lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
        lcd_para.x = 20;
        lcd_para.y = 20;
        lcd_para.bc = DARKBLUE;
        lcd_para.fc = WHITE;
        lcd_para.mode = 24;
        snprintf(tmp_str, sizeof(tmp_str), "��ѹ��%7.2f kPa", g_SysData.Data.Sample.sticPress / 1000.0);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

        /*��ѹ*/
        lcd_para.y = 60;
        snprintf(tmp_str, sizeof(tmp_str), "��ѹ��%7.2f Pa", g_SysData.Data.Sample.dynPress);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

        /*����*/
        lcd_para.y = 100;
        snprintf(tmp_str, sizeof(tmp_str), "������%7.2f MPa", g_SysData.Data.Sample.blowGasPress / 1000.0);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
    }
    
    /*��ҳ*/
    lcd_para.x = 200;    
    lcd_para.y = 200;
    lcd_para.fc = YELLOW;
    lcd_para.mode = 24;
    if (cur_main_pagenum == 1)
    {
        sprintf(tmp_str, "%s", "����һҳ");
    }
    else if (cur_main_pagenum == 2)
    {
        sprintf(tmp_str, "%s", "����һҳ");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void main_wind_right(void *para)
{
	if (cur_main_pagenum == 1) cur_main_pagenum = 2;
    if (cur_main_pagenum == 2) cur_main_pagenum = 1;
    
	ui_cur_state = MAIN_WIND;
}

int read_key_value(void)
{
//    vTaskDelay(200 / portTICK_PERIOD_MS);//��������
    return KEY_NULL;
}

void GUI_handle(void)
{
    uint32_t key = KEY_NULL;
    uint8_t mpara = 0;
	uint8_t clean_flag = 1;
	uint8_t flash_menu_flag = 0;
	
    key = read_key_value();
    if (key != KEY_NULL)//�а�������
    {
        for (uint8_t i=0; i<UI_STATE_ARR_MAX; i++)
        {
            if (ui_cur_state == UIStateArray[i].cur_state)
            {
                ui_pre_state = ui_cur_state;

                switch (key)
                {
                    case KEY_UP:
                        ui_cur_state = UIStateArray[i].up_state;
                        break;

                    case KEY_DOWN:
                        ui_cur_state = UIStateArray[i].down_state;
                        break;  

                    case KEY_RIGHT:
                        ui_cur_state = UIStateArray[i].right_state;
                        break;   

                    case KEY_OK:
                        ui_cur_state = UIStateArray[i].ok_state;
                        break;  

                    case KEY_RETURN:
                        ui_cur_state = UIStateArray[i].return_state;
                        break; 

                    default :break;    
                }

                break;
            }
        }

        if ((ui_cur_state != STATE_NULL) && (ui_cur_state != ui_pre_state))
        {
            clean_flag = is_clean();//�жϵ�ǰ�����Ƿ���Ҫ�������ػ�
            if (clean_flag)
            {
                clean_sercen();
            }
            MenuFun[ui_cur_state](&mpara);
        }
    }

    /* ��û�а������µĿ���ʱ�䣬���滹�Ƿ���Ҫˢ�£���Ҫ������Ҫ��˸�Ľ���*/
	if (key == KEY_NULL)
	{
		flash_menu_flag = is_flash_menu();
		if (flash_menu_flag)
		{
			MenuFun[ui_cur_state](&mpara);
		}
	}
}


/**********************************************************************************************************
*	�� �� ��: APP_GUI
*	����˵��: APP_GUI function
*	��    ��: argument   ����ָ��
*	�� �� ֵ: NONE
**********************************************************************************************************/
void APP_GUI(void  * argument)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);

    clean_sercen_white();
    
	while(1)
	{        
	    GUI_handle();
	    
        LOG_PRINT(DEBUG_TASK,"APP_Display \r\n");
        //vTaskDelay(sMaxBlockTime);
	}
}

