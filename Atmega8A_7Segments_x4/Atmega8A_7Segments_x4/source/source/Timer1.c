/*
 * Timer1.c
 *
 * Created: 3/3/2021 4:04:03 PM
 *  Author: vladi
 */ 

#include <avr/io.h>
#include "../headers/Timer1.h"
#define F 8000000ul

void Init_Timer1(uint8_t time_T1)
{
	TCCR1A&=~((1<<WGM11)|(1<<WGM10));
	TCCR1B|=(1<<WGM12)|(1<<CS12)|(1<<CS10);
	TCCR1B&=~(1<<CS11);
	TIMSK|=(1<<OCIE1A);
	OCR1A=(time_T1*(F/1000/1024))-1;
}

void Init_Timer0()
{
	TCCR0|=(1<<CS01)|(1<<CS00);
	TIMSK|=(1<<TOIE0);
}

