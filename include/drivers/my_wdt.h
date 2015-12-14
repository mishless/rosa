/*
 * my_wdt.h
 *
 * Created: 14.12.2015. 5:50:05 PM
 *  Author: Vukan
 */ 


#ifndef MY_WDT_H_
#define MY_WDT_H_

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

#endif /* MY_WDT_H_ */