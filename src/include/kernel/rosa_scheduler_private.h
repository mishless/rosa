/*
 * rosa_scheduler_private.h
 *
 * Created: 12/17/2015 1:33:33 PM
 *  Author: Kolja
 */ 


#ifndef ROSA_SCHEDULER_PRIVATE_H_
#define ROSA_SCHEDULER_PRIVATE_H_

#include "rosa_task_private.h"

Task *getCRT(void);

unsigned int isSchedulerStarted(void);

#endif /* ROSA_SCHEDULER_PRIVATE_H_ */