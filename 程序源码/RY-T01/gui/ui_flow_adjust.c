#include "includes.h"

static uint16_t adjust_time_s = 0;
static uint8_t adjust_start_flag = 0;
static uint8_t adjust_success_flag = 0;
static uint8_t sec_signal = 0;
static uint8_t flowAdjustType; //流速零点校准类型 0有反吹气校零 1无反吹气校零

void flow_adjust_enter(void *para)
{
    adjust_time_s = 0;
    adjust_start_flag = 0;
    adjust_success_flag = 0;
    sec_signal = ex_sec_signal;
    flowAdjustType = 0;
    
    ui_cur_state = FLOW_ADJUST;
}

void flow_adjust(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 55;
    lcd_para.y = 60;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    if (flowAdjustType == 0) {
        snprintf(tmp_str, sizeof(tmp_str), "→ 有反吹气校零");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   有反吹气校零");
    }    
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 55;
    lcd_para.y = 100;
    lcd_para.mode = 24;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (flowAdjustType == 1) {
        snprintf(tmp_str, sizeof(tmp_str), "→ 无反吹气校零");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   无反吹气校零");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 80;
    lcd_para.y = 140;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "*请短接H-L管！");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void flow_adjust_up(void *para)
{
    flowAdjustType = (flowAdjustType + 1) % 2;

    flow_adjust(NULL);
    ui_cur_state = FLOW_ADJUST;
}

void flow_adjust_down(void *para)
{
    flowAdjustType = (flowAdjustType + 1) % 2;

    flow_adjust(NULL);
    ui_cur_state = FLOW_ADJUST;
}

void flow_adjust_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_3;
    config_menu_one_select_3(NULL);
}

void flow_adjust_ok(void *para)
{
    adjust_start_flag = 1;

    flow_adjust_ing(NULL); //校准开始 立马刷新
    ui_cur_state = FLOW_ADJUST_ING;
}

void flow_adjust_ing(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 110;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "校准中");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 80;
    lcd_para.y = 120;
    lcd_para.mode = 24;
    if (sec_signal != ex_sec_signal)
    {
        sec_signal = ex_sec_signal;
        if (adjust_start_flag)
        {
            adjust_time_s++;
        }
    }
    snprintf(tmp_str, sizeof(tmp_str), "计时：%02d/20s", adjust_time_s);
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
    
    //校准完成
    if (adjust_time_s == 20)
    {
        adjust_success_flag = 1; //校准成功
        
        clean_sercen();
        flow_adjust_finish(NULL);
        ui_cur_state = FLOW_ADJUST_FINISH;
    }

}

void flow_adjust_ing_return(void *para)
{
    adjust_success_flag = 0; //校准失败
            
    //clean_sercen();
    flow_adjust_finish(NULL);
    ui_cur_state = FLOW_ADJUST_FINISH;
}

void flow_adjust_finish(void *para)
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
    if (adjust_success_flag) {
        snprintf(tmp_str, sizeof(tmp_str), "校准成功");
    } else {
        lcd_para.fc = RED;
        snprintf(tmp_str, sizeof(tmp_str), "校准失败");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void flow_adjust_finish_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_3;
    config_menu_one_select_3(NULL);
}





