#ifndef ISCREEN_H_
#define ISCREEN_H_

#include "../State.cpp"

static const struct TftBorders {
	uint8_t tft_first_column = 0;
	uint8_t tft_first_row = 8;
	uint8_t tft_last_row = 125;
	uint8_t tft_last_column = 159;
	uint8_t tft_middle_column = (tft_last_column - tft_first_column) /2;
	uint8_t tft_middle_row = (tft_last_row - tft_first_row)/2 + tft_first_row +8;
	uint8_t tft_quarter_column = tft_last_column / 4 - 4;
	uint8_t tft_textHight = 22;
} tB;

class IScreen {
protected:
	TFT * TFTscreen;
	State * oldState;

public:

	IScreen(TFT *TFTscreen, State* oldState) {
		this->TFTscreen = TFTscreen;
		this->oldState = oldState;
	};

	virtual ~IScreen() { }
	virtual void init();
	virtual void refresh();
	virtual void refresh(State*);
};

#endif
