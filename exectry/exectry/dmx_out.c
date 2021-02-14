#include "dmx_out.h"

void dmxInit()
{
	//RX_EN pin: OUTPUT, HIGH (disabled)
	DDRC |= _BV(PC7);
	PORTC |= _BV(PC7);
	
	//TX_EN pin: OUTPUT, HIGH (enabled)
	DDRE |= _BV(PE2);
	PORTE |= _BV(PE2);
	
	//TX_DATA pin: OUTPUT, HIGH (Mark Before Break)
	DDRD |= _BV(PD3);
	PORTD |= _BV(PD3);
}

void dmxStartTransmit()
{
	PORTD &= ~_BV(PD3);
	_delay_us(174); //Break time: 176 - 2 = 174
	PORTD |= _BV(PD3);
	_delay_us(9); //Mark After Break time: 12 - 3 = 9
	
	UCSR1A |= _BV(U2X);
	UCSR1B = 0x08; // 0x48; with TXCIE
	
	//data (8N2, 250000baud)
	UCSR1C = 0x0e;
	UBRR1H = 0;
	UBRR1L = 0x03;
	
	//ZERO Start Code
	dmxSendByte(0x00);
}

void dmxSendByte(uint8_t data)
{
	UDR1 = data;
	while (!(UCSR1A & _BV(UDRE)));
}

void dmxEndTransmit()
{
	UCSR1B &= ~_BV(TXEN);
	PORTD |= _BV(PD3);
}

void dmxTest()
{
	dmxStartTransmit();
	
	
	dmxSendByte(0xff);
	dmxSendByte(0x00);
	dmxSendByte(0xa0);
	dmxSendByte(0x00);
	dmxSendByte(0xff);
	dmxSendByte(0x00);
	
	for (int i = 7; i <= 512; i++)
		dmxSendByte(i);
	
	dmxEndTransmit();
}