/*
 * Defines.h
 *
 * Created: 3/9/2017 12:14:14 AM
 *  Author: Cristina
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_

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

#define ADDR_TSET 0
#define ADDR_TINC 1
#define ADDR_TMEN 2
#define ADDR_REC 3
#define ADDR_KP 4
#define ADDR_KD 5
#define ADDR_KI 6



#endif /* DEFINES_H_ */



/*
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * V0 pin to ground
 * K pin to ground
 * A pin to 5V with a resistor (680 ohms)
 */
