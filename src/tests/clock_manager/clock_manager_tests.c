/*
 * clock_manager_tests.c
 *
 * Created: 18.12.2015. 14:20:54
 *  Author: Miha
 */ 

#include <avr32/io.h>
#include "clock_manager_tests.h"
#include "helper_functions.h"
#include "rosa_api_call.h"
#include "rosa_task_private.h"
#include "rosa_ker.h"
#include "tests.h"
#include "rosa_queue_manager.h"
#include "priority_queue.h"

#define TEST_NAME "test"
#define STACK_SIZE 128
#define TEST_PRIORITY 5
#define LOW_PRIORITY 4

//		ROSA_TimerTickCount

void check_tick_count_updates_for_20_ticks(void)
{
	int i;
	ROSA_TickCount starting_ticks = ROSA_TimerTickCount();
	
	for (i = 0; i < 20; i++)
	{
		busy_wait(TICK);
		if (ROSA_TimerTickCount() != starting_ticks + i + 1)
			send_fail();
	}
	send_success();
}

void scm_getTicks_01(void)
{
	send_id("SCM-GETTICKS-01");
	
	ROSA_CreateTask(&check_tick_count_updates_for_20_ticks, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}

void scm_getTicks_02(void)
{
	send_id("SCM-GETTICKS-02");
	
	if (ROSA_TimerTickCount() == 0)
		send_success();
	else
		send_fail();
}

//		ROSA_DelayRelative

int is_delayed = 1;

void delay_for_long_time_and_set_flag(void)
{
	ROSA_DelayRelative(10000000);
	is_delayed = 0;
}

void check_if_other_is_delayed(void)
{
	busy_wait(1000);
	
	if (is_delayed)
		send_success();
	else
		send_fail();
}

void scm_delayRelative_01(void)
{
	send_id("SCM-RELATIVE-01");
	
	ROSA_CreateTask(&delay_for_long_time_and_set_flag, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_CreateTask(&check_if_other_is_delayed, TEST_NAME, STACK_SIZE, LOW_PRIORITY, NULL);
	ROSA_Start();
}

void delay_and_check_for_wake_up(void)
{
	ROSA_DelayRelative(10);
	send_success();
}

void scm_delayRelative_02(void)
{
	send_id("SCM-RELATIVE-02");
	
	ROSA_CreateTask(&delay_and_check_for_wake_up, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}

TaskHandle task_delay_myself;

void delay_for_long_time(void)
{
	ROSA_DelayRelative(10000000);
}

void check_if_other_is_in_delay_queue(void)
{
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task != (Task*) task_delay_myself)
		send_fail();
	if (delayQueue.size != 1)
		send_fail();
		
	send_success();
}

void scm_delayRelative_03(void)
{
	send_id("SCM-RELATIVE-03");
	
	ROSA_CreateTask(&delay_for_long_time, TEST_NAME, STACK_SIZE, TEST_PRIORITY, &task_delay_myself);
	ROSA_CreateTask(&check_if_other_is_in_delay_queue, TEST_NAME, STACK_SIZE, LOW_PRIORITY, NULL);
	ROSA_Start();
}

ROSA_TickCount ticks_before_delay;

void write_current_ticks_and_delay(void)
{
	ticks_before_delay = ROSA_TimerTickCount();
	ROSA_DelayRelative(100);
}

void check_other_tasks_wakeup_time(void)
{
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task->wakeUpTime != ticks_before_delay + 100)
		send_fail();
		
	send_success();
}

void scm_delayRelative_04(void)
{
	send_id("SCM-RELATIVE-04");
	
	ROSA_CreateTask(&write_current_ticks_and_delay, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_CreateTask(&check_other_tasks_wakeup_time, TEST_NAME, STACK_SIZE, LOW_PRIORITY, NULL);
	ROSA_Start();
}

void delay_and_check_for_how_long_have_i_been_delayed(void)
{
	ROSA_TickCount start_time = ROSA_TimerTickCount();
	ROSA_DelayRelative(123);
	
	ROSA_TickCount difference = ROSA_TimerTickCount() - start_time;
	if (difference == 0 || difference == 1)
		send_success();
	
	send_fail();
}

void scm_delayRelative_05(void)
{
	send_id("SCM-RELATIVE-05");
	
	ROSA_CreateTask(&delay_and_check_for_how_long_have_i_been_delayed, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}

void delay_to_past(void)
{
	busy_wait(100 * 1000);
	ROSA_DelayRelative(-1);
	send_success();
}

void scm_delayRelative_06(void)
{
	send_id("SCM-RELATIVE-06");
	
	ROSA_CreateTask(&delay_to_past, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}

void delay_and_check_delay_queue_after(void)
{
	ROSA_DelayRelative(10);
	PriorityQueue DELAYqueue = fetchDELAYqueue();
	if (DELAYqueue.size == 0)
		send_success();
		
	send_fail();
}

void scm_delayRelative_07(void)
{
	send_id("SCM-RELATIVE-07");
	
	ROSA_CreateTask(&delay_and_check_delay_queue_after, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}

TaskHandle task_delay_long, task_delay_medium, task_delay_short;

void delay_long(void)
{
	ROSA_DelayRelative(150);
}

void delay_medium(void)
{
	ROSA_DelayRelative(100);
	
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task != (Task*) task_delay_long)
		send_fail();
	if (delayQueue.size != 1)
		send_fail();
	
	send_success();
}

void delay_short(void)
{
	ROSA_DelayRelative(50);
	
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task != (Task*) task_delay_medium)
		send_fail();
	if (delayQueue.size != 2)
		send_fail();
	
	while (1);
}

void check_state_of_tasks_in_delay_queue(void)
{
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task != (Task*) task_delay_short)
		send_fail();
	if (delayQueue.size != 3)
		send_fail();
	
	while (1);
}

void scm_delayRelative_08(void)
{
	send_id("SCM-RELATIVE-08");
	
	ROSA_CreateTask(&delay_medium, TEST_NAME, STACK_SIZE, TEST_PRIORITY, &task_delay_medium);
	ROSA_CreateTask(&delay_long, TEST_NAME, STACK_SIZE, LOW_PRIORITY, &task_delay_long);
	ROSA_CreateTask(&delay_short, TEST_NAME, STACK_SIZE, LOW_PRIORITY - 1, &task_delay_short);
	ROSA_CreateTask(&check_state_of_tasks_in_delay_queue, TEST_NAME, STACK_SIZE, LOW_PRIORITY - 2, NULL);
	ROSA_Start();
}

void delay_for_zero_ticks(void)
{
	ROSA_TickCount start_time = ROSA_TimerTickCount();
	ROSA_DelayRelative(0);
	
	ROSA_TickCount difference = ROSA_TimerTickCount() - start_time;
	if (difference == 1 || difference == 2)
		send_success();
		
	send_fail();
}

void scm_delayRelative_09(void)
{
	send_id("SCM-RELATIVE-09");
	
	ROSA_CreateTask(&delay_and_check_delay_queue_after, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}

//		ROSA_DelayAbsolute

int is_absolute_delayed = 1;

void delay_absolute_for_long_time_and_set_flag(void)
{
	ROSA_DelayAbsolute(0, -1);
	is_absolute_delayed = 0;
}

void check_if_other_is_absolute_delayed(void)
{
	busy_wait(1000);
	
	if (is_absolute_delayed)
		send_success();
	else
		send_fail();
}

void scm_delayAbsolute_01(void)
{
	send_id("SCM-ABSOLUTE-01");
	
	ROSA_CreateTask(&delay_absolute_for_long_time_and_set_flag, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_CreateTask(&check_if_other_is_absolute_delayed, TEST_NAME, STACK_SIZE, LOW_PRIORITY, NULL);
	ROSA_Start();
}

void delay_absolute_and_check_for_wake_up(void)
{
	ROSA_DelayAbsolute(0, 10);
	send_success();
}

void scm_delayAbsolute_02(void)
{
	send_id("SCM-ABSOLUTE-02");
	
	ROSA_CreateTask(&delay_absolute_and_check_for_wake_up, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}

TaskHandle task_absolute_delay_myself;

void absolute_delay_for_long_time(void)
{
	ROSA_DelayAbsolute(0, -1);
}

void check_if_absolute_delayed_task_is_in_delay_queue(void)
{
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task != (Task*) task_absolute_delay_myself)
		send_fail();
	if (delayQueue.size != 1)
		send_fail();
	
	send_success();
}

void scm_delayAbsolute_03(void)
{
	send_id("SCM-ABSOLUTE-03");
	
	ROSA_CreateTask(&absolute_delay_for_long_time, TEST_NAME, STACK_SIZE, TEST_PRIORITY, &task_absolute_delay_myself);
	ROSA_CreateTask(&check_if_absolute_delayed_task_is_in_delay_queue, TEST_NAME, STACK_SIZE, LOW_PRIORITY, NULL);
	ROSA_Start();
}

ROSA_TickCount ticks_before_absolute_delay;

void write_current_ticks_and_delay_absolute(void)
{
	ticks_before_absolute_delay = ROSA_TimerTickCount();
	ROSA_DelayAbsolute(0, 666666);
}

void check_absolute_delayed_tasks_wakeup_time(void)
{
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task->wakeUpTime != ticks_before_absolute_delay + 666666)
		send_fail();
	
	send_success();
}

void scm_delayAbsolute_04(void)
{
	send_id("SCM-ABSOLUTE-04");
	
	ROSA_CreateTask(&write_current_ticks_and_delay_absolute, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_CreateTask(&check_absolute_delayed_tasks_wakeup_time, TEST_NAME, STACK_SIZE, LOW_PRIORITY, NULL);
	ROSA_Start();
}

void delay_absolute_and_check_for_how_long_have_i_been_delayed(void)
{
	ROSA_DelayAbsolute(0, 1256);
	
	if (ROSA_TimerTickCount() == 1256)
		send_success();
	
	send_fail();
}

void scm_delayAbsolute_05(void)
{
	send_id("SCM-ABSOLUTE-05");
	
	ROSA_CreateTask(&delay_absolute_and_check_for_how_long_have_i_been_delayed, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}

void delay_absolute_and_check_delay_queue_after(void)
{
	ROSA_DelayAbsolute(0,100);
	PriorityQueue DELAYqueue = fetchDELAYqueue();
	if (DELAYqueue.size == 0)
		send_success();
	
	send_fail();
}

void scm_delayAbsolute_06(void)
{
	send_id("SCM-ABSOLUTE-06");
	
	ROSA_CreateTask(&delay_absolute_and_check_delay_queue_after, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}

TaskHandle task_absolute_delay_long, task_absolute_delay_medium, task_absolute_delay_short;

void delay_absolute_long(void)
{
	ROSA_DelayRelative(150);
}

void delay_absolute_medium(void)
{
	ROSA_DelayRelative(100);
	
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task != (Task*) task_absolute_delay_long)
		send_fail();
	if (delayQueue.size != 1)
		send_fail();
	
	send_success();
}

void delay_absolute_short(void)
{
	ROSA_DelayRelative(50);
	
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task != (Task*) task_absolute_delay_medium)
		send_fail();
	if (delayQueue.size != 2)
		send_fail();
	
	while(1);
}

void check_if_3_tasks_in_delay_queue(void)
{
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task != (Task*) task_absolute_delay_short)
		send_fail();
	if (delayQueue.size != 3)
		send_fail();
	
	while(1);
}

void scm_delayAbsolute_07(void)
{
	send_id("SCM-ABSOLUTE-07");
	
	ROSA_CreateTask(&delay_medium, TEST_NAME, STACK_SIZE, TEST_PRIORITY, &task_absolute_delay_medium);
	ROSA_CreateTask(&delay_long, TEST_NAME, STACK_SIZE, LOW_PRIORITY, &task_absolute_delay_long);
	ROSA_CreateTask(&delay_short, TEST_NAME, STACK_SIZE, LOW_PRIORITY - 1, &task_absolute_delay_short);
	ROSA_CreateTask(&check_other_tasks_wakeup_time, TEST_NAME, STACK_SIZE, LOW_PRIORITY - 2, NULL);
	ROSA_Start();	
}

void delay_absolute_to_past(void)
{
	busy_wait(100 * 1000);
	ROSA_TickCount start_time = ROSA_TimerTickCount();
	ROSA_DelayAbsolute(0,50);
	
	ROSA_TickCount difference = ROSA_TimerTickCount() - start_time;
	if (difference == 1 || difference == 2)
		send_success();
		
	send_fail();
}

void scm_delayAbsolute_08(void)
{
	send_id("SCM-ABSOLUTE-08");
	
	ROSA_CreateTask(&delay_absolute_to_past, TEST_NAME, STACK_SIZE, TEST_PRIORITY, NULL);
	ROSA_Start();
}
