/*
 * rosa_queue_manager.c
 *
 * Created: 12/16/2015 6:59:15 PM
 *  Author: Kolja
 */ 
#include "priority_queue.h"
#include "rosa_config.h"

PriorityQueue* READYqueue;

int READYcomparator(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement)
{
	return ((*firstElement).task->originalPriority) > ((*secondElement).task->originalPriority);
}

void READYcallback(void)
{
	//TODO when scheduler is finished and currently running task
}

void initializeQueues(void)
{
	READYqueue = createPriorityQueue( MAX_NUMBER_TASKS, &READYcomparator, &READYcallback );
}

void putInREADYqueue( Task* task )
{
	PriorityQueueElement newElement;
	newElement.task = task;
	enqueue(READYqueue, &newElement);
}

Task* getFromREADYqueue( void )
{
	PriorityQueueElement * element = dequeue(READYqueue);
	return element->task;
}

Task* peekREADYqueue( void )
{
	PriorityQueueElement * element = peek(READYqueue);
	return element->task;
}

unsigned int isREADYqueueEmpty( void )
{
	return isEmpty(READYqueue);
}


#if DEBUG_COMPILATION

PriorityQueue fetchREADYqueue(void)
{
	return *READYqueue;
}

#endif