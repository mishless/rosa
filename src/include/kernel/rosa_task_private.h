/*
 * rosa_task_private.h
 *
 */ 

#ifndef ROSA_TASK_PRIVATE_H_
#define ROSA_TASK_PRIVATE_H_

#include "rosa_api_call.h"

struct{
	unsigned int data[MAX_NUMBER_SEMAPHORES];
	unsigned int size;
} Stack;

struct{
	TaskHandle* prev;
	TaskHandle* next;
	tcb* t;
	unsigned int originalPriority;
	Stack temporaryPriority;
	ROSA_TickCount period;
	ROSA_TickCount deadline;
	ROSA_TickCount wakeUpTime;
} Task;

void setTaskDelay(TaskHandle* task, 
					ROSA_TickCount wakeUpTime);


#endif /* ROSA_TASK_PRIVATE_H_ */