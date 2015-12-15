#include "priority_queue.h"

#define LEFT(x) (2 * (x) + 1)
#define RIGHT(x) (2 * (x) + 2)
#define PARENT(x) ((x)/2)

/* Create priority queue by specifying: maximum size of the queue, comparator function and callbackOnQueueChange function. */
PriorityQueue *createPriorityQueue(unsigned int maximumSize, int (*comparator)(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement), void (*callbackOnChange)()) {
    PriorityQueue *queue = NULL;
    queue = malloc(sizeof(*queue));
    queue->comparator = comparator;
    queue->callbackOnChange = callbackOnChange;
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
        queue->data[lastIndex] = element;
        queue->size++;
        PriorityQueueElement* temporaryElement = NULL;
        while (lastIndex > 0 && queue->comparator(queue->data[lastIndex], queue->data[PARENT(lastIndex)]) > 0) {
            temporaryElement = queue->data[lastIndex];
            queue->data[lastIndex] =  queue->data[PARENT(lastIndex)];
            queue->data[PARENT(lastIndex)] = temporaryElement;
            lastIndex = PARENT(lastIndex);
        }
    }
}

PriorityQueueElement *dequeue(PriorityQueue *queue) {
    if (queue->size > 0) {
        PriorityQueueElement* element = NULL;
        element = &(queue->data[0]);
        queue->data[0] = queue->data[queue->size-1];
        queue->size--;
        heapify(queue, 0);
        return element;
    }
}

void heapify(PriorityQueue *queue, unsigned int index) {
    PriorityQueueElement *temporaryElement = NULL;
    unsigned int leftIndex, rightIndex, largestElementIndex = index;
    leftIndex = LEFT(index);
    rightIndex = RIGHT(index);

    if (leftIndex < queue->size && queue->comparator(queue->data[index], queue->data[leftIndex]) < 0) {
        largestElementIndex = leftIndex;
    }
    if (rightIndex < queue->size && queue->comparator(queue->data[index], queue->data[rightIndex]) < 0) {
        largestElementIndex = rightIndex;
    }
    if (largestElementIndex != index) {
        temporaryElement = queue->data[index];
        queue->data[index] = queue->data[largestElementIndex];
        queue->data[largestElementIndex] = temporaryElement;
        heapify(queue, largestElementIndex);
    }
}

PriorityQueueElement *peek(PriorityQueue *queue) {
    return queue->data[0];
}

unsigned int isEmpty(PriorityQueue *queue) {
    if (queue->size > 0) {
        return 0;
    }
    return 1;
}



