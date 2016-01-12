/*
 * tests.c
 *
 * Created: 17.12.2015. 12:46:41 AM
 *  Author: Vukan
 */ 

#include "tests.h"
#include "drivers/my_wdt.h"
#include "helper_functions.h"

#define SMALL_RESET_TIME 12
#define STACK_PATTERN 0x55555555

void send_fail()
{
	usartWriteChar(&AVR32_USART0, FAILURE_CHAR);
	set_wdt(SMALL_RESET_TIME);
	BLOCK;
}

void send_success()
{
	usartWriteChar(&AVR32_USART0, SUCCESS_CHAR);
	set_wdt(SMALL_RESET_TIME);
	BLOCK;
}

void send_result(unsigned int number)
{
	char str[15];
	num2str(number, str);
	usart_write(str);
	usartWriteChar(&AVR32_USART0, 0);
	set_wdt(SMALL_RESET_TIME);
	BLOCK;
}

void test_sp();

void run_test(Test test)
{
	char str[50];
	
	//asm("pushm LR\nmov LR, SP\nnop\nnop\npopm LR");
	
	if(test.type == TEST_FUNCTIONAL)
	{
		usart_write("id=");
		usart_write(test.id);
		usart_write("&description=");
		usart_write(test.description);
		usart_write("&plan=");
		usart_write(test.plan);
		usart_write("&suite=");
		usart_write(test.suite);
		usart_write("&type=");
		
		if(test.type == TEST_FUNCTIONAL)
		{
			usart_write("f");
		}
		else if(test.type == TEST_SPEED_PERFORMANCE)
		{
			usart_write("sp");
		}
		else if(test.type == TEST_MEMORY_PERFORMANCE)
		{
			usart_write("mp");
		}
			
		usartWriteChar(&AVR32_USART0, 0);
		((void (*)())test.function)();
	}
	else
	{
		usart_write("id=");
		usart_write(test.id);
		usart_write("&description=");
		usart_write(test.description);
		usart_write("&plan=");
		usart_write(test.plan);
		usart_write("&suite=");
		usart_write(test.suite);
		usart_write("&type=");
		
		if(test.type == TEST_FUNCTIONAL)
		{
			usart_write("f");
		}
		else if(test.type == TEST_SPEED_PERFORMANCE)
		{
			usart_write("sp");
		}
		else if(test.type == TEST_MEMORY_PERFORMANCE)
		{
			usart_write("mp");
		}
		
		usart_write("&min=");
		usart_write(test.start_parameter);
		usart_write("&max=");
		usart_write(test.end_parameter);
		usartWriteChar(&AVR32_USART0, 0);
		
		usartGetLine(&AVR32_USART0, str);
		
		/*If the test doesn't use a parameter, just run it*/
		if(str[0] == '-')
		{
			((void (*)())test.function)();
		}
		
		/*If the test uses a parameter, run it with the received parameter*/
		else
		{
			((void (*)(unsigned int))test.function)(atoi(str));
		}
	}
}

void run_test_manual(Test test)
{
	if (test.type == TEST_FUNCTIONAL)
	{
		((void (*)())test.function)();
	}
	else 
	{
		((void (*)())test.function)();
	}
}

void run_test_manual_param(Test test, unsigned int number)
{
	((void (*)(unsigned int))test.function)(number);
}


void fill_stack(TaskHandle task)
{
	unsigned int i;
	
	int* stack = ((Task*)task)->t->dataarea - ((Task*)task)->t->datasize;
	
	for(i = 0; i < ((Task*)task)->t->datasize; i++)
	{
		stack[i] = STACK_PATTERN;
	}
}
unsigned int get_max_stack(TaskHandle task)
{
	unsigned int i = 0;
	
	int* stack = ((Task*)task)->t->dataarea - ((Task*)task)->t->datasize;
	
	while((stack[i] == STACK_PATTERN) && (i < ((Task*)task)->t->datasize)) i++;
	
	return ((Task*)task)->t->datasize - i;
}

extern unsigned int taskCounter;

unsigned int TestStack_ROSA_CreateTask(void (*functionBody) (void), char * functionNameChArr, unsigned int maxStackSize, unsigned int taskPriority, TaskHandle *taskHandle)
{
	
	//Disable interrupts
	interruptDisable();

	Task* task = malloc(sizeof(Task));
	// Check if memory was successfully allocated for task structure
	if (task == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	task->t = malloc(sizeof(tcb));
	// Check if memory was successfully allocated for task structure
	if (task->t == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	int *stack = malloc(sizeof(int)*maxStackSize);

	// Check if memory was successfully allocated for the maximum size of the stack
	if (stack == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}
	
	// Allocate memory for stack structure that holds temporary priorities
	task->temporaryPriority = createStack(MAX_NUMBER_SEMAPHORES);
	// Check if memory was successfully allocated for stack structure
	if (task->temporaryPriority == NULL)
	{
		return NOT_ENOUGH_MEMORY;
	}

	// Check if the length of the function name is valid
	unsigned int name_size = strlen(functionNameChArr);
	if (name_size > 4 || name_size < 1)
	{
		return INVALID_NAME;
	}

	// Check if the maximum number of tasks is already created
	if (++taskCounter > MAX_NUMBER_TASKS)
	{
		return TOO_MANY_TASKS;
	}

	// Check if the specified priority for the task is valid (different than 0 - the IDLE task priority)
	if (taskPriority == 0)
	{
		return INVALID_PRIORITY;
	}

	// Create TCB structure for task
	ROSA_tcbCreate(task->t, functionNameChArr, functionBody, stack, maxStackSize);
	
	/*TESTING IN PROGRESS*/
	fill_stack(task);
	/*END OF TESTING IN PROGRESS*/
		
	// Assign original priority to be equal to task priority
	task->originalPriority = taskPriority;
	// Put the newly created task in the READY queue
	putInREADYqueue(task);
	// Populate the task handle with a pointer to the newly created task
	*taskHandle = (TaskHandle*)task;

	// If the scheduler has been started check if CRT priority is lower than the new task priority and if so yeild
	if (isSchedulerStarted() == 1)
	{
		if (getPriority(getCRT()) < taskPriority)
		{
			putInREADYqueue(getCRT());
			ROSA_yield();
		}
	}
	
	// Enable interrupts
	interruptEnable();

	// Return SUCCESS
	return SUCCESS;
}

unsigned int TestStack_ROSA_CreateCyclicTask(void (*functionBody) (void), char * functionNameChArr, unsigned int maxStackSize, unsigned int taskPriority, ROSA_TickCount taskPeriod, ROSA_TickCount taskDeadline, TaskHandle *taskHandle)
{
	unsigned int res;
	
	res = ROSA_CreateCyclicTask(functionBody, functionNameChArr, maxStackSize, taskPriority, taskPeriod, taskDeadline, taskHandle);
	fill_stack(*taskHandle);
	return res;
}