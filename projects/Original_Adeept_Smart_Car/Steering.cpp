
#include <Arduino.h>

#include "PinAssign.cpp"
#include <Servo.h>

#ifndef Steering_Def
#define Steering_Def

class Steering {
private:
	Servo dirServo;                  // define servo to control turning of smart car
	unsigned char lastDir = 0;
	unsigned char writeCount = 0;

	const static unsigned char offset = 50;
	const static unsigned char adjustment = 5;

public:

	void init() {
		setServo(255/2);
		delay(50);
		dirServo.detach();
	}

	void setServo(unsigned char potyValue) {
		unsigned char currentPotyValue = max(0, min(255, potyValue));

		int currentDir = 180 - map(currentPotyValue, 0, 255, offset, 180-offset);

		currentDir += adjustment;

//Serial.print("Steering direction: "); Serial.println(currentDir);
		if (abs(currentPotyValue - 255/2 ) > 10 || abs(currentDir - lastDir) > 4) {
			writeCount = 0;
		}

		if (writeCount < 3) {
			dirServo.attach(PinAssign::SteeringServoPin);
			dirServo.write(currentDir);

			lastDir = currentDir;
			writeCount++;
		}
		else {
			dirServo.detach();
		}
	}

};

#endif
