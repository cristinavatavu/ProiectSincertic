
//INCLUDES
#include <avr/io.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <stdlib.h>
#include "Defines.h"
#include <EEPROM.h>




//GLOBAL VARIABLES
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int button = A0;
int current_menu = MENU_MAIN;
int TSet = 0; //temp setata
float kd = 0.0;  
float ki = 0.0;
float kp = 0.0;
int tInc = 0; //timpul de incalzire - in secunde
int tMen = 0; //timpul de mentinere - in secunde
int tRec = 0; //timpul de descrestere atemp - in secunde
int debounce = 0;
int pressedButton = NO_BUTTON_PRESSED;
int addr;




//PROTOTYPES
void MenuHandler(int);
void MainMenu(int);
void TempMenu(int);
void TimeMenu(int);
void ParamMenu(int);
void ParamSubmenu(int);
void TimeSubmenu(int);
void DisplayMainMenu();
void DisplayParamMenu();
void DisplayTimeMenu();
void SimplePrint(int); //printeaza un int pe prima linie
void TimePrint(int); //printeaza timpul in format timp





//FUNCTIONS
void setup() {
	lcd.begin(16, 2); //nr columns, nr rows
	pinMode(A0, INPUT_PULLUP);
	DisplayMainMenu();
}



void loop() {
	//lcd.setCursor(0,0);
	int valoare = analogRead(button);
	float voltage = valoare * (5.0 / 1023.0);
	
	//TESTARE BUTOANE
/*
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(voltage);*/
	

	if(voltage<=1) //cand e apasat un buton se citesc maxim 1V, 1.2 pt orice eventualitate
	{
		if(voltage>= 0.06 && voltage<= 0.08) //apasat butonul down
		{
			debounce ++;
			if(debounce == 3)
			{
				pressedButton = BUTTON_DOWN;
				MenuHandler(pressedButton);
				debounce = 0;
			}
		}
		else if(voltage >= 0.35 && voltage <= 0.4) //apasat butonul 2 - up
		{
			debounce ++;
			if(debounce == 3)
			{
				pressedButton = BUTTON_UP;
				MenuHandler(pressedButton);
				debounce = 0;
			}
		}
		else if(voltage >= 0.85 && voltage <= 0.92) // apasat butonul 3 - ok
		{
			debounce ++;
			if(debounce == 3)
			{
				pressedButton = BUTTON_OK;
				MenuHandler(pressedButton);
				debounce = 0;
			}
		}
		else if(voltage >= 0.95 && voltage <= 1) //apasat butonul 4 - back
		{
			debounce ++;
			if(debounce == 3)
			{
				pressedButton = BUTTON_BACK;
				MenuHandler(pressedButton);
				debounce = 0;
			}
			
		}
	}
	else
	{
		pressedButton = NO_BUTTON_PRESSED;
	}
	
	/*lcd.setCursor(15, 0);
	lcd.print(debounce);*/

	delay(100);


}




//************************************************************ MENU HANDLER ********************************************
//selecteaza ce e de facut in functie de meniul curent
void MenuHandler(int btn)
{
	switch(current_menu)
	{
		case MENU_MAIN:
		{
			MainMenu(btn);
		}
		break;
		
		case MENU_TEMP:
		{
			TempMenu(btn);
		}
		break;
		
		case MENU_TIME:
		{
			TimeMenu(btn);
		}
		break;
		
		case MENU_PARAM:
		{
			ParamMenu(btn);
		}break;
		
		//submeniuri
		case MENU_KD:
		case MENU_KI:
		case MENU_KP:
		{
			ParamSubmenu(btn);	
		}
		break;
		
		case MENU_TINC:
		case MENU_TMEN:
		case MENU_TDEC:
		{
			TimeSubmenu(btn);
		}
		break;
		
		default:
		{
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Menu error");
		}break;
	}
	
}








//***************************************************************************************************************************
//********************************************************* SUBMENUES ***********************************************
//***************************************************************************************************************************
//trece dintr-un meniu in altul, selecteaza un item

//*****************************************  MAIN MENU
void MainMenu(int btn)
{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - start
			{
				lcd.clear();
				lcd.setCursor(7,0);
				lcd.print(":)");
				delay(4000);
				lcd.clear();
				lcd.setCursor(5,0);
				lcd.print("RUNNING");
			}
				
				
			else if(btn == BUTTON_UP) //apasat butonul 2 - Temp
			{
				TSet = 0;
				SimplePrint(TSet);
				lcd.setCursor(3, 0);
				lcd.print("'C");
				current_menu = MENU_TEMP;
			}
				
				
				
			else if(btn == BUTTON_OK) // apasat butonul 3 - timp
			{
					
					tInc = 0;  //s-ar putea ca initializarile astea sa nu fie bune aici
					tMen = 0;
					tRec = 0;
					DisplayTimeMenu();	
			}
				
				
			else if(btn == BUTTON_BACK) //apasat butonul 4 - param reg
			{
				kp = 0;
				kd = 0;
				ki = 0;
				DisplayParamMenu();
			}
				
}




