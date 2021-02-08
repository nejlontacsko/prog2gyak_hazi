/*
 * matrix.h
 *
 * Created: 2021.02.08. 22:34:19
 *  Author: Soma
 */ 


#ifndef MATRIX_H_
#define MATRIX_H_
#include <avr/io.h>
void matrix();
void matrixInit();
void segEnable(int state);
void segDigit(uint8_t digit, uint8_t val);
void write7seg(char number);

#endif /* MATRIX_H_ */