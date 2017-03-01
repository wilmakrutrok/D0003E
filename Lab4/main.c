/*
 * Lab4.c
 *
 * Created: 2017-02-21 11:33:14
 * Author : Robin Andersson
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "PulseGenerator.h"
#include "GUI.h"
#include "TinyTimber.h"
#include "LCD.h"

PulseGenerator g1 = initPulseGenerator(4, 0, 0, 0, 1);		//freq, pos, old, saved, gate, whichpulse
PulseGenerator g2 = initPulseGenerator(6, 0, 0, 0, 1);
LCD lcd = initLCD(&g1, &g2, 0);
GUI gui = initGUI(&g1, &g1, &g2, &lcd);

int main(void) {
  init();
  
  INSTALL(&gui, changePortE, IRQ_PCINT0);
  INSTALL(&gui, changePortB, IRQ_PCINT1);
  
  return TINYTIMBER(&lcd, updateLCD, 0);
}

