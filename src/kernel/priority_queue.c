#include "priority_queue.h"

#define LEFT(x) (2*(x) + 1)
#define RIGHT(x) (2*(x) + 2)
#define PARENT(x) ((x-1)/2)

unsigned int counter = 0;

/* Create priority queue by specifying: maximum size of the queue, comparator function and callbackOnQueueChange function. */
PriorityQueue *createPriorityQueue(unsigned int maximumSize, int (*comparator)(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement)) {
	PriorityQueue *queue = NULL;
	queue = malloc(sizeof(*queue));
	queue->maximumSize = maximumSize;
	queue->comparator = comparator;
	queue->data = malloc(maximumSize * sizeof(*(queue->data)));
	queue->size = 0;
	return queue;
}

/* Delete a priority queue and all data associated. */
void deletePriorityQueue(PriorityQueue *queue) {
	int i;
	for(i=0; i<queue->size; i++) {
		free(queue->data[i]);
	}
	free(queue->data);
	free(queue);
}

/* Enqueue a new element to the priority queue. */
void enqueue(PriorityQueue *queue, PriorityQueueElement *element) {
	if (queue->size < queue->maximumSize) {
		int lastIndex = queue->size;
		element->id = counter++;
		queue->data[lastIndex] = element;
		queue->size++;
		PriorityQueueElement* temporaryElement = NULL;
		while (lastIndex > 0 && (
		(queue->comparator(queue->data[lastIndex], queue->data[PARENT(lastIndex)]) > 0) ||
		((queue->comparator(queue->data[lastIndex], queue->data[PARENT(lastIndex)]) == 0) && (queue->data[lastIndex]->id < queue->data[PARENT(lastIndex)]->id)))) {
			temporaryElement = queue->data[lastIndex];
			queue->data[lastIndex] =  queue->data[PARENT(lastIndex)];
			queue->data[PARENT(lastIndex)] = temporaryElement;
			lastIndex = PARENT(lastIndex);
		}
	}
}

/* Dequeue the first element from the priority queue */
PriorityQueueElement *dequeue(PriorityQueue *queue) {
	if (queue->size > 0) {
		PriorityQueueElement* element = NULL;
		element = queue->data[0];
		queue->data[0] = queue->data[queue->size-1];
		queue->data[queue->size-1] = NULL;
		queue->size--;
		heapify(queue, 0);
		return element;
	}
	return NULL;
}

/* Heapify procedure for priority queue to restore its heapify property */
void heapify(PriorityQueue *queue, unsigned int index) {
	PriorityQueueElement *temporaryElement = NULL;
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
		heapify(queue, largestElementIndex);
	}
}

/* Peek the first element of the priority queue */
PriorityQueueElement *peek(PriorityQueue *queue) {
	return queue->data[0];
}

/* Returns true if the priority queue is empty and false if it is not. */
unsigned int isEmpty(PriorityQueue *queue) {
	if (queue->size > 0) {
		return 0;
	}
	return 1;
}