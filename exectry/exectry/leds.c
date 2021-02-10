#include "leds.h"

char ledUniqueMask = 0;

void ledSet(int id, int val)
{
	if (id > 1 && id < _BV(6))
<<<<<<< HEAD
	ledClearUniqueArea();
=======
		ledClearUniqueArea();
>>>>>>> 82d01f5947e4b5df1b89b5d8faccdb78b623c15b
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

void ledTrigger(int id)
{
	char pv = (PIND & 0xf0) | (PINB >> 4);
	
	ledSet(id, !(pv & id));
}

void ledTrigger(int id)
{
	char pv = (PIND & 0xf0) | (PINB >> 4);
	
	ledSet(id, !(pv & id));
}

void ledTest()
{
	for (int i = 1; i < 256; i <<= 1)
	{
		ledSet(i, ON);
		_delay_ms(200);
		ledSet(i, OFF);
		_delay_ms(50);
	}
	PORTD |= 0xf0;
	PORTB |= 0xf0;
	_delay_ms(200);
	PORTD &= ~0xf0;
	PORTB &= ~0xf0;
}

void ledInit(char uniqueMask)
{
	DDRD |= 0xf0;
	DDRB |= 0xf0;
	ledTest();
	ledUniqueMask = uniqueMask;
}

void ledClearUniqueArea()
{
	PORTD &= ~(ledUniqueMask & 0xf0);
	PORTB &= ~((ledUniqueMask & 0x0f) << 4);
}