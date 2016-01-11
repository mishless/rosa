/*
 * task_manager_tests.c
 *
 * Created: 16.12.2015. 7:55:47 PM
 *  Author: Vukan
 */ 

#include <avr32/io.h>
#include "task_manager_tests.h"
#include "helper_functions.h"
#include "rosa_api_call.h"
#include "rosa_task_private.h"
#include "rosa_ker.h"
#include "tests.h"
#include "rosa_queue_manager.h"
#include "priority_queue.h"

#define TEST_PTR (void*) 42
#define TASK_NAME "test"
#define PRIORITY_5 5
#define PRIORITY_4 4

#define TEST_PERIOD 25
#define TEST_DEADLINE 20

const char plan_tm[] =				"Task Manager";
const char suite_create_task[] =	"Create Task";

/*************** TEST SUITE: Task Create ***************/

/*** TM-CREATE-01 ***/
	
void tm_create_01_main()
{	
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	if( ((Task*)task_handle)->t->staddr != TEST_PTR)
	{		
		send_fail();
		BLOCK;
	}
	if( compare_strings(task_name, ((Task*)task_handle)->t->id, 4) != 0)
		send_fail();
	if( ((Task*)task_handle)->t->datasize != SMALL_STACK_SIZE)
		send_fail();
	if( ((Task*)task_handle)->originalPriority != PRIORITY_5)		
		send_fail();
		
	send_success();
}

Test tm_create_01 = {
	.id =			"TM-CREATE-01",
	.description =	"ROSA_CreateTask should allocate and populate the task structure correctly and assign it to the provided handle.",
	.plan =			"Task Manager",
	.suite =		"Create Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_01_main};

/*** TM-CREATE-02 ***/

void tm_create_02_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	PriorityQueue ready_queue;
	
	ready_queue = fetchREADYqueue();
	
	/*Check queue size*/
	if(ready_queue.size != 1)
		send_fail();
	if( ( *(ready_queue.data[0]) ).task != (Task*) task_handle )
		send_fail();
		
	send_success();
}

Test tm_create_02 = {
	.id =			"TM-CREATE-02",
	.description =	"ROSA_CreateTask should insert the newly created task in the READY queue.",
	.plan =			"Task Manager",
	.suite =		"Create Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_02_main};

/*** TM-CREATE-03 ***/

void tm_create_03_main()
{
	TaskHandle taskHighPriority, taskMediumPriority, taskLowPriority;
	char task_name[5] = TASK_NAME;
	PriorityQueue ready_queue;
	
	const unsigned int priorityHigh = 5;
	const unsigned int priorityMedium = 4;
	const unsigned int priorityLow = 3;
	
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, priorityMedium, &taskMediumPriority);
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, priorityLow, &taskLowPriority);
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, priorityHigh, &taskHighPriority);
	
	ready_queue = fetchREADYqueue();
	
	if(ready_queue.size != 3)
		send_fail();
	if ((*(ready_queue.data[0]) ).task != (Task*) taskHighPriority)
		send_fail();
	if ((*(ready_queue.data[1]) ).task != (Task*) taskLowPriority)
		send_fail();
	if ((*(ready_queue.data[2]) ).task != (Task*) taskMediumPriority)
		send_fail();
		
	send_success();
}

Test tm_create_03 = {
	.id =			"TM-CREATE-03",
	.description =	"Multiple calls to ROSA_CreateTask should populate the READY queue in the correct way with different task priorities.",
	.plan =			"Task Manager",
	.suite =		"Create Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_03_main};

/*** TM-CREATE-04 ***/

void tm_create_04_main()
{
	TaskHandle task1, task2, task3;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task1);
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task2);
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task3);
	
	PriorityQueue ready_queue = fetchREADYqueue();
	
	if(ready_queue.size != 3)
		send_fail();
	if ((*(ready_queue.data[0]) ).task != (Task*) task1)
		send_fail();
	if ((*(ready_queue.data[1]) ).task != (Task*) task2)
		send_fail();
	if ((*(ready_queue.data[2]) ).task != (Task*) task3)
		send_fail();
	
	send_success();
}

