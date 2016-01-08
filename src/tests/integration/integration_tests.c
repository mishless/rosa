/*
 * integration_tests.c
 *
 * Created: 8.1.2016. 17:17:12
 *  Author: Miha
 */

#include <avr32/io.h>
#include "helper_functions.h"
#include "rosa_ker.h"
#include "tests.h"
#include "integration_tests.h"
#include "rosa_api_call.h"

#define TASK_NAME "intg"
#define SMALL_STACK_SIZE 1000

#define BIG_TIMEOUT 10000000

#define PRIORITY_2 2
#define PRIORITY_3 3

ROSA_TickCount tick_count_1, tick_count_2, tick_count_3;

void tick_getter_1(void)
{
	while (1)
	{
		tick_count_1 = ROSA_TimerTickCount();
	}
}

void tick_getter_2(void)
{
	while (1)
	{
		tick_count_2 = ROSA_TimerTickCount();
	}
}

void tick_getter_and_tick_checker(void)
{
	tick_count_3 = ROSA_TimerTickCount();
	if (tick_count_1 == ROUND_ROBIN_PERIOD - 1 && tick_count_2 == 2*ROUND_ROBIN_PERIOD - 1 && tick_count_3 == 2*ROUND_ROBIN_PERIOD)
	{
		send_success();
	}
	else
	{
		send_fail();
	}
}

void it_01_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(tick_getter_1, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_CreateTask(tick_getter_2, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	ROSA_CreateTask(tick_getter_and_tick_checker, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	
	ROSA_StartScheduler();
}

Test it_01 = {
	.id = "IT-01",
	.description = "Three tasks with same priority should run in Round Robin.",
	.plan = "Integration",
	.suite = "Integration",
	.type = TEST_FUNCTIONAL,
	.function = it_01_main,
};

SemaphoreHandle somethingProduced, somethingConsumed;
int something_produced = 0, something_consumed = 0;

void producer(void)
{
	while (1)
	{
		ROSA_SemaphoreTake(somethingConsumed, BIG_TIMEOUT);
		something_produced = 1;
		ROSA_SemaphoreGive(somethingProduced);
	}
}

void consumer(void)
{
	while (1)
	{
		ROSA_SemaphoreTake(somethingProduced, BIG_TIMEOUT);
		something_consumed = 1;
		ROSA_SemaphoreGive(somethingConsumed);
	}
}

void producer_consumer_checker(void)
{
	int i;
	ROSA_DelayRelative(100);
	for (i = 0; i < 10; i++)
	{
		if (something_consumed != 1 || something_produced != 1)
		{
			send_fail();
		}
		something_consumed = 0;
		something_produced = 0;
		ROSA_DelayRelative(50);
	}
	send_success();
}

void it_02_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(producer, task_name, SMALL_STACK_SIZE, PRIORITY_2, NULL);
	ROSA_CreateTask(consumer, task_name, SMALL_STACK_SIZE, PRIORITY_2, NULL);
	ROSA_CreateTask(producer_consumer_checker, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	
	ROSA_SemaphoreCreateBinary(&somethingProduced, SEMAPHORE_FREE);
	ROSA_SemaphoreCreateBinary(&somethingConsumed, SEMAPHORE_OCCUPIED);
	
	ROSA_StartScheduler();
}

Test it_02 = {
	.id = "IT-02",
	.description = "Producer / Consumer with two binary semaphores.",
	.plan = "Integration",
	.suite = "Integration",
	.type = TEST_FUNCTIONAL,
	.function = it_02_main,
};
