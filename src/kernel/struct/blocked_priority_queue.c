/*
* blocked_priority_queue.c
*
* Created: 12/18/2015 7:18:20 PM
*  Author: Mihaela Stoycheva
*/
#include "blocked_priority_queue.h"

#define LEFT(x) (2*(x) + 1)
#define RIGHT(x) (2*(x) + 2)
#define PARENT(x) ((x-1)/2)

/* Create blocked priority queue by specifying: maximum size of the queue, comparator function and callbackOnQueueChange function. */
BlockedPriorityQueue *createBlockedPriorityQueue(unsigned int maximumSize, int (*comparator)(BlockedPriorityQueueElement *firstElement, BlockedPriorityQueueElement *secondElement)) {
	BlockedPriorityQueue *queue = NULL;
	queue = malloc(sizeof(*queue));
	queue->maximumSize = maximumSize;
	queue->comparator = comparator;
	queue->data = malloc(maximumSize * sizeof(*(queue->data)));
	queue->size = 0;
	queue->counter = 0;
	return queue;
}

/* Delete a priority queue and all data associated. */
void deleteBlockedPriorityQueue(BlockedPriorityQueue *queue) {
	int i;
	for(i=0; i<queue->size; i++) {
		free(queue->data[i]);
	}
	free(queue->data);
	free(queue);
}

/* Enqueue a new element to the priority queue. */
void enqueueBlockedPriorityQueue(BlockedPriorityQueue *queue, BlockedPriorityQueueElement *element) {
	if (queue->size < queue->maximumSize) {
		/* Assign id to the element that is to be enqueued */
		element->id = queue->counter++;

		/* Clone the element that is to be enqueued and assign it an id */
		BlockedPriorityQueueElement *secondaryElement = malloc(sizeof(BlockedPriorityQueueElement));
		secondaryElement->task = element->task;
		secondaryElement->buddyQueue = queue;

		int primaryLastIndex = queue->size;
		queue->data[primaryLastIndex] = element;
		queue->size++;
		BlockedPriorityQueueElement* temporaryElement = NULL;
		while (primaryLastIndex > 0 && (
		(queue->comparator(queue->data[primaryLastIndex], queue->data[PARENT(primaryLastIndex)]) > 0) ||
		((queue->comparator(queue->data[primaryLastIndex], queue->data[PARENT(primaryLastIndex)]) == 0) && (queue->data[primaryLastIndex]->id < queue->data[PARENT(primaryLastIndex)]->id)))) {
			temporaryElement = queue->data[primaryLastIndex];
			queue->data[primaryLastIndex] =  queue->data[PARENT(primaryLastIndex)];
			queue->data[PARENT(primaryLastIndex)] = temporaryElement;
			primaryLastIndex = PARENT(primaryLastIndex);
		}
		secondaryElement->buddyElement = &(queue->data[primaryLastIndex]);
		secondaryElement->id = queue->data[primaryLastIndex]->buddyQueue->counter++;

		int secondaryLastIndex = queue->data[primaryLastIndex]->buddyQueue->size;
		queue->data[primaryLastIndex]->buddyQueue->data[secondaryLastIndex] = secondaryElement;
		queue->data[primaryLastIndex]->buddyQueue->size++;

		queue->data[primaryLastIndex]->buddyElement = &(queue->data[primaryLastIndex]->buddyQueue->data[secondaryLastIndex]);

		updateBuddies(queue);

		BlockedPriorityQueueElement* secondaryTemporaryElement = NULL;
		while (secondaryLastIndex > 0 && (
		(queue->data[primaryLastIndex]->buddyQueue->comparator(queue->data[primaryLastIndex]->buddyQueue->data[secondaryLastIndex], queue->data[primaryLastIndex]->buddyQueue->data[PARENT(secondaryLastIndex)]) > 0) ||
		((queue->data[primaryLastIndex]->buddyQueue->comparator(queue->data[primaryLastIndex]->buddyQueue->data[secondaryLastIndex], queue->data[primaryLastIndex]->buddyQueue->data[PARENT(secondaryLastIndex)]) == 0) &&
		(queue->data[primaryLastIndex]->buddyQueue->data[secondaryLastIndex]->id < queue->data[primaryLastIndex]->buddyQueue->data[PARENT(secondaryLastIndex)]->id)))) {
			secondaryTemporaryElement = queue->data[primaryLastIndex]->buddyQueue->data[secondaryLastIndex];
			queue->data[primaryLastIndex]->buddyQueue->data[secondaryLastIndex] =  queue->data[primaryLastIndex]->buddyQueue->data[PARENT(secondaryLastIndex)];
			queue->data[primaryLastIndex]->buddyQueue->data[PARENT(secondaryLastIndex)] = secondaryTemporaryElement;
			secondaryLastIndex = PARENT(secondaryLastIndex);
		}

		updateBuddies(queue->data[primaryLastIndex]->buddyQueue);
	}
}

