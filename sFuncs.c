#include "sFuncs.h"
#include <stdint.h>
#include "myIo.h"

//the lookup table for calculated ADC readings based on 
//thermistor resistance at 10 given temperatures
//with a 10 deg rise between points
// i.e deltaT = 10
//start temp = 0
const int LUT_Therm[TEMP_LUT_SIZE] =
	{866, 786, 547, 550, 433, 332, 240, 171, 128};

const int LUT_Light[LIGHT_LUT_SZ] =
{990, 770, 680, 600, 500};

const int LUT_Moist[MOIST_LUT_SZ] =
{1000, 600, 400, 200};


//uses linear interpolation to find the value between 2 points
//in the LUT and then adds this value to the offset from the
//start of the LUT 
//This value can be seen to be value y in the following equation
// y = y1 + [(y2 - y1)/(x2 - x1).(x - x1)
int16_t ADCToTemp( int adc, int T0, int dT )
{
	if(adc == 0)
		return 0;
	
	int temp;
	int i;
	int max, min;

	//find adjacent points in the LUTe
	for ( i = 0; (i < TEMP_LUT_SIZE) && (adc < (LUT_Therm[i])); i++)
	{	
		temp = 0;//zero temp on each iteration
		max = LUT_Therm[i];
		min = LUT_Therm[i+1];

		temp = (max - adc) * dT;//(x - x1).(y2 -y1) step 1 interpolation
		temp /= max - min;// [(x - x1).(y2 -y1)}/(x2 - x1)
		temp += (i-1)*dT+T0;// + y1
	}
	return temp;
}

int adc_to_light(int adc)
{
	int max, min, i;
	
	for ( i = 0; (i < LIGHT_LUT_SZ) && (adc < (LUT_Light[i])); i++)
	{
		max = LUT_Light[i];
		min = LUT_Light[i+1];
	}
	return i;
}

int adc_to_moist(int adc)
{
	int max, min, i;
	
	for ( i = 0; (i < MOIST_LUT_SZ) && (adc < (LUT_Moist[i])); i++)
	{
		max = LUT_Moist[i];
		min = LUT_Moist[i+1];
	}
	return i;
}

int avg(int b[],int m)
{
	if(m == 0)
		return 0;
	int tmp = 0;
		for (int i = 0; i < m; i++)
		{
			tmp += b[i];
		}
	tmp /= m;
	return tmp;
}

void clrBuff(int b[], const int s)
{
	for (int i = 0; i < s; i++) 
	{
		b[i] = 0;
	}
}

	
