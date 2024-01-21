#include "includes.h"

uint8_t* hal_lcd_driver_intface(void* para,uint8_t* str,uint16_t len)
{
    uint16_t cmd;
    FNC_LCD_DISP_DRAW_PARA* p=(FNC_LCD_DISP_DRAW_PARA*)para;
    
    cmd = p->cmd;
    //=0000  + 4字节保留 + 2字节保留 + 2字节背景色 +2字节保留 + 2字节显示字长度 01 + 1字节状态数据=1 开启 =0关闭
    if(cmd == FCN_LCD_ONOFF)
    {

    }
    //=0001 + 4字节保留 + 2字节保留 + 2字节背景色 +2字节保留 + 2字节显示字长度 00 + 无数据
    else if(cmd == FCN_LCD_DISP_CLEAR)
    {
        LCD_Clear(p->bc);
    }
    //=0002 + 4字节坐标 + 2保留 + 2字节保留 +2字节保留 + 2字节显示字长度  00+ 无数据
    else if(cmd == FNC_LCD_DISP_SETCURSOR)
    {

    }
    //=0003 +  4字节坐标 + 2保留 + 2字节背景色+2字节字体颜色+ 2字节显示字长度 00 +无数据
    else if(cmd == FNC_LCD_DISP_DRAW_POINT)
    {

    }
    //=0004 + 4字节坐标 + 2保留 + 2字节背景色+2字节字体颜色+ 2字节显示字长度 + 4字节结束点坐标数据
    else if(cmd == FNC_LCD_DISP_DRAW_LINE)
    {
        LCD_DrawLine(p->x,p->y, (uint16_t)(str[1]*256+str[0]), (uint16_t)(str[3]*256+str[2]),p->fc);
    }
    //=0005 + 4字节坐标 + 2保留 + 2字节背景色+2字节字体颜色+ 2字节显示字长度 + 4字节结束点坐标数据
    else if(cmd == FNC_LCD_DISP_DRAW_RECT)
    {
        LCD_DrawRectangle(p->x, p->y, (uint16_t)(str[1]*256+str[0]), (uint16_t)(str[3]*256+str[2]),p->fc);					
    }
    //=0006 + 4字节坐标 + 2保留 + 2字节背景色+2字节字体颜色+ 2字节显示字长度 + 2字节半径
    else if(cmd == FNC_LCD_DISP_DRAW_CIRC)
    {
        LCD_DrawCircle(p->x,p->y,(uint16_t)(str[1]*256+str[0]),p->fc);
    }
    //=0007 + 4字节坐标 + 2字节字体规格 + 2字节背景色+2字节字体颜色+ 2字节显示字长度 + "显示数据GB2312码" 
    else if(cmd == FNC_LCD_DISP_DRAW_CHAR)
    {		
        //LCD_ShowStringX(p->x,p->y,str,len,p->mode,p->fc,p->bc);
    }
    //=0008 + 4字节坐标 + 图片序号
    else if(cmd == FNC_LCD_DISP_DRAW_PIC)
    {
        //LCD_Darwpic(p->x,p->y,p->mode);
    }				
    //=0009 + 4字节坐标 + 字符串 + 2字节字体规格 + 2字节字体颜色
    else if(cmd == FNC_LCD_DISP_DRAW_STRING)
    {
        LCD_Putstring(p->x,p->y,str,len,p->mode,p->fc,p->bc);
    }

    return 0;
}