/* Update buddy elements of every element in the blocked priority queue */
void updateBuddies(BlockedPriorityQueue* queue) {
	int i = 0;
	for (i = 0; i<queue->size; i++) {
		if ((*queue->data[i]->buddyElement)->buddyElement != &(queue->data[i])) {
			(*queue->data[i]->buddyElement)->buddyElement = &(queue->data[i]);
		}
	}
}

/* Dequeue the first element from the blocked priority queue */
BlockedPriorityQueueElement *dequeueBlockedPriorityQueue(BlockedPriorityQueue *queue) {
	if (queue->size > 0) {
		BlockedPriorityQueueElement* element = NULL;
		element = queue->data[0];
		queue->data[0] = queue->data[queue->size-1];
		queue->data[queue->size-1] = NULL;
		queue->size--;
		heapifyBlockedPriorityQueue(queue, 0);

		updateBuddies(queue);
		/* Delete buddy element and heapify buddy queue */
		unsigned int index = element->buddyElement - ((struct BlockedPriorityQueueElement**) element->buddyQueue->data);
		element->buddyQueue->data[index] = element->buddyQueue->data[element->buddyQueue->size-1];
		element->buddyQueue->data[element->buddyQueue->size-1] = NULL;
		element->buddyQueue->size--;
		heapifyBlockedPriorityQueue(element->buddyQueue, index);

		updateBuddies(element->buddyQueue);
		return element;
	}
}

/* Heapify procedure for priority queue to restore its heapify property */
void heapifyBlockedPriorityQueue(BlockedPriorityQueue *queue, unsigned int index) {
	BlockedPriorityQueueElement *temporaryElement = NULL;
	unsigned int leftIndex, rightIndex, largestElementIndex = index;
	leftIndex = LEFT(index);
	rightIndex = RIGHT(index);

	if (leftIndex < queue->size && ((queue->comparator(queue->data[largestElementIndex], queue->data[leftIndex]) < 0) ||
	((queue->comparator(queue->data[largestElementIndex], queue->data[leftIndex]) == 0) && (queue->data[largestElementIndex]->id > queue->data[leftIndex]->id)))) {
		largestElementIndex = leftIndex;
	}
	if (rightIndex < queue->size && ((queue->comparator(queue->data[largestElementIndex], queue->data[rightIndex]) < 0) ||
	((queue->comparator(queue->data[largestElementIndex], queue->data[rightIndex]) == 0) &&
	(queue->data[largestElementIndex]->id > queue->data[rightIndex]->id)))) {
		largestElementIndex = rightIndex;
	}
	if (largestElementIndex != index) {
		temporaryElement = queue->data[index];
		queue->data[index] = queue->data[largestElementIndex];
		queue->data[largestElementIndex] = temporaryElement;
		heapifyBlockedPriorityQueue(queue, largestElementIndex);
	}
}

/* Peek the first element of the priority queue */
BlockedPriorityQueueElement *peekBlockedPriorityQueue(BlockedPriorityQueue *queue) {
	return queue->data[0];
}

/* Returns true if the priority queue is empty and false if it is not. */
unsigned int isEmptyBlockedPriorityQueue(BlockedPriorityQueue *queue) {
	if (queue->size > 0) {
		return 0;
	}
	return 1;
}