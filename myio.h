/*
*	File: myio.h
*
*
*/


#ifndef _SERIAL_H
#define _SERIAL_H

#include <avr/io.h>
#include <avr/sleep.h>
#include "myTypes.h"


#define FOSC 16000000
#define BAUD 9600
#define BAUD_PRESCALER FOSC/16/BAUD-1

//we can use this to represent 3 bytes
//ie 24 bits
#define RX_SIZE 24
#define TX_SIZE 24



//max sensors
//can be set via serial
//uint8_t sensNo = 3;

enum a_pin {a1=1, a2, a3};

//used by sendBuff and readToBuff
 char rxBuff[RX_SIZE];
 uint8_t txBuff[TX_SIZE];

//for sendStr
int bSent;

void ioinit(void);
void adcinit(void);
void rxBlock(void);
void rxEnable(void);

uint8_t sendBuff(char[], int);
void readUDR(char[], volatile int*);

int sendStr(char*);
void putC(char);
void sendInt(int);

void readSensor(int, int[], int*);
void readSensorISR(int, int*);

#endif
