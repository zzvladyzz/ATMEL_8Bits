/*
 * _7_Segments.h
 *
 * Created: 3/3/2021 11:50:00 AM
 *  Author: vladi
 */ 




#ifndef SEGMENTS_H_
#define SEGMENTS_H_
#include <avr/io.h>

void Init_Segment();
void ControlLed(uint8_t Control);
void Display_7_Seg(int16_t Valor,uint8_t LedControl);
void Unidad(uint8_t U);
void Decena(uint8_t D);
void Centena(uint8_t C);
void UnidadMil(uint8_t UM);
void Segments(uint8_t Seg);

#endif /* 7_SEGMENTS_H_ */


