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
#define PRIORITY_5 5
#define PRIORITY_4 4
#define PRIORITY_3 3
#define TEST_PTR (void*) 42

#define TEST_PERIOD 25
#define TEST_DEADLINE 20

/*************** TEST SUITE: Performance Memory testing of system calls ***************/

TaskHandle null_replacer_handle;

/*** PT-MT-SC-01 ***/
void pt_mt_sc_01_main()
{
	TaskHandle task_handle;
	
	TestStack_ROSA_CreateTask(TEST_PTR, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	send_result(get_max_stack(getCRT()));

}

Test pt_mt_sc_01 = {
	.id =			"PT-MT-SC-01",
	.description =	"Create a task before the scheduler has started.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_01_main};

/*** PT-MT-SC-03 ***/

void task_pt_mt_sc_03()
{
	TaskHandle task_handle2;
	
	TestStack_ROSA_CreateTask(TEST_PTR, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_4,  &task_handle2);

	send_result(get_max_stack(getCRT()));

}

void pt_mt_sc_03_main()
{
	TaskHandle task_handle;
	
	TestStack_ROSA_CreateTask(task_pt_mt_sc_03, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	
	ROSA_Start();
}

Test pt_mt_sc_03 = {
	.id =			"PT-MT-SC-03",
	.description =	"Create a task when the ready queue is empty and the scheduler has started.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_03_main};

/*** PT-MT-SC-04 ***/

void task_pt_mt_sc_04()
{
	TaskHandle task_handle2;

	TestStack_ROSA_CreateTask(TEST_PTR, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_4, &task_handle2);

	send_result(get_max_stack(getCRT()));

}

void pt_mt_sc_04_main(unsigned int maximum_tasks)
{
	TaskHandle task_handle;
	int i;
	for (i = 0; i < (maximum_tasks-1) ; i++)
	{
		TestStack_ROSA_CreateTask(task_pt_mt_sc_04, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	}
	
	ROSA_Start();
}

Test pt_mt_sc_04 = {
	.id =			"PT-MT-SC-04",
	.description =	"Create a task after the ready queue has only one free slot  .",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "2",
	.end_parameter = "10",
.function =		pt_mt_sc_04_main};



/*** PT-MT-SC-05 ***/

void task_pt_mt_sc_05()
{
	TaskHandle task_handle2;

	TestStack_ROSA_CreateCyclicTask(TEST_PTR, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_4, TEST_PERIOD, TEST_DEADLINE, &task_handle2);

	send_result(get_max_stack(getCRT()));

}

void pt_mt_sc_05_main()
{
	TaskHandle task_handle;
	
	TestStack_ROSA_CreateCyclicTask(task_pt_mt_sc_05, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task_handle);
	
	ROSA_Start();
}

Test pt_mt_sc_05 = {
	.id =			"PT-MT-SC-05",
	.description =	"Create a cyclic task when the ready queue is empty and the scheduler has started.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_05_main};


/*** PT-MT-SC-06 ***/

void task_pt_mt_sc_06()
{
	TaskHandle task_handle2;

	TestStack_ROSA_CreateCyclicTask(TEST_PTR, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_4,TEST_PERIOD, TEST_DEADLINE, &task_handle2);

	send_result(get_max_stack(getCRT()));

}

void pt_mt_sc_06_main(unsigned int maximum_tasks)
{
	TaskHandle task_handle;
	int i;
	for (i = 0; i < (maximum_tasks-1) ; i++)
	{
		TestStack_ROSA_CreateCyclicTask(task_pt_mt_sc_06, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, TEST_PERIOD, TEST_DEADLINE, &task_handle);
	}
	
	ROSA_Start();
}

Test pt_mt_sc_06 = {
	.id =			"PT-MT-SC-06",
	.description =	"Create a cyclic task after the ready queue has only one free slot.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "2",
	.end_parameter = "10",
.function =		pt_mt_sc_06_main};

/*** PT-MT-SC-07 ***/

TaskHandle task_handle_7;
	
void task_pt_mt_sc_07_high()
{
	ROSA_TerminateTask();
}

void task_pt_mt_sc_07_low()
{
	send_result(get_max_stack(task_handle_7));
}


void pt_mt_sc_07_main()
{
	TaskHandle task_handle2;
	
	TestStack_ROSA_CreateTask(task_pt_mt_sc_07_low, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &task_handle_7);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_07_high, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle2);
	
	ROSA_Start();
}

Test pt_mt_sc_07 = {
	.id =			"PT-MT-SC-07",
	.description =	"Terminate currently running task and switch to a new task.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_07_main};


/*** PT-MT-SC-08 ***/


void task_pt_mt_sc_08()
{
	ROSA_TimerTickCount();
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_08_main()
{
	TaskHandle task_handle;
	
	TestStack_ROSA_CreateTask(task_pt_mt_sc_08, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle);
	ROSA_Start();
}

Test pt_mt_sc_08 = {
	.id =			"PT-MT-SC-08",
	.description =	"Get current ticks from a task. This is fixed time.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_08_main};

void dummy_main(unsigned int param)
{
	send_result(param*42);
}

Test dummy_test = {
	.id =			"DUMMY-42",
	.description =	"Testing the Testing tool. Inception. Deep stuff!",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "1",
	.end_parameter = "10",
.function =		dummy_main};

/*** PT-MT-SC-09 ***/

TaskHandle task_handle_9;

void task_pt_mt_sc_09_low(void)
{
	send_result(get_max_stack(task_handle_9));
}

void task_pt_mt_sc_09_high(void)
{
	ROSA_DelayAbsolute(0, 20);
}

void pt_mt_sc_09_main(void)
{
	ROSA_CreateTask(task_pt_mt_sc_09_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_09_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_9);

	ROSA_StartScheduler();
}

Test pt_mt_sc_09 = {
	.id =			"PT-MT-SC-09",
	.description =	"Measure time for executing absolute delay and switch to the next highest priority task when the delay queue is empty.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_09_main};

/*** PT-MT-SC-10 ***/

TaskHandle task_handle_10;

void task_pt_mt_sc_10_high(void)
{
	ROSA_DelayAbsolute(0, 30);
}

void task_pt_mt_sc_10_medium(void)
{
	ROSA_DelayAbsolute(0, 10000);
}

void task_pt_mt_sc_10_low(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_10_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_10);
	send_result(get_max_stack(task_handle_10));
}

void pt_mt_sc_10_main(unsigned int number_of_medium_prio_tasks)
{
	int i;
	TestStack_ROSA_CreateTask(task_pt_mt_sc_10_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_3, &null_replacer_handle);
	
	for (i = 0; i < number_of_medium_prio_tasks; i++)
	{
		TestStack_ROSA_CreateTask(task_pt_mt_sc_10_medium, "med", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	}
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_10 = {
	.id =			"PT-MT-SC-10",
	.description =	"Measure time for executing absolute delay and switch to the next highest priority task when the delay queue has only one free slot and all tasks' wake up time is after the newly delayed one.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "10",
.function =		pt_mt_sc_10_main};

/*** PT-MT-SC-11 ***/
TaskHandle task_handle_11;

void task_pt_mt_sc_11_low(void)
{
	send_result(get_max_stack(task_handle_11));
}

void task_pt_mt_sc_11_high(void)
{
	ROSA_DelayRelative(20);
}

void pt_mt_sc_11_main(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_11_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_11_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_11);

	ROSA_StartScheduler();
}

Test pt_mt_sc_11 = {
	.id =			"PT-MT-SC-11",
	.description =	"Measure time for executing relative delay and switch to the next highest priority task when the delay queue is empty.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_11_main};

/*** PT-MT-SC-12 ***/
TaskHandle task_handle_12;

void task_pt_mt_sc_12_high(void)
{
	ROSA_DelayRelative(30);
}

void task_pt_mt_sc_12_medium(void)
{
	ROSA_DelayRelative(10000);
}

void task_pt_mt_sc_12_low(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_12_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_12);

	send_result(get_max_stack(task_handle_12));
}

void pt_mt_sc_12_main(unsigned int number_of_medium_prio_tasks)
{
	int i;
	TestStack_ROSA_CreateTask(task_pt_mt_sc_12_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_3, &null_replacer_handle);
	
	for (i = 0; i < number_of_medium_prio_tasks; i++)
	{
		TestStack_ROSA_CreateTask(task_pt_mt_sc_12_medium, "med", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	}
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_12 = {
	.id =			"PT-MT-SC-12",
	.description =	"Measure time for executing relative delay and switch to the next highest priority task when the delay queue has only one free slot and all tasks wake up time is after the newly delayed one.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "10",
.function =		pt_mt_sc_12_main};

/*** PT-MT-SC-13 ***/

TaskHandle task_handle_13;

void task_pt_mt_sc_13_low(void)
{
	send_result(get_max_stack(task_handle_13));
}

void task_pt_mt_sc_13_high(void)
{
	ROSA_EndCycle();
}

void pt_mt_sc_13_main(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_13_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	TestStack_ROSA_CreateCyclicTask(task_pt_mt_sc_13_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, 30, 30, &task_handle_13);

	ROSA_StartScheduler();
}

Test pt_mt_sc_13 = {
	.id =			"PT-MT-SC-13",
	.description =	"Measure time for executing end cycle and switch to the next highest priority task when the delay queue is empty.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_13_main};

/*** PT-MT-SC-14 ***/
TaskHandle task_handle_14;

void task_pt_mt_sc_14_high(void)
{
	ROSA_EndCycle();
}

void task_pt_mt_sc_14_medium(void)
{
	ROSA_DelayRelative(10000);
}

void task_pt_mt_sc_14_low(void)
{
	TestStack_ROSA_CreateCyclicTask(task_pt_mt_sc_14_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, 30, 30, &task_handle_14);
	
	send_result(get_max_stack(task_handle_14));
}

void pt_mt_sc_14_main(unsigned int number_of_medium_prio_tasks)
{
	int i;
	TestStack_ROSA_CreateTask(task_pt_mt_sc_14_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_3, &null_replacer_handle);
	
	for (i = 0; i < number_of_medium_prio_tasks; i++)
	{
		TestStack_ROSA_CreateTask(task_pt_mt_sc_14_medium, "med", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	}
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_14 = {
	.id =			"PT-MT-SC-14",
	.description =	"Measure time for executing end cycle and switch to the next highest priority task when the delay queue has only one free slot and all tasks' wake up time is after the newly delayed one.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "10",
.function =		pt_mt_sc_14_main};

/*** PT-MT-SC-15 ***/

TaskHandle task_handle_15;

void task_pt_mt_sc_15_high(void)
{
	send_result(get_max_stack(task_handle_15));
}

void task_pt_mt_sc_15_low(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_15_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &null_replacer_handle);
}

void pt_mt_sc_15_main(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_15_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &task_handle_15);
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_15 = {
	.id =			"PT-MT-SC-15",
	.description =	"Creating new higher priority task and switch to it after the scheduler has been started.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_15_main};

/*** PT-MT-SC-16 ***/

void pt_mt_sc_16_main(void)
{
	ROSA_SemaphoreCreateBinary(&null_replacer_handle, SEMAPHORE_FREE);
	send_result(get_max_stack(getCRT()));
}

Test pt_mt_sc_16 = {
	.id =			"PT-MT-SC-16",
	.description =	"Create a semaphore.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_16_main};

/*** PT-MT-SC-17 ***/

void task_pt_mt_sc_17(void)
{
	ROSA_SemaphoreCreateBinary(&null_replacer_handle, SEMAPHORE_FREE);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_17_main(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_17, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_17 = {
	.id =			"PT-MT-SC-17",
	.description =	"Create a semaphore from a task.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_17_main};


/*** PT-MT-SC-18 ***/

SemaphoreHandle sem;
	
void neki_task()
{
	ROSA_SemaphoreDelete(sem);
	send_result(get_max_stack(getCRT()));
}
void pt_mt_sc_18_main(void)
{
	ROSA_SemaphoreCreateBinary(&sem, SEMAPHORE_FREE);
	ROSA_CreateTask(neki_task, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &null_replacer_handle);
	ROSA_Start();
}

Test pt_mt_sc_18 = {
	.id =			"PT-MT-SC-18",
	.description =	"Delete a semaphore from a currently running task.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_18_main};

/*** PT-MT-SC-20 ***/

SemaphoreHandle sem_20;

void task_pt_mt_sc_20(void)
{
	ROSA_SemaphoreTake(sem_20, 0);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_20_main(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_20, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	ROSA_SemaphoreCreateBinary(&sem_20, SEMAPHORE_FREE);
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_20 = {
	.id =			"PT-MT-SC-20",
	.description =	"Take a free binary semaphore from a task.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_20_main};

/*** PT-MT-SC-21 ***/

SemaphoreHandle sem_21;
TaskHandle task_handle_21;

void task_pt_mt_sc_21_low(void)
{
	ROSA_SemaphoreTake(sem_21, 10000);
}

void task_pt_mt_sc_21_high(void)
{
	ROSA_SemaphoreTake(sem_21, 100);
}

void task_pt_mt_sc_21_medium(void)
{
	ROSA_SemaphoreTake(sem_21, 0);
	ROSA_DelayRelative(200);
	
	TestStack_ROSA_CreateTask(task_pt_mt_sc_21_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_21);

	send_result(get_max_stack(task_handle_21));
}

void pt_mt_sc_21_main(unsigned int number_of_low_prio_tasks)
{
	int i;
	
	ROSA_SemaphoreCreateBinary(&sem_21, SEMAPHORE_FREE);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_21_medium, "med", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	
	for (i = 0; i < number_of_low_prio_tasks; i++)
	{
		TestStack_ROSA_CreateTask(task_pt_mt_sc_21_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_3, &null_replacer_handle);
	}
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_21 = {
	.id =			"PT-MT-SC-21",
	.description =	"Take an occupied binary semaphore from a task that is blocking maximum amount of tasks minus one with lower priority and longer wakeUpTimes.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "10",
.function =		pt_mt_sc_21_main};

/*** PT-MT-SC-22 ***/

SemaphoreHandle sem_22;

void task_pt_mt_sc_22(void)
{
	ROSA_SemaphoreGive(sem_22);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_22_main(void)
{
	ROSA_SemaphoreCreateBinary(&sem_22, SEMAPHORE_OCCUPIED);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_22, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_22 = {
	.id =			"PT-MT-SC-22",
	.description =	"Give an occupied binary semaphore from a task.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_22_main};

/*** PT-MT-SC-23 ***/

SemaphoreHandle sem_23;
TaskHandle task_handle_23;

void task_pt_mt_sc_23_high(void)
{
	ROSA_SemaphoreTake(sem_23, 10000);
	send_result(get_max_stack(task_handle_23));
}

void task_pt_mt_sc_23_low(void)
{
	ROSA_SemaphoreGive(sem_23);
}

void pt_mt_sc_23_main(void)
{
	ROSA_SemaphoreCreateBinary(&sem_23, SEMAPHORE_OCCUPIED);
	
	TestStack_ROSA_CreateTask(task_pt_mt_sc_23_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &null_replacer_handle);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_23_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &task_handle_23);
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_23 = {
	.id =			"PT-MT-SC-23",
	.description =	"Give an occupied binary semaphore from a task and switch to the task that was blocked by it.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_23_main};

/*** PT-MT-SC-24 ***/

SemaphoreHandle sem_24;
TaskHandle task_handle_24;

void task_pt_mt_sc_24_low(void)
{
	ROSA_SemaphoreTake(sem_24, 10000);
	send_result(get_max_stack(task_handle_24));
}

void task_pt_mt_sc_24_high(void)
{
	ROSA_DelayRelative(50);
	ROSA_SemaphoreGive(sem_24);
	ROSA_DelayRelative(50);
}

void pt_mt_sc_24_main(unsigned int number_of_low_prio_tasks)
{
	int i;
	
	ROSA_SemaphoreCreateBinary(&sem_24, SEMAPHORE_OCCUPIED);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_24_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_24);
	
	for (i = 0; i < number_of_low_prio_tasks; i++)
	{
		TestStack_ROSA_CreateTask(task_pt_mt_sc_24_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &null_replacer_handle);
	}
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_24 = {
	.id =			"PT-MT-SC-24",
	.description =	"Give an occupied binary semaphore from a task and switch to the task that was blocked by it when maximum number of tasks were blocked with the same priority.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "1",
	.end_parameter = "10",
.function =		pt_mt_sc_24_main};
