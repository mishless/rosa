/*
 * rosa_scheduler_manager.h
 *
 * Created: 15/12/2015 18:34:43
 *  Author: Serena Santi
 */ 


#ifndef ROSA_SCHEDULER_MANAGER_H_
#define ROSA_SCHEDULER_MANAGER_H_

/*Made to avoid crappy function naming.
  Otherwise there would be ROSA_Start and ROSA_start*/

#define ROSA_Start() ROSA_StartScheduler()
void ROSA_StartScheduler(void);

void scheduler();

#endif /* ROSA_SCHEDULER_MANAGER_H_ */