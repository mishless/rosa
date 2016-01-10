/*
 * clock_manager_tests.c
 *
 * Created: 18.12.2015. 14:20:54
 *  Author: Miha
 */ 

#include <avr32/io.h>
#include "helper_functions.h"
#include "rosa_api_call.h"
#include "rosa_task_private.h"
#include "clock_manager_tests.h"
#include "rosa_ker.h"
#include "tests.h"
#include "rosa_queue_manager.h"
#include "priority_queue.h"

#define TEST_NAME "test"
#define LARGE_STACK_SIZE 2048

#define PRIORITY_6 6
#define PRIORITY_5 5
#define PRIORITY_4 4
#define PRIORITY_3 3

/*************** TEST SUITE: Get Ticks ***************/

/** SCM-GETTICKS-01 **/

void check_tick_count_updates_for_20_ticks(void)
{
	int i;
	ROSA_TickCount difference, current_ticks, ticks_before = ROSA_TimerTickCount();
	
	for (i = 0; i < 20; i++)
	{
		busy_wait(TICK);
		current_ticks = ROSA_TimerTickCount();
		difference = current_ticks - ticks_before;
		ticks_before = current_ticks;
		if (difference != 0 && difference != 1 && difference != 2) // busy_wait is very inexact
			send_fail();
	}
	send_success();
}

