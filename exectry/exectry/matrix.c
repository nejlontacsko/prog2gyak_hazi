/*
 * matrix.c
 *
 * Created: 2021.02.08. 22:34:08
 *  Author: Soma
 */ 

#include "matrix.h"
#include "utils.h"
#include "LCD_T-Bird3.h"
#include <avr/io.h>
#include <avr/interrupt.h>
uint8_t digit[4]={0};
int j=0;
unsigned char row=0x08;
unsigned char outNum=0;
unsigned char pincCurr;
unsigned char pincPrev[4];
unsigned char currentByte=0;
unsigned char addresstoSet=0;
ipAddress ip;
ipAddress subnet;


void matrix(){
	unsigned char bill;
	char op=99;
	PORTC = row; //címzés //kezdõcím: 0x08, elsõ sor
	bill = PINC & 0x7f; //maszkolás 0b01111111
	pincCurr = bill;
	unsigned char currentRow=0;
	switch(row){
		case 0x08:
		currentRow=0;
		break;
		case 0x10:
		currentRow=1;
		break;
		case 0x20:
		currentRow=2;
		break;
		case 0x40:
		currentRow=3;
		break;
	}

	if (pincCurr != pincPrev[currentRow])
	{
		switch(pincCurr)
		{
			case 69:
				op=0;
			break;
			
			case 14:
				op=1;
			break;
			
			case 13:
				op=2;
			break;
			
			case 11:
				op=3;
			break;
			
			case 22:
				op=4;
			break;
			
			case 21:
				op=5;
			break;
			
			case 19:
				op=6;
			break;
			
			case 38:
				op=7;
			break;
			
			case 37:
				op=8;
			break;
			
			case 35:
				op=9;
			break;
			
			case 70:
				op=10;
			break;
			
			case 67:
				op=11;
			break;
			
			default:;
			break;
		}
	}
	matrixProcess(op);
	pincPrev[currentRow] = (pincCurr);
	if( row == 0x40 ) row = 0x08; //elsõ sorra állítás
else row = row << 1; //sorcímzés léptetése
}
int readMatrix(){
	for(char rowN=0x08;rowN<=0x40;rowN=rowN<<1){
	unsigned char bill;
	char op=99;
	PORTC = rowN; //címzés //kezdõcím: 0x08, elsõ sor
	bill = PINC & 0x7f; //maszkolás 0b01111111
	pincCurr = bill;
	unsigned char currentRow=0;
	switch(rowN){
		case 0x08:
		currentRow=0;
		break;
		case 0x10:
		currentRow=1;
		break;
		case 0x20:
		currentRow=2;
		break;
		case 0x40:
		currentRow=3;
		break;
	}

	if (pincCurr != pincPrev[currentRow])
	{
		switch(pincCurr)
		{
			case 69:
			op=0;
			return 0;
			break;
			
			case 14:
			op=1;
			return 1;
			break;
			
			case 13:
			op=2;
			return 2;
			break;
			
			case 11:
			op=3;
			return 3;
			break;
			
			case 22:
			op=4;
			return 4;
			break;
			
			case 21:
			op=5;
			return 5;
			break;
			
			case 19:
			op=6;
			return 6;
			break;
			
			case 38:
			op=7;
			return 7;
			break;
			
			case 37:
			op=8;
			return 8;
			break;
			
			case 35:
			op=9;
			return 9;
			break;
			
			case 70:
			op=10;
			return 10;
			break;
			
			case 67:
			op=11;
			return 11;
			break;
			
			default:;
			return 99;
			break;
		}
		//return op;
	}
		
	pincPrev[currentRow] = (pincCurr);
	
}
	/*if( row == 0x40 ) row = 0x08; //elsõ sorra állítás
	else row = row << 1; //sorcímzés léptetése*/
}

void matrixProcess(int op){
	switch(op){
		case 10:
		if(currentByte<4){
			if(addresstoSet==0)
			ip.ip[currentByte]=outNum;
			else
			subnet.ip[currentByte]=outNum;
			printIps();
			outNum=0;
			currentByte++;
			if(currentByte==4){
				addresstoSet=!addresstoSet;
				outNum=0;
				currentByte=0;
				printIps();
			}
		}
		break;
		case 11:
		if(outNum){
			if(addresstoSet==0)
			ip.ip[currentByte]=outNum;
			else
			subnet.ip[currentByte]=outNum;
		}
		addresstoSet=!addresstoSet;
		outNum=0;
		currentByte=0;
		printIps();
		break;
		case 99:
		break;
		default:
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
		break;
	}
}
ISR(TIMER0_OVF_vect)
{
	matrix();
	write7seg(outNum);
}

void matrixInit(){
	//7seg
	DDRA=0xFF;
	//matrix
	DDRC=0x78;
	//timer interval set
	TCCR0 = (1<<CS01)| (1<<CS00);
	//global interrupt enable
	sei();
}

void ipInputMode(int state){
	if(state){
		printIps();
		TIMSK=(1<<TOIE0);
	}
	else{
		TIMSK=(0<<TOIE0);
		PORTA=0x00;
	}
}

void write7seg(char number){
	digit[0]=number%10; //elõállítás digitenként
	digit[1]=(number/10)%10;
	digit[2]=(number/100)%10;
	j = (j+1)%3; //folyamatos kiírás
	PORTA=0x80 | (j)<<4 | digit[j];
}

void printIps(){
	Static_Clear();
	char ipString[16];
	iptoString(ip,ipString,!addresstoSet);
	char subnetString[16];
	iptoString(subnet,subnetString,addresstoSet);
	lcdPrintHun("IP address:",ipString,"Subnet address",subnetString);
}
