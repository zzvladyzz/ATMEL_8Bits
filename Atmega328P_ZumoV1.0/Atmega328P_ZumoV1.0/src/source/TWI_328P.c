/*
 * TWI_328P.c
 *
 * Created: 4/4/2021 10:53:52 PM
 *  Author: vladi
 */ 
#include "../header/TWI_328P.h"

#define PrescalerValue		1			/*		Prescaler entre 1/4/16/64			*/
#define SCL_CLK				100000ul	/*		Velocidad de trabajo del TWI		*/ 
#define BITRATE				(((F_CPU/SCL_CLK)-16)/(2*PrescalerValue))
uint8_t TWI_Master_OR_Slave=0;
uint8_t TWI_Status_Master=0;			/*		Estato del Maestro			*/
uint8_t TWI_Status_Slave=0;				/*		Estado del Esclavo			*/
uint8_t TWI_Control_Slave=0;			/*		Control del estado de transmision del ST		*/

/*			Configuracion del ATmega328P		*/
/*			SlaveAddress=0	Direccion del esclavo				*/
/*			S_M=0	Esclavo				*/
/*			S_M=1	Maestro				*/
/*			GCall=1	Habilita la llamada general pero se coloca casi siempre en 0					*/
/*			IntSlave=1	Habilita la interrupcion cuando es configurado en modo esclavo				*/
void	TWI_Config(uint8_t SlaveAddress,uint8_t S_M,uint8_t GCall,uint8_t IntSlave)
{
	TWI_Status_Master=0;
	TWI_Status_Slave=0;
	TWI_Control_Slave=0;
	if (S_M==0)
	{
		TWI_Master_OR_Slave=0;
		if (IntSlave==0)							/*		Esclavo sin habilitacion de la interrupcion		*/
		{
			TWAR=(SlaveAddress<<1) | GCall;
			TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		}
		else if (IntSlave==1)						/*		Esclavo conn habilitacion de la interrupcion		*/
		{
			TWAR=(SlaveAddress<<1) | GCall;
			TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWIE)|(1<<TWINT);			/*		Envio de ACK si se registra una comunicacion		*/
		}
	}
	else if(S_M==1)									/*		Maestro sin habilitacion de la interrupcion		*/
	{												/*		La interrupcion no se habilito para este caso debido a que las aplicaciones no lo requeriran		*/
		TWI_Master_OR_Slave=1;
		TWBR=BITRATE;
		switch(PrescalerValue)
		{
			case 1:
					TWSR|=0x00;
				break;
			case 4:
					TWSR|=0x01;
				break;
			case 16:
					TWSR|=0x02;
				break;
			case 64:
					TWSR|=0x03;
				break;
		}}
}
	

