/*
 * USART_328P.h
 *
 * Created: 4/11/2021 10:28:15 PM
 *  Author: vladi
 */ 


#ifndef USART_328P_H_
#define USART_328P_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU				8000000ul

void	USART_init(uint16_t BAUDRATE,uint8_t EN_RX,uint8_t EN_TX,uint8_t EN_INT,char PARITY[10], uint8_t STOP_BITS,uint8_t MODE_USART,uint8_t BITS);
void	USART_TransmitData(uint8_t Data);
void	USART_TransmitString(char *Data,uint8_t USART_Lenght);
uint8_t USART_ReceiveData();



#endif /* USART_328P_H_ */