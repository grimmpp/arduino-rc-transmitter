// global libs
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
// local libs
#include "PinAssign.cpp"
#include "Motor.cpp"
// shared btw projects
#include <NrfMessage.h>
#include <NrfMessage.cpp>

int delayTime = 10; //ms

RF24 radio(9, 10);                // define the object to control NRF24L01
byte addresses[5] = "00007";
int radioTimeout = 500; //ms

NrfMessage nrfMsg;

Motor motorRight = Motor(PinAssign::MotorADirPin, PinAssign::MotorApwmPin);
Motor motorLeft = Motor(PinAssign::MotorBDirPin, PinAssign::MotorBpwmPin);

void setup() {
	Serial.begin(115200);
	Serial.println("Start initializing 4WD_RC_Car_AddeptDriverBoard ...");

	Serial.println("Init NRF24 ...");
	radio.begin();                      // initialize RF24
	radio.setRetries(0, 15);            // set retries times
	radio.setPALevel(RF24_PA_LOW);      // set power
	radio.openReadingPipe(1, addresses);// open delivery channel
	radio.startListening();             // start monitoring

	Serial.println("Started");
}

int timeout = 0;
void receiveData(){

	if (radio.available()) {             // if receive the data
		radio.read(&nrfMsg, sizeof(nrfMsg));

		nrfMsg.print();

		timeout = 0;
		if (nrfMsg.initialized) {
			Serial.println("Received data");

			int speedRight = nrfMsg.poti_right_Y;
			int speedLeft = nrfMsg.poti_right_Y;

			int speed = nrfMsg.poti_right_Y;

			// if active
			int steeringVal = abs(nrfMsg.poti_right_X - 255/2 );
			if (steeringVal > 10) {
				int dSpeed = (int)( steeringVal * ( (double)map(abs(255/2 - speed), 0,255/2, 100, 200) / 100.0 ) );
				// forward
				if (speed > 255/2) {
					// go left
					if (nrfMsg.poti_right_X < 255/2) speedLeft = max(0, speed - dSpeed);
					// go right
					else  speedRight = max(0, speed - dSpeed);
				}
				// backwards
				else {
					// go left
					if (nrfMsg.poti_right_X < 255/2) speedLeft = min(255, speed + dSpeed);
					// go right
					else  speedRight = min(255, speed + dSpeed);
				}
			}

			Serial.print("SL: "); Serial.print(speedLeft); Serial.print("SR: "); Serial.print(speedRight); Serial.println();
			motorRight.setSpeed(speedRight);
			motorLeft.setSpeed(speedLeft);

		} else {
			Serial.print("Cannot read radio: "); Serial.println(radio.available(), DEC);
			//byte* b;
			//radio.read(b, radio.available());
			delay(10);
		}
	}
	else {
		// reset/switch off because of missing signal
		if (timeout >= radioTimeout) {

			// stop engines
			motorRight.setSpeed(255/2);
			motorLeft.setSpeed(255/2);
		}
		else {
			timeout += delayTime;
		}
	}
}

void loop() {
	receiveData();
	delay(delayTime);
}
