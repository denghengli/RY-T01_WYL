/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-03     Denghengli   the first version
 */

#include "drv_task.h"

static struct task *task_list_head = NULL;/*the head of the task list*/

/**********************************************************************************************************
*	Function: creat_task
*	Description: creat a task
*	Parameters: NONE
*	Return: task
**********************************************************************************************************/
struct task *creat_task(unsigned char task_id, \
                        unsigned long prio_ms, \
                        void *param, \
                        void (*process)(void *param))
{
	struct task *new_task = NULL;
	struct task *p_task = NULL;
	struct soft_timer *new_timer = NULL;
    
	/* create a new task */
	new_task = TASK_MALLOC(sizeof(struct task));
	
	/* does the new task created success */
	if (new_task == NULL)
	{
		return NULL;
	}
	else
	{
		new_timer = creat_soft_timer();
        if (new_timer == NULL)
		{
			TASK_FREE(new_task);
			return NULL;
		}
		else
		{
			new_task->id        = task_id;
            new_task->prio_ms   = prio_ms;
			new_task->status    = TASK_STATUS_INIT;
			new_task->timer     = new_timer;
            new_task->param     = param;
			new_task->process   = process;
		}
	}
	
	/* does the task list already exit */
	if (task_list_head == NULL)
	{
        new_task->next = NULL;
		task_list_head = new_task;
	}
	else
	{
		/* add the new task to the end of the task list */
		p_task = task_list_head;
		while(p_task->next != NULL)
		{
			p_task = p_task->next;
		}
		p_task->next = new_task;
		new_task->next = NULL;
	}
	
	return new_task;
}

/**********************************************************************************************************
*	Function: task_del
*	Description: delete a task
*	Parameters: delete timer
*	Return: NONE
**********************************************************************************************************/
void task_del(struct task **this_task)
{
	struct task *p_task = task_list_head;
	struct task *t_task = NULL;
	
	/* does this task is vaild */
	if (this_task == NULL)
		return;
	
	/* find this timer */
	while (p_task != NULL)
	{
		if (p_task == *this_task)
		{
			/* head node is the find soft timmer */
			if (p_task == task_list_head)
			{
				soft_timer_del(&p_task->timer);
				task_list_head = p_task->next;
				TASK_FREE(p_task);
			}
			else
			{
				soft_timer_del(&p_task->timer);
				t_task->next = p_task->next;
				TASK_FREE(p_task);
			}
		}
		t_task = p_task;
		p_task = p_task->next;
	}
	
	/* delete external pointer */
	*this_task = NULL;
}

/**********************************************************************************************************
*	Function: task_close
*	Description: switch task state to close state
*	Parameters:  task
*	Return: NONE
**********************************************************************************************************/
void task_close (struct task *this_task)
{
    this_task->status = TASK_STATUS_CLOSE;
	soft_timer_stop(this_task->timer);
}

/**********************************************************************************************************
*	Function: task_status_ready
*	Description: switch task state to ready state
*	Parameters:  task status
*	Return: NONE
**********************************************************************************************************/
void task_status_ready(void *param)
{
    unsigned char *task_status = (unsigned char*)param;
    
    if (*task_status == TASK_STATUS_RUNNING)
        *task_status = TASK_STATUS_READY;
}

/**********************************************************************************************************
*	Function: task_start
*	Description: start a task
*	Parameters:  start task
*	Return: NONE
**********************************************************************************************************/
void task_start(struct task *this_task)
{
    if (this_task == NULL)
      return;
    
    this_task->status = TASK_STATUS_RUNNING;
    
    /* config the soft timer*/
	soft_timer_config(this_task->timer, \
                      this_task->prio_ms, \
                      SOFT_TIMER_MODE_RERIOD, \
                      SOFT_TIMER_UNIT_MS, \
                      (void*)this_task->status,\
                      task_status_ready);
    
    /* start soft timer */
	soft_timer_start(this_task->timer);
	
}

/**********************************************************************************************************
*	Function: task_schedule
*	Description: Task processing and scheduling in a loop(while(1))
*	Parameters:  NONE
*	Return: NONE 
**********************************************************************************************************/
void task_schedule(void)
{
	struct task *p_task = task_list_head;

	/* effective task */
	while (p_task != NULL)
	{
		if (p_task->status == TASK_STATUS_READY)
		{
            p_task->process(p_task->param);
            p_task->status = TASK_STATUS_RUNNING;
		}
		
		p_task = p_task->next;
	}
}