Test tm_create_04 = {
	.id =			"TM-CREATE-04",
	.description =	"Multiple calls to ROSA_CreateTask should populate the READY queue in the correct way with same task priorities.",
	.plan =			"Task Manager",
	.suite =		"Create Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_04_main};

/*** TM-CREATE-05 ***/

void tm_create_05_main()
{
	char task_name[5] = TASK_NAME;
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	if (return_code != SUCCESS)
		send_fail();
		
	send_success();
}

Test tm_create_05 = {
	.id =			"TM-CREATE-05",
	.description =	"Return code from ROSA_CreateTask should be SUCCESS if parameters are valid.",
	.plan =			"Task Manager",
	.suite =		"Create Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_05_main};

/*** TM-CREATE-06 ***/

void tm_create_06_main()
{
	char task_name[5] = TASK_NAME;
	const int invalid_priority = 0;
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, invalid_priority, NULL);
	
	if (return_code != INVALID_PRIORITY)
		send_fail();
	
	send_success();
}

Test tm_create_06 = {
	.id =			"TM-CREATE-06",
	.description =	"Return code from ROSA_CreateTask should be INVALID_PRIORITY when 0 is used for priority.",
	.plan =			"Task Manager",
	.suite =		"Create Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_06_main};

/*Not used anymore, since every stack size is valid
void tm_create_07_main()
{
	char task_name[5] = TASK_NAME;
	const int invalid_stack_size = 10;
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, invalid_stack_size, PRIORITY_5, NULL);
	
	if (return_code != INVALID_STACK_SIZE)
	send_fail();
	
	send_success();
}
*/

/*** TM-CREATE-08 ***/

void tm_create_08_main()
{
	char invalid_name[7] = "invalid";
	
	int return_code = ROSA_CreateTask(TEST_PTR, invalid_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	if (return_code != INVALID_NAME)
		send_fail();
	
	send_success();
}

Test tm_create_08 = {
	.id =			"TM-CREATE-08",
	.description =	"Return code from ROSA_CreateTask should be TOO_MANY_TASKS when it 3 tasks are created.",
	.plan =			"Task Manager",
	.suite =		"Create Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_08_main};

/*** TM-CREATE-09 ***/

void tm_create_09_main()
{
	char task_name[5] = TASK_NAME;
	int i;
	
	for (i = 0; i < MAX_NUMBER_TASKS; i++)
	{
		ROSA_CreateTask(TEST_PTR, task_name, 100, PRIORITY_5, NULL);
	}
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, 100, PRIORITY_5, NULL);
	
	if (return_code != TOO_MANY_TASKS)
		send_fail();
	
	send_success();
}

Test tm_create_09 = {
	.id =			"TM-CREATE-09",
	.description =	"Return code from ROSA_CreateTask should be TOO_MANY_TASKS when the system is out of memory.",
	.plan =			"Task Manager",
	.suite =		"Create Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_create_09_main};

/*** TM-CREATE-10 ***/

void tm_create_10_main()
{
	char task_name[5] = TASK_NAME;
	
	int enough_memory;
	do
	{
		enough_memory = (int) malloc(1);
	} while (enough_memory);
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	if (return_code != NOT_ENOUGH_MEMORY)
		send_fail();
	
	send_success();
}

Test tm_create_10 = {
	.id =			"TM-CREATE-10",
	.description =	"ROSA_CreateCyclicTask should allocate and populate the task structure correctly and assign it to the provided handle.",
	.plan =			"Task Manager",
	.suite =		"Create Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_create_10_main};


/*************** TEST SUITE: Task Create Cyclic ***************/

/*** TM-CREATE-CYCLIC-01 ***/

void tm_create_cyclic_01_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task_handle);
	
	if( ((Task*)task_handle)->t->staddr != TEST_PTR)
	{		
		send_fail();
		BLOCK;
	}
	if( compare_strings(task_name, ((Task*)task_handle)->t->id, 4) != 0)
		send_fail();
	if( ((Task*)task_handle)->t->datasize != SMALL_STACK_SIZE)
		send_fail();
	if( ((Task*)task_handle)->originalPriority != PRIORITY_5)		
		send_fail();
	if( ((Task*)task_handle)->deadline != TEST_DEADLINE)
		send_fail();
	if( ((Task*)task_handle)->period != TEST_PERIOD)
		send_fail();
		
	send_success();
}

Test tm_create_cyclic_01 = {
	.id =			"TM-CREATE-CYCLIC-01",
	.description =	"ROSA_CreateCyclicTask should allocate and populate the task structure correctly and assign it to the provided handle.",
	.plan =			"Task Manager",
	.suite =		"Create Cyclic Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_cyclic_01_main};

/*** TM-CREATE-CYCLIC-02 ***/

void tm_create_cyclic_02_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task_handle);
	PriorityQueue ready_queue;
	
	ready_queue = fetchREADYqueue();
	
	/*Check queue size*/
	if(ready_queue.size != 1)
		send_fail();
	if( ( *(ready_queue.data[0]) ).task != (Task*) task_handle )
		send_fail();
		
	send_success();
}

Test tm_create_cyclic_02 = {
	.id =			"TM-CREATE-CYCLIC-02",
	.description =	"ROSA_CreateCyclicTask should insert the newly created task in the READY queue.",
	.plan =			"Task Manager",
	.suite =		"Create Cyclic Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_cyclic_02_main};

/*** TM-CREATE-CYCLIC-03 ***/

void tm_create_cyclic_03_main()
{
	TaskHandle taskHighPriority, taskMediumPriority, taskLowPriority;
	char task_name[5] = TASK_NAME;
	
	const unsigned int priorityHigh = 5;
	const unsigned int priorityMedium = 4;
	const unsigned int priorityLow = 3;
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, priorityMedium, TEST_PERIOD, TEST_DEADLINE, &taskMediumPriority);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, priorityLow, TEST_PERIOD, TEST_DEADLINE, &taskLowPriority);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, priorityHigh, TEST_PERIOD, TEST_DEADLINE, &taskHighPriority);
	
	PriorityQueue ready_queue = fetchREADYqueue();
	
	if(ready_queue.size != 3)
		send_fail();
	if ((*(ready_queue.data[0]) ).task != (Task*) taskHighPriority)
		send_fail();
	if ((*(ready_queue.data[1]) ).task != (Task*) taskLowPriority)
		send_fail();
	if ((*(ready_queue.data[2]) ).task != (Task*) taskMediumPriority)
		send_fail();
		
	send_success();
}

