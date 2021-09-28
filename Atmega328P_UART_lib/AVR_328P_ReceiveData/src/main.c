/*
 * AVR_328P_ReceiveData.c
 *
 * Created: 4/12/2021 3:57:40 AM
 * Author : vladi
 */ 

#define F_CPU 8000000ul
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "header/USART_328P.h"

ISR(USART_RX_vect);
int main(void)
{
    /* Replace with your application code */
    DDRB=0xFF;
	USART_init(9600,1,1,1,"none",0,0,8);
	while (1) 
    {
	for (int i=0;i<5;i++)
	{
		USART_TransmitData(i);
		_delay_ms(500);
	}
    }
}
ISR(USART_RX_vect)
{
	PORTB=UDR0;
}
