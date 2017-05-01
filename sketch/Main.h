/*
 * Defines.h
 *
 * Created: 3/9/2017 12:14:14 AM
 *  Author: Cristina
 */ 


#ifndef DEFINES_H_
#define DEFINES_H_


#define ADDR_STATUS 0
#define ADDR_KD sizeof(byte)
#define ADDR_KI sizeof(byte) + sizeof(float)
#define ADDR_KP sizeof(byte) + 2*sizeof(float)
#define ADDR_TINC sizeof(byte) + 3*sizeof(float)
#define ADDR_TMEN sizeof(byte) + 3*sizeof(float) + sizeof(int)
#define ADDR_TREC sizeof(byte) + 3*sizeof(float) + 2*sizeof(int)
#define ADDR_TSET sizeof(byte) + 3*sizeof(float) + 3*sizeof(int)

#define DEFAULT_STATUS 0
#define DEFAULT_KD 3.0f
#define DEFAULT_KI 2.0f
#define DEFAULT_KP 1.2f
#define DEFAULT_TINC 300
#define DEFAULT_TMEN 300
#define DEFAULT_TDEC 300
#define DEFAULT_TSET 30

#define BUTTON_PRESSED 0
#define BUTTON_RELEASED 1

#define FACTOR10_THRESH 30
#define FACTOR100_THRESH 60

#define BULB_PIN_PWM 6 //pentru ca daca il setez pe 9 sau 10 va folosi timerul 1
#define TEMP_SENSOR_READ_PIN A5
#define BUTTON_READ_PIN A0

typedef struct Parameters_t
{
	    byte status;  //0x00 -> Waiting
					//0x01 -> start
					//0x02 -> incalzire
					//0x03 -> mentinere
					//0x04 -> racire
					//0x05 -> proces finalizat
		float kd;
		float ki;
		float kp;
		int tInc; //timpul de incalzire - in secunde
		int tMen; //timpul de mentinere - in secunde
		int tDec; //timpul de descrestere atemp - in secunde
		int TSet; //temp setata de mentinere - in grade celsius
};



extern struct Parameters_t Parameters_st;
extern int upCounter;
extern int downCounter;
extern int backCounter;
extern int upFactor;
extern int downFactor;
void PWMGenerator(int);
extern float current_temp;
extern int timeRemaining;

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
