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

#define TASK_NAME "test"
#define SMALL_STACK_SIZE 1000
#define PRIORITY_5 5
#define PRIORITY_4 4


void sm_create_binary_01()
{
	/*ROSA_SemaphoreCreateBinary should allocate and populate the semaphore structure 
	  correctly and assign it to the provided handle.*/
	
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE_BINARY-01");
	
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
	
	send_id("SM-CREATE_BINARY-02");
	
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
	
	send_id("SM-CREATE_BINARY-05");
	
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
	
	send_id("SM-CREATE_BINARY-06");
	
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
	
	send_id("SM-CREATE_BINARY-07");
	
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
	
	send_id("SM-CREATE_BINARY-08");
	
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
	char task_name[4] = TASK_NAME;
	
	send_id("SM-CREATE_BINARY-09");
	
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
	char task_name[4] = TASK_NAME;
	
	send_id("SM-CREATE_BINARY-10");
	
	ROSA_CreateTask(task_sm_cb_10, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

void sm_create_prio_01()
{	
	SemaphoreHandle semaphore_handle;
	
	send_id("SM-CREATE_PRIO-01");
	
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
	
	send_id("SM-CREATE_PRIO-02");
	
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
	
	send_id("SM-CREATE_PRIO-05");
	
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
	
	send_id("SM-CREATE_PRIO-06");
	
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
	
	send_id("SM-CREATE_PRIO-07");
	
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
	
	send_id("SM-CREATE_PRIO-08");
	
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
	
	send_id("SM-CREATE_PRIO-09");
	
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

void sm_create_prio_10()
{
	TaskHandle task_handle;
	char task_name[4] = TASK_NAME;
	
	send_id("SM-CREATE_PRIO-10");
	
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
	char task_name[4] = TASK_NAME;
	
	send_id("SM-CREATE_PRIO-11");
	
	ROSA_CreateTask(task_sm_cp_11, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

void sm_create_mix_01()
{
	SemaphoreHandle semaphore_handle_binary;
	SemaphoreHandle semaphore_handle_prio;
	
	send_id("SM-CREATE_MIX-01");
	
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
	
	if( ((Semaphore*)semaphore_handle_prio)->id != 0)
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
	
	if( ((Semaphore*)semaphore_handle_prio)->id != 0)
	{
		send_fail();
	}
	
	send_success();
}

void sm_create_mix_02()
{
	TaskHandle task_handle;
	char task_name[4] = TASK_NAME;
	
	send_id("SM-CREATE_MIX-02");
	
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
		if(ROSA_SemaphoreDelete(&semaphore_handle) != SUCCESS)
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
	char task_name[4] = TASK_NAME;
	
	send_id("SM-CREATE_MIX-02");
	
	ROSA_CreateTask(task_sm_d_03, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

void sm_delete_04()
{
	send_id("SM-CREATE_MIX-04");
	
	SemaphoreHandle semaphore_handle;
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	if(ROSA_SemaphoreDelete(&semaphore_handle) != SUCCESS)
	{
		send_fail();
	}

	send_success();	
}

void sm_delete_05()
{
	send_id("SM-CREATE_MIX-05");
	
	SemaphoreHandle semaphore_handle;
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	if(ROSA_SemaphoreDelete((void *) 42) != SEMAPHORE_NOT_FOUND)
	{
		send_fail();
	}

	send_success();
}