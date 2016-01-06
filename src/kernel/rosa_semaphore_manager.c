/*
 * rosa_semaphore_manager.c
 *
 */ 

#include "rosa_int.h"
#include "rosa_semaphore_manager.h"
#include "rosa_semaphore_private.h"

unsigned int semaphore_counter = 0;
unsigned int semaphore_id = 0;

int BLOCKEDcomparator(BlockedPriorityQueueElement *firstElement, BlockedPriorityQueueElement *secondElement)
{
	if (getPriority(firstElement->task) == getPriority(secondElement->task)) {
		return 0;
		} else if (getPriority(firstElement->task) > getPriority(secondElement->task)) {
		return 1;
	}
	return -1;
}

unsigned int ROSA_SemaphoreCreateBinary (SemaphoreHandle* handle, unsigned int initialValue)
{
	//Disable interrupts
	interruptDisable();
	
	Semaphore* semaphore = malloc(sizeof(Semaphore));
	
	// Check if memory was successfully allocated for semaphore structure
	if (semaphore == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	semaphore->SemaphoreBlockedQueue = createBlockedPriorityQueue(MAX_NUMBER_TASKS, &BLOCKEDcomparator);
	
	// Check if memory was successfully allocated for blocked priority queue structure
	if (semaphore->SemaphoreBlockedQueue == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}
	
	semaphore->id = semaphore_id++;
	
	// Check if the maximum number of tasks is already created
	if (++semaphore_counter > MAX_NUMBER_SEMAPHORES)
	{
		return TOO_MANY_SEMAPHORES;
	}
	
	semaphore->type = BINARY;
	
	if(initialValue != SEMAPHORE_OCCUPIED && initialValue != SEMAPHORE_FREE)
	{
		return INVALID_INITIAL_VALUE;
	}
	
	
	semaphore->state = initialValue;
	
	*handle = (SemaphoreHandle*)semaphore;
	
	//Enable interrupts
	interruptEnable();
	
	// Return SUCCESS
	return SUCCESS;
}

unsigned int ROSA_SemaphoreCreatePrio (SemaphoreHandle* handle, unsigned int initialValue, unsigned int priority)
{
	//Disable interrupts
	interruptDisable();
	
	Semaphore* semaphore = malloc(sizeof(Semaphore));
	
	// Check if memory was successfully allocated for semaphore structure
	if (semaphore == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	semaphore->SemaphoreBlockedQueue = createBlockedPriorityQueue(MAX_NUMBER_TASKS, &BLOCKEDcomparator);
	
	// Check if memory was successfully allocated for blocked priority queue structure
	if (semaphore->SemaphoreBlockedQueue == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}
	
	semaphore->id = semaphore_id++;
	
	// Check if the maximum number of tasks is already created
	if (++semaphore_counter > MAX_NUMBER_SEMAPHORES)
	{
		return TOO_MANY_SEMAPHORES;
	}
	
	semaphore->type = MUTEX;
	
	if(initialValue != SEMAPHORE_OCCUPIED && initialValue != SEMAPHORE_FREE)
	{
		return INVALID_INITIAL_VALUE;
	}
	
	if(priority == 0)
	{
		return INVALID_PRIORITY;
	}
	
	semaphore->priority = priority;
	
	semaphore->state = initialValue;
	
	*handle = (SemaphoreHandle*)semaphore;
	
	//Enable interrupts
	interruptEnable();
	
	// Return SUCCESS
	return SUCCESS;
}

unsigned int ROSA_SemaphoreDelete (SemaphoreHandle* handle)
{
	//Disable interrupts
	interruptDisable();
	
	//cast semaphore handle
	Semaphore* semaphore = (Semaphore*) handle;
	
	if (semaphore == NULL)
	{
		return SEMAPHORE_NOT_FOUND;
	}
	
	//deallocate blocked queue memory
	deleteBlockedPriorityQueue(semaphore->SemaphoreBlockedQueue);
	
	//deallocate semaphore structure memory
	free((void*)semaphore);
	
	// Decrement the semaphore counter
	semaphore_counter--;
	
	//Enable interrupts
	interruptEnable();
	
	// Check if the memory was deallocated successfully
	if(semaphore != NULL)
	{
		return FAILURE;
	}
	
	return SUCCESS;
}

unsigned int ROSA_SemaphoreTake (SemaphoreHandle handle, ROSA_TickCount timeout)
{
	// Return SUCCESS
	return SUCCESS;
}

unsigned int ROSA_SemaphoreGive (SemaphoreHandle handle)
{
	// Return SUCCESS
	return SUCCESS;
}