/*
 * Menu.cpp
 *
 * Created: 3/11/2017 5:20:45 PM
 *  Author: Cristina
 */ 


#include <avr/io.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <string.h>
#include <stdlib.h>
#include <EEPROM.h>
#include "Main.h"
#include "Menu.h"
#include "Display.h"
#include "EEPROMfct.h"


//Global variables
int current_state = MENU_MAIN;
//Parameters_t Parameters_st;


//************************************************************ MENU HANDLER ********************************************
//selecteaza ce e de facut in functie de meniul curent
void MenuHandler(int btn)
{
	switch(current_state)
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
				current_state = STATE_RUNNING_INC;
				Parameters_st.status = 1;
				EEPROM_writeAnything(ADDR_STATUS, Parameters_st.status);
			}
				
				
			else if(btn == BUTTON_UP) //apasat butonul 2 - Temp
			{
				/*TSet = 0;*/
				SimplePrint(Parameters_st.TSet);
				lcd.setCursor(3, 0);
				lcd.print((char)223);
				lcd.setCursor(4,0);
				lcd.print('C');
				current_state = MENU_TEMP;
			}
				
				
				
			else if(btn == BUTTON_OK) // apasat butonul 3 - timp
			{
					DisplayTimeMenu();	
			}
				
				
			else if(btn == BUTTON_BACK) //apasat butonul 4 - param reg
			{
				DisplayParamMenu();
			}
				
}




//********************************************************* TEMP MENU
void TempMenu(int btn)
{
				if(btn == BUTTON_DOWN) //apasat butonul 1 - down
				{
					if(downFactor == 1 && (Parameters_st.TSet - 1 >= TEMP_LOWER_LIMIT) ) // nu permite sa se ceara o temp prea mica
					{
						Parameters_st.TSet --;
						SimplePrint(Parameters_st.TSet);
						lcd.setCursor(3, 0);
						lcd.print((char)223);
						lcd.print(4,0);
						lcd.print('C');
					}
					else if(downFactor == 10 && (Parameters_st.TSet - 5 >= TEMP_LOWER_LIMIT))
					{
						Parameters_st.TSet -= 5;
						SimplePrint(Parameters_st.TSet);
						lcd.setCursor(3, 0);
						lcd.print((char)223);
						lcd.setCursor(4,0);
						lcd.print('C');
					}
					else if(downFactor == 100 && (Parameters_st.TSet - 10 >= TEMP_LOWER_LIMIT))
					{
						Parameters_st.TSet -= 10;
						SimplePrint(Parameters_st.TSet);
						lcd.setCursor(3, 0);
						lcd.print((char)223);
						lcd.setCursor(4,0);
						lcd.print('C');
					}
				}
				else if(btn == BUTTON_UP) //apasat butonul 2 - up
				{
					if(upFactor == 1)
					{
						Parameters_st.TSet ++;
						SimplePrint(Parameters_st.TSet);
						lcd.setCursor(3, 0);
						lcd.print((char)223);
						lcd.print(4,0);
						lcd.print('C');
					}
					else if(upFactor == 10)
					{
						Parameters_st.TSet += 5;
						SimplePrint(Parameters_st.TSet);
						lcd.setCursor(3, 0);
						lcd.print((char)223);
						lcd.print(4,0);
						lcd.print('C');
					}
					else if(upFactor == 100)
					{
						Parameters_st.TSet += 10;
						SimplePrint(Parameters_st.TSet);
						lcd.setCursor(3, 0);
						lcd.print((char)223);
						lcd.print(4,0);
						lcd.print('C');
					}
				}
				else if(btn == BUTTON_OK) // apasat butonul 3 - ok
				{
					EEPROM_writeAnything(ADDR_TSET, Parameters_st.TSet);
					DisplayMainMenu();
				}
				else if(btn == BUTTON_BACK) //apasat butonul 4 - back to main menu
				{
					Parameters_st.TSet = DEFAULT_TSET;
					DisplayMainMenu();
					
				}
}



//********************************************************  TIME MENU
void TimeMenu(int btn)
{
				if(btn == BUTTON_DOWN) //apasat butonul 1 - tInc
				{
					TimePrint(Parameters_st.tInc);
					current_state = MENU_TINC;
				}
				
				
				else if(btn == BUTTON_UP) //apasat butonul 2 - tMen
				{
					TimePrint(Parameters_st.tMen);
					current_state = MENU_TMEN;
				}
				
				
				else if(btn == BUTTON_OK) // apasat butonul 3 - tRcr
				{
						TimePrint(Parameters_st.tDec);
						current_state = MENU_TDEC;
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
					SimplePrint(Parameters_st.kd);
					current_state = MENU_KD;
				}
				
				else if(btn == BUTTON_UP) //apasat butonul 2 - kp
				{
					SimplePrint(Parameters_st.kp);
					current_state = MENU_KP;
				}
				
				
				else if(btn == BUTTON_OK) // apasat butonul 3 - ki
				{	
					SimplePrint(Parameters_st.ki);
					current_state = MENU_KI;
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
	
	switch (current_state)
	{
		case MENU_KI:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				Parameters_st.ki -= (float)downFactor/10;
				SimplePrint(Parameters_st.ki);
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
				Parameters_st.ki += (float)upFactor/10;
				SimplePrint(Parameters_st.ki);
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				EEPROM_writeAnything(ADDR_KI, Parameters_st.ki);
				DisplayParamMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				Parameters_st.ki = DEFAULT_KI;
				DisplayParamMenu();
			}

		}break;
		
		case MENU_KP:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				Parameters_st.kp -= (float)downFactor/10;
				SimplePrint(Parameters_st.kp);
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
				Parameters_st.kp += (float)upFactor/10;
				SimplePrint(Parameters_st.kp);
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				EEPROM_writeAnything(ADDR_KP, Parameters_st.kp);
				DisplayParamMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				Parameters_st.kp = DEFAULT_KP;
				DisplayParamMenu();
			}
		}break;
		
		case MENU_KD:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				Parameters_st.kd -= (float)downFactor/10;
				SimplePrint(Parameters_st.kd);
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
				Parameters_st.kd += (float)upFactor/10;
				SimplePrint(Parameters_st.kd);
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				EEPROM_writeAnything(ADDR_KD, Parameters_st.kd);
				DisplayParamMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				Parameters_st.kd = DEFAULT_KD;
				DisplayParamMenu();
			}
		}break;
		
	}
	
}



