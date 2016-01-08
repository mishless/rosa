/*
 * performance_tests.c
 *
 * Created: 08.01.2016. 2:21:16 PM
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
#include "performance_tests.h"

#define TASK_NAME "test"
#define SMALL_STACK_SIZE 1000
#define PRIORITY_5 5
#define PRIORITY_4 4
#define TEST_PTR (void*) 42

#define TEST_PERIOD 25
#define TEST_DEADLINE 20

/*************** TEST SUITE: Performance testing of system calls ***************/

/*** PT-SC-01 ***/


void pt_sc_01_main()
{
	TIMER_DECLARE();
	TaskHandle task_handle;
	
	unsigned int ticksBefore, ticksAfter;
		
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_CreateTask(TEST_PTR, "task", SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);

}
Test pt_sc_01 = {
	.id =			"PT-SC-01",
	.description =	"Create a task before the scheduler has started.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_01_main};

/*** PT-SC-02 ***/


void pt_sc_02_main()
{
	TIMER_DECLARE();
	TaskHandle task_handle;
	
	unsigned int ticksBefore, ticksAfter;
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_CreateCyclicTask(TEST_PTR, TASK_NAME, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task_handle);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);

}
Test pt_sc_02 = {
	.id =			"PT-SC-02",
	.description =	"Create a cyclic task before the scheduler has started.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_02_main};

/*** PT-SC-03 ***/

void task_pt_sc_03()
{
	TIMER_DECLARE();
	TaskHandle task_handle2;
	
	unsigned int ticksBefore, ticksAfter;

	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();

	ROSA_CreateTask(TEST_PTR, TASK_NAME, SMALL_STACK_SIZE, PRIORITY_5,  &task_handle2);

	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);

}

void pt_sc_03_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(task_pt_sc_03, "task", SMALL_STACK_SIZE, PRIORITY_4, &task_handle);
	
	ROSA_Start();
}
Test pt_sc_03 = {
	.id =			"PT-SC-03",
	.description =	"Create a task when the ready queue is empty and the scheduler has started.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_03_main};

/*** PT-SC-04 ***/

void task_pt_sc_04()
{
	TIMER_DECLARE();
	TaskHandle task_handle2;
	
	unsigned int ticksBefore, ticksAfter;

	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();

	ROSA_CreateTask(TEST_PTR, TASK_NAME, SMALL_STACK_SIZE, PRIORITY_5, &task_handle2);

	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);

}

void pt_sc_04_main(unsigned int maximum_tasks)
{
	TaskHandle task_handle;
	int i;
	for (i = 0; i < (maximum_tasks-1) ; i++)
	{
		ROSA_CreateTask(task_pt_sc_04, "task", SMALL_STACK_SIZE, PRIORITY_4, &task_handle);
	}
	
	ROSA_Start();
}
Test pt_sc_04 = {
	.id =			"PT-SC-04",
	.description =	"Create a task after the ready queue has only one free slot  .",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "2",
	.end_parameter = "42",
.function =		pt_sc_04_main};



/*** PT-SC-05 ***/

void task_pt_sc_05()
{
	TIMER_DECLARE();
	TaskHandle task_handle2;
	
	unsigned int ticksBefore, ticksAfter;

	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();

	ROSA_CreateCyclicTask(TEST_PTR, TASK_NAME, SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task_handle2);

	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);

}

void pt_sc_05_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateCyclicTask(task_pt_sc_05, "task", SMALL_STACK_SIZE, PRIORITY_4, TEST_PERIOD, TEST_DEADLINE, &task_handle);
	
	ROSA_Start();
}
Test pt_sc_05 = {
	.id =			"PT-SC-05",
	.description =	"Create a cyclic task when the ready queue is empty and the scheduler has started.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_05_main};


/*** PT-SC-06 ***/

void task_pt_sc_06()
{
	TIMER_DECLARE();
	TaskHandle task_handle2;
	
	unsigned int ticksBefore, ticksAfter;

	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();

	ROSA_CreateCyclicTask(TEST_PTR, TASK_NAME, SMALL_STACK_SIZE, PRIORITY_5,TEST_PERIOD, TEST_DEADLINE, &task_handle2);

	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);

}

void pt_sc_06_main(unsigned int maximum_tasks)
{
	TaskHandle task_handle;
	int i;
	for (i = 0; i < (maximum_tasks-1) ; i++)
	{
		ROSA_CreateCyclicTask(task_pt_sc_06, "task", SMALL_STACK_SIZE, PRIORITY_4, TEST_PERIOD, TEST_DEADLINE, &task_handle);
	}
	
	ROSA_Start();
}
Test pt_sc_06 = {
	.id =			"PT-SC-06",
	.description =	"Create a cyclic task after the ready queue has only one free slot  .",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "2",
	.end_parameter = "42",
.function =		pt_sc_06_main};

/*** PT-SC-07 ***/

unsigned int ticksBefore =0 ,ticksAfter = 0;

void task_pt_sc_07_high()
{
	TIMER_DECLARE();
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_TerminateTask();
}

void task_pt_sc_07_low()
{
	TIMER_DECLARE();
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}


void pt_sc_07_main()
{
	TaskHandle task_handle;
	TaskHandle task_handle2;
	
	ROSA_CreateTask(task_pt_sc_07_low, "task", SMALL_STACK_SIZE, PRIORITY_4, &task_handle);
	ROSA_CreateTask(task_pt_sc_07_high, TASK_NAME, SMALL_STACK_SIZE, PRIORITY_5, &task_handle2);
	
	ROSA_Start();
}
Test pt_sc_07 = {
	.id =			"PT-SC-07",
	.description =	"Terminate currently running task and switch to a new task.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_07_main};


/*** PT-SC-08 ***/


void task_pt_sc_08()
{
	TIMER_DECLARE();
	unsigned int time = 99;
	
	TIMER_INIT();
	TIMER_START();
	ROSA_TimerTickCount();
	time = TIMER_VALUE();
	send_result(time);
}

void pt_sc_08_main()
{
	TaskHandle task_handle;
	
	ROSA_CreateTask(task_pt_sc_08, "task", SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	ROSA_Start();
}
Test pt_sc_08 = {
	.id =			"PT-SC-08",
	.description =	"Get current ticks from a task. This is fixed time.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_08_main};

void dummy_main(unsigned int param)
{
	send_result(param*42);
}

Test dummy_test = {
	.id =			"DUMMY-42",
	.description =	"Testing the Testing tool. Inception. Deep stuff!",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "1",
	.end_parameter = "42",
.function =		dummy_main};