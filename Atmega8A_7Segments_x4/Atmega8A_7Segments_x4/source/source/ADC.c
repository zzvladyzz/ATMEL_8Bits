/*
 * ADC.c
 *
 * Created: 3/3/2021 4:53:58 PM
 *  Author: vladi
 */ 

#include "../headers/ADC.h"
void Init_ADC()
{
	
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
	
}




