/*
 * IncFile1.h
 *
 * Created: 14.12.2015. 7:40:28 PM
 *  Author: Vukan
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

/*Macro for entering an infinite loop*/
#define BLOCK while(1);

/*A function that converts a number to string*/
void num2str(unsigned int num, char*str);

/*A function that performs a busy wait. It is not very precise*/
void busy_wait(int milliseconds);

#endif /* INCFILE1_H_ */