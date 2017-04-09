
//INCLUDES
#include <avr/io.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <stdlib.h>
#include "Main.h"
#include <EEPROM.h>
#include "Menu.h"
#include "Display.h"
#include "EEPROMfct.h"
#include "PID_v1.h"



//GLOBAL VARIABLES
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int button = BUTTON_READ_PIN;
int downCounter = 0;
int upCounter = 0;
int backCounter = 0;
int upFactor = 1;
int downFactor = 1;
int upResetDebouncing = 0;
int downResetDebouncing= 0;
int backResetDebouncing = 0;
float current_temp = 0;
double Setpoint, Input, Output;
float tempAvg = 0;
int tempAvgIndex =0;

//int current_menu = MENU_MAIN;
int debounce = 0;
int pressedButton = NO_BUTTON_PRESSED;
Parameters_t Parameters_st;

PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

void ManageDebouncing(int, int);  //schimba factorul cu care scad sau cresc parametrii in functie de tempul cat butonul a fost apasat
								  //in cazul butonului de back timpul de apasare schimba fucntia
								  //butonul de ok nu are nevoie de debouncing de acest tip si se face printr-o variabila
void ExecuteFactoryReset();





//FUNCTIONS
void setup() {
	
	lcd.begin(16, 2); //nr columns, nr rows
	pinMode(BUTTON_READ_PIN, INPUT_PULLUP);
	pinMode(TEMP_SENSOR_READ_PIN, INPUT);
	pinMode(BULB_PIN_PWM, OUTPUT);
	
	EEPROM_readAnything(0, Parameters_st); //parametrii de functionare se incarca din EEPROM
	myPID.SetTunings(Parameters_st.kp, Parameters_st.ki, Parameters_st.kd);
	Setpoint = Parameters_st.TSet;
	
	if((Parameters_st.status != 0x05) && (Parameters_st.status != 0x00)) //verifica statusul procesului anterior
	{
		lcd.setCursor(0,0);
		lcd.print("Proces anterior");
		lcd.setCursor(0,1);
		lcd.print("nefinalizat!");
		delay(3000);
	}
	myPID.SetMode(AUTOMATIC);
	DisplayMainMenu();
	
}



void loop() {
	
	int valoare = analogRead(button);
	float voltage = valoare * (5.0 / 1023.0);

	
	//TESTARE BUTOANE
/*
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(voltage);*/
	

	if(voltage<=1) //cand e apasat un buton se citesc maxim 1V, pt orice eventualitate
	{
		if(voltage>= 0.06 && voltage<= 0.08) //apasat butonul down
		{
			ManageDebouncing(BUTTON_DOWN, BUTTON_PRESSED);
			
			if(downCounter%3 == 0)
			{
				pressedButton = BUTTON_DOWN;
				MenuHandler(pressedButton);
			}
		}
		else if(voltage >= 0.20 && voltage <= 0.23) //apasat butonul 2 - up
		{
			ManageDebouncing(BUTTON_UP, BUTTON_PRESSED);
			if(upCounter % 3 == 0)
			{
				pressedButton = BUTTON_UP;
				MenuHandler(pressedButton);
			}
		}
		else if(voltage >= 0.75 && voltage <= 0.79) // apasat butonul 3 - ok
		{
			debounce ++;
			if(debounce == 3)
			{
				pressedButton = BUTTON_OK;
				MenuHandler(pressedButton);
				debounce = 0;
			}
		}
		else if(voltage >= 0.85 && voltage <= 0.9) //apasat butonul 4 - back sau factory reset
		{
			ManageDebouncing(BUTTON_BACK, BUTTON_PRESSED);	
		}
			delay(100);
	}
	else //button released
	{
		pressedButton = NO_BUTTON_PRESSED;
		if(downCounter > 0)  //s-a eliberat butonul de down, se face debounce pana cand sa se reseteze counterul
		{
			ManageDebouncing(BUTTON_DOWN, BUTTON_RELEASED);
		}
		if(upCounter > 0)
		{
			ManageDebouncing(BUTTON_UP, BUTTON_RELEASED);
		}
		if(backCounter >0)
		{
			ManageDebouncing(BUTTON_BACK,BUTTON_RELEASED);
		}
			delay(100);
	}
	
	
	if(current_menu == MENU_RUNNING)  //citirile de la senzor si transmiterea de PWM se fac doar cand procesul e in rulare
	{
		float voltage = (float) analogRead(TEMP_SENSOR_READ_PIN) * (5000.0 / 1023.0); //voltajul in mV de la senzor
		if(tempAvgIndex < 5) //face media a 5 masuratori
		{
			tempAvg += (float) voltage/10.0; //10mV = 1grC
			tempAvgIndex ++;
		}
		else
		{
			tempAvgIndex = 0;
			current_temp = (float)tempAvg/5.0;
			tempAvg = 0;
			DisplayRunningInfo();
		}
		//Input = current_temp;
		//PWMGenerator(Output);
		PWMGenerator(100);
		delay(500);

		

	}
}








