/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-03     Denghengli   the first version
 */
#include  <stdlib.h>
#include  <stdio.h>
#include "drv_soft_timer.h"

static struct soft_timer *soft_timer_list_head = NULL;/*the head of the soft timer list*/

/**********************************************************************************************************
* This function will creat a soft timer
* @param NONE
* @return the soft timer
**********************************************************************************************************/
struct soft_timer *creat_soft_timer(void)
{
	struct soft_timer *new_timer = NULL;
	struct soft_timer *p_timer = NULL;
	
	/* create a new soft timer */
	new_timer = SOFT_TIMER_MALLOC(sizeof(struct soft_timer));
	
	/* does the new soft timer created success */
	if (new_timer == NULL)
	{
		return NULL;
	}
	else
	{
		new_timer->timeout = 0;
		new_timer->count = 0;
		new_timer->mode = SOFT_TIMER_MODE_UNUSED;
		new_timer->status = SOFT_TIMER_STATUS_UNUSED;
	}
	
	/* does the soft timer list already exit */
	if (soft_timer_list_head == NULL)
	{
        new_timer->next = NULL;
		soft_timer_list_head = new_timer;
	}
	else
	{
		/* add the new soft timer to the end of the soft timer list */
		p_timer = soft_timer_list_head;
		while(p_timer->next != NULL)
		{
			p_timer = p_timer->next;
		}
		p_timer->next = new_timer;
		new_timer->next = NULL;
	}
	
	return new_timer;
}

/**********************************************************************************************************
*	Function: soft_timer_del
*	Description: delete a soft timer
*	Parameters: delete timer
*	Return: NONE
**********************************************************************************************************/
void soft_timer_del(struct soft_timer **this_timer)
{
	struct soft_timer *p_timer = soft_timer_list_head;
	struct soft_timer *t_timer = NULL;
	
	/* does this soft timer is vaild */
	if (*this_timer == NULL)
		return;
	
	/* find this timer delete it from timer list */
	while (p_timer != NULL)
	{
		if (p_timer == *this_timer)
		{
			/* head node is the find soft timmer */
			if (p_timer == soft_timer_list_head)
			{
				soft_timer_list_head = p_timer->next;
				SOFT_TIMER_FREE(p_timer);
			}
			else
			{
				t_timer->next = p_timer->next;
				SOFT_TIMER_FREE(p_timer);
			}
		}
		t_timer = p_timer;
		p_timer = p_timer->next;
	}
	
	/* delete external pointer */
	*this_timer = NULL;
}

/**********************************************************************************************************
*	Function: soft_timer_start
*	Description: start a soft timer
*	Parameters:  start timer
*	Return: NONE
**********************************************************************************************************/
void soft_timer_start(struct soft_timer *this_timer)
{
    if (this_timer->timeout == 0)
      return;
    
	this_timer->status = SOFT_TIMER_STATUS_RUNNING;
    this_timer->count  = this_timer->timeout;
}

/**********************************************************************************************************
*	Function: soft_timer_stop
*	Description: stop a soft timer
*	Parameters:  stop timer
*	Return: NONE
**********************************************************************************************************/
void soft_timer_stop(struct soft_timer *this_timer)
{
	this_timer->status = SOFT_TIMER_STATUS_STOP;
}

/**********************************************************************************************************
*	Function: soft_timer_reload
*	Description: reload a soft timer
*	Parameters:  reload timer
*	Return: NONE
**********************************************************************************************************/
void soft_timer_reload(struct soft_timer *this_timer)
{
    if (this_timer->timeout == 0)
      return;
    
	this_timer->status = SOFT_TIMER_STATUS_RUNNING;
    this_timer->count  = this_timer->timeout;
}

/**********************************************************************************************************
*	Function: soft_timer_config
*	Description: config a soft timer
*	Parameters:  timer
*	Return: NONE
**********************************************************************************************************/
void soft_timer_config(struct soft_timer *this_timer, \
                              unsigned long timeout, \
                              unsigned char mode, \
                              unsigned char unit, \
                              void *param,\
                              void (*timeout_cb)(void *param))

{
    this_timer->mode  = mode;
    this_timer->unit  = unit;
    this_timer->param = param;
    this_timer->timeout_cb = timeout_cb;
	soft_timer_set_timeout(this_timer, timeout);
}

/**********************************************************************************************************
*	Function: soft_timer_set_timeout
*	Description: set a soft timer timeout
*	Parameters:  this_timer:timer  timeout:MS S MIN
*	Return: NONE
**********************************************************************************************************/
void soft_timer_set_timeout(struct soft_timer *this_timer, unsigned long timeout)
{
	/* Since the increments are ms, different units of timing need to be converted to ms */
	if (this_timer->unit == SOFT_TIMER_UNIT_MS)
		this_timer->timeout = timeout;
	else if (this_timer->unit == SOFT_TIMER_UNIT_S)
		this_timer->timeout = timeout*1000;
	else if (this_timer->unit == SOFT_TIMER_UNIT_MIN)
		this_timer->timeout = timeout*60*1000;
}

/**********************************************************************************************************
*	Function: get_soft_timer_status
*	Description: get a soft timer status
*	Parameters:  timer
*	Return: timer status 
**********************************************************************************************************/
unsigned char get_soft_timer_status(struct soft_timer *this_timer)
{
	return this_timer->status;
}


/**********************************************************************************************************
*	Function: soft_timer_exe
*	Description: execute in a clock reference
*	Parameters:  NONE
*	Return: NONE 
**********************************************************************************************************/
void soft_timer_exe()
{
    struct soft_timer *p_timer = soft_timer_list_head;
	
	/* effective timer */
	while (p_timer != NULL)
	{
		if (p_timer->status == SOFT_TIMER_STATUS_RUNNING)
		{
			/* Counting time decrement */
			if (p_timer->count < SOFT_TIMER_TICK_MS)
				p_timer->count = 0;
			else
				p_timer->count -= SOFT_TIMER_TICK_MS;
			
			/* time out */
			if (p_timer->count == 0)
			{
				if (p_timer->mode == SOFT_TIMER_MODE_SIGLE)
				{
					p_timer->status = SOFT_TIMER_STATUS_COMPLETED;
					if (p_timer->timeout_cb != NULL)
						p_timer->timeout_cb(p_timer->param);
				}
				else if (p_timer->mode == SOFT_TIMER_MODE_RERIOD)
				{
					p_timer->status = SOFT_TIMER_STATUS_RUNNING;
					p_timer->count = p_timer->timeout;
					if (p_timer->timeout_cb != NULL)
						p_timer->timeout_cb(p_timer->param);
				}
			}
		}
		
		p_timer = p_timer->next;
	}
}


