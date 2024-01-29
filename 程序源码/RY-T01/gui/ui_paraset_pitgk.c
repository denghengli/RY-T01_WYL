#include "includes.h"

static uint8_t right_pos = 0; //右键移动的位置
static char pitg_k_char[3]; //两位整数+两位小数

void pitg_k_set_enter(void *para)
{
    int integer,dec;

    right_pos = 0;
    integer = (int)g_SysData.Data.Para.piTGRatioK;
    dec = (int)(g_SysData.Data.Para.piTGRatioK * 100) % 100;
    
    pitg_k_char[0] = integer % 10;
    pitg_k_char[1] = dec / 10;
    pitg_k_char[2] = dec % 10;
    
    ui_cur_state = PITG_K_SET;
}

void pitg_k_set(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    uint16_t para_value = *(uint16_t*)para;
    static uint16_t hold_delay_cnt = 0;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 100;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "皮托管系数：");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 120;
    lcd_para.y = 120;
    lcd_para.mode = 32;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (para_value == 1) //立刻显示
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        snprintf(tmp_str, sizeof(tmp_str), "%d.%d%d", pitg_k_char[0], pitg_k_char[1], pitg_k_char[2]);
    }
    else if (para_value == 2) //立刻隐藏
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        
        if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " .%d%d", pitg_k_char[1], pitg_k_char[2]);
        else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d. %d", pitg_k_char[0], pitg_k_char[2]);
        else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d.%d ", pitg_k_char[0], pitg_k_char[1]);
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
                snprintf(tmp_str, sizeof(tmp_str), "%d.%d%d", pitg_k_char[0], pitg_k_char[1], pitg_k_char[2]);
            }
            else
            {
                if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " .%d%d", pitg_k_char[1], pitg_k_char[2]);
                else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d. %d", pitg_k_char[0], pitg_k_char[2]);
                else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d.%d ", pitg_k_char[0], pitg_k_char[1]);
            }
        }
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void pitg_k_set_up(void *para)
{
    uint16_t para_value = 1; //立即显示

    pitg_k_char[right_pos] = (pitg_k_char[right_pos] + 1) % 10; //0 - 9

    pitg_k_set(&para_value);
    ui_cur_state = PITG_K_SET;
}

void pitg_k_set_down(void *para)
{
    uint16_t para_value = 1; //立即显示

    if (pitg_k_char[right_pos] - 1 == -1) pitg_k_char[right_pos] = 9;
    else pitg_k_char[right_pos] -= 1;
    
    pitg_k_set(&para_value);
    ui_cur_state = PITG_K_SET;
}

void pitg_k_set_right(void *para)
{
    uint16_t para_value = 2; //立即隐藏
    right_pos = (right_pos + 1) % 3;

    pitg_k_set(&para_value);
    ui_cur_state = PITG_K_SET;
}

void pitg_k_set_return(void *para)
{
    ui_cur_state = PARA_SET_ONE_SELECT_4;
    para_set_one_select_4(NULL);
}

void pitg_k_set_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    int integer,dec;
    
    //保存参数
    integer = pitg_k_char[0];
    dec = pitg_k_char[1] * 10 + pitg_k_char[2];
    g_SysData.Data.Para.piTGRatioK = integer + (float)(dec / 100.0 + 0.001);
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

void pitg_k_set_ok_return(void *para)
{
    ui_cur_state = PARA_SET_ONE_SELECT_4;
    para_set_one_select_4(NULL);
}



