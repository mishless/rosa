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
#include "rosa_ker.h"
#include "rosa_int.h"

ROSA_TickCount systemTime = 0;

void ROSA_DelayRelative (ROSA_TickCount tickCount)
{
	ROSA_TickCount time;
	Task* task;
	
	interruptDisable();
	
	time = systemTime;
	task = getCRT();
	task->wakeUpTime = time + tickCount;
	
	putInDELAYqueue(task);
	
	interruptEnable();
	
	ROSA_yield();
}

void ROSA_DelayAbsolute (ROSA_TickCount reference, ROSA_TickCount tickCount)
{
	Task* task;
	
	interruptDisable();
	
	task = getCRT();
	task->wakeUpTime = reference + tickCount;
	
	putInDELAYqueue(task);
	
	interruptEnable();
	
	ROSA_yield();
}

ROSA_TickCount ROSA_TimerTickCount (void)
{
	return systemTime;
}

void ROSA_EndCycle (void)
{
	ROSA_TickCount time;
	Task* task;
	
	interruptDisable();
	
	task = getCRT();
	time = systemTime;
	
	/*EC formula*/
	task->wakeUpTime = task->period * (time/task->period + 1);
	
	/*This should trigger ROSA_yield*/
	putInDELAYqueue(task);
	
	interruptEnable();
	
	ROSA_yield();
}