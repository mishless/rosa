#include "priority_queue.h"

int intComparator(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement) {
	if (firstElement->priority == secondElement->priority) {
		if (firstElement->id < secondElement->id) {
			return 1;
		}
	} else if (firstElement->priority > secondElement->priority) {
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
	element1->priority = 1;
	PriorityQueueElement *element2 = malloc(sizeof(PriorityQueueElement));
	element2->priority = 2;
	PriorityQueueElement *element3 = malloc(sizeof(PriorityQueueElement));
	element3->priority = 3;
	PriorityQueueElement *element4 = malloc(sizeof(PriorityQueueElement));
	element4->priority = 4;
	PriorityQueueElement *element5 = malloc(sizeof(PriorityQueueElement));
	element5->priority = 5;
	PriorityQueueElement *element6 = malloc(sizeof(PriorityQueueElement));
	element6->priority = 5;
	enqueue(queue, element1);
	enqueue(queue, element2);
	enqueue(queue, element3);
	enqueue(queue, element4);
	enqueue(queue, element5);
	enqueue(queue, element6);
	deletePriorityQueue(queue);
	return 0;
}
