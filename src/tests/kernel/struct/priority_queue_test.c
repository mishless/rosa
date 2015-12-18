#include "priority_queue.h"

int intComparator(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement) {
	if (firstElement->task->originalPriority == secondElement->task->originalPriority) {
		return 0;
		} else if (firstElement->task->originalPriority > secondElement->task->originalPriority) {
		return 1;
	}
	return -1;
}

void callbackOnChange(void) {
	return;
	/* For now do nothing */
}

int mainTest()
{
	PriorityQueue *queue = createPriorityQueue(3, (&intComparator));
	Task *task1 = malloc(sizeof(Task));
	task1->originalPriority = 1;
	PriorityQueueElement *element1 = malloc(sizeof(PriorityQueueElement));
	element1->task = task1;

	Task *task2 = malloc(sizeof(Task));
	task2->originalPriority = 2;
	PriorityQueueElement *element2 = malloc(sizeof(PriorityQueueElement));
	element2->task = task2;

	Task *task3 = malloc(sizeof(Task));
	task3->originalPriority = 3;
	PriorityQueueElement *element3 = malloc(sizeof(PriorityQueueElement));
	element3->task = task3;

	enqueue(queue, element1);
	enqueue(queue, element2);
	enqueue(queue, element3);
	int i;
	PriorityQueueElement *element = NULL;
	for (i=0; i<queue->maximumSize; i++) {
		element = dequeue(queue);

	}
	deletePriorityQueue(queue);
	return 0;
}
