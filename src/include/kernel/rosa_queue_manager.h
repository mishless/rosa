/*
 * rosa_queue_manager.h
 *
 * Created: 12/16/2015 6:58:38 PM
 *  Author: Kolja
 */ 


#ifndef ROSA_QUEUE_MANAGER_H_
#define ROSA_QUEUE_MANAGER_H_

void putInREADYqueue( Task* task );
Task* getFromREADYqueue( void );
Task* peekREADYqueue( void );
unsigned int isREADYqueueEmpty( void );


#endif /* ROSA_QUEUE_MANAGER_H_ */