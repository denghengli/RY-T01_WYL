/*
* File       : drv_lcd.c
* Function   : lcd驱动
* Change Logs:
* Date           Author       Notes
* 2023-11-17     denghengli   first version
*/

#include "includes.h"
#include "hal_lcd_font8x16.h"
#include "ascii.h"
#include "dr_pic.h"

//延时函数
void lcd_delay_ms(uint32_t m)
{
    volatile uint32_t i, j;
    for (i=0; i<m; i++)
        for (j=0; j<10000; j++);
}


void LCD_WR_REG(uint8_t data)
{ 
  LCD_CS_L(); 
  LCD_RD_H();//写地址  
  LCD_RS_L();//写地址  
  
  DATAOUT(data); 
  
  LCD_WR_L(); 
  LCD_WR_H(); 
  LCD_CS_H();   
} 

//读寄存器
uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{							   
  uint16_t t;
  LL_GPIO_InitTypeDef GPIO_InitStruct;
  
  LCD_WR_REG(LCD_Reg);   //写入要读的寄存器号  
  GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  LCD_RS_H();
  LCD_CS_L();
  //读取数据(读寄存器时,并不需要读2次)
  LCD_RD_L();					   
  t = DATAIN;
  LCD_RD_H();
  LCD_CS_H();   
  
  GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  return t;  
}   


//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
uint16_t LCD_BGR2RGB(uint16_t c)
{
  uint16_t  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);
}		 

void lcd_set_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  LCD_WR_REG(0x2A);//set_column_address //240 8.2.20
  LCD_WR_DATA(x1>>8);
  LCD_WR_DATA(x1);
  LCD_WR_DATA(x2>>8);
  LCD_WR_DATA(x2);
  
  LCD_WR_REG(0x2B);// set_page_address //320 8.2.21
  LCD_WR_DATA(y1>>8);
  LCD_WR_DATA(y1);
  LCD_WR_DATA(y2>>8);
  LCD_WR_DATA(y2);
}
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
  lcd_set_area(Xpos, Ypos, Xpos, Ypos);
} 
//画点
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
  LCD_SetCursor(x,y);//设置光标位置 
  LCD_WR_REG(0x2c);//开始写入GRAM
  LCD_WR_DATA(color); 	
} 	 

