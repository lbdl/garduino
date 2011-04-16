/*
*	File:   isr.c
*   
*   Interrupt routines used in main.c
*	This file only actually defines the 
*	setup of the ISR's they need to be
*	defined in main for them to actually work
*	however we do the enabling of the various
*	registers here
*
*/
//#define DEBUG

#include "isr.h"
#include "myio.h"

#ifdef DEBUG
#include "myio.h"
#endif

//runtime for pump
int pTime = 5;


void ISRinit(void)
{
	//enable global interrupts
	//see interrupts.h
	sei();


	//enable RX interrupts
	UCSR0B |= 1 << RXCIE0;

	//8 bit timer1
	//set mode to CTC and source
	//to tap prescaler 64
	TCCR0A |= 1 << WGM01;
	TCCR0B |= 3;

	//set compare registerA
	OCR0A = 0xf9; //249

	//enable interrupt on compare to OCR0A
	TIMSK0 |= 1 << OCIE0A;

	//8 bit timer2
	

	//16 bit timer
}

void switching_proc(uint8_t *p, int m)
{	
	static int rTime = 0;
	
	if( !(*p))
	{
		rTime = (m + pTime)%60;
		*p = 1;
		//turn on pump Digital9
		PORTB |= (1 << PORTB1);
	}
	
	if(p)
	{
#ifdef DEBUG
		sendStr("\r\n m:");
		sendInt(m);
		sendStr("\r\n rTime: ");
		sendInt(rTime);
#endif
		//check that rTime done
		if(m == rTime)
		{
			*p = 0;
			//switch off pump Digital9
			PORTB &= ~(1 << PORTB1);
		}
	}
		
}






