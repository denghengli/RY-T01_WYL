#include "includes.h"

static uint8_t right_pos = 0; //右键移动的位置
static char ao_comp_char[7]; //偏移+0.11，系数0.12,共7位

void speed_ao_comp_enter(void *para)
{
    int integer,dec;

    right_pos = 0;
    integer = (int)fabs(g_SysData.Data.Para.speedAOOffset);
    dec = (int)(fabs(g_SysData.Data.Para.speedAOOffset) * 100) % 100;
    //符号位
    if (g_SysData.Data.Para.speedAOOffset >= 0) ao_comp_char[0] = 0;
    else ao_comp_char[0] = 1;
    ao_comp_char[1] = integer % 10;
    ao_comp_char[2] = dec / 10;
    ao_comp_char[3] = dec % 10;

    integer = (int)g_SysData.Data.Para.speedAOK;
    dec = (int)(g_SysData.Data.Para.speedAOK * 100) % 100;
    ao_comp_char[4] = integer % 10;
    ao_comp_char[5] = dec / 10;
    ao_comp_char[6] = dec % 10;
    
    ui_cur_state = SPEED_AO_COMP;
}

void speed_ao_comp(void *para)
{
    char tip_str[64] = {0};
    char tmp_str[64] = {0};
    char tmp_str1[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    uint16_t para_value = *(uint16_t*)para;
    static uint16_t hold_delay_cnt = 0;
    
    memset(tmp_str, 0, sizeof(tmp_str));
    memset(tmp_str1, 0, sizeof(tmp_str1));
    if (para_value == 1) //立刻显示
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        if (ao_comp_char[0] == 0) 
        {
            snprintf(tmp_str, sizeof(tmp_str), "+%d.%d%d", ao_comp_char[1], ao_comp_char[2], ao_comp_char[3]);
            snprintf(tmp_str1, sizeof(tmp_str1), "%d.%d%d", ao_comp_char[4], ao_comp_char[5], ao_comp_char[6]);
        }
        else
        {
            snprintf(tmp_str, sizeof(tmp_str), "-%d.%d%d", ao_comp_char[1], ao_comp_char[2], ao_comp_char[3]);
            snprintf(tmp_str1, sizeof(tmp_str1), "%d.%d%d", ao_comp_char[4], ao_comp_char[5], ao_comp_char[6]);
        }
    }
    else if (para_value == 2) //立刻隐藏
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        if (right_pos <= 3) //偏移量
        {
            if (ao_comp_char[0] == 0)
            {
                if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d.%d%d", ao_comp_char[1], ao_comp_char[2], ao_comp_char[3]);
                else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "+ .%d%d", ao_comp_char[2], ao_comp_char[3]);
                else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "+%d. %d", ao_comp_char[1], ao_comp_char[3]);
                else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "+%d.%d ", ao_comp_char[1], ao_comp_char[2]);
            }
            else
            {
                if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d.%d%d", ao_comp_char[1], ao_comp_char[2], ao_comp_char[3]);
                else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "- .%d%d", ao_comp_char[2], ao_comp_char[3]);
                else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "-%d. %d", ao_comp_char[1], ao_comp_char[3]);
                else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "-%d.%d ", ao_comp_char[1], ao_comp_char[2]);
            }
            snprintf(tmp_str1, sizeof(tmp_str1), "%d.%d%d", ao_comp_char[4], ao_comp_char[5], ao_comp_char[6]);
        }
        else //系数
        {
            if (right_pos == 4) snprintf(tmp_str1, sizeof(tmp_str1), " .%d%d", ao_comp_char[5], ao_comp_char[6]);
            else if (right_pos == 5) snprintf(tmp_str1, sizeof(tmp_str1), "%d. %d", ao_comp_char[4], ao_comp_char[6]);
            else if (right_pos == 6) snprintf(tmp_str1, sizeof(tmp_str1), "%d.%d ", ao_comp_char[4], ao_comp_char[5]);
        }
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
                if (ao_comp_char[0] == 0) 
                {
                    snprintf(tmp_str, sizeof(tmp_str), "+%d.%d%d", ao_comp_char[1], ao_comp_char[2], ao_comp_char[3]);
                    snprintf(tmp_str1, sizeof(tmp_str1), "%d.%d%d", ao_comp_char[4], ao_comp_char[5], ao_comp_char[6]);
                }
                else
                {
                    snprintf(tmp_str, sizeof(tmp_str), "-%d.%d%d", ao_comp_char[1], ao_comp_char[2], ao_comp_char[3]);
                    snprintf(tmp_str1, sizeof(tmp_str1), "%d.%d%d", ao_comp_char[4], ao_comp_char[5], ao_comp_char[6]);
                }
            }
            else
            {
                if (right_pos <= 3) //偏移量
                {
                    if (ao_comp_char[0] == 0)
                    {
                        if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d.%d%d", ao_comp_char[1], ao_comp_char[2], ao_comp_char[3]);
                        else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "+ .%d%d", ao_comp_char[2], ao_comp_char[3]);
                        else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "+%d. %d", ao_comp_char[1], ao_comp_char[3]);
                        else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "+%d.%d ", ao_comp_char[1], ao_comp_char[2]);
                    }
                    else
                    {
                        if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d.%d%d", ao_comp_char[1], ao_comp_char[2], ao_comp_char[3]);
                        else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "- .%d%d", ao_comp_char[2], ao_comp_char[3]);
                        else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "-%d. %d", ao_comp_char[1], ao_comp_char[3]);
                        else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "-%d.%d ", ao_comp_char[1], ao_comp_char[2]);
                    }
                    snprintf(tmp_str1, sizeof(tmp_str1), "%d.%d%d", ao_comp_char[4], ao_comp_char[5], ao_comp_char[6]);
                }
                else //系数
                {
                    if (right_pos == 4) snprintf(tmp_str1, sizeof(tmp_str1), " .%d%d", ao_comp_char[5], ao_comp_char[6]);
                    else if (right_pos == 5) snprintf(tmp_str1, sizeof(tmp_str1), "%d. %d", ao_comp_char[4], ao_comp_char[6]);
                    else if (right_pos == 6) snprintf(tmp_str1, sizeof(tmp_str1), "%d.%d ", ao_comp_char[4], ao_comp_char[5]);
                }
            }
        }
    }

    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 100;
    lcd_para.y = 40;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tip_str, sizeof(tip_str), "偏移量（mA）：");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tip_str, strlen(tip_str));
    lcd_para.x = 110;
    lcd_para.y = 80;
    lcd_para.mode = 32;
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 130;
    lcd_para.y = 135;
    lcd_para.mode = 24;
    snprintf(tip_str, sizeof(tip_str), "系数：");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tip_str, strlen(tip_str));
    lcd_para.x = 120;
    lcd_para.y = 175;
    lcd_para.mode = 32;
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str1, strlen(tmp_str1));
}

