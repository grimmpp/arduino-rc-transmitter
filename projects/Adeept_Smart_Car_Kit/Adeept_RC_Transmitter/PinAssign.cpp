#include <Arduino.h>

#ifndef PinAssign_Def
#define PinAssign_Def

class PinAssign {
public:
	// LEDs
	const static int led1Pin = 6;        // define pin for LED1 which is close to NRF24L01 and used to indicate the state of NRF24L01
	const static int led2Pin = 7;        // define pin for LED2 which is the mode is displayed in the car remote control mode  
	const static int led3Pin = 8;        // define pin for LED3 which is the mode is displayed in the car auto mode

	// Buttons
	const static int APin = 2;           // define pin for D2
	const static int BPin = 3;           // define pin for D3
	const static int CPin = 4;           // define pin for D4
	const static int DPin = 5;           // define pin for D5

	//Steering servo
	const static int MotorPin = 2;       // define pin for direction X of joystick U2

	const static int ServoPin = 3;       // define pin for direction Y of joystick U1
	const static int ultrasonicPin = 0;  // define pin for direction x of joystick U2

	// blue potis are not used.
	const static int pot6Pin = 5;        // define R6
	const static int pot5Pin = 4;        // define R1
	
};

#endif
