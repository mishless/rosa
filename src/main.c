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

/********************* TESTS ************************/

/***** Task Manager Tests *****/
#define TM_CREATE_TESTS &tm_create_01, &tm_create_02, &tm_create_03, &tm_create_04, &tm_create_05, &tm_create_06, &tm_create_08, &tm_create_09, &tm_create_10
#define TM_CREATE_CYCLIC_TESTS	&tm_create_cyclic_01, &tm_create_cyclic_02, &tm_create_cyclic_03, &tm_create_cyclic_04, &tm_create_cyclic_05, &tm_create_cyclic_06, &tm_create_cyclic_08, &tm_create_cyclic_09, &tm_create_cyclic_10 
#define TM_TT_CREATE_TESTS &tm_tt_create_01, &tm_tt_create_02, &tm_tt_create_03, &tm_tt_create_04, &tm_tt_create_05, &tm_tt_create_06, &tm_tt_create_08, &tm_tt_create_09
#define TM_MIXED_CREATE_TESTS	&tm_mixed_create_01, &tm_mixed_create_02
#define TM_TERMINATE_TESTS		&tm_terminate_01, /*&tm_terminate_02,*/ &tm_terminate_03

#define TM_TESTS	TM_CREATE_TESTS, TM_CREATE_CYCLIC_TESTS, TM_TT_CREATE_TESTS, TM_MIXED_CREATE_TESTS, TM_TERMINATE_TESTS


/***** System Clock Manager Tests *****/
#define SCM_GET_TICKS_TESTS &scm_getTicks_01, &scm_getTicks_02
#define SCM_DELAY_RELATIVE_TESTS &scm_delayRelative_01, &scm_delayRelative_02, &scm_delayRelative_03, &scm_delayRelative_04, &scm_delayRelative_05, &scm_delayRelative_06, &scm_delayRelative_07, &scm_delayRelative_08, &scm_delayRelative_09
#define SCM_DELAY_ABSOLUTE_TESTS &scm_delayAbsolute_01, &scm_delayAbsolute_02, &scm_delayAbsolute_03, &scm_delayAbsolute_04, &scm_delayAbsolute_05, &scm_delayAbsolute_06, &scm_delayAbsolute_07, &scm_delayAbsolute_08	
#define SCM_END_CYCLE_TESTS &scm_endCycle_01, &scm_endCycle_02, &scm_endCycle_03, &scm_endCycle_04, &scm_endCycle_05, &scm_endCycle_06, &scm_endCycle_07, &scm_endCycle_08															

#define SCM_TESTS SCM_GET_TICKS_TESTS, SCM_DELAY_RELATIVE_TESTS, SCM_DELAY_ABSOLUTE_TESTS, SCM_END_CYCLE_TESTS


/***** Scheduler Tests *****/
#define SCH_TESTS		&s_schedule_01, &s_schedule_02, &s_schedule_03				

/***** Semaphore Manager tests *****/
#define SM_CREATE_BINARY_TESTS &sm_create_binary_01, &sm_create_binary_02, &sm_create_binary_05, &sm_create_binary_06, &sm_create_binary_07, &sm_create_binary_08, &sm_create_binary_09, &sm_create_binary_10
#define SM_CREATE_PRIORITY_TESTS &sm_create_prio_01, &sm_create_prio_02, &sm_create_prio_05, &sm_create_prio_06, &sm_create_prio_07, &sm_create_prio_08, &sm_create_prio_09, &sm_create_prio_10, &sm_create_prio_11
#define SM_CREATE_MIX_TESTS &sm_create_mix_01, &sm_create_mix_02
#define SM_DELETE_TESTS &sm_delete_03, &sm_delete_04
#define SM_TAKE_BINARY_TESTS &sm_take_binary_01, &sm_take_binary_02, &sm_take_binary_03, &sm_take_binary_04, &sm_take_binary_05, &sm_take_binary_06, &sm_take_binary_07
#define SM_TAKE_PRIORITY_TESTS &sm_take_prio_01, &sm_take_prio_02, &sm_take_prio_03, &sm_take_prio_04, &sm_take_prio_05, &sm_take_prio_06, &sm_take_prio_07, &sm_take_prio_10, &sm_take_prio_11, &sm_take_prio_12, &sm_take_prio_13, &sm_take_prio_14, &sm_take_prio_15
#define SM_GIVE_BINARY_TESTS &sm_give_binary_01, &sm_give_binary_02, &sm_give_binary_03, &sm_give_binary_04, &sm_give_binary_05
#define SM_GIVE_PRIORITY_TESTS &sm_give_priority_01, &sm_give_priority_02, &sm_give_priority_03, &sm_give_priority_04, &sm_give_priority_05

#define SM_TESTS SM_CREATE_BINARY_TESTS, SM_CREATE_PRIORITY_TESTS, SM_CREATE_MIX_TESTS, SM_DELETE_TESTS, SM_TAKE_BINARY_TESTS, SM_TAKE_PRIORITY_TESTS, SM_GIVE_BINARY_TESTS, SM_GIVE_PRIORITY_TESTS

/***** Integration Manager tests *****/
#define INTEGRATION_TESTS &it_01, &it_02

/***** Stress tests *****/
#define STRESS_TESTS &it_st_01, &it_st_02, &swedish_test

/***** Performance tests *****/
#define PERFORMANCE_TESTS &pt_sc_08

/***** All tests *****/
#define ALL_TESTS	TM_TESTS, SCM_TESTS, SCH_TESTS, SM_TESTS, INTEGRATION_TESTS, STRESS_TESTS

/****** Active test cases *****/
Test *tests[] = {&sm_take_prio_13};
	
int main(void){
	char c = 0;
	char str[MAX_STR_LEN];
	unsigned int test_to_run = -1;
	unsigned int task_num = sizeof(tests)/sizeof(int*);
	
	/*Set up the system*/
	disable_wdt();
	ROSA_init();
	set_wdt(19);
	
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
			run_test(*tests[test_to_run]);
		}
	}

}
