#include "includes.h"

uint8_t is_clean(void)
{
	unsigned char res = 1;

	/* 主界面显示    */
	if (ui_cur_state == MAIN_WIND && ui_pre_state == MAIN_WIND)
	{
		res = 0;
	}

	/*配置菜单上下*/
    if (ui_cur_state == CONFIG_MENU_ONE_SELECT_1 && ui_pre_state == CONFIG_MENU_ONE_SELECT_2)
	{
		res = 0;
	}
	if (ui_cur_state == CONFIG_MENU_ONE_SELECT_2 && (ui_pre_state == CONFIG_MENU_ONE_SELECT_1 || ui_pre_state == CONFIG_MENU_ONE_SELECT_3))
	{
		res = 0;
	}
	if (ui_cur_state == CONFIG_MENU_ONE_SELECT_3 && (ui_pre_state == CONFIG_MENU_ONE_SELECT_2 || ui_pre_state == CONFIG_MENU_ONE_SELECT_4))
	{
		res = 0;
	}
	if (ui_cur_state == CONFIG_MENU_ONE_SELECT_4 && (ui_pre_state == CONFIG_MENU_ONE_SELECT_3 || ui_pre_state == CONFIG_MENU_ONE_SELECT_5))
	{
		res = 0;
	}
	if (ui_cur_state == CONFIG_MENU_ONE_SELECT_5 && (ui_pre_state == CONFIG_MENU_ONE_SELECT_4 || ui_pre_state == CONFIG_MENU_TWO_SELECT_1))
	{
		res = 0;
	}
	if (ui_cur_state == CONFIG_MENU_TWO_SELECT_1 && (ui_pre_state == CONFIG_MENU_ONE_SELECT_5 || ui_pre_state == CONFIG_MENU_TWO_SELECT_2))
	{
		res = 0;
	}
	if (ui_cur_state == CONFIG_MENU_TWO_SELECT_2 && ui_pre_state == CONFIG_MENU_TWO_SELECT_1)
	{
		res = 0;
	}
	
	return res;
}


