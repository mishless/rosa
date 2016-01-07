/*****************************************************************************

                 ,//////,   ,////    ,///' /////,
                ///' ./// ///'///  ///,    ,, //
               ///////,  ///,///   '/// //;''//,
             ,///' '///,'/////',/////'  /////'/;,

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

#include "rosa_config.h"
#include "drivers/delay.h"
#include "kernel/rosa_int.h"
#include "rosa_queue_manager.h"
#include "rosa_scheduler_private.h"

/***********************************************************
 * timerInterruptHandler
 *
 * Comment:
 * 	This is the basic timer interrupt service routine.
 **********************************************************/

ROSA_TickCount round_robin_counter = 0;

#define ROSA_yieldFromISRFixed() asm("sub SP, -0x4");\
								 ROSA_yieldFromISR();\
								 asm("sub SP, 0x4")

__attribute__((__interrupt__))
void timerISR(void)
{
	int sr;
	volatile avr32_tc_t * tc = &AVR32_TC;

	//Read the timer status register to determine if this is a valid interrupt
	sr = tc->channel[0].sr;
	if(sr & AVR32_TC_CPCS_MASK)
	{
		systemTime++;
		while(1)
		{
			if(isDELAYqueueEmpty() == 1) break;
			if((peekDELAYqueue())->wakeUpTime > systemTime) break;
			putInREADYqueue(getFromDELAYqueue());
		}
	
		if(isREADYqueueEmpty() == 0)
		{
			if(getPriority(peekREADYqueue()) > getPriority(getCRT()))
			{
				putInREADYqueue(getCRT());
				ROSA_yieldFromISRFixed();
			}
			
			else if (isEmptyStack(((Task*)getCRT())->temporaryPriority))
			{
				if(getPriority(peekREADYqueue()) == getPriority(getCRT()))
				{
					if(round_robin_counter == ROUND_ROBIN_PERIOD)
					{
						putInREADYqueue(getCRT());
						ROSA_yieldFromISRFixed();
					}
					else
					round_robin_counter++;
				}
			}				
		}
	}
}


/***********************************************************
 * timerPeriodSet
 *
 * Comment:
 * 	Set the timer period to 'ms' milliseconds.
 *
 **********************************************************/
int timerPeriodSet(unsigned int ms)
{
	int rc, prescale;
	int f[] = { 2, 8, 32, 128 };
	//FOSC0 / factor_prescale * time[s];
	prescale = AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK5;
	rc = FOSC0 / f[prescale - 1] * ms / 1000;
	timerPrescaleSet(prescale);
	timerRCSet(rc);
	return rc * prescale / FOSC0;
}
