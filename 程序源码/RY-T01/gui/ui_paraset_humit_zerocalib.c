#include "includes.h"

static uint16_t calib_time_s = 0;
static uint8_t calib_start_flag = 0;
static uint8_t sec_signal = 0;
 
void humit_zero_calib_enter(void *para)
{
    calib_start_flag = 0;
    calib_time_s = 0;
    sec_signal = ex_sec_signal;
    ui_cur_state = HUMIT_ZERO_CALIB;
}

void humit_zero_calib(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 80;
    lcd_para.y = 60;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "湿度零点校准");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 80;
    lcd_para.y = 100;
    lcd_para.mode = 24;
    if (sec_signal != ex_sec_signal)
    {
        sec_signal = ex_sec_signal;
        if (calib_start_flag)
        {
            calib_time_s++;
        }
    }
    snprintf(tmp_str, sizeof(tmp_str), "计时：%03d/180s", calib_time_s);
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 80;
    lcd_para.y = 140;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "*N2气流量>5L/min");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
    
    //校准完成
    if (calib_time_s == 180)
    {
        clean_sercen();
        humit_zero_calib_finish(NULL);
        ui_cur_state = HUMIT_ZERO_CALIB_FINISH;
    }
}

void humit_zero_calib_return(void *para)
{
    ui_cur_state = PARA_SET_ONE_SELECT_5;
    para_set_one_select_5(NULL);
}

void humit_zero_calib_ok(void *para)
{
    calib_start_flag = 1;
    
    humit_zero_calib(NULL);
    ui_cur_state = HUMIT_ZERO_CALIB;
}

void humit_zero_calib_finish(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    //保存参数
    g_SysData.Data.Para.humitZero = g_SysData.Data.Sample.humit;
    ParaData_Save(0); 

    //提示成功
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 100;
    lcd_para.y = 100;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "校零成功");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void humit_zero_calib_finish_return(void *para)
{
    ui_cur_state = PARA_SET_ONE_SELECT_5;
    para_set_one_select_5(NULL);
}


