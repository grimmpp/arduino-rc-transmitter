#include <Arduino.h>

#include "./settings/EepromStorage.cpp"

#ifndef MenuState_Def
#define MenuState_Def

enum MenuIndex : uint8_t {MenuIndex_TFT_Brightness=0, MenuIndex_ReceiverProfile=1};
const static uint8_t MenuIndex_ElementCount = 2;

class MenuState {
private:
	MenuIndex currentIndex = MenuIndex_ReceiverProfile;

	static const uint8_t countOfValuesLines = 3;
	static const uint8_t lineLength = 24;

public:
	bool opened = false;
	uint8_t values[2] = {0,0};

	char* menuEntryName;
	char** menuEntryValues;

	MenuState() {
		menuEntryName = new char[lineLength];
		menuEntryName[0] = '\0';

		// initialized all arrays and keep the memory statically
		menuEntryValues = new char*[countOfValuesLines];
		for (uint8_t i=0; i<countOfValuesLines; i++){
			menuEntryValues[i] = new char[lineLength];
			menuEntryValues[i][0] = 'A';
			menuEntryValues[i][1] = '\0';
		}
	}

	~MenuState() { }

	void setMenuEntryName(char* entryName) {
		for (char i=0; entryName[i] != '\0'; i++) {
			menuEntryName[i] = entryName[i];
		}
		menuEntryName[strlen(entryName)] = '\0';
	}

	void setMenuEntryValues(char** entryValues) {
//Serial.print("MenuState: "); Serial.print("setMenuEntryValues: iter "); Serial.println(iter);
		for (char i=0; i<maxValueLines(); i++) {
			for (char j=0; j<lineLength; j++) {
				menuEntryValues[i][j] = entryValues[i][j];
			}
		}
	}



	MenuIndex getSelectedMenuIndex() {
		return currentIndex;
	}

	void selectIndex(MenuIndex index) {
		currentIndex = index;
	}

	char* getMenuEntryName() {
		return menuEntryName;
	}

	void setData(MenuState* menuState) {
		opened = menuState->opened;
		currentIndex = menuState->getSelectedMenuIndex();
		for (uint8_t i=0; i<maxMenuEntryCount(); i++) {
			values[i] = menuState->values[i];
		}
		setMenuEntryName( menuState->getMenuEntryName() );
		setMenuEntryValues( menuState->menuEntryValues );
	}

	void setData(EepromValues *data) {
		opened = data->isMenuOpen;
		currentIndex = data->menuIndex;
		for (uint8_t i=0; i<maxMenuEntryCount(); i++) { values[i] = data->values[i]; }
	}

	uint8_t getReceiverProfile() {
		return values[MenuIndex_ReceiverProfile];
	}

	uint8_t getTftBrightness() {
		return values[MenuIndex_TFT_Brightness];
	}

	void getEepromValues(EepromValues *data) {
		data->isMenuOpen = opened;
		data->menuIndex = currentIndex;
		for (uint8_t i=0; i<maxMenuEntryCount(); i++) { data->values[i] = this->values[i]; }
	}

	uint8_t maxMenuEntryCount() {
		return MenuIndex_ElementCount;
	}

	uint8_t maxValueLines() {
		return countOfValuesLines;
	}
};

#endif MenuState_Def
