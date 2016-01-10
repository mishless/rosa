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

/*************** TEST SUITE: Semaphore Binary Create ***************/

/*** SM-CREATE-BINARY-01 ***/

void sm_create_binary_01_main()
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

Test sm_create_binary_01 = {
	.id =			"SM-CREATE-BINARY-01",
	.description =	"Check if semaphore structure is populated.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
	.function =		sm_create_binary_01_main};

/*** SM-CREATE-BINARY-02 ***/

void sm_create_binary_02_main()
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

Test sm_create_binary_02 = {
	.id =			"SM-CREATE-BINARY-02",
	.description =	"Check if blocked queue is allocated.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
	.function =		sm_create_binary_02_main};

/*** SM-CREATE-BINARY-05 ***/

void sm_create_binary_05_main()
{
	SemaphoreHandle semaphore_handle;
	
	int return_code = ROSA_SemaphoreCreateBinary(&semaphore_handle, 5);
	
	if(return_code != INVALID_INITIAL_VALUE)
	{
		send_fail();
	}
	
	send_success();
}

Test sm_create_binary_05 = {
	.id =			"SM-CREATE-BINARY-05",
	.description =	"Return code from ROSA_SemaphoreCreateBinary should be INVALID_INITIAL_VALUE when 0 or 1 are not used for initial value.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_binary_05_main};

/*** SM-CREATE-BINARY-06 ***/

void sm_create_binary_06_main()
{
	SemaphoreHandle semaphore_handle;
	int i = 0;
	
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

Test sm_create_binary_06 = {
	.id =			"SM-CREATE-BINARY-06",
	.description =	"Return code from ROSA_SemaphoreCreateBinary should be TOO_MANY_SEMAPHORES when too many semaphores are created, putting 2 as max number of semaphores in the configuration file.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_binary_06_main};

/*** SM-CREATE-BINARY-07 ***/

void sm_create_binary_07_main()
{
	SemaphoreHandle semaphore_handle;
	
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

Test sm_create_binary_07 = {
	.id =			"SM-CREATE-BINARY-07",
	.description =	"Return code from ROSA_SemaphoreCreateBinary should be NOT_ENOUGH_MEMORY when there is no more memory for allocating the semaphore.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_binary_07_main};

/*** SM-CREATE-BINARY-08 ***/

void sm_create_binary_08_main()
{
	SemaphoreHandle semaphore_handle;
	
	int return_code = ROSA_SemaphoreCreateBinary(&semaphore_handle, SEMAPHORE_FREE);
	
	if(return_code != SUCCESS)
	{
		send_fail();
	}
	
	send_success();
}

Test sm_create_binary_08 = {
	.id =			"SM-CREATE-BINARY-08",
	.description =	"Return code from ROSA_SemaphoreCreateBinary should be SUCCESS if the semaphore is created without problems.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_binary_08_main};

/*** SM-CREATE-BINARY-09 ***/

void task_sm_cb_09()
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

void sm_create_binary_09_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_cb_09, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();		
}

Test sm_create_binary_09 = {
	.id =			"SM-CREATE-BINARY-09",
	.description =	"Check if semaphore creation from a task populates the semaphore structure.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_binary_09_main};

/*** SM-CREATE-BINARY-10 ***/

void task_sm_cb_10()
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

void sm_create_binary_10_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("SM-CREATE-BINARY-10");
	
	ROSA_CreateTask(task_sm_cb_10, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_create_binary_10 = {
	.id =			"SM-CREATE-BINARY-10",
	.description =	"Create a binary semaphore from task and check blocked queue.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_binary_10_main};


/*************** TEST SUITE: Semaphore Priority Create ***************/


/*** SM-CREATE-PRIO-01 ***/

void sm_create_prio_01_main()
{	
	SemaphoreHandle semaphore_handle;
	
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

Test sm_create_prio_01 = {
	.id =			"SM-CREATE-PRIO-01",
	.description =	"Check if semaphore structure is populated.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_prio_01_main};

/*** SM-CREATE-PRIO-02 ***/

void sm_create_prio_02_main()
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

Test sm_create_prio_02 = {
	.id =			"SM-CREATE-PRIO-02",
	.description =	"Check if blocked queue is allocated.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_prio_02_main};

/*** SM-CREATE-PRIO-05 ***/

void sm_create_prio_05_main()
{
	SemaphoreHandle semaphore_handle;
	
	int return_code = ROSA_SemaphoreCreatePrio(&semaphore_handle, 5, PRIORITY_5);
	
	if(return_code != INVALID_INITIAL_VALUE)
	{
		send_fail();
	}
	
	send_success();
}

Test sm_create_prio_05 = {
	.id =			"SM-CREATE-PRIO-05",
	.description =	"Return code from ROSA_SemaphoreCreatePrio should be INVALID_INITIAL_VALUE when 0 or 1 are not used for initial value.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_prio_05_main};

/*** SM-CREATE-PRIO-06 ***/

void sm_create_prio_06_main()
{
	SemaphoreHandle semaphore_handle;
	int i = 0;
	
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

Test sm_create_prio_06 = {
	.id =			"SM-CREATE-PRIO-06",
	.description =	"Return code from ROSA_SemaphoreCreatePrio should be TOO_MANY_SEMAPHORES when too many semaphores are created.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_prio_06_main};

/*** SM-CREATE-PRIO-07 ***/

void sm_create_prio_07_main()
{
	SemaphoreHandle semaphore_handle;
	
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

Test sm_create_prio_07 = {
	.id =			"SM-CREATE-PRIO-07",
	.description =	"Return code from ROSA_SemaphoreCreatePrio should be NOT_ENOUGH_MEMORY when there is no more memory for allocating the semaphore.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_prio_07_main};

/*** SM-CREATE-PRIO-08 ***/

void sm_create_prio_08_main()
{
	SemaphoreHandle semaphore_handle;
	
	int return_code = ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, 0);
	
	if(return_code != INVALID_PRIORITY)
	{
		send_fail();
	}
	
	send_success();
}

Test sm_create_prio_08 = {
	.id =			"SM-CREATE-PRIO-08",
	.description =	"Return code from ROSA_SemaphoreCreatePrio should be INVALID_PRIORITY if the semaphore is created with priority 0.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_prio_08_main};

/*** SM-CREATE-PRIO-09 ***/

void sm_create_prio_09_main()
{
	SemaphoreHandle semaphore_handle;
	
	int return_code = ROSA_SemaphoreCreatePrio(&semaphore_handle, SEMAPHORE_FREE, PRIORITY_5);
	
	if(return_code != SUCCESS)
	{
		send_fail();
	}
	
	send_success();
}

Test sm_create_prio_09 = {
	.id =			"SM-CREATE-PRIO-09",
	.description =	"Return code from ROSA_SemaphoreCreatePrio should be SUCCESS if the semaphore is created without problems.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_prio_09_main};

/*** SM-CREATE-PRIO-10 ***/

void task_sm_cp_10()
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

void sm_create_prio_10_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_cp_10, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();		
}

Test sm_create_prio_10 = {
	.id =			"SM-CREATE-PRIO-10",
	.description =	"Check if semaphore creation from a task populates the semaphore structure.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_prio_10_main};

/*** SM-CREATE-PRIO-11 ***/

void task_sm_cp_11()
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

void sm_create_prio_11_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_cp_11, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_create_prio_11 = {
	.id =			"SM-CREATE-PRIO-11",
	.description =	"Create a priority semaphore from task and check blocked queue.",
	.plan =			"Semaphore Manager",
	.suite =		"Create Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_prio_11_main};

/*************** TEST SUITE: Mixed Semaphore Create ***************/

/*** SM-CREATE-MIX-01 ***/

void sm_create_mix_01_main()
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

Test sm_create_mix_01 = {
	.id =			"SM-CREATE-MIX-01",
	.description =	"Creation of two types of semaphores.",
	.plan =			"Semaphore Manager",
	.suite =		"Mixed Create Semaphores",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_mix_01_main};

/*** SM-CREATE-MIX-02 ***/

void task_sm_cm_02()
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

void sm_create_mix_02_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_cm_02, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_create_mix_02 = {
	.id =			"SM-CREATE-MIX-02",
	.description =	"Creation of two types of semaphores from a task.",
	.plan =			"Semaphore Manager",
	.suite =		"Mixed Create Semaphores",
	.type =			TEST_FUNCTIONAL,
.function =		sm_create_mix_02_main};

/*************** TEST SUITE: Delete Semaphore ***************/

/*** SM-DELETE-03 ***/

void task_sm_d_03()
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

void sm_delete_03_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_d_03, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_delete_03 = {
	.id =			"SM-DELETE-03",
	.description =	"Check for memory leaks using stress test.",
	.plan =			"Semaphore Manager",
	.suite =		"Delete Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_delete_03_main};

/*** SM-DELETE-04 ***/

void sm_delete_04_main()
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

Test sm_delete_04 = {
	.id =			"SM-DELETE-04",
	.description =	"Delete a semaphore and check return code.",
	.plan =			"Semaphore Manager",
	.suite =		"Delete Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_delete_04_main};

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

/*************** TEST SUITE: Take Binary Semaphore ***************/

/*** SM-TAKE-BINARY-01 ***/

void task_sm_tb_01()
{
	ROSA_SemaphoreTake(semaphore_handle_t, 0);
	
	if( ((Semaphore*)semaphore_handle_t)->state == SEMAPHORE_OCCUPIED)
	{
		send_success();
	}
	
	send_fail();
}

void sm_take_binary_01_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_FREE);
	
	ROSA_CreateTask(task_sm_tb_01, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_take_binary_01 = {
	.id =			"SM-TAKE-BINARY-01",
	.description =	"Check if it possible to take a binary semaphore.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
	.function =		sm_take_binary_01_main};

/*** SM-TAKE-BINARY-02 ***/

void task_sm_tb_02_h()
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

void sm_take_binary_02_main()
{
	TaskHandle task_handle_high;
	TaskHandle task_handle_low;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_tb_02_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle_high);
	ROSA_CreateTask(task_sm_tb_02_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &task_handle_low);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_take_binary_02 = {
	.id =			"SM-TAKE-BINARY-02",
	.description =	"Check if  an occupied binary semaphore can't be taken.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_binary_02_main};

/*** SM-TAKE-BINARY-03 ***/

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

void sm_take_binary_03_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_tb_03_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tb_03_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_take_binary_03 = {
	.id =			"SM-TAKE-BINARY-03",
	.description =	"Check if a blocked task is in the blocked queue.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_binary_03_main};

/*** SM-TAKE-BINARY-04 ***/

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

void sm_take_binary_04_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_tb_04_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tb_04_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_take_binary_04 = {
	.id =			"SM-TAKE-BINARY-04",
	.description =	"Check if after have taken a semaphore, it is still occupied.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_binary_04_main};

/*** SM-TAKE-BINARY-05 ***/

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

void sm_take_binary_05_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_tb_05_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tb_05_m, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tb_05_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_take_binary_05 = {
	.id =			"SM-TAKE-BINARY-05",
	.description =	"Check if two tasks that take a semaphore both end up in the blocked queue.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_binary_05_main};

/*** SM-TAKE-BINARY-06 ***/

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

void sm_take_binary_06_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_tb_06_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tb_06_m, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tb_06_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_take_binary_06 = {
	.id =			"SM-TAKE-BINARY-06",
	.description =	"Check if tasks are sorted in blocked queue.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_binary_06_main};

/*** SM-TAKE-BINARY-07 ***/

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

void sm_take_binary_07_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(task_sm_tb_07, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	
	ROSA_SemaphoreCreateBinary(&semaphore_handle_t, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
	send_fail();
}

Test sm_take_binary_07 = {
	.id =			"SM-TAKE-BINARY-07",
	.description =	"Check if taking a semaphore with timeout 0 doesn't block the task.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_binary_07_main};

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

/*************** TEST SUITE: Take Priority Semaphore ***************/

/*** SM-TAKE-PRIO-01 ***/

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

void sm_take_prio_01_main()
{	
	char task_name[5] = TASK_NAME;
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_01, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	
	ROSA_Start();
	send_fail();
}

Test sm_take_prio_01 = {
	.id =			"SM-TAKE-PRIO-01",
	.description =	"Check if it possible to take a priority semaphore.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_01_main};

/*** SM-TAKE-PRIO-02 ***/

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

void sm_take_prio_02_main()
{	
	char task_name[5] = TASK_NAME;
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_02_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_02_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}

Test sm_take_prio_02 = {
	.id =			"SM-TAKE-PRIO-02",
	.description =	"Check if  an occupied priority semaphore can't be taken.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_02_main};

/*** SM-TAKE-PRIO-02 ***/

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

void sm_take_prio_03_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_03_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_03_m, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tp_03_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}

Test sm_take_prio_03 = {
	.id =			"SM-TAKE-PRIO-03",
	.description =	"Check if  a blocked task is in the blocked queue.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_03_main};

/*** SM-TAKE-PRIO-04 ***/

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

void sm_take_prio_04_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_04_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_04_l, task_name, SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}

Test sm_take_prio_04 = {
	.id =			"SM-TAKE-PRIO-04",
	.description =	"Check if  an occupied semaphore is still occupied after taking it.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_04_main};

/*** SM-TAKE-PRIO-05 ***/

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
	ledOn(61);
	ROSA_SemaphoreTake(semaphore_handle_t, MAX_TIMEOUT);
}

void task_sm_tp_05_ll()
{
	ledOn(62);
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

void sm_take_prio_05_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_6);
	
	ROSA_CreateTask(task_sm_tp_05_h, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_6, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_05_m, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_5, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tp_05_l, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	ROSA_CreateTask(task_sm_tp_05_ll, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_3, &taskLowLowPriority);
	
	ROSA_Start();
	send_fail();
}

Test sm_take_prio_05 = {
	.id =			"SM-TAKE-PRIO-05",
	.description =	"Check if two task taking a semaphore both end up in the blocked queue.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_05_main};

/*** SM-TAKE-PRIO-06 ***/

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

void sm_take_prio_06_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_06_h, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_06_m, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_4, &taskMiddlePriority);
	ROSA_CreateTask(task_sm_tp_06_l, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_3, &taskLowPriority);
	ROSA_CreateTask(task_sm_tp_06_ll, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_2, &taskLowLowPriority);
	
	ROSA_Start();
	send_fail();
}

Test sm_take_prio_06 = {
	.id =			"SM-TAKE-PRIO-06",
	.description =	"Check if tasks are sorted in the blocked queue.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_06_main};

/*** SM-TAKE-PRIO-07 ***/

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

void sm_take_prio_07_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_SemaphoreCreatePrio(&semaphore_handle_t, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_CreateTask(task_sm_tp_07_h, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskHighPriority);
	ROSA_CreateTask(task_sm_tp_07_l, task_name, SMALL_STACK_SIZE, PRIORITY_4, &taskLowPriority);
	
	ROSA_Start();
	send_fail();
}

Test sm_take_prio_07 = {
	.id =			"SM-TAKE-PRIO-07",
	.description =	"Check if  taking a semaphore with timeout 0 doesn't block the task.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_07_main};

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

void sm_take_prio_10_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(take_higher_priority_semaphore_and_check_own_priority, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_checking_own_changed_priority);
	ROSA_SemaphoreCreatePrio(&semaphore_higher_priority_than_task, SEMAPHORE_FREE, PRIORITY_6);
	ROSA_StartScheduler();
}

Test sm_take_prio_10 = {
	.id =			"SM-TAKE-PRIO-10",
	.description =	"Check if task with lower priority inherits the priority of the semaphore that is higher than the task.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_10_main};

/*** SM-TAKE-PRIO-11 ***/


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

void sm_take_prio_11_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(take_lower_priority_semaphore_and_check_own_priority, task_name, SMALL_STACK_SIZE, PRIORITY_6, &task_checking_own_not_changed_priority);
	ROSA_SemaphoreCreatePrio(&semaphore_lower_priority_than_task, SEMAPHORE_FREE, PRIORITY_4);
	ROSA_StartScheduler();
}

Test sm_take_prio_11 = {
	.id =			"SM-TAKE-PRIO-11",
	.description =	"Check if task with higher priority doesn't inherit the priority of the semaphore that is lower than the task.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_11_main};

/*** SM-TAKE-PRIO-12 ***/

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

void sm_take_prio_12_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(take_a_lot_of_semaphores_and_check_priorities, task_name, SMALL_STACK_SIZE, PRIORITY_4, &task_checking_own_priorities);
	ROSA_SemaphoreCreatePrio(&semaphore_priority_high_stack_checking, SEMAPHORE_FREE, PRIORITY_6);
	ROSA_SemaphoreCreatePrio(&semaphore_priority_medium_stack_checking, SEMAPHORE_FREE, PRIORITY_5);
	ROSA_SemaphoreCreatePrio(&semaphore_priority_low_stack_checking, SEMAPHORE_FREE, PRIORITY_3);
	
	ROSA_StartScheduler();
}

Test sm_take_prio_12 = {
	.id =			"SM-TAKE-PRIO-12",
	.description =	"Check the temporary priority of a task that takes four different semaphores.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_12_main};

/*** SM-TAKE-PRIO-13 ***/

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

void sm_take_prio_13_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(delays_without_semaphore, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(delays_with_semaphore, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_SemaphoreCreatePrio(&semaphore_delayed_with, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_StartScheduler();
}

Test sm_take_prio_13 = {
	.id =			"SM-TAKE-PRIO-13",
	.description =	"Higher priority task should not execute if the semaphore with higher priority is taken by a lower priority task.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_13_main};

/*** SM-TAKE-PRIO-14 ***/

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

void sm_take_prio_14_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(high_priority_priority_inversion, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(medium_priority_priority_inversion, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_CreateTask(low_priority_priority_inversion, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreatePrio(&semaphore_priority_inversion_checking, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_StartScheduler();
}

Test sm_take_prio_14 = {
	.id =			"SM-TAKE-PRIO-14",
	.description =	"Check that priority inversion doesn't occur.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_14_main};

/*** SM-TAKE-PRIO-15 ***/

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

void sm_take_prio_15_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(high_priority_deadlock, task_name, SMALL_STACK_SIZE, 2, NULL);
	ROSA_CreateTask(low_priority_deadlock, task_name, SMALL_STACK_SIZE, 1, NULL);
	ROSA_SemaphoreCreatePrio(&semaphore_deadlock_checking_1, SEMAPHORE_FREE, 2);
	ROSA_SemaphoreCreatePrio(&semaphore_deadlock_checking_2, SEMAPHORE_FREE, 2);
	
	ROSA_StartScheduler();
}

Test sm_take_prio_15 = {
	.id =			"SM-TAKE-PRIO-15",
	.description =	"Check that deadlocks don't occur.",
	.plan =			"Semaphore Manager",
	.suite =		"Take Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_take_prio_15_main};

/*************** TEST SUITE: Give Binary Semaphore ***************/

/*** SM-GIVE-BINARY-01 ***/

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

void sm_give_binary_01_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(give_occupied_binary_semaphore_and_check_its_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreateBinary(&give_me_while_i_am_occupied_binary, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
}

Test sm_give_binary_01 = {
	.id =			"SM-GIVE-BINARY-01",
	.description =	"Check if the semaphore state is free after being released (Before the semaphore should be occupied).",
	.plan =			"Semaphore Manager",
	.suite =		"Give Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_binary_01_main};

/*** SM-GIVE-BINARY-02 ***/

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

void sm_give_binary_02_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(give_free_binary_semaphore_and_check_its_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreateBinary(&give_me_while_i_am_free_binary, SEMAPHORE_FREE);
	
	ROSA_StartScheduler();
}

Test sm_give_binary_02 = {
	.id =			"SM-GIVE-BINARY-02",
	.description =	"Check if the semaphore state is free after being released.",
	.plan =			"Semaphore Manager",
	.suite =		"Give Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_binary_02_main};

/*** SM-GIVE-BINARY-03 ***/

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

void sm_give_binary_03_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(take_and_give_binary_semaphore_and_check_queue_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreateBinary(&take_me_and_give_me_binary, SEMAPHORE_FREE);
	
	ROSA_StartScheduler();
}

Test sm_give_binary_03 = {
	.id =			"SM-GIVE-BINARY-03",
	.description =	"Semaphore queue and delayed semaphore queue should be empty if a task takes a semaphore and after that it release it.",
	.plan =			"Semaphore Manager",
	.suite =		"Give Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_binary_03_main};

/*** SM-GIVE-BINARY-04 ***/

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

void sm_give_binary_04_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(take_and_delay_and_give_and_delay_binary, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(take_binary_semaphore_taken_and_released_by_another, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_SemaphoreCreateBinary(&taken_and_given_by_one_and_taken_by_other_binary, SEMAPHORE_FREE);
	
	ROSA_StartScheduler();
}

Test sm_give_binary_04 = {
	.id =			"SM-GIVE-BINARY-04",
	.description =	"A task should be able to obtain a semaphore after it has been released by another task.",
	.plan =			"Semaphore Manager",
	.suite =		"Give Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_binary_04_main};

/*** SM-GIVE-BINARY-05 ***/

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

void sm_give_binary_05_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(take_binary_semaphore_with_high_prio, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(take_binary_semaphore_with_medium_prio, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_CreateTask(take_binary_semaphore_with_low_prio, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_CreateTask(release_binary_semaphore, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_2, NULL);
	ROSA_SemaphoreCreateBinary(&wanted_to_be_taked_by_everyone_binary, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
}

Test sm_give_binary_05 = {
	.id =			"SM-GIVE-BINARY-05",
	.description =	"Check if the correct task is executing after it has been blocked by a semaphore.",
	.plan =			"Semaphore Manager",
	.suite =		"Give Binary Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_binary_05_main};

/*************** TEST SUITE: Give Priority Semaphore ***************/

/*** SM-GIVE-PRIORITY-01 ***/

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

void sm_give_priority_01_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(give_occupied_priority_semaphore_and_check_its_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreatePrio(&give_me_while_i_am_occupied_priority, SEMAPHORE_FREE, PRIORITY_3);
	
	ROSA_StartScheduler();
}

Test sm_give_priority_01 = {
	.id =			"SM-GIVE-PRIORITY-01",
	.description =	"Check if the semaphore state is free after being released (Before the semaphore should be occupied).",
	.plan =			"Semaphore Manager",
	.suite =		"Give Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_priority_01_main};

/*** SM-GIVE-PRIORITY-02 ***/

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

void sm_give_priority_02_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(give_free_priority_semaphore_and_check_its_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreatePrio(&give_me_while_i_am_free_priority, SEMAPHORE_FREE, PRIORITY_3);
	
	ROSA_StartScheduler();
}

Test sm_give_priority_02 = {
	.id =			"SM-GIVE-PRIORITY-02",
	.description =	"Check if the semaphore state is free after being released.",
	.plan =			"Semaphore Manager",
	.suite =		"Give Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_priority_02_main};

/*** SM-GIVE-PRIORITY-03 ***/

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

void sm_give_priority_03_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(take_and_give_priority_semaphore_and_check_queue_state, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_SemaphoreCreatePrio(&take_me_and_give_me_priority, SEMAPHORE_FREE, PRIORITY_3);
	
	ROSA_StartScheduler();
}

Test sm_give_priority_03 = {
	.id =			"SM-GIVE-PRIORITY-03",
	.description =	"Semaphore queue and delayed semaphore queue should be empty if a task takes a semaphore and after that it release it.",
	.plan =			"Semaphore Manager",
	.suite =		"Give Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_priority_03_main};

/*** SM-GIVE-PRIORITY-04 ***/

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

void sm_give_priority_04_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(take_and_delay_and_give_and_delay_priority, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(take_priority_semaphore_taken_and_released_by_another, task_name, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_SemaphoreCreatePrio(&taken_and_given_by_one_and_taken_by_other_priority, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_StartScheduler();
}

Test sm_give_priority_04 = {
	.id =			"SM-GIVE-PRIORITY-04",
	.description =	"A task should be able to obtain a semaphore after it has been released by another task.",
	.plan =			"Semaphore Manager",
	.suite =		"Give Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_priority_04_main};

/*** SM-GIVE-PRIORITY-05 ***/

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

void sm_give_priority_05_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(take_priority_semaphore_with_high_prio, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(take_priority_semaphore_with_medium_prio, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_CreateTask(take_priority_semaphore_with_low_prio, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_CreateTask(release_priority_semaphore, task_name, SUPER_SMALL_STACK_SIZE, PRIORITY_2, NULL);
	ROSA_SemaphoreCreatePrio(&wanted_to_be_taked_by_everyone_priority, SEMAPHORE_FREE, PRIORITY_5);
	
	ROSA_StartScheduler();
}

Test sm_give_priority_05 = {
	.id =			"SM-GIVE-PRIORITY-05",
	.description =	"Check if the correct task is executing after it has been blocked by a semaphore.",
	.plan =			"Semaphore Manager",
	.suite =		"Give Priority Semaphore",
	.type =			TEST_FUNCTIONAL,
.function =		sm_give_priority_05_main};