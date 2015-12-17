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
#define STACK_SIZE 128
#define TEST_PRIORITY 6

#define TEST_PERIOD 25
#define TEST_DEADLINE 20

void tm_create_01()
{
	/*ROSA_CreateTask should allocate and populate the task structure 
	  correctly and assign it to the provided handle.*/
	
	TaskHandle task_handle;
	char task_name[4] = TASK_NAME;
	
	send_id("TM-CREATE-01");
	
	ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, &task_handle);
	
	if( ((Task*)task_handle)->t->staddr != TEST_PTR)
	{		
		send_fail();
		BLOCK;
	}
	if( compare_strings(task_name, ((Task*)task_handle)->t->id, 4) != 0)
		send_fail();
	if( ((Task*)task_handle)->t->datasize != STACK_SIZE)
		send_fail();
	if( ((Task*)task_handle)->originalPriority != TEST_PRIORITY)		
		send_fail();
		
	send_success();
}

void tm_create_02()
{
	TaskHandle task_handle;
	char task_name[4] = TASK_NAME;
		
	send_id("TM-CREATE-02");
	
	ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, &task_handle);
	PriorityQueue ready_queue;
	
	ready_queue = fetchREADYqueue();
	
	/*Check queue size*/
	if(ready_queue.size != 1)
		send_fail();
	if( ( *(ready_queue.data[0]) ).task != (Task*) task_handle )
		send_fail();
		
	send_success();
}

void tm_create_03()
{
	TaskHandle taskHighPriority, taskMediumPriority, taskLowPriority;
	char task_name[4] = TASK_NAME;
	
	const unsigned int priorityHigh = 5;
	const unsigned int priorityMedium = 4;
	const unsigned int priorityLow = 3;
	
	send_id("TM-CREATE-03");
	
	ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, priorityMedium, &taskMediumPriority);
	ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, priorityLow, &taskLowPriority);
	ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, priorityHigh, &taskHighPriority);
	
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

void tm_create_04()
{
	TaskHandle task1, task2, task3;
	char task_name[4] = TASK_NAME;
	
	send_id("TM-CREATE-04");
	
	ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, &task1);
	ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, &task2);
	ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, &task3);
	
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

void tm_create_05()
{
	char task_name[4] = TASK_NAME;
	
	send_id("TM-CREATE-05");
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, NULL);
	
	if (return_code != SUCCESS)
		send_fail();
		
	send_success();
}

void tm_create_06()
{
	char task_name[4] = TASK_NAME;
	const int invalid_priority = 0;
	
	send_id("TM-CREATE-06");
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, invalid_priority, NULL);
	
	if (return_code != INVALID_PRIORITY)
		send_fail();
	
	send_success();
}


void tm_create_07()
{
	char invalid_name[7] = "invalid";
	
	send_id("TM-CREATE-07");
	
	int return_code = ROSA_CreateTask(TEST_PTR, invalid_name, STACK_SIZE, TEST_PRIORITY, NULL);
	
	if (return_code != INVALID_NAME)
		send_fail();
	
	send_success();
}

void tm_create_08()
{
	char task_name[4] = TASK_NAME;
	int i;
	
	send_id("TM-CREATE-08");
	
	for (i = 0; i < MAX_NUMBER_TASKS; i++)
	{
		ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, NULL);
	}
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, NULL);
	
	if (return_code != TOO_MANY_TASKS)
		send_fail();
	
	send_success();
}

void tm_create_09()
{
	char task_name[4] = TASK_NAME;
	
	send_id("TM-CREATE-09");
	
	int enough_memory;
	do
	{
		enough_memory = (int) malloc(1);
	} while (enough_memory);
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, NULL);
	
	if (return_code != NOT_ENOUGH_MEMORY)
		send_fail();
	
	send_success();
}

void tm_create_10()
{
	char task_name[4] = TASK_NAME;
	const int invalid_stack_size = 10;
	
	send_id("TM-CREATE-10");
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, invalid_stack_size, TEST_PRIORITY, NULL);
	
	if (return_code != INVALID_STACK_SIZE)
		send_fail();
	
	send_success();
}

