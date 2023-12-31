#include "lcd.h"
#include "string.h"
#include "sys.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "pic.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STC12LE5A60S2,晶振30M  单片机工作电压3.3V
//TFT液晶驱动 for C51
//淘宝网站：https://shop358474914.taobao.com/?spm=a1z10.3-c-s.0.0.572f42e3ylbxzu
//我司提供技术支持，任何技术问题欢迎随时交流学习
//创建日期:2013/5/13
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 深圳市思迈微电子有限公司 2009-2019
//All rights reserved
//********************************************************************************

//========================variable==========================//
u16 ColorTab[5]={BRED,YELLOW,RED,GREEN,BLUE};//定义颜色数组
//=====================end of variable======================//

//******************************************************************
//函数名：  DrawTestPage
//作者：    思迈微电子
//日期：    2013-02-22
//功能：    绘制测试界面
//输入参数：str :字符串指针
//返回值：  无
//修改记录：
//******************************************************************
void DrawTestPage(u8 *str)
{
//绘制固定栏up
LCD_Fill(0,0,lcddev.width,20,BLUE);
//绘制固定栏down
LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
POINT_COLOR=WHITE;
Gui_StrCenter(0,1,WHITE,BLUE,str,16,1);//居中显示
Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"思迈微版权所有",16,1);//居中显示
//绘制测试区域
LCD_Fill(0,20,lcddev.width,lcddev.height-20,BLACK);
}

//******************************************************************
//函数名：  main_test
//作者：    思迈微电子
//日期：    2013-02-22
//功能：    绘制思迈微电子综合测试程序主界面
//输入参数：无
//返回值：  无
//修改记录：
//******************************************************************
void main_test(void)
{
	DrawTestPage("思迈微电子综合测试程序");
	
	Gui_StrCenter(0,30,RED,BLUE,"思迈微电子",16,1);//居中显示
	Gui_StrCenter(0,60,RED,BLUE,"综合测试程序",16,1);//居中显示	
	Gui_StrCenter(0,90,YELLOW,BLUE,"3.2' ST7789 240X320",16,1);//居中显示
	Gui_StrCenter(0,120,BLUE,BLUE,"思迈微 2014-02-25",16,1);//居中显示
	delay_ms(1500);		
	delay_ms(1500);
}

//******************************************************************
//函数名：  Test_Color
//作者：    思迈微电子
//日期：    2013-02-22
//功能：    颜色填充测试，依次填充白色、黑色、红色、绿色、蓝色
//输入参数：无
//返回值：  无
//修改记录：
//******************************************************************
void Test_Color(void)
{
	DrawTestPage("测试1:纯色填充测试");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	Show_Str(lcddev.width-50,30,BLUE,YELLOW,"White",16,1);delay_ms(500);
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,BLACK);
	Show_Str(lcddev.width-50,30,BLUE,YELLOW,"Black",16,1);delay_ms(500);
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,RED);
	Show_Str(lcddev.width-50,30,BLUE,YELLOW,"Red",16,1); delay_ms(500);
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,GREEN);
	Show_Str(lcddev.width-50,30,BLUE,YELLOW,"Green",16,1);delay_ms(500);
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,BLUE);
	Show_Str(lcddev.width-50,30,WHITE,YELLOW,"Blue",16,1);delay_ms(500);

}

//******************************************************************
//函数名：  Test_FillRec
//作者：    思迈微电子
//日期：    2013-02-22
//功能：    矩形框显示和填充测试，依次显示粉红色、黄色、红色、绿色、蓝色矩形框，
//       	延时1500毫秒后，依次按照粉红色、黄色、红色、绿色、蓝色填充矩形框 
//输入参数：无
//返回值：  无
//修改记录：
//******************************************************************
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("测试2:GUI矩形填充测试");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++) 
	{
		LCD_DrawRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
		POINT_COLOR=ColorTab[i];
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	{
		LCD_DrawFillRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
		POINT_COLOR=ColorTab[i];
	}
	delay_ms(1500);
}

//******************************************************************
//函数名：  Test_FillRec
//作者：    思迈微电子
//日期：    2013-02-22
//功能：    圆形框显示和填充测试，依次显示粉红色、黄色、红色、绿色、蓝色圆形框，
//       	延时1500毫秒后，依次按照粉红色、黄色、红色、绿色、蓝色填充圆形框 
//输入参数：无
//返回值：  无
//修改记录：
//******************************************************************
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("测试3:GUI画圆填充测试");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++)  
		gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,0);
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	  	gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,1);
	delay_ms(1500);
}

