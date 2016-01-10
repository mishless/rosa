/*
 * tests.c
 *
 * Created: 17.12.2015. 12:46:41 AM
 *  Author: Vukan
 */ 

#include "tests.h"
#include "drivers/my_wdt.h"
#include "helper_functions.h"

#define SMALL_RESET_TIME 12

void send_fail()
{
	usartWriteChar(&AVR32_USART0, FAILURE_CHAR);
	set_wdt(SMALL_RESET_TIME);
	BLOCK;
}

void send_success()
{
	usartWriteChar(&AVR32_USART0, SUCCESS_CHAR);
	set_wdt(SMALL_RESET_TIME);
	BLOCK;
}

void send_result(unsigned int number)
{
	char str[15];
	num2str(number, str);
	usart_write(str);
	usartWriteChar(&AVR32_USART0, 0);
	set_wdt(SMALL_RESET_TIME);
	BLOCK;
}

void test_sp();

void run_test(Test test)
{
	char str[50];
	
	//asm("pushm LR\nmov LR, SP\nnop\nnop\npopm LR");
	
	if(test.type == TEST_FUNCTIONAL)
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
		((void (*)())test.function)();
	}
	else if(test.type == TEST_PERFORMANCE)
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
		usart_write("&min=");
		usart_write(test.start_parameter);
		usart_write("&max=");
		usart_write(test.end_parameter);
		usartWriteChar(&AVR32_USART0, 0);
		
		usartGetLine(&AVR32_USART0, str);
		
		/*If the test doesn't use a parameter, just run it*/
		if(str[0] == '-')
		{
			((void (*)())test.function)();
		}
		
		/*If the test uses a parameter, run it with the received parameter*/
		else
		{
			((void (*)(unsigned int))test.function)(atoi(str));
		}
	}
}

void run_test_manual(Test test)
{
	((void (*)())test.function)();
}