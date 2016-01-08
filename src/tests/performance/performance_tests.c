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