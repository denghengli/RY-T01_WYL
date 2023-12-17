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
	
	return res;
}
