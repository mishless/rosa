/*
 * rosa_queue_manager.c
 *
 * Created: 12/16/2015 6:59:15 PM
 *  Author: Kolja
 */ 
#include "priority_queue.h"
#include "rosa_config.h"

PriorityQueue* READYqueue;
PriorityQueue* DELAYqueue;

int READYcomparator(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement)
{
	return ((*firstElement).task->originalPriority) > ((*secondElement).task->originalPriority);
}

void READYcallback(void)
{
	//TODO when scheduler is finished and currently running task
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

int DELAYcomparator(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement)
{
	return ((*firstElement).task->wakeUpTime) < ((*secondElement).task->wakeUpTime);
}

void DELAYcallback(void)
{
	//TODO when scheduler is finished and currently running task
}

void putInDELAYqueue( Task* task )
{
	PriorityQueueElement newElement;
	newElement.task = task;
	enqueue(DELAYqueue, &newElement);
}

Task* getFromDELAYqueue( void )
{
	PriorityQueueElement * element = dequeue(DELAYqueue);
	return element->task;
}

Task* peekDELAYqueue( void )
{
	PriorityQueueElement * element = peek(DELAYqueue);
	return element->task;
}

unsigned int isDELAYqueueEmpty( void )
{
	return isEmpty(DELAYqueue);
}


void initializeQueues(void)
{
	READYqueue = createPriorityQueue( MAX_NUMBER_TASKS, &READYcomparator, &READYcallback );
	DELAYqueue = createPriorityQueue( MAX_NUMBER_TASKS, &DELAYcomparator, &DELAYcallback );
}

#if DEBUG_COMPILATION

PriorityQueue fetchREADYqueue(void)
{
	return *READYqueue;
}

PriorityQueue fetchDELAYqueue(void)
{
	return *DELAYqueue;
}

#endif