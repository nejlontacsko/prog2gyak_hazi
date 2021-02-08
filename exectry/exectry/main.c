#include "LCD_T-Bird3.h"
#include "leds.h"
#include "buttonHandler.h"
#include "matrix.h"

#include <stdio.h>
#include <avr/interrupt.h>

//Glyphs..
char
    ethConn[8] = { 0x00, 0xe0, 0x1b, 0x11, 0x11, 0x1f, 0x00, 0x00 },
      enter[8] = { 0x01, 0x01, 0x05, 0x0D, 0x1F, 0x0C, 0x04, 0x00 },
	  check[8] = { 0x00, 0x01, 0x03, 0x16, 0x1c, 0x08, 0x00, 0x00 },
	  cross[8] = { 0x00, 0x11, 0x0a, 0x04, 0x0a, 0x11, 0x00, 0x00 },
	upArrow[8] = { 0x00, 0x04, 0x04, 0x0E, 0x0E, 0x1F, 0x00, 0x00 },
  downArrow[8] = { 0x00, 0x00, 0x1F, 0x0E, 0x0E, 0x04, 0x04, 0x00 };


int main(void)
{
	//important contribution
	DDRA = 0xff;
	PORTA = 0;
	
	lcdInit();
	ledInit();
	matrixInit();
	Static_Limit(5);
	Static_CustomChar(7, enter);
	Static_Clear();	
	//Static_Print("Készenlét.", LCD_EMPTY_LINE, LCD_EMPTY_LINE, LCD_EMPTY_LINE);
	
	DDRG &= ~0x1f;
	
	char buf[17];
	memset(buf, 0, 17);
	
	char ping_prev = 0, ping_curr;
	
	while (1) 
    {
		ping_curr = PING;
		if (ping_curr != ping_prev)
		{
			sprintf(buf, "PORTG: 0x%02x", PING);
			driver(PING);
			lcdPrint(buf, 0);
		}
		
		ping_prev = ping_curr;
		
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