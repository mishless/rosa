/*
 * rosa_scheduler_manager.c
 *
 * Created: 12/16/2015 9:21:01 PM
 *  Author: Kolja
 */ 
#include "kernel/rosa_scheduler_manager.h"
#include "kernel/rosa_task_private.h"
#include "rosa_ker.h"
#include "rosa_queue_manager.h"
#include "rosa_tim.h"
#include "rosa_int.h"

extern ROSA_TickCount round_robin_counter;

unsigned int isStarted = 0;

Task* currentlyRunningTask = NULL ;

void ROSA_StartScheduler(void)
{
	currentlyRunningTask = getFromREADYqueue();
	tcb* t = currentlyRunningTask->t;
	TCBLIST = t;
	timerStart();
	/*ROSA_start will take TCBLIST and put it in EXECTASK*/
	ROSA_start();
}

void scheduler()
{
	interruptDisable();
	
	round_robin_counter = 0;
	currentlyRunningTask = getFromREADYqueue();
	EXECTASK = currentlyRunningTask->t;
	
	interruptEnable();
}

unsigned int isSchedulerStarted(void)
{
	return isStarted;
}

/*Task* getCRT()
{
	return currentlyRunningTask;
}*/