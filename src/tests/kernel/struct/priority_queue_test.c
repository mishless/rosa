#include "priority_queue.h"

int intComparator(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement) {
	if (firstElement->task->originalPriority == secondElement->task->originalPriority) {
		return 0;
		} else if (firstElement->task->originalPriority > secondElement->task->originalPriority )  {
		return 1;
	}
	return -1;
}

void callbackOnChange() {
	/* For now do nothing */
}

int test1()
{
	PriorityQueue *queue = createPriorityQueue(10, (&intComparator), (&callbackOnChange));
	PriorityQueueElement *element1 = malloc(sizeof(PriorityQueueElement));
	element1->task->originalPriority = 1;
	PriorityQueueElement *element2 = malloc(sizeof(PriorityQueueElement));
	element2->task->originalPriority = 2;
	PriorityQueueElement *element3 = malloc(sizeof(PriorityQueueElement));
	element3->task->originalPriority = 3;
	PriorityQueueElement *element4 = malloc(sizeof(PriorityQueueElement));
	element4->task->originalPriority = 4;
	PriorityQueueElement *element5 = malloc(sizeof(PriorityQueueElement));
	element5->task->originalPriority = 5;
	PriorityQueueElement *element6 = malloc(sizeof(PriorityQueueElement));
	element6->task->originalPriority = 5;
	enqueue(queue, element1);
	enqueue(queue, element2);
	enqueue(queue, element3);
	enqueue(queue, element4);
	enqueue(queue, element5);
	enqueue(queue, element6);
	deletePriorityQueue(queue);
	return 0;
}
