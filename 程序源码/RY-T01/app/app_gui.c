#include "includes.h"

#define UI_STATE_ARR_MAX 50

#define KEY_NULL    0x00
#define KEY_UP      0x01
#define KEY_DOWN    0x02
#define KEY_RIGHT   0x03
#define KEY_OK      0x04
#define KEY_RETURN  0x05

volatile uint8_t ex_sec_signal = 0;
volatile uint8_t ui_pre_state = STATE_NULL;
volatile uint8_t ui_cur_state = MAIN_WIND;

const UI_STATE_TRANS UIStateArray[UI_STATE_ARR_MAX] =
{
    /*��ǰ      �ϼ�      �¼�       �Ҽ�       OK��   ���ؼ�*/
    {START_LOGO, START_LOGO, START_LOGO, START_LOGO, START_LOGO, START_LOGO}, 
    //{MAIN_WIND,  MAIN_WIND, MAIN_WIND,  MAIN_WIND_RIGHT,  CONFIG_MENU_ONE_SELECT_1,  MAIN_WIND},
    {MAIN_WIND,  MAIN_WIND, MAIN_WIND,  MAIN_WIND_RIGHT,  PARA_SET_ONE_SELECT_1,  MAIN_WIND},

    //���õ������˵���
    {CONFIG_MENU_ONE_SELECT_1,  CONFIG_MENU_ONE_SELECT_1, CONFIG_MENU_ONE_SELECT_2,  CONFIG_MENU_ONE_SELECT_1,  CONFIG_MENU_ONE_SELECT_1,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_2,  CONFIG_MENU_ONE_SELECT_1, CONFIG_MENU_ONE_SELECT_3,  CONFIG_MENU_ONE_SELECT_2,  CONFIG_MENU_ONE_SELECT_2,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_3,  CONFIG_MENU_ONE_SELECT_2, CONFIG_MENU_ONE_SELECT_4,  CONFIG_MENU_ONE_SELECT_3,  CONFIG_MENU_ONE_SELECT_3,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_4,  CONFIG_MENU_ONE_SELECT_3, CONFIG_MENU_ONE_SELECT_5,  CONFIG_MENU_ONE_SELECT_4,  SMOOTH_TIME_SET_ENTER,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_5,  CONFIG_MENU_ONE_SELECT_4, CONFIG_MENU_TWO_SELECT_1,  CONFIG_MENU_ONE_SELECT_5,  SECTION_AREA_SET_ENTER,  MAIN_WIND},
    {CONFIG_MENU_TWO_SELECT_1,  CONFIG_MENU_ONE_SELECT_5, CONFIG_MENU_TWO_SELECT_2,  CONFIG_MENU_ONE_SELECT_1,  BLOW_FREQ_SET_ENTER,  MAIN_WIND},
    {CONFIG_MENU_TWO_SELECT_2,  CONFIG_MENU_TWO_SELECT_1, CONFIG_MENU_TWO_SELECT_2,  CONFIG_MENU_TWO_SELECT_2,  PASSWORD_LOGIN_ENTER,  MAIN_WIND},

    //ƽ��ʱ�����ý���
    {SMOOTH_TIME_SET,  SMOOTH_TIME_SET_UP, SMOOTH_TIME_SET_DOWN,  SMOOTH_TIME_SET_RIGHT,  SMOOTH_TIME_SET_OK,  SMOOTH_TIME_SET_RETURN},
    {SMOOTH_TIME_SET_OK,  SMOOTH_TIME_SET_OK, SMOOTH_TIME_SET_OK,  SMOOTH_TIME_SET_OK,  SMOOTH_TIME_SET_OK_RETURN,  SMOOTH_TIME_SET_OK_RETURN},

    //�̵���������ý���
    {SECTION_AREA_SET,  SECTION_AREA_SET_UP, SECTION_AREA_SET_DOWN,  SECTION_AREA_SET_RIGHT,  SECTION_AREA_SET_OK,  SECTION_AREA_SET_RETURN},
    {SECTION_AREA_SET_OK,  SECTION_AREA_SET_OK, SECTION_AREA_SET_OK,  SECTION_AREA_SET_OK,  SECTION_AREA_SET_OK_RETURN,  SECTION_AREA_SET_OK_RETURN},

    //����Ƶ�����ý���
    {BLOW_FREQ_SET,  BLOW_FREQ_SET_UP, BLOW_FREQ_SET_DOWN,  BLOW_FREQ_SET_RIGHT,  BLOW_FREQ_SET_OK,  BLOW_FREQ_SET_RETURN},
    {BLOW_FREQ_SET_OK,  BLOW_FREQ_SET_OK, BLOW_FREQ_SET_OK,  BLOW_FREQ_SET_OK,  BLOW_FREQ_SET_OK_RETURN,  BLOW_FREQ_SET_OK_RETURN},

    //�������������¼����
    {PASSWORD_LOGIN,  PASSWORD_LOGIN_UP, PASSWORD_LOGIN_DOWN,  PASSWORD_LOGIN_RIGHT,  PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_RETURN},
    {PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_OK, PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_ERROR_RETURN},

    //�˵�����
    {PARA_SET_ONE_SELECT_1,  PARA_SET_ONE_SELECT_1, PARA_SET_ONE_SELECT_2,  PARA_SET_ONE_SELECT_1,  BLOW_INTER_SET_ENTER,  CONFIG_MENU_TWO_SELECT_2},
    {PARA_SET_ONE_SELECT_2,  PARA_SET_ONE_SELECT_1, PARA_SET_ONE_SELECT_3,  PARA_SET_ONE_SELECT_2,  AUTO_CALIB_ZERO_ENTER,  CONFIG_MENU_TWO_SELECT_2},
    {PARA_SET_ONE_SELECT_3,  PARA_SET_ONE_SELECT_2, PARA_SET_ONE_SELECT_4,  PARA_SET_ONE_SELECT_3,  SPEED_K_SET_ENTER,  CONFIG_MENU_TWO_SELECT_2},
    {PARA_SET_ONE_SELECT_4,  PARA_SET_ONE_SELECT_3, PARA_SET_ONE_SELECT_5,  PARA_SET_ONE_SELECT_4,  PITG_K_SET_ENTER,  CONFIG_MENU_TWO_SELECT_2},
    {PARA_SET_ONE_SELECT_5,  PARA_SET_ONE_SELECT_4, PARA_SET_TWO_SELECT_1,  PARA_SET_ONE_SELECT_5,  HUMIT_ZERO_CALIB_ENTER,  CONFIG_MENU_TWO_SELECT_2},
    {PARA_SET_TWO_SELECT_1,  PARA_SET_ONE_SELECT_5, PARA_SET_TWO_SELECT_2,  PARA_SET_ONE_SELECT_1,  CANCEL_HUMIT_ZERO_ENTER,  CONFIG_MENU_TWO_SELECT_2},
    {PARA_SET_TWO_SELECT_2,  PARA_SET_TWO_SELECT_1, PARA_SET_TWO_SELECT_3,  PARA_SET_TWO_SELECT_2,  HUMIT_UNIT_SELECT_ENTER,  CONFIG_MENU_TWO_SELECT_2},
    {PARA_SET_TWO_SELECT_3,  PARA_SET_TWO_SELECT_2, PARA_SET_TWO_SELECT_3,  PARA_SET_TWO_SELECT_3,  RESTORE_FACTORY_ENTER,  CONFIG_MENU_TWO_SELECT_2},

    //�����������
    {BLOW_INTER_SET,  BLOW_INTER_SET_UP, BLOW_INTER_SET_DOWN,  BLOW_INTER_SET_RIGHT,  BLOW_INTER_SET_OK,  BLOW_INTER_SET_RETURN},
    {BLOW_INTER_SET_OK,  BLOW_INTER_SET_OK, BLOW_INTER_SET_OK,  BLOW_INTER_SET_OK,  BLOW_INTER_SET_OK_RETURN,  BLOW_INTER_SET_OK_RETURN},

    //�Զ�У�����
    {AUTO_CALIB_ZERO,  AUTO_CALIB_ZERO, AUTO_CALIB_ZERO,  AUTO_CALIB_ZERO_RIGHT,  AUTO_CALIB_ZERO_OK,  AUTO_CALIB_ZERO_RETURN},
    {AUTO_CALIB_ZERO_OK,  AUTO_CALIB_ZERO_OK, AUTO_CALIB_ZERO_OK,  AUTO_CALIB_ZERO_OK,  AUTO_CALIB_ZERO_OK_RETURN,  AUTO_CALIB_ZERO_OK_RETURN},
    
    //�ٶȳ�ϵ��������У׼ϵ�������ý���
    {SPEED_K_SET,  SPEED_K_SET_UP, SPEED_K_SET_DOWN,  SPEED_K_SET_RIGHT,  SPEED_K_SET_OK,  SPEED_K_SET_RETURN},
    {SPEED_K_SET_OK,  SPEED_K_SET_OK, SPEED_K_SET_OK,  SPEED_K_SET_OK,  SPEED_K_SET_OK_RETURN,  SPEED_K_SET_OK_RETURN},

    //Ƥ�й�ϵ�����ý���
    {PITG_K_SET,  PITG_K_SET_UP, PITG_K_SET_DOWN,  PITG_K_SET_RIGHT,  PITG_K_SET_OK,  PITG_K_SET_RETURN},
    {PITG_K_SET_OK,  PITG_K_SET_OK, PITG_K_SET_OK,  PITG_K_SET_OK,  PITG_K_SET_OK_RETURN,  PITG_K_SET_OK_RETURN},
	
	//ʪ�����У׼
	{HUMIT_ZERO_CALIB,  HUMIT_ZERO_CALIB, HUMIT_ZERO_CALIB,  HUMIT_ZERO_CALIB,  HUMIT_ZERO_CALIB_OK,  HUMIT_ZERO_CALIB_RETURN},
	{HUMIT_ZERO_CALIB_FINISH,  HUMIT_ZERO_CALIB_FINISH, HUMIT_ZERO_CALIB_FINISH,  HUMIT_ZERO_CALIB_FINISH,  HUMIT_ZERO_CALIB_FINISH_RETURN,  HUMIT_ZERO_CALIB_FINISH_RETURN},
	
	//ȡ��ʪ��У��ֵ
	{CANCEL_HUMIT_ZERO,  CANCEL_HUMIT_ZERO, CANCEL_HUMIT_ZERO,  CANCEL_HUMIT_ZERO_RIGHT,  CANCEL_HUMIT_ZERO_OK,  CANCEL_HUMIT_ZERO_RETURN},
	{CANCEL_HUMIT_ZERO_OK,  CANCEL_HUMIT_ZERO_OK, CANCEL_HUMIT_ZERO_OK,  CANCEL_HUMIT_ZERO_OK,  CANCEL_HUMIT_ZERO_OK_RETURN,  CANCEL_HUMIT_ZERO_OK_RETURN},
	
	//ʪ�ȿ���
	{HUMIT_UNIT_SELECT,  HUMIT_UNIT_SELECT_UP, HUMIT_UNIT_SELECT_DOWN,  HUMIT_UNIT_SELECT,  HUMIT_UNIT_SELECT_OK,  HUMIT_UNIT_SELECT_RETURN},
	{HUMIT_UNIT_SELECT_OK,  HUMIT_UNIT_SELECT_OK, HUMIT_UNIT_SELECT_OK,  HUMIT_UNIT_SELECT_OK,  HUMIT_UNIT_SELECT_OK_RETURN,  HUMIT_UNIT_SELECT_OK_RETURN},
	
	//�ָ���������
    {RESTORE_FACTORY,  RESTORE_FACTORY, RESTORE_FACTORY,  RESTORE_FACTORY_RIGHT,  RESTORE_FACTORY_OK,  RESTORE_FACTORY_RETURN},
    {RESTORE_FACTORY_OK,  RESTORE_FACTORY_OK, RESTORE_FACTORY_OK,  RESTORE_FACTORY_OK,  RESTORE_FACTORY_OK_RETURN,  RESTORE_FACTORY_OK_RETURN}
};

