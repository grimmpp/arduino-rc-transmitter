#include <Wire.h> 
#include <SoftwareSerial.h>
#include "../RC_Library/InterConnector.cpp"
#include "../RC_Library/ButtonValues.cpp"

// SoftwareSerial softSerial(A4, A5); // RX, TX
InterConnector *interConnector;

#define pinBtnBottomLeft 2  // bottom left 
#define pinBtnMiddleLeft 3  // middle left 
#define pinBtnBottomRight 4  // bottom right
#define pinBtnMiddleRight 5  // middle right 
#define pinBtnTopLeft 6  // top left
#define pinBtnTopRight 7  // top right

//#define maxPotiVal 735
int maxPotiVal = 0;
char initPhase = 0;

// Poti left
#define pinPotLX 6
#define pinPotLY 7
// Poti right
#define pinPotRY 2
#define pinPotRX 3

ButtonValues values;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pinBtnBottomLeft, INPUT);
  pinMode(pinBtnMiddleLeft, INPUT);
  pinMode(pinBtnBottomRight, INPUT);
  pinMode(pinBtnMiddleRight, INPUT);
  pinMode(pinBtnTopLeft, INPUT);
  pinMode(pinBtnTopRight, INPUT);

  interConnector = new InterConnector(A4, A5); // RX, TX
  initPhase = 0;
  
  Serial.println("Start");
}

int mapToRange(int val) {
  if (val > maxPotiVal) { maxPotiVal = val; }
  return min(255, max(0,map(val, 0, maxPotiVal, 0, 255)));
}

void loop() {

	if (interConnector->receiveCommand() == GET_NEXT_MESSAGE) {

		// put your main code here, to run repeatedly:
		values.btn_bottom_right = digitalRead(pinBtnBottomRight) == HIGH;
		values.btn_bottom_left = digitalRead(pinBtnBottomLeft) == HIGH;
		values.btn_middle_right = digitalRead(pinBtnMiddleRight) == HIGH;
		values.btn_middle_left = digitalRead(pinBtnMiddleLeft) == HIGH;
		values.btn_top_right = digitalRead(pinBtnTopRight) == HIGH;
		values.btn_top_left = digitalRead(pinBtnTopLeft) == HIGH;

		values.poti_left_Y = mapToRange( analogRead(pinPotLY) );
		values.poti_left_X = mapToRange( maxPotiVal - analogRead(pinPotLX) );
		values.poti_right_Y = mapToRange( analogRead(pinPotRY) );
		values.poti_right_X = mapToRange( maxPotiVal - analogRead(pinPotRX) );

		if (initPhase > 2) {
			values.initialized = true;
		}
		else {
			int potiLY = values.poti_left_Y; //analogRead(pinPotLY);
			int potiLX = values.poti_left_X; //analogRead(pinPotLX);
			if (initPhase == 0 && potiLY == 0) { initPhase++; }
			else if (initPhase == 1 && abs(potiLY - 2 * potiLX) < 30 && potiLY == 255) { initPhase++; }
			else if (initPhase == 2 && potiLY == 0) { initPhase++; }
			//else if (initPhase == 3 && abs(potiLY - potiLX) < 30 && abs(potiLY - 255/2) < 30) { initPhase++; }
			values.initialized = false;
		}

		interConnector->sendButtonValues(values);

		//*
		Serial.println();
		Serial.print("init: "); Serial.print((int)initPhase);
		values.print();
		//*/
	} else {
		delay(5);
	}
}




