/*
 * my_timer.c
 *
 * Created: 08.01.2016. 2:32:24 PM
 *  Author: Vukan
 */ 

#define TC_BASE 0xFFFF3800
#define TIMER1_OFFSET 0x40
#define TIMER1_ADDRESS (TC_BASE + TIMER1_OFFSET)
#define CCR_OFFSET 0x00
#define CCR_ADDRESS (TIMER1_ADDRESS + CCR_OFFSET)
#define CMR_OFFSET 0x04
#define CMR_ADDRESS (TIMER1_ADDRESS + CMR_OFFSET)
#define CV_OFFSET 0x10
#define CV_ADDRESS (TIMER1_ADDRESS + CV_OFFSET)
#define CMR (*((volatile unsigned int *)(CMR_ADDRESS)))
#define CCR (*((volatile unsigned int *)(CCR_ADDRESS)))
