/*
 * semaphore_manager_tests.c
 *
 */ 

#include <avr32/io.h>
#include "helper_functions.h"
#include "rosa_api_call.h"
#include "rosa_task_private.h"
#include "rosa_ker.h"
#include "tests.h"
#include "rosa_queue_manager.h"
#include "priority_queue.h"
#include "blocked_priority_queue.h"
#include "semaphore_manager_tests.h"
#include "rosa_semaphore_manager.h"
#include "rosa_semaphore_private.h"

#define BIG_TIMEOUT 100000000
#define TASK_NAME			"test"
#define SMALL_STACK_SIZE	4000
#define PRIORITY_6			6
#define PRIORITY_5			5
#define PRIORITY_4			4
#define PRIORITY_3			3
#define PRIORITY_2			2
#define TIMEOUT_TIME		100
#define MAX_TIMEOUT			4000000

SemaphoreHandle semaphore_higher_priority_than_task;
TaskHandle task_checking_own_changed_priority;

SemaphoreHandle semaphore_handle_t;
SemaphoreHandle semaphore_handle_h;
SemaphoreHandle semaphore_handle_m;
SemaphoreHandle semaphore_handle_l;

TaskHandle taskHighPriority;
TaskHandle taskMiddlePriority;
TaskHandle taskLowPriority;
TaskHandle taskLowLowPriority;

unsigned int flag = 0;
>>>>>>> Stashed changes

void sm_create_binary_01()
{
	/*ROSA_SemaphoreCreateBinary should allocate and populate the semaphore structure 
	  correctly and assign it to the provided handle.*/
	
	SemaphoreHandle semaphore_handle;

	send_id("SM-CREATE-BINARY-01");
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	
	if( ((Semaphore*)semaphore_handle)->type != BINARY)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->state != SEMAPHORE_FREE)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->SemaphoreBlockedQueue == NULL)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->id != 0)
	{
		send_fail();
	}
		
	send_success();
}

void sm_create_binary_02()
{	
	SemaphoreHandle semaphore_handle;
	Semaphore* semaphore;
	
	send_id("SM-CREATE-BINARY-02");
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	
	semaphore = (Semaphore*) semaphore_handle;
	
	if(semaphore->SemaphoreBlockedQueue->maximumSize != MAX_NUMBER_TASKS)
	{
		send_fail();
	}
		
	send_success();
}

void sm_create_binary_05()
{
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE-BINARY-05");
	
	int return_code = ROSA_SemaphoreCreateBinary(&semaphore_handle, 5);
	
	if(return_code != INVALID_INITIAL_VALUE)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_binary_06()
{
	SemaphoreHandle semaphore_handle;
	int i = 0;
	
	send_id("SM-CREATE-BINARY-06");
	
	for (i = 0; i < MAX_NUMBER_SEMAPHORES; i++)
	{
		ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);		
	}
	
	int return_code = ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	
	if(return_code != TOO_MANY_SEMAPHORES)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_binary_07()
{
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE-BINARY-07");
	
	int enough_memory;
	do
	{
		enough_memory = (int) malloc(1);
	} while (enough_memory);
	
	int return_code = ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	
	if(return_code != NOT_ENOUGH_MEMORY)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_binary_08()
{
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE-BINARY-08");
	
	int return_code = ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	
	if(return_code != SUCCESS)
	{
		send_fail();
	}
	
	send_success();
}

void task_sm_cb_09 ()
{
	SemaphoreHandle semaphore_handle;
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	
	if( ((Semaphore*)semaphore_handle)->type != BINARY)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->state != SEMAPHORE_FREE)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->SemaphoreBlockedQueue == NULL)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->id != 0)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_binary_09()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("SM-CREATE-BINARY-09");
	
	ROSA_CreateTask(task_sm_cb_09, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();		
}

void task_sm_cb_10 ()
{
	SemaphoreHandle semaphore_handle;
	Semaphore* semaphore;
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	
	semaphore = (Semaphore*) semaphore_handle;
	
	if(semaphore->SemaphoreBlockedQueue->maximumSize != MAX_NUMBER_TASKS)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_binary_10()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("SM-CREATE-BINARY-10");
	
	ROSA_CreateTask(task_sm_cb_10, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

void sm_create_prio_01()
{	
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE-PRIO-01");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, PRIORITY_5);
	
	if( ((Semaphore*)semaphore_handle)->type != MUTEX)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->priority != PRIORITY_5)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->state != SEMAPHORE_FREE)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->SemaphoreBlockedQueue == NULL)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->id != 0)
	{
		send_fail();
	}
		
	send_success();
}

void sm_create_prio_02()
{	
	SemaphoreHandle semaphore_handle;
	Semaphore* semaphore;
	
	send_id("SM-CREATE-PRIO-02");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, PRIORITY_5);
	
	semaphore = (Semaphore*) semaphore_handle;
	
	if(semaphore->SemaphoreBlockedQueue->maximumSize != MAX_NUMBER_TASKS)
	{
		send_fail();
	}
		
	send_success();
}

