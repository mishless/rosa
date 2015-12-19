/*****************************************************************************

                 ,//////,   ,////    ,///' /////,
                ///' ./// ///'///  ///,    ,, //
               ///////,  ///,///   '/// ///''\\,
             ,///' '///,'/////',/////'  /////'\\,

    Copyright 2010 Marcus Jansson <mjansson256@yahoo.se>

    This file is part of ROSA - Realtime Operating System for AVR32.

    ROSA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ROSA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ROSA.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/
/* Tab size: 4 */

//Standard library includes
#include <avr32/io.h>

//Kernel includes
#include "kernel/rosa_ker.h"

//Driver includes
#include "drivers/led.h"
#include "drivers/delay.h"
#include "drivers/usart.h"

//Include configuration
#include "rosa_config.h"

#include "util/helper_functions.h"
#include "drivers/my_wdt.h"
#include "rosa_api_call.h"
#include "tests.h"

#define TASK_NUM_CHAR 255
#define MAX_STR_LEN 30

unsigned int global_i;

#define TM_CREATE_TESTS tm_create_01, tm_create_02, tm_create_03, tm_create_04, tm_create_05, tm_create_06, tm_create_07, \
			 tm_create_08, tm_create_09, tm_create_10, tm_create_11, tm_create_12, tm_create_13, tm_create_14, \
			 tm_create_15, tm_create_16, tm_create_17, tm_create_18, tm_create_19, tm_create_20, tm_create_21, tm_create_22
#define TM_TERMINATE_TESTS tm_terminate_01, /*tm_terminate_02, */tm_terminate_03

#define SCM_GET_TICKS_TESTS scm_getTicks_01, scm_getTicks_02
#define SCM_DELAY_RELATIVE_TESTS scm_delayRelative_01, scm_delayRelative_02, scm_delayRelative_03, scm_delayRelative_04, \
								scm_delayRelative_05, scm_delayRelative_06, scm_delayRelative_07, scm_delayRelative_08, \
								scm_delayRelative_09
#define SCM_DELAY_ABSOLUTE_TESTS scm_delayAbsolute_01, scm_delayAbsolute_02, scm_delayAbsolute_03, scm_delayAbsolute_04, \
								scm_delayAbsolute_05, scm_delayAbsolute_06, scm_delayAbsolute_07, scm_delayAbsolute_08																
#define SCM_TESTS SCM_GET_TICKS_TESTS, SCM_DELAY_RELATIVE_TESTS, SCM_DELAY_ABSOLUTE_TESTS

#define ALL_TESTS TM_CREATE_TESTS, TM_TERMINATE_TESTS, SCM_TESTS
							
/*Pointers to all the test cases*/
void (*tests[])(void) = {scm_delayRelative_02};

int main(void){
	char c = 0;
	char str[MAX_STR_LEN];
	unsigned int test_to_run = -1;
	unsigned int task_num = sizeof(tests)/sizeof(int*);
	
	/*Set up the system*/
	disable_wdt();
	ROSA_init();
	set_wdt(17);
	
	/*Start the communication*/
	while(1)
	{
		usartWriteChar(&AVR32_USART0, '?');

		while((c = usartGetChar(&AVR32_USART0)) == 0);
			
		/*Does PC want the number of tests?*/
		if(c == '#')
		{
			/*Tell the PC the number of tests*/
			num2str(task_num, str);
			usartWriteLine(&AVR32_USART0, str);
			usartWriteChar(&AVR32_USART0, 0);
		}
			
		/*The PC sent a test number to run*/
		else if(c == '%')
		{
			/*Get the desired test from the PC*/
			usartGetLine(&AVR32_USART0, str);		
			test_to_run = atoi(str);
			
			/*Run the selected test*/
			tests[test_to_run]();
		}
	}

}
