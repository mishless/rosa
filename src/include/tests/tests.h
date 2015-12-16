/*
 * tests.h
 *
 * Created: 16.12.2015. 7:32:54 PM
 *  Author: Vukan
 */ 


#ifndef TESTS_H_
#define TESTS_H_

#include "task_manager_tests.h"
#include "drivers/usart.h"
#include "helper_functions.h"

#define SUCCESS_CHAR '^'
#define FAILURE_CHAR '_'

#define send_fail()		usartWriteChar(&AVR32_USART0, FAILURE_CHAR);
						
#define send_success()	usartWriteChar(&AVR32_USART0, SUCCESS_CHAR);

#endif /* TESTS_H_ */