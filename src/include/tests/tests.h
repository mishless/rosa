/*
 * tests.h
 *
 * Created: 16.12.2015. 7:32:54 PM
 *  Author: Vukan
 */ 


#ifndef TESTS_H_
#define TESTS_H_

#include "clock_manager_tests.h"
#include "task_manager_tests.h"
#include "drivers/usart.h"
#include "helper_functions.h"

#define SUCCESS_CHAR '^'
#define FAILURE_CHAR '_'

void send_fail();
void send_success();

#endif /* TESTS_H_ */