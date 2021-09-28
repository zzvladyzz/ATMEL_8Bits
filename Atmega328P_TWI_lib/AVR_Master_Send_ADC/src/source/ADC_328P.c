/*
 * ADC_328P.c
 *
 * Created: 4/4/2021 10:56:24 PM
 *  Author: vladi
 */ 

#include "../header/ADC_328P.h"


void ADC_Init()
{
	ADMUX|=(1<<REFS0)|(1<<ADLAR)|(1<<MUX1);
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
}

uint8_t ADC_ReadValue()
{
	ADCSRA|=(1<<ADSC);
	while(!(ADCSRA&(1<<ADIF)));
	//ADCSRA&=~(1<<ADIF);
	return ADCH;
}

