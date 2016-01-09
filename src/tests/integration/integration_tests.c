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
#define SMALL_STACK_SIZE 4096

#define BIG_TIMEOUT 10000000

#define PRIORITY_1 1
#define PRIORITY_2 2
#define PRIORITY_3 3
#define PRIORITY_4 4

#define PERIOD 800
#define DEADLINE PERIOD

ROSA_TickCount tick_count_1, tick_count_2, tick_count_3;

unsigned int iteration = 0;

void tick_getter_1(void)
{
	while (1)
	{
		if(iteration == 0) iteration = 1;
		else if(iteration == 3) iteration = 4;
		tick_count_1 = ROSA_TimerTickCount();
	}
}

void tick_getter_2(void)
{
	while (1)
	{
		if(iteration == 1)
		{
			iteration = 2;
		}
		if(iteration == 4) 
		{
			iteration = 5;
		}
		tick_count_2 = ROSA_TimerTickCount();
	}
}

void tick_getter_and_tick_checker(void)
{
	while(1)
	{
		tick_count_3 = ROSA_TimerTickCount();
		
		if(iteration == 2) iteration = 3;
		
		
		
		if(iteration == 5)
		{
			if ( (tick_count_1 == (4*ROUND_ROBIN_PERIOD - 1)) &&
			(tick_count_2 == (5*ROUND_ROBIN_PERIOD - 1)) &&
			(tick_count_3 == (5*ROUND_ROBIN_PERIOD)) )
			{
				send_success();
			}
			else
			{
				send_fail();
			}
		}
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
	ROSA_DelayRelative(10);
	for (i = 0; i < 2; i++)
	{
		if (something_consumed != 1 || something_produced != 1)
		{
			send_fail();
		}
		something_consumed = 0;
		something_produced = 0;
		ROSA_DelayRelative(10);
	}
	send_success();
}

void it_02_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_CreateTask(producer, "prod", SMALL_STACK_SIZE, PRIORITY_2, NULL);
	ROSA_CreateTask(consumer, "cons", SMALL_STACK_SIZE, PRIORITY_2, NULL);
	ROSA_CreateTask(producer_consumer_checker, task_name, SMALL_STACK_SIZE, PRIORITY_3, NULL);
	
	ROSA_SemaphoreCreateBinary(&somethingProduced, SEMAPHORE_OCCUPIED);
	ROSA_SemaphoreCreateBinary(&somethingConsumed, SEMAPHORE_FREE);
	
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

SemaphoreHandle semaphore_1, semaphore_2;

void task_1(void)
{
	while (1)
	{
		ROSA_DelayRelative(100);
		ROSA_SemaphoreTake(semaphore_1, 10);
		ROSA_DelayRelative(50);
		ROSA_SemaphoreGive(semaphore_1);
		ROSA_SemaphoreTake(semaphore_2, 10);
		ROSA_DelayRelative(50);
		ROSA_SemaphoreGive(semaphore_2);
		ROSA_DelayRelative(50);
	}
}

void task_2(void)
{
	while (1)
	{
		ROSA_DelayRelative(50);
		ROSA_SemaphoreTake(semaphore_2, 10);
		ROSA_DelayRelative(100);
		ROSA_SemaphoreGive(semaphore_2);
		ROSA_DelayRelative(50);
	}
}

void task_3(void)
{
	while (1)
	{
		ROSA_DelayRelative(100);
	}
}

void task_4(void)
{
	while (1)
	{
		ROSA_DelayRelative(50);
		ROSA_SemaphoreTake(semaphore_1, 10);
		ROSA_DelayRelative(200);
		ROSA_SemaphoreGive(semaphore_1);
		ROSA_DelayRelative(50);
	}
}

void swedish_test_main()
{
	char task_name[5] = TASK_NAME;
	
	ROSA_SemaphoreCreatePrio(&semaphore_1, SEMAPHORE_FREE, PRIORITY_4);
	ROSA_SemaphoreCreatePrio(&semaphore_2, SEMAPHORE_FREE, PRIORITY_4);
	
	ROSA_CreateCyclicTask(task_1, task_name, 4096, PRIORITY_4, PERIOD, DEADLINE, NULL);
	ROSA_CreateCyclicTask(task_2, task_name, 4096, PRIORITY_3, PERIOD, DEADLINE, NULL);
	ROSA_CreateCyclicTask(task_3, task_name, 4096, PRIORITY_2, PERIOD, DEADLINE, NULL);
	ROSA_CreateCyclicTask(task_4, task_name, 4096, PRIORITY_1, PERIOD, DEADLINE, NULL);
	
	ROSA_StartScheduler();
}

Test swedish_test = {
	.id = "IT-03",
	.description = "Swedish test.",
	.plan = "Integration",
	.suite = "Integration",
	.type = TEST_FUNCTIONAL,
	.function = swedish_test_main,
};
