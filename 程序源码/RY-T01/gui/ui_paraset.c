#include "includes.h"

const char *para_set_list[] = 
{
	"反吹间隔时间  ",
	"自动校零控制  ",
	"速度场系数    ",		
	"皮托管系数    ",		
	"湿度零点校准  ",	
	
    "取消湿度校零值",	
    "湿度单位      ",	
    "恢复出厂设置  ",
    "              ",
    "              "
};

const char *adv_para_set_list[] = 
{
	"反吹间隔时间  ",
	"自动校零控制  ",
	"速度场系数    ",		
	"皮托管系数    ",		
	"湿度零点校准  ",	
	
    "取消湿度校零值",	
    "湿度单位      ",	
    "恢复出厂设置  ",
    "湿度传感器类型",
    "温度模拟量补偿",

    "压力模拟量补偿",
    "流速模拟量补偿",
    "湿度模拟量补偿",
    "              ",
    "              "
};

void para_set(void *para)
{
    unsigned int index = *(unsigned int *)para;
	unsigned int set_pos = 0;
	unsigned int page = 0;
	unsigned int i = 0;
	unsigned char buf[8] = {0};
	char tmp_str[64] = {0};
	FNC_LCD_DISP_DRAW_PARA lcd_para;	
	set_pos = *(unsigned int *)para  % 5;
	page = *(unsigned int *)para / 5;
	
	/*title*/
	lcd_para.cmd = FNC_LCD_DISP_DRAW_LINE;
	lcd_para.bc = DARKBLUE;
	lcd_para.fc = RED;
	lcd_para.mode = 24;
	lcd_para.x = LCD_W / 2 - 40 - 40;
	lcd_para.y = 17;
	buf[0] = LCD_W / 2 - 40;
	buf[1] = 0;
	buf[2] = 17;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);
	lcd_para.x = LCD_W / 2 - 40 - 40;
	lcd_para.y = 18;
	buf[0] = LCD_W / 2 - 40;
	buf[1] = 0;
	buf[2] = 18;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);
	lcd_para.x = (LCD_W / 2) + 40;
	lcd_para.y = 17;
	buf[0] = (unsigned char)((LCD_W / 2) + 40 + 40);
	buf[1] = 0;
	buf[2] = 17;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);
	lcd_para.x = LCD_W / 2 + 40;
	lcd_para.y = 18;
	buf[0] = (unsigned char)(LCD_W / 2 + 40 + 40);
	buf[1] = 0;
	buf[2] = 18;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);
	lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
	lcd_para.bc = DARKBLUE;
	lcd_para.fc = YELLOW;
	lcd_para.mode = 24;
	lcd_para.x = 16;
	lcd_para.y = 0;
	lcd_para.x = LCD_W / 2 - 24; 
	lcd_para.y = 5;
	sprintf(tmp_str, "参数");
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

	/*导航项目*/	
	for (i = 0; i < 5; i++)
	{				
		lcd_para.y = (i + 1) * (LCD_H / 6);			
		/*选中*/
		if (i == set_pos)
		{
		    lcd_para.x = 48;
		    lcd_para.bc = DARKBLUE;			
			lcd_para.fc = WHITE;			
		    sprintf(tmp_str, "→ ");
		    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

		    lcd_para.x = 84;
			lcd_para.fc = DARKBLUE;			
			lcd_para.bc = WHITE;
			//高级配置界面
			if (ui_user == 2) sprintf(tmp_str, "%s", adv_para_set_list[page * 5 + i]);
			else sprintf(tmp_str, "%s", para_set_list[page * 5 + i]);
		}	
		else
		{
		    lcd_para.x = 48;
		    lcd_para.bc = DARKBLUE;			
			lcd_para.fc = WHITE;			
		    sprintf(tmp_str, "  ");
		    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
		    
		    lcd_para.x = 84;	
		    //高级配置界面
			if (ui_user == 2) sprintf(tmp_str, "%s", adv_para_set_list[page * 5 + i]);
			else sprintf(tmp_str, "%s", para_set_list[page * 5 + i]);
		}
		hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
	}
}


void para_set_one_select_1(void *para)
{
	unsigned int i = 0;
	para_set(&i);    
}

void para_set_one_select_2(void *para)  
{
    unsigned int i = 1;
	para_set(&i);
}

void para_set_one_select_3(void *para)
{
    unsigned int i = 2;
	para_set(&i);    
}

void para_set_one_select_4(void *para)
{
    unsigned int i = 3;
	para_set(&i);    
}

void para_set_one_select_5(void *para)
{
    unsigned int i = 4;
	para_set(&i);    
}

void para_set_two_select_1(void *para)
{
    unsigned int i = 5;
	para_set(&i);    
}

void para_set_two_select_2(void *para)
{
    unsigned int i = 6;
	para_set(&i);    
}

void para_set_two_select_3(void *para)
{
    unsigned int i = 7;
	para_set(&i);    
}

//高级配置
void para_set_two_select_4(void *para)
{
    unsigned int i = 8;
	para_set(&i);    
}

void para_set_two_select_5(void *para)
{
    unsigned int i = 9;
	para_set(&i);    
}

void para_set_thr_select_1(void *para)
{
    unsigned int i = 10;
	para_set(&i);    
}

void para_set_thr_select_2(void *para)
{
    unsigned int i = 11;
	para_set(&i);    
}

void para_set_thr_select_3(void *para)
{
    unsigned int i = 12;
	para_set(&i);    
}

