/*
 * USART_328P.c
 *
 * Created: 4/11/2021 10:27:40 PM
 *  Author: vladi
 */ 
#include "../header/USART_328P.h"


void	USART_Init(uint16_t BAUDRATE,uint8_t EN_RX,uint8_t EN_TX,uint8_t EN_INT,char PARITY[10], uint8_t STOP_BITS,uint8_t MODE_USART,uint8_t BITS)
{

	UBRR0L=(uint8_t) ((F_CPU/BAUDRATE/16)-1);
	UBRR0H=(uint8_t) (((F_CPU/BAUDRATE/16)-1)>>8);
	UCSR0B|=(EN_RX<<RXEN0);
	UCSR0B|=(EN_TX<<TXEN0);
	UCSR0B|=(EN_INT<<RXCIE0);
	if (STOP_BITS==1)
	{
		UCSR0C&=~(1<<USBS0);								/*		Stop Bits 1		*/
	} 
	else if (STOP_BITS==2)
	{
		UCSR0C|=(1<<USBS0);									/*		Stop Bits 2		*/
	}
	switch (MODE_USART)
	{
		case 0 :					/*		Mode Asynchronous		*/
			UCSR0C&=~(1<<UMSEL01);
			UCSR0C&=~(1<<UMSEL00);
			break;
		case 1 :					/*		Mode Synchronous		*/
			UCSR0C&=~(1<<UMSEL01);
			UCSR0C|=(1<<UMSEL00);
			break;
		default :
			UCSR0C&=~(1<<UMSEL01);
			UCSR0C&=~(1<<UMSEL00);
			break;
	}
	char None[10]="none";
	char Par[10]="par";
	char Impar[10]="impar";
	if (PARITY==None)
	{			/*		Parity Disable		*/
				UCSR0C&=~(1<<UPM01);
				UCSR0C&=~(1<<UPM00);
	}	
	else if (PARITY==Par)
	{			/*		Parity	Par		*/
				UCSR0C|=(1<<UPM01);
				UCSR0C&=~(1<<UPM00);
	}
	else if (PARITY==Impar)
	{			/*		Parity Impar		*/
				UCSR0C|=(1<<UPM01);
				UCSR0C|=(1<<UPM00);
	}
	else
	{
				UCSR0C&=~(1<<UPM01);
				UCSR0C&=~(1<<UPM00);
	}
	switch (BITS)
	{
		case 5 :					/*		5 Bits		*/
				UCSR0B&=~(1<<UCSZ02);
				UCSR0C&=~(1<<UCSZ01);
				UCSR0C&=~(1<<UCSZ00);
			break;
		case 6 :					/*		6 Bits		*/
				UCSR0B&=~(1<<UCSZ02);
				UCSR0C&=~(1<<UCSZ01);
				UCSR0C|=(1<<UCSZ00);
			break;
		case 7 :					/*		7 Bits		*/
				UCSR0B&=~(1<<UCSZ02);
				UCSR0C|=(1<<UCSZ01);
				UCSR0C&=~(1<<UCSZ00);
			break;
		case 8 :					/*		8 Bits		*/
				UCSR0B&=~(1<<UCSZ02);
				UCSR0C|=(1<<UCSZ01);
				UCSR0C|=(1<<UCSZ00);
			break;
		default :
				UCSR0B&=~(1<<UCSZ02);
				UCSR0C|=(1<<UCSZ01);
				UCSR0C|=(1<<UCSZ00);
			break;
	}
}

void	USART_TransmitData(unsigned char Data)
{
	while(!(UCSR0A&(1<<UDRE0)));		/*	UDRE0=1 Entonces el buffer esta vacio y al tener ! entonces se vuelve False y sale del While */
	UDR0=Data;							/*	Cargamos los datos en UDR0		*/
}
void	USART_TransmitString(char *Data,uint8_t USART_Lenght)
{
		for (uint8_t USART_i=0;USART_i<USART_Lenght;USART_i++)
		{
		USART_TransmitData(Data[USART_i]);
		}
}
uint8_t USART_ReceiveData()
{
	while(!(UCSR0A&(1<<RXC0)));
	return UDR0;
}

/*
ISR(USART_RX_vect)
{
	return UDR0;
}
*/