void ManageDebouncing(int button, int buttonState)
{
	//resetFactor = 0 : button pressed, count the number of presses, if they reached the threshold, change the multiplication factor
	//resetFactor = 1 : button released, count if it was released for enough time, if yes, reset all the counters and the factor
	switch(button)
	{
		case BUTTON_DOWN:
		{
			if(buttonState == BUTTON_PRESSED) 
			{
				downCounter ++;
				if(downCounter == FACTOR10_THRESH)
				{
					downFactor = 10;
				}
				else if(downCounter == FACTOR100_THRESH)
				{
					downFactor = 100;
				}

			}
			else if (buttonState == BUTTON_RELEASED)
			{
				downResetDebouncing ++;
				if(downResetDebouncing == 5)
				{
					downResetDebouncing = 0;
					downCounter = 0;
					downFactor = 1;
				}
			}
			
		}break;
		
		case BUTTON_UP:
		{
			if(buttonState == BUTTON_PRESSED) //button pressed
			{
				upCounter ++;
				if(upCounter == FACTOR10_THRESH)
				{
					upFactor = 10;
				}
				else if(upCounter == FACTOR100_THRESH)
				{
					upFactor = 100;
				}
			}
			else if(buttonState == BUTTON_RELEASED)
			{
				upResetDebouncing ++;
				if(upResetDebouncing == 5)
				{
					upResetDebouncing = 0;
					upCounter = 0;
					upFactor = 1;
				}	
			}	
		}break;
		
		case BUTTON_BACK:   //pressing the back button in any menu continuously for 30 times generates a factory reset
		{
			if(buttonState == BUTTON_PRESSED)
			{
				backCounter ++;
				if(backCounter == 40)
				{
					ExecuteFactoryReset();
				}
			}
			else if (buttonState == BUTTON_RELEASED)
			{
				backResetDebouncing++;
				if(backResetDebouncing == 5 && backCounter < 12) //dc dupa mai putin de 12 apasari se elibereaza butonul de back nu se vrea factory reset
				{
					backResetDebouncing = 0;
					backCounter = 0;
					pressedButton = BUTTON_BACK;
					MenuHandler(pressedButton);
				}
			}
			
				
		}break;
	}
}







void ExecuteFactoryReset()
{
	//reset all working variables
	int downCounter = 0;
	upCounter = 0;
	backCounter = 0;
	upFactor = 1;
	downFactor = 1;
	upResetDebouncing = 0;
	downResetDebouncing= 0;
	debounce = 0;
	current_menu = MENU_MAIN;
	
	//reset all process parameters to default values
	Parameters_st.status = DEFAULT_STATUS;
	Parameters_st.kd = DEFAULT_KD;
	Parameters_st.kp = DEFAULT_KP;
	Parameters_st.ki = DEFAULT_KI;
	Parameters_st.tInc = DEFAULT_TINC;
	Parameters_st.tMen = DEFAULT_TMEN;
	Parameters_st.tRec = DEFAULT_TREC;
	Parameters_st.TSet = DEFAULT_TSET;
	
	//write the default parameters in eeprom
	EEPROM_writeAnything(0, Parameters_st);
	
	//give feedback about reset
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Reset done!");
	delay(3000);
	DisplayMainMenu();
}



void PWMGenerator(int dutyCycle)
{
	int parameter;
	float percent = (float)255/100; //parametrul maximm e 255 care inseamna 100 =>
									//=>fiecare procent din vactorul de umplere reprezinta 2.55 in plus la parametru
	
	if(dutyCycle > 100)
	{
		parameter = 255;  //limiteaza la 100%
	}
	else
	{
		parameter = (int) (percent * dutyCycle); 
	}
	
	analogWrite(BULB_PIN_PWM, parameter);
}
