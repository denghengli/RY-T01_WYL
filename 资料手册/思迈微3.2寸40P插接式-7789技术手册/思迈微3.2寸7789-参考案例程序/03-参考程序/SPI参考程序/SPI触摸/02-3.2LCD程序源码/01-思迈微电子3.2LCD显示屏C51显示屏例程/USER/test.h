#ifndef __TEST_H__
#define __TEST_H__
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//测试硬件：单片机STC12LE5A60S2,晶振30M  单片机工作电压3.3V
//TFT液晶驱动 for C51
//淘宝网站：https://shop358474914.taobao.com/?spm=a1z10.3-c-s.0.0.572f42e3ylbxzu
//创建日期:2013/5/13
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 深圳市思迈微电子有限公司 2009-2019
//All rights reserved
//********************************************************************************
void DrawTestPage(u8 *str);
void Test_Color(void);
void Test_FillRec(void);
void Test_Circle(void);
void English_Font_test(void);
void Chinese_Font_test(void);
void Pic_test(void);
void Load_Drow_Dialog(void);
void Touch_Test(void);
void main_test(void);


#endif