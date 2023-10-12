#include "sys.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��51  ��Ƭ��������ѹ3.3V
//�Ա���վ��https://shop358474914.taobao.com/?spm=a1z10.3-c-s.0.0.572f42e3ylbxzu
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//��������:2015/5/5
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ˼��΢����
//All rights reserved
//All rights reserved
//********************************************************************************

/**************************************************************************************
//=======================================Һ���������߽���==========================================//
//P2���8λ���ݿ�,DB8-DB15��������P2^0-P2^7;8λģʽ��ֻʹ�ø�8λ
#define  LCD_DataPortH P2 
//P0���8λ���ݿ�,DB0-DB7��������P0^0-P0^7;��ȷ��P0���Ѿ�����10K����,����̫С����С4.7K,�Ƽ�10K.    
#define  LCD_DataPortL P0     
//=======================================Һ���������߽���==========================================//
CS=P1^3;		//Ƭѡ	
RS=P1^2;  		//����/�����л�
WR=P1^1;		//д����
RD=P1^0;		//������
RESET=P3^3;	 	//��λ 
LCD_BL=P3^2;	//�������
//=========================================������������=========================================//
//��ʹ�ô�������ģ�鱾������������ɲ�����
DCLK	  =    P3^6; //������SPI����ʱ���źŽ�P3.6  
TCS       =    P3^7; //����ƬѡTCS��P3.7
DIN       =    P3^4; //MOSI��P3.4	
DOUT      =    P3^5; //MISO��P3.5																						   
Penirq    =    P4^0; //PEN���Ž�P4.0���絥Ƭ����P4�飬�����и�����������IO���޸Ĵ��붨��
**************************************************************************************************/	

//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 
//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;


//******************************************************************
//��������  LCD_WR_REG
//���ߣ�   ˼��΢����
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λָ��
//���������Reg:��д���ָ��ֵ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WR_REG(int Reg)	 
{	
	LCD_RS=0;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8λ��������д��ʱ�� 
	LCD_DataPortH=Reg;	
#else	//16λ��������д��ʱ�� 
	LCD_DataPortH=Reg>>8;	
	LCD_DataPortL=Reg;	
#endif
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
} 

//******************************************************************
//��������  LCD_WR_DATA
//���ߣ�   ˼��΢����
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д8λ����
//���������Data:��д�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
 void LCD_WR_DATA(int Data)
{
	LCD_RS=1;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8λ��������д��ʱ�� 
	LCD_DataPortH=Data;	
#else	//16λ��������д��ʱ�� 
	LCD_DataPortH=Data>>8;	
	LCD_DataPortL=Data;	
#endif
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
}

//******************************************************************
//��������  LCD_WR_DATA_16Bit
//���ߣ�   ˼��΢����
//���ڣ�    2013-02-22
//���ܣ�    ��Һ��������д��д16λ����
//���������Data:��д�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
 void LCD_WR_DATA_16Bit(int Data)
{
	LCD_RS=1;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8λ��������д��ʱ�� 
	LCD_DataPortH=Data>>8;//��8λ����	
	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=Data;	//��8λ����
	LCD_WR=0;
	LCD_WR=1;
#else	//16λ��������д��ʱ�� 
	LCD_DataPortH=Data>>8;	
	LCD_DataPortL=Data;	
	LCD_WR=0;
	LCD_WR=1;
#endif
	LCD_CS=1;
}

//******************************************************************
//��������  LCD_WriteReg
//���ߣ�   ˼��΢����
//���ڣ�    2013-02-22
//���ܣ�    д�Ĵ�������
//���������LCD_Reg:�Ĵ�����ַ
//			LCD_RegValue:Ҫд�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

//******************************************************************
//��������  LCD_WriteRAM_Prepare
//���ߣ�   ˼��΢����
//���ڣ�    2013-02-22
//���ܣ�    ��ʼдGRAM
//			�ڸ�Һ��������RGB����ǰ��Ӧ�÷���дGRAMָ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}