Test tm_create_cyclic_03 = {
	.id =			"TM-CREATE-CYCLIC-03",
	.description =	"Multiple calls to ROSA_CreateCyclicTask should populate the READY queue in the correct way with different task priorities.",
	.plan =			"Task Manager",
	.suite =		"Create Cyclic Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_cyclic_03_main};

/*** TM-CREATE-CYCLIC-04 ***/

void tm_create_cyclic_04_main()
{
	TaskHandle task1, task2, task3;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task1);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task2);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task3);
	
	PriorityQueue ready_queue = fetchREADYqueue();
	
	if(ready_queue.size != 3)
		send_fail();
	if ((*(ready_queue.data[0]) ).task != (Task*) task1)
		send_fail();
	if ((*(ready_queue.data[1]) ).task != (Task*) task2)
		send_fail();
	if ((*(ready_queue.data[2]) ).task != (Task*) task3)
		send_fail();
	
	send_success();
}

Test tm_create_cyclic_04 = {
	.id =			"TM-CREATE-CYCLIC-04",
	.description =	"Multiple calls to ROSA_CreateCyclicTask should populate the READY queue in the correct way with same task priorities.",
	.plan =			"Task Manager",
	.suite =		"Create Cyclic Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_create_cyclic_04_main};

/*** TM-CREATE-CYCLIC-05 ***/

void tm_create_cyclic_05_main()
{
	char task_name[5] = TASK_NAME;
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != SUCCESS)
		send_fail();
		
	send_success();
}

