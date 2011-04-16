/*
*
*   File: main.c
*   
*   
*	
*	
*
*
*/


//debug info uncomment to send dubug info
//to USART
#define DEBUG

//#define ISRTEST




#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/atomic.h>

//#include "bitmasks.h"
#include "myio.h"
#include "sFuncs.h"
#include "isr.h"
#include "ui.h"
#include "myTypes.h"

/*===========GLOBALS=============*/

//offset indicator for rxBuff
volatile  int bPos = 0;

//hz counter
volatile uint16_t hz = 0;

//interrupt flags
volatile  uint8_t isr[ISR_MAX];

//handshake to start serial io
volatile uint8_t hs = 0;


/*=======INTERRUPT PROTOTYPES====*/

//ISR(_VECTOR(int));



int main()
{
	//we arent going to worry about the possibility of
	//a continous uptime of a 100 years here
	int sec=0;int min=0; int hr=0;

	//counters for value buffers
	int lPos = 0; int tPos = 0; int mPos = 0;
	
	//counter for polling control
	uint8_t ctr = 0;
	
	//flag for pump
	uint8_t pump = 0;
	
	//flags for ui
	uint8_t ui[3] = {0,0,0};
	
	//buffers for values
	int lBuff[MAX_VBUFF_SZ]; int mBuff[MAX_VBUFF_SZ]; int tBuff[MAX_VBUFF_SZ];
	
	//init buffers
	clrBuff(lBuff, MAX_VBUFF_SZ);
	clrBuff(mBuff, MAX_VBUFF_SZ);
	clrBuff(tBuff, MAX_VBUFF_SZ);
	
	//avg values
	int avgL = 0; int avgM = 0; int avgT = 0;

	//set interrupts on
	ISRinit();

	ioinit();
	//do ADC init
	adcinit();

	for(;;)
	{
	
		if(isr[0])//check flag for Rx
		{
			
			//setup menu if first RX
			if(! ui[0])
			{
				main_menu2();
				ui[0] = 1;
			}
			
			//block Rx while we read from buffer
			rxBlock();
			
			
					
			//read from rxBuff
			//we have a long buffer in case of a 
			//failed Tx's and for future use.
			//we just increment the counter and hence the
			//buffer offset after a read
			//we are only interested in the last byte received
			//as this is our control

			char c = rxBuff[bPos-1];//ie the last char recieved
			switch (c) 
			{
				case 'o':
					overide_msg();
				if(!pump)
						switching_proc(&pump, min);
					else 
						switching_proc(&pump, min);
					rxEnable();
					break;
				case 'd':
					data_menu();
					rxEnable();
					break;
				case 't':
					for(int i = 0; i << 7; i++)
					{	
						readSensor(temp, tBuff, &tPos);
					}
					temp_disp();
					//sendInt(avg(tBuff, tPos));
					sendInt(ADCToTemp( avg(lBuff, lPos), 0, 10 ) );
					sendStr("\r\n");
					main_menu2();
					rxEnable();
					break;
				case 'l':
					for(int i = 0; i < 7; i++)
					{
						readSensor(light, lBuff, &lPos);
					}
					light_disp();
					//sendInt(avg(lBuff, lPos));
					sendInt(adc_to_light( avg(lBuff, lPos) ) );
					sendStr("\r\n");
					main_menu2();
					rxEnable();
					break;
				case 'm':
					readSensor(moist, mBuff, &mPos);
					moist_disp();
					//sendInt(mBuff[mPos]);
					sendInt(adc_to_moist( avg(mBuff, mPos) ) );
					sendStr("\r\n");
					main_menu2();
					rxEnable();
					break;
				default:
					main_menu2();
					rxEnable();
					break;
			}
			
			//reset flag
			isr[0] = 0;
		}

		if(isr[1])//0.001 sec passed
		{					
			
			do
			{
				 ATOMIC_BLOCK(ATOMIC_FORCEON)
				 {
					int cpHz = hz;
					
				
					 
					//seconds
					if( !(cpHz%1000)  )
					{
						//clear the hz counter if too big
						//as we have declared it int
						//so max val = 0xff
						if(cpHz > 0x7fff)
							hz = 0;
						
						//toggle LED 12
						PORTB ^= (1 << PORTB4);
						
						//LED interface
						if(pump)
						{
							//synch the LED'S
							if(PORTB & (1 << PORTB4))
								PORTB |= (1 << PORTB2);
							else 
								PORTB &= ~(1 << PORTB2);
							
							//toggle LED
							PORTB ^= (1 << PORTB2);
						}
								
							

							
						//}
						else
						{
							PORTB &= ~(1 << PORTB2);
						}
						
						//increase seconds
						sec++;
						
						//reneable interrupts while we deal with
						//tasks at the second mark sending data
						NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE)
						{
							//sec++;
							if (!(sec%60)) 
							{
								sec = 0;
								min++;
								if (!(min%60)) 
								{
									min = 0;
									hr++;
								}
							}
							
							//process for dry conditions
							if ( (avgM > 400) && (!(pump) ) )
							{
								switching_proc(&pump, min);
							}
							else if(pump)
							{
								switching_proc(&pump, min);
							}
							
							#ifdef DEBUG
							//send data over serial
							//if serial on
							if (hs)//serial on ?
							{
									
								//sendStr("\rSeconds: ");
								//sendInt(sec);
								sendStr("\r\nMins: ");
								sendInt(min);
								sendStr("\r\nAVG T: ");
								sendInt(avgT);
								sendStr("\r\nAVG M ADC: ");
								sendInt(avgM);
								sendStr("\r\nAVG L ADC: ");
								sendInt(avgL);
								
							}
							#endif
						}
					}
					 
					 //0.1 seconds
					 if( !(cpHz%100) )
					 {
						 if(!(pump))
						 {
							 //calculate values and averages
							 avgT = ADCToTemp(avg(tBuff, MAX_VBUFF_SZ), 0, 10);
							 avgM = avg(mBuff, MAX_VBUFF_SZ);
							 avgL = avg(lBuff, MAX_VBUFF_SZ);
						 }

						 
					 }//end 0.1
					 
					 //0.01 seconds
					 if (!(cpHz%10)) 
					 {
						 NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE)
						 {
							 
							 //poll sensors and assign values to
							 //respective arrays we can only
							 //currently read from 1 signal
							 //as the circuit gets too noisy
							 //with several ADC conversions
							 //happening at once
							 if(!(pump))
								 readSensor(moist, mBuff, &mPos);
								 //readSensor(light, lBuff, &lPos);
								 //readSensor(temp, tBuff, &tPos);
						 }
					 }//end 0.01 seconds
					 
					 //reset flag
					 isr[1] = 0;
				}
			
			}while(isr[1]);	
		}
		
	}

	}//end main loop



//ISR for recieve
ISR(USART_RX_vect)
{
	//set flag
	isr[0] = 1;
	
	//set handshake flag
	hs = 1;
	
	//read from the UDR0 buffer
	readUDR(rxBuff, &bPos);
	
}

//ISR at 1000hz for polling sensors and 
//general operations
ISR(TIMER0_COMPA_vect)
{
	//set flag
	isr[1] = 1;

	//increment hz count
	hz++;
}



