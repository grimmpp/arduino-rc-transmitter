
#include <nRF24L01.h>
#include <RF24.h>

#include "./State.cpp"

#include "../RC_Library/NrfMessage.h"

static RF24* rf24;
static uint8_t profileIndex;

class Nrf24Controller {
private:

	static void sendToOriginalCar(State* state) {
		int data[9] =  {512, 512, 1, 0, 1, 1, 512, 512, 512};
		data[0] = state->poti_right_X * 4;
		data[1] = state->poti_left_Y * 4;
		data[2] = state->btn_bottom_left;
		if (state->mode == 'A') {
		  data[3] = 0;
		  data[4] = 1;
		} else {
		  data[3] = 1;
		  data[4] = 0;
		}
		data[5] = state->btn_bottom_right;
		data[6] = 512;
		data[7] = 512;
		data[8] = state->poti_right_Y * 4;

		if (rf24->write( data, sizeof(data) )) {
	//if (debug)
			Serial.println("NRF: Original Car Data was sent.");
		} else {
	//if (debug)
			Serial.println("NRF: Was not able to send Original Car Data!");
		}
	}

	static void sendToCar(State* state) {
		NrfMessage nrfMsg;
		nrfMsg.initialized = state->initialized;
		nrfMsg.btn_bottom_left = state->btn_bottom_left;
		nrfMsg.btn_bottom_right = state->btn_bottom_right;
		nrfMsg.btn_middle_left = state->btn_middle_left;
		nrfMsg.btn_middle_right = state->btn_middle_right;
		nrfMsg.btn_top_left = state->btn_top_left;
		nrfMsg.btn_top_right = state->btn_top_right;
		nrfMsg.poti_left_X = state->poti_left_X;
		nrfMsg.poti_left_Y = state->poti_left_Y;
		nrfMsg.poti_right_X = state->poti_right_X;
		nrfMsg.poti_right_Y = state->poti_right_Y;
		nrfMsg.mode = state->mode;

		if (rf24->write( &nrfMsg, sizeof(NrfMessage) )) {
			Serial.println("NRF: Standard Car Data was sent.");
		} else {
			Serial.println("NRF: Was not able to send Standard Car Data!");
		}
	}

	static void setAddress() {
		rf24->openWritingPipe(ReceiverProfiles[profileIndex].adr);
		rf24->setRetries(15, 15);
		rf24->setPALevel(RF24_PA_MIN);
		rf24->stopListening();
	}

public:

	static void initRF24(uint16_t _cepin, uint16_t _cspin) {
		rf24 = new RF24(_cepin, _cspin); // CE, CSN
		rf24->begin();

		profileIndex = 255;
	}

	static void sendData(State* state) {
		if (profileIndex != state->menuState->getReceiverProfile()) {
			profileIndex = state->menuState->getReceiverProfile();
			setAddress();
		}

		if (profileIndex == 0) { // Original Car
			sendToOriginalCar(state);
		}
		else if (profileIndex == 1) { // Standard Car
			sendToCar(state);
		}
	}
};


