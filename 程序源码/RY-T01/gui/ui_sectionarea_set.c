#include "includes.h"

static uint8_t right_pos = 0; //右键移动的位置
static char section_area_char[5]; //两位整数+三位小数

void section_area_set_enter(void *para)
{
    int integer,dec;

    right_pos = 0;
    integer = (int)g_SysData.Data.Para.sectionArea;
    dec = (int)(g_SysData.Data.Para.sectionArea * 1000) % 1000;
    
    section_area_char[0] = integer / 10;
    section_area_char[1] = integer % 10;
    section_area_char[2] = dec / 100;
    section_area_char[3] = (dec % 100) / 10;
    section_area_char[4] = dec % 10;
	
    ui_cur_state = SECTION_AREA_SET;
}

void section_area_set(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    uint16_t para_value = *(uint16_t*)para;
    static uint16_t hold_delay_cnt = 0;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 65;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "烟道截面积（㎡）：");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 100;
    lcd_para.y = 110;
    lcd_para.mode = 32;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (para_value == 1) //立刻显示
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        snprintf(tmp_str, sizeof(tmp_str), "%d%d.%d%d%d", section_area_char[0], section_area_char[1], section_area_char[2], section_area_char[3], section_area_char[4]);
    }
    else if (para_value == 2) //立刻隐藏
    {
        hold_delay_cnt = 10; //持续一会之后再闪烁
        
        if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d.%d%d%d", section_area_char[1], section_area_char[2], section_area_char[3], section_area_char[4]);
        else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d .%d%d%d", section_area_char[0], section_area_char[2], section_area_char[3], section_area_char[4]);
        else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d%d. %d%d", section_area_char[0], section_area_char[1], section_area_char[3], section_area_char[4]);
        else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "%d%d.%d %d", section_area_char[0], section_area_char[1], section_area_char[2], section_area_char[4]);
		else if (right_pos == 4) snprintf(tmp_str, sizeof(tmp_str), "%d%d.%d%d ", section_area_char[0], section_area_char[1], section_area_char[2], section_area_char[3]);
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
                snprintf(tmp_str, sizeof(tmp_str), "%d%d.%d%d%d", section_area_char[0], section_area_char[1], section_area_char[2], section_area_char[3], section_area_char[4]);
            }
            else
            {
				if (right_pos == 0) snprintf(tmp_str, sizeof(tmp_str), " %d.%d%d%d", section_area_char[1], section_area_char[2], section_area_char[3], section_area_char[4]);
				else if (right_pos == 1) snprintf(tmp_str, sizeof(tmp_str), "%d .%d%d%d", section_area_char[0], section_area_char[2], section_area_char[3], section_area_char[4]);
				else if (right_pos == 2) snprintf(tmp_str, sizeof(tmp_str), "%d%d. %d%d", section_area_char[0], section_area_char[1], section_area_char[3], section_area_char[4]);
				else if (right_pos == 3) snprintf(tmp_str, sizeof(tmp_str), "%d%d.%d %d", section_area_char[0], section_area_char[1], section_area_char[2], section_area_char[4]);
				else if (right_pos == 4) snprintf(tmp_str, sizeof(tmp_str), "%d%d.%d%d ", section_area_char[0], section_area_char[1], section_area_char[2], section_area_char[3]);
            }
        }
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void section_area_set_up(void *para)
{
    uint16_t para_value = 1; //立即显示

    section_area_char[right_pos] = (section_area_char[right_pos] + 1) % 10; //0 - 9

    section_area_set(&para_value);
    ui_cur_state = SECTION_AREA_SET;
}

void section_area_set_down(void *para)
{
    uint16_t para_value = 1; //立即显示

    if (section_area_char[right_pos] - 1 == -1) section_area_char[right_pos] = 9;
    else section_area_char[right_pos] -= 1;
    
    section_area_set(&para_value);
    ui_cur_state = SECTION_AREA_SET;
}

void section_area_set_right(void *para)
{
    uint16_t para_value = 2; //立即隐藏
    right_pos = (right_pos + 1) % 5;

    section_area_set(&para_value);
    ui_cur_state = SECTION_AREA_SET;
}

void section_area_set_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_4;
    config_menu_one_select_4(NULL);
}

void section_area_set_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    int integer,dec;
    
    //保存参数
    integer = section_area_char[0] * 10 + section_area_char[1];
    dec = section_area_char[2] * 100 + section_area_char[3] * 10 + section_area_char[4];
    g_SysData.Data.Para.sectionArea = integer + (float)(dec / 1000.0 + 0.001);
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

void section_area_set_ok_return(void *para)
{
    ui_cur_state = CONFIG_MENU_ONE_SELECT_4;
    config_menu_one_select_4(NULL);
}


