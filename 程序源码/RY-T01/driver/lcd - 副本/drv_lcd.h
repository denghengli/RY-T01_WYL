/*
 * File       : drv_lcd.h
 * Function   : lcd����
 * Change Logs:
 * Date           Author       Notes
 * 2023-11-17     denghengli   first version
 */

#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__

//2.4/2.8��TFTҺ������(ILI9325/RM68021/ILI9320 Verision)
//����LCD�ĳߴ�
#define LCD_W 320
#define LCD_H 240

//-----------------LCD�˿ڶ���---------------- 
#define	LCD_LED_H() LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_5) //LCD����    		 PC5
#define	LCD_LED_L() LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_5) //LCD����         PC5
#define	LCD_CS_H()	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9)  //Ƭѡ�˿�  	     PC9
#define	LCD_CS_L()	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_9)  //Ƭѡ�˿�  	     PC9
#define	LCD_RS_H()	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8)  //����/����        PC8	   
#define	LCD_RS_L()	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_8)  //����/����        PC8	   
#define	LCD_WR_H()	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7)  //д����			 PC7
#define	LCD_WR_L()	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7)  //д����			 PC7
#define	LCD_RD_H()	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_6)  //������			 PC6
#define	LCD_RD_L()	LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_6)  //������			 PC6
							    
//PB0~15,��Ϊ������
#define DATAOUT(x) LL_GPIO_WriteOutputPort(GPIOB, x) //�������	
#define DATAIN     LL_GPIO_ReadInputPort(GPIOB)    //��������   

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

void LCD_Init(void);
void LCD_Clear(uint16_t Color);	 
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color);//����
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y); //����
void LCD_DrawCircle(uint16_t x0,uint16_t y0,uint16_t r,uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);		   
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_ShowStringX(uint16_t x,uint16_t y,const uint8_t *p,uint16_t len,uint16_t mg,uint16_t fcolor, uint16_t bcolor);

void LCD_Putstring(uint16_t x, uint16_t y, char *s,uint16_t len,uint16_t size,uint16_t fcolor, uint16_t bcolor);
void LCD_Darwpic(uint16_t x, uint16_t y,uint16_t num);

//д16λ���ݺ���
//�ú궨��,����ٶ�.
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
	
	 



