#include <stdint.h>

#ifndef _SENSORFUNCTIONS_H
#define	_SENSORFUNCTIONS_H



//constants for the thermistor interpolation
#define TEMP_LUT_SIZE 9
#define LIGHT_LUT_SZ 5
#define MOIST_LUT_SZ 4
#define TEMP_ZERO -10
#define DELTA_T 10




uint8_t ADC_light(int);
uint8_t ADC_moist(int);

int16_t ADCToTemp( int, int, int);

int adc_to_light(int);
int adc_to_moist(int);

int avg(int[], int);
void clrBuff(int[], const int);

#endif
