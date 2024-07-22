#include "includes.h"

static uint8_t right_pos = 0; //右键移动的位置
static char blow_freq_char[2]; //1-10

void blow_freq_set_enter(void *para)
{
    right_pos = 0;
    
    blow_freq_char[0] = g_SysData.Data.Para.blowFreq / 10;
    blow_freq_char[1] = g_SysData.Data.Para.blowFreq % 10;
    
    ui_cur_state = BLOW_FREQ_SET;
}

void blow_freq_set(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    uint16_t para_value = *(uint16_t*)para;
    static uint16_t hold_delay_cnt = 0;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 90;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "反吹频次：");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 120;
    lcd_para.y = 120;
    lcd_para.mode = 32;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (para_value == 1) //立刻显示
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        snprintf(tmp_str, sizeof(tmp_str), "%d%d", blow_freq_char[0], blow_freq_char[1]);
    }
    else if (para_value == 2) //立刻隐藏
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        
        if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d", blow_freq_char[1]);
        else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d ", blow_freq_char[0]);
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
                snprintf(tmp_str, sizeof(tmp_str), "%d%d", blow_freq_char[0], blow_freq_char[1]);
            }
            else
            {
                if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d", blow_freq_char[1]);
                else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d ", blow_freq_char[0]);
            }
        }
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void blow_freq_set_up(void *para)
{
    uint16_t para_value = 1; //立即显示
    
    if (right_pos == 0)
    {
        if (blow_freq_char[1]) blow_freq_char[0] = 0; //0
        else blow_freq_char[0] = (blow_freq_char[0] + 1) % 2; //0 - 1
    }
    else
    {
        if (blow_freq_char[0] == 1) blow_freq_char[right_pos] = 0;
        else blow_freq_char[right_pos] = (blow_freq_char[right_pos] + 1) % 10; //0 - 9
    }

    blow_freq_set(&para_value);
    ui_cur_state = BLOW_FREQ_SET;
}

void blow_freq_set_down(void *para)
{
    uint16_t para_value = 1; //立即显示
    
    if (right_pos == 0)
    {
        if (blow_freq_char[1])
        {
            blow_freq_char[0] = 0;
        }
        else
        {
            if (blow_freq_char[0] - 1 == -1) blow_freq_char[0] = 1;
            else blow_freq_char[0] -= 1;
        }
    }
    else
    {
        if (blow_freq_char[0])
        {
            blow_freq_char[right_pos] = 0;
        }
        else
        {
            if (blow_freq_char[right_pos] - 1 == -1) blow_freq_char[right_pos] = 9;
            else blow_freq_char[right_pos] -= 1;
        }
    }
    
    blow_freq_set(&para_value);
    ui_cur_state = BLOW_FREQ_SET;
}

void blow_freq_set_right(void *para)
{
    uint16_t para_value = 2; //立即隐藏
    right_pos = (right_pos + 1) % 2;

    blow_freq_set(&para_value);
    ui_cur_state = BLOW_FREQ_SET;
}

void blow_freq_set_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_5;
    config_menu_one_select_5(NULL);
}

void blow_freq_set_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;

    //保存参数
    g_SysData.Data.Para.blowFreq = blow_freq_char[0] * 10 + blow_freq_char[1];
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

void blow_freq_set_ok_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_5;
    config_menu_one_select_5(NULL);
}


