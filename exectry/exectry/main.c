#include "LCD_T-Bird3.h"
#include "leds.h"

#include <stdio.h>
#include <avr/interrupt.h>

char //Glyphs
    ethConn[8] = { 0x00, 0xe0, 0x1b, 0x11, 0x11, 0x1f, 0x00, 0x00 },
      enter[8] = { 0x01, 0x01, 0x05, 0x0D, 0x1F, 0x0C, 0x04, 0x00 },
	  check[8] = { 0x00, 0x01, 0x03, 0x16, 0x1c, 0x08, 0x00, 0x00 },
	  cross[8] = { 0x00, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x00, 0x00 },
	upArrow[8] = { 0x00, 0x04, 0x04, 0x0E, 0x0E, 0x1F, 0x00, 0x00 },
  downArrow[8] = { 0x00, 0x00, 0x1F, 0x0E, 0x0E, 0x04, 0x04, 0x00 };

typedef enum state
{
	Init,
	ScreenIpMode,
	ScreenSetIp,
	ScreenStats,
	ScreenDirect
} State;


int main(void)
{
	State currentState = Init, nextState = ScreenStats;
	
	DDRA = 0xff;
	PORTA = 0;
	
	lcdInit();
	ledInit(0x3e);
	
	Static_Limit(8);
	Static_Clear();	
	
	DDRG &= ~0x1f;
	
	char pingPrev = 0, pingCurr;

	while (1) 
    {
		pingCurr = PING;
		if ((pingCurr != pingPrev && pingCurr > 0) || currentState == Init)
		{
			switch(pingCurr)
			{
				case 0x10:
					nextState = (currentState != ScreenIpMode) ? ScreenIpMode : ScreenStats;
					break;
				case 0x08:
					nextState = (currentState != ScreenSetIp) ? ScreenSetIp : ScreenStats;
					break;
				case 0x04:
					//TODO:	nyelvváltás EEPROM-ból
					break;
				case 0x02:
					nextState = (currentState != ScreenDirect) ? ScreenDirect : ScreenStats;
					break;
				case 0x01:
					ledTrigger(LED_BLACKOUT);
					break;
				default:
					break;
			}
			
			currentState = nextState;
			
			switch (currentState)
			{
				case ScreenStats:
					ledSet(LED_SCR_STAT,  ON);
					lcdPrintHun(LCD_EMPTY_LINE, "  Statisztikák  ", "    képernyõ", LCD_EMPTY_LINE);
					break;
				case ScreenIpMode:
					ledSet(LED_SCR_MODE, ON);
					lcdPrintHun(LCD_EMPTY_LINE, "   IP üzemmód   ", "    képernyõ", LCD_EMPTY_LINE);
					break;
				case ScreenSetIp:
					ledSet(LED_SCR_SET,  ON);
					lcdPrintHun(LCD_EMPTY_LINE, "  Cím beállító  ", "    képernyõ", LCD_EMPTY_LINE);
					break;
				case ScreenDirect:
					ledSet(LED_SCR_DRCT,  ON);
					lcdPrintHun(LCD_EMPTY_LINE, "   DMX direkt   ", "    képernyõ", LCD_EMPTY_LINE);
					break;
				default:
					break;
			}
			
		}
		
		pingPrev = pingCurr;
    }
}