//初始化lcd
//该初始化函数可以初始化各种ILI93XX液晶,但是其他函数是基于ILI9320的!!!
//在其他型号的驱动芯片上没有测试! 
void LCD_Init(void)
{ 
    LL_GPIO_InitTypeDef GPIO_InitStruct; 

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);//打开复用时钟 
    //PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);	
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);

    //GPIOC5~9复用推挽输出 	
    GPIO_InitStruct.Pin = (LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_8 | LL_GPIO_PIN_9);
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);	

    //PORTB 推挽输出 	
    GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    LCD_LED_H();//关背光	
  
    lcd_delay_ms(120);
    //**************************//LCD SETING 

    //**************************//LCD SETING 
    LCD_WR_REG(0xCF);  //8.4.2  
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0xc1); 
    LCD_WR_DATA(0X30); 

    LCD_WR_REG(0xED);  //8.4.5
    LCD_WR_DATA(0x64); 
    LCD_WR_DATA(0x03); 
    LCD_WR_DATA(0X12); 
    LCD_WR_DATA(0X81); 

    LCD_WR_REG(0xE8);  
    LCD_WR_DATA(0x85); 
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x78); 

    LCD_WR_REG(0xCB);  
    LCD_WR_DATA(0x39); 
    LCD_WR_DATA(0x2C); 
    LCD_WR_DATA(0x00); 

    LCD_WR_DATA(0x34); 
    LCD_WR_DATA(0x02); 

    LCD_WR_REG(0xF7);  
    LCD_WR_DATA(0x20); 

    LCD_WR_REG(0xEA);  
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x00); 

    LCD_WR_REG(0xC0);    //Power control 
    LCD_WR_DATA(0x21);   //VRH[5:0] 

    LCD_WR_REG(0xC1);    //Power control 
    LCD_WR_DATA(0x11);   //SAP[2:0];BT[3:0] 

    LCD_WR_REG(0xC5);    //VCM control 
    LCD_WR_DATA(0x31); 
    LCD_WR_DATA(0x3f); 

    LCD_WR_REG(0xC7);    //VCM control2 
    LCD_WR_DATA(0x93); 

    LCD_WR_REG(0x36);    // Memory Access Control 
    LCD_WR_DATA(0x68); 

    LCD_WR_REG(0x3A);   //8.2.33
    LCD_WR_DATA(0x55); 

    LCD_WR_REG(0xB1);   
    LCD_WR_DATA(0x00);   
    LCD_WR_DATA(0x17); 

    LCD_WR_REG(0xB6);    // Display Function Control 
    LCD_WR_DATA(0x0A); 
    LCD_WR_DATA(0xA2); 

    LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
    LCD_WR_DATA(0x00); 

    LCD_WR_REG(0x26);    //Gamma curve selected 
    LCD_WR_DATA(0x01); 

    LCD_WR_REG(0xE0);    //Set Gamma 
    LCD_WR_DATA(0x0F); 
    LCD_WR_DATA(0x1f); 
    LCD_WR_DATA(0x1D); 
    LCD_WR_DATA(0x09); 
    LCD_WR_DATA(0x0B); 
    LCD_WR_DATA(0x04); 
    LCD_WR_DATA(0x4E); 
    LCD_WR_DATA(0X92); 
    LCD_WR_DATA(0x40); 
    LCD_WR_DATA(0x0A); 
    LCD_WR_DATA(0x15); 
    LCD_WR_DATA(0x07); 
    LCD_WR_DATA(0x14); 
    LCD_WR_DATA(0x06); 
    LCD_WR_DATA(0x0F); 

    LCD_WR_REG(0XE1);    //Set Gamma 8.3.25
    LCD_WR_DATA(0x00); 
    LCD_WR_DATA(0x1C); 
    LCD_WR_DATA(0x1F); 
    LCD_WR_DATA(0x03); 
    LCD_WR_DATA(0x0F); 
    LCD_WR_DATA(0x05); 
    LCD_WR_DATA(0x37); 
    LCD_WR_DATA(0x24); 
    LCD_WR_DATA(0x4C); 
    LCD_WR_DATA(0x04); 
    LCD_WR_DATA(0x0E); 
    LCD_WR_DATA(0x0C); 
    LCD_WR_DATA(0x30); 
    LCD_WR_DATA(0x34); 
    LCD_WR_DATA(0x0F); 
    //**************************************************************
    LCD_WR_REG(0x2A);//set_column_address //240 8.2.20
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x3f);

    LCD_WR_REG(0x2B);// set_page_address //320 8.2.21
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xEF);

    LCD_WR_REG(0x11);//exit_sleep_mode
    lcd_delay_ms(120);
    LCD_WR_REG(0x29);//set_display_on
    lcd_delay_ms(25);
    LCD_WR_REG(0x2c);//send DDRAM set

    LCD_Clear(WHITE);	
    LCD_LED_L();//点亮背光	 
}


//清屏函数
//Color:要清屏的填充色
void LCD_Clear(uint16_t Color)
{
    uint32_t index=0;      
    lcd_set_area(0, 0,320, 240);
    LCD_WR_REG(0x2c);     //开始写入GRAM	 	  
    for(index=0;index<76800;index++)
    {
        LCD_WR_DATA(Color);    
    }
}  

//画线
//x1,y1:起点坐标
//x2,y2:终点坐标  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  uint16_t t; 
  int xerr=0,yerr=0,delta_x,delta_y,distance; 
  int incx,incy,uRow,uCol; 
  
  delta_x=x2-x1; //计算坐标增量 
  delta_y=y2-y1; 
  uRow=x1; 
  uCol=y1; 
  if(delta_x>0)incx=1; //设置单步方向 
  else if(delta_x==0)incx=0;//垂直线 
  else {incx=-1;delta_x=-delta_x;} 
  if(delta_y>0)incy=1; 
  else if(delta_y==0)incy=0;//水平线 
  else{incy=-1;delta_y=-delta_y;} 
  if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
  else distance=delta_y; 
  for(t=0;t<=distance+1;t++ )//画线输出 
  {  
    LCD_DrawPoint(uRow,uCol,color);//画点 
    xerr+=delta_x ; 
    yerr+=delta_y ; 
    if(xerr>distance) 
    { 
      xerr-=distance; 
      uRow+=incx; 
    } 
    if(yerr>distance) 
    { 
      yerr-=distance; 
      uCol+=incy; 
    } 
  }  
}    

