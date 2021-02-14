#ifndef DMX_OUT_H
#define DMX_OUT_H

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

void dmxInit();

void dmxStartTransmit();

void dmxSendByte(uint8_t);

void dmxEndTransmit();

void dmxTest();

#endif