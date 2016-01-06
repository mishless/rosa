/*
 * rosa_queue_manager.h
 *
 * Created: 12/16/2015 6:58:38 PM
 *  Author: Kolja
 */ 


#ifndef ROSA_QUEUE_MANAGER_H_
#define ROSA_QUEUE_MANAGER_H_

#include "priority_queue.h"
#include "rosa_ker.h"
#include "blocked_priority_queue.h"
#include "priority_queue.h"
#include "rosa_config.h"
#include "rosa_task_private.h"
#include "rosa_scheduler_private.h"

void putInREADYqueue( Task* task );
Task* getFromREADYqueue( void );
Task* peekREADYqueue( void );
unsigned int isREADYqueueEmpty( void );

void putInDELAYqueue( Task* task );
Task* getFromDELAYqueue( void );
Task* peekDELAYqueue( void );
unsigned int isDELAYqueueEmpty( void );

void putInBLOCKEDqueue( Task* task );
Task* getFromBLOCKEDqueue( void );
Task* peekBLOCKEDqueue( void );
unsigned int isBLOCKEDqueueEmpty( void );

void initializeQueues( void );

#if DEBUG_COMPILATION

PriorityQueue fetchREADYqueue(void);
PriorityQueue fetchDELAYqueue(void);
BlockedPriorityQueue fetchBLOCKEDqueue(void);


#endif

#endif /* ROSA_QUEUE_MANAGER_H_ */