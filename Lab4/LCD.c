/*
 * LCD.c
 *
 * Created: 2017-02-21 14:31:16
 *  Author: Robin Andersson
 */

#include "LCD.h"
#include "PulseGenerator.h"
#include "TinyTimber.h"
#include <avr/io.h> 
#include <ctype.h>

void writeChar(LCD *self, char ch, int pos) {
	// Writes a char ch on position pos on the lcd
	char mask = 0x00;
	char lcddr = 0xEC;
	uint16_t SCCTable[10] =  {0x1551, 0x0110, 0x1e11, 0x1B11, 0x0B50, 0x1B41, 0x1F41, 0x0111, 0x1F51, 0x0B51};
	uint16_t sccChar = 0x0000;
	char nibble = 0x00;
	int number;
	
	// checks if ch is in 0..9
	if (isdigit(ch)) {
		number = ch - '0';
	} else if (ch <= 9 && ch >= 0) {
		number = ch;
	} else {
		return;
	}
	
	if (ch == 1) {
		LCDDR13 ^= 1;
	}

	// get the scc value from our scc table
	sccChar = SCCTable[number];
	
	// decides which mask is used depending on if the desired position is even or not
	if (pos % 2 == 0) {
		mask = 0xF0;
	} else {
		mask = 0x0F;
	}
	
	// checks if the desired position is in 0..5
	if (pos < 0 || pos > 5) {
		return;
	}
	
	// 5, 4 >> LCDDR2 and 3, 2 >> LCDDR1 and 1, 0 >> LCDDR0
	lcddr += pos >> 1;
	
	// use our 4 nibbles to show the desired number at the desired position
	for (int i = 0; i < 4; i++) {
		nibble = sccChar & 0x000f;
		sccChar = sccChar >> 4;
		
		// shift nibble left 4 times if pos is uneven because then mask = 0x0F
		if (pos % 2 != 0) {
			nibble = nibble << 4;
		}

		// use the nibble and the mask to light the parts that nibble says
		_SFR_MEM8(lcddr) = _SFR_MEM8(lcddr) | ((_SFR_MEM8(lcddr) & mask) | nibble);
		lcddr += 5;
	}
}

void printAt(LCD *self, int num, int pos) {
	int pp = pos;
	writeChar(self, (num % 100) / 10 + '0', pp);
	pp++;
	writeChar(self, num % 10 + '0', pp);
}

void init(LCD *self, int arg) {
	// disables clock prescaler
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// enables the logical interrupt sources
	EIMSK = 0xC0;
	EIFR = 0xC0;
	PCMSK1 = 0xD0;
	PCMSK0 = 0x0C;
	
	PORTB = 0xD0;			//PORTB = (1 << PORTB7)|(1 << PORTB6)|(1 << PORTB4);
	PORTE = 0x0C;			//PORTE = (1 << PORTE3)|(1 << PORTE2);
	
	// LCD enabled, low power waveform, no frame interrupt, no blanking
	LCDCRA = 0xC0;		// (1 << LCDEN) | (1 << LCDAB);
	// external asynchronous clock source, 1/3 bias, 1/4 duty cycle, 25 segments enabled
	LCDCRB = 0xB7;		// (1 << LCDCS) | (1 << LCDMUX0) | (1 << LCDMUX1) | (1 << LCDPM0) | (1 << LCDPM1) | (1 << LCDPM2);
	// prescaler setting N=16, clock divider setting D=8
	LCDFRR = 0x07;		// (1 << LCDCD0) | (1 << LCDCD1) | (1 << LCDCD2);
	// drive time 300 microseconds, contrast control voltage 3.35 V
	LCDCCR = 0x0F;		// (1 << LCDCC0) | (1 << LCDCC1) | (1 << LCDCC2) | (1 << LCDCC3);
	
	LCDDR0 = 0x4; // init left pulse
	
	//TCCR1B = 0x0D;
}

void updateLCD(LCD *self, int arg) {
	printAt(self, self->g1->frequency, 0);
	printAt(self, self->g2->frequency, 4);
}

void change(LCD *self, int pulse) {
	if (pulse == 4) {
		LCDDR0 = 0x4;
		LCDDR1 &= 0xFD;
	} else if (pulse == 6) {
		LCDDR0 &= 0xFB;
		LCDDR1 = 0x2;
	}
}
