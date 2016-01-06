/*
*
* ROSA Task Manager Component
* Provides functionality for creating tasks and cyclic tasks with static priority,
* terminating a running task from within itself as well as set delay for a task and
* get current priority of a task.
*
*/

#include "kernel/rosa_task_manager.h"
#include "kernel/rosa_task_private.h"
#include "kernel/rosa_queue_manager.h"
#include "rosa_config.h"
#include "kernel/rosa_ker.h"
#include "rosa_scheduler_private.h"
#include "stack.h"
#include "helper_functions.h"
#include "rosa_int.h"
#include <stdlib.h>

unsigned int taskCounter = 0;

/*
* ROSA_CreateTask function creates a new task and adds it to the ready queue.
* If necessary it also invoked ROSA_yield.
*/
unsigned int ROSA_CreateTask(void (*functionBody) (void), char *functionNameChArr, unsigned int maxStackSize, unsigned int taskPriority, TaskHandle *taskHandle)
{
	//Disable interrupts
	interruptDisable();

	Task* task = malloc(sizeof(Task));
	// Check if memory was successfully allocated for task structure
	if (task == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	task->t = malloc(sizeof(tcb));
	// Check if memory was successfully allocated for task structure
	if (task->t == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	int *stack = malloc(maxStackSize);
	// Check if memory was successfully allocated for the maximum size of the stack
	if (stack == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	// Allocate memory for stack structure that holds temporary priorities
	task->temporaryPriority = createStack(MAX_NUMBER_SEMAPHORES);
	// Check if memory was successfully allocated for stack structure
	if (task->temporaryPriority == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	// Check if the length of the function name is valid
	unsigned int name_size = strlen(functionNameChArr);
	if (name_size > 4 || name_size < 1)
	{
		return INVALID_NAME;
	}

	// Check if the maximum number of tasks is already created
	if (++taskCounter > MAX_NUMBER_TASKS)
	{
		return TOO_MANY_TASKS;
	}

	// Check if the specified priority for the task is valid (different than 0 - the IDLE task priority)
	if (taskPriority == 0)
	{
		return INVALID_PRIORITY;
	}

	// Create TCB structure for task
	ROSA_tcbCreate(task->t, functionNameChArr, functionBody, stack, maxStackSize);
	// Assign original priority to be equal to task priority
	task->originalPriority = taskPriority;
	// Put the newly created task in the READY queue
	putInREADYqueue(task);
	// Populate the task handle with a pointer to the newly created task
	*taskHandle = (TaskHandle*)task;

	// Enable interrupts
	interruptEnable();

	// If the scheduler has been started check if CRT priority is lower than the new task priority and if so yeild
	if (isSchedulerStarted() == 1)
	{
		if (getPriority(getCRT()) < taskPriority)
		{
			putInREADYqueue(getCRT());
			ROSA_yield();
		}
	}

	// Return SUCCESS
	return SUCCESS;
}

/*
* ROSA_CreateCyclicTask function creates a new cyclic task and adds it to the ready queue.
* If necessary it also invoked ROSA_yield.
*/
unsigned int ROSA_CreateCyclicTask(void (*functionBody) (void), char *functionNameChArr, unsigned int maxStackSize, unsigned int taskPriority, ROSA_TickCount taskPeriod, ROSA_TickCount taskDeadline, TaskHandle *taskHandle)
{
	//Disable interrupts
	interruptDisable();

	Task* task = malloc(sizeof(Task));
	// Check if memory was successfully allocated for task structure
	if (task == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	task->t = malloc(sizeof(tcb));
	// Check if memory was successfully allocated for task structure
	if (task->t == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	int *stack = malloc(maxStackSize);
	// Check if memory was successfully allocated for the maximum size of the stack
	if (stack == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	// Allocate memory for stack structure that holds temporary priorities
	task->temporaryPriority = createStack(MAX_NUMBER_SEMAPHORES);
	// Check if memory was successfully allocated for stack structure
	if (task->temporaryPriority == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	// Check if the length of the function name is valid
	unsigned int name_size = strlen(functionNameChArr);
	if (name_size > 4 || name_size < 1)
	{
		return INVALID_NAME;
	}

	// Check if the maximum number of tasks is already created
	if (++taskCounter > MAX_NUMBER_TASKS)
	{
		return TOO_MANY_TASKS;
	}

	// Check if the specified priority for the task is valid (different than 0 - the IDLE task priority)
	if (taskPriority == 0)
	{
		return INVALID_PRIORITY;
	}

	// Create TCB structure for task
	ROSA_tcbCreate(task->t, functionNameChArr, functionBody, stack, maxStackSize);
	// Assign original priority to be equal to task priority
	task->originalPriority = taskPriority;
	// Assign deadline to be equal to specified deadlines
	task->deadline = taskDeadline;
	// Assign period to be equal to the specified period
	task->period = taskPeriod;
	// Put the newly created task in the READY queue
	putInREADYqueue(task);
	// Populate the task handle with a pointer to the newly created task
	*taskHandle = (TaskHandle*)task;

	// Enable interrupts
	interruptEnable();

	// If the scheduler has been started check if CRT priority is lower than the new task priority and if so yeild
	if (isSchedulerStarted() == 1)
	{
		if (getPriority(getCRT()) < taskPriority)
		{
			putInREADYqueue(getCRT());
			ROSA_yield();
		}
	}

	// Return SUCCESS
	return SUCCESS;
}

/*
* ROSA_TerminateTask function terminates a currently running task from within itself
* and invokes ROSA_yield.
*/
unsigned int ROSA_TerminateTask(void)
{
	// Disable interrupts
	interruptDisable();

	// Get the CRT
	Task *task = getCRT();

	//Deallocate memory
	destroyStack(task->temporaryPriority);
	// Calculate the pointer which was acquired with malloc
	free(task->t->dataarea - task->t->datasize);
	free(task->t);
	free((void*) task);

	// Decrement the task counter
	taskCounter--;
	// Enable interrupts
	interruptEnable();
	
	ROSA_yield();

	// Check if the memory was deallocated successfully
	if(task != NULL)
	{
		return FAILURE;
	}

	return SUCCESS;
}

/*
* setTaskDelay function sets a new wake up time for the task.
*/
void setTaskDelay(Task* task, ROSA_TickCount wakeUpTime)
{
	(*task).wakeUpTime = wakeUpTime;
}

/*
* getPriority function gets the current priority of the task.
*/
unsigned int getPriority(Task *task)
{
	if (isEmptyStack(task->temporaryPriority))
	{
		return task->originalPriority;
	}
	unsigned int priority = popFromStack(task->temporaryPriority);
	return priority;
}

ROSA_TickCount getWakeUpTime(Task *task)
{
	return task->wakeUpTime;
}