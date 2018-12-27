/*
 * Ultra.cpp
 *
 *  Created on: May 14, 2018
 *      Author: Philipp
 */
#include <Servo.h>

#include <PinAssign.cpp>

#ifndef Ultra_Def
#define Ultra_Def

class Ultra {
private:
	Servo servo;

public:
	void init() {
		servo.attach(PinAssign::UltraServoPin);
		servo.write(105);
		delay(200);
		servo.detach();
	}
};

#endif
