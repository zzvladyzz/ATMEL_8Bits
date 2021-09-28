/*
 * INIT_MEMS.h
 *
 * Created: 4/15/2021 12:56:39 AM
 *  Author: vladi
 */ 


#ifndef INIT_MEMS_H_
#define INIT_MEMS_H_

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "USART_328P.h"
#include "L3GD20H.h"
#include "LSM303D.h"
#include "TWI_328P.h"


/*		Constantes		*/
#define SENSORS_GAUSS_TO_MICROTESLA		100				/*	 Gauss to micro-Tesla multiplier	*/
#define SENSORS_GRAVITY_EARTH			9.80665F		/*	 Earth's gravity in m/s^2			*/
#define SENSORS_DPS_TO_RADS				0.017453293F	/*	 Degrees/s to rad/s multiplier		*/                            \
#define SENSORS_RADS_TO_DPS				57.29577793F	/*	 Rad/s to degrees/s  multiplier		*/
#define PI								3.14158F
/*		Valores de compensacion del magnetometro		*/

/*		PinOut Zumo 		*/

#define ON_QTR			PORTD	|=	(1<<PORTD2)
#define OFF_QTR			PORTD	&=~	(1<<PORTD2)

#define ON_Buzzer		PORTD	|=	(1<<PORTD3)
#define OFF_Buzzer		PORTD	&=~	(1<<PORTD3)

#define ON_Led			PORTB	|=	(1<<PORTB5)
#define OFF_Led			PORTB	&=~	(1<<PORTB5)

#define BackWard_L		PORTB	|=	(1<<PORTB0)
#define ForWard_L		PORTB	&=~	(1<<PORTB0)

#define BackWard_R		PORTD	|=	(1<<PORTD7)
#define ForWard_R		PORTD	&=~	(1<<PORTD7)

#define Button			PINB&(1<<PINB4)

#define ON_Motors		TCCR1B	|=	(1<<CS10)
#define OFF_Motors		TCCR1B	&=~	(1<<CS10)

#define ON_Timer2       TCCR2B	|=	((1<<CS22)|(1<<CS21)|(1<<CS20))
#define OFF_Timer2		TCCR2B	&=~	((1<<CS22)|(1<<CS21)|(1<<CS20))

#define ON_Timer0		TCCR0B	|=	((1<<CS02)|(1<<CS00))
#define OFF_Timer0		TCCR0B	&=~	((1<<CS02)|(1<<CS00))



typedef struct PID
{
	float	Ts	;		/*	Periodo de muestreo		*/
	float	Kp	;
	float	Ki	;	
	float	Kd	;
	float	K1	;
	float	K2	;
	float	K3	;
	float	Speed	;
	float	Offset	;	  
	float	C_PID[2];
	float	e_PID[3];
	float	SetPoint;
	float	borrar;
	
	uint8_t EN;

}PID;
typedef struct IMU
{
	float	yaw			;	
	float	pitch		;
	float	roll		;
	float	Angulo_Aceleracion_X		;
	float	Angulo_Aceleracion_Y		;
	int16_t	A_x			;
	int16_t	A_y			;		
	int16_t	A_z			;
	int16_t	G_x			;
	int16_t	G_y			;
	int16_t	G_z			;
	int16_t	M_x			;
	int16_t	M_y			;
	int16_t	M_z			;
	float	C_Ax		;
	float	C_Ay		;
	float	C_Az		;
	float	C_Mx		;
	float	C_My		;
	float	C_Mz		;
	float	C_Gx		;
	float	C_Gy		;
	float	C_Gz		;
	float	Alpha		;	
	float	Beta		;
	float   Min_Mx		;
	float   Max_Mx		;
	float   Min_My		;
	float   Max_My		;
	
}IMU;

typedef struct QTR_Array
{

	uint16_t Sensors[6];
	uint16_t Sensors_Max[6];
	uint16_t Sensors_Min[6];
	uint16_t Dump[6];
	uint16_t Position;
	uint16_t Counter;
	float	 S_A;
	float	 S_B;
	uint32_t S_C;	 
	
}QTR_Array;


struct ZUMO_INIT
{
	PID PID;
	IMU	IMU;
	QTR_Array QTR;
}ZUMO;


void	L3GD20H_Init();
void	LSM303D_Init();
void	ZUMO_Init();
void	ZUMO_Magnetometer();
void	ZUMO_Accelerometer();
void	ZUMO_Gyroscope();
void	TIMER_Init();
void	ZUMO_Motor(uint16_t L_PWM,uint16_t R_PWM);
void	ZUMO_Gyroscope_Accelerometer();
void	ZUMO_Magnetometer_Calibrate();
void	ZUMO_PID();
void	ZUMO_PID_Init(float KP,float KI,float KD,float T);
void	ZUMO_QTR_Calibrate(uint8_t Sample);
void	ZUMO_QTR_Sensors();
void	pressButton();
void	Float_to_Str(char *buffer, float f);
void	Serial_Print_Sensor();
void	Serial_Print_SensorMax();
void	Serial_Print_SensorMin();
void	Serial_Print_IMU();
void	Serial_Print_Calibrated();
void	Serial_Print_DAQ();

#endif /* INIT_MEMS_H_ */