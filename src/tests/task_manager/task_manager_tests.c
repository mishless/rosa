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
#define SMALL_STACK_SIZE 1000
#define PRIORITY_5 5
#define PRIORITY_4 4

#define TEST_PERIOD 25
#define TEST_DEADLINE 20

// ROSA_CreateTask

void tm_create_01()
{
	/*ROSA_CreateTask should allocate and populate the task structure 
	  correctly and assign it to the provided handle.*/
	
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("TM-CREATE-01");
	
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

void tm_create_02()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
		
	send_id("TM-CREATE-02");
	
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

void tm_create_03()
{
	TaskHandle taskHighPriority, taskMediumPriority, taskLowPriority;
	char task_name[5] = TASK_NAME;
	PriorityQueue ready_queue;
	
	const unsigned int priorityHigh = 5;
	const unsigned int priorityMedium = 4;
	const unsigned int priorityLow = 3;
	
	send_id("TM-CREATE-03");
	
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

void tm_create_04()
{
	TaskHandle task1, task2, task3;
	char task_name[5] = TASK_NAME;
	
	send_id("TM-CREATE-04");
	
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

void tm_create_05()
{
	char task_name[5] = TASK_NAME;
	
	send_id("TM-CREATE-05");
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	if (return_code != SUCCESS)
		send_fail();
		
	send_success();
}

void tm_create_06()
{
	char task_name[5] = TASK_NAME;
	const int invalid_priority = 0;
	
	send_id("TM-CREATE-06");
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, invalid_priority, NULL);
	
	if (return_code != INVALID_PRIORITY)
		send_fail();
	
	send_success();
}

/*Not used anymore, since every stack size is valid*/
void tm_create_07()
{
	char task_name[5] = TASK_NAME;
	const int invalid_stack_size = 10;
	
	send_id("TM-CREATE-07");
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, invalid_stack_size, PRIORITY_5, NULL);
	
	if (return_code != INVALID_STACK_SIZE)
	send_fail();
	
	send_success();
}


void tm_create_08()
{
	char invalid_name[7] = "invalid";
	
	send_id("TM-CREATE-08");
	
	int return_code = ROSA_CreateTask(TEST_PTR, invalid_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	if (return_code != INVALID_NAME)
		send_fail();
	
	send_success();
}

void tm_create_09()
{
	char task_name[5] = TASK_NAME;
	int i;
	
	send_id("TM-CREATE-09");
	
	for (i = 0; i < MAX_NUMBER_TASKS; i++)
	{
		ROSA_CreateTask(TEST_PTR, task_name, 100, PRIORITY_5, NULL);
	}
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, 100, PRIORITY_5, NULL);
	
	if (return_code != TOO_MANY_TASKS)
		send_fail();
	
	send_success();
}

void tm_create_10()
{
	char task_name[5] = TASK_NAME;
	
	send_id("TM-CREATE-10");
	
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


// ROSA_CreateCyclicTask

void tm_create_cyclic_01()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("TM-CREATE-CYCLIC-01");
	
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

void tm_create_cyclic_02()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
		
	send_id("TM-CREATE-CYCLIC-02");
	
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

void tm_create_cyclic_03()
{
	TaskHandle taskHighPriority, taskMediumPriority, taskLowPriority;
	char task_name[5] = TASK_NAME;
	
	const unsigned int priorityHigh = 5;
	const unsigned int priorityMedium = 4;
	const unsigned int priorityLow = 3;
	
	send_id("TM-CREATE-CYCLIC-03");
	
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

void tm_create_cyclic_04()
{
	TaskHandle task1, task2, task3;
	char task_name[5] = TASK_NAME;
	
	send_id("TM-CREATE-CYCLIC-04");
	
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

void tm_create_cyclic_05()
{
	char task_name[5] = TASK_NAME;
	
	send_id("TM-CREATE-CYCLIC-05");
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != SUCCESS)
		send_fail();
		
	send_success();
}

void tm_create_cyclic_06()
{
	char task_name[5] = TASK_NAME;
	const int invalid_priority = 0;
	
	send_id("TM-CREATE-CYCLIC-06");
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, SMALL_STACK_SIZE, invalid_priority, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != INVALID_PRIORITY)
		send_fail();
	
	send_success();
}

/*Not used anymore, since every stack size is valid*/
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

void tm_create_cyclic_08()
{
	char invalid_name[] = "invalid";
	
	send_id("TM-CREATE-CYCLIC-08");
	
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, invalid_name, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != INVALID_NAME)
		send_fail();
	
	send_success();
}

void tm_create_cyclic_09()
{
	char task_name[5] = TASK_NAME;
	int i;
	
	send_id("TM-CREATE-CYCLIC-09");
	
	for (i = 0; i < MAX_NUMBER_TASKS; i++)
	{
		ROSA_CreateCyclicTask(TEST_PTR, task_name, 100, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, NULL);
	}
	int return_code = ROSA_CreateCyclicTask(TEST_PTR, task_name, 100, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, NULL);
	
	if (return_code != TOO_MANY_TASKS)
		send_fail();
	
	send_success();
}

void tm_create_cyclic_10()
{
	char task_name[5] = TASK_NAME;
	
	send_id("TM-CREATE-CYCLIC-10");
	
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

// mixed ROSA_CreateCyclicTask and ROSA_CreateTask

void tm_mixed_create_01()
{
	TaskHandle taskCyclic1, taskCyclic2, taskOrdinary;
	char task_name[5] = TASK_NAME;
	
	send_id("TM-MIXED-CREATE-01");
	
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

void tm_mixed_create_02()
{
	TaskHandle taskHighPriority, taskMediumPriority, taskLowPriority;
	char task_name[5] = TASK_NAME;
	PriorityQueue ready_queue;
	
	const unsigned int priorityHigh = 5;
	const unsigned int priorityMedium = 4;
	const unsigned int priorityLow = 3;
	
	send_id("TM-MIXED-CREATE-02");
	
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

/****************TM-CREATE-23****************/
unsigned int tmcr23_flag = 0;
void tmcr23_high()
{
	tmcr23_flag = 1;
	ROSA_TerminateTask();
	send_fail();
}

void tmcr23_low()
{
	TaskHandle high;
	ROSA_CreateTask(tmcr23_high, "high", 100, 6, &high);
	while(tmcr23_flag == 0);
	send_success();
}

void tm_create_23()
{
	TaskHandle low;
	
	send_id("TM-CREATE-23");
	
	ROSA_CreateTask(tmcr23_low, "tst", 100, 5, &low);
	
	ROSA_Start();
	send_fail();
}

/************TM-CREATE-24***************/
void tmcr24_low()
{
	send_fail();
}

void tmcr24_high()
{
	TaskHandle low;
	ROSA_CreateTask(tmcr24_low, "low", SMALL_STACK_SIZE, 4, &low);
	busy_wait(20);
	send_success();
}



void tm_create_24()
{
	TaskHandle high;
	
	send_id("TM-CREATE-24");
	ROSA_CreateTask(tmcr24_high, "high", SMALL_STACK_SIZE, 5, &high);
	ROSA_Start();
	send_fail();
}

//Create Task functionality from a task

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

void tm_tt_create_01()
{
	TaskHandle task_handle;
	
	send_id("TM-TT-CREATE-01");
	
	ROSA_CreateTask(tmtc1, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

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

void tm_tt_create_02()
{
	TaskHandle task_handle;
	
	send_id("TM-TT-CREATE-02");
	
	ROSA_CreateTask(tmtc2, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

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

void tm_tt_create_03()
{
	TaskHandle task_handle;
	
	send_id("TM-TT-CREATE-03");
	
	ROSA_CreateTask(tmtc3, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

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

void tm_tt_create_04()
{
	TaskHandle task_handle;
	
	send_id("TM-TT-CREATE-04");
	
	ROSA_CreateTask(tmtc4, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

void tmtc5()
{
	char task_name[5] = TASK_NAME;
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	if (return_code != SUCCESS)
	send_fail();
	
	send_success();
}

void tm_tt_create_05()
{
	TaskHandle task_handle;
	
	send_id("TM-TT-CREATE-05");
	
	ROSA_CreateTask(tmtc5, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

void tmtc6()
{
	char task_name[5] = TASK_NAME;
	const int invalid_priority = 0;
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, SMALL_STACK_SIZE, invalid_priority, NULL);
	
	if (return_code != INVALID_PRIORITY)
	send_fail();
	
	send_success();
}

void tm_tt_create_06()
{
	TaskHandle task_handle;
	
	send_id("TM-TT-CREATE-06");
	
	ROSA_CreateTask(tmtc6, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

void tmtc7()
{
	char task_name[5] = TASK_NAME;
	const int invalid_stack_size = 10;
	
	int return_code = ROSA_CreateTask(TEST_PTR, task_name, invalid_stack_size, PRIORITY_5, NULL);
	
	if (return_code != INVALID_STACK_SIZE)
	send_fail();
	
	send_success();
}

/*Not used anymore, since every stack size is valid*/
void tm_tt_create_07()
{
	TaskHandle task_handle;
	
	send_id("TM-TT-CREATE-07");
	
	ROSA_CreateTask(tmtc7, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

void tmtc8()
{
	char invalid_name[7] = "invalid";
	
	int return_code = ROSA_CreateTask(TEST_PTR, invalid_name, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	if (return_code != INVALID_NAME)
	send_fail();
	
	send_success();
}

void tm_tt_create_08()
{
	TaskHandle task_handle;
	
	send_id("TM-TT-CREATE-08");
	
	ROSA_CreateTask(tmtc8, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

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

void tm_tt_create_09()
{
	TaskHandle task_handle;
	
	send_id("TM-TT-CREATE-09");
	
	ROSA_CreateTask(tmtc8, "task", SMALL_STACK_SIZE, 5, &task_handle);
	ROSA_Start();
	send_fail();
}

// TERMINATE

void success_sender(void)
{
	send_success();
}

void terminate_myself(void)
{
	ROSA_TerminateTask();
	send_fail();
}

void tm_terminate_01()
{
	int high_priority = 5, low_priority = 3;
	
	send_id("TM-TERMINATE-01");
	
	ROSA_CreateTask(&terminate_myself, "high", SMALL_STACK_SIZE, high_priority, NULL);
	ROSA_CreateTask(&success_sender, "low", SMALL_STACK_SIZE, low_priority, NULL);
	
	ROSA_Start();
	
	send_fail();
}


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


unsigned int task_creation_counter = 0;

void make_clone_and_terminate(void)
{
	if (++task_creation_counter == 65)
		send_success();
		
	if (ROSA_CreateTask(&make_clone_and_terminate, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL) != SUCCESS)
		send_fail();
		
	ROSA_TerminateTask();
}

void tm_terminate_03()
{
	send_id("TM-TERMINATE-03");
	
	ROSA_CreateTask(&make_clone_and_terminate, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	ROSA_Start();
	
	send_fail();
}