const state_fun MenuFun[] = 
{
    NULL,

    start_logo,
    main_wind,
    main_wind_right,

    //���õ������˵���
    config_menu_one_select_1,
    config_menu_one_select_2,
    config_menu_one_select_3,
    config_menu_one_select_4,
    config_menu_one_select_5,
    config_menu_two_select_1,
    config_menu_two_select_2,

    //ƽ��ʱ�����ý���
    smooth_time_set_enter,
    smooth_time_set,
    smooth_time_set_up,
    smooth_time_set_down,
    smooth_time_set_right,
    smooth_time_set_return,
    smooth_time_set_ok,    
    smooth_time_set_ok_return,

    //�̵���������ý���
    section_area_set_enter,
    section_area_set,
    section_area_set_up,
    section_area_set_down,
    section_area_set_right,
    section_area_set_return,
    section_area_set_ok,    
    section_area_set_ok_return,
	
	//����Ƶ�����ý���
	blow_freq_set_enter,
	blow_freq_set,
	blow_freq_set_up,
	blow_freq_set_down,
	blow_freq_set_right,
	blow_freq_set_return,
	blow_freq_set_ok,    
	blow_freq_set_ok_return,

    //�������������¼����
	password_login_enter,
	password_login,
	password_login_up,
	password_login_down,
	password_login_right,
	password_login_return,
	password_login_ok,    
	password_login_error_return,

    //���õ������˵���
    para_set_one_select_1,
    para_set_one_select_2,
    para_set_one_select_3,
    para_set_one_select_4,
    para_set_one_select_5,
    para_set_two_select_1,
    para_set_two_select_2,	
    para_set_two_select_3,	

	//����������ý���
	blow_inter_set_enter,
	blow_inter_set,
	blow_inter_set_up,
	blow_inter_set_down,
	blow_inter_set_right,
	blow_inter_set_return,
	blow_inter_set_ok,    
	blow_inter_set_ok_return, 
    
	//�Զ�У�����
	auto_calib_zero_enter,
	auto_calib_zero,
	auto_calib_zero_right,
	auto_calib_zero_return,
	auto_calib_zero_ok,
	auto_calib_zero_ok_return,

    //�ٶȳ�ϵ��������У׼ϵ�������ý���
    speed_k_set_enter,
    speed_k_set,
    speed_k_set_up,
    speed_k_set_down,
    speed_k_set_right,
    speed_k_set_return,
    speed_k_set_ok,    
    speed_k_set_ok_return,

    //Ƥ�й�ϵ�����ý���
    pitg_k_set_enter,
    pitg_k_set,
    pitg_k_set_up,
    pitg_k_set_down,
    pitg_k_set_right,
    pitg_k_set_return,
    pitg_k_set_ok,    
    pitg_k_set_ok_return,
	
	//ʪ�����У׼
	humit_zero_calib_enter,
	humit_zero_calib,
	humit_zero_calib_return,
	humit_zero_calib_ok,
	humit_zero_calib_finish,
	humit_zero_calib_finish_return,

	//ȡ��ʪ��У��ֵ
	cancel_humit_zero_enter,
	cancel_humit_zero,
	cancel_humit_zero_right,
	cancel_humit_zero_return,
	cancel_humit_zero_ok,
	cancel_humit_zero_ok_return,

	//ʪ�ȿ���
	humit_unit_select_enter,
	humit_unit_select,
	humit_unit_select_up,
	humit_unit_select_down,
	humit_unit_select_return,
	humit_unit_select_ok,
	humit_unit_select_ok_return,

	//�ָ���������
	restore_factory_enter,
	restore_factory,
	restore_factory_right,
	restore_factory_return,
	restore_factory_ok,
	restore_factory_ok_return
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
        vTaskDelay(100 / portTICK_PERIOD_MS);//��������
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

