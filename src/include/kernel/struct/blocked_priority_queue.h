/*
* blocked_priority_queue.h
*
* Created: 12/18/2015 7:16:13 PM
*  Author: Mihaela Stoycheva
*/


#ifndef BLOCKED_PRIORITY_QUEUE_H_
#define BLOCKED_PRIORITY_QUEUE_H_

#include <stdlib.h>
#include "rosa_task_private.h"

/* BlockedPriorityQueueElement contains id, task information, pointer to another buddy BlockedPriorityQueueElement and a pointer to another buddy BlockedPriorityQueue. */
struct BlockedPriorityQueueElement {
	unsigned int id;
	Task* task;
	struct BlockedPriorityQueueElement **buddyElement;
	struct BlockedPriorityQueue *buddyQueue;
};

typedef struct BlockedPriorityQueueElement BlockedPriorityQueueElement;

/* BlockedPriorityQueue contains size, maximumSize, array of pointers to elements of type BlockedPriorityQueue, comparator function and callbackOnChange function. */
struct BlockedPriorityQueue {
	unsigned int size;
	unsigned int counter;
	unsigned int maximumSize;
	BlockedPriorityQueueElement **data;
	int (*comparator)(BlockedPriorityQueueElement *firstElement, BlockedPriorityQueueElement *secondElement);
};

typedef struct BlockedPriorityQueue BlockedPriorityQueue;

BlockedPriorityQueue *createBlockedPriorityQueue(unsigned int maximumSize, int (*comparator)(BlockedPriorityQueueElement *firstElement, BlockedPriorityQueueElement *secondElement));
void deleteBlockedPriorityQueue(BlockedPriorityQueue *queue);
void enqueueBlockedPriorityQueue(BlockedPriorityQueue *queue, BlockedPriorityQueueElement *element);
BlockedPriorityQueueElement *peekBlockedPriorityQueue(BlockedPriorityQueue *queue);
BlockedPriorityQueueElement *dequeueBlockedPriorityQueue(BlockedPriorityQueue *queue);
void heapifyBlockedPriorityQueue(BlockedPriorityQueue *queue, unsigned int index);
unsigned int isEmptyBlockedPriorityQueue(BlockedPriorityQueue *queue);
void updateBuddies(BlockedPriorityQueue* queue);

#endif /* BLOCKED_PRIORITY_QUEUE_H_ */