//******************************************************************
//��������  LCD_Clear
//���ߣ�   ˼��΢����
//���ڣ�    2013-02-22
//���ܣ�    LCDȫ�������������
//���������Color:Ҫ���������ɫ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Clear(u16 Color)
{
	u16 i,j;
	LCD_SetWindows(0,0,lcddev.width-1,lcddev.height-1);	
    for(i=0;i<lcddev.width;i++)
	 {
	  for (j=0;j<lcddev.height;j++)
	   	{
        	 LCD_WR_DATA_16Bit(Color);
	    }

	  }
}

//******************************************************************
//��������  LCD_DrawPoint
//���ߣ�   ˼��΢����
//���ڣ�    2013-02-22
//���ܣ�    ��ָ��λ��д��һ�����ص�����
//���������(x,y):�������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetWindows(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA_16Bit(POINT_COLOR); 	    
} 	 

//******************************************************************
//��������  LCD_Reset
//���ߣ�   ˼��΢����
//���ڣ�    2013-02-22
//���ܣ�    LCD��λ������Һ����ʼ��ǰҪ���ô˺���
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Reset(void)
{
	LCD_RESET=1;
	delay_ms(50);	
	LCD_RESET=0;
	delay_ms(50);
	LCD_RESET=1;
	delay_ms(50);
}

