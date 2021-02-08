/*
 * matrix.c
 *
 * Created: 2021.02.08. 22:34:08
 *  Author: Soma
 */ 

#include "matrix.h"
#include <avr/io.h>
#include <avr/interrupt.h>
uint8_t digit[4]={0};
int j=0;
unsigned char row=0x08;
unsigned char outNum=0;
char lastmatrixState=0;
unsigned char pincCurr;
unsigned char pincPrev[4];


void matrix(){
	unsigned char bill;
	char op=99;
	PORTC = row; //címzés //kezdõcím: 0x08, elsõ sor
	bill = PINC & 0x7f; //maszkolás 0b01111111
	pincCurr = bill;
	char currentRow=0;
	switch(row){
		case 0x08:
		currentRow=1;
		break;
		case 0x10:
		currentRow=2;
		break;
		case 0x20:
		currentRow=3;
		break;
		case 0x40:
		currentRow=4;
		break;
	}
	if (pincCurr != pincPrev[currentRow])
	{
		switch(pincCurr)
		{
			case 69:
			//lcdPrintHun("0","csipkebokor","vesszõ","");
			//outNum=outNum+0;
			op=0;
			break;
			case 14:
			//lcdPrintHun("1","kurva anyad","C","");
			//outNum=outNum+1;
			op=1;
			break;
			case 13:
			//lcdPrintHun("2","faszom","a","Cbe");
			//outNum=outNum+2;
			op=2;
			break;
			case 11:
			//lcdPrintHun("3","leesett a ","meggy","a geCbe");
			//outNum=outNum+3;
			op=3;
			break;
			case 22:
			//lcdPrintHun("4","agyhalál","","");
			//outNum=outNum+4;
			op=4;
			break;
			case 21:
			//lcdPrintHun("5","elfogyott a string","","");
			//outNum=outNum+4;
			op=5;
			break;
			default:
			break;
		}
	}
	
	if(op<9){
	if(outNum*10<255){
		if(outNum>10)
		outNum = outNum*10+op;
		else if(outNum>0){
			outNum = outNum*10+op;
		}
		else {
			outNum=op;
		}
	}
	else outNum=255;
	}
	//
	
	pincPrev[currentRow] = pincCurr;
	if( row == 0x40 )row = 0x08; //elsõ sorra állítás
	else row = row << 1; //sorcímzés léptetése
}

ISR(TIMER0_OVF_vect)
{
	//readMatrix(&outNum);
	matrix();
	write7seg(outNum);
	//segDigit(0,1);
}

void matrixInit(){
	//7seg
	DDRA=0xFF;
	//matrix
	DDRC=0x78;
	//timer interval set
	TCCR0 = (1<<CS01)| (1<<CS00);
	//interrupt enable
	sei();
}

void segEnable(int state){
	if(state)
	TIMSK=(1<<TOIE0);
	else{
		TIMSK=(0<<TOIE0);
		PORTA=0x00;
	}
}


void segDigit(uint8_t digit, uint8_t val){
	if (digit>3) return;
	if (val>9) return;
	PORTA=0x80 | (digit)<<4 | val;
}

void write7seg(char number){
	digit[0]=number%10; //elõállítás digitenként
	digit[1]=(number/10)%10;
	digit[2]=(number/100)%10;
	j = (j+1)%4; //folyamatos kiírás
	segDigit(j,digit[j]);
}