void speed_ao_comp_up(void *para)
{
    uint16_t para_value = 1; //立即显示

    if (right_pos == 0) {
        ao_comp_char[right_pos] = (ao_comp_char[right_pos] + 1) % 2; //0 - 1
    } else {
        ao_comp_char[right_pos] = (ao_comp_char[right_pos] + 1) % 10; //0 - 9
    }
    
    speed_ao_comp(&para_value);
    ui_cur_state = SPEED_AO_COMP;
}

void speed_ao_comp_down(void *para)
{
    uint16_t para_value = 1; //立即显示

    if (right_pos == 0) {
        if (ao_comp_char[right_pos] - 1 == -1) ao_comp_char[right_pos] = 1;
        else ao_comp_char[right_pos] -= 1;
    } else {
        if (ao_comp_char[right_pos] - 1 == -1) ao_comp_char[right_pos] = 9;
        else ao_comp_char[right_pos] -= 1;
    }
    
    speed_ao_comp(&para_value);
    ui_cur_state = SPEED_AO_COMP;
}

void speed_ao_comp_right(void *para)
{
    uint16_t para_value = 2; //立即隐藏
    right_pos = (right_pos + 1) % 7;

    speed_ao_comp(&para_value);
    ui_cur_state = SPEED_AO_COMP;
}

void speed_ao_comp_return(void *para)
{
    ui_cur_state = PARA_SET_THR_SELECT_2;
    para_set_thr_select_2(NULL);
}

void speed_ao_comp_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    int integer,dec;
    float value;
    
    //保存参数
    integer = ao_comp_char[1];
    dec = ao_comp_char[2] * 10 + ao_comp_char[3];
    value = integer + (float)(dec / 100.0 + 0.001);
    g_SysData.Data.Para.speedAOOffset = ao_comp_char[0] == 1 ? -value : value;

    integer = ao_comp_char[4];
    dec = ao_comp_char[5] * 10 + ao_comp_char[6];
    value = integer + (float)(dec / 100.0 + 0.001);
    g_SysData.Data.Para.speedAOK = value;
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

void speed_ao_comp_ok_return(void *para)
{
    ui_cur_state = PARA_SET_THR_SELECT_2;
    para_set_thr_select_2(NULL);
}




