/*
 * Atmega16A_I2C_UART.cpp
 *
 * Created: 12/31/2020 11:55:34 PM
 * Author : vladi
 */ 
#define F_CPU		8000000UL
#define Baud_Rate	9600
#define Baud_Prescale	((F_CPU/(16UL*Baud_Rate))-1) 
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

unsigned char UART_Rx()
{	
		while ((UCSRA&(1<<RXC))==0);
		return UDR;
}
void UART_Tx(char caracter)
{
	while (!(UCSRA&(1<<UDRE)));  // Mientras no se vacie el Reg UDR no sale
	UDR=caracter;
}
void UART_Tx_String(char *str)
{
	unsigned char j=0;
	while (str[j]!=0)
	{
		UART_Tx(str[j]);
		j++;
	}
}

int main(void)
{
	char c;
    /* Inicializamos USART */
	UCSRB|=(1<<RXEN)|(1<<TXEN);   //Habilitamos RX y TX
	UCSRC|=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); //Tamaño de bits 
	UBRRL|=Baud_Prescale;			  //Velocidad de baudios	
	UBRRH|=(Baud_Prescale>>8);
	
	DDRB=DDRB|(1<<PB2);
	UART_Tx_String("\n\t Hola");
    while (1) 
    {
		c=UART_Rx();
		if(c=='e')
		{
			PORTB|=(1<<PB2);
			UART_Tx_String("\n\r Led encendido");
		}
		else if (c=='a')
		{
			PORTB&=~(1<<PB2);
			UART_Tx_String("\n\r Led apagado");
		}
		else {UART_Tx_String("\n\r Tecla erronea");}
		
		
    }
}

