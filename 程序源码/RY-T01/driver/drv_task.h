/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-03     Denghengli   the first version
 */
#ifndef _DRIVER_TASK_
#define _DRIVER_TASK_

#include "drv_soft_timer.h"

/* It needs to be modified according to the user's own environment before use £¡£¡£¡£¡£¡£¡£¡*/
#define TASK_MALLOC(size) malloc(size)
#define TASK_FREE(p)      free(p)

/* task status */
#define TASK_STATUS_INIT      11
#define TASK_STATUS_CLOSE     22
#define TASK_STATUS_RUNNING   33
#define TASK_STATUS_READY     44

struct task
{
	struct task *next;            /* task list */
	unsigned char id;             /* task ID */
    unsigned long prio_ms;        /* task proritity(ms)*/
	unsigned char status;         /* task status*/
    struct soft_timer *timer;     /* task timer*/
	void (*process)(void *param); /* task process*/
    void *param;                  /* task param*/
};

extern struct task *creat_task(unsigned char task_id, \
                               unsigned long prio_ms, \
                               void *param, \
                               void (*process)(void*));

extern void task_del   (struct task **this_task);
extern void task_close (struct task *this_task);
extern void task_start (struct task *this_task);
extern void task_schedule(void);

#endif