//********************************************************* TEMP MENU
void TempMenu(int btn)
{
				if(btn == BUTTON_DOWN) //apasat butonul 1 - down
				{
					if(TSet > 0)
					{
						TSet --;
						SimplePrint(TSet);
						lcd.setCursor(3, 0);
						lcd.print("'C");
					}
				}
				else if(btn == BUTTON_UP) //apasat butonul 2 - up
				{
					TSet ++;
					SimplePrint(TSet);
					lcd.setCursor(3, 0);
					lcd.print("'C");
				}
				else if(btn == BUTTON_OK) // apasat butonul 3 - ok
				{
					//salveaza in EEPROM
					DisplayMainMenu();
				}
				else if(btn == BUTTON_BACK) //apasat butonul 4 - back to main menu
				{
					DisplayMainMenu();
					
				}
}



//********************************************************  TIME MENU
void TimeMenu(int btn)
{
				if(btn == BUTTON_DOWN) //apasat butonul 1 - tInc
				{
					TimePrint(tInc);
					current_menu = MENU_TINC;
				}
				
				
				else if(btn == BUTTON_UP) //apasat butonul 2 - tMen
				{
					TimePrint(tMen);
					current_menu = MENU_TMEN;
				}
				
				
				else if(btn == BUTTON_OK) // apasat butonul 3 - tRcr
				{
						TimePrint(tRec);
						current_menu = MENU_TDEC;
				}
				else if(btn == BUTTON_BACK) //apasat butonul 4 - back to main menu
				{
					DisplayMainMenu();
				}
				
}




//************************************************ PARAM MENU
void ParamMenu(int btn )
{
				if(btn == BUTTON_DOWN) //apasat butonul 1 - kd
				{
					SimplePrint(kd);
					current_menu = MENU_KD;
				}
				
				else if(btn == BUTTON_UP) //apasat butonul 2 - kp
				{
					SimplePrint(kp);
					current_menu = MENU_KP;
				}
				
				
				else if(btn == BUTTON_OK) // apasat butonul 3 - ki
				{	
					SimplePrint(ki);
					current_menu = MENU_KI;
				}
				else if(btn == BUTTON_BACK) //apasat butonul 4 - back to main menu
				{
					DisplayMainMenu();
				}
				
}




//****************************************** PARAM SUBMENU
void ParamSubmenu(int btn)
{
	//pt salvare in EEPROM trebuie facut si un case in fct de current_menuu ca sa stii ce sa salvezi
	
	switch (current_menu)
	{
		case MENU_KI:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				if(ki > 0)
				{
					ki--;
					SimplePrint(ki);
				}
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
				ki++;
				SimplePrint(ki);
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				//salvare KI in EEPROM
				DisplayParamMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				DisplayParamMenu();
			}

		}break;
		
		case MENU_KP:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				if(kp > 0)
				{
					kp--;
					SimplePrint(kp);
				}
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
				kp++;
				SimplePrint(kp);
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				//salvare in EEPROM
				DisplayParamMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				DisplayParamMenu();
			}
		}break;
		
		case MENU_KD:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				if(kd > 0)
				{
					kd--;
					SimplePrint(kd);
				}
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
				kd++;
				SimplePrint(kd);
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				//salvare in EEPROM
				DisplayParamMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				DisplayParamMenu();
			}
		}break;
		
	}
	
}



//******************************************** TIME SUBMENU
void TimeSubmenu(int btn)
	//pt salvare in EEPROM trebuie facut si un case in fct de current_menuu ca sa stii ce sa salvezi
{
	switch(current_menu)
	{
		case MENU_TINC:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				if(tInc>0)
				{
					tInc --;
					TimePrint(tInc);
				}
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
				tInc ++;
				TimePrint(tInc);
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				DisplayTimeMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				DisplayTimeMenu();
			}
		}break;
		
		case MENU_TMEN:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				if(tMen>0)
				{
					tMen --;
					TimePrint(tMen);
				}
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
					tMen++;
					TimePrint(tMen);
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				DisplayTimeMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				DisplayTimeMenu();	
			}
		}break;
		
		case MENU_TDEC:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				if(tRec>0)
				{
					tRec --;
					TimePrint(tRec);
				}
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
					tRec++;
					TimePrint(tRec);
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				DisplayTimeMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				DisplayTimeMenu();
			}
		}break;
		
	}

}











//************************************************************ DISPLAY FUNCTIONS ****************************************************************

void SimplePrint(int valoare)
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
	lcd.setCursor(3,0);
	lcd.print(sec);
		
}


void DisplayMainMenu()
{
	current_menu = MENU_MAIN;
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("1.Go!");
	
	lcd.setCursor(8,0);
	lcd.print("3.Time");
	
	lcd.setCursor(0,1);
	lcd.print("2.T'C");
	
	lcd.setCursor(8,1);
	lcd.print("4.Reg");
}

void DisplayTimeMenu()
{
	current_menu = MENU_TIME;
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("1.t Inc");
	
	lcd.setCursor(8,0);
	lcd.print("3.t Rcr");
	
	lcd.setCursor(0,1);
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
	
	lcd.setCursor(8,0);
	lcd.print("3.ki");
	
	lcd.setCursor(0,1);
	lcd.print("2.kp");
	
	lcd.setCursor(8,1);
	lcd.print("4.Back");
	
}
