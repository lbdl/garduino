/*
*	T.E. Storey
*	10540411
*	For commons held throughout the app
*	
*/



#ifndef BITMASKS_H
#define BITMASKS_H



//to set pins
//actually allready defined
#define P1 0x01  // b00000001
#define P2 0x02  // b00000010
#define P3 0x04  // b00000100
#define P4 0x08  // b00001000
#define P5 0x10  // b00010000
#define P6 0x20  // b00100000
#define P7 0x40  // b01000000
#define P8 0x80  // b10000000

#define D2 0x04	//portD
#define	D3 0x08	//portD
#define	D4 0x10	//portD
#define	D5 0x20	//portD
#define	D6 0x40	//portD
#define	D7 0x80	//portD
#define	D8 0x01		//portB
#define	D9 0x02		//portB
#define	D10 0x04	//portB
#define	D11 0x08	//portB
#define	D12_HIGH PORTB = 0x10	//portB
#define	D12_LOW PORTB = 0x00	//portB
#define	D13 0x20	//portB

#define PINS_HIGH 0xff	// b11111111
#define PINS_LOW 0x00	// b00000000

#define INPUT 0x00
#define OUTPUT 0xff




#endif
