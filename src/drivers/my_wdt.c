/*
 * my_wdt.c
 *
 * Created: 17.12.2015. 10:23:22 PM
 *  Author: Vukan
 */ 

#include "drivers/my_wdt.h"

#define WDT_CONTROL (*((volatile unsigned int*) 0xFFFF0D30))
#define WDT_CLEAR_OFFSET 0x04
#define WDT_CLEAR (*((volatile unsigned int*) (0xFFFF0D30+WDT_CLEAR_OFFSET)))

void set_wdt(unsigned int psel)
{
	WDT_CONTROL = (0x55 << 24) | (psel << 8) | 0x1;
	WDT_CONTROL = (0xAA << 24) | (psel << 8) | 0x1;
}

void disable_wdt()
{
	WDT_CONTROL = (0x55 << 24) | (31 << 8);
	WDT_CONTROL = (0xAA << 24) | (31 << 8);
}

void reset_board()
{
	set_wdt(10);
}