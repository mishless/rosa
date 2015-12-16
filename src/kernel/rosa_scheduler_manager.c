/*
 * rosa_scheduler_manager.c
 *
 * Created: 12/16/2015 9:21:01 PM
 *  Author: Kolja
 */ 
#include "kernel/rosa_scheduler_manager.h"
#include "kernel/rosa_task_private.h"

Task* currentlyRunningTask = NULL ;

Task *getCRT(void)
{
	return currentlyRunningTask;
}