Test tm_create_cyclic_05 = {
	.id =			"TM-CREATE-CYCLIC-05",
	.description =	"Return code from ROSA_CreateCyclicTask should be SUCCESS if parameters are valid.",
	.plan =			"Task Manager",
	.suite =		"Create Cyclic Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_cyclic_05_main};

/*** TM-CREATE-CYCLIC-06 ***/

void tm_create_cyclic_06_main()
{
	char task_name[5] = TASK_NAME;
	const int invalid_priority = 0;
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, invalid_priority, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != INVALID_PRIORITY)
		send_fail();
	
	send_success();
}

Test tm_create_cyclic_06 = {
	.id =			"TM-CREATE-CYCLIC-06",
	.description =	"Return code from ROSA_CreateCyclicTask should be INVALID_PRIORITY when 0 is used for priority.",
	.plan =			"Task Manager",
	.suite =		"Create Cyclic Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_cyclic_06_main};


/*Not used anymore, since every stack size is valid
void tm_create_cyclic_07()
{
	TaskHandle task;
	char task_name[5] = TASK_NAME;
	
	send_id("TM-CREATE-CYCLIC-07");
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, 0, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task);
	
	if (return_code != INVALID_STACK_SIZE)
	{
		send_fail();
	}
	
	send_success();
}
*/

/*** TM-CREATE-CYCLIC-09 ***/

void tm_create_cyclic_08_main()
{
	char invalid_name[] = "invalid";
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, invalid_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != INVALID_NAME)
		send_fail();
	
	send_success();
}

Test tm_create_cyclic_08 = {
	.id =			"TM-CREATE-CYCLIC-08",
	.description =	"Return code from ROSA_CreateCyclicTask should be TOO_MANY_TASKS when it 3 tasks are created.",
	.plan =			"Task Manager",
	.suite =		"Create Cyclic Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_create_cyclic_08_main};

/*** TM-CREATE_CYCLIC-09 ***/

void tm_create_cyclic_09_main()
{
	char task_name[5] = TASK_NAME;
	int i;
	
	for (i = 0; i < MAX_NUMBER_TASKS; i++)
	{
		ROSA_CreateCyclicTask(TEST_PTR, task_name, 100, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, NULL);
	}
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, 100, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != TOO_MANY_TASKS)
		send_fail();
	
	send_success();
}

Test tm_create_cyclic_09 = {
	.id =			"TM-CREATE-CYCLIC-09",
	.description =	"Return code from ROSA_CreateCyclicTask should be NOT_ENOUGH_MEMORY when the system is out of memory.",
	.plan =			"Task Manager",
	.suite =		"Create Cyclic Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_create_cyclic_09_main};

/*** TM-CREATE-CYCLIC-10 ***/

void tm_create_cyclic_10_main()
{
	char task_name[5] = TASK_NAME;
	
	int enough_memory;
	do
	{
		enough_memory = (int) malloc(1);
	} while (enough_memory);
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != NOT_ENOUGH_MEMORY)
		send_fail();
	
	send_success();
}

Test tm_create_cyclic_10 = {
	.id =			"TM-CREATE-CYCLIC-10",
	.description =	"Creation of a cyclic task and a normal task with the same priority.",
	.plan =			"Task Manager",
	.suite =		"Create Cyclic Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_create_cyclic_10_main};

/**************** TEST SUITE: CREATE TASK MIXED ****************/

/*** TM-MIXED-CREATE-01 ***/

void tm_mixed_create_01_main()
{
	TaskHandle taskCyclic1, taskCyclic2, taskOrdinary;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &taskCyclic1);
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, &taskOrdinary);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &taskCyclic2);

	PriorityQueue ready_queue = fetchREADYqueue();
	
	if(ready_queue.size != 3)
		send_fail();
	if ((*(ready_queue.data[0]) ).task != (Task*) taskCyclic1)
		send_fail();
	if ((*(ready_queue.data[1]) ).task != (Task*) taskOrdinary)
		send_fail();
	if ((*(ready_queue.data[2]) ).task != (Task*) taskCyclic2)
		send_fail();
	
	send_success();
}

