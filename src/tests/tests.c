/*
 * tests.c
 *
 * Created: 17.12.2015. 12:46:41 AM
 *  Author: Vukan
 */ 

#include "tests.h"
#include "drivers/my_wdt.h"

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