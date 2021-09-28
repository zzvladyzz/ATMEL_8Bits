/*
 * TWI_328P.h
 *
 * Created: 4/4/2021 10:53:36 PM
 *  Author: vladi
 */ 


#ifndef TWI_328P_H_
#define TWI_328P_H_

#include <avr/io.h>
#include <avr/interrupt.h>

 
void	TWI_Config(uint8_t SlaveAddress,uint8_t S_M,uint8_t GCall,uint8_t IntSlave);
void	TWI_Start();
void	TWI_RepeatStart();
void	TWI_Stop();
void	TWI_Adrress(uint8_t Data_Adrress, uint8_t W_R);
void	TWI_TransmitData(uint8_t Data);
uint8_t TWI_ReceiveData(uint8_t ACK_NACK);
void	TWI_Listen();
ISR(TWI_vect);
#endif /* TWI_328P_H_ */