#ifndef __HAL_LCD_H__
#define __HAL_LCD_H__

//=========功能接口定义==========================
#define FCN_LCD_ONOFF     	 	0x0000	//=0000 + 1字节状态位 =1 开启 = 0 关闭
#define FCN_LCD_DISP_CLEAR		0x0001	//=0001 + 2字节颜色 清屏或置色
#define FNC_LCD_DISP_SETCURSOR	0x0002	//=0002 + 4字节(x,y)
#define FNC_LCD_DISP_DRAW_POINT	0x0003	//=0003 + 4字节画点 
#define FNC_LCD_DISP_DRAW_LINE	0x0004	//=0004 + 8字节画线  
#define FNC_LCD_DISP_DRAW_RECT	0x0005	//=0005 + 8字节画矩形
#define FNC_LCD_DISP_DRAW_CIRC	0x0006	//=0006 + 4字节坐标 + 2字节半径
#define FNC_LCD_DISP_DRAW_CHAR 	0x0007	//=0007 + 4字节坐标 + 2字节字体规格 + 1字节字体颜色+ 1字节显示字长度 + "显示数据GB2312码" 
#define FNC_LCD_DISP_DRAW_PIC 	0x0008	//=0008 + 4字节坐标 + 4字节图片规格 + 图片数据 
#define FNC_LCD_DISP_DRAW_STRING 0x0009	//=0009 + 4字节坐标 + 字符串 + 2字节字体规格 + 2字节字体颜色

//=========参数结构体 ============================
typedef struct __FNC_LCD_DISP_DRAW_PARA
{
  uint16_t  cmd;	
  uint16_t 	x;
  uint16_t 	y;
  uint16_t 	mode;
  uint16_t 	bc;
  uint16_t 	fc;
}FNC_LCD_DISP_DRAW_PARA;

uint8_t* hal_lcd_driver_intface(void* para,uint8_t* str,uint16_t len);

#endif

