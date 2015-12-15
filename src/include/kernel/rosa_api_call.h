/*
 * rosa_api_call.h
 *
 */ 

#ifndef ROSA_API_CALL_H_
#define ROSA_API_CALL_H_

#include "rosa_config.h"

typedef volatile unsigned long long ROSA_TickCount;

typedef void * TaskHandle;

typedef void * SemaphoreHandle;

#include "rosa_scheduler_manager.h"
#include "rosa_task_manager.h"
#include "rosa_clock_manager.h"
#include "rosa_semaphore_manager.h"

//Return codes
#define SUCCESS					0
#define INVALID_PRIORITY		1
#define	INVALID_STACK_SIZE		2
#define	NOT_ENOUGH_MEMORY		5

#define	INVALID_NAME			3
#define TOO_MANY_TASKS			4

 
#endif 