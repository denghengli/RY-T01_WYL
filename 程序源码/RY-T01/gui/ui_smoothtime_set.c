#include "includes.h"

static uint8_t right_pos = 0; //右键移动的位置
static char smooth_time_char[3]; //1-600

void smooth_time_set_enter(void *para)
{
    right_pos = 0;
    
    smooth_time_char[0] = g_SysData.Data.Para.smoothTime / 100;
    smooth_time_char[1] = (g_SysData.Data.Para.smoothTime % 100) / 10;
    smooth_time_char[2] = g_SysData.Data.Para.smoothTime % 10;
    
    ui_cur_state = SMOOTH_TIME_SET;
}

void smooth_time_set(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    uint16_t para_value = *(uint16_t*)para;
    static uint16_t hold_delay_cnt = 0;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 80;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "平滑时间（s）：");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 120;
    lcd_para.y = 120;
    lcd_para.mode = 32;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (para_value == 1) //立刻显示
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        snprintf(tmp_str, sizeof(tmp_str), "%d%d%d", smooth_time_char[0], smooth_time_char[1], smooth_time_char[2]);
    }
    else if (para_value == 2) //立刻隐藏
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        
        if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d%d", smooth_time_char[1], smooth_time_char[2]);
        else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d %d", smooth_time_char[0], smooth_time_char[2]);
        else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d%d ", smooth_time_char[0], smooth_time_char[1]);
    }
    else //闪烁
    {
        if (hold_delay_cnt) 
        {
            hold_delay_cnt--;
        } 
        else 
        {
            if (ex_sec_signal)
            {
                snprintf(tmp_str, sizeof(tmp_str), "%d%d%d", smooth_time_char[0], smooth_time_char[1], smooth_time_char[2]);
            }
            else
            {
                if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d%d", smooth_time_char[1], smooth_time_char[2]);
                else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d %d", smooth_time_char[0], smooth_time_char[2]);
                else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d%d ", smooth_time_char[0], smooth_time_char[1]);
            }
        }
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void smooth_time_set_up(void *para)
{
    uint16_t para_value = 1; //立即显示
    
    if (right_pos == 0)
    {
        if (smooth_time_char[1] || smooth_time_char[2]) smooth_time_char[0] = (smooth_time_char[0] + 1) % 6; //0 - 5
        else smooth_time_char[0] = (smooth_time_char[0] + 1) % 7; //0 - 6
    }
    else
    {
        if (smooth_time_char[0] == 6) smooth_time_char[right_pos] = 0;
        else smooth_time_char[right_pos] = (smooth_time_char[right_pos] + 1) % 10; //0 - 9
    }

    smooth_time_set(&para_value);
    ui_cur_state = SMOOTH_TIME_SET;
}

void smooth_time_set_down(void *para)
{
    uint16_t para_value = 1; //立即显示
    
    if (right_pos == 0)
    {
        if (smooth_time_char[1] || smooth_time_char[2])
        {
            if (smooth_time_char[0] - 1 == -1) smooth_time_char[0] = 5;
            else smooth_time_char[0] -= 1;
        }
        else
        {
            if (smooth_time_char[0] - 1 == -1) smooth_time_char[0] = 6;
            else smooth_time_char[0] -= 1;
        }
    }
    else
    {
        if (smooth_time_char[0] == 6)
        {
            smooth_time_char[right_pos] = 0;
        }
        else
        {
            if (smooth_time_char[right_pos] - 1 == -1) smooth_time_char[right_pos] = 9;
            else smooth_time_char[right_pos] -= 1;
        }
    }
    
    smooth_time_set(&para_value);
    ui_cur_state = SMOOTH_TIME_SET;
}

void smooth_time_set_right(void *para)
{
    uint16_t para_value = 2; //立即隐藏
    right_pos = (right_pos + 1) % 3;

    smooth_time_set(&para_value);
    ui_cur_state = SMOOTH_TIME_SET;
}

void smooth_time_set_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_3;
    config_menu_one_select_3(NULL);
}

void smooth_time_set_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;

    //保存参数
    g_SysData.Data.Para.smoothTime = smooth_time_char[0] * 100 + 
                                    smooth_time_char[1] * 10 +
                                    smooth_time_char[2];
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

void smooth_time_set_ok_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_3;
    config_menu_one_select_3(NULL);
}