//******************************************************************
//��������  LCD_Init
//���ߣ�   ˼��΢����
//���ڣ�    2013-02-22
//���ܣ�    LCD��ʼ��
//�����������
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************
void LCD_Init(void)
{
	LCD_Reset(); //��ʼ��֮ǰ��λ
	LCD_RD=1;	 //RD����û���õ������ߴ���

//************* Start Initial Sequence **********//		
	LCD_WR_REG(0x11);
		delay_ms(100); //Delay 120ms
	  LCD_WR_REG(0X36);// Memory Access Control
		if(USE_HORIZONTAL==0)LCD_WR_DATA(0x00);
		else if(USE_HORIZONTAL==1)LCD_WR_DATA(0xC0);
		else if(USE_HORIZONTAL==2)LCD_WR_DATA(0x70);
		else LCD_WR_DATA(0xA0);
		LCD_WR_REG(0X3A);
		LCD_WR_DATA(0X05);
	//--------------------------------ST7789S Frame rate setting-------------------------

		LCD_WR_REG(0xb2);
		LCD_WR_DATA(0x0c);
		LCD_WR_DATA(0x0c);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x33);
		LCD_WR_DATA(0x33);
		LCD_WR_REG(0xb7);
		LCD_WR_DATA(0x35);
		//---------------------------------ST7789S Power setting-----------------------------

		LCD_WR_REG(0xbb);
		LCD_WR_DATA(0x35);
		LCD_WR_REG(0xc0);
		LCD_WR_DATA(0x2c);
		LCD_WR_REG(0xc2);
		LCD_WR_DATA(0x01);
		LCD_WR_REG(0xc3);
		LCD_WR_DATA(0x13);
		LCD_WR_REG(0xc4);
		LCD_WR_DATA(0x20);
		LCD_WR_REG(0xc6);
		LCD_WR_DATA(0x0f);
		LCD_WR_REG(0xca);
		LCD_WR_DATA(0x0f);
		LCD_WR_REG(0xc8);
		LCD_WR_DATA(0x08);
		LCD_WR_REG(0x55);
		LCD_WR_DATA(0x90);
		LCD_WR_REG(0xd0);
		LCD_WR_DATA(0xa4);
		LCD_WR_DATA(0xa1);
		//--------------------------------ST7789S gamma setting------------------------------
		LCD_WR_REG(0xe0);
		LCD_WR_DATA(0xd0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x06);
		LCD_WR_DATA(0x09);
		LCD_WR_DATA(0x0b);
		LCD_WR_DATA(0x2a);
		LCD_WR_DATA(0x3c);
		LCD_WR_DATA(0x55);
		LCD_WR_DATA(0x4b);
		LCD_WR_DATA(0x08);
		LCD_WR_DATA(0x16);
		LCD_WR_DATA(0x14);
		LCD_WR_DATA(0x19);
		LCD_WR_DATA(0x20);
		LCD_WR_REG(0xe1);
		LCD_WR_DATA(0xd0);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x06);
		LCD_WR_DATA(0x09);
		LCD_WR_DATA(0x0b);
		LCD_WR_DATA(0x29);
		LCD_WR_DATA(0x36);
		LCD_WR_DATA(0x54);
		LCD_WR_DATA(0x4b);
		LCD_WR_DATA(0x0d);
		LCD_WR_DATA(0x16);
		LCD_WR_DATA(0x14);
		LCD_WR_DATA(0x21);
		LCD_WR_DATA(0x20);
		LCD_WR_REG(0x29);	

	//����LCD���Բ���
	LCD_SetParam();//����LCD����	 
	LCD_BL=1;//��������	 
}
/*************************************************
��������LCD_SetWindows
���ܣ�����lcd��ʾ���ڣ��ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{	
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(xStar>>8);
	LCD_WR_DATA(0x00FF&xStar);		
	LCD_WR_DATA(xEnd>>8);
	LCD_WR_DATA(0x00FF&xEnd);

	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(yStar>>8);
	LCD_WR_DATA(0x00FF&yStar);		
	LCD_WR_DATA(yEnd>>8);
	LCD_WR_DATA(0x00FF&yEnd);	

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM				
}   

/*************************************************
��������LCD_SetCursor
���ܣ����ù��λ��
��ڲ�����xy����
����ֵ����
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos);		

	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		

	LCD_WriteRAM_Prepare();	//��ʼд��GRAM	
} 

//����LCD����
//������к�����ģʽ�л�
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x2C;
//#if USE_HORIZONTAL==0	//ʹ������ 
//	lcddev.dir=0;//����				 	 		
//	lcddev.width=240;
//	lcddev.height=320;
//	lcddev.setxcmd=0x2A;
//	lcddev.setycmd=0x2B;	
//	LCD_WriteReg(0x36,0x00);

//#else if USE_HORIZONTAL==1//����	 
//	lcddev.dir=1;//����
//	lcddev.width=320;
//	lcddev.height=240;
//	lcddev.setxcmd=0x2A;
//	lcddev.setycmd=0x2B;			
//	LCD_WriteReg(0x36,0xc0);
//#else if USE_HORIZONTAL==2//����	 
//	lcddev.dir=1;//����
//	lcddev.width=240;
//	lcddev.height=320;
//	lcddev.setxcmd=0x2A;
//	lcddev.setycmd=0x2B;			
//	LCD_WriteReg(0x36,0x70);
//#else USE_HORIZONTAL==3//����	 
//	lcddev.dir=1;//����
//	lcddev.width=320;
//	lcddev.height=240;
//	lcddev.setxcmd=0x2A;
//	lcddev.setycmd=0x2B;			
//	LCD_WriteReg(0x36,0xA0);
    switch(USE_HORIZONTAL)
		{
			case 0:
			lcddev.width=240;
			lcddev.height=320;
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;			
			LCD_WriteReg(0x36,0x00);
      break;	
      case 1:
      lcddev.width=320;
			lcddev.height=240;
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;			
			LCD_WriteReg(0x36,0x70); 
      break;			
      case 2:
	  	lcddev.width=240;
			lcddev.height=320;
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;			
			LCD_WriteReg(0x36,0xc0);
      break;
      default:
      lcddev.width=320;
			lcddev.height=240;
			lcddev.setxcmd=0x2A;
			lcddev.setycmd=0x2B;			
			LCD_WriteReg(0x36,0xa0);
      break;			
		}
//#endif
}