//画矩形
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
  LCD_DrawLine(x1,y1,x2,y1,color);
  LCD_DrawLine(x1,y1,x1,y2,color);
  LCD_DrawLine(x1,y2,x2,y2,color);
  LCD_DrawLine(x2,y1,x2,y2,color);
}

//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void LCD_DrawCircle(uint16_t x0,uint16_t y0,uint16_t r,uint16_t color)
{
  int a,b;
  int di;
  a=0;b=r;	  
  di=3-(r<<1);             //判断下个点位置的标志
  while(a<=b)
  {
    LCD_DrawPoint(x0-b,y0-a,color);             //3           
    LCD_DrawPoint(x0+b,y0-a,color);             //0           
    LCD_DrawPoint(x0-a,y0+b,color);             //1       
    LCD_DrawPoint(x0-b,y0-a,color);             //7           
    LCD_DrawPoint(x0-a,y0-b,color);             //2             
    LCD_DrawPoint(x0+b,y0+a,color);             //4               
    LCD_DrawPoint(x0+a,y0-b,color);             //5
    LCD_DrawPoint(x0+a,y0+b,color);             //6 
    LCD_DrawPoint(x0-b,y0+a,color);             
    a++;
    //使用Bresenham算法画圆     
    if(di<0)di +=4*a+6;	  
    else
    {
      di+=10+4*(a-b);   
      b--;
    } 
    LCD_DrawPoint(x0+a,y0+b,color);
  }
} 



//在指定位置显示一个字符
//x:0~312
//y:0~224
//num:要显示的字符:" "--->"~"
//size:字体大小 16/48
//fcolor:前景色
//bcolor:背景色
#define MAX_CHAR_POSX 320
#define MAX_CHAR_POSY 240
void lcd_putchar(uint16_t x,uint16_t y,uint8_t num,uint16_t size,uint16_t fcolor,uint16_t bcolor)
{       
  uint8_t m;
  uint8_t pos,t,i;      
  if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
  
  if(size == 24)
  {
    //水平方向GRAM结束地址
    lcd_set_area(x, y, x+(16-1),y+size-1);
  }
  else if(size == 16)
  {
    //水平方向GRAM结束地址
    lcd_set_area(x, y, x+(8-1),y+size-1);
  }
  else if(size == 32)
  {
    //水平方向GRAM结束地址
    lcd_set_area(x, y, x+(24-1),y+size-1);
  }
  else if(size == 48)
  {
    //水平方向GRAM结束地址
    lcd_set_area(x, y, x+(24-1),y+size-1);
  }
  else if(size == 64)
  {
    //水平方向GRAM结束地址
    lcd_set_area(x, y, x+(32-1),y+size-1);
  }
  else
  {
    
  }
  LCD_WR_REG(0x2c);       //开始写入GRAM	    
  num=num-' ';//得到偏移后的值
  
  
  
  
  if(size == 24)
  {
    for(pos=0;pos<size;pos++)
    {
      for(i=0;i<2;i++)
      {
        m = ASC12_24[num*48+pos*2+i];
        for(t=0;t<8;t++)
        {          				
          if((m&0x80)==0x80)
          {
            LCD_WR_DATA(fcolor);					
          }
          else
          {
            LCD_WR_DATA(bcolor);					
          }
          m<<=1;
        }		
      }
      
    }	
  }
  else if(size == 16)
  {
    for(pos=0;pos<size;pos++)
    {
      m = ASC8_16[num*16+pos] - ASC8_16[pos];
      for(t=0;t<8;t++)
      {          				
        if((m&0x80)==0x80)
        {
          LCD_WR_DATA(fcolor);					
        }
        else
        {
          LCD_WR_DATA(bcolor);					
        }				
        m<<=1;
      }
      
    }
  }
  else if (size == 32){
    for(pos = 0;pos < size;pos++)
    {
      for(i = 0;i < 3;i++)
      {
        m = ASC_24_32[num*96+pos*3+i];
        for(t=0;t<8;t++)
        {          				
          if((m&0x80)==0x80)
          {
            LCD_WR_DATA(fcolor);					
          }
          else
          {
            LCD_WR_DATA(bcolor);					
          }
          m<<=1;
        }
      }
    }
  }
  
  else if (size == 48){
    for(pos = 0;pos < size;pos++)
    {
      for(i = 0;i < 3;i++)
      {
        m = ASC_24_48[num*144+pos*3+i];
        for(t=0;t<8;t++)
        {          				
          if((m&0x80)==0x80)
          {
            LCD_WR_DATA(fcolor);					
          }
          else
          {
            LCD_WR_DATA(bcolor);					
          }				
          m<<=1;
        }
      }
    }
  }
  else if (size == 64){
    for(pos = 0;pos < size;pos++)
    {
      for(i = 0;i < 4;i++)
      {
        m = ASC_32_64[num*256+pos*4+i];
        for(t=0;t<8;t++)
        {          				
          if((m&0x80)==0x80)
          {
            LCD_WR_DATA(fcolor);					
          }
          else
          {
            LCD_WR_DATA(bcolor);					
          }				
          m<<=1;
        }		
      }			
    }
  }
  
  
  else
  {
    
  }
  
  
  
  //恢复窗体大小	 
  lcd_set_area(0, 0,320, 240);
}  



