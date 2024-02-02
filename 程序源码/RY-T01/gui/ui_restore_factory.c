#include "includes.h"

static uint8_t restoreFlag = 0; //恢复出厂设置标志

void restore_factory_enter(void *para)
{
    restoreFlag = 1;

    ui_cur_state = RESTORE_FACTORY;
}

void restore_factory(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 60;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "确认恢复出厂设置");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 70;
    lcd_para.y = 120;
    lcd_para.mode = 24;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (restoreFlag) {
        snprintf(tmp_str, sizeof(tmp_str), "→ 是     否");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   是  → 否");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void restore_factory_right(void *para)
{
    restoreFlag = (restoreFlag + 1) % 2;

    restore_factory(NULL);
    ui_cur_state = RESTORE_FACTORY;
}

void restore_factory_return(void *para)
{
    ui_cur_state = PARA_SET_TWO_SELECT_3;
    para_set_two_select_3(NULL);
}

void restore_factory_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    //提示成功
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 60;
    lcd_para.y = 100;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    if (restoreFlag == 1) {
        snprintf(tmp_str, sizeof(tmp_str), "恢复出厂设置成功");
        ParaData_Factory();
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "取消恢复出厂设置");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void restore_factory_ok_return(void *para)
{
    ui_cur_state = PARA_SET_TWO_SELECT_3;
    para_set_two_select_3(NULL);
}





