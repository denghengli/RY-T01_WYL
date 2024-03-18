#include "includes.h"

static uint8_t humitType; //湿度传感器类型

void humit_sensor_type_enter(void *para)
{
    humitType = g_SysData.Data.Para.humitType;

    ui_cur_state = HUMIT_SENSOR_TYPE;
}

void humit_sensor_type(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 120;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    if (humitType == 0) {
        snprintf(tmp_str, sizeof(tmp_str), "→ A");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   A");
    }    
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 120;
    lcd_para.y = 120;
    lcd_para.mode = 24;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (humitType == 1) {
        snprintf(tmp_str, sizeof(tmp_str), "→ B");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   B");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void humit_sensor_type_up(void *para)
{
    humitType = (humitType + 1) % 2;

    humit_sensor_type(NULL);
    ui_cur_state = HUMIT_SENSOR_TYPE;
}

void humit_sensor_type_down(void *para)
{
    humitType = (humitType + 1) % 2;

    humit_sensor_type(NULL);
    ui_cur_state = HUMIT_SENSOR_TYPE;
}

void humit_sensor_type_return(void *para)
{
    ui_cur_state = PARA_SET_TWO_SELECT_4;
    para_set_two_select_4(NULL);
}

void humit_sensor_type_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    //保存参数
    g_SysData.Data.Para.humitType = humitType;
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

void humit_sensor_type_ok_return(void *para)
{
    ui_cur_state = PARA_SET_TWO_SELECT_4;
    para_set_two_select_4(NULL);
}