Test tm_mixed_create_01 = {
	.id =			"TM-MIXED-CREATE-01",
	.description =	"Creation of a cyclic task and a normal task with the same priority.",
	.plan =			"Task Manager",
	.suite =		"Create Mixed Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_mixed_create_01_main};

/*** TM-MIXED-CREATE-02 ***/

void tm_mixed_create_02_main()
{
	TaskHandle taskHighPriority, taskMediumPriority, taskLowPriority;
	char task_name[5] = TASK_NAME;
	PriorityQueue ready_queue;
	
	const unsigned int priorityHigh = 5;
	const unsigned int priorityMedium = 4;
	const unsigned int priorityLow = 3;
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, priorityMedium, TEST_PERIOD, TEST_DEADLINE, &taskMediumPriority);
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, priorityLow, &taskLowPriority);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, priorityHigh, TEST_PERIOD, TEST_DEADLINE, &taskHighPriority);

	ready_queue = fetchREADYqueue();
	
	if(ready_queue.size != 3)
		send_fail();
	if ((*(ready_queue.data[0]) ).task != (Task*) taskHighPriority)
		send_fail();
	if ((*(ready_queue.data[1]) ).task != (Task*) taskLowPriority)
		send_fail();
	if ((*(ready_queue.data[2]) ).task != (Task*) taskMediumPriority)
		send_fail();
	
	send_success();
}

