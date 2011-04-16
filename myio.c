/*
*   File: myio.c
*
*   implementation for myio.h
*   contains the function for basic io
*   and serial communuation over USART
*
*	NOTE, it is possible to use the AVR Libc
*	port of stdio.h  but it requires a lot of
*	space and processing to set up the macros
*	that the functions require hence the 
*	decision was made to implement our own
*	library as we dont need all the functionality
*	provided by stdio.h
*
*/


//for testing
//#define DEBUG

#include "myio.h"
#include <stdlib.h>
#include <string.h>




	uint8_t sendBuff(char c[], int ctr) 
	{

		int i;

		for(i = 0; i < ctr; i++)
		{
			  while ( !(UCSR0A & (1 << UDRE0)));//wait 
			  UDR0 = c[i]; 
			  
		}
		return i + 1;
	}


 void readUDR(char c[], volatile int *pos)
	{
		
		#ifdef DEBUG
			sendStr("function readUDR\n\r");
			char* str = "";
			sendStr("bPos: ");
			itoa(*pos,str,10);
			sendStr(str);
			sendStr("\n\r");
		#endif

		//make buffer circular we can then
		//do a check in main for the correct starting
		//char in main, if it isnt then we have an
		//error and can deal with it
		if(*pos == 23)
		{
			*pos = 0;
			rxBuff[*pos] = UDR0;
			(*pos)++;
		}
		
		rxBuff[*pos] = UDR0;
		(*pos)++;

	}

	void putC(char c)
	{
		//wait till ready to send
		while ( !(UCSR0A & (1 << UDRE0)));

		//send byte
		UDR0 = c;

		//increment count
		bSent++;

	}


	void sendInt(int i)
	{
		//we have to dynamicly allocate memory here
		//for the char* that will be copied to
		//in itoa()
		//also note here type int is in fact
		//a signed 16bit int see <stdint.h>

		//might be safer to use ltoa() and
		//do a cast before calling the function...


			//allocate the memory 
			char* str; 
			str = malloc(sizeof(i)+1);//+1 for NULL terminator
			itoa(i,str,10);
			sendStr(str);
			sendStr("\n\r");

			//free the memory
			free(str);
	}



	int sendStr(char* s)
	{
		bSent = 0;

		while(*s) putC(*s++);

		return bSent;
	}


	void rxEnable(void)
	{
		UCSR0B |= (1 << RXEN0);
	}

	void rxBlock(void)
	{
		UCSR0B &= ~(1 << RXEN0);
	}

	void readSensor(int pin, int vals[], int *vPos)
	{
		
		if(*vPos > MAX_VBUFF_SZ - 1)
			*vPos = 0;
		
		//set ADMUX input via pin
		ADMUX |= (pin << MUX0);
		ADCSRA |=(1<<ADSC);
		while( ! (ADCSRA & (1<<ADIF)));
		vals[*vPos] = ADCL;
		vals[*vPos] |= (ADCH << 8);
		(*vPos)++;
		ADCSRA |= (1 << ADIF);//reset ADIF flag
	}

	void readSensorISR(int pin, int *flag)
	{
		//set global flag 
		*flag = pin;
		ADMUX |= (pin << MUX0);
		ADCSRA |= (1 << ADIF);//reset ADIF flag to be sure
		ADCSRA |=(1<<ADIE);//enable ISR
		//set idle mode for ADC noise reduce
		SMCR |= (0 << SM0);
		sleep_cpu();//start the conversion
	}


	

	void adcinit(void)
	{
		//block digital on analogue pins
		DIDR0 = 0x3f;
		
		//setup ADR circuitry
		//ADCSRA |= ((1 << ADEN) | (1 << REFS0) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
		ADCSRA = ( (1<<ADEN) | (7 << ADPS0) | (1 << ADIF) );//using ISR	
		//vcc as ref
		ADMUX = 0x01;
		//ADMUX = 0x00;
	}


	//this needs to have default mode
	//which it currently is i.e if n other option
	//use these defaults
	void ioinit(void)
	{
		DDRC = INPUT;	//set to input
		DDRB = OUTPUT;  //set to output
		//DDRD = OUTPUT;
		
		//set LEDS off
		PORTB &= ~(1 << PORTB2);
		PORTB &= ~(1 << PORTB4);

		//init USART
		//set TX and RX
		UCSR0B |= (1<<TXEN0) | (1<<RXEN0);

		//set frame size, no parity 1 stop bit 8 data bits
		UCSR0C |= 3 << UCSZ00;

		//set baud rate
		UBRR0H = BAUD_PRESCALER >> 8;
		UBRR0L = BAUD_PRESCALER;
		

	}

