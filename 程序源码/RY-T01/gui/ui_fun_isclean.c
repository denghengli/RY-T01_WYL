#include "includes.h"

uint8_t is_clean(void)
{
	unsigned char res = 1;

	/* 主界面显示    */
	if (ui_cur_state == MAIN_WIND)
	{
		res = 0;
	}
	
	return res;
}


