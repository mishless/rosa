/*
 * rosa_clock_manager.c
 *
 * Created: 17.12.2015. 8:10:25 PM
 *  Author: Vukan
 */ 

#include "rosa_clock_manager.h"
#include "rosa_task_manager.h"
#include "rosa_queue_manager.h"
#include "rosa_scheduler_private.h"

ROSA_TickCount systemTime = 0;

void ROSA_DelayRelative (ROSA_TickCount tickCount)
{
	ROSA_TickCount time;
	Task* task;
	
	time = systemTime;
	task = getCRT();
	task->wakeUpTime = time + tickCount;
	
	/*This should trigger ROSA_yield*/
	putInDELAYqueue(task);
}

void ROSA_DelayAbsolute (ROSA_TickCount reference,
ROSA_TickCount tickCount)
{
	Task* task;
	task = getCRT();
	task->wakeUpTime = reference + tickCount;
	
	/*This should trigger ROSA_yield*/
	putInDELAYqueue(task);
}

ROSA_TickCount ROSA_TimerTickCount (void)
{
	return systemTime;
}

void ROSA_EndCycle (void)
{
	ROSA_TickCount time;
	Task* task;
	
	task = getCRT();
	time = systemTime;
	
	/*EC formula*/
	task->wakeUpTime = task->period * (time/task->period + 1);
	
	/*This should trigger ROSA_yield*/
	putInDELAYqueue(task);
}