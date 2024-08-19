#include "includes.h"

const char *menu_list[] = 
{
//	"设备状态    ",
	"手动反吹    ",
	"流速零点校准",		
	"平滑时间    ",		
	"烟道截面积  ",		
    "反吹频次    ",	
    "参数设置    ",	
    "            ",
    "            ",
    "            ",
    "            ",
};

void config_menu(void *para)
{
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
	sprintf(tmp_str, "菜单");
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
			sprintf(tmp_str, "%s", menu_list[page * 5 + i]);
		}	
		else
		{
		    lcd_para.x = 48;
		    lcd_para.bc = DARKBLUE;			
			lcd_para.fc = WHITE;			
		    sprintf(tmp_str, "  ");
		    hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
		    
		    lcd_para.x = 84;			
		    sprintf(tmp_str, "%s", menu_list[page * 5 + i]);
		}
		hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
	}
}


void config_menu_one_select_1(void *para)
{
	unsigned int i = 0;
	config_menu(&i);    
}

void config_menu_one_select_2(void *para)  
{
    unsigned int i = 1;
	config_menu(&i);
}

void config_menu_one_select_3(void *para)
{
    unsigned int i = 2;
	config_menu(&i);    
}

void config_menu_one_select_4(void *para)
{
    unsigned int i = 3;
	config_menu(&i);    
}

void config_menu_one_select_5(void *para)
{
    unsigned int i = 4;
	config_menu(&i);    
}

void config_menu_two_select_1(void *para)
{
    unsigned int i = 5;
	config_menu(&i);    
}


