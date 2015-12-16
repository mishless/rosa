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

#include "tests.h"

#define TASK_NUM_CHAR 255
#define MAX_STR_LEN 30


unsigned int global_i;


/*Pointers to all the test cases*/
void (*tests[])(void) = {tm_create_01, tm_create_02};

int main(void)
{
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
