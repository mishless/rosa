/*
* stack.c
*
* Created: 12/17/2015 3:51:12 PM
* Author: Mihaela Stoycheva
*/
#include "stack.h"

#define NULL 0

Stack *createStack(unsigned int size) {
	Stack *stack = malloc(sizeof(Stack));
	if(stack == NULL)
		return NULL;
	
	stack->data = malloc(sizeof(int) * size);
	if(stack->data == NULL)
		return NULL;
		
	stack->size = size;
	stack->top = -1;
	
	return stack;
}

void destroyStack(Stack *stack) {
	free(stack->data);
	free(stack);
}

void pushIntoStack(Stack *stack, unsigned int element) {
	if (stack->top + 1 < stack->size) {
		stack->data[++stack->top] = element;
	}
}

unsigned int popFromStack(Stack *stack) {
	if (stack->top > -1) {
		unsigned int top = stack->data[stack->top--];
		return top;
	}
	
	/*Shouldn't get to this case ever*/
	return 0;
}

unsigned int isEmptyStack(Stack *stack) {
	if (stack->top > -1) {
		return 1;
	}
	return 0;
}