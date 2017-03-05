/*
 * LCD.h
 *
 * Created: 2017-03-05 10:48:18
 *  Author: Robin Andersson
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "TinyTimber.h"

typedef struct {
	Object super;
} LCD;

void writeChar(char ch, int pos);
void printAt(long num, int pos);
void init();
void updateNorth(LCD *self, int data);
void updateSouth(LCD *self, int data);
void updateStatus(Controller *self, int whichInterrupt);

#define initLCD() {initObject()}

#endif /* LCD_H_ */