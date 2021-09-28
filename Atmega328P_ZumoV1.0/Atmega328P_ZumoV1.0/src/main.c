/*
 * Atmega328P_ZumoV1.0.c
 *
 * Created: 4/14/2021 2:17:16 AM
 * Author : vladi
 */ 

#include "header/ZUMO.h"

uint8_t t=0;



int main(void)
{
	ZUMO_Init();
	pressButton();
	OFF_Timer0;
	ON_Timer2;
	OFF_Motors;
	sei();
	while (1) 
    {		
			_delay_ms(500);
			cli();
			Serial_Print_IMU();
			sei();
    }
	
}
/*		Sensado a 100 hz 	*/
ISR(TIMER0_COMPA_vect)
{
	
}

/*		PID	 a 	65 Hz					*/
ISR(TIMER2_COMPA_vect)
{
	cli();
	ZUMO_QTR_Sensors();
	ZUMO_Magnetometer();
	ZUMO_PID();
	sei();
}

