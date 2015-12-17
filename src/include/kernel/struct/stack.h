/*
* stack.h
*
* Created: 12/17/2015 3:43:28 PM
* Author: Mihaela Stoycheva
*/


#ifndef STACK_H_
#define STACK_H_

typedef struct {
	unsigned int *data;
	int top;
	unsigned int size;
} Stack;

Stack *createStack(unsigned int size);
void destroyStack(Stack *stack);
void pushIntoStack(Stack *stack, unsigned int element);
unsigned int popFromStack(Stack *stack);
unsigned int isEmptyStack(Stack *stack);

#endif /* STACK_H_ */
