#include "LCD_T-Bird3.h"

static int limit = 8;

//Hungarian letters
static char
	upperAA[8] = { 0x04, 0x04, 0x00, 0x0e, 0x11, 0x1f, 0x11, 0x11 }, //Á
	lowerAA[8] = { 0x04, 0x04, 0x00, 0x0e, 0x01, 0x0f, 0x11, 0x0f }, //á
	upperEE[8] = { 0x04, 0x04, 0x00, 0x1f, 0x10, 0x1e, 0x10, 0x1f }, //É
	lowerEE[8] = { 0x04, 0x04, 0x00, 0x0e, 0x11, 0x1f, 0x10, 0x0e }, //é
	upperII[8] = { 0x04, 0x04, 0x00, 0x0e, 0x04, 0x04, 0x04, 0x0e }, //Í
	lowerII[8] = { 0x04, 0x04, 0x00, 0x04, 0x0c, 0x04, 0x04, 0x0e }, //í
	upperOO[8] = { 0x04, 0x04, 0x0e, 0x11, 0x11, 0x11, 0x11, 0x0e }, //Ó
	lowerOO[8] = { 0x04, 0x04, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e }, //ó
	upperDO[8] = { 0x0a, 0x0a, 0x0e, 0x11, 0x11, 0x11, 0x11, 0x0e }, //Õ
	lowerDO[8] = { 0x0a, 0x0a, 0x00, 0x0e, 0x11, 0x11, 0x11, 0x0e }, //õ
	upperUU[8] = { 0x04, 0x04, 0x11, 0x11, 0x11, 0x11, 0x13, 0x0d }, //Ú
	lowerUU[8] = { 0x04, 0x04, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0d }, //ú
	upperDU[8] = { 0x0a, 0x0a, 0x11, 0x11, 0x11, 0x11, 0x13, 0x0d }, //Û
	lowerDU[8] = { 0x0a, 0x0a, 0x00, 0x11, 0x11, 0x11, 0x13, 0x0d }; //û

void _setDataDirection(int dir)
{
	if (dir)
		DDRE &= ~(LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4);
	else
		DDRE |= LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4;
}		

void _writeRegister(int reg, uint8_t data)
{
	_setDataDirection(LCD_DIR_WR);
	if (reg)
		PORTF |= LCD_PIN_RS;
	PORTF &=~ LCD_PIN_RW;
	_delay_ms(1);
	PORTF |= LCD_PIN_EN;
	_delay_ms(1);
	PORTE |= (data & 15) << 4;
	_delay_ms(1);
	PORTF &= 0xf1;
	_delay_ms(1);
	PORTE &= 15;
	_delay_ms(1);
}

uint8_t _readRegister(int reg)
{
	_setDataDirection(LCD_DIR_RD);
	//TODO
	return 0;
}

uint8_t _lcdRowAddr(int row)
{
	switch (row)
	{
		case 0:	return 0x00; break;
		//case 1:	return 0x28; break;
		case 1:	return 0x40; break;
		case 2:	return 0x10; break;
		case 3:	return 0x50; break;
	}
	return 0;
}


void wait4BF()
{
	//TODO
}

void lcdSendCmd(uint8_t cmd)
{
	_writeRegister(LCD_CTRL_REGISTER, (cmd & 0xf0) >> 4);
	_writeRegister(LCD_CTRL_REGISTER, cmd & 0x0f);
	_delay_ms(1);
}

void lcdSendByte(uint8_t data)
{
	_writeRegister(LCD_DATA_REGISTER, (data & 0xf0) >> 4);
	_writeRegister(LCD_DATA_REGISTER, data & 0x0f);
	_delay_ms(1);
}

