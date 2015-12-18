/*
 * rosa_clock_manager.h
 *
 */ 


#ifndef ROSA_CLOCK_MANAGER_H_
#define ROSA_CLOCK_MANAGER_H_

#include "rosa_api_call.h"
#include "rosa_task_private.h"



void ROSA_DelayRelative (ROSA_TickCount tickCount);

void ROSA_DelayAbsolute (ROSA_TickCount reference,
							ROSA_TickCount tickCount);
							
ROSA_TickCount ROSA_TimerTickCount (void);

void ROSA_EndCycle (void);

#endif /* ROSA_CLOCK_MANAGER_H_ */