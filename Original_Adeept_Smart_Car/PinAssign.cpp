#include <Arduino.h>

#ifndef PinAssign_Def
#define PinAssign_Def

class PinAssign {
public:
	//Lights
	const static char RPin = A3;
	const static char GPin = A4;
	const static char BPin = A5;

	//Steering servo
	const static char SteeringServoPin = 2;              // define pin for signal line of the last servo

	const static char UltraServoPin = 3;

	const static int MotorADirPin = 7;    // define pin used to control rotational direction of motor A
	const static int MotorApwmPin = 6;    // define pin for PWM used to control rotational speed of motor A
	const static int MotorAsnsPin = 0;    // define pin for detecting current of motor A

	const static int MotorBDirPin = 4;    // define pin used to control rotational direction of motor B
	const static int MotorBpwmPin = 5;    // define pin for PWM used to control rotational speed of motor B
	const static int MotorBsnsPin = 1;    // define pin for detecting current of motor B
};

#endif
