//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//˼��΢����
//���̵�ַ��https://shop358474914.taobao.com/?spm=a1z10.3-c-s.0.0.572f42e3ylbxzu
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : �Թ�
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  3.3v��Դ
//              SCL  P10��SCLK��
//              SDA  P11��MOSI��
//              RES  P12
//              DC   P13
//              CS  P14
//							BLK P15
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : �Թ�
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) ˼��΢����2014/3/16
//All rights reserved
//******************************************************************************/
#include "REG51.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"

int main(void)
{
	u8 i,j;
	float t=0;
	LCD_Init();//LCD��ʼ��
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	while(1)
	{
		LCD_Fill(0,0,LCD_W,LCD_H,RED);
		LCD_Fill(0,0,LCD_W,LCD_H,GREEN);
		LCD_Fill(0,0,LCD_W,LCD_H,BLUE);
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		LCD_ShowChinese(0,0,"˼��΢����",RED,WHITE,32,0);
		LCD_ShowString(0,40,"LCD_W:",RED,WHITE,16,0);
		LCD_ShowIntNum(48,40,LCD_W,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowIntNum(128,40,LCD_H,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowString(0,70,"Increaseing Nun:",RED,WHITE,16,0);
		LCD_ShowFloatNum1(128,70,t,4,RED,WHITE,16);
		t+=0.11;
		for(j=0;j<5;j++)
		{
			for(i=0;i<6;i++)
			{
				LCD_ShowPicture(40*i,120+j*40,40,40,gImage_1);
			}
		}
	}
}

	