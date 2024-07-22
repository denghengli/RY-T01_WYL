#include "includes.h"

static uint16_t blow_time_s = 0;
static uint8_t blow_start_flag = 0;
static uint8_t blow_success_flag = 0;
static uint8_t sec_signal = 0;
static uint8_t handleBlowType; //手动反吹类型 0皮托管反吹 1湿度管反吹

void handle_blow_enter(void *para)
{
    blow_time_s = 0;
    blow_start_flag = 0;
    blow_success_flag = 0;
    sec_signal = ex_sec_signal;
    handleBlowType = 0;
    
    ui_cur_state = HANDLE_BLOW;
}

void handle_blow(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 70;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    if (handleBlowType == 0) {
        snprintf(tmp_str, sizeof(tmp_str), "→ 皮托管反吹");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   皮托管反吹");
    }    
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 70;
    lcd_para.y = 120;
    lcd_para.mode = 24;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (handleBlowType == 1) {
        snprintf(tmp_str, sizeof(tmp_str), "→ 湿度管反吹");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   湿度管反吹");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void handle_blow_up(void *para)
{
    handleBlowType = (handleBlowType + 1) % 2;

    handle_blow(NULL);
    ui_cur_state = HANDLE_BLOW;
}

void handle_blow_down(void *para)
{
    handleBlowType = (handleBlowType + 1) % 2;

    handle_blow(NULL);
    ui_cur_state = HANDLE_BLOW;
}

void handle_blow_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_1;
    config_menu_one_select_1(NULL);
}

void handle_blow_ok(void *para)
{
    blow_start_flag = 1;

    handle_blow_ing(NULL); //反吹开始 立马刷新
    ui_cur_state = HANDLE_BLOW_ING;

    //开启管路
    if (handleBlowType == 0) {
        valve_ctrl(eSYSSTA_PITG_BLOW, 1);
    } else if (handleBlowType == 1) {
        valve_ctrl(eSYSSTA_HUMIT_BLOW, 1);
    }
}

void handle_blow_ing(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 110;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "反吹中");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 80;
    lcd_para.y = 120;
    lcd_para.mode = 24;
    if (sec_signal != ex_sec_signal)
    {
        sec_signal = ex_sec_signal;
        if (blow_start_flag)
        {
            blow_time_s++;
        }
    }
    snprintf(tmp_str, sizeof(tmp_str), "计时：%02d/30s", blow_time_s);
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
    
    //反吹完成
    if (blow_time_s == 30)
    {
        blow_success_flag = 1; //反吹成功
        
        clean_sercen();
        handle_blow_finish(NULL);
        ui_cur_state = HANDLE_BLOW_FINISH;
    }

}

void handle_blow_ing_return(void *para)
{
    blow_success_flag = 0; //校准失败
            
    //clean_sercen();
    handle_blow_finish(NULL);
    ui_cur_state = HANDLE_BLOW_FINISH;
}

void handle_blow_finish(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    //提示成功
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 100;
    lcd_para.y = 100;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    if (blow_success_flag) {
        snprintf(tmp_str, sizeof(tmp_str), "反吹成功");
    } else {
        lcd_para.fc = RED;
        snprintf(tmp_str, sizeof(tmp_str), "反吹失败");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    //关闭管路
    if (handleBlowType == 0) {
        valve_ctrl(eSYSSTA_PITG_BLOW, 0);
    } else if (handleBlowType == 1) {
        valve_ctrl(eSYSSTA_HUMIT_BLOW, 0);
    }
}

void handle_blow_finish_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_1;
    config_menu_one_select_1(NULL);
}




