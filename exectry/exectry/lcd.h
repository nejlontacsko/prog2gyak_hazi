#pragma once

#ifndef LCD_H
#define LCD_H

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

#include "utils.h"

#define LCD_PIN_D7 _BV(PE7)
#define LCD_PIN_D6 _BV(PE6)
#define LCD_PIN_D5 _BV(PE5)
#define LCD_PIN_D4 _BV(PE4)
#define LCD_PIN_EN _BV(PF3)
#define LCD_PIN_RW _BV(PF2)
#define LCD_PIN_RS _BV(PF1)

#define LCD_DIR_RD 1
#define LCD_DIR_WR 0

#define LCD_CTRL_REGISTER 0
#define LCD_DATA_REGISTER 1

#define LCD_CMD_CLRDISP 0x01
#define LCD_CMD_RETHOME 0x02
#define LCD_CMD_ENTMODE 0x04
#define LCD_CMD_DSPCTRL 0x08
#define LCD_CMD_CDSHIFT 0x10
#define LCD_CMD_FUNCSET 0x20
#define LCD_CMD_CGRAMAD 0x40
#define LCD_CMD_DDRAMAD 0x80

#define LCD_EMPTY_LINE ((char[]){0x00})
	
#define LCD_FLAG_BIT _BV(7)
#define LCD_CURR_ADD (~LCD_FLAG_BIT)

#define Static_Print(l0, l1, l2, l3) \
   _Static_assert(STRLEN(l0) <= 17, "Line 0 is too long."); \
   _Static_assert(STRLEN(l1) <= 17, "Line 1 is too long."); \
   _Static_assert(STRLEN(l2) <= 17, "Line 2 is too long."); \
   _Static_assert(STRLEN(l3) <= 17, "Line 3 is too long."); \
   lcdPrintHun(l0, l1, l2, l3)
   
#define Static_Limit(l) \
   _Static_assert(l > 1 || l < 8, "Custom character limit must be in [1..8] interval."); \
   DO_PRAGMA(message( \
		"[Line " STRINGIFY(__LINE__) "] Limit sat to " #l ".")); \
   lcdSetLimit(l)
   
#define Static_Blink(b) \
	lcdSendCmd(LCD_CMD_DSPCTRL | _BV(2) | (b & 1))
	
#define Static_SetCursorPos(x, y) \
	lcdSendCmd(LCD_CMD_DDRAMAD | (_lcdRowAddr(x) + y))

#define Static_SetCursorRow(row) \
	Static_SetCursorPos(row, 0)
   
#define Static_CustomChar(location, charmap) \
	lcdCreateChar(location, charmap);  \
	DO_PRAGMA(message( \
		"[Line " STRINGIFY(__LINE__) "] The '" #charmap "' character stored in DDRAM location " #location ".")); \
	lcdSetLimit(lcdGetLimit() - 1); \
	DO_PRAGMA(message("\tDynamic accented letter limit decreased by 1! (Max. 8)"))
	
#define Static_Clear() \
	lcdSendCmd(LCD_CMD_CLRDISP); \
	lcdSendCmd(LCD_CMD_RETHOME)

void _setDataDirection(int);

void    _writeRegister(int, uint8_t);
uint8_t _readRegister(int);

uint8_t	_lcdRowAddr(int);

void    lcdWait4BF();

void    lcdSendCmd(uint8_t);
void    lcdSendByte(uint8_t);

void    lcdInit();

void    lcdCreateChar(uint8_t, char*);

void    lcdPrint(const char*, int);

int     lcdPrintHun(const char*, const char*, const char*, const char*);

int     lcdGetLimit();
void    lcdSetLimit(int);

#endif