/*
 * Atmega8A_7Segments_x4.c
 *
 * Created: 3/3/2021 11:47:58 AM
 * Author : vladi
 */ 
#define F_CPU 8000000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include "headers/Segments.h"
#include "headers/Timer1.h"
#include "headers/ADC.h"
#include <util/delay.h>

uint8_t selector=0;
uint8_t a=0;
uint16_t ADC_value=0;
int main(void)
{
	cli();
	Init_Segment();
	Init_Timer1(100);		// adc
	Init_Timer0();	// display
	Init_ADC();
	sei();
	/* Replace with your application code */
	while (1)
	{
		
		
		
	}
}
ISR(TIMER1_COMPA_vect)
{
	ADCSRA|=(1<<ADSC);
	while(!(ADCSRA&(1<<ADIF)));
	ADCSRA&=~(1<<ADIF);
	ADC_value=ADC;	
}

ISR(TIMER0_OVF_vect)
{
	Display_7_Seg(ADC_value,selector);
	selector++;
	if (selector>3)
	{
		selector=0;
	}
}









