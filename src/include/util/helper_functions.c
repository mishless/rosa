/*
 * CFile1.c
 *
 * Created: 14.12.2015. 7:40:23 PM
 *  Author: Vukan
 */ 

#include "helper_functions.h"
#include <avr32/io.h>
#include "drivers/usart.h"

#define CYCLES_PER_MS 12000000/11

void busy_wait(int milliseconds)
{
	long volatile cycles = (milliseconds * CYCLES_PER_MS);
	while (cycles != 0)
	{
		cycles--;
	}
}

void num2str(unsigned int num, char*str)
{
	unsigned int exp = 1;
	unsigned int digit;
	if(num == 0)
	{
		str[0] = '0';
		str[1] = 0;
		return;
	}
	while(exp <= num) exp*= 10;
	exp/=10;
	
	do
	{
		digit = num/exp;
		*str++ = digit + '0';
		num = num % exp;
		exp/=10;
	} while (exp != 0);
	*str = 0;
}


void send_id(char* id)
{
	usartWriteLine(&AVR32_USART0, id);
	usartWriteChar(&AVR32_USART0, 0);
}

int compare_strings(char* str1, char* str2, unsigned int n)
{
	unsigned int i;
	
	for(i = 0; i < n; i++)
	{
		if(*str1++ != *str2++) return -1;
	}
	
	return 0;
}

unsigned int strlen(char* str)
{
	unsigned int i = 0;
	while(*str++ != 0) i++;
	return i;
}