#include "leds.h"

void led(int id, int val)
{
	if (val)
		if (id & 0xf0)
			PORTD |= id;
		else
			PORTB |= id << 4;
	else
		if (id & 0xf0)
			PORTD &= ~id;
		else
			PORTB &= ~(id << 4);
}

void ledTest()
{
	for (int i = 1; i < 256; i <<= 1)
	{
		led(i, ON);
		_delay_ms(200);
		led(i, OFF);
		_delay_ms(50);
	}
	PORTD |= 0xf0;
	PORTB |= 0xf0;
	_delay_ms(200);
	PORTD &= ~0xf0;
	PORTB &= ~0xf0;
}

void ledInit()
{
	DDRD |= 0xf0;
	DDRB |= 0xf0;
	ledTest();
}