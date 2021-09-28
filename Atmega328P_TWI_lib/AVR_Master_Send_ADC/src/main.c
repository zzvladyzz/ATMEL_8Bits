/*
 * AVR_Master_Send_ADC.c
 *
 * Created: 4/4/2021 8:59:19 PM
 * Author : vladi
 */ 
#define  F_CPU	8000000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "header/ADC_328P.h"
#include "header/TWI_328P.h"
#define		W		0
#define		R		1
#define		Ack		1
#define		Nack	0
#define		Address 0x00

int main(void)
{
	cli();
	DDRB=0x00;		//	Input
	DDRD=0x00;		//	Input
	PORTB=0xff;		//	Pull-up
	PORTD=0x37;		//	Pull-up	

	EICRA|=(1<<ISC01);
	EIMSK|=(1<<INT0);
	ADC_Init();
	TWI_Config(Address,1,0,0);
	sei();
    while (1) 
    {		
		asm("nop");
    }
	return 0;
}

ISR(INT0_vect)
{
	cli();
	uint8_t Direction=PIND&0x03;
	//uint8_t DataPort=PINB&0xff;
	TWI_Start();
	TWI_Adrress(Direction,W);
	TWI_TransmitData(ADC_ReadValue());
	TWI_Stop();	
	sei();
}






