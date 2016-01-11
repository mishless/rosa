/*
 * tests.h
 *
 * Created: 16.12.2015. 7:32:54 PM
 *  Author: Vukan
 */ 


#ifndef TESTS_H_
#define TESTS_H_

typedef struct
{
	char* id;
	char* description;
	char* plan;
	char* suite;
	unsigned int type;
	void* function;
	char* start_parameter;
	char* end_parameter;
} Test;

#include "clock_manager_tests.h"
#include "task_manager_tests.h"
#include "scheduler_manager_tests.h"
#include "semaphore_manager_tests.h"
#include "stress_tests.h"
#include "integration_tests.h"
#include "performance_tests.h"
#include "memory_tests.h"
#include "context_switch_tests.h"
#include "drivers/usart.h"
#include "helper_functions.h"
#include "rosa_config.h"
#include "rosa_api_call.h"

#define SUCCESS_CHAR '^'
#define FAILURE_CHAR '_'

#define TEST_FUNCTIONAL 0
#define TEST_SPEED_PERFORMANCE 1
#define TEST_MEMORY_PERFORMANCE 2

#define SMALL_STACK_SIZE 2048
#define SUPER_SMALL_STACK_SIZE 2048

void send_fail();
void send_success();
void send_result(unsigned int number);

void run_test(Test test);
void run_test_manual(Test test);
void run_test_manual_param(Test test, unsigned int number);

void fill_stack(TaskHandle task);
unsigned int get_max_stack(TaskHandle task);
unsigned int TestStack_ROSA_CreateTask(void (*functionBody) (void), char * functionNameChArr, unsigned int maxStackSize, unsigned int taskPriority, TaskHandle *taskHandle);
unsigned int TestStack_ROSA_CreateCyclicTask(void (*functionBody) (void), char * functionNameChArr, unsigned int maxStackSize, unsigned int taskPriority, ROSA_TickCount taskPeriod, ROSA_TickCount taskDeadline, TaskHandle *taskHandle);
#endif /* TESTS_H_ */