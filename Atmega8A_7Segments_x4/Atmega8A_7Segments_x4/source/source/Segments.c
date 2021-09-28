/*
 * _7_Segments.c
 *
 * Created: 3/3/2021 11:50:14 AM
 *  Author: vladi
 */ 
#include <avr/io.h>
#include "../headers/Segments.h"

#define		LedA (1<<PORTB0)
#define		LedB (1<<PORTB1)
#define		LedC (1<<PORTB2)
#define		LedD (1<<PORTB3)
#define		LedE (1<<PORTB4)
#define		LedF (1<<PORTB5)
#define		LedG (1<<PORTD7)
#define		LedD1 (1<<PORTD6)
#define		LedD2 (1<<PORTD5)
#define		LedD3 (1<<PORTD4)
#define		LedD4 (1<<PORTD3)
#define		Point (1<<PORTD2)

void Init_Segment()
{
	PORTD&=0x00;
	PORTB&=0x00;
	DDRD|=(LedG|LedD1|LedD2|LedD3|LedD4|Point);
	DDRB|=(LedA|LedB|LedC|LedD|LedE|LedF);
	PORTD=0x00;
	PORTB=0x00;
	
}
void Segments(uint8_t Seg)
{
	switch (Seg)
	{
	case 0 :
		PORTB|=LedA|LedB|LedC|LedD|LedE|LedF;
		PORTD&=~LedG;
		break;
	case 1 :
		PORTB|=LedB|LedC;
		PORTB&=~(LedA|LedD|LedE|LedF);
		PORTD&=~LedG;
	break;
	case 2 :
		PORTB|=LedA|LedB|LedD|LedE;
		PORTB&=~(LedC|LedF);
		PORTD|=LedG;
	break;
	case 3 :
		PORTB|=LedA|LedB|LedC|LedD;
		PORTB&=~(LedE|LedF);
		PORTD|=LedG;
	break;
	case 4 :
		PORTB|=LedB|LedC|LedF;
		PORTB&=~(LedA|LedE|LedD);
		PORTD|=LedG;
	break;
	case 5 :
		PORTB|=LedA|LedC|LedD|LedF;
		PORTB&=~(LedB|LedE);
		PORTD|=LedG;
	break;
	case 6 :
		PORTB|=LedA|LedC|LedD|LedE|LedF;
		PORTB&=~(LedB);
		PORTD|=LedG;
	break;
	case 7 :
		PORTB|=LedA|LedB|LedC;
		PORTB&=~(LedD|LedE|LedF);
		PORTD&=~LedG;
	break;
	case 8 :
		PORTB|=LedA|LedB|LedC|LedD|LedE|LedF;
		PORTD|=LedG;
	break;
	case 9 :
		PORTB|=LedA|LedB|LedC|LedD|LedF;
		PORTB&=~LedE;
		PORTD|=LedG;
	break;
		
	}
}
void ControlLed(uint8_t Control)
{
	switch (Control)
	{
	case 0 :
		PORTD|=LedD1;
		PORTD|=LedD2;
		PORTD|=LedD3;
		PORTD|=LedD4;
		break;
	case 1 :
		PORTD&=~LedD1;
		PORTD|=LedD2;
		PORTD|=LedD3;
		PORTD|=LedD4;
		break;
	case 2 :
		PORTD|=LedD1;
		PORTD&=~LedD2;
		PORTD|=LedD3;
		PORTD|=LedD4;
		break;
	case 3 :
		PORTD|=LedD1;
		PORTD|=LedD2;
		PORTD&=~LedD3;
		PORTD|=LedD4;
		break;
	case 4 :
		PORTD|=LedD1;
		PORTD|=LedD2;
		PORTD|=LedD3;
		PORTD&=~LedD4;
		break;
	}
}
void Display_7_Seg(int16_t Valor,uint8_t LedControl)
{
	
	uint16_t U,D,C,UM;
	
	U=Valor%1000;
	UM=Valor/1000;
	C=U/100;
	U=U%100;
	D=U/10;
	U=U%10;
	switch (LedControl)
	{
	case 0:
		UnidadMil(UM);
		break;
	case 1:
		Centena(C);
		break;
	case 2:
		Decena(D);
		break;
	case 3:
		Unidad(U);
		break;
	}
}
void Unidad(uint8_t U)
{
	ControlLed(0);
	Segments(U);
	ControlLed(4);
	//_delay_ms(5);
}
void Decena(uint8_t D)
{
	ControlLed(0);
	Segments(D);
	ControlLed(3);
	//_delay_ms(5);
}
void Centena(uint8_t C)
{
	ControlLed(0);
	Segments(C);
	ControlLed(2);
	//_delay_ms(5);
}
void UnidadMil(uint8_t UM)
{
	ControlLed(0);
	Segments(UM);
	ControlLed(1);
	//_delay_ms(5);
}

