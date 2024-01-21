#include "includes.h"

uint8_t* hal_lcd_driver_intface(void* para,uint8_t* str,uint16_t len)
{
    uint16_t cmd;
    FNC_LCD_DISP_DRAW_PARA* p=(FNC_LCD_DISP_DRAW_PARA*)para;
    
    cmd = p->cmd;
    //=0000  + 4�ֽڱ��� + 2�ֽڱ��� + 2�ֽڱ���ɫ +2�ֽڱ��� + 2�ֽ���ʾ�ֳ��� 01 + 1�ֽ�״̬����=1 ���� =0�ر�
    if(cmd == FCN_LCD_ONOFF)
    {

    }
    //=0001 + 4�ֽڱ��� + 2�ֽڱ��� + 2�ֽڱ���ɫ +2�ֽڱ��� + 2�ֽ���ʾ�ֳ��� 00 + ������
    else if(cmd == FCN_LCD_DISP_CLEAR)
    {
        LCD_Clear(p->bc);
    }
    //=0002 + 4�ֽ����� + 2���� + 2�ֽڱ��� +2�ֽڱ��� + 2�ֽ���ʾ�ֳ���  00+ ������
    else if(cmd == FNC_LCD_DISP_SETCURSOR)
    {

    }
    //=0003 +  4�ֽ����� + 2���� + 2�ֽڱ���ɫ+2�ֽ�������ɫ+ 2�ֽ���ʾ�ֳ��� 00 +������
    else if(cmd == FNC_LCD_DISP_DRAW_POINT)
    {

    }
    //=0004 + 4�ֽ����� + 2���� + 2�ֽڱ���ɫ+2�ֽ�������ɫ+ 2�ֽ���ʾ�ֳ��� + 4�ֽڽ�������������
    else if(cmd == FNC_LCD_DISP_DRAW_LINE)
    {
        LCD_DrawLine(p->x,p->y, (uint16_t)(str[1]*256+str[0]), (uint16_t)(str[3]*256+str[2]),p->fc);
    }
    //=0005 + 4�ֽ����� + 2���� + 2�ֽڱ���ɫ+2�ֽ�������ɫ+ 2�ֽ���ʾ�ֳ��� + 4�ֽڽ�������������
    else if(cmd == FNC_LCD_DISP_DRAW_RECT)
    {
        LCD_DrawRectangle(p->x, p->y, (uint16_t)(str[1]*256+str[0]), (uint16_t)(str[3]*256+str[2]),p->fc);					
    }
    //=0006 + 4�ֽ����� + 2���� + 2�ֽڱ���ɫ+2�ֽ�������ɫ+ 2�ֽ���ʾ�ֳ��� + 2�ֽڰ뾶
    else if(cmd == FNC_LCD_DISP_DRAW_CIRC)
    {
        LCD_DrawCircle(p->x,p->y,(uint16_t)(str[1]*256+str[0]),p->fc);
    }
    //=0007 + 4�ֽ����� + 2�ֽ������� + 2�ֽڱ���ɫ+2�ֽ�������ɫ+ 2�ֽ���ʾ�ֳ��� + "��ʾ����GB2312��" 
    else if(cmd == FNC_LCD_DISP_DRAW_CHAR)
    {		
        //LCD_ShowStringX(p->x,p->y,str,len,p->mode,p->fc,p->bc);
    }
    //=0008 + 4�ֽ����� + ͼƬ���
    else if(cmd == FNC_LCD_DISP_DRAW_PIC)
    {
        //LCD_Darwpic(p->x,p->y,p->mode);
    }				
    //=0009 + 4�ֽ����� + �ַ��� + 2�ֽ������� + 2�ֽ�������ɫ
    else if(cmd == FNC_LCD_DISP_DRAW_STRING)
    {
        LCD_Putstring(p->x,p->y,str,len,p->mode,p->fc,p->bc);
    }

    return 0;
}



