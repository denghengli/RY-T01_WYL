#include "includes.h"

#define UI_STATE_ARR_MAX 25

#define KEY_NULL    0x00
#define KEY_UP      0x01
#define KEY_DOWN    0x02
#define KEY_RIGHT   0x03
#define KEY_OK      0x04
#define KEY_RETURN  0x05

volatile uint8_t ex_sec_signal = 0;
volatile uint8_t ui_pre_state = STATE_NULL;
volatile uint8_t ui_cur_state = MAIN_WIND;
uint8_t cur_main_pagenum = 1; //������ҳ��

const char *set_menu[] = 
{
	"�豸״̬    ",
	"�ֶ�����    ",
	"�������У׼",		
	"ƽ��ʱ��    ",		
	"�̵������  ",		
    "����Ƶ��    ",	
    "��������    ",	
    "            ",
    "            ",
    "            ",
};

const UI_STATE_TRANS UIStateArray[UI_STATE_ARR_MAX] =
{
    /*��ǰ      �ϼ�      �¼�       �Ҽ�       OK��   ���ؼ�*/
    {START_LOGO, START_LOGO, START_LOGO, START_LOGO, START_LOGO, START_LOGO}, 
    {MAIN_WIND,  MAIN_WIND, MAIN_WIND,  MAIN_WIND_RIGHT,  CONFIG_MENU_ONE_SELECT_1,  MAIN_WIND},

    //���õ������˵���
    {CONFIG_MENU_ONE_SELECT_1,  CONFIG_MENU_ONE_SELECT_1, CONFIG_MENU_ONE_SELECT_2,  CONFIG_MENU_ONE_SELECT_1,  CONFIG_MENU_ONE_SELECT_1,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_2,  CONFIG_MENU_ONE_SELECT_1, CONFIG_MENU_ONE_SELECT_3,  CONFIG_MENU_ONE_SELECT_2,  CONFIG_MENU_ONE_SELECT_2,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_3,  CONFIG_MENU_ONE_SELECT_2, CONFIG_MENU_ONE_SELECT_4,  CONFIG_MENU_ONE_SELECT_3,  CONFIG_MENU_ONE_SELECT_3,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_4,  CONFIG_MENU_ONE_SELECT_3, CONFIG_MENU_ONE_SELECT_5,  CONFIG_MENU_ONE_SELECT_4,  CONFIG_MENU_ONE_SELECT_4,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_5,  CONFIG_MENU_ONE_SELECT_4, CONFIG_MENU_TWO_SELECT_1,  CONFIG_MENU_ONE_SELECT_5,  CONFIG_MENU_ONE_SELECT_5,  MAIN_WIND},

    {CONFIG_MENU_TWO_SELECT_1,  CONFIG_MENU_ONE_SELECT_5, CONFIG_MENU_TWO_SELECT_2,  CONFIG_MENU_ONE_SELECT_1,  CONFIG_MENU_TWO_SELECT_1,  MAIN_WIND},
    {CONFIG_MENU_TWO_SELECT_2,  CONFIG_MENU_TWO_SELECT_1, CONFIG_MENU_TWO_SELECT_2,  CONFIG_MENU_TWO_SELECT_2,  CONFIG_MENU_TWO_SELECT_2,  MAIN_WIND},
};

const state_fun MenuFun[] = 
{
    NULL,

    start_logo,
    main_wind,
    main_wind_right,
    
    config_menu_one_select_1,
    config_menu_one_select_2,
    config_menu_one_select_3,
    config_menu_one_select_4,
    config_menu_one_select_5,
    config_menu_two_select_1,
    config_menu_two_select_2,
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
    static int sec_display_flag = 1;

    if (ex_sec_signal != sec_display_flag)    
    {
        sec_display_flag = ex_sec_signal;
        
        if (cur_main_pagenum == 1)
        {
            /*�¶�*/
            lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
            lcd_para.x = 20;
            lcd_para.y = 20;
            lcd_para.bc = DARKBLUE;
            lcd_para.fc = WHITE;
            lcd_para.mode = 24;
            snprintf(tmp_str, sizeof(tmp_str), "  �¶ȣ� %7.2f  ��", g_SysData.Data.Sample.ptTem);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

            /*ʪ��*/
            lcd_para.y = 60;
            snprintf(tmp_str, sizeof(tmp_str), "  ʪ�ȣ� %7.2f  %%V", g_SysData.Data.Sample.humit);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

            /*����*/
            lcd_para.y = 100;
            snprintf(tmp_str, sizeof(tmp_str), "  ���٣� %7.2f  m/s", g_SysData.Data.Sample.speed);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

            /*����*/
            lcd_para.y = 140;
            snprintf(tmp_str, sizeof(tmp_str), "  ������ %7.2f  m3/s", g_SysData.Data.Sample.flow);
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
            snprintf(tmp_str, sizeof(tmp_str), "  ��ѹ�� %7.2f  Pa", g_SysData.Data.Sample.sticPress / 1000.0);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

            /*��ѹ*/
            lcd_para.y = 60;
            snprintf(tmp_str, sizeof(tmp_str), "  ��ѹ�� %7.2f  Pa", g_SysData.Data.Sample.dynPress);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

            /*����*/
            lcd_para.y = 100;
            snprintf(tmp_str, sizeof(tmp_str), "  ������ %7.2f  KPa", g_SysData.Data.Sample.blowGasPress / 1000.0);
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
}

void main_wind_right(void *para)
{
	if (cur_main_pagenum == 1) cur_main_pagenum = 2;
    else if (cur_main_pagenum == 2) cur_main_pagenum = 1;
    
	ui_cur_state = MAIN_WIND;
}

int read_key_value(void)
{
    int key = KEY_NULL;
    int iostate = 0;
    
    //���º��ǵ͵�ƽ��Ч
    iostate = !DRV_Pin_Read(epin_KEY4) << 4 |
              !DRV_Pin_Read(epin_KEY3) << 3 |
              !DRV_Pin_Read(epin_KEY2) << 2 |
              !DRV_Pin_Read(epin_KEY1) << 1 |
              !DRV_Pin_Read(epin_KEY0);
    //��������
    if (iostate)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);//��������
        iostate = !DRV_Pin_Read(epin_KEY4) << 4 |
              !DRV_Pin_Read(epin_KEY3) << 3 |
              !DRV_Pin_Read(epin_KEY2) << 2 |
              !DRV_Pin_Read(epin_KEY1) << 1 |
              !DRV_Pin_Read(epin_KEY0);
        
        if (iostate & 0x10)
        {
            key = KEY_UP;
        }
        else if (iostate & 0x08)
        {
            key = KEY_DOWN;
        }
        else if (iostate & 0x04)
        {
            key = KEY_RIGHT;
        }
        else if (iostate & 0x02)
        {
            key = KEY_OK;
        }
        else if (iostate & 0x01)
        {
            key = KEY_RETURN;
        }
    }

    return key;
}


