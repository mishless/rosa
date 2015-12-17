/*
* stack.c
*
* Created: 12/17/2015 3:51:12 PM
* Author: Mihaela Stoycheva
*/
#include "stack.h"

Stack *createStack(unsigned int size) {
	Stack *stack = malloc(sizeof(Stack));
	stack->data = malloc(sizeof(int) * size);
	stack->size = size;
	stack->top = -1;
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
}

unsigned int isEmptyStack(Stack *stack) {
	if (stack->top > -1) {
		return 1;
	}
	return 0;
}