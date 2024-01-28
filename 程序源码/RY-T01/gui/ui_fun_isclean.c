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

	//平滑时间
    if ((ui_cur_state == SMOOTH_TIME_SET_UP || ui_cur_state == SMOOTH_TIME_SET_DOWN || ui_cur_state == SMOOTH_TIME_SET_RIGHT) && ui_pre_state == SMOOTH_TIME_SET)
    {
        res = 0;
    }
    //烟道截面积
    if ((ui_cur_state == SECTION_AREA_SET_UP || ui_cur_state == SECTION_AREA_SET_DOWN || ui_cur_state == SECTION_AREA_SET_RIGHT) && ui_pre_state == SECTION_AREA_SET)
    {
        res = 0;
    }
    //反吹频次
    if ((ui_cur_state == BLOW_FREQ_SET_UP || ui_cur_state == BLOW_FREQ_SET_DOWN || ui_cur_state == BLOW_FREQ_SET_RIGHT) && ui_pre_state == BLOW_FREQ_SET)
    {
        res = 0;
    }
    //密码登录
    if ((ui_cur_state == PASSWORD_LOGIN_UP || ui_cur_state == PASSWORD_LOGIN_DOWN || ui_cur_state == PASSWORD_LOGIN_RIGHT) && ui_pre_state == PASSWORD_LOGIN)
    {
        res = 0;
    }
	
	return res;
}


