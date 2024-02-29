/*
 * Copyright (c) 20019-2020, wanweiyingchuang
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-09-03     Denghengli   the first version
 */
#ifndef _DRIVER_SOFTIMER_
#define _DRIVER_SOFTIMER_
#include  <stdlib.h>
#include "FreeRTOS.h"

/* It needs to be modified according to the user's own environment before use ！！！！！！！*/
#define SOFT_TIMER_TICK_MS      1000
#define SOFT_TIMER_MALLOC(size) pvPortMalloc(size)
#define SOFT_TIMER_FREE(p)      vPortFree(p)

/* soft timer unit */
#define SOFT_TIMER_UNIT_MIN           10
#define SOFT_TIMER_UNIT_S             20
#define SOFT_TIMER_UNIT_MS            30

/* soft timer mode */
#define SOFT_TIMER_MODE_UNUSED         0
#define SOFT_TIMER_MODE_SIGLE         100
#define SOFT_TIMER_MODE_RERIOD        200

/* soft timer status */
#define SOFT_TIMER_STATUS_UNUSED      0
#define SOFT_TIMER_STATUS_STOP        11
#define SOFT_TIMER_STATUS_RUNNING     22
#define SOFT_TIMER_STATUS_COMPLETED   33

struct soft_timer
{
	struct soft_timer *next; /* timer list */
	unsigned char mode;      /* run mode */
	unsigned char status;    /* timer status*/
	unsigned char unit;      /* unit：s or ms*/
	unsigned long timeout;   /* out time*/
	unsigned long count;     /* out time*/
    void *param;             /* timeout_callback param*/
	void (*timeout_cb)(void *param);/* timeout_callback*/
};

extern struct soft_timer *creat_soft_timer(void);
extern void soft_timer_del   (struct soft_timer **this_timer);
extern void soft_timer_start (struct soft_timer *this_timer);
extern void soft_timer_stop  (struct soft_timer *this_timer);
extern void soft_timer_reload(struct soft_timer *this_timer);

extern void soft_timer_config(struct soft_timer *this_timer, \
                              unsigned long timeout, \
                              unsigned char mode, \
                              unsigned char unit, \
                              void *param,\
                              void (*timeout_cb)(void *param));

extern void soft_timer_set_timeout(struct soft_timer *this_timer, unsigned long timeout);
extern unsigned char get_soft_timer_status(struct soft_timer *this_timer);
extern void soft_timer_exe();

#endif