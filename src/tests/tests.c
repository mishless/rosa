/*
 * tests.c
 *
 * Created: 17.12.2015. 12:46:41 AM
 *  Author: Vukan
 */ 

#include "tests.h"
#include "drivers/my_wdt.h"
#include "helper_functions.h"

void send_fail()
{
	usartWriteChar(&AVR32_USART0, FAILURE_CHAR);
	set_wdt(7);
	BLOCK;
}

void send_success()
{
	usartWriteChar(&AVR32_USART0, SUCCESS_CHAR);
	set_wdt(7);
	BLOCK;
}

void run_test(Test test)
{
	usart_write("id=");
	usart_write(test.id);
	usart_write("&description=");
	usart_write(test.description);
	usart_write("&plan=");
	usart_write(test.plan);
	usart_write("&suite=");
	usart_write(test.suite);
	usart_write("&type=")
	usart_write( (test.type == TEST_FUNCTIONAL) ? ("f") : ("p") );
	usartWriteChar(&AVR32_USART0, 0);
	
	test.function();
}
	