#include "includes.h"
#include "font_ascii.h"
#include "font_gb.h"

//-----------------LCD�˿ڶ���---------------- 
#define LCD_BLK_Clr()  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12)//BLK
#define LCD_BLK_Set()  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_12)
#define LCD_RES_Clr()  LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_14)//RES
#define LCD_RES_Set()  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_14)
#define LCD_DC_Clr()   LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1)//DC
#define LCD_DC_Set()   LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_1)
#define LCD_CS_Clr()   LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4)//CS
#define LCD_CS_Set()   LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_4)

static void LCD_GPIO_Init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    
    /**SPI1 GPIO Configuration
    PB1   ------> DC
    PB14  ------> RESET
    PA12  ------> ����
    PA4   ------> CS
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1 | LL_GPIO_PIN_14;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_12);
}

static void LCD_DelayMs(uint32_t m)
{
    volatile uint32_t i, j;
    for (i=0; i<m; i++)
        for (j=0; j<10000; j++);
}


/******************************************************************************
����˵����LCD��������д�뺯��
������ݣ�dat  Ҫд��Ĵ�������
����ֵ��  ��
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
	LCD_CS_Clr();
	spi1SendData(dat);
	LCD_CS_Set();
}

/******************************************************************************
����˵����LCDд������
������ݣ�dat д�������
����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
����˵����LCDд������
������ݣ�dat д�������
����ֵ��  ��
******************************************************************************/
void LCD_WR_DATA16(uint16_t dat)
{
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
����˵����LCDд������
������ݣ�dat д�������
����ֵ��  ��
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
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
void LCD_SetWindows(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    LCD_WR_REG(0x2a);//�е�ַ����
    LCD_WR_DATA16(x1);
    LCD_WR_DATA16(x2);
    LCD_WR_REG(0x2b);//�е�ַ����
    LCD_WR_DATA16(y1);
    LCD_WR_DATA16(y2);
    LCD_WR_REG(0x2c);//������д
}


void LCD_Init(void)
{
	MX_SPI1_Init();
	LCD_GPIO_Init();//��ʼ��GPIO
	
    LCD_RES_Clr();
    LCD_DelayMs(5);
    LCD_RES_Set();
    LCD_DelayMs(5);
//    LCD_BLK_Set(); //������
//    LCD_DelayMs(100);
    
    //************* Start Initial Sequence **********//	
    LCD_WR_REG(0x11);
    LCD_DelayMs(100); //Delay 120ms
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

//��������
//Color:Ҫ���������ɫ
void LCD_Clear(uint16_t Color)
{
    uint16_t i=0, j=0;  
    
    LCD_SetWindows(0, 0, LCD_W - 1, LCD_H - 1);
 	  
    for(i=0; i<LCD_W; i++)
    {
        for (j=0; j<LCD_H; j++)
        {
            LCD_WR_DATA16(Color);
        }
    }
}  


/******************************************************************************
����˵������ָ�����������ɫ
������ݣ�xsta,ysta   ��ʼ����
          xend,yend   ��ֹ����
          color       Ҫ������ɫ
����ֵ��  ��
******************************************************************************/
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_SetWindows(xsta,ysta,xend-1,yend-1);//������ʾ��Χ
	for(i=ysta;i<yend;i++)
	{													   	 	
		for(j=xsta;j<xend;j++)
		{
			LCD_WR_DATA16(color);
		}
	}
    
    LCD_SetWindows(0, 0, LCD_W - 1, LCD_H - 1); //�ָ���������Ϊȫ��
}

/******************************************************************************
      ����˵������ָ��λ�û���
      ������ݣ�x,y ��������
                color �����ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
	LCD_SetWindows(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA16(color);
} 


/******************************************************************************
  ����˵��������
  ������ݣ�x1,y1   ��ʼ����
            x2,y2   ��ֹ����
            color   �ߵ���ɫ
  ����ֵ��  ��
******************************************************************************/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//����
		xerr+=delta_x;
		yerr+=delta_y;
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


/******************************************************************************
  ����˵����������
  ������ݣ�x1,y1   ��ʼ����
            x2,y2   ��ֹ����
            color   ���ε���ɫ
  ����ֵ��  ��
******************************************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
  ����˵������Բ
  ������ݣ�x0,y0   Բ������
            r       �뾶
            color   Բ����ɫ
  ����ֵ��  ��
******************************************************************************/
void LCD_DrawCircle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}

/******************************************************************************
  ����˵������ʾͼƬ
  ������ݣ�x,y�������
            length ͼƬ����
            width  ͼƬ���
            pic[]  ͼƬ����    
  ����ֵ��  ��
******************************************************************************/
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[])
{
	uint16_t i,j;
	uint32_t k=0;
    
	LCD_SetWindows(x, y, x+length-1, y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}

//******************************************************************
//��������  LCD_ShowChar
//���ܣ�    ��ʾ����Ӣ���ַ�
//���������(x,y):�ַ���ʾλ����ʼ����
//        	fc:ǰ�û�����ɫ
//			bc:������ɫ
//			num:��ֵ��0-94��
//			size:�����С 12\16\24\32
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************  
void LCD_ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t num, uint8_t size)
{  
    uint16_t i,j,m=0;
    uint16_t sizex, charByteNum;//һ���ַ���ռ�ֽڴ�С
    uint8_t data;
    
    sizex = size/2;
	//charByteNum = (sizex/8+((sizex%8)?1:0))*size;
    if (size == 12) charByteNum = 12;
    else if (size == 16) charByteNum = 16;
    else if (size == 24) charByteNum = 48;
    else if (size == 32) charByteNum = 64;
    else return;
    
	num = num - ' ';//�õ�ƫ�ƺ��ֵ
	LCD_SetWindows(x, y, x+size/2-1, y+size-1);//���õ���������ʾ����
    
    for (i=0; i<charByteNum; i++)
    {
        if (size == 12) data = ascii_1206[num][i];//����6x12����
        else if (size == 16) data = ascii_1608[num][i];//����8x16����
        else if (size == 24) data = ascii_2412[num][i];//����12x24����
        else if (size == 32) data = ascii_3216[num][i];//����16x32����
        
        for(j=0; j<8; j++)
        {
            if(data&(0x01<<j))LCD_WR_DATA16(fc);
            else LCD_WR_DATA16(bc);
            m++;
            if(m%sizex == 0)
            {
                m = 0;
                break;
            }
        }
    }  	   	 	  
} 

