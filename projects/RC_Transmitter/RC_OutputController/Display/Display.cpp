#include <TFT.h> // Hardware-specific library
#include <SPI.h>

#include "./JoyStickScreen.cpp"
#include "./MenuScreen.cpp"
#include "../State.cpp"


enum ScreenType : unsigned char {ScreenType_NotSet, ScreenType_JoyStick, ScreenType_Menu};

class Display {
private:
	TFT *TFTscreen;
	IScreen *jSScreen;
	IScreen *mScreen;
	IScreen *activeScreen;
	State * oldState;
	ScreenType screenType = ScreenType_NotSet;

public:

	Display::Display(uint8_t CS, uint8_t RS, uint8_t RST) {
		TFTscreen = new TFT(CS, RS /*DC*/, RST);
		TFTscreen->begin();
		TFTscreen->background(0, 0, 0);

		this->oldState = new State();
		this->jSScreen = new JoyStickScreen(TFTscreen, this->oldState);
		this->mScreen = new MenuScreen(TFTscreen, this->oldState);
		this->activeScreen = this->jSScreen;
	}

	void refresh(State *data) {

		if (screenType == ScreenType_NotSet) {
			if (data->menuState->opened) setScreen(ScreenType_Menu);
			else setScreen(ScreenType_JoyStick);
		} else if (!this->oldState->menuState->opened && data->menuState->opened) {
			setScreen(ScreenType_Menu);
		} else if (this->oldState->menuState->opened && !data->menuState->opened) {
			setScreen(ScreenType_JoyStick);
		}

		if (this->oldState->menuState->getTftBrightness() != data->menuState->getTftBrightness()) {
			Color_Brightness = data->menuState->getTftBrightness();
			this->activeScreen->init();
		}

		this->activeScreen->refresh(data);

		this->oldState->setData(data);
	}

	void setScreen(ScreenType st) {
		this->screenType = st;

		switch(this->screenType) {
		case ScreenType_JoyStick:
			this->activeScreen = this->jSScreen;
			break;
		case ScreenType_Menu:
			this->activeScreen = this->mScreen;
			break;
		default:
			break;
		}

		this->activeScreen->init();
	}
};
