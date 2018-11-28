#include <Arduino.h>

#include "../RC_Library/ButtonValues.h"
#include "./State.cpp"

class StateController {
private:
	const static bool debug = true;

public:
	StateController() {	}

	static void updateState(ButtonValues* btnData, State* state) {
		bool saveSettings = false;

		state->setButtonValues(btnData);

		bool changeScreens = isMenuCombiPressed(btnData);
		saveSettings |= changeScreens;
		state->menuState->opened = state->menuState->opened ^ changeScreens;
		if (changeScreens) delay(250);

//if (changeScreens) Serial.print("menu opened: "); Serial.println(state->menuState->opened);

		// set mode on control screen
		if (!state->menuState->opened) {
			char nextMode = getNextMode(state);
			saveSettings |= state->mode != nextMode;

			state->mode = nextMode;
		}

		// in menu
		else {
			if (isOnlyRightUpperButtonPressed(btnData)) {
				MenuStateController::increaseIndex(state->menuState);
				saveSettings = true;
			}
			else if (isOnlyLeftUpperButtonPressed(btnData)) {
				MenuStateController::decreaseIndex(state->menuState);
				saveSettings = true;
			}
			else if (isOnlyRightMiddleButtonPressed(btnData)) {
				MenuStateController::increaseValues(state->menuState);
				saveSettings = true;
			}
			else if (isOnlyLeftMiddleButtonPressed(btnData)) {
				MenuStateController::decreaseValues(state->menuState);
				saveSettings = true;
			}

			if (saveSettings) {
//if (debug) Serial.println("StateController: updateMenuState");
				MenuStateController::updateState(state->menuState);
//if (debug) Serial.println("StateController: updateMenuEntryValues");
				MenuStateController::writeMenuEntry(state->menuState);
				MenuStateController::writeMenuEntryValues(state->menuState);
			}

		}
		if (saveSettings) {
//if (debug) Serial.println("StateController: saveToEeprom");
			saveToEEPROM(state);
//if (debug) Serial.println("StateController: savedToEeprom");
		}

		if (isOnlyRightUpperButtonPressed(btnData)) {
			Serial.print("Free Mem: "); Serial.println(StateController::freeRam());
		}
	}

	static void loadFromEEPROM(State* state) {
		EepromValues *values = new EepromValues();
		EepromStorage::getEepromValues(values);
		state->setData(values);

		MenuStateController::writeMenuEntry(state->menuState);
		MenuStateController::writeMenuEntryValues(state->menuState);
		delete values;
	}

	static void saveToEEPROM(State* state) {
		EepromValues *values = new EepromValues();
		state->getEepromValues(values);
		EepromStorage::setEepromValues(*values);
		delete values;
	}

	static bool isMenuCombiPressed(ButtonValues *btnData) {
		return btnData->btn_bottom_left &&
				btnData->btn_bottom_right &&
				btnData->btn_top_left &&
				btnData->btn_top_right &&
				!btnData->btn_middle_left &&
				!btnData->btn_middle_right;
	}

	static bool isOnlyRightUpperButtonPressed(ButtonValues *btnData) {
		return !btnData->btn_bottom_left &&
				!btnData->btn_bottom_right &&
				!btnData->btn_top_left &&
				btnData->btn_top_right &&
				!btnData->btn_middle_left &&
				!btnData->btn_middle_right;
	}

	static bool isOnlyLeftUpperButtonPressed(ButtonValues *btnData) {
		return !btnData->btn_bottom_left &&
				!btnData->btn_bottom_right &&
				btnData->btn_top_left &&
				!btnData->btn_top_right &&
				!btnData->btn_middle_left &&
				!btnData->btn_middle_right;
	}

	static bool isJoyStickRightIsUp(ButtonValues *btnData) {
		return btnData->poti_right_X < (255/2 + 15) &&
				btnData->poti_right_X > (255/2 - 15) &&
				btnData->poti_right_Y > 200;
	}

	static bool isJoyStickRightICentered(ButtonValues *btnData) {
		return btnData->poti_right_X < (255/2 + 15) &&
				btnData->poti_right_X > (255/2 - 15) &&
				btnData->poti_right_Y < (255/2 + 15) &&
				btnData->poti_right_Y > (255/2 - 15);
	}

	static bool isJoyStickRightIsDown(ButtonValues *btnData) {
		return btnData->poti_right_X < (255/2 + 15) &&
				btnData->poti_right_X > (255/2 - 15) &&
				btnData->poti_right_Y < 55;
	}

	static bool isNoButtonPressed(ButtonValues *btnData) {
		return !btnData->btn_bottom_left &&
				!btnData->btn_bottom_right &&
				!btnData->btn_top_left &&
				!btnData->btn_top_right &&
				!btnData->btn_middle_left &&
				!btnData->btn_middle_right;
	}

	static bool isOnlyRightMiddleButtonPressed(ButtonValues* btnData) {
		return !btnData->btn_bottom_left &&
				!btnData->btn_bottom_right &&
				!btnData->btn_top_left &&
				!btnData->btn_top_right &&
				!btnData->btn_middle_left &&
				btnData->btn_middle_right;
	}

	static bool isOnlyLeftMiddleButtonPressed(ButtonValues* btnData) {
			return !btnData->btn_bottom_left &&
					!btnData->btn_bottom_right &&
					!btnData->btn_top_left &&
					!btnData->btn_top_right &&
					btnData->btn_middle_left &&
					!btnData->btn_middle_right;
		}

	static char getNextMode(State* state) {
		char mode = state->mode;
		if (isOnlyLeftMiddleButtonPressed(state) && !state->modeBtnPressed) {
			if ( mode > 'A' ) {
				state->modeBtnPressed = true;
				mode--;
			}
		} else if (isOnlyRightMiddleButtonPressed(state) && !state->modeBtnPressed) {
			if (mode < 'Z') {
				state->modeBtnPressed = true;
				mode++;
			}
		} else if (!state->btn_middle_left && !state->btn_middle_right) {
			state->modeBtnPressed = false;
		}
		return mode;
	}

	static bool isNrfSenderActive(State* state) {
		return state->initialized && !state->menuState->opened;
	}


	static int freeRam ()
	{
		extern int __heap_start, *__brkval;
		int v;
		return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
	}
};
