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
	
	ROSA_yield();
	
	interruptEnable();
}

void ROSA_DelayAbsolute (ROSA_TickCount reference, ROSA_TickCount tickCount)
{
	interruptDisable();
	getCRT()->wakeUpTime = reference + tickCount;
	
	putInDELAYqueue(getCRT());
	
	ROSA_yield();
	
	interruptEnable();
}

ROSA_TickCount ROSA_TimerTickCount (void)
{
	return systemTime;
}

void ROSA_EndCycle (void)
{
	interruptDisable();
	
	/*EC formula*/
	getCRT()->wakeUpTime = getCRT()->period * (systemTime/getCRT()->period + 1);
	
	/*This should trigger ROSA_yield*/
	putInDELAYqueue(getCRT());
	
	ROSA_yield();
	
	interruptEnable();
}