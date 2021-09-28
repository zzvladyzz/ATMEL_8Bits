/*
 * AVR_328P_UART.c
 *
 * Created: 4/11/2021 10:23:20 PM
 * Author : vladi
 */ 
#define F_CPU 16000000ul
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "header/USART_328P.h"

int main(void)
{
    /* Replace with your application code */
	USART_init(9600,1,1,1,"none",1,0,8);
	char hola[]="hola\n\r";
	DDRB=0xFF;
	
	while (1) 
    {
	//for (int i=0;i<10;i++)
	//{
		//USART_TransmitData(i);
		//_delay_ms(1000);
		//PORTB^=(1<<PORTB5);
	//}	
	USART_TransmitString(hola,strlen(hola));
	_delay_ms(1000);
	PORTB^=(1<<PORTB5);
    //}
	
	_delay_ms(3000);
	}
}

ISR(USART_RX_vect)
{

}