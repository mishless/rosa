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
#define TIMER1_CV (*((volatile unsigned int *)(CV_ADDRESS)))
#define SOFTWARE_TRIGGER_VALUE 0x5

#define TIMER_DECLARE() register unsigned int software_trigger = 0x5;\
						register volatile unsigned int* tc_ccr_addr = (volatile unsigned int*)CCR_ADDRESS;\
						register volatile unsigned int* cv_address = (volatile unsigned int*)CV_ADDRESS
						
#define TIMER_INIT() 	TIMER1_CMR = 0x00008001;\
						TIMER1_CCR = 0x00000001
						
#define TIMER_START() TIMER1_CCR = SOFTWARE_TRIGGER_VALUE
#define TIMER_VALUE() TIMER1_CV

extern Test pt_sc_01;
extern Test pt_sc_02;
extern Test pt_sc_03;
extern Test pt_sc_04;
extern Test pt_sc_05;
extern Test pt_sc_06;
extern Test pt_sc_07;
extern Test pt_sc_08;
extern Test pt_sc_09;
extern Test pt_sc_10;
extern Test pt_sc_11;
extern Test pt_sc_12;
extern Test pt_sc_13;
extern Test pt_sc_14;
extern Test pt_sc_15;
extern Test pt_sc_16;
extern Test pt_sc_17;
extern Test pt_sc_18;
extern Test pt_sc_19;
extern Test pt_sc_20;
extern Test pt_sc_21;
extern Test pt_sc_22;
extern Test pt_sc_23;
extern Test pt_sc_24;
extern Test pt_sc_25;

extern Test dummy_test;

#endif /* PERFORMANCE_TESTS_H_ */