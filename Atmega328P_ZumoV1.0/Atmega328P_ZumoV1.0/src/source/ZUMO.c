/*
 * L3GD20H.c
 *
 * Created: 4/14/2021 2:21:43 AM
 *  Author: vladi
 */   

#include "../header/ZUMO.h"


//////////////////////////////////////////////////////////////////////////
//		P5	  |		P4		|	P3	  |		P2	  |		P1	 |	P0		//			
//			  |				|		  |	 		  |			 |			//
//||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||//	
//				VISTA	INFERIROR	ZUMO REFLECTANCE SENSOR				//	
//////////////////////////////////////////////////////////////////////////
/*	Arduino				Atmega328P					ZumoPin				*/
/*		D0		-->		PD0(RX)					-->	RX					*/
/*		D1		-->		PD1(TX)					-->	TX					*/
/*		D2		-->		PD2						-->	ON QTR				*/
/*		D3		-->		PD3(PWM-->OC2B)			-->	BUZZER				*/
/*		D4		-->		PD4						-->	ZUMO_REFLECTANCE_P0	*/
/*		D5		-->		PD5(PWM-->OC0B)			-->	ZUMO_REFLECTANCE_P5	*/
/*		D6		-->		PD6(PWM-->OC0A)			-->						*/
/*		D7		-->		PD7						-->	RIGHT_MOTOR			*/
/*		D8		-->		PB0						-->	LEFT__MOTOR			*/
/*		D9		-->		PB1(PWM-->OC1A)			-->	RIGHT_MOTOR_PWM		*/
/*		D10		-->		PB2(PWM-->OC1B)(SS)		-->	LEFT__MOTOR_PWM		*/
/*		D11		-->		PB3(PWM-->OC2A)(MOSI)	-->	ZUMO_REFLECTANCE_P2	*/
/*		D12		-->		PB4(MISO)				-->	PUSHBUTTON			*/
/*		D13		-->		PB5(SCK)				-->	YELLOW_USER_LED		*/
/*		A0		-->		PC0						-->	ZUMO_REFLECTANCE_P3	*/
/*		A1		-->		PC1						-->	BATTERY_LEVEL		*/
/*		A2		-->		PC2						-->	ZUMO_REFLECTANCE_P4	*/
/*		A3		-->		PC3						-->	ZUMO_REFLECTANCE_P1	*/
/*		A4		-->		PC4(SDA)				-->	SDA					*/
/*		A5		-->		PC5(SCL)				-->	SCL					*/
//////////////////////////////////////////////////////////////////////////
////		0=input
////		1=output
void ZUMO_Init()
{
	/*		Configurar pin ZUMO		*/
	DDRD=0x00;
	DDRB=0x00;
	DDRC=0x00;
	
	DDRD=(1<<DDD1)|(1<<DDD2)|(1<<DDD3)|(1<<DDD6)|(1<<DDD7);
	DDRB=(1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB5);
	DDRC&=~((1<<DDC0)|(1<<DDC2)|(1<<DDC3));
	
	
	PORTB|=(1<<PORTB4);
	PORTC&=~((1<<PORTC0)|(1<<PORTC2)|(1<<PORTC3));
	PORTD&=~((1<<PORTD4)|(1<<PORTD5));
	
	
	/*		Inicializamos Variables del proyecto		*/
	for (int a=0;a<6;a++)
	{
		ZUMO.QTR.Sensors[a]		=	0;
		ZUMO.QTR.Sensors_Min[a]	=	0;
		ZUMO.QTR.Sensors_Max[a]	=	0;
		ZUMO.QTR.Dump[a]		=	0;
	}
	ZUMO.QTR.Counter=0;
	
	ZUMO.IMU.Alpha	=    0.00F;
	ZUMO.IMU.Beta	=    0.00F;
	ZUMO.IMU.Min_Mx	=    0.00F;
	ZUMO.IMU.Max_Mx	=    0.00F;
	ZUMO.IMU.Min_My	=    0.00F;
	ZUMO.IMU.Max_My	=    0.00F;
	
	ZUMO.QTR.Position=0;
	
	/*		Inicializamos perifericos		*/
	cli();
	USART_Init(9600,1,1,0,"none",1,0,8);
	TWI_Config(0x00,1,0,0);
	L3GD20H_Init();
	LSM303D_Init();
	TIMER_Init();
	
	/*		Inicializamos Variables				*/
	OFF_Led;
	OFF_QTR;
	OFF_Motors;
	OFF_Timer0;
	OFF_Timer2;
	ForWard_L;
	ForWard_R;
	ZUMO_Motor(0,0);
	
	pressButton();
	ZUMO_QTR_Calibrate(50);
	pressButton();
	ZUMO_Magnetometer_Calibrate();
	pressButton();
	ZUMO_PID_Init(25.0000,5.2500,0.0900,0.01536);
	
	sei();
}
void TIMER_Init()
{
	/*		Timer0	-->	Sensores de linea												*/
	/*																		*/
	/*				F_CLK							F_CLK					*/
	/*	F_timer=---------------		--> OCR0A=--------------------	- 1		*/
	/*			(OCR0A+1)*Prescaler				F_timer*Prescaler			*/
	/*				16MHz													*/
	/*	OCR0A=--------------------	- 1	 -->	OCR0A=155					*/
	/*				100Hz x 1024												*/
	
	TCCR0A	&=~	(1<<COM0A1);
	TCCR0A	&=~	(1<<COM0A0);
	TCCR0A	&=~	(1<<COM0B1);
	TCCR0A	&=~	(1<<COM0B0);
	
	/*		Modo comparacion		*/
	TCCR0A	&=~	(1<<WGM00);
	TCCR0A	|=	(1<<WGM01);
	TCCR0B	&=~	(1<<WGM02);
	
	/*		Genera 100 Hz para la lectura de QTR	*/
	OCR0A=155;				
	OCR0B=0;				
	
	/*			Deshabilitamos el Timer0		*/
	TCCR0B	&=~	(1<<CS00);
	TCCR0B	&=~	(1<<CS01);
	TCCR0B	&=~	(1<<CS02);
	
	/*			Habilitamos la interrupcion por comparacion	*/	
	TIMSK0|=(1<<OCIE0A);
		
	/*	Timer1	-->	PWM			*/
	/*				F_CLK							F_CLK					*/
	/*	F_pwm	=---------------		--> ICR1=--------------------  - 1	*/
	/*			 (ICR1+1)*Prescaler					F_pwm*Prescaler			*/
	/*				16MHz													*/
	/*	ICR1	=--------------------	- 1	 -->	ICR1=1065				*/
	/*				15KHz x 1												*/
	ICR1=1065;
	OCR1A=0;								/*	Right	*/
	OCR1B=0;								/*	Left	*/
	
	TCCR1A=(1<<COM1A1)|(1<<COM1B1);			/*		Salida Low al comparar	*/	
	/*		Modo 14		*/
	TCCR1A&=~(1<<WGM10);
	TCCR1A|=(1<<WGM11);
	TCCR1B|=(1<<WGM12);
	TCCR1B|=(1<<WGM13);
	/*	Timer 1 deshabilitado	*/
	TCCR1B&=~(1<<CS10);						
	TCCR1B&=~(1<<CS11);
	TCCR1B&=~(1<<CS12);
	
	/*				Timer2	-->		Muestreo para PID						*/
	/*																		*/
	/*				F_CLK							F_CLK					*/
	/*	F_timer=---------------		--> OCR2A=--------------------	- 1		*/
	/*			(OCR2A+1)*Prescaler				F_timer*Prescaler			*/
	/*				16MHz													*/
	/*	OCR2A=--------------------	- 1	 -->	OCR2A=239					*/
	/*				65Hz x 1024												*/
	TCCR2A	&=~	(1<<WGM20);
	TCCR2A	|=	(1<<WGM21);
	TCCR2B	&=~	(1<<WGM22);
	
	OCR2A=239;
	OCR2B=0;
	TIMSK2|=(1<<OCIE2A);
	
	/*		Deshabilitamos timer 2		*/
	TCCR2B&=~(1<<CS22);
	TCCR2B&=~(1<<CS21);
	TCCR2B&=~(1<<CS20);
	
}
void ZUMO_Motor(uint16_t L_PWM,uint16_t R_PWM)
{
	if (L_PWM<=1000 && R_PWM<=1000 && L_PWM>=0 && R_PWM>=0)
	{
		/*		Conversion de Magnitud	con un maximo de 1000	*/	
		OCR1B=(uint16_t)((ICR1/1000)*(L_PWM));
		OCR1A=(uint16_t)((ICR1/1000)*(R_PWM));
	}
	else
	{
		OCR1B=0;
		OCR1A=0;		
	}
}	
void L3GD20H_Init()
{
	TWI_Start();
	TWI_Adrress(L3GD20H_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(L3GD20H_LOW_ODR);
	TWI_TransmitData(0x00);
	
	TWI_RepeatStart();
	TWI_Adrress(L3GD20H_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(L3GD20H_CTRL1);
	TWI_TransmitData(0x6F);
	
	TWI_RepeatStart();
	TWI_Adrress(L3GD20H_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(L3GD20H_CTRL4);
	TWI_TransmitData(0x00);
	TWI_Stop();
	
}
void LSM303D_Init()
{
	TWI_Start();
	TWI_Adrress(LSM303D_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(LSM303D_CTRL1);
	TWI_TransmitData(0x57);
	
	TWI_RepeatStart();
	TWI_Adrress(LSM303D_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(LSM303D_CTRL2);
	TWI_TransmitData(0x00);						/*		Probar 0x02		*/
	
	TWI_RepeatStart();
	TWI_Adrress(LSM303D_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(LSM303D_CTRL5);
	TWI_TransmitData(0x64);
	
	TWI_RepeatStart();
	TWI_Adrress(LSM303D_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(LSM303D_CTRL6);
	TWI_TransmitData(0x00);
	
	TWI_RepeatStart();
	TWI_Adrress(LSM303D_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(LSM303D_CTRL7);
	TWI_TransmitData(0x00);
	TWI_Stop();
	
}
void ZUMO_Magnetometer()
{
		TWI_Start();
		TWI_Adrress(LSM303D_SA0_HIGH_ADDRESS,0);
		TWI_TransmitData(LSM303D_OUT_X_L_M|(1<<7));
		TWI_RepeatStart();
		TWI_Adrress(LSM303D_SA0_HIGH_ADDRESS,1);
		uint8_t M_x_l=TWI_ReceiveData(1);
		uint8_t M_x_h=TWI_ReceiveData(1);
		uint8_t M_y_l=TWI_ReceiveData(1);
		uint8_t M_y_h=TWI_ReceiveData(1);
		uint8_t M_z_l=TWI_ReceiveData(1);
		uint8_t M_z_h=TWI_ReceiveData(0);
		TWI_Stop();
		
		ZUMO.IMU.M_x = (int16_t)(M_x_h<<8 | M_x_l);
		ZUMO.IMU.M_y = (int16_t)(M_y_h<<8 | M_y_l);
		ZUMO.IMU.M_z = (int16_t)(M_z_h<<8 | M_z_l);
		ZUMO.IMU.M_x =	ZUMO.IMU.M_x-ZUMO.IMU.Alpha;
		ZUMO.IMU.M_y =	ZUMO.IMU.M_y-ZUMO.IMU.Beta;

		ZUMO.IMU.C_Mx=(float)(ZUMO.IMU.M_x*Factor_Conversion_LSM303D_Mag);		/*	SENSORS_GAUSS_TO_MICROTESLA	*/
		ZUMO.IMU.C_My=(float)(ZUMO.IMU.M_y*Factor_Conversion_LSM303D_Mag);		/*	SENSORS_GAUSS_TO_MICROTESLA	*/
		ZUMO.IMU.C_Mz=(float)(ZUMO.IMU.M_z*Factor_Conversion_LSM303D_Mag);			/*	SENSORS_GAUSS_TO_MICROTESLA	*/
		
		ZUMO.IMU.yaw=(float)atan2(ZUMO.IMU.C_My,ZUMO.IMU.C_Mx)*(180.0/PI);
		
		if(ZUMO.IMU.yaw <0 )
		{
			ZUMO.IMU.yaw+=360;
		}
		
		if(ZUMO.PID.EN==1)
		{
			ZUMO.IMU.yaw=ZUMO.IMU.yaw+ZUMO.PID.Offset;
			if (ZUMO.PID.Offset>=0)
			{
				if (ZUMO.IMU.yaw>360)
				{
					ZUMO.IMU.yaw=ZUMO.IMU.yaw-360;
				}
			}
			else
			{
				if (ZUMO.IMU.yaw<0)
				{
					ZUMO.IMU.yaw=360+ZUMO.IMU.yaw;
				}
			}
		}
}
void ZUMO_Accelerometer()
{
	TWI_Start();
	TWI_Adrress(LSM303D_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(LSM303D_OUT_X_L_A|(1<<7));
	TWI_RepeatStart();
	TWI_Adrress(LSM303D_SA0_HIGH_ADDRESS,1);
	uint8_t A_x_l=TWI_ReceiveData(1);
	uint8_t A_x_h=TWI_ReceiveData(1);
	uint8_t A_y_l=TWI_ReceiveData(1);
	uint8_t A_y_h=TWI_ReceiveData(1);
	uint8_t A_z_l=TWI_ReceiveData(1);
	uint8_t A_z_h=TWI_ReceiveData(0);
	TWI_Stop();
	
	ZUMO.IMU.A_x = (int16_t)((A_x_h<<8)|A_x_l);
	ZUMO.IMU.A_y = (int16_t)((A_y_h<<8)|A_y_l);
	ZUMO.IMU.A_z = (int16_t)((A_z_h<<8)|A_z_l);
	ZUMO.IMU.A_x = ZUMO.IMU.A_x*0.94639-232.8;
	ZUMO.IMU.A_y = ZUMO.IMU.A_y*0.98219-231.7;
	ZUMO.IMU.A_z = ZUMO.IMU.A_z*0.95762-1023.7;
	
	ZUMO.IMU.C_Ax = (float)(ZUMO.IMU.A_x*Factor_Conversion_LSM303D_Acc*SENSORS_GRAVITY_EARTH);
	ZUMO.IMU.C_Ay = (float)(ZUMO.IMU.A_y*Factor_Conversion_LSM303D_Acc*SENSORS_GRAVITY_EARTH);
	ZUMO.IMU.C_Az = (float)(ZUMO.IMU.A_z*Factor_Conversion_LSM303D_Acc*SENSORS_GRAVITY_EARTH);
}
void ZUMO_Gyroscope()
{
	TWI_Start();
	TWI_Adrress(L3GD20H_SA0_HIGH_ADDRESS,0);
	TWI_TransmitData(L3GD20H_OUT_X_L|(1<<7));		/*	Segun el datasheet se debe mandar 0b1 +	Direccion registro para que el IMU envie los datos que se necesiten		*/
	TWI_RepeatStart();								/*	Al mandar 0b0 + Direccion del registro solo se recibira un dato del IMU											*/
	TWI_Adrress(L3GD20H_SA0_HIGH_ADDRESS,1);
	uint8_t G_x_l = TWI_ReceiveData(1);
	uint8_t G_x_h = TWI_ReceiveData(1);
	uint8_t G_y_l = TWI_ReceiveData(1);
	uint8_t G_y_h = TWI_ReceiveData(1);
	uint8_t G_z_l = TWI_ReceiveData(1);
	uint8_t G_z_h = TWI_ReceiveData(0);	
	TWI_Stop();
	
	
	ZUMO.IMU.G_x = (int16_t)(G_x_h << 8 | G_x_l);	/*		Combinamos los MSB=OUT_X_H con LSB=OUT_X_L		*/
	ZUMO.IMU.G_y = (int16_t)(G_y_h << 8 | G_y_l);	/*		Se desplaza 8 posiciones a la derecha el MSB y se hace un or con LSB para no alterar los bits que ya estan leidas	*/
	ZUMO.IMU.G_z = (int16_t)(G_z_h << 8 | G_z_l);
	
	ZUMO.IMU.C_Gx=(float)(ZUMO.IMU.G_x*Factor_Conversion_L3GD20H_Gyr);
	ZUMO.IMU.C_Gy=(float)(ZUMO.IMU.G_y*Factor_Conversion_L3GD20H_Gyr);
	ZUMO.IMU.C_Gz=(float)(ZUMO.IMU.G_z*Factor_Conversion_L3GD20H_Gyr);
}
void ZUMO_PID_Init(float KP,float KI,float KD,float T)
{
	ZUMO.PID.Kp=KP;
	ZUMO.PID.Ki=KI;
	ZUMO.PID.Kd=KD;
	ZUMO.PID.K1=KP+KI*T+(KD/T);
	ZUMO.PID.K2=-KP-2*(KD/T);
	ZUMO.PID.K3=KD/T;
	ZUMO.PID.e_PID[0]=0;
	ZUMO.PID.e_PID[1]=0;
	ZUMO.PID.e_PID[2]=0;
	ZUMO.PID.C_PID[0]=0;
	ZUMO.PID.C_PID[1]=0;
	ZUMO.PID.Speed=300;
	ZUMO.PID.Offset=0;
	ZUMO.PID.SetPoint=180;
	ZUMO.PID.EN=0;
	ZUMO.PID.Ts=T;
	ZUMO_Magnetometer();
	ZUMO.PID.Offset=180-ZUMO.IMU.yaw;
	ZUMO.PID.EN=1;
}
void ZUMO_PID()
{
	ZUMO.PID.e_PID[0]=ZUMO.PID.SetPoint-ZUMO.IMU.yaw;
	ZUMO.PID.C_PID[0]=ZUMO.PID.C_PID[1]+ZUMO.PID.K1*ZUMO.PID.e_PID[0]+ZUMO.PID.K2*ZUMO.PID.e_PID[1]+ZUMO.PID.K3*ZUMO.PID.e_PID[2];
	int16_t speed=0;
	if (ZUMO.PID.C_PID[0]>ZUMO.PID.Speed)
	{
		ZUMO.PID.C_PID[0]=ZUMO.PID.Speed;	
	}
	else if (ZUMO.PID.C_PID[0]<(-1*ZUMO.PID.Speed))
	{
		ZUMO.PID.C_PID[0]=-1*ZUMO.PID.Speed;
	}
	if (ZUMO.PID.e_PID[0]>=0)
	{
		BackWard_R;
		ForWard_L;
		speed=(int16_t)ZUMO.PID.C_PID[0];
	}
	else if (ZUMO.PID.e_PID[0]<0)
	{
		BackWard_L;
		ForWard_R;
		speed=(int16_t)(-1*ZUMO.PID.C_PID[0]);	
	}
	ZUMO_Motor(80+speed,80+speed);
	//ZUMO_Motor(ZUMO.PID.Speed+ZUMO.PID.C_PID[0],ZUMO.PID.Speed-ZUMO.PID.C_PID[0]);
	ZUMO.PID.C_PID[1]=ZUMO.PID.C_PID[0];
	ZUMO.PID.e_PID[2]=ZUMO.PID.e_PID[1];
	ZUMO.PID.e_PID[1]=ZUMO.PID.e_PID[0];
	
}
void ZUMO_Gyroscope_Accelerometer()
{
	
	  /*		Calcular los ángulos con Acelerometro y Giroscopio		*/
	  ZUMO.IMU.Angulo_Aceleracion_X=(float)atan(ZUMO.IMU.C_Ay/sqrt(pow(ZUMO.IMU.C_Ax,2)+pow(ZUMO.IMU.C_Az,2)));
	  ZUMO.IMU.Angulo_Aceleracion_X*=(180.0/PI);
	  ZUMO.IMU.Angulo_Aceleracion_Y=(float)atan(-1*(ZUMO.IMU.C_Ax/sqrt(pow(ZUMO.IMU.C_Ay,2)+pow(ZUMO.IMU.C_Az,2))));
	  ZUMO.IMU.Angulo_Aceleracion_Y*=(180.0/PI);
	  ZUMO.IMU.roll=0.98*(ZUMO.IMU.roll+ZUMO.IMU.C_Gx*ZUMO.PID.Ts)+0.02*ZUMO.IMU.Angulo_Aceleracion_X;
	  ZUMO.IMU.pitch=0.98*(ZUMO.IMU.pitch+ZUMO.IMU.C_Gy*ZUMO.PID.Ts)+0.02*ZUMO.IMU.Angulo_Aceleracion_Y;
	  
}
void ZUMO_Magnetometer_Calibrate()
{
	OFF_Motors;
	ForWard_L;
	BackWard_R;
	ZUMO_Motor(200,200);
	ZUMO_Magnetometer();
	ZUMO.IMU.Min_Mx=ZUMO.IMU.M_x;
	ZUMO.IMU.Max_Mx=ZUMO.IMU.M_x;
	ZUMO.IMU.Min_My=ZUMO.IMU.M_y;
	ZUMO.IMU.Max_My=ZUMO.IMU.M_y;
	ON_Motors;
	for(uint8_t i=0;i<100;i++)
	{
		ZUMO_Magnetometer();
		if(ZUMO.IMU.M_x>=ZUMO.IMU.Max_Mx)
		{
			ZUMO.IMU.Max_Mx=ZUMO.IMU.M_x;
		}
		else if (ZUMO.IMU.M_x<ZUMO.IMU.Min_Mx)
		{
			ZUMO.IMU.Min_Mx=ZUMO.IMU.M_x;
		}
			
		if(ZUMO.IMU.M_y>=ZUMO.IMU.Max_My)
		{
			ZUMO.IMU.Max_My=ZUMO.IMU.M_y;
		}
		else if (ZUMO.IMU.M_y<ZUMO.IMU.Min_My)
		{
			ZUMO.IMU.Min_My=ZUMO.IMU.M_y;
		}
		_delay_ms(100);
	}
	ZUMO.IMU.Alpha	=(ZUMO.IMU.Max_Mx+ZUMO.IMU.Min_Mx)/2;
	ZUMO.IMU.Beta	=(ZUMO.IMU.Max_My+ZUMO.IMU.Min_My)/2;
	OFF_Motors;	
	ZUMO_Motor(0,0);
	ForWard_L;
	ForWard_R;
}
void ZUMO_QTR_Calibrate(uint8_t Sample)
{
	uint8_t True=0;
	uint8_t Pa,Pb,Pc,Pd,Pe,Pf;
	for (uint8_t s=0;s<Sample;s++)
	{
	//////////////////////////
	ON_QTR;
	ZUMO.QTR.Counter=0;
	Pa=0;
	Pb=0;
	Pc=0;
	Pd=0;
	Pe=0;
	Pf=0;

	DDRD	|=	(1<<DDD4)|(1<<DDD5);
	DDRC	|=	(1<<DDC3)|(1<<DDC0)|(1<<DDC2);
	DDRB	|=	(1<<DDB3);
	
	PORTD	|=	(1<<PORTD4)|(1<<PORTD5);
	PORTC	|=	(1<<PORTC3)|(1<<PORTC0)|(1<<PORTC2);
	PORTB	|=	(1<<PORTB3);
	
	_delay_us(10);
	
	DDRD	&=~	((1<<DDD4)|(1<<DDD5));
	DDRC	&=~	((1<<DDC3)|(1<<DDC0)|(1<<DDC2));
	DDRB	&=~	(1<<DDB3);
	
	PORTD	&=~	((1<<PORTD4)|(1<<PORTD5));
	PORTC	&=~	((1<<PORTC3)|(1<<PORTC0)|(1<<PORTC2));
	PORTB	&=~	(1<<PORTB3);
	
	while (Pa==0 || Pb==0 || Pc==0 || Pd==0 || Pe==0 || Pf==0 )
	{
		ZUMO.QTR.Counter++;
		
		if ((!(PIND&(1<<PIND4)))&&(Pa==0))
		{
			ZUMO.QTR.Dump[0]=ZUMO.QTR.Counter;
			Pa=1;
		}
		if ((!(PINC&(1<<PINC3)))&&(Pb==0))
		{
			ZUMO.QTR.Dump[1]=ZUMO.QTR.Counter;
			Pb=1;
		}
		if ((!(PINB&(1<<PINB3)))&&(Pc==0))
		{
			ZUMO.QTR.Dump[2]=ZUMO.QTR.Counter;
			Pc=1;
		}
		if ((!(PINC&(1<<PINC0)))&&(Pd==0))
		{
			ZUMO.QTR.Dump[3]=ZUMO.QTR.Counter;
			Pd=1;
		}
		if ((!(PINC&(1<<PINC2)))&&(Pe==0))
		{
			ZUMO.QTR.Dump[4]=ZUMO.QTR.Counter;
			Pe=1;
		}
		if ((!(PIND&(1<<PIND5)))&&(Pf==0))
		{
			ZUMO.QTR.Dump[5]=ZUMO.QTR.Counter;
			Pf=1;
		}
	}
	
	OFF_QTR;
	//////////////////////////
	if (True==1)
		{
			for (uint8_t j=0;j<6;j++)
			{
				if (ZUMO.QTR.Dump[j] < ZUMO.QTR.Sensors_Min[j])
					{
						ZUMO.QTR.Sensors_Min[j]=ZUMO.QTR.Dump[j];
					}
				else if (ZUMO.QTR.Dump[j] >= ZUMO.QTR.Sensors_Max[j])
					{
						ZUMO.QTR.Sensors_Max[j]=ZUMO.QTR.Dump[j];
					}
			}
		}
	else
		{
			True=1;
			for (uint8_t j=0;j<6;j++)
			{
				ZUMO.QTR.Sensors_Min[j]=ZUMO.QTR.Dump[j];
				ZUMO.QTR.Sensors_Max[j]=ZUMO.QTR.Dump[j];
			}
		}
		_delay_ms(200);
	}
	OFF_QTR;
	
}
void ZUMO_QTR_Sensors()
{
	/*    Codigo Mejorado	*/	
	ON_QTR;
	ZUMO.QTR.Counter=0;
	uint8_t Pos0=0;
	uint8_t Pos1=0;
	uint8_t Pos2=0;
	uint8_t Pos3=0;
	uint8_t Pos4=0;
	uint8_t Pos5=0;

	DDRD	|=	(1<<DDD4)|(1<<DDD5);
	DDRC	|=	(1<<DDC3)|(1<<DDC0)|(1<<DDC2);
	DDRB	|=	(1<<DDB3);
	
	PORTD	|=	(1<<PORTD4)|(1<<PORTD5);
	PORTC	|=	(1<<PORTC3)|(1<<PORTC0)|(1<<PORTC2);
	PORTB	|=	(1<<PORTB3);
	
	_delay_us(10);
	
	DDRD	&=~	((1<<DDD4)|(1<<DDD5));
	DDRC	&=~	((1<<DDC3)|(1<<DDC0)|(1<<DDC2));
	DDRB	&=~	(1<<DDB3);
	
	PORTD	&=~	((1<<PORTD4)|(1<<PORTD5));
	PORTC	&=~	((1<<PORTC3)|(1<<PORTC0)|(1<<PORTC2));
	PORTB	&=~	(1<<PORTB3);
	
	while (Pos0==0 || Pos1==0 || Pos2==0 || Pos3==0 || Pos4==0 || Pos5==0 )
	{
		ZUMO.QTR.Counter++;
		
		if ((!(PIND&(1<<PIND4)))&&(Pos0==0))
		{
			ZUMO.QTR.Sensors[0]=ZUMO.QTR.Counter;
			Pos0=1;
		}
		if ((!(PINC&(1<<PINC3)))&&(Pos1==0))
		{
			ZUMO.QTR.Sensors[1]=ZUMO.QTR.Counter;
			Pos1=1;
		}
		if ((!(PINB&(1<<PINB3)))&&(Pos2==0))
		{
			ZUMO.QTR.Sensors[2]=ZUMO.QTR.Counter;
			Pos2=1;
		}
		if ((!(PINC&(1<<PINC0)))&&(Pos3==0))
		{
			ZUMO.QTR.Sensors[3]=ZUMO.QTR.Counter;
			Pos3=1;
		}
		if ((!(PINC&(1<<PINC2)))&&(Pos4==0))
		{
			ZUMO.QTR.Sensors[4]=ZUMO.QTR.Counter;
			Pos4=1;
		}
		if ((!(PIND&(1<<PIND5)))&&(Pos5==0))
		{
			ZUMO.QTR.Sensors[5]=ZUMO.QTR.Counter;
			Pos5=1;
		}
	}
	OFF_QTR;
	//////////////////////////
    
	for (uint8_t i=0;i<6;i++)
	{
		if (ZUMO.QTR.Sensors[i]<ZUMO.QTR.Sensors_Min[i])
		{
			ZUMO.QTR.Sensors[i]=ZUMO.QTR.Sensors_Min[i]+1;
		}
		if (ZUMO.QTR.Sensors[i]>ZUMO.QTR.Sensors_Max[i])
		{
			ZUMO.QTR.Sensors[i]=ZUMO.QTR.Sensors_Max[i]-1;
		}
	}
	
	uint32_t Pos=0;
	ZUMO.QTR.S_C=0;
	for (uint8_t val=0;val<6;val++)
	{
		ZUMO.QTR.S_A=(float)(ZUMO.QTR.Sensors_Max[val]-ZUMO.QTR.Sensors_Min[val]);
		ZUMO.QTR.S_A=1000/ZUMO.QTR.S_A;
		
		ZUMO.QTR.S_B=(float)(ZUMO.QTR.Sensors[val]-ZUMO.QTR.Sensors_Min[val]);
		ZUMO.QTR.Sensors[val]=(int)(ZUMO.QTR.S_A*ZUMO.QTR.S_B);
		Pos=Pos+val*ZUMO.QTR.Sensors[val];
		ZUMO.QTR.S_C=ZUMO.QTR.S_C+ZUMO.QTR.Sensors[val];
	}
	Pos=(1000*Pos)/(ZUMO.QTR.S_C);
	ZUMO.QTR.Position=(int16_t)(Pos);
}
void pressButton()
{
	ON_Led;
	//while(Button)
	while(USART_ReceiveData()!='s')
	{
		OFF_Led;
		_delay_ms(100);
		ON_Led;
		_delay_ms(10);
	}
	OFF_Led;
}
