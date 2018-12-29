// global libs
#include <TFT.h> // Hardware-specific library
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial.h>
// local libs
#include "./RgbLed.cpp"
#include "./Display/Display.cpp"
#include "./StateController.cpp"
#include "./State.cpp"
#include "./Nrf24Controller.cpp"
// shared btw projects
#include <ButtonValues.h>
#include <InterConnector.h>
#include <InterConnector.cpp>


// LED = 5V
// SCK 13    +  1K Resistor for tft
// SDA = MOSI 11    +  1K Resistor for tft
#define tft_cs   10
#define tft_dc   9    // A0    + 1K Resistor for tft
#define tft_rst  4

// SCK 13
// MISO 12
// MOSI 11
#define nrf_cs 8
#define nrf_ce 7

#define pinLedB 6
#define pinLedR 5
#define pinLedG 3

unsigned short luminosityLed = 20;

RgbLed *rgbLed;
InterConnector *interConnector;
Display *display;

//ButtonValues values;
State *currentState;

StateController *stateController;

//bool debug = false;

void setup() {
	Serial.begin(115200);
	Serial.println("Start to initialize RC_OutputController ...");

	display = new Display(tft_cs, tft_dc, tft_rst);

	currentState = new State();
	currentState->init();
	StateController::loadFromEEPROM(currentState);
	//currentState->menuState->opened = true;

	//set led to blue
	rgbLed = new RgbLed(pinLedR, pinLedG, pinLedB);
	rgbLed->setLuminosity(luminosityLed);
	rgbLed->setBlue();

	interConnector = new InterConnector(A5, A4);  // RX, TX

	Nrf24Controller::initRF24(nrf_ce, nrf_cs);

	Serial.println("Started");
}

uint8_t iteration=0;

void loop() {
	ButtonValues btnData;
	if (interConnector->receiveButtonValues(&btnData)) {
//if (debug) Serial.println("OutputController: updateState");
		StateController::updateState(&btnData, currentState);

//Serial.print("Menu open: "); Serial.println(currentState->menuState->opened);
//Serial.print("Menu Index: "); Serial.println((unsigned char)currentState->menuState->getSelectedMenuIndex());

		bool sendData = StateController::isNrfSenderActive(currentState);

//if (debug) Serial.println("OutputController: set led");
		// Set rgb led
		if (sendData) {
			if (!rgbLed->isBlueSet()) rgbLed->setBlue();
		} else {
			if (!rgbLed->isGreenSet()) rgbLed->setGreen();
		}

//if (debug) Serial.println("OutputController: refresh display");
		// refresh display
		display->refresh(currentState);

//if (debug) Serial.println("OutputController: send data");
		// send data via nrf
		if (sendData && iteration == 0) {
			Nrf24Controller::sendData(currentState);
		}
		delay(10);
	}

	iteration++;
	if (iteration>5) iteration=0;
}


