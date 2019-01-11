
#ifndef DisplayButtonData_Def
#define DisplayButtonData_Def

#include <ButtonValues.h>
#include "./MenuStateController.cpp"

class State : public ButtonValues {

public:
	char mode = 'A';
	bool modeBtnPressed = false;
	MenuState *menuState = new MenuState();

	State() : ButtonValues() { }

	void init() {
		this->poti_left_Y = 255/2;
		this->poti_left_X = 255/2;
		this->poti_right_Y = 255/2;
		this->poti_right_X = 255/2;
		this->initialized = false;
	}

	void setButtonValues(ButtonValues *data) {
		this->btn_bottom_left = data->btn_bottom_left;
		this->btn_bottom_right = data->btn_bottom_right;
		this->btn_middle_left = data->btn_middle_left;
		this->btn_middle_right = data->btn_middle_right;
		this->btn_top_left = data->btn_top_left;
		this->btn_top_right = data->btn_top_right;
		this->initialized = data->initialized;
		this->poti_left_X = data->poti_left_X;
		this->poti_left_Y = data->poti_left_Y;
		this->poti_right_X = data->poti_right_X;
		this->poti_right_Y = data->poti_right_Y;
	}

	void setData(State* state) {
		this->setButtonValues(state);
		this->mode = state->mode;
		this->menuState->setData(state->menuState);
	}

	void setData(EepromValues *values) {
		mode = values->mode;
		menuState->setData(values);
	}

	EepromValues* getEepromValues(EepromValues *values) {
		values->mode = mode;
		menuState->getEepromValues(values);
		return values;
	}
};

#endif
