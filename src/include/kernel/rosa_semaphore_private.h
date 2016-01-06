/*
 * rosa_semaphore_private.h
 *
 */ 

#ifndef ROSA_SEMAPHORE_PRIVATE_H_
#define ROSA_SEMAPHORE_PRIVATE_H_

#include "rosa_api_call.h"
#include "blocked_priority_queue.h"

typedef struct
{
	unsigned int id;
	unsigned int type;
	unsigned int priority;
	unsigned int state;
	BlockedPriorityQueue* SemaphoreBlockedQueue;	
}Semaphore;


#endif /* ROSA_SEMAPHORE_PRIVATE_H_ */