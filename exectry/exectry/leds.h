#ifndef LEDS_H_
#define LEDS_H_

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

#define LED_COM_RX   _BV(7)
#define LED_COM_TX   _BV(6)
#define LED_NET_POLL _BV(5)
#define LED_SCR_MODE _BV(4)
#define LED_SCR_SET  _BV(3)
#define LED_SCR_STAT _BV(2)
#define LED_SCR_DRCT _BV(1)
#define LED_BLACKOUT _BV(0)

#define ON 1
#define OFF 0

void ledSet(int id, int val);

void ledTrigger(int id);

void ledTest();

void ledInit(char);

void ledClearUniqueArea();

//extern char ledUniqueMask;

#endif