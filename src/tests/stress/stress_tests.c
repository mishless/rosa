/*
 * stress_tests.c
 *
 * Created: 8.1.2016. 17:58:07
 *  Author: Miha
 */ 

#include <avr32/io.h>
#include "helper_functions.h"
#include "rosa_ker.h"
#include "tests.h"
#include "stress_tests.h"
#include "rosa_api_call.h"

#define TASK_NAME "strs"
#define STACK_SIZE 1000

#define BIG_TIMEOUT 10000000

#define PRIORITY_2 2
#define PRIORITY_3 3

int semaphores_taken = 0;
int semaphore_last_reserved = 0;

SemaphoreHandle bunch_of_semaphores[MAX_NUMBER_TASKS];

void semaphore_taker_and_incrementer(void)
{
	int my_semaphore = semaphore_last_reserved;
	semaphore_last_reserved++;
	while (1)
	{
		ROSA_SemaphoreTake(bunch_of_semaphores[my_semaphore], BIG_TIMEOUT);
		semaphores_taken++;
		ROSA_SemaphoreGive(bunch_of_semaphores[my_semaphore]);
		if (semaphores_taken > 100)
		{
			send_success();
		}
	}
}

void it_st_01_main()
{
	int i;
	char task_name[5] = TASK_NAME;
	
	for (i = 0; i < MAX_NUMBER_TASKS - 1; i++)
	{
		ROSA_CreateTask(semaphore_taker_and_incrementer, task_name, STACK_SIZE, PRIORITY_3, NULL);
	}
	ROSA_SemaphoreCreateBinary(&bunch_of_semaphores[0], SEMAPHORE_FREE);
	for (i = 1; i < MAX_NUMBER_TASKS - 1; i++)
	{
		ROSA_SemaphoreCreateBinary(&bunch_of_semaphores[i], SEMAPHORE_OCCUPIED);
	}
	
	ROSA_StartScheduler();
}

Test it_st_01 = {
	.id = "IT-ST-01",
	.description = "Create the maximum number of tasks and maximum number of semaphores and ever task will give the semaphore used by the next one.",
	.plan = "Integration",
	.suite = "Stress",
	.type = TEST_FUNCTIONAL,
	.function = it_st_01_main,
};

SemaphoreHandle tortured_forever[MAX_NUMBER_SEMAPHORES];

void it_st_02_main()
{
	int i, j;
	for (i = 0; i < 500; i++)
	{
		for (j = 0; j < MAX_NUMBER_SEMAPHORES; j++)
		{
			ROSA_SemaphoreCreateBinary(&tortured_forever[j], SEMAPHORE_FREE);
		}
		for (j = 0; j < MAX_NUMBER_SEMAPHORES; j++)
		{
			ROSA_SemaphoreDelete(tortured_forever[j]);
		}
	}
	send_success();
}

Test it_st_02 = {
	.id = "IT-ST-02",
	.description = "Create and delete maximum number of binary semaphores 1000 times.",
	.plan = "Integration",
	.suite = "Stress",
	.type = TEST_FUNCTIONAL,
	.function = it_st_02_main,
};
