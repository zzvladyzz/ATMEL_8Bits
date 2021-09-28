/*
 * AVR_Slave_Receive_ADC.c
 *
 * Created: 4/6/2021 9:32:18 PM
 * Author : vladi
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "header/TWI_328P.h"


/*		ISR		*/
#define TWI_Size	1
uint8_t TWI_Ref=0;
uint8_t *TWI_Out=&TWI_Ref;
uint8_t TWI_Loop=0;
//////////////////
ISR(TWI_vect);


ISR(INT0_vect);

int main(void)
{
	
	cli();
	DDRD=0x00;		//	Input
	DDRB=0xff;		//	Output
	PORTD=0xff;		//	Pull-up
	EICRA|=(1<<ISC01);
	EIMSK|=(1<<INT0);
	TWI_Config(PIND&0x03,0,0,1);
	sei();
	while (1)
	{
		PORTB=*TWI_Out;
	}
	return 0;
}
ISR(INT0_vect)
{
	TWAR=((PIND & 0x03)<<1) | 0;
}


ISR(TWI_vect)
{
	uint8_t TWI_Status_INT;
	uint8_t TWI_Data_Receive=TWI_Size;
	TWI_Status_INT=TWSR&0xF8;
	if (TWI_Status_INT==0x60 || TWI_Status_INT==0x68 ||TWI_Status_INT==0x70 ||TWI_Status_INT==0x78)
	{
		TWCR=(1<<TWEA)|(1<<TWIE)|(1<<TWINT)|(1<<TWEN);
		TWI_Loop=0;
	}
	else if (TWI_Status_INT==0x80 || TWI_Status_INT==0x90)
	{
		
		if ((TWI_Data_Receive-TWI_Loop)==0)
		{
			TWI_Loop=0;
			TWCR=(0<<TWEA)|(1<<TWIE)|(1<<TWINT)|(1<<TWEN);
		}
		else if ((TWI_Data_Receive-TWI_Loop)>0)
		{
			TWI_Loop++;
			TWCR=(1<<TWEA)|(1<<TWIE)|(1<<TWINT)|(1<<TWEN);
		}
		TWI_Ref=TWDR;

	}
	else if (TWI_Status_INT==0x88 || TWI_Status_INT==0x98)
	{
		TWCR=(1<<TWEA)|(1<<TWIE)|(1<<TWINT)|(1<<TWEN);
		TWI_Ref=TWDR;

	}
	else if (TWI_Status_INT==0xA0)
	{
		TWCR|=(1<<TWINT);
	}
}

