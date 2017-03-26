/*
 * Menu.h
 *
 * Created: 3/18/2017 7:50:54 PM
 *  Author: Cristina
 */ 




#ifndef MENU_H_
#define MENU_H_



#define MENU_MAIN 0
#define MENU_TEMP 1
#define MENU_TIME 2
#define MENU_PARAM 3

//submeniuri
#define MENU_KD 30
#define MENU_KI 31
#define MENU_KP 32

#define MENU_TINC 20
#define MENU_TMEN 21
#define MENU_TDEC 22


#define BUTTON_DOWN 1
#define BUTTON_UP 2
#define BUTTON_OK 3
#define BUTTON_BACK 4
#define NO_BUTTON_PRESSED 0

#define TEMP_LOWER_LIMIT 20 //ca sa nu permiti ca sa ceri temperatura prea mica pt Tset

//PROTOTYPES
void MenuHandler(int);
void MainMenu(int);
void TempMenu(int);
void TimeMenu(int);
void ParamMenu(int);
void ParamSubmenu(int);
void TimeSubmenu(int);


//Global variables
extern int current_menu;



#endif /* MENU_H_ */