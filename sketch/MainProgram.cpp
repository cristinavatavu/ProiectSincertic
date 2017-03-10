
//INCLUDES
#include <avr/io.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <stdlib.h>
#include "Defines.h"



//GLOBAL VARIABLES
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);



//PROTOTYPES
void setup();
void loop();
int Display(int, int, int);



//FUNCTIONS
int main(void)
{
	setup();
	
	lcd.setCursor(0, 0);  //coloana(0-15, 8 e mijlocul), rand (0-1)
	lcd.print("hello world");
	
	while (1)
	{
		loop();
	}
}




void setup ()
{
	lcd.begin(16, 2); //nr columns, nr rows
}


void loop()
{
	
}