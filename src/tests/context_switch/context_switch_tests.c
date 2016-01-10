/*
 * context_switch_tests.c
 *
 * Created: 10.01.2016. 2:21:06 PM
 *  Author: Vukan
 */ 

#include "context_switch_tests.h"
#include "tests.h"

#define PRIORITY_6 6
#define PRIORITY_5 5
#define BIG_TIMEOUT 1000000

SemaphoreHandle cs_01_sem;

void t1_cs_02(void);

/*** CS-01-t1 ***/

void cs_01_t1()
{
	ROSA_SemaphoreTake(cs_01_sem, BIG_TIMEOUT);
	send_success();
}

void cs_01_t2()
{
	ROSA_SemaphoreGive(cs_01_sem);
	send_fail();
}
void cs_01_main()
{
	TaskHandle t1, t2;
	
	ROSA_CreateTask(cs_01_t1, "t1", SMALL_STACK_SIZE, PRIORITY_6, &t1);
	ROSA_CreateTask(cs_01_t2, "t2", SMALL_STACK_SIZE, PRIORITY_5, &t2);
	ROSA_SemaphoreCreateBinary(&cs_01_sem, SEMAPHORE_OCCUPIED);
	
	ROSA_Start();
}

Test cs_01 = {
	.id =			"CS-01",
	.description =	"Context switch from normal -> normal.",
	.plan =			"Context Switch",
	.suite =		"Context Switch",
	.type =			TEST_FUNCTIONAL,
.function =		cs_01_main};

/*** CS-02 ***/

unsigned int cs_02_flag = 0;

void dummy_function_cs_02()
{
	while(cs_02_flag != 2)
	{
		if(cs_02_flag == 0)
		{
			cs_02_flag = 1;
		}
	}
}

void t1_cs_02()
{
	dummy_function_cs_02();
	send_success();
}

void t2_cs_02()
{
	while(1)
	{
		if(cs_02_flag == 1)
		{
			cs_02_flag = 2;
		}
	}
}

void cs_02_main()
{
	TaskHandle t1, t2;
	
	ROSA_CreateTask(t1_cs_02, "t1", SMALL_STACK_SIZE, PRIORITY_5, &t1);
	ROSA_CreateTask(t2_cs_02, "t2", SMALL_STACK_SIZE, PRIORITY_5, &t2);
	ROSA_Start();
}

Test cs_02 = {
	.id =			"CS-02",
	.description =	"Context switch from ISR -> ISR.",
	.plan =			"Context Switch",
	.suite =		"Context Switch",
	.type =			TEST_FUNCTIONAL,
.function =		cs_02_main};

/*** CS-03 ***/

void t1_cs_03()
{
	ROSA_DelayRelative(10);
	send_success();
}
void cs_03_main()
{
	TaskHandle t1;
	ROSA_CreateTask(t1_cs_03, "t1", SMALL_STACK_SIZE, PRIORITY_5, &t1);
	ROSA_Start();
}

Test cs_03 = {
	.id =			"CS-03",
	.description =	"Context switch from normal -> ISR.",
	.plan =			"Context Switch",
	.suite =		"Context Switch",
	.type =			TEST_FUNCTIONAL,
.function =		cs_03_main};


/*** CS-04 ***/

void t1_cs_04()
{
	ROSA_DelayRelative(10);
	ROSA_DelayRelative(10);
}

void t2_cs_04()
{
	busy_wait(15);
	send_success();
}

void cs_04_main()
{
	TaskHandle t1, t2;
	ROSA_CreateTask(t1_cs_04, "t1", SMALL_STACK_SIZE, PRIORITY_6, &t1);
	ROSA_CreateTask(t2_cs_04, "t2", SMALL_STACK_SIZE, PRIORITY_5, &t2);
	ROSA_Start();
}

Test cs_04 = {
	.id =			"CS-04",
	.description =	"Context switch from ISR -> normal.",
	.plan =			"Context Switch",
	.suite =		"Context Switch",
	.type =			TEST_FUNCTIONAL,
.function =		cs_04_main};