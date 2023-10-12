#include "sys.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机51  单片机工作电压3.3V
//淘宝网站：https://shop358474914.taobao.com/?spm=a1z10.3-c-s.0.0.572f42e3ylbxzu
//我司提供技术支持，任何技术问题欢迎随时交流学习
//创建日期:2015/5/5
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 思迈微电子
//All rights reserved
//All rights reserved
//********************************************************************************

/**************************************************************************************
//=======================================液晶屏数据线接线==========================================//
//P2组高8位数据口,DB8-DB15依次连接P2^0-P2^7;8位模式下只使用高8位
#define  LCD_DataPortH P2 
//P0组低8位数据口,DB0-DB7依次连接P0^0-P0^7;请确认P0口已经上拉10K电阻,不宜太小，最小4.7K,推荐10K.    
#define  LCD_DataPortL P0     
//=======================================液晶屏控制线接线==========================================//
CS=P1^3;		//片选	
RS=P1^2;  		//数据/命令切换
WR=P1^1;		//写控制
RD=P1^0;		//读控制
RESET=P3^3;	 	//复位 
LCD_BL=P3^2;	//背光控制
//=========================================触摸屏触接线=========================================//
//不使用触摸或者模块本身不带触摸，则可不连接
DCLK	  =    P3^6; //触摸屏SPI总线时钟信号接P3.6  
TCS       =    P3^7; //触摸片选TCS接P3.7
DIN       =    P3^4; //MOSI接P3.4	
DOUT      =    P3^5; //MISO接P3.5																						   
Penirq    =    P4^0; //PEN引脚接P4.0，如单片机无P4组，请自行更改其他可用IO并修改代码定义
**************************************************************************************************/	

//LCD的画笔颜色和背景色	   
u16 POINT_COLOR=0x0000;	//画笔颜色
u16 BACK_COLOR=0xFFFF;  //背景色 
//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;


//******************************************************************
//函数名：  LCD_WR_REG
//作者：   思迈微电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位指令
//输入参数：Reg:待写入的指令值
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WR_REG(int Reg)	 
{	
	LCD_RS=0;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8位并行数据写入时序 
	LCD_DataPortH=Reg;	
#else	//16位并行数据写入时序 
	LCD_DataPortH=Reg>>8;	
	LCD_DataPortL=Reg;	
#endif
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
} 

//******************************************************************
//函数名：  LCD_WR_DATA
//作者：   思迈微电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写8位数据
//输入参数：Data:待写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
 void LCD_WR_DATA(int Data)
{
	LCD_RS=1;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8位并行数据写入时序 
	LCD_DataPortH=Data;	
#else	//16位并行数据写入时序 
	LCD_DataPortH=Data>>8;	
	LCD_DataPortL=Data;	
#endif
	LCD_WR=0;
	LCD_WR=1;
	LCD_CS=1;
}

//******************************************************************
//函数名：  LCD_WR_DATA_16Bit
//作者：   思迈微电子
//日期：    2013-02-22
//功能：    向液晶屏总线写入写16位数据
//输入参数：Data:待写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
 void LCD_WR_DATA_16Bit(int Data)
{
	LCD_RS=1;
	LCD_CS=0;
#if LCD_USE8BIT_MODEL==1 //8位并行数据写入时序 
	LCD_DataPortH=Data>>8;//高8位数据	
	LCD_WR=0;
	LCD_WR=1;
	LCD_DataPortH=Data;	//低8位数据
	LCD_WR=0;
	LCD_WR=1;
#else	//16位并行数据写入时序 
	LCD_DataPortH=Data>>8;	
	LCD_DataPortL=Data;	
	LCD_WR=0;
	LCD_WR=1;
#endif
	LCD_CS=1;
}