void config_menu(void *para)
{
	unsigned int set_pos = 0;
	unsigned int page = 0;
	unsigned int i = 0;
	unsigned char buf[8] = {0};
	char tmp_str[64] = {0};
	FNC_LCD_DISP_DRAW_PARA lcd_para;	
	set_pos = *(unsigned int *)para  % 5;
	page = *(unsigned int *)para / 5;
	
	/*title*/
	lcd_para.cmd = FNC_LCD_DISP_DRAW_LINE;
	lcd_para.bc = DARKBLUE;
	lcd_para.fc = RED;
	lcd_para.mode = 24;
	lcd_para.x = LCD_W / 2 - 40 - 40;
	lcd_para.y = 17;
	buf[0] = LCD_W / 2 - 40;
	buf[1] = 0;
	buf[2] = 17;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);
	lcd_para.x = LCD_W / 2 - 40 - 40;
	lcd_para.y = 18;
	buf[0] = LCD_W / 2 - 40;
	buf[1] = 0;
	buf[2] = 18;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);
	lcd_para.x = (LCD_W / 2) + 40;
	lcd_para.y = 17;
	buf[0] = (unsigned char)((LCD_W / 2) + 40 + 40);
	buf[1] = 0;
	buf[2] = 17;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);
	lcd_para.x = LCD_W / 2 + 40;
	lcd_para.y = 18;
	buf[0] = (unsigned char)(LCD_W / 2 + 40 + 40);
	buf[1] = 0;
	buf[2] = 18;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);
	lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
	lcd_para.bc = DARKBLUE;
	lcd_para.fc = YELLOW;
	lcd_para.mode = 24;
	lcd_para.x = 16;
	lcd_para.y = 0;
	lcd_para.x = LCD_W / 2 - 24; 
	lcd_para.y = 5;
	sprintf(tmp_str, "����");
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

	/*������Ŀ*/	
	for (i = 0; i < 5; i++)
	{				
		lcd_para.y = (i + 1) * (LCD_H / 6);			
		/*ѡ��*/
		if (i == set_pos)
		{
		    lcd_para.x = 48;
		    lcd_para.bc = DARKBLUE;			
			lcd_para.fc = WHITE;			
		    sprintf(tmp_str, "�� ");
		    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

		    lcd_para.x = 84;
			lcd_para.fc = DARKBLUE;			
			lcd_para.bc = WHITE;
			sprintf(tmp_str, "%s", set_menu[page * 5 + i]);
		}	
		else
		{
		    lcd_para.x = 48;
		    lcd_para.bc = DARKBLUE;			
			lcd_para.fc = WHITE;			
		    sprintf(tmp_str, "  ");
		    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
		    
		    lcd_para.x = 84;			
		    sprintf(tmp_str, "%s", set_menu[page * 5 + i]);
		}
		hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
	}
}


void config_menu_one_select_1(void *para)
{
	unsigned int i = 0;
	config_menu(&i);    
}

void config_menu_one_select_2(void *para)  
{
    unsigned int i = 1;
	config_menu(&i);
}

void config_menu_one_select_3(void *para)
{
    unsigned int i = 2;
	config_menu(&i);    
}

void config_menu_one_select_4(void *para)
{
    unsigned int i = 3;
	config_menu(&i);    
}

void config_menu_one_select_5(void *para)
{
    unsigned int i = 4;
	config_menu(&i);    
}

void config_menu_two_select_1(void *para)
{
    unsigned int i = 5;
	config_menu(&i);    
}

void config_menu_two_select_2(void *para)
{
    unsigned int i = 6;
	config_menu(&i);    
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
    
    clean_sercen();
    
	while(1)
	{        
	    GUI_handle();
        
        LOG_PRINT(DEBUG_TASK,"APP_Display \r\n");
        //vTaskDelay(sMaxBlockTime);
	}
}