void sm_create_prio_05()
{
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE-PRIO-05");
	
	int return_code = ROSA_SemaphoreCreatePrio(&semaphore_handle, 5, PRIORITY_5);
	
	if(return_code != INVALID_INITIAL_VALUE)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_prio_06()
{
	SemaphoreHandle semaphore_handle;
	int i = 0;
	
	send_id("SM-CREATE-PRIO-06");
	
	for (i = 0; i < MAX_NUMBER_SEMAPHORES; i++)
	{
		ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, PRIORITY_5);
	}
	
	int return_code = ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, PRIORITY_5);
	
	if(return_code != TOO_MANY_SEMAPHORES)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_prio_07()
{
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE-PRIO-07");
	
	int enough_memory;
	do
	{
		enough_memory = (int) malloc(1);
	} while (enough_memory);
	
	int return_code = ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, PRIORITY_5);
	
	if(return_code != NOT_ENOUGH_MEMORY)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_prio_08()
{
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE-PRIO-08");
	
	int return_code = ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, 0);
	
	if(return_code != INVALID_PRIORITY)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_prio_09()
{
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE-PRIO-09");
	
	int return_code = ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, PRIORITY_5);
	
	if(return_code != SUCCESS)
	{
		send_fail();
	}
	
	send_success();
}

void task_sm_cp_10 ()
{
	SemaphoreHandle semaphore_handle;
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, PRIORITY_5);
	
	if( ((Semaphore*)semaphore_handle)->type != MUTEX)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->state != SEMAPHORE_FREE)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->SemaphoreBlockedQueue == NULL)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle)->id != 0)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_prio_10()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("SM-CREATE-PRIO-10");
	
	ROSA_CreateTask(task_sm_cp_10, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();		
}

void task_sm_cp_11 ()
{
	SemaphoreHandle semaphore_handle;
	Semaphore* semaphore;
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, PRIORITY_5);
	
	semaphore = (Semaphore*) semaphore_handle;
	
	if(semaphore->SemaphoreBlockedQueue->maximumSize != MAX_NUMBER_TASKS)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_prio_11()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("SM-CREATE-PRIO-11");
	
	ROSA_CreateTask(task_sm_cp_11, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

void sm_create_mix_01()
{
	SemaphoreHandle semaphore_handle_binary;
	SemaphoreHandle semaphore_handle_prio;
	
	send_id("SM-CREATE-MIX-01");
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_binary, SEMAPHORE_FREE);
	ROSA_SemaphoreCreatePrio(&semaphore_handle_prio, SEMAPHORE_FREE, PRIORITY_5);
	
	if( ((Semaphore*)semaphore_handle_binary)->type != BINARY)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_binary)->state != SEMAPHORE_FREE)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_binary)->SemaphoreBlockedQueue == NULL)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_binary)->id != 0)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->type != MUTEX)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->priority != PRIORITY_5)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->state != SEMAPHORE_FREE)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->SemaphoreBlockedQueue == NULL)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->id != 1)
	{
		send_fail();
	}
		
	send_success();
}

void task_sm_cm_02 ()
{
	SemaphoreHandle semaphore_handle_binary;
	SemaphoreHandle semaphore_handle_prio;
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_binary, SEMAPHORE_FREE);
	ROSA_SemaphoreCreatePrio(&semaphore_handle_prio, SEMAPHORE_FREE, PRIORITY_5);
	
	if( ((Semaphore*)semaphore_handle_binary)->type != BINARY)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_binary)->state != SEMAPHORE_FREE)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_binary)->SemaphoreBlockedQueue == NULL)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_binary)->id != 0)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->type != MUTEX)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->priority != PRIORITY_5)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->state != SEMAPHORE_FREE)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->SemaphoreBlockedQueue == NULL)
	{
		send_fail();
	}
	
	if( ((Semaphore*)semaphore_handle_prio)->id != 1)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_mix_02()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("SM-CREATE-MIX-02");
	
	ROSA_CreateTask(task_sm_cm_02, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

void task_sm_d_03 ()
{
	SemaphoreHandle semaphore_handle;
	int i = 0;
		
	while(i < MAX_NUMBER_SEMAPHORES)
	{
		if(ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE) != SUCCESS)
		{
			send_fail();
		}
			if(ROSA_SemaphoreDelete(semaphore_handle) != SUCCESS)
		{
			send_fail();
		}
		i++;
	}
	
	send_success();
}