void scm_getTicks_01_main(void)
{
	/* ROSA_TimerTickCount successive calls should return larger numbers 
	compared to previous calls. */
	ROSA_CreateTask(&check_tick_count_updates_for_20_ticks, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_getTicks_01 = {
	.id =			"SCM-GETTICKS-01",
	.description =	"ROSA_TimerTickCount successive calls should return larger numbers compared to previous calls.",
	.plan =			"System Clock Manager",
	.suite =		"Get Ticks",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_getTicks_01_main
};

/** SCM-GETTICKS-02 **/

void scm_getTicks_02_main(void)
{
	/* ROSA_TimerTickCount calls before starting scheduler should always return zero. */

	if (ROSA_TimerTickCount() == 0)
		send_success();
	else
		send_fail();
}

Test scm_getTicks_02 = {
	.id =			"SCM-GETTICKS-02",
	.description =	"ROSA_TimerTickCount calls before starting scheduler should always return zero.",
	.plan =			"System Clock Manager",
	.suite =		"Get Ticks",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_getTicks_02_main
};

/*************** TEST SUITE: Delay Relative ***************/

/** SCM-RELATIVE-01 **/

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

void scm_delayRelative_01_main(void)
{
	/* ROSA_ DelayRelative should delay a task for a long time and task should stop working for a short time. */
	
	ROSA_CreateTask(&delay_for_long_time_and_set_flag, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(&check_if_other_is_delayed, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();
}

Test scm_delayRelative_01 = {
	.id =			"SCM-RELATIVE-01",
	.description =	"ROSA_ DelayRelative should delay a task for a long time and task should stop working for a short time.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Relative",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayRelative_01_main
};

/** SCM-RELATIVE-02 **/

void relativeDelayAndSendSuccess(void)
{
	ROSA_DelayRelative(10);                              
	send_success();
}

void scm_delayRelative_02_main(void)
{
	ROSA_CreateTask(&relativeDelayAndSendSuccess, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_delayRelative_02 = {
	.id =			"SCM-RELATIVE-02",
	.description =	"Task calling ROSA_DelayRelative should wake up.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Relative",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayRelative_02_main
};

/** SCM-RELATIVE-03 **/

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

void scm_delayRelative_03_main(void)
{
	/* Task which calls ROSA_DelayRelative should be in DELAY queue. */
	ROSA_CreateTask(&delay_for_long_time, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, &task_delay_myself);
	ROSA_CreateTask(&check_if_other_is_in_delay_queue, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();
}

Test scm_delayRelative_03 = {
.id =			"SCM-RELATIVE-03",
.description =	"Task which calls ROSA_DelayRelative should be in DELAY queue.",
.plan =			"System Clock Manager",
.suite =		"Delay Relative",
.type =			TEST_FUNCTIONAL,
.function =		scm_delayRelative_03_main
};

/** SCM-RELATIVE-04 **/

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

void scm_delayRelative_04_main(void)
{
	/* Task which calls ROSA_DelayRelative should have correct wake up time. */

	ROSA_CreateTask(&write_current_ticks_and_delay, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(&check_other_tasks_wakeup_time, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();
}

Test scm_delayRelative_04 = {
	.id =			"SCM-RELATIVE-04",
	.description =	"Task which calls ROSA_DelayRelative should have correct wake up time.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Relative",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayRelative_04_main
};

/** SCM-RELATIVE-05 **/

void delay_and_check_for_how_long_have_i_been_delayed(void)
{
	ROSA_TickCount start_time = ROSA_TimerTickCount();
	ROSA_DelayRelative(123);
	
	ROSA_TickCount difference = ROSA_TimerTickCount() - start_time;
	if (difference == 123 || difference == 124)
		send_success();
	
	send_fail();
}

void scm_delayRelative_05_main(void)
{
	/** Task which calls ROSA_DelayRelative should wake up at correct time. **/
	ROSA_CreateTask(&delay_and_check_for_how_long_have_i_been_delayed, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_delayRelative_05 = {
	.id =			"SCM-RELATIVE-05",
	.description =	"Task which calls ROSA_DelayRelative should wake up at correct time.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Relative",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayRelative_05_main
};

/** SCM-RELATIVE-06 **/

void delay_to_past(void)
{
	busy_wait(100 * TICK);
	ROSA_DelayRelative(-1);
	send_success();
}

void sendFailTask(void) {
	busy_wait(3);
	send_fail();
}

void scm_delayRelative_06_main(void)
{
	/* Task which calls ROSA_DelayRelative for invalid amount of time might wake up immediately. */

	ROSA_CreateTask(&delay_to_past, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(&sendFailTask, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();
}

Test scm_delayRelative_06 = {
	.id =			"SCM-RELATIVE-06",
	.description =	"Task which calls ROSA_DelayRelative for invalid amount of time might wake up immediately.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Relative",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayRelative_06_main
};

/** SCM-RELATIVE-07 **/

void delay_and_check_delay_queue_after(void)
{
	ROSA_DelayRelative(10);
	PriorityQueue DELAYqueue = fetchDELAYqueue();
	if (DELAYqueue.size == 0)
		send_success();
	send_fail();
}

void scm_delayRelative_07_main(void)
{
	/* Then task wakes up, it shouldn't be in DELAY queue. */

	ROSA_CreateTask(&delay_and_check_delay_queue_after, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_delayRelative_07 = {
	.id =			"SCM-RELATIVE-07",
	.description =	"Then task wakes up, it shouldn't be in DELAY queue.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Relative",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayRelative_07_main
};

/** SCM-RELATIVE-08 **/

TaskHandle task_delay_long, task_delay_medium, task_delay_short;

void delay_long(void)
{
	ROSA_DelayRelative(150);
}

void delay_medium(void)
{
	ROSA_DelayRelative(100);
}

void delay_short(void)
{
	ROSA_DelayRelative(50);
}

void check_state_of_tasks_in_delay_queue(void)
{
	Task* task = NULL;
	task = getFromDELAYqueue();
	if (task != (Task*) task_delay_short)
	{
		send_fail();
	}
	task = getFromDELAYqueue();
	if (task != (Task*) task_delay_medium)
	{
		send_fail();
	}
	task = getFromDELAYqueue();
	if (task != (Task*) task_delay_long)
	{
		send_fail();
	}
	send_success();
}

void scm_delayRelative_08_main(void)
{
	/* Delay 3 tasks and check the sorting in the DELAY queue. */

	ROSA_CreateTask(&delay_long, TEST_NAME, LARGE_STACK_SIZE, PRIORITY_5, &task_delay_long);
	ROSA_CreateTask(&delay_medium, TEST_NAME, LARGE_STACK_SIZE, PRIORITY_6, &task_delay_medium);
	ROSA_CreateTask(&delay_short, TEST_NAME, LARGE_STACK_SIZE, PRIORITY_4, &task_delay_short);
	ROSA_CreateTask(&check_state_of_tasks_in_delay_queue, TEST_NAME, LARGE_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_Start();
}

Test scm_delayRelative_08 = {
	.id =			"SCM-RELATIVE-08",
	.description =	"Delay 3 tasks and check the sorting in the DELAY queue.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Relative",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayRelative_08_main
};

/** SCM-RELATIVE-09 **/

void delay_for_zero_ticks(void)
{
	ROSA_TickCount start_time = ROSA_TimerTickCount();
	ROSA_DelayRelative(0);
	ROSA_TickCount difference = ROSA_TimerTickCount() - start_time;
	if (difference == 1 || difference == 2)
		send_success();
	send_fail();
}

void scm_delayRelative_09_main (void)
{
	/* ROSA_DelayRelative with argument zero should wake up task on next tick. */

	ROSA_CreateTask(&delay_and_check_delay_queue_after, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_delayRelative_09 = {
	.id =			"SCM-RELATIVE-09",
	.description =	"ROSA DelayRelative with argument zero should wake up task on next tick.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Relative",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayRelative_09_main
};

/*************** TEST SUITE: Delay Absolute ***************/

/** SCM-ABSOULUTE-01 **/

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

void scm_delayAbsolute_01_main(void)
{
	/* ROSA_DelayAbsolute should delay a task for a long time and task should stop working for a short time. */

	ROSA_CreateTask(&delay_absolute_for_long_time_and_set_flag, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(&check_if_other_is_absolute_delayed, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();
}

Test scm_delayAbsolute_01 = {
	.id =			"SCM-ABSOULUTE-01",
	.description =	"ROSA-DelayAbsolute should delay a task for a long time and task should stop working for a short time.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayAbsolute_01_main
};

/** SCM-ABSOULUTE-02 **/

void delay_absolute_and_check_for_wake_up(void)
{
	ROSA_DelayAbsolute(0, 10);
	send_success();
}

void scm_delayAbsolute_02_main(void)
{
	/* Task calling ROSA_DelayAbsolute should wake up. */

	ROSA_CreateTask(&delay_absolute_and_check_for_wake_up, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_delayAbsolute_02 = {
	.id =			"SCM-ABSOULUTE-02",
	.description =	"Task calling ROSA_DelayAbsolute should wake up.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayAbsolute_02_main
};

/** SCM-ABSOULUTE-03 **/

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

void scm_delayAbsolute_03_main(void)
{
	/* Task which calls ROSA_DelayAbsolute should be in DELAY queue. */

	ROSA_CreateTask(&absolute_delay_for_long_time, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, &task_absolute_delay_myself);
	ROSA_CreateTask(&check_if_absolute_delayed_task_is_in_delay_queue, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();
}

Test scm_delayAbsolute_03 = {
	.id =			"SCM-ABSOULUTE-03",
	.description =	"Task which calls ROSA-DelayAbsolute should be in DELAY queue.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayAbsolute_03_main
};

/** SCM-ABSOULUTE-04 **/

ROSA_TickCount ticks_before_absolute_delay;

void write_current_ticks_and_delay_absolute(void)
{
	ticks_before_absolute_delay = ROSA_TimerTickCount();
	ROSA_DelayAbsolute(ticks_before_absolute_delay, 666666);
}

void check_absolute_delayed_tasks_wakeup_time(void)
{
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (( *(delayQueue.data[0]) ).task->wakeUpTime != ticks_before_absolute_delay + 666666)
		send_fail();
	
	send_success();
}

void scm_delayAbsolute_04_main(void)
{
	ROSA_CreateTask(&write_current_ticks_and_delay_absolute, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_CreateTask(&check_absolute_delayed_tasks_wakeup_time, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();
}

Test scm_delayAbsolute_04 = {
	.id =			"SCM-ABSOULUTE-04",
	.description =	"Task which calls ROSA-DelayAbsolute should have correct wake up time.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayAbsolute_04_main
};

/** SCM-ABSOULUTE-05 **/

void delay_absolute_and_check_for_how_long_have_i_been_delayed(void)
{
	ROSA_TickCount start_time = ROSA_TimerTickCount();
	ROSA_DelayAbsolute(start_time, 1256);
	
	ROSA_TickCount difference = ROSA_TimerTickCount() - start_time;
	if (difference == 1256 || difference == 1257)
		send_success();
	
	send_fail();
}

void scm_delayAbsolute_05_main(void)
{
	/* Task which calls ROSA_DelayAbsolute should wake up at correct time. */
	ROSA_CreateTask(&delay_absolute_and_check_for_how_long_have_i_been_delayed, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_delayAbsolute_05 = {
	.id =			"SCM-ABSOULUTE-05",
	.description =	"Task which calls ROSA-DelayAbsolute should wake up at correct time.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayAbsolute_05_main
};

/** SCM-ABSOULUTE-06 **/

void delay_absolute_and_check_delay_queue_after(void)
{
	ROSA_DelayAbsolute(0,100);
	PriorityQueue DELAYqueue = fetchDELAYqueue();
	if (DELAYqueue.size == 0)
		send_success();
	
	send_fail();
}

void scm_delayAbsolute_06_main(void)
{
	/* When task wakes up, it shouldn't be in DELAY queue. */
	ROSA_CreateTask(&delay_absolute_and_check_delay_queue_after, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_delayAbsolute_06 = {
	.id =			"SCM-ABSOULUTE-06",
	.description =	"When task wakes up, it shouldn't be in DELAY queue.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayAbsolute_06_main
};

/** SCM-ABSOULUTE-07 **/

TaskHandle task_absolute_delay_long, task_absolute_delay_medium, task_absolute_delay_short;

void delay_absolute_long(void)
{
	ROSA_DelayAbsolute(0, 150);
}

void delay_absolute_medium(void)
{
	ROSA_DelayAbsolute(0, 100);
}

void delay_absolute_short(void)
{
	ROSA_DelayAbsolute(0, 50);
}

void check_if_3_tasks_in_delay_queue(void)
{
	Task* task = NULL;
	task = getFromDELAYqueue();
	if (task != (Task*) task_absolute_delay_short)
	{
		send_fail();
	}
	task = getFromDELAYqueue();
	if (task != (Task*) task_absolute_delay_medium)
	{
		send_fail();
	}
	task = getFromDELAYqueue();
	if (task != (Task*) task_absolute_delay_long)
	{
		send_fail();
	}
	send_success();
}

void scm_delayAbsolute_07_main(void)
{
	/* Delay 3 tasks and check the sorting in the DELAY queue. */

	ROSA_CreateTask(&delay_absolute_medium, TEST_NAME, LARGE_STACK_SIZE, PRIORITY_6, &task_absolute_delay_medium);
	ROSA_CreateTask(&delay_absolute_long, TEST_NAME, LARGE_STACK_SIZE, PRIORITY_5, &task_absolute_delay_long);
	ROSA_CreateTask(&delay_absolute_short, TEST_NAME, LARGE_STACK_SIZE, PRIORITY_4, &task_absolute_delay_short);
	ROSA_CreateTask(&check_if_3_tasks_in_delay_queue, TEST_NAME, LARGE_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_Start();	
}

Test scm_delayAbsolute_07 = {
	.id =			"SCM-ABSOULUTE-07",
	.description =	"Delay 3 tasks and check the sorting in the DELAY queue.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayAbsolute_07_main
};

/** SCM-ABSOULUTE-08 **/

void delay_absolute_to_past(void)
{
	busy_wait(100 * TICK);
	ROSA_TickCount start_time = ROSA_TimerTickCount();
	ROSA_DelayAbsolute(0,50);
	
	ROSA_TickCount difference = ROSA_TimerTickCount() - start_time;
	if (difference == 1 || difference == 2)
		send_success();
		
	send_fail();
}

void scm_delayAbsolute_08_main(void)
{
	/* ROSA_DelayAbsolute with wake up time in past should wake up task after 1 tick. */

	ROSA_CreateTask(&delay_absolute_to_past, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_delayAbsolute_08 = {
	.id =			"SCM-ABSOULUTE-08",
	.description =	"ROSA_DelayAbsolute with wake up time in past should wake up task after 1 tick.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_delayAbsolute_08_main
};



/*************** TEST SUITE: End Cycle ***************/

/** SCM-ENDCYCLE-01 **/

int executionAfterEndCycle = 0;

void cyclicTaskWithLongPeriod(void)
{
	ROSA_EndCycle();
	executionAfterEndCycle = 1;
}

void checkExecutionAfterEndCycle(void)
{
	busy_wait(1000);
	if (executionAfterEndCycle)
	{
		send_fail();
	}
	send_success();
}

void scm_endCycle_01_main(void)
{
	/* Cyclic task with long period calling ROSA_EndCycle should stop working. */

	ROSA_CreateCyclicTask(&cyclicTaskWithLongPeriod, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, (ROSA_TickCount) 50000, (ROSA_TickCount) 50000, NULL);
	ROSA_CreateTask(&checkExecutionAfterEndCycle, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();	
}

Test scm_endCycle_01 = {
	.id =			"SCM-ENDCYCLE-01",
	.description =	"Cyclic task with long period calling ROSA_EndCycle should stop working.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_endCycle_01_main
};

/** SCM-ENDCYCLE-02 **/

void cyclicTaskWithShortPeriodSendingSuccess(void)
{
	ROSA_EndCycle();
	send_success();
}

void scm_endCycle_02_main(void)
{
	/* Cyclic task calling ROSA_EndCycle should wake up. */

	ROSA_CreateCyclicTask(&cyclicTaskWithShortPeriodSendingSuccess, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, (ROSA_TickCount) 10, (ROSA_TickCount) 10, NULL);
	ROSA_Start();
}

Test scm_endCycle_02 = {
	.id =			"SCM-ENDCYCLE-02",
	.description =	"Cyclic task calling ROSA_EndCycle should wake up.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_endCycle_02_main
};

/** SCM-ENDCYCLE-03 **/

TaskHandle cyclicTaskThatCallsEndCycleHandle;

void cyclicTaskThatCallsEndCycle(void)
{
	ROSA_EndCycle();
}

void checkDelayQueue(void)
{
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (delayQueue.data[0]->task != (Task*) cyclicTaskThatCallsEndCycleHandle)
	{
		send_fail();
	}
	if (delayQueue.size != 1)
	{
		send_fail();
	}
	send_success();
}

void scm_endCycle_03_main(void)
{
	/* Cyclic task which calls ROSA_EndCycle should be in DELAY queue. */
	ROSA_CreateCyclicTask(&cyclicTaskThatCallsEndCycle, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, (ROSA_TickCount) 50000, (ROSA_TickCount) 5000, &cyclicTaskThatCallsEndCycleHandle);
	ROSA_CreateTask(&checkDelayQueue, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();
}

Test scm_endCycle_03 = {
	.id =			"SCM-ENDCYCLE-03",
	.description =	"Cyclic task which calls ROSA_EndCycle should be in DELAY queue.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_endCycle_03_main
};

/** SCM-ENDCYCLE-04 **/

TaskHandle cyclicTaskThatBusyWaitsAndCallsEndCycleHandle;

void cyclicTaskThatBusyWaitsAndCallsEndCycle(void)
{
	busy_wait(3);
	ROSA_EndCycle();
}

void checkWakeUpTime(void)
{
	PriorityQueue delayQueue = fetchDELAYqueue();
	if (delayQueue.data[0]->task != (Task*) cyclicTaskThatBusyWaitsAndCallsEndCycleHandle)
	{
		send_fail();
	}
	if (((Task*) delayQueue.data[0]->task)->wakeUpTime != 10)
	{
		send_fail();
	}
	send_success();
}

void scm_endCycle_04_main(void)
{
	/* Cyclic task which calls ROSA_EndCycle should have correct wake up time, according to EC formula, if task didn't miss period. */

	ROSA_CreateCyclicTask(&cyclicTaskThatBusyWaitsAndCallsEndCycle, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, (ROSA_TickCount) 10, (ROSA_TickCount) 10, &cyclicTaskThatBusyWaitsAndCallsEndCycleHandle);
	ROSA_CreateTask(&checkWakeUpTime, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_Start();
}

Test scm_endCycle_04 = {
	.id =			"SCM-ENDCYCLE-04",
	.description =	"Cyclic task which calls ROSA_EndCycle should have correct wake up time, according to EC formula, if task didn't miss period.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_endCycle_04_main
};

/** SCM-ENDCYCLE-05 **/

TaskHandle cyclicTaskThatMissesItsPeriodHandle;
int afterDelayAbsolute = 0;

void cyclicTaskThatMissesItsPeriod(void)
{
	ROSA_DelayAbsolute(0, 167);
	afterDelayAbsolute = 1;
	ROSA_EndCycle();
	send_fail();
}

void checkWakeUpTimeAfterMissedPeriod(void)
{
	PriorityQueue delay_q;
	
	while(afterDelayAbsolute != 1)
		;
	
	delay_q = fetchDELAYqueue();
	
	if(delay_q.data[0]->task->wakeUpTime == 170)
		send_success();	
	else
		send_fail();
}

void scm_endCycle_05_main(void)
{
	/* Cyclic task which calls ROSA_EndCycle should have wake up time at correct time, according to EC formula, if the task misses its period. */

	ROSA_CreateCyclicTask(&cyclicTaskThatMissesItsPeriod, TEST_NAME, 4096, PRIORITY_6, (ROSA_TickCount) 10, (ROSA_TickCount) 10, &cyclicTaskThatMissesItsPeriodHandle);
	ROSA_CreateTask(&checkWakeUpTimeAfterMissedPeriod, TEST_NAME, 4096, PRIORITY_5, NULL);
	ROSA_Start();
}

Test scm_endCycle_05 = {
	.id =			"SCM-ENDCYCLE-05",
	.description =	"Cyclic task which calls ROSA_EndCycle should have wake up time at correct time, according to EC formula, if the task misses its period.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_endCycle_05_main
};

/** SCM-ENDCYCLE-06 **/

int executedAtLeastOnce = 0;

void cyclicTaskThatDoesntMissPeriod(void)
{
	if (executedAtLeastOnce == 1)
	{
		ROSA_TickCount currentTicks = ROSA_TimerTickCount();
		if (currentTicks != 10 && currentTicks != 11 && currentTicks != 12)
		{
			send_fail();
		}
		send_success();
	}
	busy_wait(3);
	executedAtLeastOnce = 1;
	ROSA_EndCycle();
}

void scm_endCycle_06_main(void)
{
	/* A cyclic task calling EC formula should wake up at the correct time, if it didn't miss period. */
	ROSA_CreateCyclicTask(&cyclicTaskThatDoesntMissPeriod, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, (ROSA_TickCount) 10, (ROSA_TickCount) 10, NULL);
	ROSA_Start();
}

Test scm_endCycle_06 = {
	.id =			"SCM-ENDCYCLE-06",
	.description =	"A cyclic task calling EC formula should wake up at the correct time, if it didn't miss period.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_endCycle_06_main
};

/** SCM-ENDCYCLE-07 **/

void cyclicTaskThatDoesntMissesPeriod(void)
{
	if (executedAtLeastOnce == 1)
	{
		ROSA_TickCount currentTicks = ROSA_TimerTickCount();
		if (currentTicks != (ROSA_TickCount) 170)
		{
			send_fail();
		}
		send_success();
	}
	ROSA_DelayAbsolute(0, 167);
	executedAtLeastOnce = 1;
	ROSA_EndCycle();
}

void scm_endCycle_07_main(void)
{
	/* A cyclic task calling EC formula should wake up at the correct time, if it misses period. */
	executedAtLeastOnce = 0;
	ROSA_CreateCyclicTask(&cyclicTaskThatDoesntMissesPeriod, TEST_NAME, LARGE_STACK_SIZE, PRIORITY_5, (ROSA_TickCount) 10, (ROSA_TickCount) 10, NULL);
	ROSA_Start();
}

Test scm_endCycle_07 = {
	.id =			"SCM-ENDCYCLE-07",
	.description =	"A cyclic task calling EC formula should wake up at the correct time, if it misses period.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_endCycle_07_main
};


/** SCM-ENDCYCLE-08 **/

void cyclicTaskThatChecksItsNotInDelayQueue(void)
{
	if (executedAtLeastOnce == 1)
	{
		PriorityQueue delayQueue = fetchDELAYqueue();
		if (delayQueue.size != 0)
		{
			send_fail();
		}
		send_success();
	}
	executedAtLeastOnce = 1;
	ROSA_EndCycle();
}

void scm_endCycle_08_main(void)
{
	/* When cyclic task wakes up, it shouldn't be in the delay queue. */

	executedAtLeastOnce = 0;
	ROSA_CreateCyclicTask(&cyclicTaskThatChecksItsNotInDelayQueue, TEST_NAME, SMALL_STACK_SIZE, PRIORITY_5, (ROSA_TickCount) 10, (ROSA_TickCount) 10, NULL);
	ROSA_Start();
}

Test scm_endCycle_08 = {
	.id =			"SCM-ENDCYCLE-08",
	.description =	"When cyclic task wakes up, it shouldn't be in the delay queue.",
	.plan =			"System Clock Manager",
	.suite =		"Delay Absolute",
	.type =			TEST_FUNCTIONAL,
	.function =		scm_endCycle_08_main
};
