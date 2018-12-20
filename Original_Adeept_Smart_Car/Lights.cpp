/*
 * Lights.cpp
 *
 *  Created on: May 12, 2018
 *      Author: Philipp
 */

#include <Arduino.h>
#include "./PinAssign.cpp"

#ifndef Lights_Def
#define Lights_Def

class Lights {
public:

	void init() {
		pinMode(PinAssign::RPin, OUTPUT);   // set RPin to output mode
		pinMode(PinAssign::GPin, OUTPUT);   // set GPin to output mode
		pinMode(PinAssign::BPin, OUTPUT);   // set BPin to output mode

		switchOff();
	}

	void setWhite() {
		digitalWrite(PinAssign::RPin, LOW);
		digitalWrite(PinAssign::GPin, LOW);
		digitalWrite(PinAssign::BPin, LOW);
	}

	void setBlue() {
		digitalWrite(PinAssign::RPin, HIGH);
		digitalWrite(PinAssign::GPin, HIGH);
		digitalWrite(PinAssign::BPin, LOW);
	}

	void setGreen() {
		digitalWrite(PinAssign::RPin, HIGH);
		digitalWrite(PinAssign::GPin, LOW);
		digitalWrite(PinAssign::BPin, HIGH);
	}

	void setRed() {
		digitalWrite(PinAssign::RPin, LOW);
		digitalWrite(PinAssign::GPin, HIGH);
		digitalWrite(PinAssign::BPin, HIGH);
	}

	void switchOff() {
		digitalWrite(PinAssign::RPin, HIGH);
		digitalWrite(PinAssign::GPin, HIGH);
		digitalWrite(PinAssign::BPin, HIGH);
	}
};

#endif

