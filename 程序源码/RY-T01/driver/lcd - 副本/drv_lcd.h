/*
 * File       : drv_lcd.h
 * Function   : lcd驱动
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-17     denghengli   first version
 */

#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__

//2.4/2.8寸TFT液晶驱动(ILI9325/RM68021/ILI9320 Verision)
//定义LCD的尺寸
#define LCD_W 320
#define LCD_H 240

//-----------------LCD端口定义---------------- 
#define	LCD_LED_H() LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_5) //LCD背光    		 PC5
#define	LCD_LED_L() LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_5) //LCD背光         PC5
#define	LCD_CS_H()	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9)  //片选端口  	     PC9
#define	LCD_CS_L()	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9)  //片选端口  	     PC9
#define	LCD_RS_H()	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8)  //数据/命令        PC8	   
#define	LCD_RS_L()	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8)  //数据/命令        PC8	   
#define	LCD_WR_H()	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7)  //写数据			 PC7
#define	LCD_WR_L()	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7)  //写数据			 PC7
#define	LCD_RD_H()	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_6)  //读数据			 PC6
#define	LCD_RD_L()	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_6)  //读数据			 PC6
							    
//PB0~15,作为数据线
#define DATAOUT(x) LL_GPIO_WriteOutputPort(GPIOB, x) //数据输出	
#define DATAIN     LL_GPIO_ReadInputPort(GPIOB)    //数据输入   

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 		 0XFFE0
#define GBLUE		 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void LCD_Init(void);
void LCD_Clear(uint16_t Color);	 
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//画点
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y); //读点
void LCD_DrawCircle(uint16_t x0,uint16_t y0,uint16_t r,uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);		   
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_ShowStringX(uint16_t x,uint16_t y,const uint8_t *p,uint16_t len,uint16_t mg,uint16_t fcolor, uint16_t bcolor);

void LCD_Putstring(uint16_t x, uint16_t y, char *s,uint16_t len,uint16_t size,uint16_t fcolor, uint16_t bcolor);
void LCD_Darwpic(uint16_t x, uint16_t y,uint16_t num);

//写16位数据函数
//用宏定义,提高速度.
#define LCD_WR_DATA(data){\
LCD_CS_L();\
LCD_RD_H();\
LCD_RS_H();\
DATAOUT(data);\
LCD_WR_L();\
LCD_WR_H();\
LCD_CS_H();\
}  																					 
					  		 
#endif  
	
	 



