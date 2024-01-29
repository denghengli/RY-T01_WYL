#include "includes.h"

static uint8_t right_pos = 0; //右键移动的位置
static char blow_inter_char[5]; //0-30000

void blow_inter_set_enter(void *para)
{
    right_pos = 0;
    
    blow_inter_char[0] = g_SysData.Data.Para.blowInterval / 10000;
    blow_inter_char[1] = (g_SysData.Data.Para.blowInterval % 10000) / 1000;
    blow_inter_char[2] = (g_SysData.Data.Para.blowInterval % 1000) / 100;
    blow_inter_char[3] = (g_SysData.Data.Para.blowInterval % 100) / 10;
    blow_inter_char[4] = (g_SysData.Data.Para.blowInterval % 10) / 1;
    
    ui_cur_state = BLOW_INTER_SET;
}

void blow_inter_set(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    uint16_t para_value = *(uint16_t*)para;
    static uint16_t hold_delay_cnt = 0;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 60;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "反吹间隔时间（分）：");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 120;
    lcd_para.y = 120;
    lcd_para.mode = 32;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (para_value == 1) //立刻显示
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        snprintf(tmp_str, sizeof(tmp_str), "%d%d%d%d%d", blow_inter_char[0], blow_inter_char[1], blow_inter_char[2], blow_inter_char[3], blow_inter_char[4]);
    }
    else if (para_value == 2) //立刻隐藏
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        
        if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d%d%d%d", blow_inter_char[1], blow_inter_char[2], blow_inter_char[3], blow_inter_char[4]);
        else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d %d%d%d", blow_inter_char[0], blow_inter_char[2], blow_inter_char[3], blow_inter_char[4]);
        else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d%d %d%d", blow_inter_char[0], blow_inter_char[1], blow_inter_char[3], blow_inter_char[4]);
        else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "%d%d%d %d", blow_inter_char[0], blow_inter_char[1], blow_inter_char[2], blow_inter_char[4]);
        else if (right_pos == 4) snprintf(tmp_str, sizeof(tmp_str), "%d%d%d%d ", blow_inter_char[0], blow_inter_char[1], blow_inter_char[2], blow_inter_char[3]);
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
                snprintf(tmp_str, sizeof(tmp_str), "%d%d%d%d%d", blow_inter_char[0], blow_inter_char[1], blow_inter_char[2], blow_inter_char[3], blow_inter_char[4]);
            }
            else
            {
                if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d%d%d%d", blow_inter_char[1], blow_inter_char[2], blow_inter_char[3], blow_inter_char[4]);
                else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d %d%d%d", blow_inter_char[0], blow_inter_char[2], blow_inter_char[3], blow_inter_char[4]);
                else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d%d %d%d", blow_inter_char[0], blow_inter_char[1], blow_inter_char[3], blow_inter_char[4]);
                else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "%d%d%d %d", blow_inter_char[0], blow_inter_char[1], blow_inter_char[2], blow_inter_char[4]);
                else if (right_pos == 4) snprintf(tmp_str, sizeof(tmp_str), "%d%d%d%d ", blow_inter_char[0], blow_inter_char[1], blow_inter_char[2], blow_inter_char[3]);
            }
        }
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void blow_inter_set_up(void *para)
{
    uint16_t para_value = 1; //立即显示
    
    if (right_pos == 0)
    {
        if (blow_inter_char[1] || blow_inter_char[2] || blow_inter_char[3] || blow_inter_char[4]) 
            blow_inter_char[0] = (blow_inter_char[0] + 1) % 3; //0 - 2
        else 
            blow_inter_char[0] = (blow_inter_char[0] + 1) % 4; //0 - 3
    }
    else
    {
        if (blow_inter_char[0] == 3) blow_inter_char[right_pos] = 0;
        else blow_inter_char[right_pos] = (blow_inter_char[right_pos] + 1) % 10; //0 - 9
    }

    blow_inter_set(&para_value);
    ui_cur_state = BLOW_INTER_SET;
}

void blow_inter_set_down(void *para)
{
    uint16_t para_value = 1; //立即显示
    
    if (right_pos == 0)
    {
        if (blow_inter_char[1] || blow_inter_char[2] || blow_inter_char[3] || blow_inter_char[4])
        {
            if (blow_inter_char[0] - 1 == -1) blow_inter_char[0] = 2;
            else blow_inter_char[0] -= 1;
        }
        else
        {
            if (blow_inter_char[0] - 1 == -1) blow_inter_char[0] = 3;
            else blow_inter_char[0] -= 1;
        }
    }
    else
    {
        if (blow_inter_char[0] == 3)
        {
            blow_inter_char[right_pos] = 0;
        }
        else
        {
            if (blow_inter_char[right_pos] - 1 == -1) blow_inter_char[right_pos] = 9;
            else blow_inter_char[right_pos] -= 1;
        }
    }
    
    blow_inter_set(&para_value);
    ui_cur_state = BLOW_INTER_SET;
}

void blow_inter_set_right(void *para)
{
    uint16_t para_value = 2; //立即隐藏
    right_pos = (right_pos + 1) % 5;

    blow_inter_set(&para_value);
    ui_cur_state = BLOW_INTER_SET;
}

void blow_inter_set_return(void *para)
{
    ui_cur_state = PARA_SET_ONE_SELECT_1;
    para_set_one_select_1(NULL);
}

void blow_inter_set_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;

    //保存参数
    g_SysData.Data.Para.blowInterval = blow_inter_char[0] * 10000 + 
                                    blow_inter_char[1] * 1000 +
                                    blow_inter_char[2] * 100 +
                                    blow_inter_char[3] * 10 + blow_inter_char[4];
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

void blow_inter_set_ok_return(void *para)
{
    ui_cur_state = PARA_SET_ONE_SELECT_1;
    para_set_one_select_1(NULL);
}


