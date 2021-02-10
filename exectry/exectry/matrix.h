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
void ipInputMode(int state);
void matrixProcess(int op);
void write7seg(char number);
void printIps();

#endif /* MATRIX_H_ */