/*
 * R2R.h
 *
 * Created: 3/1/2021 5:34:14 AM
 *  Author: vladi
 */ 


#ifndef R2R_H_
#define R2R_H_

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void _Init_T();
void R2R(uint8_t signal,uint8_t select);

#endif /* R2R_H_ */