void lcd_putGB(uint16_t x, uint16_t  y, uint8_t c[2], uint16_t size,uint16_t fcolor, uint16_t bcolor)
{
  uint8_t m;
  uint16_t i,j,k;
  
  if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
  
  //水平方向GRAM起始地址
  if(size == 24)
  {
    //水平方向GRAM结束地址
    lcd_set_area(x, y, x+(24-1),y+size-1);
  }
  else if(size == 16)
  {
    //水平方向GRAM结束地址
    lcd_set_area(x, y, x+(16-1),y+size-1);
  }
  else if (size == 32){
    //水平方向GRAM结束地址
    lcd_set_area(x, y, x+(32-1),y+size-1);
  }
  else if (size == 48){
    //水平方向GRAM结束地址
    lcd_set_area(x, y, x+(48-1),y+size-1);
  }
  else
  {
    
  }
  LCD_WR_REG(0x2c);       //开始写入GRAM	
  
  
  
  
  if(size == 24)
  {
    for (k=0;k<GB_24_MAX_CONT;k++) 
    {
      if ((GB_24[k].Index[0]==c[0])&&(GB_24[k].Index[1]==c[1]))
      { 
        for(i=0;i<72;i++) 
        {
          m=GB_24[k].Msk[i];
          for(j=0;j<8;j++) 
          {
            if((m&0x80)==0x80)
            {
              LCD_WR_DATA(fcolor);					
            }
            else
            {
              LCD_WR_DATA(bcolor);					
            }
            
            m<<=1;
          } 
        }
      }  
    }
  }
  else if(size == 32)
  {
    for (k=0;k<GB_32_MAX_CONT;k++) 
    {
      if ((GB_32[k].Index[0]==c[0])&&(GB_32[k].Index[1]==c[1]))
      { 
        for(i=0;i < 128;i++) 
        {
          m=GB_32[k].Msk[i];
          for(j=0;j<8;j++) 
          {
            if((m&0x80)==0x80)
            {
              LCD_WR_DATA(fcolor);					
            }
            else
            {
              LCD_WR_DATA(bcolor);					
            }					
            m<<=1;
          } 
        }
      }  
    }
  }
  else if(size == 16)
  {
    for (k=0;k<GB_16_MAX_CONT;k++) 
    {
      if ((GB_16[k].Index[0]==c[0])&&(GB_16[k].Index[1]==c[1]))
      { 
        for(i=0;i<32;i++) 
        {
          m=GB_16[k].Msk[i];
          for(j=0;j<8;j++) 
          {
            if((m&0x80)==0x80)
            {
              LCD_WR_DATA(fcolor);					
            }
            else
            {
              LCD_WR_DATA(bcolor);					
            }
            
            m<<=1;
          } 
        }
      }  
    }	
  }	
  else if(size == 48)
  {
    
  }
  else
  {
    
  }
  
  
  
  
  //恢复窗体大小	 
  lcd_set_area(0, 0,320, 240);
}





