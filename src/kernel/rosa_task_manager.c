/*
 * rosa_task_manager.c
 *
 */ 

#include "kernel/rosa_task_manager.h"
#include "kernel/rosa_task_private.h"
#include "kernel/rosa_queue_manager.h"
#include "rosa_config.h"
#include "kernel/rosa_ker.h"
#include "rosa_scheduler_private.h"
#include "stack.h"

#include <stdlib.h>

unsigned int task_counter = 0;

unsigned int ROSA_CreateTask (void (*functionBody) (void),
								char * functionNameChArr,
								unsigned int maxStackSize,
								unsigned int taskPriority,
								TaskHandle* taskHandle)
{
	Task* task;
	int* stack;
	
	task = malloc(sizeof(Task));
	
	if(task == NULL)
		return NOT_ENOUGH_MEMORY;
	
	task->t = malloc(sizeof(tcb));
	
	if(task->t == NULL)
		return NOT_ENOUGH_MEMORY;
		
	stack = malloc(maxStackSize);
	
    if(stack == NULL)
        return NOT_ENOUGH_MEMORY;
    
	/*Allocate memory for stack for temporary priorities*/
	task->temporaryPriority = createStack(MAX_NUMBER_SEMAPHORES);
	if(task->temporaryPriority == NULL)
		return NOT_ENOUGH_MEMORY;
		

		
	int name_size = sizeof(functionNameChArr)/sizeof(char);
	if( name_size > 4 || name_size < 1 ) 
		return INVALID_NAME;
	if( task_counter+1 == MAX_NUMBER_TASKS )
		return TOO_MANY_TASKS;
	if( taskPriority == 0)
		return INVALID_PRIORITY;
	if( maxStackSize < MINIMAL_STACK_SIZE)
		return INVALID_STACK_SIZE;
	ROSA_tcbCreate(task->t, functionNameChArr, functionBody, stack, maxStackSize);
	task->originalPriority = taskPriority;
	putInREADYqueue(task);
	*taskHandle = (TaskHandle*)task;
	task_counter++;
	return SUCCESS;
}

unsigned int ROSA_CreateCyclicTask (void (*functionBody) (void),
								char * functionNameChArr,
								unsigned int maxStackSize,
								unsigned int taskPriority,
								ROSA_TickCount taskPeriod,
								ROSA_TickCount taskDeadline,
								TaskHandle* taskHandle)
{
	Task* task;
	int* stack;
	
	task = malloc(sizeof(Task));
	
	if(task == NULL)
		return NOT_ENOUGH_MEMORY;
	
	task->t = malloc(sizeof(tcb));
	
	if(task->t == NULL)
		return NOT_ENOUGH_MEMORY;
	
	stack = malloc(maxStackSize);
	if(stack == NULL)
		return NOT_ENOUGH_MEMORY;
	
	/*Allocate memory for stack for temporary priorities*/
	task->temporaryPriority = createStack(MAX_NUMBER_SEMAPHORES);
	if(task->temporaryPriority == NULL)
		return NOT_ENOUGH_MEMORY;
		
	if( sizeof(functionNameChArr)/sizeof(char) > 4 || sizeof(functionNameChArr)/sizeof(char) < 1 )
		return INVALID_NAME;
	if( task_counter+1 == MAX_NUMBER_TASKS )
		return TOO_MANY_TASKS;
	if( taskPriority == 0)
		return INVALID_PRIORITY;
	if( maxStackSize < MINIMAL_STACK_SIZE)
		return INVALID_STACK_SIZE;
	ROSA_tcbCreate(task->t, functionNameChArr, functionBody, stack, maxStackSize);
	task->originalPriority = taskPriority;
	task->deadline = taskDeadline;
	task->period = taskPeriod;
	putInREADYqueue(task);
	*taskHandle = (TaskHandle*)task;
	task_counter++;
	return SUCCESS;							
}

unsigned int ROSA_TerminateTask (void)
{
	Task* task;
	task = getCRT();

	/*Deallocate memory*/
	destroyStack(task->temporaryPriority);
	free(task->t->dataarea);
	free(task->t);
	free((void*) task);
	
	ROSA_yield();
	
	if(task != NULL)
		return FAILURE;
		
	return FAILURE;
}

void setTaskDelay(Task* task,
					ROSA_TickCount wakeUpTime)
{
	(*task).wakeUpTime = wakeUpTime;			
}

unsigned int getPriority(Task *task) {
	if (isEmptyStack(task->temporaryPriority)) {
		return task->originalPriority;
	}
	unsigned int priority = popFromStack(task->temporaryPriority);
	return priority;
}