//******************************************************************
//函数名：  LCD_WriteReg
//作者：   思迈微电子
//日期：    2013-02-22
//功能：    写寄存器数据
//输入参数：LCD_Reg:寄存器地址
//			LCD_RegValue:要写入的数据
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue)
{
  LCD_WR_REG(LCD_Reg);
	LCD_WR_DATA(LCD_RegValue);
}

//******************************************************************
//函数名：  LCD_WriteRAM_Prepare
//作者：   思迈微电子
//日期：    2013-02-22
//功能：    开始写GRAM
//			在给液晶屏传送RGB数据前，应该发送写GRAM指令
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_WriteRAM_Prepare(void)
{
 	LCD_WR_REG(lcddev.wramcmd);	  
}


//******************************************************************
//函数名：  LCD_Clear
//作者：   思迈微电子
//日期：    2013-02-22
//功能：    LCD全屏填充清屏函数
//输入参数：Color:要清屏的填充色
//返回值：  无
//修改记录：无
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
//函数名：  LCD_DrawPoint
//作者：   思迈微电子
//日期：    2013-02-22
//功能：    在指定位置写入一个像素点数据
//输入参数：(x,y):光标坐标
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetWindows(x,y,x,y);//设置光标位置 
	LCD_WR_DATA_16Bit(POINT_COLOR); 	    
} 	 

//******************************************************************
//函数名：  LCD_Reset
//作者：   思迈微电子
//日期：    2013-02-22
//功能：    LCD复位函数，液晶初始化前要调用此函数
//输入参数：无
//返回值：  无
//修改记录：无
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
//函数名：  LCD_Init
//作者：   思迈微电子
//日期：    2013-02-22
//功能：    LCD初始化
//输入参数：无
//返回值：  无
//修改记录：无
//******************************************************************
void LCD_Init(void)
{
	LCD_Reset(); //初始化之前复位
	LCD_RD=1;	 //RD引脚没有用到，拉高处理

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

	//设置LCD属性参数
	LCD_SetParam();//设置LCD参数	 
	LCD_BL=1;//点亮背光	 
}
/*************************************************
函数名：LCD_SetWindows
功能：设置lcd显示窗口，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
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

	LCD_WriteRAM_Prepare();	//开始写入GRAM				
}   

/*************************************************
函数名：LCD_SetCursor
功能：设置光标位置
入口参数：xy坐标
返回值：无
*************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	  	    			
	LCD_WR_REG(lcddev.setxcmd);	
	LCD_WR_DATA(Xpos>>8);
	LCD_WR_DATA(0x00FF&Xpos);		

	
	LCD_WR_REG(lcddev.setycmd);	
	LCD_WR_DATA(Ypos>>8);
	LCD_WR_DATA(0x00FF&Ypos);		

	LCD_WriteRAM_Prepare();	//开始写入GRAM	
} 

//设置LCD参数
//方便进行横竖屏模式切换
void LCD_SetParam(void)
{ 	
	lcddev.wramcmd=0x2C;
//#if USE_HORIZONTAL==0	//使用竖屏 
//	lcddev.dir=0;//竖屏				 	 		
//	lcddev.width=240;
//	lcddev.height=320;
//	lcddev.setxcmd=0x2A;
//	lcddev.setycmd=0x2B;	
//	LCD_WriteReg(0x36,0x00);

//#else if USE_HORIZONTAL==1//横屏	 
//	lcddev.dir=1;//横屏
//	lcddev.width=320;
//	lcddev.height=240;
//	lcddev.setxcmd=0x2A;
//	lcddev.setycmd=0x2B;			
//	LCD_WriteReg(0x36,0xc0);
//#else if USE_HORIZONTAL==2//竖屏	 
//	lcddev.dir=1;//竖屏
//	lcddev.width=240;
//	lcddev.height=320;
//	lcddev.setxcmd=0x2A;
//	lcddev.setycmd=0x2B;			
//	LCD_WriteReg(0x36,0x70);
//#else USE_HORIZONTAL==3//横屏	 
//	lcddev.dir=1;//横屏
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





