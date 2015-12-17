/*
 * my_wdt.h
 *
 * Created: 14.12.2015. 5:50:05 PM
 *  Author: Vukan
 */ 


#ifndef MY_WDT_H_
#define MY_WDT_H_

void set_wdt(unsigned int psel);
void disable_wdt();
void reset_board();

#endif /* MY_WDT_H_ */