//******************************************************************
//函数名：  English_Font_test
//作者：    思迈微电子
//日期：    2013-02-22
//功能：    英文显示测试 
//输入参数：无
//返回值：  无
//修改记录：
//******************************************************************
void English_Font_test(void)
{
	DrawTestPage("测试4:英文显示测试");
	POINT_COLOR=RED;
	BACK_COLOR=BLUE;
	LCD_ShowString(10,30,12,"6X12:abcdefghijklmnopqrstuvwxyz",0);
	LCD_ShowString(10,45,12,"6X12:ABCDEFGHIJKLMNOP0123456789",1);
	LCD_ShowString(10,60,12,"6X12:~!@#$%^&*()_+{}:<>?/|-+.",0);
	LCD_ShowString(10,80,16,"8X16:abcdefghijklmnopqrstuvw",0);
	LCD_ShowString(10,100,16,"8X16:ABCDEFGHIJKLMN01234567",1);
	LCD_ShowString(10,120,16,"8X16:~!@#$%^&*()_+{}:<>?/|",0); 
	delay_ms(1200);
}

//******************************************************************
//函数名：  Chinese_Font_test
//作者：    思迈微电子
//日期：    2013-02-22
//功能：    中文显示测试
//输入参数：无
//返回值：  无
//修改记录：
//******************************************************************
void Chinese_Font_test(void)
{	
	DrawTestPage("测试5:中文显示测试");
	Show_Str(10,30,BLUE,YELLOW,"16X16:思迈微电子技术有限公司欢迎您",16,0);
	Show_Str(10,50,BLUE,YELLOW,"16X16:Welcome思迈微电子",16,1);
	Show_Str(10,70,BLUE,YELLOW,"24X24:思迈微中文测试",24,1);
	Show_Str(10,100,BLUE,YELLOW,"32X32:字体测试",32,1);
	delay_ms(1200);
}

//******************************************************************
//函数名：  Pic_test
//作者：    思迈微电子
//日期：    2013-02-22
//功能：    图片显示测试，依次显示三幅40X40 QQ图像
//输入参数：无
//返回值：  无
//修改记录：
//******************************************************************
void Pic_test(void)
{
	DrawTestPage("测试6:图片显示测试");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	Gui_Drawbmp16(30,30,gImage_qq);
	Show_Str(30+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(90,30,gImage_qq);
	Show_Str(90+12,75,BLUE,YELLOW,"QQ",16,1);
	Gui_Drawbmp16(150,30,gImage_qq);
	Show_Str(150+12,75,BLUE,YELLOW,"QQ",16,1);
	delay_ms(1200);
}

//******************************************************************
//函数名：  Touch_Test
//作者：    思迈微电子
//日期：    2013-02-22
//功能：    触摸手写测试
//输入参数：无
//返回值：  无
//修改记录：
//******************************************************************
void Touch_Test(void)
{ 
	u8 i=0;
	u16 j=0;
	u16 colorTemp=0;		
	DrawTestPage("测试7:Touch测试");
	LCD_ShowString(lcddev.width-24,0,16,"RST",1);//显示清屏区域
	LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,RED); 
	POINT_COLOR=RED;
	while(1)
	{	 	 
		if(Convert_Pos())			//触摸屏被按下
		{	
		 	if(tp_pixlcd.x<lcddev.width&&tp_pixlcd.y<lcddev.height)
			{	
				if(tp_pixlcd.x>(lcddev.width-24)&&tp_pixlcd.y<16)
				{
					DrawTestPage("测试7:Touch测试");//清除
					POINT_COLOR=colorTemp;
					LCD_ShowString(lcddev.width-24,0,16,"RST",1);//显示清屏区域
					LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,POINT_COLOR); 
				}
				else if((tp_pixlcd.x>(lcddev.width-60)&&tp_pixlcd.x<(lcddev.width-50+20))&&tp_pixlcd.y<20)
				{
				LCD_Fill(lcddev.width-52,2,lcddev.width-50+20,18,ColorTab[j%5]); 
				POINT_COLOR=ColorTab[(j++)%5];
				colorTemp=POINT_COLOR;
				delay_ms(10);
				}

				else 
				TP_Draw_Big_Point(tp_pixlcd.x,tp_pixlcd.y,POINT_COLOR);    			   
			}
		}else delay_ms(10);	//没有按键按下的时候 	    				
	}   
}