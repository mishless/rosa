/*
*
* ROSA Task Manager Component Header
*
*/

#ifndef ROSA_TASK_MANAGER_H_
#define ROSA_TASK_MANAGER_H_

#include "rosa_api_call.h"

unsigned int ROSA_CreateTask (void (*functionBody) (void), char *functionNameChArr, unsigned int maxStackSize, unsigned int taskPriority, TaskHandle *taskHandle);
unsigned int ROSA_CreateCyclicTask (void (*functionBody) (void), char * functionNameChArr, unsigned int maxStackSize, unsigned int taskPriority, ROSA_TickCount taskPeriod, ROSA_TickCount taskDeadline, TaskHandle *taskHandle);		
unsigned int ROSA_TerminateTask (void);

#endif