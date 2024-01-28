#include "includes.h"

uint8_t is_flash_menu(void)
{
	unsigned char res = 0;
	
    /*START LOGO*/
    if (ui_cur_state == START_LOGO)
    {
        res = 1;
    }

    /*	CHECH MENU */
	if (ui_cur_state == MAIN_WIND)
	{
		res = 1;
	}

	if (ui_cur_state == SMOOTH_TIME_SET)
	{
		res = 1;
	}
	if (ui_cur_state == SECTION_AREA_SET)
	{
		res = 1;
	}
	if (ui_cur_state == BLOW_FREQ_SET)
	{
		res = 1;
	}
	if (ui_cur_state == PASSWORD_LOGIN)
	{
		res = 1;
	}
	
	return res;
}