void lcdInit()
{
	DDRF = LCD_PIN_EN | LCD_PIN_RW | LCD_PIN_RS;
	
	_delay_ms(40);
	
	_writeRegister(LCD_CTRL_REGISTER, 3);
	_delay_ms(5);
	_writeRegister(LCD_CTRL_REGISTER, 3);
	_delay_ms(1);
	_writeRegister(LCD_CTRL_REGISTER, 3);
	_delay_ms(1);
	_writeRegister(LCD_CTRL_REGISTER, 2);
	
	lcdSendCmd(LCD_CMD_FUNCSET | 8);
	lcdSendCmd(LCD_CMD_DSPCTRL | 14);
	lcdSendCmd(LCD_CMD_CLRDISP);
	lcdSendCmd(LCD_CMD_ENTMODE | 2);
	lcdSendCmd(LCD_CMD_RETHOME);

#if NO_POST
	lcdPrintHun("  T-Bird alapú", "ArtNetv4+SLLCPv1", "   csomópont", ">> Betöltés...");
	_delay_ms(500);
	lcdPrintHun("     T-Bird     ", "    firmware    ", "  verzió v1.0", ">> Betöltés...");
	_delay_ms(200);
#endif
}

void lcdCreateChar(uint8_t location, char charmap[])
{
	lcdSendCmd(LCD_CMD_CGRAMAD | (location << 3));
	for (int i = 0; i < 8; i++)
		lcdSendByte(charmap[i]);
}

void lcdPrint(const char* str, int row)
{
	Static_SetCursorRow(row);

	int i = 0;
	while (i < 16 && *str)
	{
		lcdSendByte(*str);
		str++;
		i++;
	}
}

int lcdPrintHun(const char* line0, const char* line1, const char* line2, const char* line3)
{
	char hunLetters[19] = {
		0xc1, 0xc9, 0xcd,	//Á, É, Í,
		0xd3, 0xd5, 0xd6,	//Ó, Õ, Ö,
		0xda, 0xdb, 0xdc,	//Ú, Û, Ü,
		0xe1, 0xe9, 0xed,	//á, é, í,
		0xf3, 0xf5, 0xf6,	//ó, õ, ö,
		0xfa, 0xfb, 0xfc,	//ú, û, ü
		0x00};
	
	char* replacement[19] = {
		upperAA, upperEE, upperII,
		upperOO, upperDO,    NULL,
		upperUU, upperDU,    NULL,
		lowerAA, lowerEE, lowerII,
		lowerOO, lowerDO,    NULL,
		lowerUU, lowerDU,    NULL,
	0x00};
	
	int len[5] = {
		0,
		strlen(line0),
		strlen(line1),
		strlen(line2),
		strlen(line3)
	};
	
	int fcnt = 0, offset = 0, found[8];
	char concat[65], subbuff[17];
	memset(found, -1, 9 * sizeof(int));
	memset(concat, 0, 65);
	
	char bufsrtcatl[17];
	STRCATL(concat, bufsrtcatl, line0, 16);
	STRCATL(concat, bufsrtcatl, line1, 16);
	STRCATL(concat, bufsrtcatl, line2, 16);
	STRCATL(concat, bufsrtcatl, line3, 16);
	
	Static_Clear();
	
	for (int i = 0; concat[i] != 0; i++)
	{
		int index = ((concat[i] > 0x7f) ? _findChar(hunLetters, concat[i]) : -1);
		if (index > -1)
		{
			if (replacement[index] != NULL)
			{
				int seen = _findInt(found, index, 8);
				if (seen > -1)
					concat[i] = seen + 8;
				else
				{
					if (fcnt < limit)
					{
						found[fcnt] = index;
						concat[i] = fcnt + 8;
						fcnt++;
					}
				}
			}
			else
			{
				if (index == 5 || index == 14)
					concat[i] = 0xef;
				else
					concat[i] = 0xf5;
			}
		}
	}
		
	for (int i = 0; i < fcnt; i++)
		lcdCreateChar(i, replacement[found[i]]);
	lcdSendCmd(LCD_CMD_RETHOME);
	
	for (int i = 0; i < 4; i++)
	{
		memset(subbuff, 0, 17);
		memcpy(subbuff, &concat[len[i]] + offset, len[i + 1]);
		lcdPrint(subbuff, i);
		offset += len[i];
	}
	
	return fcnt;
}

int lcdGetLimit()
{
	return limit;
}

void lcdSetLimit(int l)
{
	limit = l;
}