void sm_delete_03()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("SM-DELETE-03");
	
	ROSA_CreateTask(task_sm_d_03, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

void sm_delete_04()
{
	send_id("SM-DELETE-04");
	
	SemaphoreHandle semaphore_handle;
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	if(ROSA_SemaphoreDelete(semaphore_handle) != SUCCESS)
	{
		send_fail();
	}

	send_success();	
}

/*void sm_delete_05()
{
	send_id("SM-DELETE-05");
	
	SemaphoreHandle semaphore_handle;
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	if(ROSA_SemaphoreDelete((void *) 42) != SEMAPHORE_NOT_FOUND)
	{
		send_fail();
	}

	send_success();
}*/

void task_sm_tb_01 ()
{
	ROSA_SemaphoreTake(semaphore_handle_t, 0);
	
	if( ((Semaphore*)semaphore_handle_t)->state == SEMAPHORE_OCCUPIED)
	{
		send_success();
	}
	
	send_fail();
}

void sm_take_binary_01()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-BINARY-01");
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_FREE);
	
	ROSA_CreateTask(task_sm_tb_01, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

void task_sm_tb_02_h ()
{
	if( (ROSA_SemaphoreTake(semaphore_handle_t, TIMEOUT_TIME)) == TIMEOUT )
	{
		send_success();
	}
	
	send_fail();
}

void task_sm_tb_02_l ()
{
	send_success();
}

void sm_take_binary_02()
{
	TaskHandle task_handle_high;
	TaskHandle task_handle_low;
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-BINARY-02");
	
	ROSA_CreateTask(task_sm_tb_02_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle_high);
	ROSA_CreateTask(task_sm_tb_02_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &task_handle_low);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

void task_sm_tb_03_h ()
{
	ROSA_SemaphoreTake(semaphore_handle_t, TIMEOUT_TIME);
}

void task_sm_tb_03_l ()
{
	BlockedPriorityQueue blocked_queue;
	blocked_queue = fetchBLOCKEDqueue();
	
	if(blocked_queue.size != 1)
	send_fail();
	
	if ((blocked_queue.data[0]->task) != (Task*) taskHighPriority)
	{
		send_fail();
	}
	send_success();
}

void sm_take_binary_03()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-BINARY-03");
	
	ROSA_CreateTask(task_sm_tb_03_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tb_03_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

void task_sm_tb_04_h ()
{
	ROSA_SemaphoreTake(semaphore_handle_t, TIMEOUT_TIME);
}

void task_sm_tb_04_l ()
{
	if( ((Semaphore*)semaphore_handle_t)->state == SEMAPHORE_OCCUPIED)
	{
		send_success();
	}
	
	send_fail();
}

void sm_take_binary_04()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-BINARY-04");
	
	ROSA_CreateTask(task_sm_tb_04_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tb_04_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

void task_sm_tb_05_h ()
{
	ROSA_SemaphoreTake(semaphore_handle_t, TIMEOUT_TIME);
}

void task_sm_tb_05_m ()
{
	ROSA_SemaphoreTake(semaphore_handle_t, TIMEOUT_TIME);
}

void task_sm_tb_05_l ()
{
	Semaphore* semaphore = (Semaphore*) semaphore_handle_t;
	BlockedPriorityQueue* blocked_queue = (BlockedPriorityQueue*)semaphore->SemaphoreBlockedQueue;
	
	if(blocked_queue->size != 2)
	send_fail();
	
	if ((blocked_queue->data[0]->task) != (Task*) taskHighPriority)
	{
		send_fail();
	}
	
	if ((blocked_queue->data[1]->task) != (Task*) taskMiddlePriority)
	{
		send_fail();
	}
	
	send_success();
}

void sm_take_binary_05()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-BINARY-05");
	
	ROSA_CreateTask(task_sm_tb_05_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tb_05_m, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tb_05_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

void task_sm_tb_06_h ()
{
	ROSA_DelayAbsolute(0, 40);
	ROSA_SemaphoreTake(semaphore_handle_t, TIMEOUT_TIME);
}

void task_sm_tb_06_m ()
{
	ROSA_SemaphoreTake(semaphore_handle_t, TIMEOUT_TIME);
}

void task_sm_tb_06_l ()
{
	Semaphore* semaphore = (Semaphore*) semaphore_handle_t;
	BlockedPriorityQueue* blocked_queue;
	
	ROSA_DelayAbsolute(0, 50);
	blocked_queue = (BlockedPriorityQueue*)semaphore->SemaphoreBlockedQueue;
	
	if(blocked_queue->size != 2)
	send_fail();
	
	if ((blocked_queue->data[0]->task) != (Task*) taskHighPriority)
	{
		send_fail();
	}
	
	if ((blocked_queue->data[1]->task) != (Task*) taskMiddlePriority)
	{
		send_fail();
	}
	
	send_success();
}

void sm_take_binary_06()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-BINARY-06");
	
	ROSA_CreateTask(task_sm_tb_06_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tb_06_m, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tb_06_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

void task_sm_tb_07 ()
{
	ROSA_TickCount ticks;
	ROSA_DelayAbsolute(0, 100);
	ROSA_SemaphoreTake(semaphore_handle_t, 0);
	
	ticks = ROSA_TimerTickCount();
	
	if (ticks == 100)
		send_success();
	else 
		send_fail();	
}

void sm_take_binary_07()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-BINARY-07");
	
	ROSA_CreateTask(task_sm_tb_07, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

/*void task_sm_tb_08_h ()
{
	ROSA_TickCount ticks;
	ROSA_DelayAbsolute(0, TIMEOUT_TIME);
	ROSA_SemaphoreTake(semaphore_handle_01, 0);
	
	ticks = ROSA_TimerTickCount();
	
	if (ticks == 100)
	send_success();
	else
	send_fail();
}

void task_sm_tb_08_l ()
{
	ROSA_TickCount ticks;
	ROSA_DelayAbsolute(0, 100);
	ROSA_SemaphoreTake(semaphore_handle_01, 0);
	
	ticks = ROSA_TimerTickCount();
	
	if (ticks == 100)
	send_success();
	else
	send_fail();
}

void sm_take_binary_08()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-BINARY-08");
	
	ROSA_CreateTask(task_sm_tb_08_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tb_08_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_01, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}*/

void task_sm_tp_01()
{
	Semaphore* sem;
	
	ROSA_SemaphoreTake(semaphore_handle_t, TIMEOUT_TIME);
	
	sem = (Semaphore*) semaphore_handle_t;
	
	if(sem->state == SEMAPHORE_OCCUPIED)
		send_success();
	else
		send_fail();
}

void sm_take_prio_01()
{
	/*Check if it possible to take a priority semaphore*/
	
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-01");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_01, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_02_h()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
	send_fail();
}

void task_sm_tp_02_l()
{
	send_success();
}

void sm_take_prio_02()
{	
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-02");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_02_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_02_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_03_h()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
	ROSA_DelayAbsolute(0, 40000000);
}

void task_sm_tp_03_m()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
}

void task_sm_tp_03_l()
{
	Semaphore* semaphore = (Semaphore*) semaphore_handle_t;
	BlockedPriorityQueue* blocked_queue;
	blocked_queue = (BlockedPriorityQueue*)semaphore->SemaphoreBlockedQueue;
	
	if(blocked_queue->size != 1)
	send_fail();
	
	if ((blocked_queue->data[0]->task) != (Task*) taskMiddlePriority)
	{
		send_fail();
	}
	
	send_success();
}

void sm_take_prio_03()
{
	/*Check if  an occupied priority semaphore can’t be taken*/
	
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-03");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_03_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_03_m, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tp_03_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_04_h()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
	ROSA_DelayAbsolute(0, 40000000);
}

void task_sm_tp_04_l()
{
	Semaphore* semaphore = (Semaphore*) semaphore_handle_t;
	
	if (semaphore->state != SEMAPHORE_OCCUPIED)
	{
		send_fail();
	}
	
	send_success();
}

void sm_take_prio_04()
{
	/*Check if  an occupied priority semaphore can’t be taken*/
	
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-04");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_04_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_04_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_05_h()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
	ROSA_DelayAbsolute(0, 40000000);
}

void task_sm_tp_05_m()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
}

void task_sm_tp_05_l()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
}

void task_sm_tp_05_ll()
{
	Semaphore* semaphore = (Semaphore*) semaphore_handle_t;
	BlockedPriorityQueue* blocked_queue;
	blocked_queue = (BlockedPriorityQueue*)semaphore->SemaphoreBlockedQueue;
	
	if(blocked_queue->size != 2)
	send_fail();
	
	if ((blocked_queue->data[0]->task) != (Task*) taskMiddlePriority)
	{
		send_fail();
	}
	
	if ((blocked_queue->data[1]->task) != (Task*) taskLowPriority)
	{
		send_fail();
	}
	
	send_success();
}

void sm_take_prio_05()
{
	/*Check if  an occupied priority semaphore can’t be taken*/
	
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-05");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_05_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_05_m, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tp_05_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	ROSA_CreateTask(task_sm_tp_05_ll, task_name, SMALL_STACK_SIZE, PRIORITY_2, &taskLowLowPriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_06_h()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
	ROSA_DelayAbsolute(0, 40000000);
}

void task_sm_tp_06_m()
{
	ROSA_DelayAbsolute(0, 10);
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
}

void task_sm_tp_06_l()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
}

void task_sm_tp_06_ll()
{
	Semaphore* semaphore = (Semaphore*) semaphore_handle_t;
	BlockedPriorityQueue* blocked_queue;
	
	ROSA_DelayAbsolute(0, 11);
	blocked_queue = (BlockedPriorityQueue*)semaphore->SemaphoreBlockedQueue;
	
	if(blocked_queue->size != 2)
	send_fail();
	
	if ((blocked_queue->data[0]->task) != (Task*) taskMiddlePriority)
	{
		send_fail();
	}
	
	if ((blocked_queue->data[1]->task) != (Task*) taskLowPriority)
	{
		send_fail();
	}
	
	send_success();
}

void sm_take_prio_06()
{
	/*Check if  an occupied priority semaphore can’t be taken*/
	
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-06");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_06_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_06_m, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tp_06_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	ROSA_CreateTask(task_sm_tp_06_ll, task_name, SMALL_STACK_SIZE, PRIORITY_2, &taskLowLowPriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_07_h()
{
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
	ROSA_DelayAbsolute(0, 40000000);
}

void task_sm_tp_07_l()
{
	ROSA_TickCount ticks;
	ROSA_DelayAbsolute(0, 100);
	ROSA_SemaphoreTake(semaphore_handle_t, 0);
	
	ticks = ROSA_TimerTickCount();
	if (ticks == 100)
	{
		send_success();
	}
	else
		send_fail();
}

void sm_take_prio_07()
{
	/*Check if  an occupied priority semaphore can’t be taken*/
	
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-07");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_07_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_07_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}

/*
void task_sm_tp_10()
{
	ROSA_SemaphoreTake(semaphore_handle_t, 0);
	if((peekFromStack(((Task*)taskHighPriority).temporaryPriority)) != PRIORITY_5)
	{
		send_fail();
	}
	else
		send_success();
}

void sm_take_prio_10()
{	
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-10");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_10, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskHighPriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_11()
{
	ROSA_SemaphoreTake(semaphore_handle_t, 0);
	if((peekFromStack(((Task*)taskHighPriority).temporaryPriority)) != PRIORITY_5)
	{
		send_fail();
	}
	else
	send_success();
}

void sm_take_prio_11()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-11");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_4);
	
	ROSA_CreateTask(task_sm_tp_11, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_12()
{
	ROSA_SemaphoreTake(semaphore_handle_h, 0);
	if((peekFromStack(((Task*)taskMiddlePriority).temporaryPriority)) != PRIORITY_3)
	{
		send_fail();
	}
	else
		send_success();
	
	ROSA_SemaphoreTake(semaphore_handle_m, 0);
	if((peekFromStack(((Task*)taskMiddlePriority).temporaryPriority)) != PRIORITY_5)
	{
		send_fail();
	}
	else
		send_success();
	
	ROSA_SemaphoreTake(semaphore_handle_l, 0);
	if((peekFromStack(((Task*)taskMiddlePriority).temporaryPriority)) != PRIORITY_5)
	{
		send_fail();
	}
	else
		send_success();
		
	ROSA_SemaphoreGive(semaphore_handle_l);
	if((peekFromStack(((Task*)taskMiddlePriority).temporaryPriority)) != PRIORITY_5)
	{
		send_fail();
	}
	else
		send_success();
		
	ROSA_SemaphoreGive(semaphore_handle_m);
	if((peekFromStack(((Task*)taskMiddlePriority).temporaryPriority)) != PRIORITY_5)
	{
		send_fail();
	}
	else
	send_success();
	
	ROSA_SemaphoreGive(semaphore_handle_h);
	if((peekFromStack(((Task*)taskMiddlePriority).temporaryPriority)) != PRIORITY_3)
	{
		send_fail();
	}
	else
	send_success();
}

void sm_take_prio_12()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-12");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_h, SEMAPHORE_FREE, PRIORITY_2);
	ROSA_SemaphoreCreatePrio(&semaphore_handle_m, SEMAPHORE_FREE, PRIORITY_5);
	ROSA_SemaphoreCreatePrio(&semaphore_handle_l, SEMAPHORE_FREE, PRIORITY_4);
	
	ROSA_CreateTask(task_sm_tp_12, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskMiddlePriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_13_h()
{
	ROSA_DelayAbsolute(0, 30);
	flag = 1;
	send_fail();
}

void task_sm_tp_13_l()
{
	ROSA_SemaphoreTake(semaphore_handle_t, 0);
	ROSA_DelayAbsolute(0, 50);
	if(flag == 0)
		send_success();
	send_fail();
}*/
/*
void sm_take_prio_13()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-13");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_13_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_13_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}

void task_sm_tp_14_h()
{
	ROSA_TickCount ticks;
	ROSA_DelayAbsolute(0, 30);
	ROSA_SemaphoreTake(semaphore_handle_t, 0);
	
	ticks = ROSA_TimerTickCount();
	if(ticks < 100)
		send_success();
	else
		send_fail();
}

void task_sm_tp_14_m()
{
	ROSA_DelayAbsolute(0, 35);
	busy_wait(1000000000);
	send_fail();
}

void task_sm_tp_14_l()
{
	ROSA_SemaphoreTake(semaphore_handle_t, 0);
	busy_wait(50);
	ROSA_SemaphoreGive(semaphore_handle_t);
}

void sm_take_prio_14()
{	
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-13");
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_14_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_14_m, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tp_14_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}
*/

void take_higher_priority_semaphore_and_check_own_priority()
{
	ROSA_SemaphoreTake(semaphore_higher_priority_than_task, 0);
	if (getPriority(((Task*) task_checking_own_changed_priority)) == (((Semaphore*) semaphore_higher_priority_than_task)->priority))
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void sm_take_prio_10()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-10");
	
	ROSA_CreateTask(take_higher_priority_semaphore_and_check_own_priority, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_checking_own_changed_priority);
	ROSA_SemaphoreCreatePrio(&semaphore_higher_priority_than_task, SEMAPHORE_FREE, PRIORITY_6);
	ROSA_StartScheduler();
}

SemaphoreHandle semaphore_lower_priority_than_task;
TaskHandle task_checking_own_not_changed_priority;

void take_lower_priority_semaphore_and_check_own_priority()
{
	ROSA_SemaphoreTake(semaphore_higher_priority_than_task, 0);
	if (getPriority(((Task*) task_checking_own_not_changed_priority)) == (((Task*) task_checking_own_not_changed_priority)->originalPriority))
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void sm_take_prio_11()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-11");
	
	ROSA_CreateTask(take_lower_priority_semaphore_and_check_own_priority, task_name, SMALL_STACK_SIZE, PRIORITY_6, &task_checking_own_not_changed_priority);
	ROSA_SemaphoreCreatePrio(&semaphore_lower_priority_than_task, SEMAPHORE_FREE, PRIORITY_4);
	ROSA_StartScheduler();
}

SemaphoreHandle semaphore_priority_high_stack_checking, semaphore_priority_medium_stack_checking, semaphore_priority_low_stack_checking;
TaskHandle task_checking_own_priorities;

void take_a_lot_of_semaphores_and_check_priorities()
{
	Task* task = (Task*) task_checking_own_priorities;
	
	if (!isEmptyStack(task->temporaryPriority))
	{
		send_fail();
	}
	
	ROSA_SemaphoreTake(semaphore_priority_low_stack_checking, 0);
	if (getPriority(task) != PRIORITY_4)
	{
		send_fail();
	}
	if (task->temporaryPriority->data[0] != PRIORITY_4)
	{
		send_fail();
	}
	
	ROSA_SemaphoreTake(semaphore_priority_high_stack_checking, 0);
	if (getPriority(task) != PRIORITY_6)
	{
		send_fail();
	}
	if (task->temporaryPriority->data[0] != PRIORITY_4)
	{
		send_fail();
	}
	if (task->temporaryPriority->data[1] != PRIORITY_6)
	{
		send_fail();
	}
	
	ROSA_SemaphoreTake(semaphore_priority_medium_stack_checking, 0);
	if (getPriority(task) != PRIORITY_6)
	{
		send_fail();
	}
	if (task->temporaryPriority->data[0] != PRIORITY_4)
	{
		send_fail();
	}
	if (task->temporaryPriority->data[1] != PRIORITY_6)
	{
		send_fail();
	}
	if (task->temporaryPriority->data[2] != PRIORITY_6)
	{
		send_fail();
	}
	
	ROSA_SemaphoreGive(semaphore_priority_medium_stack_checking);
	if (getPriority(task) != PRIORITY_6)
	{
		send_fail();
	}
	if (task->temporaryPriority->data[0] != PRIORITY_4)
	{
		send_fail();
	}
	if (task->temporaryPriority->data[1] != PRIORITY_6)
	{
		send_fail();
	}
	
	ROSA_SemaphoreGive(semaphore_priority_high_stack_checking);
	if (getPriority(task) != PRIORITY_4)
	{
		send_fail();
	}
	if (task->temporaryPriority->data[0] != PRIORITY_4)
	{
		send_fail();
	}
	
	ROSA_SemaphoreGive(semaphore_priority_low_stack_checking);
	if (getPriority(task) != PRIORITY_4)
	{
		send_fail();
	}
	if (!isEmptyStack(task->temporaryPriority))
	{
		send_fail();
	}
	send_success();
}

void sm_take_prio_12()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-12");
	
	ROSA_CreateTask(take_a_lot_of_semaphores_and_check_priorities, task_name, SMALL_STACK_SIZE, PRIORITY_4, &task_checking_own_priorities);
	ROSA_SemaphoreCreatePrio(&semaphore_priority_high_stack_checking, SEMAPHORE_FREE, PRIORITY_6);
	ROSA_SemaphoreCreatePrio(&semaphore_priority_medium_stack_checking, SEMAPHORE_FREE, PRIORITY_5);
	ROSA_SemaphoreCreatePrio(&semaphore_priority_low_stack_checking, SEMAPHORE_FREE, PRIORITY_3);
	
	ROSA_StartScheduler();
}

SemaphoreHandle semaphore_delayed_with;
int did_task_without_semaphore_run = 0;

void delays_without_semaphore()
{
	ROSA_DelayAbsolute(0, 30);
	did_task_without_semaphore_run = 1;
	send_fail();
}

void delays_with_semaphore()
{
	ROSA_SemaphoreTake(semaphore_delayed_with, 10000);
	ROSA_DelayAbsolute(0, 50);
	if (did_task_without_semaphore_run == 0)
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void sm_take_prio_13()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-13");
	
	ROSA_CreateTask(delays_without_semaphore, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(delays_with_semaphore, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_SemaphoreCreatePrio(&semaphore_delayed_with, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_StartScheduler();
}

SemaphoreHandle semaphore_priority_inversion_checking;

void high_priority_priority_inversion()
{
	ROSA_DelayAbsolute(0, 30);
	ROSA_SemaphoreTake(semaphore_priority_inversion_checking, 0);
	if (ROSA_TimerTickCount() < 100)
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void medium_priority_priority_inversion()
{
	ROSA_DelayAbsolute(0, 35);
	while (1);
}

void low_priority_priority_inversion()
{
	ROSA_SemaphoreTake(semaphore_priority_inversion_checking, 0);
	busy_wait(50);
	ROSA_SemaphoreGive(semaphore_priority_inversion_checking);
}

void sm_take_prio_14()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-14");
	
	ROSA_CreateTask(high_priority_priority_inversion, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(medium_priority_priority_inversion, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_CreateTask(low_priority_priority_inversion, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreatePrio(&semaphore_priority_inversion_checking, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_StartScheduler();
}

SemaphoreHandle semaphore_deadlock_checking_1, semaphore_deadlock_checking_2;

void high_priority_deadlock()
{
	ROSA_DelayAbsolute(0, 50);
	ROSA_SemaphoreTake(semaphore_deadlock_checking_1, 0);
	ROSA_SemaphoreTake(semaphore_deadlock_checking_2, 0);
	send_success();
}

void low_priority_deadlock()
{
	ROSA_SemaphoreTake(semaphore_deadlock_checking_1, 0);
	busy_wait(100);
	ROSA_SemaphoreTake(semaphore_deadlock_checking_2, 0);
	busy_wait(50);
	ROSA_SemaphoreGive(semaphore_deadlock_checking_2);
	ROSA_SemaphoreGive(semaphore_deadlock_checking_1);
	BLOCK
}

void sm_take_prio_15()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-TAKE-PRIO-15");
	
	ROSA_CreateTask(high_priority_deadlock, task_name, SMALL_STACK_SIZE, 2, NULL);
	ROSA_CreateTask(low_priority_deadlock, task_name, SMALL_STACK_SIZE, 1, NULL);
	ROSA_SemaphoreCreatePrio(&semaphore_deadlock_checking_1, SEMAPHORE_FREE, 2);
	ROSA_SemaphoreCreatePrio(&semaphore_deadlock_checking_2, SEMAPHORE_FREE, 2);
	
	ROSA_StartScheduler();
}

SemaphoreHandle give_me_while_i_am_occupied_binary;

void give_occupied_binary_semaphore_and_check_its_state()
{
	ROSA_SemaphoreGive(give_me_while_i_am_occupied_binary);
	
	if (((Semaphore *) give_me_while_i_am_occupied_binary)->state == SEMAPHORE_FREE)
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void sm_give_binary_01()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-BINARY-01");
	
	ROSA_CreateTask(give_occupied_binary_semaphore_and_check_its_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreateBinary(&give_me_while_i_am_occupied_binary, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
}

SemaphoreHandle give_me_while_i_am_free_binary;

void give_free_binary_semaphore_and_check_its_state()
{
	ROSA_SemaphoreGive(give_me_while_i_am_free_binary);
	
	if (((Semaphore *) give_me_while_i_am_free_binary)->state == SEMAPHORE_FREE)
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void sm_give_binary_02()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-BINARY-02");
	
	ROSA_CreateTask(give_free_binary_semaphore_and_check_its_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreateBinary(&give_me_while_i_am_free_binary, SEMAPHORE_FREE);
	
	ROSA_StartScheduler();
}

SemaphoreHandle take_me_and_give_me_binary;

void take_and_give_binary_semaphore_and_check_queue_state()
{
	ROSA_SemaphoreTake(take_me_and_give_me_binary, 10);
	ROSA_SemaphoreGive(take_me_and_give_me_binary);
	
	BlockedPriorityQueue blockedQueue = fetchBLOCKEDqueue();
	if (isEmptyBlockedPriorityQueue(&blockedQueue))
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void sm_give_binary_03()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-BINARY-03");
	
	ROSA_CreateTask(take_and_give_binary_semaphore_and_check_queue_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreateBinary(&take_me_and_give_me_binary, SEMAPHORE_FREE);
	
	ROSA_StartScheduler();
}

SemaphoreHandle taken_and_given_by_one_and_taken_by_other_binary;

void take_and_delay_and_give_and_delay_binary()
{
	ROSA_SemaphoreTake(taken_and_given_by_one_and_taken_by_other_binary, 0);
	ROSA_DelayAbsolute(0, 50);
	ROSA_SemaphoreGive(taken_and_given_by_one_and_taken_by_other_binary);
	ROSA_DelayAbsolute(0, 100000);
}

void take_binary_semaphore_taken_and_released_by_another()
{
	ROSA_SemaphoreTake(taken_and_given_by_one_and_taken_by_other_binary, 1000);
	if (ROSA_TimerTickCount() < 50)
	{
		send_fail();
	}
	else
	{
		send_success();
	}
}

void sm_give_binary_04()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-BINARY-04");
	
	ROSA_CreateTask(take_and_delay_and_give_and_delay_binary, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(take_binary_semaphore_taken_and_released_by_another, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_SemaphoreCreateBinary(&taken_and_given_by_one_and_taken_by_other_binary, SEMAPHORE_FREE);
	
	ROSA_StartScheduler();
}

SemaphoreHandle wanted_to_be_taked_by_everyone_binary;

void take_binary_semaphore_with_high_prio()
{
	ROSA_DelayAbsolute(0, 30);
	ROSA_SemaphoreTake(wanted_to_be_taked_by_everyone_binary, 100000);
	send_success();
}

void take_binary_semaphore_with_medium_prio()
{
	ROSA_SemaphoreTake(wanted_to_be_taked_by_everyone_binary, 100000);
	send_fail();
}

void take_binary_semaphore_with_low_prio()
{
	ROSA_DelayAbsolute(0, 40);
	ROSA_SemaphoreTake(wanted_to_be_taked_by_everyone_binary, 100000);
	send_fail();
}

void release_binary_semaphore()
{
	ROSA_DelayAbsolute(0, 50);
	ROSA_SemaphoreGive(wanted_to_be_taked_by_everyone_binary);
}

void sm_give_binary_05()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-BINARY-05");
	
	ROSA_CreateTask(take_binary_semaphore_with_high_prio, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(take_binary_semaphore_with_medium_prio, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_CreateTask(take_binary_semaphore_with_low_prio, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_CreateTask(release_binary_semaphore, task_name, SMALL_STACK_SIZE, PRIORITY_2, NULL);
	ROSA_SemaphoreCreateBinary(&wanted_to_be_taked_by_everyone_binary, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
}

SemaphoreHandle give_me_while_i_am_occupied_priority;

void give_occupied_priority_semaphore_and_check_its_state()
{
	ROSA_SemaphoreTake(give_me_while_i_am_occupied_priority, 100000);
	ROSA_SemaphoreGive(give_me_while_i_am_occupied_priority);
	
	if (((Semaphore *) give_me_while_i_am_occupied_priority)->state == SEMAPHORE_FREE)
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void sm_give_priority_01()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-PRIORITY-01");
	
	ROSA_CreateTask(give_occupied_priority_semaphore_and_check_its_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreatePrio(&give_me_while_i_am_occupied_priority, SEMAPHORE_FREE, PRIORITY_3);
	
	ROSA_StartScheduler();
}

SemaphoreHandle give_me_while_i_am_free_priority;

void give_free_priority_semaphore_and_check_its_state()
{
	ROSA_SemaphoreGive(give_me_while_i_am_free_priority);
	
	if (((Semaphore *) give_me_while_i_am_free_priority)->state == SEMAPHORE_FREE)
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void sm_give_priority_02()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-PRIORITY-02");
	
	ROSA_CreateTask(give_free_priority_semaphore_and_check_its_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreatePrio(&give_me_while_i_am_free_priority, SEMAPHORE_FREE, PRIORITY_3);
	
	ROSA_StartScheduler();
}

SemaphoreHandle take_me_and_give_me_priority;

void take_and_give_priority_semaphore_and_check_queue_state()
{
	ROSA_SemaphoreTake(take_me_and_give_me_priority, 10);
	ROSA_SemaphoreGive(take_me_and_give_me_priority);
	
	BlockedPriorityQueue blockedQueue = fetchBLOCKEDqueue();
	if (isEmptyBlockedPriorityQueue(&blockedQueue))
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void sm_give_priority_03()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-PRIORITY-03");
	
	ROSA_CreateTask(take_and_give_priority_semaphore_and_check_queue_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreatePrio(&take_me_and_give_me_priority, SEMAPHORE_FREE, PRIORITY_3);
	
	ROSA_StartScheduler();
}

SemaphoreHandle taken_and_given_by_one_and_taken_by_other_priority;

void take_and_delay_and_give_and_delay_priority()
{
	ROSA_SemaphoreTake(taken_and_given_by_one_and_taken_by_other_priority, 0);
	ROSA_DelayAbsolute(0, 50);
	ROSA_SemaphoreGive(taken_and_given_by_one_and_taken_by_other_priority);
	ROSA_DelayAbsolute(0, 100000);
}

void take_priority_semaphore_taken_and_released_by_another()
{
	ROSA_SemaphoreTake(taken_and_given_by_one_and_taken_by_other_priority, 1000);
	if (ROSA_TimerTickCount() < 50)
	{
		send_fail();
	}
	else
	{
		send_success();
	}
}

void sm_give_priority_04()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-PRIORITY-04");
	
	ROSA_CreateTask(take_and_delay_and_give_and_delay_priority, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(take_priority_semaphore_taken_and_released_by_another, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_SemaphoreCreatePrio(&taken_and_given_by_one_and_taken_by_other_priority, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_StartScheduler();
}

SemaphoreHandle wanted_to_be_taked_by_everyone_priority;

void take_priority_semaphore_with_high_prio()
{
	ROSA_DelayAbsolute(0, 30);
	ROSA_SemaphoreTake(wanted_to_be_taked_by_everyone_priority, 100000);
	send_success();
}

void take_priority_semaphore_with_medium_prio()
{
	ROSA_DelayAbsolute(0, 20);
	ROSA_SemaphoreTake(wanted_to_be_taked_by_everyone_priority, 100000);
	send_fail();
}

void take_priority_semaphore_with_low_prio()
{
	ROSA_DelayAbsolute(0, 40);
	ROSA_SemaphoreTake(wanted_to_be_taked_by_everyone_priority, 100000);
	send_fail();
}

void release_priority_semaphore()
{
	ROSA_SemaphoreTake(wanted_to_be_taked_by_everyone_priority, 100000);
	ROSA_DelayAbsolute(0, 50);
	ROSA_SemaphoreGive(wanted_to_be_taked_by_everyone_priority);
}

void sm_give_priority_05()
{
	char task_name[5] = TASK_NAME;
	
	send_id("SM-GIVE-PRIORITY-05");
	
	ROSA_CreateTask(take_priority_semaphore_with_high_prio, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(take_priority_semaphore_with_medium_prio, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_CreateTask(take_priority_semaphore_with_low_prio, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_CreateTask(release_priority_semaphore, task_name, SMALL_STACK_SIZE, PRIORITY_2, NULL);
	ROSA_SemaphoreCreatePrio(&wanted_to_be_taked_by_everyone_priority, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_StartScheduler();
}
