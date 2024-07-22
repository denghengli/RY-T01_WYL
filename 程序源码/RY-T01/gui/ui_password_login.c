#include "includes.h"

static uint8_t right_pos = 0; //右键移动的位置
static char password_char[4]; //1234

void password_login_enter(void *para)
{
    right_pos = 0;
    
    password_char[0] = 0;
    password_char[1] = 0;
    password_char[2] = 0;
    password_char[3] = 0;
    
    ui_cur_state = PASSWORD_LOGIN;
}

void password_login(void *para)
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
    snprintf(tmp_str, sizeof(tmp_str), "输入密码");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 95;
    lcd_para.y = 120;
    lcd_para.mode = 32;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (para_value == 1) //立刻显示
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        snprintf(tmp_str, sizeof(tmp_str), "%d %d %d %d", password_char[0], password_char[1], password_char[2], password_char[3]);
    }
    else if (para_value == 2) //立刻隐藏
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        
        if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str),      "  %d %d %d", password_char[1], password_char[2], password_char[3]);
        else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d   %d %d", password_char[0], password_char[2], password_char[3]);
        else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d %d   %d", password_char[0], password_char[1], password_char[3]);
        else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "%d %d %d  ", password_char[0], password_char[1], password_char[2]);
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
                snprintf(tmp_str, sizeof(tmp_str), "%d %d %d %d", password_char[0], password_char[1], password_char[2], password_char[3]);
            }
            else
            {
                if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str),      "  %d %d %d", password_char[1], password_char[2], password_char[3]);
                else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d   %d %d", password_char[0], password_char[2], password_char[3]);
                else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d %d   %d", password_char[0], password_char[1], password_char[3]);
                else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "%d %d %d  ", password_char[0], password_char[1], password_char[2]);
            }
        }
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void password_login_up(void *para)
{
    uint16_t para_value = 1; //立即显示
    
    if (right_pos == 0) password_char[0] = (password_char[0] + 1) % 10; //0 - 9
    else if (right_pos == 1) password_char[1] = (password_char[1] + 1) % 10; //0 - 9
    else if (right_pos == 2) password_char[2] = (password_char[2] + 1) % 10; //0 - 9
    else if (right_pos == 3) password_char[3] = (password_char[3] + 1) % 10; //0 - 9

    password_login(&para_value);
    ui_cur_state = PASSWORD_LOGIN;
}

void password_login_down(void *para)
{
    uint16_t para_value = 1; //立即显示
    
    if (right_pos == 0)
    {
        if (password_char[0] - 1 == -1) password_char[0] = 9;
        else password_char[0] -= 1;
    }
    else if (right_pos == 1)
    {
        if (password_char[1] - 1 == -1) password_char[1] = 9;
        else password_char[1] -= 1;
    }
    else if (right_pos == 2)
    {
        if (password_char[2] - 1 == -1) password_char[2] = 9;
        else password_char[2] -= 1;
    }
    else if (right_pos == 3)
    {
        if (password_char[3] - 1 == -1) password_char[3] = 9;
        else password_char[3] -= 1;
    }
    
    password_login(&para_value);
    ui_cur_state = PASSWORD_LOGIN;
}

void password_login_right(void *para)
{
    uint16_t para_value = 2; //立即隐藏
    right_pos = (right_pos + 1) % 4;

    password_login(&para_value);
    ui_cur_state = PASSWORD_LOGIN;
}

void password_login_return(void *para)
{
    ui_cur_state = CONFIG_MENU_TWO_SELECT_1;
    config_menu_two_select_1(NULL);
}

void password_login_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    int password = password_char[0] * 1000 + password_char[1] * 100 +
                   password_char[2] * 10 + password_char[3];
    
    //密码匹配
    if (password == 1234)
    {
        ui_cur_state = PARA_SET_ONE_SELECT_1;
        para_set_one_select_1(NULL);
        ui_user = 1;
    }
    else if (password == 5678)
    {
        ui_cur_state = PARA_SET_ONE_SELECT_1;
        para_set_one_select_1(NULL);
        ui_user = 2;
    }
    else
    {
        lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
        lcd_para.x = 100;
        lcd_para.y = 100;
        lcd_para.bc = DARKBLUE;
        lcd_para.fc = RED;
        lcd_para.mode = 24;
        snprintf(tmp_str, sizeof(tmp_str), "密码错误");
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
    }
}

void password_login_error_return(void *para)
{
    ui_cur_state = CONFIG_MENU_TWO_SELECT_1;
    config_menu_two_select_1(NULL);
}


