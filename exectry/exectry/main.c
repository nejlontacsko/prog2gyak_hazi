#include "LCD_T-Bird3.h"
#include "leds.h"

#include <stdio.h>
#include <avr/interrupt.h>

//Glyphs
char
    ethConn[8] = { 0x00, 0xe0, 0x1b, 0x11, 0x11, 0x1f, 0x00, 0x00 },
      enter[8] = { 0x01, 0x01, 0x05, 0x0D, 0x1F, 0x0C, 0x04, 0x00 },
	  check[8] = { 0x00, 0x01, 0x03, 0x16, 0x1c, 0x08, 0x00, 0x00 },
	  cross[8] = { 0x00, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x00, 0x00 },
	upArrow[8] = { 0x00, 0x04, 0x04, 0x0E, 0x0E, 0x1F, 0x00, 0x00 },
  downArrow[8] = { 0x00, 0x00, 0x1F, 0x0E, 0x0E, 0x04, 0x04, 0x00 };

enum States
{
	Init,
	ScreenIpMode,
	ScreenSetIp,
	ScreenStats,
	ScreenDirect
};


int main(void)
{
	DDRA = 0xff;
	PORTA = 0;
	
	lcdInit();
	ledInit(0x3e);
	
	Static_Limit(5);
	Static_CustomChar(7, enter);
	Static_Clear();	
	//Static_Print("Készenlét.", LCD_EMPTY_LINE, LCD_EMPTY_LINE, LCD_EMPTY_LINE);
	
	DDRG &= ~0x1f;
	
	char buf[17];
	memset(buf, 0, 17);
	
	char pingPrev = 0, pingCurr;
	
	while (1) 
    {
		pingCurr = PING;
		if (pingCurr != pingPrev)
		{
			sprintf(buf, "PING: 0x%02x", PING);
			lcdPrint(buf, 0);
			sprintf(buf, "PORTleds: 0x%02x", (PIND & 0xf0) | (PINB >> 4));
			lcdPrint(buf, 1);
			
			switch(pingCurr)
			{
				case 0x10:
					ledSet(LED_SCR_MODE, ON);
					
					break;
				case 0x08:
					ledSet(LED_SCR_SET,  ON);
					break;
				case 0x04:
					ledSet(LED_SCR_STAT, ON);
					break;
				case 0x02:
					ledSet(LED_SCR_DRCT, ON);
					break;
				case 0x01:
					ledTrigger(LED_BLACKOUT);
					break;
				default:
					break;
			}
		}
		
		pingPrev = pingCurr;
		
		/*lcdSendCmd(LCD_CMD_RETHOME);
		lcdSendCmd(LCD_CMD_DDRAMAD + 0x5f);
		lcdSendByte(7);
		_delay_ms(500);
		
		lcdSendCmd(LCD_CMD_RETHOME);
		lcdSendCmd(LCD_CMD_DDRAMAD + 0x5f);
		lcdSendByte(' ');
		_delay_ms(500);*/
		//Static_Clear();
    }
}