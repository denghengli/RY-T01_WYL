#ifndef __APP_GUI__
#define __APP_GUI__

typedef enum
{
    STATE_NULL = 0,
    START_LOGO = 1,
    MAIN_WIND,
    MAIN_WIND_RIGHT
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

extern volatile uint8_t ui_pre_state;
extern volatile uint8_t ui_cur_state;

typedef void (*state_fun)(void *para);

uint8_t is_clean(void);
uint8_t is_flash_menu(void);

void start_logo(void *para);
void main_wind(void *para);
void main_wind_right(void *para);

#endif
