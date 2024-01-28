#ifndef __APP_GUI__
#define __APP_GUI__

typedef enum
{
    STATE_NULL = 0,
    START_LOGO = 1,
    MAIN_WIND,
    MAIN_WIND_RIGHT,

    //菜单界面
    CONFIG_MENU_ONE_SELECT_1,
    CONFIG_MENU_ONE_SELECT_2,
    CONFIG_MENU_ONE_SELECT_3,
    CONFIG_MENU_ONE_SELECT_4,
    CONFIG_MENU_ONE_SELECT_5,
    CONFIG_MENU_TWO_SELECT_1,
    CONFIG_MENU_TWO_SELECT_2,

    //平滑时间设置界面
    SMOOTH_TIME_SET_ENTER,
    SMOOTH_TIME_SET,
    SMOOTH_TIME_SET_UP,
    SMOOTH_TIME_SET_DOWN,
    SMOOTH_TIME_SET_RIGHT,
    SMOOTH_TIME_SET_RETURN,
    SMOOTH_TIME_SET_OK,    
    SMOOTH_TIME_SET_OK_RETURN,

    //烟道截面积设置界面
    SECTION_AREA_SET_ENTER,
    SECTION_AREA_SET,
    SECTION_AREA_SET_UP,
    SECTION_AREA_SET_DOWN,
    SECTION_AREA_SET_RIGHT,
    SECTION_AREA_SET_RETURN,
    SECTION_AREA_SET_OK,    
    SECTION_AREA_SET_OK_RETURN,

    //反吹频次设置界面
    BLOW_FREQ_SET_ENTER,
    BLOW_FREQ_SET,
    BLOW_FREQ_SET_UP,
    BLOW_FREQ_SET_DOWN,
    BLOW_FREQ_SET_RIGHT,
    BLOW_FREQ_SET_RETURN,
    BLOW_FREQ_SET_OK,    
    BLOW_FREQ_SET_OK_RETURN,

    //密码登录
    PASSWORD_LOGIN_ENTER,
    PASSWORD_LOGIN,
    PASSWORD_LOGIN_UP,
    PASSWORD_LOGIN_DOWN,
    PASSWORD_LOGIN_RIGHT,
    PASSWORD_LOGIN_RETURN,
    PASSWORD_LOGIN_OK,    
    PASSWORD_LOGIN_ERROR_RETURN,
    
}UISTATE;

typedef struct
{
	UISTATE cur_state;      /*当前状态*/
    
	UISTATE up_state;       /*按下 向上键状态*/
	UISTATE down_state;     /*按下 向下键状态*/
	UISTATE right_state;    /*按下 向右键状态*/
	UISTATE ok_state;       /*按下 OK键状态*/
    UISTATE return_state;   /*按下 返回键状态*/
    
} UI_STATE_TRANS;

extern volatile uint8_t ex_sec_signal;
extern volatile uint8_t ui_pre_state;
extern volatile uint8_t ui_cur_state;

typedef void (*state_fun)(void *para);

uint8_t is_clean(void);
uint8_t is_flash_menu(void);

void start_logo(void *para);
void main_wind(void *para);
void main_wind_right(void *para);

//菜单界面
void config_menu_one_select_1(void *para);
void config_menu_one_select_2(void *para);
void config_menu_one_select_3(void *para);
void config_menu_one_select_4(void *para);
void config_menu_one_select_5(void *para);
void config_menu_two_select_1(void *para);
void config_menu_two_select_2(void *para);

//平滑时间设置界面
void smooth_time_set_enter(void *para);
void smooth_time_set(void *para);
void smooth_time_set_up(void *para);
void smooth_time_set_down(void *para);
void smooth_time_set_right(void *para);
void smooth_time_set_return(void *para);
void smooth_time_set_ok(void *para);
void smooth_time_set_ok_return(void *para);

//烟道截面积设置界面
void section_area_set_enter(void *para);
void section_area_set(void *para);
void section_area_set_up(void *para);
void section_area_set_down(void *para);
void section_area_set_right(void *para);
void section_area_set_return(void *para);
void section_area_set_ok(void *para);
void section_area_set_ok_return(void *para);

//烟道截面积设置界面
void blow_freq_set_enter(void *para);
void blow_freq_set(void *para);
void blow_freq_set_up(void *para);
void blow_freq_set_down(void *para);
void blow_freq_set_right(void *para);
void blow_freq_set_return(void *para);
void blow_freq_set_ok(void *para);
void blow_freq_set_ok_return(void *para);

//密码登录界面
void password_login_enter(void *para);
void password_login(void *para);
void password_login_up(void *para);
void password_login_down(void *para);
void password_login_right(void *para);
void password_login_return(void *para);
void password_login_ok(void *para);
void password_login_error_return(void *para);

#endif
