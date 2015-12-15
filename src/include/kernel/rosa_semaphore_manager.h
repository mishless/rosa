/*
 * rosa_semaphore_manager.h
 *
 */ 

#ifndef ROSA_SEMAPHORE_MANAGER_H_
#define ROSA_SEMAPHORE_MANAGER_H_

#include "rosa_api_call.h"

unsigned int ROSA_SemaphoreCreateBinary (SemaphoreHandle* handle,
											unsigned int initialValue);
											
unsigned int ROSA_SemaphoreCreatePrio (SemaphoreHandle* handle,
											unsigned int initialValue,
											unsigned int priority);
											
unsigned int ROSA_SemaphoreDelete (SemaphoreHandle* handle);

unsigned int ROSA_SemaphoreTake (SemaphoreHandle handle,
									ROSA_TickCount timeout);
									
unsigned int ROSA_SemaphoreGive (SemaphoreHandle handle);

#endif /* ROSA_SEMAPHORE_MANAGER_H_ */