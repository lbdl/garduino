/*
*   File: isr.h
*
*   interrupts and their vars and definitions 
*	also routines to run after interrupts have fired
*
*   note: all the vectors for interrupts are defined at the bottom
*   of iom328p.h we could just use this file but we will keep to the
*   use of io.h as it allows us to decouple for further work on 
*   differing atmel chips...
*
*   #define ISR_MAX is the number of interrupts defined
*   used in main to set a array of interrupts allowing us to change 
*   behavior dependant on interrupts firing 

*	
*/

#ifndef _ISR_H
#define _ISR_H

//set max ISR's here
#define ISR_MAX 4


#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdint.h>





void ISRinit(void);

void switching_proc(uint8_t*, int);

void tx_procs();

ISR(_VECTOR(int));



#endif