Test tm_mixed_create_02 = {
	.id =			"TM-MIXED-CREATE-02",
	.description =	"Creation of a cyclic task and a normal task with different priority.",
	.plan =			"Task Manager",
	.suite =		"Create Mixed Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_mixed_create_02_main};


/************ TEST SUITE: CREATE TASK FROM TASK ************/

/*** TM-TT-CREATE-01 ***/

void tmtc1()
{
	TaskHandle task_handle_1;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask((void*) 50, task_name, 10, 3, &task_handle_1);
	
	if( ((Task*)task_handle_1)->t->staddr != ((void*) 50))
	{		
		send_fail();
	}
	if( compare_strings(task_name, ((Task*)task_handle_1)->t->id, 4) != 0)
		send_fail();
	if( ((Task*)task_handle_1)->t->datasize != 10)
		send_fail();
	if( ((Task*)task_handle_1)->originalPriority != 3)		
		send_fail();
		
	send_success();
}

void tm_tt_create_01_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(tmtc1, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

Test tm_tt_create_01 = {
	.id =			"TM-TT-CREATE-01",
	.description =	"ROSA_CreateTask should allocate and populate the task structure correctly and assign it to the provided handle.",
	.plan =			"Task Manager",
	.suite =		"Create Task From Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_tt_create_01_main};

/*** TM-TT-CREATE-02 ***/

void tmtc2()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	PriorityQueue ready_queue;
	
	ready_queue = fetchREADYqueue();
	
	ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	
	ready_queue = fetchREADYqueue();
	
	/*Check queue size*/
	if(ready_queue.size != 2)
		/*Ready queue should be 2 because the idle task exists too*/
		send_fail();
	if( ( *(ready_queue.data[0]) ).task != (Task*) task_handle )
		send_fail();
	
	send_success();
}

void tm_tt_create_02_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(tmtc2, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

Test tm_tt_create_02 = {
	.id =			"TM-TT-CREATE-02",
	.description =	"ROSA_CreateTask should insert the newly created task in the READY queue.",
	.plan =			"Task Manager",
	.suite =		"Create Task From Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_tt_create_02_main};

/*** TM-TT-CREATE-03 ***/

void tmtc3()
{
	TaskHandle taskHighPriority, taskMediumPriority, taskLowPriority;
	char task_name[5] = TASK_NAME;
	PriorityQueue ready_queue;
	
	const unsigned int priorityHigh = 5;
	const unsigned int priorityMedium = 4;
	const unsigned int priorityLow = 3;
	
	ROSA_CreateTask(TEST_PTR, task_name, 10, priorityMedium, &taskMediumPriority);
	ROSA_CreateTask(TEST_PTR, task_name, 10, priorityLow, &taskLowPriority);
	ROSA_CreateTask(TEST_PTR, task_name, 10, priorityHigh, &taskHighPriority);
	
	ready_queue = fetchREADYqueue();
	
	if(ready_queue.size != 4)
		send_fail();
	if ((*(ready_queue.data[0]) ).task != (Task*) taskHighPriority)
		send_fail();
	if ((*(ready_queue.data[2]) ).task != (Task*) taskLowPriority)
		send_fail();
	if ((*(ready_queue.data[1]) ).task != (Task*) taskMediumPriority)
		send_fail();
	
	send_success();
}

void tm_tt_create_03_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(tmtc3, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

Test tm_tt_create_03 = {
	.id =			"TM-TT-CREATE-03",
	.description =	"Multiple calls to ROSA_CreateTask should populate the READY queue in the correct way with different task priorities.",
	.plan =			"Task Manager",
	.suite =		"Create Task From Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_tt_create_03_main};

/*** TM-TT-CREATE-04 ***/

void tmtc4()
{
	TaskHandle task1, task2, task3;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(TEST_PTR, task_name, 10, PRIORITY_5, &task1);
	ROSA_CreateTask(TEST_PTR, task_name, 10, PRIORITY_5, &task2);
	ROSA_CreateTask(TEST_PTR, task_name, 10, PRIORITY_5, &task3);
	
	PriorityQueue ready_queue = fetchREADYqueue();
	
	if(ready_queue.size != 4)
		send_fail();
	if ((*(ready_queue.data[0]) ).task != (Task*) task1)
		send_fail();
	if ((*(ready_queue.data[2]) ).task != (Task*) task2)
		send_fail();
	if ((*(ready_queue.data[1]) ).task != (Task*) task3)
		send_fail();
	
	send_success();
}

void tm_tt_create_04_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(tmtc4, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

Test tm_tt_create_04 = {
	.id =			"TM-TT-CREATE-04",
	.description =	"Multiple calls to ROSA_CreateTask should populate the READY queue in the correct way with same task priorities.",
	.plan =			"Task Manager",
	.suite =		"Create Task From Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_tt_create_04_main};

/*** TM-TT-CREATE-05 ***/
void tmtc5()
{
	char task_name[5] = TASK_NAME;
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	if (return_code != SUCCESS)
	send_fail();
	
	send_success();
}

void tm_tt_create_05_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(tmtc5, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

Test tm_tt_create_05 = {
	.id =			"TM-TT-CREATE-05",
	.description =	"Return code from ROSA_CreateTask should be SUCCESS if parameters are valid.",
	.plan =			"Task Manager",
	.suite =		"Create Task From Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_tt_create_05_main};

/*** TM-TT-CREATE-06 ***/

void tmtc6()
{
	char task_name[5] = TASK_NAME;
	const int invalid_priority = 0;
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, invalid_priority, NULL);
	
	if (return_code != INVALID_PRIORITY)
	send_fail();
	
	send_success();
}

void tm_tt_create_06_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(tmtc6, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

Test tm_tt_create_06 = {
	.id =			"TM-TT-CREATE-06",
	.description =	"Return code from ROSA_CreateTask should be INVALID_PRIORITY when 0 is used for priority.",
	.plan =			"Task Manager",
	.suite =		"Create Task From Task",
	.type =			TEST_FUNCTIONAL,
	.function =		tm_tt_create_06_main};

/*

void tmtc7()
{
	char task_name[5] = TASK_NAME;
	const int invalid_stack_size = 10;
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, invalid_stack_size, PRIORITY_5, NULL);
	
	if (return_code != INVALID_STACK_SIZE)
	send_fail();
	
	send_success();
}

void tm_tt_create_07()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(tmtc7, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}*/

/*** TM-TT-CREATE-08 ***/

void tmtc8()
{
	char invalid_name[7] = "invalid";
	
	int return_code = ROSA_CreateTask(TEST_PTR, invalid_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	if (return_code != INVALID_NAME)
	send_fail();
	
	send_success();
}

void tm_tt_create_08_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(tmtc8, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

Test tm_tt_create_08 = {
	.id =			"TM-TT-CREATE-08",
	.description =	"Return code from ROSA_CreateTask should be TOO_MANY_TASKS when it 3 tasks are created.",
	.plan =			"Task Manager",
	.suite =		"Create Task From Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_tt_create_08_main};

/*** TM-TT-CREATE-09 ***/

void tmtc9()
{
	char task_name[5] = TASK_NAME;
	int i;
	
	for (i = 0; i < MAX_NUMBER_TASKS; i++)
	{
		ROSA_CreateTask(TEST_PTR, task_name, 100, PRIORITY_5, NULL);
	}
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, 100, PRIORITY_5, NULL);
	
	if (return_code != TOO_MANY_TASKS)
	send_fail();
	
	send_success();
}

void tm_tt_create_09_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(tmtc8, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

Test tm_tt_create_09 = {
	.id =			"TM-TT-CREATE-09",
	.description =	"Return code from ROSA_CreateTask should be TOO_MANY_TASKS when it 3 tasks are created.",
	.plan =			"Task Manager",
	.suite =		"Create Task From Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_tt_create_09_main};

/********** TEST SUITE: TERMINATE TASK **************/

void success_sender(void)
{
	send_success();
}

void terminate_myself(void)
{
	ROSA_TerminateTask();
	send_fail();
}

/*** TM-TERMINATE-01 ***/

void tm_terminate_01_main()
{
	int high_priority = 5, low_priority = 3;
	
	ROSA_CreateTask(&terminate_myself, "high", SMALL_STACK_SIZE, high_priority, NULL);
	ROSA_CreateTask(&success_sender, "low", SMALL_STACK_SIZE, low_priority, NULL);
	
	ROSA_Start();
	
	send_fail();
}

Test tm_terminate_01 = {
	.id =			"TM-TERMINATE-01",
	.description =	"When user calls and invokes ROSA_TerminateTask, the task should be terminated.",
	.plan =			"Task Manager",
	.suite =		"Terminate Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_terminate_01_main};

/*void mess_up_CRT_and_terminate(void)
{
	setCRT(42);
	if (ROSA_TerminateTask() == FAILURE)
		send_success();
		
	send_fail();
}

void tm_terminate_02()
{
	send_id("TM-TERMINATE-02");
	
	ROSA_CreateTask(&mess_up_CRT_and_terminate, "mess", SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(&success_sender, "low", STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_Start();
	
	send_fail();
}*/

/*** TM-TERMINATE-03 ***/
unsigned int task_creation_counter = 0;

void make_clone_and_terminate(void)
{
	if (++task_creation_counter == 65)
		send_success();
		
	if (ROSA_CreateTask(&make_clone_and_terminate, "high", 4096, PRIORITY_5, NULL) != SUCCESS)
		send_fail();
		
	ROSA_TerminateTask();
}

void tm_terminate_03_main()
{
	ROSA_CreateTask(&make_clone_and_terminate, "high", 4096, PRIORITY_5, NULL);
	
	ROSA_Start();
	
	send_fail();
}

Test tm_terminate_03 = {
	.id =			"TM-TERMINATE-03",
	.description =	"Check for memory leaks using stress test.",
	.plan =			"Task Manager",
	.suite =		"Terminate Task",
	.type =			TEST_FUNCTIONAL,
.function =		tm_terminate_03_main};