void tm_create_11()
{
	TaskHandle task_handle;
	char task_name[4] = TASK_NAME;
	
	send_id("TM-CREATE-11");
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, &task_handle);
	
	if( ((Task*)task_handle)->t->staddr != TEST_PTR)
	{		
		send_fail();
		BLOCK;
	}
	if( compare_strings(task_name, ((Task*)task_handle)->t->id, 4) != 0)
		send_fail();
	if( ((Task*)task_handle)->t->datasize != STACK_SIZE)
		send_fail();
	if( ((Task*)task_handle)->originalPriority != TEST_PRIORITY)		
		send_fail();
	if( ((Task*)task_handle)->deadline != TEST_DEADLINE)
		send_fail();
	if( ((Task*)task_handle)->period != TEST_PERIOD)
		send_fail();
		
	send_success();
}

void tm_create_12()
{
	TaskHandle task_handle;
	char task_name[4] = TASK_NAME;
		
	send_id("TM-CREATE-12");
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, &task_handle);
	PriorityQueue ready_queue;
	
	ready_queue = fetchREADYqueue();
	
	/*Check queue size*/
	if(ready_queue.size != 1)
		send_fail();
	if( ( *(ready_queue.data[0]) ).task != (Task*) task_handle )
		send_fail();
		
	send_success();
}

void tm_create_13()
{
	TaskHandle taskHighPriority, taskMediumPriority, taskLowPriority;
	char task_name[4] = TASK_NAME;
	
	const unsigned int priorityHigh = 5;
	const unsigned int priorityMedium = 4;
	const unsigned int priorityLow = 3;
	
	send_id("TM-CREATE-13");
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, priorityMedium, TEST_PERIOD, TEST_DEADLINE, &taskMediumPriority);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, priorityLow, TEST_PERIOD, TEST_DEADLINE, &taskLowPriority);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, priorityHigh, TEST_PERIOD, TEST_DEADLINE, &taskHighPriority);
	
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

void tm_create_14()
{
	TaskHandle task1, task2, task3;
	char task_name[4] = TASK_NAME;
	
	send_id("TM-CREATE-14");
	
	ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, &task1);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, &task2);
	ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, &task3);
	
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

void tm_create_15()
{
	char task_name[4] = TASK_NAME;
	
	send_id("TM-CREATE-15");
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != SUCCESS)
		send_fail();
		
	send_success();
}

void tm_create_16()
{
	char task_name[4] = TASK_NAME;
	const int invalid_priority = 0;
	
	send_id("TM-CREATE-16");
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, invalid_priority, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != INVALID_PRIORITY)
		send_fail();
	
	send_success();
}


void tm_create_17()
{
	char invalid_name[] = "invalid";
	
	send_id("TM-CREATE-17");
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, invalid_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != INVALID_NAME)
		send_fail();
	
	send_success();
}

void tm_create_18()
{
	char task_name[4] = TASK_NAME;
	int i;
	
	send_id("TM-CREATE-18");
	
	for (i = 0; i < MAX_NUMBER_TASKS; i++)
	{
		ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, NULL);
	}
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != TOO_MANY_TASKS)
		send_fail();
	
	send_success();
}

void tm_create_19()
{
	char task_name[4] = TASK_NAME;
	
	send_id("TM-CREATE-19");
	
	int enough_memory;
	do
	{
		enough_memory = (int) malloc(1);
	} while (enough_memory);
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != NOT_ENOUGH_MEMORY)
		send_fail();
	
	send_success();
}

void tm_create_20()
{
	char task_name[4] = TASK_NAME;
	const int invalid_stack_size = 10;
	
	send_id("TM-CREATE-20");
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, invalid_stack_size, TEST_PRIORITY, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != INVALID_STACK_SIZE)
		send_fail();
	
	send_success();
}

/* tm_create_21 - tm_create_23 */

void high_priority_terminate_myself(void)
{
	ROSA_TerminateTask();
	send_fail();
}

void lower_priority_send_success(void)
{
	send_success();
}

void tm_terminate_01()
{
	int high_priority = 5, low_priority = 3;
	
	send_id("TM-TERMINATE-01");
	
	ROSA_CreateTask(&high_priority_terminate_myself, "high", STACK_SIZE, high_priority, NULL);
	ROSA_CreateTask(&lower_priority_send_success, "low", STACK_SIZE, low_priority, NULL);
	
	ROSA_Start();
	
	send_fail();
}