/******************************************************************************
  ����˵������ʾ����12x12����
  ������ݣ�x,y��ʾ����
            *s Ҫ��ʾ�ĺ���
            fc �ֵ���ɫ
            bc �ֵı���ɫ
            sizey �ֺ�
            mode:  0�ǵ���ģʽ  1����ģʽ
  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(FONT_GB12)/sizeof(typFNT_GB12);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if((FONT_GB12[k].Index[0]==*(s))&&(FONT_GB12[k].Index[1]==*(s+1)))
		{ 	
			LCD_SetWindows(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(FONT_GB12[k].Msk[i]&(0x01<<j))LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(FONT_GB12[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
  ����˵������ʾ����16x16����
  ������ݣ�x,y��ʾ����
            *s Ҫ��ʾ�ĺ���
            fc �ֵ���ɫ
            bc �ֵı���ɫ
            sizey �ֺ�
            mode:  0�ǵ���ģʽ  1����ģʽ
  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
    uint8_t i,j,m=0;
    uint16_t k;
    uint16_t HZnum;//������Ŀ
    uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    uint16_t x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=sizeof(FONT_GB16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((FONT_GB16[k].Index[0]==*(s))&&(FONT_GB16[k].Index[1]==*(s+1)))
		{ 	
			LCD_SetWindows(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(FONT_GB16[k].Msk[i]&(0x01<<j))LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(FONT_GB16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 


/******************************************************************************
  ����˵������ʾ����24x24����
  ������ݣ�x,y��ʾ����
            *s Ҫ��ʾ�ĺ���
            fc �ֵ���ɫ
            bc �ֵı���ɫ
            sizey �ֺ�
            mode:  0�ǵ���ģʽ  1����ģʽ
  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(FONT_GB24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((FONT_GB24[k].Index[0]==*(s))&&(FONT_GB24[k].Index[1]==*(s+1)))
		{ 	
			LCD_SetWindows(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(FONT_GB24[k].Msk[i]&(0x01<<j))LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(FONT_GB24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
  ����˵������ʾ����32x32����
  ������ݣ�x,y��ʾ����
            *s Ҫ��ʾ�ĺ���
            fc �ֵ���ɫ
            bc �ֵı���ɫ
            sizey �ֺ�
            mode:  0�ǵ���ģʽ  1����ģʽ
  ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese32x32(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i,j,m=0;
	uint16_t k;
	uint16_t HZnum;//������Ŀ
	uint16_t TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(FONT_GB32)/sizeof(typFNT_GB32);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((FONT_GB32[k].Index[0]==*(s))&&(FONT_GB32[k].Index[1]==*(s+1)))
		{ 	
			LCD_SetWindows(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(FONT_GB32[k].Msk[i]&(0x01<<j))LCD_WR_DATA16(fc);
						else LCD_WR_DATA16(bc);
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(FONT_GB32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}

//******************************************************************
//��������  LCD_Putstring
//���ܣ�    ��ʾһ���ַ���,������Ӣ����ʾ
//���������x,y :�������
// 			fc:ǰ�û�����ɫ
//			bc:������ɫ
//			str :�ַ���	 
//			size:�����С
//			mode:ģʽ	0,���ģʽ;1,����ģʽ
//����ֵ��  ��
//�޸ļ�¼����
//******************************************************************    	
void LCD_Putstring(uint16_t x, uint16_t y, uint8_t *str, uint16_t len, uint16_t size, uint16_t fcolor, uint16_t bcolor) 
{					
    uint16_t x0 = x;							  	  
    uint16_t i = 0;
    
    if (size != 12 && size != 16 && size != 24 && size != 32)
        return;
    
    for(i = 0;i < len && (*str != 0);)
    { 
        if(*str < 0x80) //�ַ�
        {
            if(x>(LCD_W-size/2)||y>(LCD_H-size)) 
                return; 

            if(*str == 0x0D)//���з���
            {         
                y += size;
                x = x0;
                str++; 
            }  
            else
            {
                LCD_ShowChar(x, y, fcolor, bcolor, *str, size);
                x += size/2; //�ַ�,Ϊȫ�ֵ�һ��
            } 
            str++; 
        }
        else//���� 
        {   
            if(x>(LCD_W-size)||y>(LCD_H-size)) 
                return;  

            if(size == 12) LCD_ShowChinese12x12(x,y,str,fcolor,bcolor,size, 0);
            else if(size == 16) LCD_ShowChinese16x16(x,y,str,fcolor,bcolor,size, 0);
            else if(size == 24) LCD_ShowChinese24x24(x,y,str,fcolor,bcolor,size, 0);
            else if(size == 32) LCD_ShowChinese32x32(x,y,str,fcolor,bcolor,size, 0);

            str += 2;
            x += size;//��һ������ƫ��  
        }						 
    }   
}






