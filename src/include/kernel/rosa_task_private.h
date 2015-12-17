/*
* rosa_task_private.h
*
*/

#ifndef ROSA_TASK_PRIVATE_H_
#define ROSA_TASK_PRIVATE_H_

#include "rosa_api_call.h"
#include "kernel/rosa_def.h"
#include "stack.h"

typedef struct {
	tcb* t;
	unsigned int originalPriority;
	Stack temporaryPriority;
	ROSA_TickCount period;
	ROSA_TickCount deadline;
	ROSA_TickCount wakeUpTime;
} Task;

void setTaskDelay(Task *task, ROSA_TickCount wakeUpTime);
unsigned int getPriority(Task *task);

#endif /* ROSA_TASK_PRIVATE_H_ */