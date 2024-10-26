#include "includes.h"

uint8_t cur_main_pagenum = 1; //主界面页号

/*上电起始界面*/
void start_logo(void *para)
{
	FNC_LCD_DISP_DRAW_PARA lcd_para;
	char tmp_str[32] = {0};
	uint8_t buf[8] = {0};

	lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
	lcd_para.bc = WHITE;
	lcd_para.fc = DARKBLUE;
	lcd_para.mode = 24;
	lcd_para.x = 32;
	lcd_para.y = LCD_H / 2 - 60 - 30;
	strncpy(tmp_str, "RY-T01", sizeof(tmp_str));
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

	lcd_para.cmd = FNC_LCD_DISP_DRAW_LINE;
	lcd_para.bc = DARKBLUE;
	lcd_para.fc = DARKBLUE;
	lcd_para.mode = 24;
	lcd_para.x = 32;
	lcd_para.y = 85 - 25;
	buf[0] = 32 + 24 * 4;
	buf[1] = 0;
	buf[2] = 85 - 25;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);

	lcd_para.x = 32;
	lcd_para.y = 87 - 25;
	buf[0] = 32 + 24 * 4;
	buf[1] = 0;
	buf[2] = 87 - 25;
	buf[3] = 0;
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)buf, 4);

	lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
	lcd_para.bc = WHITE;
	lcd_para.fc = DARKBLUE;
	lcd_para.mode = 24;
	lcd_para.x = 32;
	lcd_para.y = LCD_H / 2 - 30 - 20;
	strncpy(tmp_str, "温压流湿一体监测仪", sizeof(tmp_str));
	hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
	
    vTaskDelay(1000 / portTICK_RATE_MS);
	ui_cur_state = MAIN_WIND;
	clean_sercen();
}

#if DZ_VERSION
void main_wind(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    static int _ex_sec_signal = 1;
    static int display_sec = 0;
    
    if (ex_sec_signal != _ex_sec_signal)    
    {
        _ex_sec_signal = ex_sec_signal;
        display_sec++;
    }
     
    if (display_sec >= 3)
    {
        display_sec = 0;
        
        /*温度*/
        lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
        lcd_para.x = 20;
        lcd_para.y = 20;
        lcd_para.bc = DARKBLUE;
        lcd_para.fc = WHITE;
        lcd_para.mode = 24;
        snprintf(tmp_str, sizeof(tmp_str), "  温度： %7.2f  ℃", g_SysData.Data.Sample.ptTem);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

        /*流速*/
        lcd_para.y = 60;
        snprintf(tmp_str, sizeof(tmp_str), "  流速： %7.2f  m/s", g_SysData.Data.Sample.speed);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

        /*静压*/
        lcd_para.y = 100;
        snprintf(tmp_str, sizeof(tmp_str), "  静压： %7.2f  Pa", g_SysData.Data.Sample.sticPress);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
        
        /*动压*/
        lcd_para.y = 140;
        snprintf(tmp_str, sizeof(tmp_str), "  动压： %7.2f  Pa", g_SysData.Data.Sample.dynPress);
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
    }
}
#else
void main_wind(void *para)
{
    char tmp_str[64] = {0};
    FNC_LCD_DISP_DRAW_PARA lcd_para;
    static int _ex_sec_signal = 1;
    static int display_sec = 0;
    
    if (ex_sec_signal != _ex_sec_signal)    
    {
        _ex_sec_signal = ex_sec_signal;
        display_sec++;
    }
     
    if (display_sec >= 3)
    {
        display_sec = 0;
        if (cur_main_pagenum == 1)
        {
            /*温度*/
            lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
            lcd_para.x = 20;
            lcd_para.y = 20;
            lcd_para.bc = DARKBLUE;
            lcd_para.fc = WHITE;
            lcd_para.mode = 24;
            snprintf(tmp_str, sizeof(tmp_str), "  温度： %7.2f  ℃", g_SysData.Data.Sample.ptTem);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

            /*湿度*/
            lcd_para.y = 60;
            if (g_SysData.Data.Para.humitDispUnit == 0) //绝对湿度
                snprintf(tmp_str, sizeof(tmp_str), "  湿度： %7.2f  %%Vol", g_SysData.Data.Sample.abshumit);
            else //相对湿度
                snprintf(tmp_str, sizeof(tmp_str), "  湿度： %7.2f  %%RH", g_SysData.Data.Sample.relhumit);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

            /*流速*/
            lcd_para.y = 100;
            snprintf(tmp_str, sizeof(tmp_str), "  流速： %7.2f  m/s", g_SysData.Data.Sample.speed);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

            /*流量*/
            lcd_para.y = 140;
            snprintf(tmp_str, sizeof(tmp_str), "  流量： %7.2f  m3/s", g_SysData.Data.Sample.flow);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
        }
        else if (cur_main_pagenum == 2)
        {
            /*静压*/
            lcd_para.cmd = FNC_LCD_DISP_DRAW_STRING;
            lcd_para.x = 20;
            lcd_para.y = 20;
            lcd_para.bc = DARKBLUE;
            lcd_para.fc = WHITE;
            lcd_para.mode = 24;
            snprintf(tmp_str, sizeof(tmp_str), "  静压： %7.2f  Pa", g_SysData.Data.Sample.sticPress);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

            /*动压*/
            lcd_para.y = 60;
            snprintf(tmp_str, sizeof(tmp_str), "  动压： %7.2f  Pa", g_SysData.Data.Sample.dynPress);
            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));

//            /*反吹*/
//            lcd_para.y = 100;
//            snprintf(tmp_str, sizeof(tmp_str), "  反吹： %7.2f  KPa", g_SysData.Data.Sample.blowGasPress);
//            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
            
//            /*湿度频率*/
//            lcd_para.y = 140;
//            snprintf(tmp_str, sizeof(tmp_str), "  频率： %04d   ", g_SysData.Data.Sample.MSN);
//            hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
        }
        
        /*翻页*/
        lcd_para.x = 200;    
        lcd_para.y = 200;
        lcd_para.fc = YELLOW;
        lcd_para.mode = 24;
        if (cur_main_pagenum == 1)
        {
            sprintf(tmp_str, "%s", "→下一页");
        }
        else if (cur_main_pagenum == 2)
        {
            sprintf(tmp_str, "%s", "→上一页");
        }
        hal_lcd_driver_intface((void *)&lcd_para, (uint8_t *)tmp_str, strlen(tmp_str));
    }
}
#endif

void main_wind_right(void *para)
{
#if !DZ_VERSION
	if (cur_main_pagenum == 1) cur_main_pagenum = 2;
    else if (cur_main_pagenum == 2) cur_main_pagenum = 1;
#endif
    
	ui_cur_state = MAIN_WIND;
}

