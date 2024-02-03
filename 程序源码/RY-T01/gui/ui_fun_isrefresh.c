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

    if (ui_cur_state == HANDLE_BLOW || ui_cur_state == HANDLE_BLOW_ING)
	{
		res = 1;
	}
    if (ui_cur_state == FLOW_ADJUST || ui_cur_state == FLOW_ADJUST_ING)
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
	if (ui_cur_state == BLOW_INTER_SET)
	{
		res = 1;
	}
	if (ui_cur_state == AUTO_CALIB_ZERO)
	{
		res = 1;
	}
	if (ui_cur_state == SPEED_K_SET)
	{
		res = 1;
	}
	if (ui_cur_state == PITG_K_SET)
	{
		res = 1;
	}
	if (ui_cur_state == HUMIT_ZERO_CALIB)
	{
		res = 1;
	}
	if (ui_cur_state == CANCEL_HUMIT_ZERO)
	{
		res = 1;
	}
	if (ui_cur_state == HUMIT_UNIT_SELECT)
	{
		res = 1;
	}
	if (ui_cur_state == RESTORE_FACTORY)
	{
		res = 1;
	}
    
	return res;
}