void LCD_Putstring(uint16_t x, uint16_t y, char *s,uint16_t len,uint16_t size,uint16_t fcolor, uint16_t bcolor) 
{
  
  uint16_t i = 0;
  for(i = 0;i < len && (*s != 0);)
  {
	
    if( *s < 0x80 ) 
    {
      
      if(size == 24)
      {
        if(x>(MAX_CHAR_POSX-16)){x=0;y+=24;}
        if(y>(MAX_CHAR_POSY-24)){y=x=0;LCD_Clear(WHITE);}			
      }
      else if(size == 16)
      {
        if(x>(MAX_CHAR_POSX-8)){x=0;y+=16;}
        if(y>(MAX_CHAR_POSY-16)){y=x=0;LCD_Clear(WHITE);}				
      }
      else if(size == 32)
      {
        if(x>(MAX_CHAR_POSX-24)){x=0;y+=32;}
        if(y>(MAX_CHAR_POSY-32)){y=x=0;LCD_Clear(WHITE);}				
      }
      else if(size == 48){
        if(x>(MAX_CHAR_POSX-24)){x=0;y+=48;}
        if(y>(MAX_CHAR_POSY-48)){y=x=0;LCD_Clear(WHITE);}	
      }
      else if(size == 64){
        if(x>(MAX_CHAR_POSX-32)){x=0;y+=64;}
        if(y>(MAX_CHAR_POSY-64)){y=x=0;LCD_Clear(WHITE);}	
      }
      else
      {
        
      }		
      
      lcd_putchar(x,y,*s,size,fcolor,bcolor);
      if(size == 24)
      {
        s++;x+=16;		
        i++;
      }
      else if(size == 16)
      {
        s++;x+=8;
        i++;
      }
      else if(size == 48){
        s++;x+=24;
        i++;
      }
      else if(size == 32){
        s++;x+=24;
        i++;
      }
      else if(size == 64){
        s++;x+=32;
        i++;
      }
      else
      {
        
      }
      
      
    }
    else 
    {			
      
      if(size == 24)
      {
        if(x>(MAX_CHAR_POSX-24)){x=0;y+=24;}
        if(y>(MAX_CHAR_POSY-24)){y=x=0;LCD_Clear(WHITE);}			
      }
      else if(size == 16)
      {
        if(x>(MAX_CHAR_POSX-16)){x=0;y+=16;}
        if(y>(MAX_CHAR_POSY-16)){y=x=0;LCD_Clear(WHITE);}				
      }
      else if(size == 48){
        if(x>(MAX_CHAR_POSX-48)){x=0;y+=48;}
        if(y>(MAX_CHAR_POSY-48)){y=x=0;LCD_Clear(WHITE);}	
      }
      else if(size == 32){
        if(x>(MAX_CHAR_POSX-32)){x=0;y+=32;}
        if(y>(MAX_CHAR_POSY-32)){y=x=0;LCD_Clear(WHITE);}	
      }
      else if(size == 64){
        if(x>(MAX_CHAR_POSX-64)){x=0;y+=64;}
        if(y>(MAX_CHAR_POSY-64)){y=x=0;LCD_Clear(WHITE);}	
      }
      else
      {
        
      }			
      
      lcd_putGB(x,y,(uint8_t *)s,size,fcolor,bcolor);
      if(size == 24)
      {
        s+=2;x+=24;
        i += 2;				
      }
      else if(size == 16)
      {
        s+=2;x+=16;
        i += 2;				
      }
      else if(size == 32)
      {
        s+=2;x+=32;
        i += 2;				
      }
      else if(size == 48){
        s+=2;x+=48;
        i += 2;	
      }
      else if(size == 64){
        s+=2;x+=32;
        i += 2;	
      }
      else
      {
        
      }			
      
      
    }		
  }
}

void LCD_Darwpic(uint16_t x, uint16_t y,uint16_t num)
{
  uint32_t i=0;      
  uint32_t j=0;     
  
  uint16_t pic2_w=112;
  uint16_t pic2_h=25;
  uint32_t pic2_size = 5608;
  switch(num)
  {
  case 1:
    {
      
    }break;
  case 2:
    {
      
      
      lcd_set_area(x, y, x+pic2_w-1,y+pic2_h-1);	
      LCD_WR_REG(0x2c);       //开始写入GRAM	  
      
      for(i=0;i<pic2_size/2;i++)
      {
        LCD_WR_DATA(gImage_2[j]+gImage_2[j+1]*256);    
        j+=2;
      }				
    }break;
  case 3:break;
  default:break;
  }
  
  //恢复窗体大小	 
  lcd_set_area(0, 0,320, 240);	
  
}  









/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**************************以下显示字符方法是通过放大算法实现的***************************************************/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LCD_MIN(a, b)   (((a) > (b) ? (b):(a)))

//最大为8倍
static uint8_t font_str[32*64];

