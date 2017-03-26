/*
 * Display.cpp
 *
 * Created: 3/18/2017 8:10:51 PM
 *  Author: Cristina
 */ 

#include <avr/io.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <stdlib.h>
#include "Main.h"
#include "Display.h"
#include "Menu.h"


//************************************************************ DISPLAY FUNCTIONS ****************************************************************

void SimplePrint(int valoare)
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(valoare);
}

void SimplePrint(float valoare)
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(valoare);
}


void TimePrint(int sec)
{
	int min = 0;
	lcd.clear();
	
	if(sec >= 60)
	{
		min = sec/60;
		sec = sec % 60;
	}
	
	if(min < 10)
	{
		lcd.setCursor(0,0);
		lcd.print("0");
		lcd.setCursor(1,0);
		lcd.print(min);
	}
	else
	{
		lcd.setCursor(0,0);
		lcd.print(min);
	}
	

	
	lcd.setCursor(2,0);
	lcd.print(":");
	
	if(sec < 10)
	{
		lcd.setCursor(3,0);
		lcd.print("0");
		lcd.setCursor(4,0);
		lcd.print(sec);
	}
	else
	{
		lcd.setCursor(3,0);
		lcd.print(sec);
	}
	
		
}


void DisplayMainMenu()
{
	current_menu = MENU_MAIN;
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("1.Go!");
	
	lcd.setCursor(0,1);
	lcd.print("3.Time");
	
	lcd.setCursor(8,0);
	lcd.print("2.T");
	//lcd.print("2.T'C");
	lcd.setCursor(11,0);
	lcd.print((char)223); //printeaza semnul de grad
	lcd.setCursor(12,0);
	lcd.print("C");
	
	lcd.setCursor(8,1);
	lcd.print("4.Reg");
}

void DisplayTimeMenu()
{
	current_menu = MENU_TIME;
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("1.t Inc");
	
	lcd.setCursor(0,1);
	lcd.print("3.t Rcr");
	
	lcd.setCursor(8,0);
	lcd.print("2.t Men");
	
	lcd.setCursor(8,1);
	lcd.print("4.Back");
	
}

void DisplayParamMenu()
{
	current_menu = MENU_PARAM;
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("1.kd");
	
	lcd.setCursor(0,1);
	lcd.print("3.ki");
	
	lcd.setCursor(8,0);
	lcd.print("2.kp");
	
	lcd.setCursor(8,1);
	lcd.print("4.Back");
	
}