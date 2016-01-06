/*
 * scheduler_tests.c
 *
 * Created: 03/01/2016 13:22:18
 *  Author: Serena Santi
 */ 

#include <avr32/io.h>
#include "scheduler_tests.h"
#include "helper_functions.h"
#include "rosa_api_call.h"
#include "rosa_ker.h"
#include "tests.h"
#include "rosa_scheduler_private.h"

#define TEST_PTR (void*) 42
#define TASK_NAME "test"
#define STACK_SIZE 1000
#define TEST_PRIORITY 6

TaskHandle task_handle_ss1;

void ss1_task()
{
	if(getCRT() == task_handle_ss1)
		send_success();
	else
		send_fail();
}

void s_schedule_01()
{
	/*ROSA_Start should start scheduler and put a task in currently running state*/
	
	char task_name[5] = TASK_NAME;
	
	send_id("S-SCHEDULE-01");
	
	ROSA_CreateTask(ss1_task, task_name, STACK_SIZE, TEST_PRIORITY, &task_handle_ss1);

	ROSA_Start();
	
	send_fail();
}

void ss2_task()
{
	send_success();
}

void s_schedule_02()
{
	/*ROSA_Start should start scheduler and check if the currently running task is newly created*/
	
	TaskHandle task_handle;
	char task_name[5] = TASK_NAME;
	
	send_id("S-SCHEDULE-02");
	
	ROSA_CreateTask(ss2_task, task_name, STACK_SIZE, TEST_PRIORITY, &task_handle);
	
	ROSA_Start();
	
	send_fail();
}

void ss3_low()
{
	send_fail();
}

void ss3_high()
{
	busy_wait(100);
	send_success();
}

void s_schedule_03()
{
	/*ROSA_Start should start scheduler and check if lower priority task of two created tasks will never execute*/
	
	TaskHandle low;
	TaskHandle high;
	
	send_id("S-SCHEDULE-03");
	
	ROSA_CreateTask(ss3_low, "low", STACK_SIZE, 4, &low);
	ROSA_CreateTask(ss3_high, "high", STACK_SIZE, 5, &high);
	
	ROSA_Start();
	
	send_fail();
}