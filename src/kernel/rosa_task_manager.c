/*
 * rosa_task_manager.c
 *
 */ 

#include "rosa_task_manager.h"
#include "rosa_task_private.h"

unsigned int ROSA_CreateTask (void (*functionBody) (void),
								char * functionNameChArr,
								unsigned int maxStackSize,
								unsigned int taskPriority,
								TaskHandle* taskHandle)
{
	Task* task = (Task*) taskHandle;
	tcb t;
	ROSA_tcbCreate(&t, functionNameChArr, &functionBody, (int*) malloc(maxStackSize), maxStackSize);
	
	return SUCCESS;
}

unsigned int ROSA_CreateTask (void (*functionBody) (void),
								char * functionNameChArr,
								unsigned int maxStackSize,
								unsigned int taskPriority,
								ROSA_TickCount taskPeriod,
								ROSA_TickCount taskDeadline,
								TaskHandle* taskHandle)
{
									
}

unsigned int ROSA_TerminateTask (void)
{
	
}

void setTaskDelay(TaskHandle* task,
					ROSA_TickCount wakeUpTime)
{
						
}