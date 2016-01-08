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
	void (*function)(void);
} Test;

#include "clock_manager_tests.h"
#include "task_manager_tests.h"
#include "scheduler_manager_tests.h"
#include "scheduler_manager_tests.h"
#include "semaphore_manager_tests.h"

#include "drivers/usart.h"
#include "helper_functions.h"

#define SUCCESS_CHAR '^'
#define FAILURE_CHAR '_'

#define TEST_FUNCTIONAL 0
#define TEST_PERFORMANCE 1

void send_fail();
void send_success();

void run_test(Test test);
#endif /* TESTS_H_ */