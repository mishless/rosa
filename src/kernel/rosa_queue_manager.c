/*
* rosa_queue_manager.c
*
* Created: 12/16/2015 6:59:15 PM
*  Author: Kolja
*/
#include "priority_queue.h"
#include "rosa_config.h"
#include "rosa_task_private.h"
#include "rosa_scheduler_private.h"
#include "rosa_queue_manager.h"
#include "rosa_ker.h"

PriorityQueue* READYqueue;
PriorityQueue* DELAYqueue;

unsigned int interrupt_flag = 0;

int READYcomparator(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement)
{
	if (getPriority(firstElement->task) == getPriority(secondElement->task)) {
		return 0;
	} else if (getPriority(firstElement->task) > getPriority(secondElement->task)) {
		return 1;
	}
	return -1;
}

void putInREADYqueue( Task* task )
{
	PriorityQueueElement *newElement = malloc(sizeof(PriorityQueueElement));
	newElement->task = task;
	enqueue(READYqueue, newElement);
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
	if (firstElement->task->wakeUpTime == secondElement->task->wakeUpTime) {
		return 0;
	} else if (firstElement->task->wakeUpTime < secondElement->task->wakeUpTime) {
		return 1;
	}
	return -1;
}

void putInDELAYqueue( Task* task )
{
	PriorityQueueElement *newElement = malloc(sizeof(PriorityQueueElement));
	newElement->task = task;
	enqueue(DELAYqueue, newElement);
}

Task* getFromDELAYqueue( void )
{
	PriorityQueueElement *element = dequeue(DELAYqueue);
	return element->task;
}

Task* peekDELAYqueue( void )
{
	PriorityQueueElement *element = peek(DELAYqueue);
	return element->task;
}

unsigned int isDELAYqueueEmpty( void )
{
	return isEmpty(DELAYqueue);
}


void initializeQueues(void)
{
	READYqueue = createPriorityQueue( MAX_NUMBER_TASKS, &READYcomparator );
	DELAYqueue = createPriorityQueue( MAX_NUMBER_TASKS, &DELAYcomparator );
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