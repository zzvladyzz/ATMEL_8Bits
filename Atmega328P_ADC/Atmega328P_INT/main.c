
#define F_CPU 8000000ul
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int main()
{
	cli();
	DDRB=0xFF;
	
	ADMUX|= 0b01100001;
	ADCSRA|=0b00000110;
	sei();
	
	while(1)
	{
		//TODO ESTO ES PARA AHORRAR ENERGIA
			ADCSRA|=(1<<ADEN);
			_delay_us(10);
			ADCSRA|=(1<<ADSC);
			while(!(ADCSRA&(1<<ADIF)));// siempre que se lee una bandera de interrupcion
			// esta debemos apagarla automaticamete debido a que esta se apaga cuando entra al ISR
			// pero al no tener esta funcion se queda asi
			ADCSRA&=~(1<<ADIF);// reiniciamos la bandera
			ADCSRA&=~(1<<ADEN);
			PORTB=ADCH;
	}
}


