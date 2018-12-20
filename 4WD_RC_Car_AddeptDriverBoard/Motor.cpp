/*
 * Motor.cpp
 *
 *  Created on: May 13, 2018
 *      Author: Philipp
 */

#include <Arduino.h>
#include "./PinAssign.cpp"

#ifndef Motor_Def
#define Motor_Def

class Motor {
private:
	int dirPin;
	int pwmPin;

public:

	Motor(int dirPin, int pwmPin) {
		this->dirPin = dirPin;
		this->pwmPin = pwmPin;

		pinMode(dirPin, OUTPUT);   // set dirPin to output mode
		pinMode(pwmPin, OUTPUT);   // set pwmPin to output mode
	}

	void setSpeed(unsigned char potyValue) {
		bool motorDirection = potyValue >= 225/2 - 5;

		//int motorSpeed = min(255, max(0, abs(potyValue - 255/2))) * 2;
		int motorSpeed = abs(potyValue - 255/2) * 2;

		if (motorSpeed < 5) motorSpeed = 0;

		//set direction
		digitalWrite(dirPin, motorDirection);

		//set speed
		analogWrite(pwmPin, motorSpeed);

	}
};

#endif

