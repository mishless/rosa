/*
 * task_manager_tests.c
 *
 * Created: 16.12.2015. 7:55:47 PM
 *  Author: Vukan
 */ 

#include <avr32/io.h>
#include "task_manager_tests.h"
#include "helper_functions.h"
#include "rosa_api_call.h"
#include "rosa_task_private.h"
#include "rosa_ker.h"
#include "tests.h"

#define TEST_PTR (void*) 42
#define TASK_NAME "test"
#define STACK_SIZE 128
#define TEST_PRIORITY 6

void tm_create_01()
{
	/*ROSA_CreateTask should allocate and populate the task structure 
	  correctly and assign it to the provided handle.*/
	
	TaskHandle task_handle;
	char task_name[4] = TASK_NAME;
	
	send_id("TM-CREATE-01");
	ROSA_CreateTask(TEST_PTR, task_name, STACK_SIZE, TEST_PRIORITY, &task_handle);
	
	if( ((Task*)task_handle)->t->staddr != TEST_PTR)
	{		
		send_fail();
		BLOCK;
	}
	if( compare_strings(task_name, ((Task*)task_handle)->t->id, 4) != 0)
	{
		send_fail();
		BLOCK;
	}
	if( ((Task*)task_handle)->t->datasize != STACK_SIZE)
	{
		send_fail();
		BLOCK;
	}
	if( ((Task*)task_handle)->originalPriority != TEST_PRIORITY)	
	{		
		send_fail();
		BLOCK;
	}
	
	send_success();
	BLOCK;
}

void tm_create_02()
{
	send_id("TM-CREATE-02");
	
	send_success();
}