uint8_t lcd_mag_get_bit(uint8_t* in_data,uint16_t larg,uint16_t x,uint16_t y)
{
  uint16_t tmp;
  uint16_t tmp2;
  uint16_t tmp3;	
  uint16_t tmp4;		
  tmp=larg*y;
  tmp/=8;
  tmp2 = x/8;
  tmp += tmp2;
  tmp3 = 7 - x%8;
  tmp4 = in_data[tmp];
  tmp4 >>=tmp3;
  tmp4 &= 0x01;
  return tmp4;
}

void lcd_mag_set_bit(uint8_t* in_data,uint16_t larg,uint16_t y,uint16_t x,uint8_t val)
{
  uint16_t tmp;
  uint16_t tmp2;
  uint16_t tmp3;	
  tmp=larg*y;
  tmp/=8;
  tmp2 = x/8;
  tmp += tmp2;
  tmp3 = 7 - x%8;
  if(val != 0) 
  {	
    in_data[tmp] |= (1<<tmp3);
  }
  else
  {
    in_data[tmp] &= ~(1<<tmp3);
  }
}


int lcd_magnify(uint8_t in_x,uint8_t in_y,uint8_t in_magnify,uint8_t* in_data,uint8_t* out_data)
{
  uint16_t p,q,a,b,c,d,e,f;
  uint16_t i,j;
  uint16_t tmp_xy;
  uint8_t  tmp;
  
  tmp_xy = in_x;
  tmp_xy*=in_y;
  
  for(i=0;i<in_y*in_magnify;i++)
  {
    for(j=0;j<in_x*in_magnify;j++)
    {
      p = in_y * i/(in_y*in_magnify);
      q = in_x * j/(in_x*in_magnify);
      a = lcd_mag_get_bit(in_data,8,q,p);
      b = lcd_mag_get_bit(in_data,8,q,p+1);
      c = lcd_mag_get_bit(in_data,8,q+1,p);
      d = lcd_mag_get_bit(in_data,8,q+1,p+1);
      e = LCD_MIN(in_y ,in_y*in_magnify - in_y*p%(in_y*in_magnify));
      f = LCD_MIN(in_x ,in_x*in_magnify - in_x*q%(in_x*in_magnify));
      tmp = (e*f*a +(in_y - e)*f*b +e*(in_x-f)*c + (in_y - e)*(in_x - f)*d)/tmp_xy;
      lcd_mag_set_bit(out_data,in_x*in_magnify,i,j,tmp);
    }	
  }
  return 0;
}

//在指定位置显示一个字符
//x:0~234
//y:0~308
//
void LCD_ShowCharX(uint16_t x,uint16_t y,uint8_t num,uint16_t magnify,uint16_t fcolor, uint16_t bcolor)
{       
  uint16_t temp;
  uint16_t pos,t;      
  if(x>MAX_CHAR_POSX||y>MAX_CHAR_POSY)return;	    
  
  
  lcd_set_area(x, y, x+(16*magnify/2-1),y+16*magnify-1);
  LCD_WR_REG(0x2c);       //开始写入GRAM	  
  
  
  if(magnify>8) magnify = 8;
  
  lcd_magnify(8,16,magnify,(uint8_t*)&Font8x16[num*16],(uint8_t*)font_str);
  
  
  for(pos=0;pos<16*magnify;pos++)
  {
    for(t=0;t<16*magnify/2;t++)
    {                 
      temp = lcd_mag_get_bit(font_str,16*magnify/2,t,pos);
      if(temp==0x01)
      {
        LCD_WR_DATA(fcolor);
      }else 
      {
        LCD_WR_DATA(bcolor);
      }
    }
  }	
  
  
  //恢复窗体大小	 
  lcd_set_area(0, 0,320, 240);
}

//显示字符串
//x,y:起点坐标  
//*p:字符串起始地址
//用16字体
void LCD_ShowStringX(uint16_t x,uint16_t y,const uint8_t *p,uint16_t len,uint16_t mg,uint16_t fcolor, uint16_t bcolor)
{         
  while(len--)
  {       
    if(x>MAX_CHAR_POSX-8){x=0;y+=16;}
    if(y>MAX_CHAR_POSY-16){y=x=0;LCD_Clear(WHITE);}
    LCD_ShowCharX(x,y,*p,mg,fcolor,bcolor);
    x+=(8*mg);
    p++;
  }
}














