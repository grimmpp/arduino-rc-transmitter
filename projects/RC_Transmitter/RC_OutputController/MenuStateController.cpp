#include "./MenuState.cpp"
#include "./settings/EepromStorage.cpp"
#include "./settings/ReceiverProfile.cpp"

#ifndef MenuStateController_Def
#define MenuStateController_Def

static const struct {
	char* TftBrightness;
	char* ReceiverProfile;
} MenuEntryNames = { "TFT Brightness", "Receiver Profile"};

class MenuStateController {
private:

	static void strCpyToPos(char* src, char* dest, uint8_t pos = 0) {
		uint8_t len = strlen(src);
		for (uint8_t i=0; i<len; i++) {
			dest[pos + i] = src[i];
		}
		dest[pos + len] = '\0';
	}

	static void setEmptyValues(MenuState* state, uint8_t startRow = 0) {
		for (char i=startRow; i<state->maxValueLines(); i++) {
			state->menuEntryValues[i][0] = '\0';
		}
	}

public:

	MenuStateController() { }

	static void updateState(MenuState* state) {
		if (state->opened) {
			writeMenuEntry(state);
		}
	}

	static void increaseIndex(MenuState* state) {
		if (hasNext(state)) {
			state->selectIndex( static_cast<MenuIndex>(static_cast<unsigned char>(state->getSelectedMenuIndex()) + 1) );
		}
	}

	static void decreaseIndex(MenuState* state) {
		if (hasPrecessor(state->getSelectedMenuIndex())) {
			state->selectIndex( static_cast<MenuIndex>(static_cast<unsigned char>(state->getSelectedMenuIndex()) - 1) );
		}
	}

	static bool hasNext(MenuState* state) {
		return state->getSelectedMenuIndex() < state->maxMenuEntryCount()-1;
	}

	static bool hasPrecessor(MenuIndex index) {
		return index > 0;
	}

	static void increaseValues(MenuState* state) {
		if (hasNextValue(state)) {
			state->values[state->getSelectedMenuIndex()]++;
		}
	}

	static void decreaseValues(MenuState* state) {
		if (hasPrecessorValue(state)) {
			state->values[state->getSelectedMenuIndex()]--;
		}
	}

	static bool hasPrecessorValue(MenuState* state) {
		char index = state->getSelectedMenuIndex();
		switch(index) {
			case MenuIndex_TFT_Brightness:
				return state->values[index] > 50;
			case MenuIndex_ReceiverProfile:
				return state->values[index] > 0;
		}
		return false;
	}

	static bool hasNextValue(MenuState* state) {
		char index = state->getSelectedMenuIndex();
		switch(index) {
			case MenuIndex_TFT_Brightness:
				return state->values[index] < 150;
			case MenuIndex_ReceiverProfile:
				return state->values[index] < PredefReceiverProfiles::maxProfiles()-1;
		}
		return false;
	}

	static void writeMenuEntry(MenuState* state) {
		MenuIndex index = state->getSelectedMenuIndex();

		switch(index) {
			case MenuIndex_TFT_Brightness:
				strCpyToPos(MenuEntryNames.TftBrightness, state->menuEntryName);
				break;

			case MenuIndex_ReceiverProfile:
				strCpyToPos(MenuEntryNames.ReceiverProfile, state->menuEntryName);
				break;

			default:
				break;
		}
//Serial.print("MenuStateController: writeMenuEntry: "); Serial.println(state->menuEntryName);
	}

	static void insertIntAsChar(char* text, uint8_t num, uint8_t offSet=0) {
//Serial.print("MenuStateController: Number: "); Serial.println(num);
		uint8_t length = num > 0 ? (int) log10 ((double) num) + 1 : 1;
//Serial.print("MenuStateController: Number length: "); Serial.println(length);
		for (uint8_t i=0; i<length; i++) {
			//text[i] = 'A';
			text[offSet + length-1-i] = (num / (int)pow(10, i)) % 10 + '0';
		}
		text[offSet + length] ='\0';
//Serial.print("MenuStateController: Number Text: "); Serial.println(*text);
	}

	static void writeMenuEntryValues(MenuState* state) {
		MenuIndex index = state->getSelectedMenuIndex();
		uint8_t value = state->values[index];

		if(index == MenuIndex_TFT_Brightness) {
			insertIntAsChar(state->menuEntryValues[0], value);
			setEmptyValues(state, 1);
		}
		else if(index == MenuIndex_ReceiverProfile) {

			ReceiverProfile rp = PredefReceiverProfiles::getProfile(value);

			char name[] = "Name:  ";
			strCpyToPos(name, state->menuEntryValues[0]);
			strCpyToPos(rp.name, state->menuEntryValues[0], strlen(name));

			char adr[] = "Address:  ";
			strCpyToPos(adr, state->menuEntryValues[1]);
			char adrVal[6];
			for (uint8_t i=0; i<5; i++) adrVal[i] = rp.adr[i];
			adrVal[5] = '\0';
			strCpyToPos(adrVal, state->menuEntryValues[1], strlen(adr));

			setEmptyValues(state, 2);
		}
	}


};

#endif MenuStateController_Def
