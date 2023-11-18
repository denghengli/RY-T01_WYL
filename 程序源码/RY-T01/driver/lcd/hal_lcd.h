#ifndef __HAL_LCD_H__
#define __HAL_LCD_H__

//=========���ܽӿڶ���==========================
#define FCN_LCD_ONOFF     	 	0x0000	//=0000 + 1�ֽ�״̬λ =1 ���� = 0 �ر�
#define FCN_LCD_DISP_CLEAR		0x0001	//=0001 + 2�ֽ���ɫ ��������ɫ
#define FNC_LCD_DISP_SETCURSOR	0x0002	//=0002 + 4�ֽ�(x,y)
#define FNC_LCD_DISP_DRAW_POINT	0x0003	//=0003 + 4�ֽڻ��� 
#define FNC_LCD_DISP_DRAW_LINE	0x0004	//=0004 + 8�ֽڻ���  
#define FNC_LCD_DISP_DRAW_RECT	0x0005	//=0005 + 8�ֽڻ�����
#define FNC_LCD_DISP_DRAW_CIRC	0x0006	//=0006 + 4�ֽ����� + 2�ֽڰ뾶
#define FNC_LCD_DISP_DRAW_CHAR 	0x0007	//=0007 + 4�ֽ����� + 2�ֽ������� + 1�ֽ�������ɫ+ 1�ֽ���ʾ�ֳ��� + "��ʾ����GB2312��" 
#define FNC_LCD_DISP_DRAW_PIC 	0x0008	//=0008 + 4�ֽ����� + 4�ֽ�ͼƬ��� + ͼƬ���� 
#define FNC_LCD_DISP_DRAW_STRING 0x0009	//=0009 + 4�ֽ����� + �ַ��� + 2�ֽ������� + 2�ֽ�������ɫ

//=========�����ṹ�� ============================
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

