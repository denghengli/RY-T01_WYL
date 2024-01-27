#ifndef __APP_GUI__
#define __APP_GUI__

typedef enum
{
    STATE_NULL = 0,
    START_LOGO = 1,
    MAIN_WIND,
    MAIN_WIND_RIGHT,

    CONFIG_MENU_ONE_SELECT_1,
    CONFIG_MENU_ONE_SELECT_2,
    CONFIG_MENU_ONE_SELECT_3,
    CONFIG_MENU_ONE_SELECT_4,
    CONFIG_MENU_ONE_SELECT_5,
    CONFIG_MENU_TWO_SELECT_1,
    CONFIG_MENU_TWO_SELECT_2,
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

void config_menu_one_select_1(void *para);
void config_menu_one_select_2(void *para);
void config_menu_one_select_3(void *para);
void config_menu_one_select_4(void *para);
void config_menu_one_select_5(void *para);
void config_menu_two_select_1(void *para);
void config_menu_two_select_2(void *para);

#endif
