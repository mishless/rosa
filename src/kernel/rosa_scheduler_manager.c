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

Task *getCRT(void)
{
	return currentlyRunningTask;
}

void scheduler()
{
	currentlyRunningTask = getFromREADYqueue();	
	EXECTASK = currentlyRunningTask->t;
}

unsigned int isSchedulerStarted(void)
{
	return isStarted;
}
