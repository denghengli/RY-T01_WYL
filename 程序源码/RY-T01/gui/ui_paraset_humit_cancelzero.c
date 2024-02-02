#include "includes.h"

static uint8_t humitZreoValib; //零点有效标志

void cancel_humit_zero_enter(void *para)
{
    humitZreoValib = g_SysData.Data.Para.humitZreoValib;

    ui_cur_state = CANCEL_HUMIT_ZERO;
}

void cancel_humit_zero(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 70;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "是否取消校零值");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 70;
    lcd_para.y = 120;
    lcd_para.mode = 24;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (humitZreoValib == 0) {
        snprintf(tmp_str, sizeof(tmp_str), "→ 是     否");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   是  → 否");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void cancel_humit_zero_right(void *para)
{
    humitZreoValib = (humitZreoValib + 1) % 2;

    cancel_humit_zero(NULL);
    ui_cur_state = CANCEL_HUMIT_ZERO;
}

void cancel_humit_zero_return(void *para)
{
    ui_cur_state = PARA_SET_TWO_SELECT_1;
    para_set_two_select_1(NULL);
}

void cancel_humit_zero_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    //保存参数
    g_SysData.Data.Para.humitZreoValib = humitZreoValib;
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

void cancel_humit_zero_ok_return(void *para)
{
    ui_cur_state = PARA_SET_TWO_SELECT_1;
    para_set_two_select_1(NULL);
}





