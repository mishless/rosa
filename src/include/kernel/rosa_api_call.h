/*
 * rosa_api_call.h
 *
 */ 

#ifndef ROSA_API_CALL_H_
#define ROSA_API_CALL_H_

#include "rosa_config.h"

typedef void * TaskHandle;

typedef void * SemaphoreHandle;

typedef volatile unsigned long long ROSA_TickCount;

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

#define TOO_MANY_SEMAPHORES		7
#define INVALID_INITIAL_VALUE	8
#define SEMAPHORE_NOT_FOUND		9
#define TIMEOUT					10

#define FAILURE                 6

//Semaphore 
	//types
#define BINARY					0
#define MUTEX					1
	//state
#define SEMAPHORE_FREE			0
#define SEMAPHORE_OCCUPIED		1
 
#endif 