/*		Inicia el START		*/
void	TWI_Start()
{
	uint8_t Condition;
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR&(1<<TWINT)));
	Condition=TWSR&0xF8;
	if(Condition==0x08)
	{
		TWI_Status_Master	=	8;				/*  8 si el START fue exitoso	*/
		return ;
	}
	TWI_Status_Master	=	0;					/*  0 si fallo el START		*/
}
/*		Repetir el START		*/
void	TWI_RepeatStart()
{
	if (TWI_Status_Master== 18 || TWI_Status_Master== 20 || TWI_Status_Master== 28 || TWI_Status_Master== 30 || TWI_Status_Master== 48 || TWI_Status_Master== 58)
	{
		uint8_t Condition;
		TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		while(!(TWCR&(1<<TWINT)));
		Condition=TWSR&0xF8;
		if(Condition==0x10)
		{
			TWI_Status_Master	=	10;				//  1 si el REPEATSTART fue exitoso
			return ;
		}
		TWI_Status_Master	=	0;					//  0 si el REPEATSTART fallo
	}
}
/*		Detener la comunicacion		*/
void	TWI_Stop()
{
	if (TWI_Status_Master== 20 || TWI_Status_Master== 28 || TWI_Status_Master== 30 || TWI_Status_Master== 48 || TWI_Status_Master== 58)
	{
		TWI_Status_Master=0;
		TWCR=(1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
		while(TWCR&(1<<TWSTO));
	}
}
/*		Comunicarse con un esclavo		*/
void	TWI_Adrress(uint8_t Data_Adrress, uint8_t W_R)
{
	if (TWI_Status_Master	==	8 || TWI_Status_Master== 10)
	{
		uint8_t Status;
		TWDR=(Data_Adrress<<1)|(W_R);
		TWCR=(1<<TWINT)|(1<<TWEN);
		while(!(TWCR&(1<<TWINT)));
		Status=TWSR&0xF8;
		if (W_R==0)
		{
			switch (Status)
			{
				case 0x18:				// Send SLA+W and Receiver ACK		w=0
						TWI_Status_Master = 18;	// 18 to indicate ACK received
						return ;
					break;
				case 0x20:				// Send SLA+W and Receiver NACK		w=0
						TWI_Status_Master = 20;	// 20 to indicate NACK received
						return ;
					break;
				case 0x38:
						TWI_Status_Master = 0;	// 0 to indicate W=Arbitration lost in SLA+W or data bytes
						return ;
					break;
				default :
						TWI_Status_Master = 0;	// 0 to indicate SLA+W/R failed
						return ;
					break;
			}	
		} 
		else if(W_R==1)
		{
			switch (Status)
			{
				case 0x40:				// Send SLA+R and Receiver ACK		r=1
						TWI_Status_Master = 40;	// Return 40 to indicate ACK received
						return ;
					break;
				case 0x48:				// Send SLA+R and Receiver NACK		r=1
						TWI_Status_Master = 48;	// Return 48 to indicate NACK received
						return ;
					break;
				case 0x38:
						TWI_Status_Master = 0;	// Return 0 to indicate R=Arbitration lost in SLA+R or NOT ACK bit
						return ;
					break;
				default :
						TWI_Status_Master = 0;	// Return 0 to indicate SLA+W/R failed
						return ;
					break;
			}	
		}
	}
}
/*		Enviar datos siendo MT o ST		*/
void	TWI_TransmitData(uint8_t Data)		
{
	if (TWI_Master_OR_Slave==1)
	{
		if (TWI_Status_Master == 18 || TWI_Status_Master == 28)
		{
			uint8_t Status;
			TWDR=Data;
			TWCR=(1<<TWINT)|(1<<TWEN);
			while(!(TWCR&(1<<TWINT)));
			Status=TWSR&0xF8;
			if (Status==0x28)			// Send Data and Receiver ACK and can i keep sending data
			{
				TWI_Status_Master	=	28;		// 2 to indicate ACK received
				return ;
			}
			else if (Status==0x30)			// Send Data and Receiver NACK 
			{
				TWI_Status_Master	=	30;		// 30 to indicate NACK received
				return ;
			}
			else if (Status==0x38)			// Failure
			{
				TWI_Status_Master	=	0;		// 0 to indicate why other master continue
				return ;
			}
			else
			{
				TWI_Status_Master	=	0;
				return;	
			}
		}
	}
	else if (TWI_Master_OR_Slave==0)
	{
		TWI_Listen();
		if (TWI_Control_Slave==2)
		{
			TWDR=Data;
			TWCR=(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
			while(!(TWCR&(1<<TWINT)));
			TWI_Status_Slave=TWSR&0xF8;
			if (TWI_Status_Slave==0xB8 )
			{
				TWI_Control_Slave=2;
				return ;
			}
			else if (TWI_Status_Slave==0xC0)
			{
				TWI_Control_Slave=0;
				return ;
			}
			return ;
		}
		return ;
	}
}
/*		Recibir datos siendo MR o Sr y enviando ACK para continuar la recepcion o NACK para detener el envio de datos		*/
uint8_t TWI_ReceiveData(uint8_t ACK_NACK)	/// Master Receive MR
{
		/*		Recibir Datos desde el Esclavo		*/
	if (TWI_Master_OR_Slave==1)
	{
		if (TWI_Status_Master== 40 || TWI_Status_Master== 50)
		{
			uint8_t Status;
			if (ACK_NACK==1)
			{
				TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
			}
			else if (ACK_NACK==0)
			{
				TWCR=(1<<TWINT)|(1<<TWEN);
			}
			while(!(TWCR&(1<<TWINT)));
			Status=TWSR&0xF8;
			switch (Status)
			{
				case 0x50:			// Data byte has been received; ACK has been returned
						TWI_Status_Master = 50;
						return TWDR;	// Return 0 for indicate continue Receive Data from Slave
					break;
				case 0x58:			// Data byte has been received; NACK has been returned
						TWI_Status_Master = 58;
						return TWDR;	// Return 1 for indicate Stop or RepeatStop
					break;
				case 0x38:
						TWI_Status_Master = 0;
						return 0;	// Return 3 to indicate arbitration lost in SLA+R or data bytes
					break;
				default :
						TWI_Status_Master = 0;
						return 0;	// Return 6 for indicate data failure
					break;
			}
		}
		if (TWI_Status_Slave==0xFF)
		{
			TWCR=(ACK_NACK<<TWEA)|(1<<TWEN)|(1<<TWINT);
			while(!(TWCR&(1<<TWINT)));
			TWI_Status_Slave=TWSR&0xF8;
			if (TWI_Status_Slave==0x80 || TWI_Status_Slave==0x90 )
			{
				TWI_Status_Slave=0xFF;
				return TWDR;
			}
			else if (TWI_Status_Slave==0x88 || TWI_Status_Slave==0x98 )
			{
				TWI_Status_Slave=0x00;
				return TWDR;
			}
			else if (TWI_Status_Slave==0xA0 )
			{
				TWI_Status_Slave=0x00;
				TWCR|=(1<<TWINT);
			}
			return 0;
		}
	}
	/*		Recepcion de datos desde el Maestro		*/
	else if (TWI_Master_OR_Slave==0)
	{
		TWI_Listen();
		if (TWI_Control_Slave==1)
		{
			TWCR=(ACK_NACK<<TWEA)|(1<<TWEN)|(1<<TWINT);
			while(!(TWCR&(1<<TWINT)));
			TWI_Status_Slave=TWSR&0xF8;
			if (TWI_Status_Slave==0x80 || TWI_Status_Slave==0x90 )
			{
				TWI_Control_Slave=1;
				return TWDR;
			}
			else if (TWI_Status_Slave==0x88 || TWI_Status_Slave==0x98 )
			{
				TWI_Control_Slave=0;
				return TWDR;
			}
			else if (TWI_Status_Slave==0xA0 )
			{
				TWI_Control_Slave=0;
				TWCR|=(1<<TWINT);
			}
			return 0;
		}
		
		return 0;
	}
return 0;
}
/*		Verificar si existe una llamada desde un maestro		*/
void	TWI_Listen()
{
	while(TWI_Control_Slave==0)
	{
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		while(!(TWCR&(1<<TWINT)));
		TWI_Status_Slave=TWSR&0xF8;
		if (TWI_Status_Slave==0x60 || TWI_Status_Slave==0x68 ||TWI_Status_Slave==0x70 ||TWI_Status_Slave==0x78)
		{
			TWI_Control_Slave=1;		/*		Se recibio SLA+W y se devolvio ACK		*/
			return ;
		}
		if (TWI_Status_Slave==0xA8 || TWI_Status_Slave==0xB0 )
		{
			TWI_Control_Slave=2;		/*		Se recibio SLA+R y se devolvio NACK		*/
			return ;
		}
		return ;
	}
}

/*		Habilitado la interrupcion esto se debe copiar en main.c mas las variables para el uso de la inteerrupcion		*/
/*		ISR_Variables		*/
/*
#define TWI_Size	3		//	Bytes a recibir
uint8_t TWI_Ref=0;			
uint8_t *TWI_Out=&TWI_Ref;	//	Salida se encuentra en *TWI_Out
uint8_t TWI_Loop=0;
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////		ISR			////////////////////////////////////////////////////
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
*/////////////////////////////////////////////////////////////////////////////////
























































