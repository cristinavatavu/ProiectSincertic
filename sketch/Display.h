/*
 * Display.h
 *
 * Created: 3/18/2017 8:10:35 PM
 *  Author: Cristina
 */ 

#include <LiquidCrystal.h>

#ifndef DISPLAY_H_
#define DISPLAY_H_

extern LiquidCrystal lcd;

void SimplePrint(int);
void SimplePrint(float);
void TimePrint(int);
void DisplayMainMenu();
void DisplayTimeMenu();
void DisplayParamMenu();
void DisplayRunningInfo();


#endif /* DISPLAY_H_ */
