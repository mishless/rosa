/*
 * performance_tests.h
 *
 * Created: 08.01.2016. 2:21:29 PM
 *  Author: Vukan
 */ 


#ifndef PERFORMANCE_TESTS_H_
#define PERFORMANCE_TESTS_H_

#define TC_BASE 0xFFFF3800
#define TIMER1_OFFSET 0x40
#define TIMER1_ADDRESS (TC_BASE + TIMER1_OFFSET)
#define CCR_OFFSET 0x00
#define CCR_ADDRESS (TIMER1_ADDRESS + CCR_OFFSET)
#define CMR_OFFSET 0x04
#define CMR_ADDRESS (TIMER1_ADDRESS + CMR_OFFSET)
#define CV_OFFSET 0x10
#define CV_ADDRESS (TIMER1_ADDRESS + CV_OFFSET)
#define TIMER1_CMR (*((volatile unsigned int *)(CMR_ADDRESS)))
#define TIMER1_CCR (*((volatile unsigned int *)(CCR_ADDRESS)))

#define TIMER_INIT() 	register unsigned int software_trigger = 0x5;\
						register volatile unsigned int* tc_ccr_addr = (volatile unsigned int*)CCR_ADDRESS;\
						register volatile unsigned int* cv_address = (volatile unsigned int*)CV_ADDRESS;\
						TIMER1_CMR = 0x00008001;\
						TIMER1_CCR = 0x00000001
						
#define TIMER_START() *tc_ccr_addr = software_trigger
#define TIMER_VALUE() *cv_address

extern Test pt_sc_08;

#endif /* PERFORMANCE_TESTS_H_ */