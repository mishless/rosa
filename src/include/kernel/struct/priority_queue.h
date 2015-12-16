#ifndef PRIORITY_QUEUE_H_INCLUDED
#define PRIORITY_QUEUE_H_INCLUDED

#include <stdlib.h>
#include "kernel/rosa_task_private.h"
/* PriorityQueueElement contains priority and task information */
typedef struct PriorityQueueElement {
    Task* task;
} PriorityQueueElement;

/* PriorityQueue contains size, maximumSize, array of pointers to elements of type PriorityQueueElement, comparator function and callbackOnChange function. */
typedef struct PriorityQueue {
    unsigned int size;
    unsigned int maximumSize;
    PriorityQueueElement **data;
    int (*comparator)(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement);
    void (*callbackOnChange)();
} PriorityQueue;

PriorityQueue *createPriorityQueue(unsigned int maximumSize, int (*comparator)(PriorityQueueElement *firstElement, PriorityQueueElement *secondElement), void (*callbackOnChange)());
void deletePriorityQueue(PriorityQueue *queue);
void enqueue(PriorityQueue *queue, PriorityQueueElement *element);
PriorityQueueElement *peek(PriorityQueue *queue);
PriorityQueueElement *dequeue(PriorityQueue *queue);
unsigned int isEmpty(PriorityQueue *queue);

#endif // PRIORITY_QUEUE_H_INCLUDED