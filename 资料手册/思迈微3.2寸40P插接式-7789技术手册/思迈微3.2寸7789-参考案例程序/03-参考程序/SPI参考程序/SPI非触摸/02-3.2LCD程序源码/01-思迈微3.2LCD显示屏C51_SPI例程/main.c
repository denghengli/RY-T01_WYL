//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//思迈微电子
//店铺地址：https://shop358474914.taobao.com/?spm=a1z10.3-c-s.0.0.572f42e3ylbxzu
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : 赵工
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  3.3v电源
//              SCL  P10（SCLK）
//              SDA  P11（MOSI）
//              RES  P12
//              DC   P13
//              CS  P14
//							BLK P15
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : 赵工
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 思迈微电子2014/3/16
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
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	while(1)
	{
		LCD_Fill(0,0,LCD_W,LCD_H,RED);
		LCD_Fill(0,0,LCD_W,LCD_H,GREEN);
		LCD_Fill(0,0,LCD_W,LCD_H,BLUE);
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		LCD_ShowChinese(0,0,"思迈微电子",RED,WHITE,32,0);
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

	