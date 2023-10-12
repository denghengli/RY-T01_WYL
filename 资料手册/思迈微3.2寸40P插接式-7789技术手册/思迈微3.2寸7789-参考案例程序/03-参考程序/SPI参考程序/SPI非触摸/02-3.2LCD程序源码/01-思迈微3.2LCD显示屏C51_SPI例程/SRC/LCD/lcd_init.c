#include "lcd_init.h"

//��׼ȷ��ʱ����
void delay_ms(unsigned int ms)
{                         
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
}


/******************************************************************************
      ����˵����LCD��������д�뺯��
      ������ݣ�dat  Ҫд��Ĵ�������
      ����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(u8 dat) 
{	
	u8 i;
	LCD_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		LCD_SCLK_Clr();
		if(dat&0x80)
		{
		   LCD_MOSI_Set();
		}
		else
		{
		   LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat<<=1;
	}	
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(u8 dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA(u16 dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      ����˵����LCDд������
      ������ݣ�dat д�������
      ����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(u8 dat)
{
	LCD_DC_Clr();//д����
	LCD_Writ_Bus(dat);
	LCD_DC_Set();//д����
}


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//������д
}


/******************************************************************************
      ����˵����LCD��ʼ������
      ������ݣ���
      ����ֵ��  ��
******************************************************************************/
void Lcd_Init(void)
{
	LCD_RES_Clr();
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);
	LCD_BLK_Set();
	delay_ms(100);
LCD_WR_REG(0x11);
		delay_ms(100); //Delay 120ms
	  LCD_WR_REG(0X36);// Memory Access Control
		if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
		else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
		else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
		else LCD_WR_DATA8(0xA0);
		LCD_WR_REG(0X3A);
		LCD_WR_DATA8(0X05);
	//--------------------------------ST7789S Frame rate setting-------------------------

		LCD_WR_REG(0xb2);
		LCD_WR_DATA8(0x0c);
		LCD_WR_DATA8(0x0c);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x33);
		LCD_WR_DATA8(0x33);
		LCD_WR_REG(0xb7);
		LCD_WR_DATA8(0x35);
		//---------------------------------ST7789S Power setting-----------------------------

		LCD_WR_REG(0xbb);
		LCD_WR_DATA8(0x35);
		LCD_WR_REG(0xc0);
		LCD_WR_DATA8(0x2c);
		LCD_WR_REG(0xc2);
		LCD_WR_DATA8(0x01);
		LCD_WR_REG(0xc3);
		LCD_WR_DATA8(0x13);
		LCD_WR_REG(0xc4);
		LCD_WR_DATA8(0x20);
		LCD_WR_REG(0xc6);
		LCD_WR_DATA8(0x0f);
		LCD_WR_REG(0xca);
		LCD_WR_DATA8(0x0f);
		LCD_WR_REG(0xc8);
		LCD_WR_DATA8(0x08);
		LCD_WR_REG(0x55);
		LCD_WR_DATA8(0x90);
		LCD_WR_REG(0xd0);
		LCD_WR_DATA8(0xa4);
		LCD_WR_DATA8(0xa1);
		//--------------------------------ST7789S gamma setting------------------------------
		LCD_WR_REG(0xe0);
		LCD_WR_DATA8(0xd0);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x06);
		LCD_WR_DATA8(0x09);
		LCD_WR_DATA8(0x0b);
		LCD_WR_DATA8(0x2a);
		LCD_WR_DATA8(0x3c);
		LCD_WR_DATA8(0x55);
		LCD_WR_DATA8(0x4b);
		LCD_WR_DATA8(0x08);
		LCD_WR_DATA8(0x16);
		LCD_WR_DATA8(0x14);
		LCD_WR_DATA8(0x19);
		LCD_WR_DATA8(0x20);
		LCD_WR_REG(0xe1);
		LCD_WR_DATA8(0xd0);
		LCD_WR_DATA8(0x00);
		LCD_WR_DATA8(0x06);
		LCD_WR_DATA8(0x09);
		LCD_WR_DATA8(0x0b);
		LCD_WR_DATA8(0x29);
		LCD_WR_DATA8(0x36);
		LCD_WR_DATA8(0x54);
		LCD_WR_DATA8(0x4b);
		LCD_WR_DATA8(0x0d);
		LCD_WR_DATA8(0x16);
		LCD_WR_DATA8(0x14);
		LCD_WR_DATA8(0x21);
		LCD_WR_DATA8(0x20);
		LCD_WR_REG(0x29);
} 








