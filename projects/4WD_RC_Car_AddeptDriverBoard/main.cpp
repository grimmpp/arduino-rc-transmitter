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

int calcSpeedLeft(int x, int y) {
	//center
	int _x = x - 255/2;
	int _y = y - 255/2;
	int result = 0;

	if (abs(_x)>10 || abs(_y)>10) {
		if (_y > 0) {
			if (_x > 0) result = max(_x,_y);
			else result = _y + _x/2;
		} 
		else {
			if (_x > 0) result = min(-1*_x,_y);
			else result = _y - _x/2;
		}
	} 
	result = result + 255/2;
	result = max(0,min(254, result));
	return result;
}

int calcSpeedRight(int x, int y) {
	//center
	int _x = x - 255/2;
	int _y = y - 255/2;
	int result = 0;

	if (abs(_x)>10 || abs(_y)>10) {
		if (_y > 0) {
			if (_x > 0) result = _y - _x/2;
			else result = max(-1*_x,_y);
		} 
		else {
			if (_x > 0) result = _y + _x/2;
			else result = min(_x,_y);
		}
	} 
	result = result + 255/2;
	result = max(0,min(254, result));
	return result;
}

int timeout = 0;
void receiveData(){

	if (radio.available()) {             // if receive the data
		radio.read(&nrfMsg, sizeof(nrfMsg));

		nrfMsg.print();

		timeout = 0;
		if (nrfMsg.initialized) {
			Serial.println("Received data");

			int speedLeft = calcSpeedLeft(nrfMsg.poti_right_X, nrfMsg.poti_right_Y);
			int speedRight = calcSpeedRight(nrfMsg.poti_right_X, nrfMsg.poti_right_Y);

			if (nrfMsg.btn_middle_right) {
				speedLeft = 254;
				speedRight = 0;
			} else if (nrfMsg.btn_middle_left) {
				speedLeft = 0;
				speedRight = 254;
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