//******************************************** TIME SUBMENU
void TimeSubmenu(int btn)
	//pt salvare in EEPROM trebuie facut si un case in fct de current_menuu ca sa stii ce sa salvezi
{
	switch(current_state)
	{
		case MENU_TINC:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{	
				if(downFactor == 1)
				{
					Parameters_st.tInc-- ; //scade cu cate o secunda
					if (Parameters_st.tInc < 0)
						Parameters_st.tInc = 0;
					TimePrint(Parameters_st.tInc);
				}
				else if(downFactor == 10)
				{
					Parameters_st.tInc -= 60 ; //scade cu cate un minut
					if (Parameters_st.tInc < 0)
						Parameters_st.tInc = 0;
					TimePrint(Parameters_st.tInc);
				}
				else if(downFactor == 100 && (Parameters_st.tInc >= 300))
				{
					Parameters_st.tInc -= 300; //scade cu cate 5 minute
					if (Parameters_st.tInc < 0)
						Parameters_st.tInc = 0;
					TimePrint(Parameters_st.tInc);
				}

			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
				if(upFactor == 1)
				{
					Parameters_st.tInc++ ; //creste cu cate o secunda
					TimePrint(Parameters_st.tInc);
				}
				else if(upFactor == 10)
				{
					Parameters_st.tInc += 60 ; //creste cu cate un minut
					TimePrint(Parameters_st.tInc);
				}
				else if(upFactor == 100)
				{
					Parameters_st.tInc += 300; //creste cu cate 5 minute
					TimePrint(Parameters_st.tInc);
				}
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				EEPROM_writeAnything(ADDR_TINC, Parameters_st.tInc);
				DisplayTimeMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				Parameters_st.tInc = DEFAULT_TINC;
				DisplayTimeMenu();
			}
		}break;
		
		case MENU_TMEN:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				if(downFactor == 1)
				{
					Parameters_st.tMen --;
					if (Parameters_st.tMen < 0)
						Parameters_st.tMen = 0;
					TimePrint(Parameters_st.tMen);
				}
				else if(downFactor == 10)
				{
					Parameters_st.tMen -= 60;
					if (Parameters_st.tMen < 0)
						Parameters_st.tMen = 0;
					TimePrint(Parameters_st.tMen);
				}
				else if(downFactor == 100)
				{
					Parameters_st.tMen -= 300;
					if (Parameters_st.tMen < 0)
						Parameters_st.tMen = 0;
					TimePrint(Parameters_st.tMen);
				}

			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
				if(upFactor == 1)
				{
					Parameters_st.tMen ++;
					TimePrint(Parameters_st.tMen);
				}
				else if(upFactor == 10)
				{
					Parameters_st.tMen += 60;
					TimePrint(Parameters_st.tMen);
				}
				else if(upFactor == 100)
				{
					Parameters_st.tMen += 300;
					TimePrint(Parameters_st.tMen);
				}
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				EEPROM_writeAnything(ADDR_TMEN, Parameters_st.tMen);
				DisplayTimeMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				Parameters_st.tMen = DEFAULT_TMEN;
				DisplayTimeMenu();	
			}
		}break;
		
		case MENU_TDEC:
		{
			if(btn == BUTTON_DOWN) //apasat butonul 1 - down
			{
				if(downFactor == 1)
				{
					Parameters_st.tDec --;
					if (Parameters_st.tDec < 0)
						Parameters_st.tDec = 0;
					TimePrint(Parameters_st.tDec);
				}
				else if(downFactor == 10)
				{
					Parameters_st.tDec -= 60;
					if (Parameters_st.tDec < 0)
						Parameters_st.tDec = 0;
					TimePrint(Parameters_st.tDec);
				}
				else if(downFactor == 100)
				{
					Parameters_st.tDec -= 300;
					if (Parameters_st.tDec < 0)
						Parameters_st.tDec = 0;
					TimePrint(Parameters_st.tDec);
				}
					
			}
			else if(btn == BUTTON_UP) //apasat butonul 2 - up
			{
					if(upFactor == 1)
					{
						Parameters_st.tDec ++;
						TimePrint(Parameters_st.tDec);
					}
					else if(upFactor == 10)
					{
						Parameters_st.tDec += 60;
						TimePrint(Parameters_st.tDec);
					}
					else if(upFactor == 100)
					{
						Parameters_st.tDec += 300;
						TimePrint(Parameters_st.tDec);
					}
			}
			else if(btn == BUTTON_OK) // apasat butonul 3 - ok
			{
				//Parameters_st.tRec = tRec;
				EEPROM_writeAnything(ADDR_TREC, Parameters_st.tDec);
				DisplayTimeMenu();
			}
			else if(btn == BUTTON_BACK) //apasat butonul 4 - back
			{
				Parameters_st.tDec = DEFAULT_TDEC;
				DisplayTimeMenu();
			}
		}break;
		
	}

}

