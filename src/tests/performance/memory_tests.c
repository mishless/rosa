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





void task2()
{
	ROSA_CreateTask(TEST_PTR, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_3, NULL);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_02_main(unsigned int maximum_tasks)
{
	int i;
	for (i = 0; i < (maximum_tasks-2) ; i++)
	{
		ROSA_CreateTask(TEST_PTR, "low", 0, PRIORITY_3, NULL);
	}
	TestStack_ROSA_CreateTask(task2, "med", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_Start();
}

Test pt_mt_sc_02 = {
	.id =			"PT-MT-SC-02",
	.description =	"Measure Memory usage of Create Task depending on the number of tasks in the ready queue.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "2",
	.end_parameter = "42",
.function =		pt_mt_sc_02_main};








void task3()
{
	ROSA_CreateCyclicTask(TEST_PTR, "t3", SUPER_SMALL_STACK_SIZE, PRIORITY_4, 42, 42, NULL);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_03_main()
{
	TestStack_ROSA_CreateTask(task3, "t1", SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test pt_mt_sc_03 = {
	.id =			"PT-MT-SC-03",
	.description =	"Measure Memory usage of Create Cyclic Task.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =		pt_mt_sc_03_main};





void task4()
{
	ROSA_CreateCyclicTask(TEST_PTR, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_3, 5, 6, NULL);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_04_main(unsigned int maximum_tasks)
{
	int i;
	for (i = 0; i < (maximum_tasks-2) ; i++)
	{
		ROSA_CreateTask(TEST_PTR, "low", 0, PRIORITY_3, NULL);
	}
	TestStack_ROSA_CreateTask(task4, "med", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_Start();
}

Test pt_mt_sc_04 = {
	.id =			"PT-MT-SC-04",
	.description =	"Measure Memory usage of Create Cyclic Task depending on the number of tasks in the ready queue.",
	.plan =			"Performance Memory",
	.suite =		"System Calls",
	.type =			TEST_MEMORY_PERFORMANCE,
	.start_parameter = "2",
	.end_parameter = "42",
.function =		pt_mt_sc_04_main};






/*** PT-MT-SC-05 ***/

/*TaskHandle task_handle_5;

void task_pt_mt_sc_05_high()
{
	ROSA_TerminateTask();
}

void task_pt_mt_sc_05_low()
{
	send_result(get_max_stack(task_handle_5));
}


void pt_mt_sc_05_main()
{
	ROSA_CreateTask(task_pt_mt_sc_05_low, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_05_high, TASK_NAME, SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_5);
	ROSA_Start();
}

Test pt_mt_sc_05 = {
	.id =            "PT-MT-SC-05",
	.description =    "Measure memory usage of Terminate Task.",
	.plan =            "Performance Memory",
	.suite =			"System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_05_main};
*/



void task_pt_mt_sc_06()
{
	ROSA_TimerTickCount();
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_06_main()
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_06, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test pt_mt_sc_06 = {
	.id =            "PT-MT-SC-06",
	.description =    "Measure Get Ticks.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_06_main};




TaskHandle task_handle_7;

void task_pt_mt_sc_07_low(void)
{
	send_result(get_max_stack(task_handle_7));
}

void task_pt_mt_sc_07_high(void)
{
	ROSA_DelayAbsolute(0, 20);
}

void pt_mt_sc_07_main(void)
{
	ROSA_CreateTask(task_pt_mt_sc_07_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_07_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_7);

	ROSA_StartScheduler();
}

Test pt_mt_sc_07 = {
	.id =            "PT-MT-SC-07",
	.description =    "Measure memory usage of Absolute Delay.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_07_main};




TaskHandle task_handle_8;

void task_pt_mt_sc_08_low(void)
{
	send_result(get_max_stack(task_handle_8));
}

void task_pt_mt_sc_08_high(void)
{
	ROSA_DelayRelative(20);
}

void pt_mt_sc_08_main(void)
{
	ROSA_CreateTask(task_pt_mt_sc_08_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_08_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_8);

	ROSA_StartScheduler();
}

Test pt_mt_sc_08 = {
	.id =            "PT-MT-SC-08",
	.description =    "Measure memory usage of Relative Delay.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_08_main};






TaskHandle task_handle_9;

void task_pt_mt_sc_09_low(void)
{
	send_result(get_max_stack(task_handle_9));
}

void task_pt_mt_sc_09_high(void)
{
	ROSA_EndCycle();
}

void pt_mt_sc_09_main(void)
{
	ROSA_CreateTask(task_pt_mt_sc_09_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	TestStack_ROSA_CreateCyclicTask(task_pt_mt_sc_09_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, 5, 5, &task_handle_9);

	ROSA_StartScheduler();
}

Test pt_mt_sc_09 = {
	.id =            "PT-MT-SC-09",
	.description =    "Measure memory usage of End Cycle.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_09_main};







TaskHandle task_handle_10;

void task_pt_mt_sc_10_high(void)
{
	send_result(get_max_stack(task_handle_10));
}

void task_pt_mt_sc_10_low(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_10_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
}

void pt_mt_sc_10_main(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_10_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &task_handle_10);
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_10 = {
	.id =            "PT-MT-SC-10",
	.description =    "Measure memory usage of calling Create Task and context switching inside of it.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_10_main};






void task_pt_mt_sc_11(void)
{
	ROSA_SemaphoreCreateBinary(NULL, SEMAPHORE_FREE);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_11_main(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_11, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_11 = {
	.id =            "PT-MT-SC-11",
	.description =    "Measure memory usage of Create Semaphore.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_11_main};






SemaphoreHandle sem;

void neki_task()
{
	ROSA_SemaphoreDelete(sem);
	send_result(get_max_stack(getCRT()));
}
void pt_mt_sc_12_main(void)
{
	ROSA_SemaphoreCreateBinary(&sem, SEMAPHORE_FREE);
	TestStack_ROSA_CreateTask(neki_task, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
	ROSA_Start();
}

Test pt_mt_sc_12 = {
	.id =            "PT-MT-SC-12",
	.description =    "Measure memory usage of Semaphore Delete.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_12_main};







SemaphoreHandle sem_13;

void task_pt_mt_sc_13(void)
{
	ROSA_SemaphoreTake(sem_13, 0);
	send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_13_main(void)
{
	TestStack_ROSA_CreateTask(task_pt_mt_sc_13, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	ROSA_SemaphoreCreateBinary(&sem_13, SEMAPHORE_FREE);
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_13 = {
	.id =            "PT-MT-SC-13",
	.description =    "Measure memory usage of Take Semaphore.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_13_main};










SemaphoreHandle sem_14;
TaskHandle task_handle_14;

void task_pt_mt_sc_14_low(void)
{
	ROSA_SemaphoreTake(sem_14, 10000);
}

void task_pt_mt_sc_14_high(void)
{
	ROSA_SemaphoreTake(sem_14, 100);
}

void task_pt_mt_sc_14_medium(void)
{
	ROSA_SemaphoreTake(sem_14, 0);
	ROSA_DelayRelative(10);
	
	TestStack_ROSA_CreateTask(task_pt_mt_sc_14_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_14);

	send_result(get_max_stack(task_handle_14));
}

void pt_mt_sc_14_main(unsigned int number_of_low_prio_tasks)
{
	int i;
	
	ROSA_SemaphoreCreateBinary(&sem_14, SEMAPHORE_FREE);
	ROSA_CreateTask(task_pt_mt_sc_14_medium, "med", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	
	for (i = 0; i < number_of_low_prio_tasks; i++)
	{
		ROSA_CreateTask(task_pt_mt_sc_14_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_3, NULL);
	}
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_14 = {
	.id =            "PT-MT-SC-14",
	.description =    "Measure memory usage of taking an occupied semaphore depending on the number of blocked tasks waiting on that semaphore.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "0",
	.end_parameter = "10",
.function =        pt_mt_sc_14_main};







SemaphoreHandle sem_15;

void task_pt_mt_sc_15(void)
{
    ROSA_SemaphoreGive(sem_15);
    send_result(get_max_stack(getCRT()));
}

void pt_mt_sc_15_main(void)
{
    ROSA_SemaphoreCreateBinary(&sem_15, SEMAPHORE_OCCUPIED);
    TestStack_ROSA_CreateTask(task_pt_mt_sc_15, "task", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
    
    ROSA_StartScheduler();
}

Test pt_mt_sc_15 = {
    .id =            "PT-MT-SC-15",
    .description =    "Measure memory usage of Give Semaphore.",
    .plan =            "Performance Memory",
    .suite =        "System Calls",
    .type =            TEST_MEMORY_PERFORMANCE,
    .start_parameter = "-1",
    .end_parameter = "-1",
.function =        pt_mt_sc_15_main};






SemaphoreHandle sem_16;
TaskHandle task_handle_16;

void task_pt_mt_sc_16_high(void)
{
	ROSA_SemaphoreTake(sem_16, 10000);
	send_result(get_max_stack(task_handle_16));
}

void task_pt_mt_sc_16_low(void)
{
	ROSA_SemaphoreGive(sem_16);
}

void pt_mt_sc_16_main(void)
{
	ROSA_SemaphoreCreateBinary(&sem_16, SEMAPHORE_OCCUPIED);
	
	TestStack_ROSA_CreateTask(task_pt_mt_sc_16_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, NULL);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_16_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, &task_handle_16);
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_16 = {
	.id =            "PT-MT-SC-16",
	.description =    "Measure memory usage of Give Semaphore when it causes a context switch.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "-1",
	.end_parameter = "-1",
.function =        pt_mt_sc_16_main};







SemaphoreHandle sem_17;
TaskHandle task_handle_17;

void task_pt_mt_sc_17_low(void)
{
	ROSA_SemaphoreTake(sem_17, 10000);
	send_result(get_max_stack(task_handle_17));
}

void task_pt_mt_sc_17_high(void)
{
	ROSA_DelayRelative(10);
	ROSA_SemaphoreGive(sem_17);
	ROSA_DelayRelative(50);
}

void pt_mt_sc_17_main(unsigned int number_of_low_prio_tasks)
{
	int i;
	
	ROSA_SemaphoreCreateBinary(&sem_17, SEMAPHORE_OCCUPIED);
	TestStack_ROSA_CreateTask(task_pt_mt_sc_17_high, "high", SUPER_SMALL_STACK_SIZE, PRIORITY_5, &task_handle_17);
	
	for (i = 0; i < number_of_low_prio_tasks; i++)
	{
		TestStack_ROSA_CreateTask(task_pt_mt_sc_17_low, "low", SUPER_SMALL_STACK_SIZE, PRIORITY_4, NULL);
	}
	
	ROSA_StartScheduler();
}

Test pt_mt_sc_17 = {
	.id =            "PT-MT-SC-17",
	.description =    "Measure memory usage of Give Semaphore depending on the number of blocked tasks waiting on that semaphore.",
	.plan =            "Performance Memory",
	.suite =        "System Calls",
	.type =            TEST_MEMORY_PERFORMANCE,
	.start_parameter = "1",
	.end_parameter = "10",
.function =        pt_mt_sc_17_main};