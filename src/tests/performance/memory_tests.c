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

/*** PT-MT-SC-01 ***/
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