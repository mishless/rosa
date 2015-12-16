/*
 * rosa_queue_manager.h
 *
 * Created: 12/16/2015 6:58:38 PM
 *  Author: Kolja
 */ 


#ifndef ROSA_QUEUE_MANAGER_H_
#define ROSA_QUEUE_MANAGER_H_
#include "priority_queue.h"


void putInREADYqueue( Task* task );
Task* getFromREADYqueue( void );
Task* peekREADYqueue( void );
unsigned int isREADYqueueEmpty( void );

#if DEBUG_COMPILATION

PriorityQueue fetchREADYqueue(void);

#endif

#endif /* ROSA_QUEUE_MANAGER_H_ */