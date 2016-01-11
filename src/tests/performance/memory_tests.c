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

void task1()
{
	ROSA_CreateTask(TEST_PTR, "test", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_01_main()
{
	TestStack_ROSA_CreateTask(task1, "t1", SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test pt_mt_sc_01 = {
	.id =			"PT-MT-SC-01",
	.description =	"Measure Memory usage of Create Task.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_01_main};





void task2()
{
	ROSA_CreateTask(TEST_PTR, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_3, NULL);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_02_main(unsigned int maximum_tasks)
{
	int i;
	for (i = 0; i < (maximum_tasks-2) ; i++)
	{
		ROSA_CreateTask(TEST_PTR, "low", 0, PRIORITY_3, NULL);
	}
	TestStack_ROSA_CreateTask(task2, "med", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_Start();
}

Test pt_mt_sc_02 = {
	.id =			"PT-MT-SC-02",
	.description =	"Measure Memory usage of Create Task depending on the number of tasks in the ready queue.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "2",
	.end_parameter = "42",
.function =		pt_mt_sc_02_main};








void task3()
{
	ROSA_CreateCyclicTask(TEST_PTR, "t3", SUPER_SMALL_STACK_SIZE, PRIORITY_4, 42, 42, NULL);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_03_main()
{
	TestStack_ROSA_CreateTask(task3, "t1", SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test pt_mt_sc_03 = {
	.id =			"PT-MT-SC-03",
	.description =	"Measure Memory usage of Create Cyclic Task.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_03_main};





void task4()
{
	ROSA_CreateCyclicTask(TEST_PTR, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_3, 5, 6, NULL);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_04_main(unsigned int maximum_tasks)
{
	int i;
	for (i = 0; i < (maximum_tasks-2) ; i++)
	{
		ROSA_CreateTask(TEST_PTR, "low", 0, PRIORITY_3, NULL);
	}
	TestStack_ROSA_CreateTask(task4, "med", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_Start();
}

Test pt_mt_sc_04 = {
	.id =			"PT-MT-SC-04",
	.description =	"Measure Memory usage of Create Cyclic Task depending on the number of tasks in the ready queue.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "2",
	.end_parameter = "42",
.function =		pt_mt_sc_04_main};






/*** PT-MT-SC-05 ***/

TaskHandle task_handle_5;

void task_pt_mt_sc_05_high()
{
	ROSA_TerminateTask();
}

void task_pt_mt_sc_05_low()
{
	send_result(get_max_stack(task_handle_5));
}


void pt_mt_sc_05_main()
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_05_low, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_05_high, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_5);
	ROSA_Start();
}

Test pt_mt_sc_05 = {
	.id =            "PT-MT-SC-05",
	.description =    "Measure memory usage of Terminate Task.",
	.plan =            "Performance Memory",
	.suite =			"System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_05_main};




void task_pt_mt_sc_06()
{
	ROSA_TimerTickCount();
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_06_main()
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_06, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test pt_mt_sc_06 = {
	.id =            "PT-MT-SC-06",
	.description =    "Measure Get Ticks.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_06_main};




TaskHandle task_handle_7;

void task_pt_mt_sc_07_low(void)
{
	send_result(get_max_stack(task_handle_7));
}

void task_pt_mt_sc_07_high(void)
{
	ROSA_DelayAbsolute(0, 20);
}

void pt_mt_sc_07_main(void)
{
	ROSA_CreateTask(task_pt_mt_sc_07_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_07_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_7);

	ROSA_StartScheduler();
}

Test pt_mt_sc_07 = {
	.id =            "PT-MT-SC-07",
	.description =    "Measure memory usage of Absolute Delay.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_07_main};