// global libs
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <Servo.h>
// local libs
#include "PinAssign.cpp"
#include "Lights.cpp"
#include "Steering.cpp"
#include "Motor.cpp"
#include "Ultra.cpp"
// shared btw projects
#include <NrfMessage.h>
#include <NrfMessage.cpp>

int delayTime = 10; //ms

RF24 radio(9, 10);                // define the object to control NRF24L01
byte addresses[5] = "00007";
int radioTimeout = 500; //ms

NrfMessage nrfMsg;

Steering steering;
Lights lights;
Ultra ultra;
Motor motorRight = Motor(PinAssign::MotorADirPin, PinAssign::MotorApwmPin);
Motor motorLeft = Motor(PinAssign::MotorBDirPin, PinAssign::MotorBpwmPin);

void setup() {
	Serial.begin(115200);
	Serial.println("Start initializing Original_Adeept_Smart_Car ...");

	Serial.println("Init lights ...");
	lights.init();

	Serial.println("Init steering ...");
	steering.init();

	Serial.println("Init ultra ...");
	ultra.init();

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
			switch (nrfMsg.mode) {
			case 'A':
				lights.setBlue();
				break;
			case 'B':
				lights.setGreen();
				break;
			default:
				lights.setWhite();
			}

			if (nrfMsg.btn_top_right) {
				lights.setRed();
			}

			steering.setServo(nrfMsg.poti_right_X);
			motorRight.setSpeed(nrfMsg.poti_right_Y);
			motorLeft.setSpeed(nrfMsg.poti_right_Y);

			if (nrfMsg.btn_top_left) {
				motorRight.setSpeed(0);
				motorLeft.setSpeed(250);
			}
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

			lights.switchOff();
			steering.init();
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
