/*
 * scheduler_tests.c
 *
 * Created: 03/01/2016 13:22:18
 *  Author: Serena Santi
 */ 

#include <avr32/io.h>
#include "scheduler_manager_tests.h"
#include "helper_functions.h"
#include "rosa_api_call.h"
#include "rosa_ker.h"
#include "tests.h"
#include "rosa_scheduler_private.h"

#define TEST_PTR (void*) 42
#define TASK_NAME "test"
#define STACK_SIZE SMALL_STACK_SIZE
#define TEST_PRIORITY 6

/*************** TEST SUITE: Scheduler start ***************/

/*** S-SCHEDULER-01 ***/

TaskHandle task_handle_ss1;

void ss1_task()
{
	if(getCRT() == task_handle_ss1)
		send_success();
	else
		send_fail();
}

void s_schedule_01_main()
{	
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(ss1_task, task_name, STACK_SIZE, TEST_PRIORITY, &task_handle_ss1);

	ROSA_Start();
	
	send_fail();
}

Test s_schedule_01 = {
	.id =			"S-SCHEDULE-01",
	.description =	"ROSA_Start should start scheduler and put task in a currently running state.",
	.plan =			"Scheduler Manager",
	.suite =		"Scheduler Start",
	.type =			TEST_FUNCTIONAL,
.function =		s_schedule_01_main};

/*** S-SCHEDULER-02 ***/

void ss2_task()
{
	send_success();
}

void s_schedule_02_main()
{
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(ss2_task, task_name, STACK_SIZE, TEST_PRIORITY, &task_handle);
	
	ROSA_Start();
	
	send_fail();
}

Test s_schedule_02 = {
	.id =			"S-SCHEDULE-02",
	.description =	"ROSA_Start should start scheduler and check if the currently running task is newly created.",
	.plan =			"Scheduler Manager",
	.suite =		"Scheduler Start",
	.type =			TEST_FUNCTIONAL,
.function =		s_schedule_02_main};

/*** S-SCHEDULER-03 ***/

void ss3_low()
{
	send_fail();
}

void ss3_high()
{
	busy_wait(100);
	send_success();
}

void s_schedule_03_main()
{
	TaskHandle low;
	TaskHandle high;
	
	ROSA_CreateTask(ss3_low, "low", STACK_SIZE, 4, &low);
	ROSA_CreateTask(ss3_high, "high", STACK_SIZE, 5, &high);
	
	ROSA_Start();
	
	send_fail();
}

Test s_schedule_03 = {
	.id =			"S-SCHEDULE-03",
	.description =	"ROSA_Start should start scheduler and check if lower priority task of two created tasks will never execute.",
	.plan =			"Scheduler Manager",
	.suite =		"Scheduler Start",
	.type =			TEST_FUNCTIONAL,
.function =		s_schedule_03_main};