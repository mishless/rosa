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
#define PRIORITY_3 3
#define TEST_PTR (void*) 42

#define TEST_PERIOD 25
#define TEST_DEADLINE 20

/*************** TEST SUITE: Performance testing of system calls ***************/

/*** PT-SC-01 ***/

unsigned int ticksBefore = 0 ,ticksAfter = 0;

void pt_sc_01_main()
{
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

void task_pt_sc_07_high()
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_TerminateTask();
}

void task_pt_sc_07_low()
{
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

/*** PT-SC-09 ***/

void task_pt_sc_09_low(void)
{
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void task_pt_sc_09_high(void)
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_DelayAbsolute(0, 20);
}

void pt_sc_09_main(void)
{
	ROSA_CreateTask(task_pt_sc_09_low, "low", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_CreateTask(task_pt_sc_09_high, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL);

	ROSA_StartScheduler();
}

Test pt_sc_09 = {
	.id =			"PT-SC-09",
	.description =	"Measure time for executing absolute delay and switch to the next highest priority task when the delay queue is empty.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_09_main};

/*** PT-SC-10 ***/

void task_pt_sc_10_high(void)
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_DelayAbsolute(0, 30);
}

void task_pt_sc_10_medium(void)
{
	ROSA_DelayAbsolute(0, 10000);
}

void task_pt_sc_10_low(void)
{
	ROSA_CreateTask(task_pt_sc_10_high, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void pt_sc_10_main(unsigned int number_of_medium_prio_tasks)
{
	int i;
	ROSA_CreateTask(task_pt_sc_10_low, "low", SMALL_STACK_SIZE, PRIORITY_3, NULL);
	
	for (i = 0; i < number_of_medium_prio_tasks; i++)
	{
		ROSA_CreateTask(task_pt_sc_10_medium, "med", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	}
	
	ROSA_StartScheduler();
}

Test pt_sc_10 = {
	.id =			"PT-SC-10",
	.description =	"Measure time for executing absolute delay and switch to the next highest priority task when the delay queue has only one free slot and all tasks’ wake up time is after the newly delayed one.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "40",
.function =		pt_sc_10_main};

/*** PT-SC-11 ***/

void task_pt_sc_11_low(void)
{
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void task_pt_sc_11_high(void)
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_DelayRelative(20);
}

void pt_sc_11_main(void)
{
	ROSA_CreateTask(task_pt_sc_11_low, "low", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_CreateTask(task_pt_sc_11_high, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL);

	ROSA_StartScheduler();
}

Test pt_sc_11 = {
	.id =			"PT-SC-11",
	.description =	"Measure time for executing relative delay and switch to the next highest priority task when the delay queue is empty.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_11_main};

/*** PT-SC-12 ***/

void task_pt_sc_12_high(void)
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_DelayRelative(30);
}

void task_pt_sc_12_medium(void)
{
	ROSA_DelayRelative(10000);
}

void task_pt_sc_12_low(void)
{
	ROSA_CreateTask(task_pt_sc_12_high, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void pt_sc_12_main(unsigned int number_of_medium_prio_tasks)
{
	int i;
	ROSA_CreateTask(task_pt_sc_12_low, "low", SMALL_STACK_SIZE, PRIORITY_3, NULL);
	
	for (i = 0; i < number_of_medium_prio_tasks; i++)
	{
		ROSA_CreateTask(task_pt_sc_12_medium, "med", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	}
	
	ROSA_StartScheduler();
}

Test pt_sc_12 = {
	.id =			"PT-SC-12",
	.description =	"Measure time for executing relative delay and switch to the next highest priority task when the delay queue has only one free slot and all tasks wake up time is after the newly delayed one.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "40",
.function =		pt_sc_12_main};

/*** PT-SC-13 ***/

void task_pt_sc_13_low(void)
{
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void task_pt_sc_13_high(void)
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_EndCycle();
}

void pt_sc_13_main(void)
{
	ROSA_CreateTask(task_pt_sc_13_low, "low", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_CreateCyclicTask(task_pt_sc_13_high, "high", SMALL_STACK_SIZE, PRIORITY_5, 30, 30, NULL);

	ROSA_StartScheduler();
}

Test pt_sc_13 = {
	.id =			"PT-SC-13",
	.description =	"Measure time for executing end cycle and switch to the next highest priority task when the delay queue is empty.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_13_main};

/*** PT-SC-14 ***/

void task_pt_sc_14_high(void)
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_EndCycle();
}

void task_pt_sc_14_medium(void)
{
	ROSA_DelayRelative(10000);
}

void task_pt_sc_14_low(void)
{
	ROSA_CreateCyclicTask(task_pt_sc_14_high, "high", SMALL_STACK_SIZE, PRIORITY_5, 30, 30, NULL);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void pt_sc_14_main(unsigned int number_of_medium_prio_tasks)
{
	int i;
	ROSA_CreateTask(task_pt_sc_14_low, "low", SMALL_STACK_SIZE, PRIORITY_3, NULL);
	
	for (i = 0; i < number_of_medium_prio_tasks; i++)
	{
		ROSA_CreateTask(task_pt_sc_14_medium, "med", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	}
	
	ROSA_StartScheduler();
}

Test pt_sc_14 = {
	.id =			"PT-SC-14",
	.description =	"Measure time for executing end cycle and switch to the next highest priority task when the delay queue has only one free slot and all tasks’ wake up time is after the newly delayed one.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "40",
.function =		pt_sc_14_main};

/*** PT-SC-15 ***/

void task_pt_sc_15_high(void)
{
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void task_pt_sc_15_low(void)
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_CreateTask(task_pt_sc_15_high, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL);
}

void pt_sc_15_main(void)
{
	ROSA_CreateTask(task_pt_sc_15_low, "low", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_StartScheduler();
}

Test pt_sc_15 = {
	.id =			"PT-SC-15",
	.description =	"Creating new higher priority task and switch to it after the scheduler has been started.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_15_main};

/*** PT-SC-16 ***/

void pt_sc_16_main(void)
{
	unsigned int ticksBefore, ticksAfter;
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_SemaphoreCreateBinary(NULL, SEMAPHORE_FREE);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

Test pt_sc_16 = {
	.id =			"PT-SC-16",
	.description =	"Create a semaphore.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_16_main};

/*** PT-SC-17 ***/

void task_pt_sc_17(void)
{
	unsigned int ticksBefore, ticksAfter;
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_SemaphoreCreateBinary(NULL, SEMAPHORE_FREE);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void pt_sc_17_main(void)
{
	ROSA_CreateTask(task_pt_sc_17, "task", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_StartScheduler();
}

Test pt_sc_17 = {
	.id =			"PT-SC-17",
	.description =	"Create a semaphore from a task.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_17_main};


/*** PT-SC-18 ***/

void pt_sc_18_main(void)
{
	unsigned int ticksBefore, ticksAfter;
	SemaphoreHandle sem;
	
	ROSA_SemaphoreCreateBinary(&sem, SEMAPHORE_FREE);
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_SemaphoreDelete(sem);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

Test pt_sc_18 = {
	.id =			"PT-SC-18",
	.description =	"Delete a semaphore.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_18_main};

/*** PT-SC-19 ***/

void task_pt_sc_19(void)
{
	unsigned int ticksBefore, ticksAfter;
	SemaphoreHandle sem;
	
	ROSA_SemaphoreCreateBinary(&sem, SEMAPHORE_FREE);
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_SemaphoreDelete(sem);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void pt_sc_19_main(void)
{
	ROSA_CreateTask(task_pt_sc_19, "task", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_StartScheduler();
}

Test pt_sc_19 = {
	.id =			"PT-SC-19",
	.description =	"Delete a semaphore from a currently running task.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_19_main};

/*** PT-SC-20 ***/

SemaphoreHandle sem_20;

void task_pt_sc_20(void)
{
	unsigned int ticksBefore, ticksAfter;
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_SemaphoreTake(sem_20, 0);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void pt_sc_20_main(void)
{
	ROSA_CreateTask(task_pt_sc_20, "task", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_SemaphoreCreateBinary(&sem_20, SEMAPHORE_FREE);
	
	ROSA_StartScheduler();
}

Test pt_sc_20 = {
	.id =			"PT-SC-20",
	.description =	"Take a free binary semaphore from a task.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_20_main};

/*** PT-SC-21 ***/

SemaphoreHandle sem_21;

void task_pt_sc_21_low(void)
{
	ROSA_SemaphoreTake(sem_21, 10000);
}

void task_pt_sc_21_high(void)
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_SemaphoreTake(sem_21, 100);
}

void task_pt_sc_21_medium(void)
{
	ROSA_SemaphoreTake(sem_21, 0);
	ROSA_DelayRelative(200);
	
	ROSA_CreateTask(task_pt_sc_21_high, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL);

	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void pt_sc_21_main(unsigned int number_of_low_prio_tasks)
{
	int i;
	
	ROSA_SemaphoreCreateBinary(&sem_21, SEMAPHORE_FREE);
	ROSA_CreateTask(task_pt_sc_21_medium, "med", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	for (i = 0; i < number_of_low_prio_tasks; i++)
	{
		ROSA_CreateTask(task_pt_sc_21_low, "low", SMALL_STACK_SIZE, PRIORITY_3, NULL);
	}
	
	ROSA_StartScheduler();
}

Test pt_sc_21 = {
	.id =			"PT-SC-21",
	.description =	"Take an occupied binary semaphore from a task that is blocking maximum amount of tasks minus one with lower priority and longer wakeUpTimes.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "40",
.function =		pt_sc_21_main};

/*** PT-SC-22 ***/

SemaphoreHandle sem_22;

void task_pt_sc_22(void)
{
	unsigned int ticksBefore, ticksAfter;
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_SemaphoreGive(sem_22);
	
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void pt_sc_22_main(void)
{
	ROSA_SemaphoreCreateBinary(&sem_22, SEMAPHORE_OCCUPIED);
	ROSA_CreateTask(task_pt_sc_22, "task", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_StartScheduler();
}

Test pt_sc_22 = {
	.id =			"PT-SC-22",
	.description =	"Give an occupied binary semaphore from a task.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_22_main};

/*** PT-SC-23 ***/

SemaphoreHandle sem_23;

void task_pt_sc_23_high(void)
{
	ROSA_SemaphoreTake(sem_23, 10000);

	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void task_pt_sc_23_low(void)
{
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_SemaphoreGive(sem_23);
}

void pt_sc_23_main(void)
{
	ROSA_SemaphoreCreateBinary(&sem_23, SEMAPHORE_OCCUPIED);
	
	ROSA_CreateTask(task_pt_sc_23_high, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(task_pt_sc_23_low, "low", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_StartScheduler();
}

Test pt_sc_23 = {
	.id =			"PT-SC-23",
	.description =	"Give an occupied binary semaphore from a task and switch to the task that was blocked by it.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_23_main};

/*** PT-SC-24 ***/

SemaphoreHandle sem_24;

void task_pt_sc_24_low(void)
{
	ROSA_SemaphoreTake(sem_24, 10000);

	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void task_pt_sc_24_high(void)
{
	ROSA_DelayRelative(50);
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_SemaphoreGive(sem_24);
}

void pt_sc_24_main(unsigned int number_of_low_prio_tasks)
{
	int i;
	
	ROSA_SemaphoreCreateBinary(&sem_24, SEMAPHORE_OCCUPIED);
	ROSA_CreateTask(task_pt_sc_24_high, "high", SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	for (i = 0; i < number_of_low_prio_tasks; i++)
	{
		ROSA_CreateTask(task_pt_sc_24_low, "low", SMALL_STACK_SIZE, PRIORITY_4, NULL);
	}
	
	ROSA_StartScheduler();
}

Test pt_sc_24 = {
	.id =			"PT-SC-24",
	.description =	"Give an occupied binary semaphore from a task and switch to the task that was blocked by it when maximum number of tasks were blocked with the same priority.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "41",
.function =		pt_sc_24_main};

/*** PT-SC-25 ***/

void task_pt_sc_25(void)
{
	ticksAfter = TIMER_VALUE();
	send_result(ticksAfter - ticksBefore);
}

void pt_sc_25_main(void)
{
	ROSA_CreateTask(task_pt_sc_25, "task", SMALL_STACK_SIZE, PRIORITY_5, NULL);
	
	TIMER_INIT();
	TIMER_START();
	ticksBefore = TIMER_VALUE();
	
	ROSA_StartScheduler();
}

Test pt_sc_25 = {
	.id =			"PT-SC-25",
	.description =	"Start scheduler.",
	.plan =			"Performance",
	.suite =		"System Calls",
	.type =			TEST_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_sc_25_main};
