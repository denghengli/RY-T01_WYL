#include "includes.h"

static uint8_t autoCalibZero; //自动校零标志

void auto_calib_zero_enter(void *para)
{
    autoCalibZero = g_SysData.Data.Para.autoCalibZero;

    ui_cur_state = AUTO_CALIB_ZERO;
}

void auto_calib_zero(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 70;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "是否开启自动校零");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 80;
    lcd_para.y = 120;
    lcd_para.mode = 24;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (autoCalibZero) {
        snprintf(tmp_str, sizeof(tmp_str), "→ 是     否");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   是  → 否");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void auto_calib_zero_right(void *para)
{
    autoCalibZero = (autoCalibZero + 1) % 2;

    auto_calib_zero(NULL);
    ui_cur_state = AUTO_CALIB_ZERO;
}

void auto_calib_zero_return(void *para)
{
    ui_cur_state = PARA_SET_ONE_SELECT_2;
    para_set_one_select_2(NULL);
}

void auto_calib_zero_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    //保存参数
    g_SysData.Data.Para.autoCalibZero = autoCalibZero;
    ParaData_Save(0); 

    //提示成功
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 100;
    lcd_para.y = 100;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "设置成功");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void auto_calib_zero_ok_return(void *para)
{
    ui_cur_state = PARA_SET_ONE_SELECT_2;
    para_set_one_select_2(NULL);
}




