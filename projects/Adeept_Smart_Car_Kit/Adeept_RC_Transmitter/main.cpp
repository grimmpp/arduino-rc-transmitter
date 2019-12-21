
#include <SPI.h>
#include "RF24.h"
#include "PinAssign.cpp"
#include <NrfMessage.h>

RF24* rf24;                   // define the object to control NRF24L01
byte addresses[5] = "00007";  // define communication address which should correspond to remote control

NrfMessage nrfMsg;

static char getNextMode(char, int, int);

void setup() {
  Serial.begin(115200);
  Serial.println("Initialize Adeept_RC_Transmitter ...");

  rf24 = new RF24(9, 10);
  rf24->begin();                      // initialize RF24
  rf24->setRetries(15, 15);            // set retries times
  rf24->setPALevel(RF24_PA_MIN);      // set power
  rf24->openWritingPipe(addresses);   // open delivery channel
  rf24->stopListening();              // stop monitoring

  pinMode(PinAssign::led1Pin, OUTPUT);   // set led1Pin to output mode
  pinMode(PinAssign::led2Pin, OUTPUT);   // set led2Pin to output mode
  pinMode(PinAssign::led3Pin, OUTPUT);   // set led3Pin to output mode
  pinMode(PinAssign::APin, INPUT_PULLUP);   // set APin to output mode
  pinMode(PinAssign::BPin, INPUT_PULLUP);   // set BPin to output mode
  pinMode(PinAssign::CPin, INPUT_PULLUP);   // set CPin to output mode
  pinMode(PinAssign::DPin, INPUT_PULLUP);   // set DPin to output mode  
  
  nrfMsg.mode = 'A';
  nrfMsg.initialized = true;
  
  Serial.println("Started");
}

void loop() {
  // put the values of rocker, switch and potentiometer into the array
  nrfMsg.poti_right_X = 254 - max(0, min(254, analogRead(PinAssign::MotorPin)/4));
  nrfMsg.poti_right_Y = max(0, min(254, analogRead(PinAssign::ServoPin)/4));
  Serial.print("poti right X: ");
  Serial.print(nrfMsg.poti_right_X);
  Serial.print(", Y: ");
  Serial.println(nrfMsg.poti_right_Y);

  nrfMsg.poti_left_X = 254 - max(0, min(254, analogRead(PinAssign::ultrasonicPin)/4));
  nrfMsg.poti_left_Y = max(0, min(254, analogRead(1)/4));;

  nrfMsg.btn_bottom_right = 0;
  nrfMsg.btn_bottom_left = 0;
  nrfMsg.btn_top_left = 1- digitalRead(PinAssign::APin);
  nrfMsg.btn_middle_left = 1- digitalRead(PinAssign::BPin);
  nrfMsg.btn_middle_right = 1- digitalRead(PinAssign::CPin);
  nrfMsg.btn_top_right = 1- digitalRead(PinAssign::DPin);
  nrfMsg.mode = getNextMode(nrfMsg.mode, nrfMsg.btn_middle_left, nrfMsg.btn_middle_right);

  nrfMsg.print();
  
  // blue potis are ignored
  //data[6] = analogRead(PinAssign::pot5Pin);
  //data[7] = analogRead(PinAssign::pot6Pin);

  // send data. If the sending succeeds, open signal LED
  if (rf24->write( &nrfMsg, sizeof(NrfMessage) )) {
			//Serial.println("NRF: Data was sent.");
      digitalWrite(PinAssign::led1Pin,HIGH);
  } else {
    Serial.println("NRF: Was not able to send data!");
  }
  // delay for a period of time, then turn off the signal LED for next sending
  delay(50);
  digitalWrite(PinAssign::led1Pin,LOW);

  if(nrfMsg.mode == 'A'){
    digitalWrite(PinAssign::led2Pin,HIGH);
    digitalWrite(PinAssign::led3Pin,LOW);
  }
  else if(nrfMsg.mode == 'B'){
      digitalWrite(PinAssign::led2Pin,LOW);
      digitalWrite(PinAssign::led3Pin,HIGH);
  } 
  else {
    digitalWrite(PinAssign::led2Pin,LOW);
    digitalWrite(PinAssign::led3Pin,LOW);
  }

  delay(50);
}

static char getNextMode(char currentMode, int pin1Value, int pin2Value) {
		char mode = currentMode;
		if (pin1Value && !pin2Value) {
			if ( mode > 'A' ) {
				mode--;
        Serial.print("New Mode: ");
        Serial.println(mode);
			}
		} else if (!pin1Value && pin2Value) {
			if (mode < 'Z') {
				mode++;
        Serial.print("New Mode: ");
        Serial.println(mode);
			}
		}
		return mode;
	}