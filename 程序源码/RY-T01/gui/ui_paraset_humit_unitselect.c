#include "includes.h"

static uint8_t humitDispUnit; //ʪ�ȵ��� 1��� 0����

void humit_unit_select_enter(void *para)
{
    humitDispUnit = g_SysData.Data.Para.humitDispUnit;

    ui_cur_state = HUMIT_UNIT_SELECT;
}

void humit_unit_select(void *para)
{
    char tmp_str[128] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 80;
    lcd_para.y = 80;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    if (humitDispUnit == 1) {
        snprintf(tmp_str, sizeof(tmp_str), "�� ���ʪ��");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   ���ʪ��");
    }    
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

    lcd_para.x = 80;
    lcd_para.y = 120;
    lcd_para.mode = 24;
    memset(tmp_str, 0, sizeof(tmp_str));
    if (humitDispUnit == 0) {
        snprintf(tmp_str, sizeof(tmp_str), "�� ����ʪ��");
    } else {
        snprintf(tmp_str, sizeof(tmp_str), "   ����ʪ��");
    }
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void humit_unit_select_up(void *para)
{
    humitDispUnit = (humitDispUnit + 1) % 2;

    humit_unit_select(NULL);
    ui_cur_state = HUMIT_UNIT_SELECT;
}

void humit_unit_select_down(void *para)
{
    humitDispUnit = (humitDispUnit + 1) % 2;

    humit_unit_select(NULL);
    ui_cur_state = HUMIT_UNIT_SELECT;
}

void humit_unit_select_return(void *para)
{
    ui_cur_state = PARA_SET_TWO_SELECT_2;
    para_set_two_select_2(NULL);
}

void humit_unit_select_ok(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    
    //�������
    g_SysData.Data.Para.humitDispUnit = humitDispUnit;
    ParaData_Save(0); 

    //��ʾ�ɹ�
    lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
    lcd_para.x = 100;
    lcd_para.y = 100;
    lcd_para.bc = DARKBLUE;
    lcd_para.fc = WHITE;
    lcd_para.mode = 24;
    snprintf(tmp_str, sizeof(tmp_str), "���óɹ�");
    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
}

void humit_unit_select_ok_return(void *para)
{
    ui_cur_state = PARA_SET_TWO_SELECT_2;
    para_set_two_select_2(NULL);
}





