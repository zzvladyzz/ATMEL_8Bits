
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "headers/R2R.h"
uint8_t grafica=0;
uint8_t selector=0;
int main(void)
{
	_Init_T();
	while (1) 
    {
		OCR1A=10;
    }
}
ISR(INT0_vect)
{
	selector++;
	if(selector>3)
	{
		selector=0;
	}
}
ISR(TIMER1_COMPA_vect)
{
	R2R(grafica,selector);
	grafica++;
	if (grafica>255)
	{
		grafica=0;
	}
}


