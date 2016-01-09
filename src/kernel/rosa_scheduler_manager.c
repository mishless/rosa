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

void idle_task(void)
{
	while (1);
}

void make_idle_task(void)
{
	int* stack;
	int stackSize = 64;
	Task* task;
	
	interruptDisable();
	
	task = malloc(sizeof(Task));
	if(task == NULL)
		return;
	
	task->t = malloc(sizeof(tcb));
	if(task->t == NULL)
		return;
	
	stack = malloc(stackSize);
	if(stack == NULL)
		return;
		
	// This is primarily for getPriority, so it doesn't return garbage, even though this task won't use IPCP.
	task->temporaryPriority = createStack(0);
	
	ROSA_tcbCreate(task->t, "idle", &idle_task, stack, stackSize);
	task->originalPriority = 0;
	
	putInREADYqueue(task);
	
	interruptEnable();
}

void ROSA_StartScheduler(void)
{
	make_idle_task();
	
	currentlyRunningTask = getFromREADYqueue();
	tcb* t = currentlyRunningTask->t;
	TCBLIST = t;
	
	timerStart();
	
	isStarted = 1;
	
	/*ROSA_start will take TCBLIST and put it in EXECTASK*/
	ROSA_start();
}

void scheduler()
{
	//interruptDisable();
	
	round_robin_counter = 0;
	currentlyRunningTask = getFromREADYqueue();
	EXECTASK = currentlyRunningTask->t;
	
	//interruptEnable();
}

unsigned int isSchedulerStarted(void)
{
	return isStarted;
}

/*Task* getCRT()
{
	return currentlyRunningTask;
}*/