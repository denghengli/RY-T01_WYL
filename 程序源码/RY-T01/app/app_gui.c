#include "includes.h"

#define KEY_NULL    0x00
#define KEY_UP      0x01
#define KEY_DOWN    0x02
#define KEY_RIGHT   0x03
#define KEY_OK      0x04
#define KEY_RETURN  0x05

volatile uint8_t ex_sec_signal = 0;
volatile uint8_t ui_pre_state = STATE_NULL;
volatile uint8_t ui_cur_state = MAIN_WIND;
uint8_t ui_user = 1; //用户等级，1:普通用户 2:高级用户

const UI_STATE_TRANS UIStateArray[] =
{
    /*当前      上键      下键       右键       OK键   返回键*/
    {START_LOGO, START_LOGO, START_LOGO, START_LOGO, START_LOGO, START_LOGO}, 
    {MAIN_WIND,  MAIN_WIND, MAIN_WIND,  MAIN_WIND_RIGHT,  CONFIG_MENU_ONE_SELECT_1,  MAIN_WIND},
    //{MAIN_WIND,  MAIN_WIND, MAIN_WIND,  MAIN_WIND_RIGHT,  PARA_SET_ONE_SELECT_1,  MAIN_WIND},

    //配置导航（菜单）
    {CONFIG_MENU_ONE_SELECT_1,  CONFIG_MENU_ONE_SELECT_1, CONFIG_MENU_ONE_SELECT_2,  CONFIG_MENU_ONE_SELECT_1,  HANDLE_BLOW_ENTER,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_2,  CONFIG_MENU_ONE_SELECT_1, CONFIG_MENU_ONE_SELECT_3,  CONFIG_MENU_ONE_SELECT_2,  FLOW_ADJUST_ENTER,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_3,  CONFIG_MENU_ONE_SELECT_2, CONFIG_MENU_ONE_SELECT_4,  CONFIG_MENU_ONE_SELECT_3,  SMOOTH_TIME_SET_ENTER,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_4,  CONFIG_MENU_ONE_SELECT_3, CONFIG_MENU_ONE_SELECT_5,  CONFIG_MENU_ONE_SELECT_4,  SECTION_AREA_SET_ENTER,  MAIN_WIND},
    {CONFIG_MENU_ONE_SELECT_5,  CONFIG_MENU_ONE_SELECT_4, CONFIG_MENU_TWO_SELECT_1,  CONFIG_MENU_ONE_SELECT_5,  BLOW_FREQ_SET_ENTER,  MAIN_WIND},
    {CONFIG_MENU_TWO_SELECT_1,  CONFIG_MENU_ONE_SELECT_5, CONFIG_MENU_TWO_SELECT_1,  CONFIG_MENU_TWO_SELECT_1,  PASSWORD_LOGIN_ENTER,  MAIN_WIND},
    //{CONFIG_MENU_TWO_SELECT_1,  CONFIG_MENU_ONE_SELECT_5, CONFIG_MENU_TWO_SELECT_1,  CONFIG_MENU_TWO_SELECT_1,  PARA_SET_ONE_SELECT_1,  MAIN_WIND},

    //手动反吹界面
    {HANDLE_BLOW,  HANDLE_BLOW_UP, HANDLE_BLOW_DOWN,  HANDLE_BLOW,  HANDLE_BLOW_OK,  HANDLE_BLOW_RETURN},
    {HANDLE_BLOW_ING,  HANDLE_BLOW_ING, HANDLE_BLOW_ING,  HANDLE_BLOW_ING,  HANDLE_BLOW_ING,  HANDLE_BLOW_ING_RETURN},
	{HANDLE_BLOW_FINISH,  HANDLE_BLOW_FINISH, HANDLE_BLOW_FINISH,  HANDLE_BLOW_FINISH,  HANDLE_BLOW_FINISH_RETURN,  HANDLE_BLOW_FINISH_RETURN},
	
    //流速零点校准界面
    {FLOW_ADJUST,  FLOW_ADJUST_UP, FLOW_ADJUST_DOWN,  FLOW_ADJUST,  FLOW_ADJUST_OK,  FLOW_ADJUST_RETURN},
    {FLOW_ADJUST_ING,  FLOW_ADJUST_ING, FLOW_ADJUST_ING,  FLOW_ADJUST_ING,  FLOW_ADJUST_ING,  FLOW_ADJUST_ING_RETURN},
	{FLOW_ADJUST_FINISH,  FLOW_ADJUST_FINISH, FLOW_ADJUST_FINISH,  FLOW_ADJUST_FINISH,  FLOW_ADJUST_FINISH_RETURN,  FLOW_ADJUST_FINISH_RETURN},
	
    //平滑时间设置界面
    {SMOOTH_TIME_SET,  SMOOTH_TIME_SET_UP, SMOOTH_TIME_SET_DOWN,  SMOOTH_TIME_SET_RIGHT,  SMOOTH_TIME_SET_OK,  SMOOTH_TIME_SET_RETURN},
    {SMOOTH_TIME_SET_OK,  SMOOTH_TIME_SET_OK, SMOOTH_TIME_SET_OK,  SMOOTH_TIME_SET_OK,  SMOOTH_TIME_SET_OK_RETURN,  SMOOTH_TIME_SET_OK_RETURN},

    //烟道截面积设置界面
    {SECTION_AREA_SET,  SECTION_AREA_SET_UP, SECTION_AREA_SET_DOWN,  SECTION_AREA_SET_RIGHT,  SECTION_AREA_SET_OK,  SECTION_AREA_SET_RETURN},
    {SECTION_AREA_SET_OK,  SECTION_AREA_SET_OK, SECTION_AREA_SET_OK,  SECTION_AREA_SET_OK,  SECTION_AREA_SET_OK_RETURN,  SECTION_AREA_SET_OK_RETURN},

    //反吹频次设置界面
    {BLOW_FREQ_SET,  BLOW_FREQ_SET_UP, BLOW_FREQ_SET_DOWN,  BLOW_FREQ_SET_RIGHT,  BLOW_FREQ_SET_OK,  BLOW_FREQ_SET_RETURN},
    {BLOW_FREQ_SET_OK,  BLOW_FREQ_SET_OK, BLOW_FREQ_SET_OK,  BLOW_FREQ_SET_OK,  BLOW_FREQ_SET_OK_RETURN,  BLOW_FREQ_SET_OK_RETURN},

    //参数设置密码登录界面
    {PASSWORD_LOGIN,  PASSWORD_LOGIN_UP, PASSWORD_LOGIN_DOWN,  PASSWORD_LOGIN_RIGHT,  PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_RETURN},
    {PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_OK, PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_OK,  PASSWORD_LOGIN_ERROR_RETURN},

    //参数设置菜单
    {PARA_SET_ONE_SELECT_1,  PARA_SET_ONE_SELECT_1, PARA_SET_ONE_SELECT_2,  PARA_SET_ONE_SELECT_1,  BLOW_INTER_SET_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_ONE_SELECT_2,  PARA_SET_ONE_SELECT_1, PARA_SET_ONE_SELECT_3,  PARA_SET_ONE_SELECT_2,  AUTO_CALIB_ZERO_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_ONE_SELECT_3,  PARA_SET_ONE_SELECT_2, PARA_SET_ONE_SELECT_4,  PARA_SET_ONE_SELECT_3,  SPEED_K_SET_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_ONE_SELECT_4,  PARA_SET_ONE_SELECT_3, PARA_SET_ONE_SELECT_5,  PARA_SET_ONE_SELECT_4,  PITG_K_SET_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_ONE_SELECT_5,  PARA_SET_ONE_SELECT_4, PARA_SET_TWO_SELECT_1,  PARA_SET_ONE_SELECT_5,  HUMIT_ZERO_CALIB_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_TWO_SELECT_1,  PARA_SET_ONE_SELECT_5, PARA_SET_TWO_SELECT_2,  PARA_SET_ONE_SELECT_1,  CANCEL_HUMIT_ZERO_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_TWO_SELECT_2,  PARA_SET_TWO_SELECT_1, PARA_SET_TWO_SELECT_3,  PARA_SET_TWO_SELECT_2,  HUMIT_UNIT_SELECT_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_TWO_SELECT_3,  PARA_SET_TWO_SELECT_2, PARA_SET_TWO_SELECT_3,  PARA_SET_TWO_SELECT_3,  RESTORE_FACTORY_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    //{PARA_SET_TWO_SELECT_3,  PARA_SET_TWO_SELECT_2, PARA_SET_TWO_SELECT_4,  PARA_SET_TWO_SELECT_3,  RESTORE_FACTORY_ENTER,  CONFIG_MENU_TWO_SELECT_1}, //高级参数设置
    
    {PARA_SET_TWO_SELECT_4,  PARA_SET_TWO_SELECT_3, PARA_SET_TWO_SELECT_5,  PARA_SET_TWO_SELECT_4,  HUMIT_SENSOR_TYPE_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_TWO_SELECT_5,  PARA_SET_TWO_SELECT_4, PARA_SET_THR_SELECT_1,  PARA_SET_TWO_SELECT_5,  TEM_AO_COMP_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_THR_SELECT_1,  PARA_SET_TWO_SELECT_5, PARA_SET_THR_SELECT_2,  PARA_SET_THR_SELECT_1,  PRESS_AO_COMP_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_THR_SELECT_2,  PARA_SET_THR_SELECT_1, PARA_SET_THR_SELECT_3,  PARA_SET_THR_SELECT_2,  SPEED_AO_COMP_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    {PARA_SET_THR_SELECT_3,  PARA_SET_THR_SELECT_2, PARA_SET_THR_SELECT_3,  PARA_SET_THR_SELECT_3,  HUMIT_AO_COMP_ENTER,  CONFIG_MENU_TWO_SELECT_1},
    
    //反吹间隔界面
    {BLOW_INTER_SET,  BLOW_INTER_SET_UP, BLOW_INTER_SET_DOWN,  BLOW_INTER_SET_RIGHT,  BLOW_INTER_SET_OK,  BLOW_INTER_SET_RETURN},
    {BLOW_INTER_SET_OK,  BLOW_INTER_SET_OK, BLOW_INTER_SET_OK,  BLOW_INTER_SET_OK,  BLOW_INTER_SET_OK_RETURN,  BLOW_INTER_SET_OK_RETURN},

    //自动校零控制
    {AUTO_CALIB_ZERO,  AUTO_CALIB_ZERO, AUTO_CALIB_ZERO,  AUTO_CALIB_ZERO_RIGHT,  AUTO_CALIB_ZERO_OK,  AUTO_CALIB_ZERO_RETURN},
    {AUTO_CALIB_ZERO_OK,  AUTO_CALIB_ZERO_OK, AUTO_CALIB_ZERO_OK,  AUTO_CALIB_ZERO_OK,  AUTO_CALIB_ZERO_OK_RETURN,  AUTO_CALIB_ZERO_OK_RETURN},
    
    //速度场系数（风速校准系数）设置界面
    {SPEED_K_SET,  SPEED_K_SET_UP, SPEED_K_SET_DOWN,  SPEED_K_SET_RIGHT,  SPEED_K_SET_OK,  SPEED_K_SET_RETURN},
    {SPEED_K_SET_OK,  SPEED_K_SET_OK, SPEED_K_SET_OK,  SPEED_K_SET_OK,  SPEED_K_SET_OK_RETURN,  SPEED_K_SET_OK_RETURN},

    //皮托管系数设置界面
    {PITG_K_SET,  PITG_K_SET_UP, PITG_K_SET_DOWN,  PITG_K_SET_RIGHT,  PITG_K_SET_OK,  PITG_K_SET_RETURN},
    {PITG_K_SET_OK,  PITG_K_SET_OK, PITG_K_SET_OK,  PITG_K_SET_OK,  PITG_K_SET_OK_RETURN,  PITG_K_SET_OK_RETURN},
	
	//湿度零点校准
	{HUMIT_ZERO_CALIB,  HUMIT_ZERO_CALIB, HUMIT_ZERO_CALIB,  HUMIT_ZERO_CALIB,  HUMIT_ZERO_CALIB_OK,  HUMIT_ZERO_CALIB_RETURN},
	{HUMIT_ZERO_CALIB_FINISH,  HUMIT_ZERO_CALIB_FINISH, HUMIT_ZERO_CALIB_FINISH,  HUMIT_ZERO_CALIB_FINISH,  HUMIT_ZERO_CALIB_FINISH_RETURN,  HUMIT_ZERO_CALIB_FINISH_RETURN},
	
	//取消湿度校零值
	{CANCEL_HUMIT_ZERO,  CANCEL_HUMIT_ZERO, CANCEL_HUMIT_ZERO,  CANCEL_HUMIT_ZERO_RIGHT,  CANCEL_HUMIT_ZERO_OK,  CANCEL_HUMIT_ZERO_RETURN},
	{CANCEL_HUMIT_ZERO_OK,  CANCEL_HUMIT_ZERO_OK, CANCEL_HUMIT_ZERO_OK,  CANCEL_HUMIT_ZERO_OK,  CANCEL_HUMIT_ZERO_OK_RETURN,  CANCEL_HUMIT_ZERO_OK_RETURN},
	
	//湿度控制
	{HUMIT_UNIT_SELECT,  HUMIT_UNIT_SELECT_UP, HUMIT_UNIT_SELECT_DOWN,  HUMIT_UNIT_SELECT,  HUMIT_UNIT_SELECT_OK,  HUMIT_UNIT_SELECT_RETURN},
	{HUMIT_UNIT_SELECT_OK,  HUMIT_UNIT_SELECT_OK, HUMIT_UNIT_SELECT_OK,  HUMIT_UNIT_SELECT_OK,  HUMIT_UNIT_SELECT_OK_RETURN,  HUMIT_UNIT_SELECT_OK_RETURN},
	
	//恢复出厂设置
    {RESTORE_FACTORY,  RESTORE_FACTORY, RESTORE_FACTORY,  RESTORE_FACTORY_RIGHT,  RESTORE_FACTORY_OK,  RESTORE_FACTORY_RETURN},
    {RESTORE_FACTORY_OK,  RESTORE_FACTORY_OK, RESTORE_FACTORY_OK,  RESTORE_FACTORY_OK,  RESTORE_FACTORY_OK_RETURN,  RESTORE_FACTORY_OK_RETURN},

    //湿度传感器类型
    {HUMIT_SENSOR_TYPE,  HUMIT_SENSOR_TYPE_UP, HUMIT_SENSOR_TYPE_DOWN,  HUMIT_SENSOR_TYPE,  HUMIT_SENSOR_TYPE_OK,  HUMIT_SENSOR_TYPE_RETURN},
    {HUMIT_SENSOR_TYPE_OK,  HUMIT_SENSOR_TYPE_OK, HUMIT_SENSOR_TYPE_OK,  HUMIT_SENSOR_TYPE_OK,  HUMIT_SENSOR_TYPE_OK_RETURN,  HUMIT_SENSOR_TYPE_OK_RETURN},

    //温度模拟量补偿
    {TEM_AO_COMP,  TEM_AO_COMP_UP, TEM_AO_COMP_DOWN,  TEM_AO_COMP_RIGHT,  TEM_AO_COMP_OK,  TEM_AO_COMP_RETURN},
    {TEM_AO_COMP_OK,  TEM_AO_COMP_OK, TEM_AO_COMP_OK,  TEM_AO_COMP_OK,  TEM_AO_COMP_OK_RETURN,  TEM_AO_COMP_OK_RETURN},

    //压力模拟量补偿
    {PRESS_AO_COMP,  PRESS_AO_COMP_UP, PRESS_AO_COMP_DOWN,  PRESS_AO_COMP_RIGHT,  PRESS_AO_COMP_OK,  PRESS_AO_COMP_RETURN},
    {PRESS_AO_COMP_OK,  PRESS_AO_COMP_OK, PRESS_AO_COMP_OK,  PRESS_AO_COMP_OK,  PRESS_AO_COMP_OK_RETURN,  PRESS_AO_COMP_OK_RETURN},

    //流速模拟量补偿
    {SPEED_AO_COMP,  SPEED_AO_COMP_UP, SPEED_AO_COMP_DOWN,  SPEED_AO_COMP_RIGHT,  SPEED_AO_COMP_OK,  SPEED_AO_COMP_RETURN},
    {SPEED_AO_COMP_OK,  SPEED_AO_COMP_OK, SPEED_AO_COMP_OK,  SPEED_AO_COMP_OK,  SPEED_AO_COMP_OK_RETURN,  SPEED_AO_COMP_OK_RETURN},

    //湿度模拟量补偿
    {HUMIT_AO_COMP,  HUMIT_AO_COMP_UP, HUMIT_AO_COMP_DOWN,  HUMIT_AO_COMP_RIGHT,  HUMIT_AO_COMP_OK,  HUMIT_AO_COMP_RETURN},
    {HUMIT_AO_COMP_OK,  HUMIT_AO_COMP_OK, HUMIT_AO_COMP_OK,  HUMIT_AO_COMP_OK,  HUMIT_AO_COMP_OK_RETURN,  HUMIT_AO_COMP_OK_RETURN},
};

const state_fun MenuFun[] = 
{
    NULL,

    start_logo,
    main_wind,
    main_wind_right,

    //配置导航（菜单）
    config_menu_one_select_1,
    config_menu_one_select_2,
    config_menu_one_select_3,
    config_menu_one_select_4,
    config_menu_one_select_5,
    config_menu_two_select_1,
    
    //手动反吹界面
    handle_blow_enter,
    handle_blow,
    handle_blow_up,
    handle_blow_down,
    handle_blow_return,
    handle_blow_ok,
    handle_blow_ing,
    handle_blow_ing_return,
    handle_blow_finish,
    handle_blow_finish_return,

    //流速零点校准界面
    flow_adjust_enter,
    flow_adjust,
    flow_adjust_up,
    flow_adjust_down,
    flow_adjust_return,
    flow_adjust_ok,
    flow_adjust_ing,
    flow_adjust_ing_return,
    flow_adjust_finish,
    flow_adjust_finish_return,
	
    //平滑时间设置界面
    smooth_time_set_enter,
    smooth_time_set,
    smooth_time_set_up,
    smooth_time_set_down,
    smooth_time_set_right,
    smooth_time_set_return,
    smooth_time_set_ok,    
    smooth_time_set_ok_return,

    //烟道截面积设置界面
    section_area_set_enter,
    section_area_set,
    section_area_set_up,
    section_area_set_down,
    section_area_set_right,
    section_area_set_return,
    section_area_set_ok,    
    section_area_set_ok_return,
	
	//反吹频次设置界面
	blow_freq_set_enter,
	blow_freq_set,
	blow_freq_set_up,
	blow_freq_set_down,
	blow_freq_set_right,
	blow_freq_set_return,
	blow_freq_set_ok,    
	blow_freq_set_ok_return,

    //参数设置密码登录界面
	password_login_enter,
	password_login,
	password_login_up,
	password_login_down,
	password_login_right,
	password_login_return,
	password_login_ok,    
	password_login_error_return,

    //配置导航（菜单）
    para_set_one_select_1,
    para_set_one_select_2,
    para_set_one_select_3,
    para_set_one_select_4,
    para_set_one_select_5,
    para_set_two_select_1,
    para_set_two_select_2,	
    para_set_two_select_3,	
    para_set_two_select_4,  
    para_set_two_select_5,
    para_set_thr_select_1,
    para_set_thr_select_2,  
    para_set_thr_select_3,

	//反吹间隔设置界面
	blow_inter_set_enter,
	blow_inter_set,
	blow_inter_set_up,
	blow_inter_set_down,
	blow_inter_set_right,
	blow_inter_set_return,
	blow_inter_set_ok,    
	blow_inter_set_ok_return, 
    
	//自动校零控制
	auto_calib_zero_enter,
	auto_calib_zero,
	auto_calib_zero_right,
	auto_calib_zero_return,
	auto_calib_zero_ok,
	auto_calib_zero_ok_return,

    //速度场系数（风速校准系数）设置界面
    speed_k_set_enter,
    speed_k_set,
    speed_k_set_up,
    speed_k_set_down,
    speed_k_set_right,
    speed_k_set_return,
    speed_k_set_ok,    
    speed_k_set_ok_return,

    //皮托管系数设置界面
    pitg_k_set_enter,
    pitg_k_set,
    pitg_k_set_up,
    pitg_k_set_down,
    pitg_k_set_right,
    pitg_k_set_return,
    pitg_k_set_ok,    
    pitg_k_set_ok_return,
	
	//湿度零点校准
	humit_zero_calib_enter,
	humit_zero_calib,
	humit_zero_calib_return,
	humit_zero_calib_ok,
	humit_zero_calib_finish,
	humit_zero_calib_finish_return,

	//取消湿度校零值
	cancel_humit_zero_enter,
	cancel_humit_zero,
	cancel_humit_zero_right,
	cancel_humit_zero_return,
	cancel_humit_zero_ok,
	cancel_humit_zero_ok_return,

	//湿度控制
	humit_unit_select_enter,
	humit_unit_select,
	humit_unit_select_up,
	humit_unit_select_down,
	humit_unit_select_return,
	humit_unit_select_ok,
	humit_unit_select_ok_return,

	//恢复出厂设置
	restore_factory_enter,
	restore_factory,
	restore_factory_right,
	restore_factory_return,
	restore_factory_ok,
	restore_factory_ok_return,

    //湿度传感器类型
    humit_sensor_type_enter,
    humit_sensor_type,
    humit_sensor_type_up,
    humit_sensor_type_down,
    humit_sensor_type_return,
    humit_sensor_type_ok,
    humit_sensor_type_ok_return,

    //温度20mA输出补偿
    tem_ao_comp_enter,
    tem_ao_comp,
    tem_ao_comp_up,
    tem_ao_comp_down,
    tem_ao_comp_right,
    tem_ao_comp_return,
    tem_ao_comp_ok,    
    tem_ao_comp_ok_return,

    //压力20mA输出补偿
    press_ao_comp_enter,
    press_ao_comp,
    press_ao_comp_up,
    press_ao_comp_down,
    press_ao_comp_right,
    press_ao_comp_return,
    press_ao_comp_ok,    
    press_ao_comp_ok_return,

    //流速20mA输出补偿
    speed_ao_comp_enter,
    speed_ao_comp,
    speed_ao_comp_up,
    speed_ao_comp_down,
    speed_ao_comp_right,
    speed_ao_comp_return,
    speed_ao_comp_ok,    
    speed_ao_comp_ok_return,

    //湿度20mA输出补偿
    humit_ao_comp_enter,
    humit_ao_comp,
    humit_ao_comp_up,
    humit_ao_comp_down,
    humit_ao_comp_right,
    humit_ao_comp_return,
    humit_ao_comp_ok,    
    humit_ao_comp_ok_return,
};

/*清屏 */
void clean_sercen(void)
{
	FNC_LCD_DISP_DRAW_PARA lcd_para;
	char tmp_str[24] = {0};

	lcd_para.cmd = FCN_LCD_DISP_CLEAR;
	lcd_para.bc = DARKBLUE;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

/*清屏 白色*/
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
    
    //按下后是低电平有效
    iostate = !DRV_Pin_Read(epin_KEY4) << 4 |
              !DRV_Pin_Read(epin_KEY3) << 3 |
              !DRV_Pin_Read(epin_KEY2) << 2 |
              !DRV_Pin_Read(epin_KEY1) << 1 |
              !DRV_Pin_Read(epin_KEY0);
    //消除抖动
    if (iostate)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);//消除抖动
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
            key = KEY_OK;
        }
        else if (iostate & 0x04)
        {
            key = KEY_DOWN;
        }
        else if (iostate & 0x02)
        {
            key = KEY_RIGHT;
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
	int ui_num = sizeof(UIStateArray) / sizeof(UIStateArray[0]);
	
    key = read_key_value();
    if (key != KEY_NULL)//有按键按下
    {
        for (uint8_t i=0; i<ui_num; i++)
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
                        if (ui_cur_state == PARA_SET_TWO_SELECT_3 && ui_user == 2)//高级用户
                        {
                            ui_cur_state = UIStateArray[i].down_state + 1;
                        }
                        else
                        {
                            ui_cur_state = UIStateArray[i].down_state;
                        }
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
            clean_flag = is_clean();//判断当前界面是否需要清屏后重绘
            if (clean_flag)
            {
                clean_sercen();
            }
            MenuFun[ui_cur_state](&mpara);
        }
    }

    /* 在没有按键按下的空闲时间，界面还是否需要刷新，主要控制需要闪烁的界面*/
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
*	函 数 名: APP_GUI
*	功能说明: APP_GUI function
*	形    参: argument   数据指针
*	返 回 值: NONE
**********************************************************************************************************/
void APP_GUI(void  * argument)
{
    TickType_t sMaxBlockTime =	pdMS_TO_TICKS(1000);
    
    clean_sercen();
    DRV_Pin_Write(epin_LCD_BLK, PIN_HIGH); //这时候再开背光，不然会花屏
    
	while(1)
	{        
	    GUI_handle();
        
        LOG_PRINT(DEBUG_TASK,"APP_Display \r\n");
        //vTaskDelay(sMaxBlockTime);
	}
}

