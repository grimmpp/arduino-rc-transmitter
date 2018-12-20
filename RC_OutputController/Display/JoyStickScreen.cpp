#include <TFT.h> // Hardware-specific library

#include "./IScreen.h"
#include "../State.cpp"

static const struct GreyBoxSizing {
public:
	uint8_t boxLength = 12;
	uint8_t shortEdge = boxLength/2;
	uint8_t longEdge = boxLength;
	uint8_t middleEdge = (shortEdge+longEdge)/2;
	uint8_t space = boxLength/6;
} gBS;

enum ProcessState : uint8_t {ShowInitialize, InitialzeDisplayed, HideInitialize, InitialzeHidden};

class JoyStickScreen : public IScreen {
private:
	ProcessState state = InitialzeHidden;

	void drawRedBorder(bool fill) {
		TFTscreen->stroke(0,0,120);
		if (fill) { TFTscreen->fill(0,0,0); }
		else TFTscreen->noFill();
		TFTscreen->rect(tB.tft_first_column, tB.tft_first_row, tB.tft_last_column-tB.tft_first_column, tB.tft_last_row-tB.tft_first_row);
	}

	void setButtonColorGreen(boolean btnVal) {
	  if (btnVal) this->TFTscreen->fill(0,80,0);
	  else this->TFTscreen->fill(0,0,0);
	}

	void setButtonColorBlue(boolean btnVal) {
	  if (btnVal) this->TFTscreen->fill(80,0,0);
	  else this->TFTscreen->fill(0,0,0);
	}

	void drawGreyMiddleBoxes() {
		// grey center boxes
		this->TFTscreen->stroke(80,80,80);
		this->TFTscreen->fill(80,80,80);
		this->TFTscreen->rect(tB.tft_quarter_column - gBS.boxLength/2, tB.tft_middle_row - gBS.boxLength/2, gBS.boxLength, gBS.boxLength);
		this->TFTscreen->rect(tB.tft_last_column-tB.tft_quarter_column - gBS.boxLength/2, tB.tft_middle_row - gBS.boxLength/2, gBS.boxLength, gBS.boxLength);
	}

	void clearTextArea() {
		TFTscreen->stroke(0,0,0);
		TFTscreen->fill(0,0,0);
		TFTscreen->rect(tB.tft_first_column+1, tB.tft_first_row+1, tB.tft_last_column-tB.tft_first_column-2, tB.tft_textHight);
		TFTscreen->noFill();
	}

	void showInitialize() {
		// show initialized
		this->clearTextArea();
		this->TFTscreen->stroke(255,255,255);
		this->TFTscreen->setTextSize(2);
		this->TFTscreen->text("Initialize", tB.tft_first_column+4, tB.tft_first_row+4);
	}

	ProcessState nextStatus(State *data) {
		switch(this->state) {
		case ShowInitialize:
			if (data->initialized) return HideInitialize;
			else return InitialzeDisplayed;
		case InitialzeDisplayed:
			if (data->initialized) return HideInitialize;
			else return InitialzeDisplayed;
		case InitialzeHidden:
			if (data->initialized) return InitialzeHidden;
			else return ShowInitialize;
		case HideInitialize:
			if (data->initialized) return InitialzeHidden;
			else return ShowInitialize;
		}
		return InitialzeDisplayed;
	}


public:
	JoyStickScreen(TFT *TFTscreen, State* oldState) : IScreen(TFTscreen, oldState) { }

	void showMode(bool init, State *data) {

		if (init) {
			this->TFTscreen->noFill();
			this->TFTscreen->stroke(80,80,80);
			this->TFTscreen->setTextSize(2);
			this->TFTscreen->text("Mode: ", tB.tft_middle_column+4, tB.tft_first_row+4);
		}

		if ( init || (data->initialized && this->oldState->mode != data->mode)) {
			// Remove old value
			char m[] = {this->oldState->mode, '\0'};
			this->TFTscreen->stroke(0,0,0);
			this->TFTscreen->setTextSize(2);
			this->TFTscreen->text(m, tB.tft_middle_column+62, tB.tft_first_row+4);

			// Show in green
			m[0] = data->mode;
			this->TFTscreen->stroke(0,80,0);
			this->TFTscreen->setTextSize(2);
			this->TFTscreen->text(m, tB.tft_middle_column+62, tB.tft_first_row+4);
		}
	}

	void showControlIt() {
		this->clearTextArea();
		this->TFTscreen->stroke(160,100,0);
		this->TFTscreen->setTextSize(1);
		this->TFTscreen->text("Control IT!", tB.tft_first_column+7, tB.tft_first_row+8);
	}

	void init() {
		if (oldState->initialized) this->state = ShowInitialize;
		else this->state = InitialzeHidden;
	}

	void refresh(State *values) {
		this->state = this->nextStatus(values);

		if (this->state == ShowInitialize || this->state == HideInitialize) {
			this->drawRedBorder(true /*fill*/);
			this->drawGreyMiddleBoxes();
		}

		if (this->state == ShowInitialize) {
			this->showInitialize();
		}

		if (this->state == HideInitialize) {
			this->showControlIt();
		}

		bool init = this->state == HideInitialize;
		this->showMode(init, values);

		init = this->state == ShowInitialize || this->state == HideInitialize;

		const unsigned char upperBorder = 109;
		const unsigned char lowerBorder = 146;

		//grey rect boxes
		this->TFTscreen->stroke(80,80,80);
		for(uint8_t i=1; i<=3; i++) {

			// top
			bool colourizeNew = values->poti_left_Y >= upperBorder + i* 36.5;
			bool colourizeOld = this->oldState->poti_left_Y >= upperBorder + i* 36.5;
			//Serial.print(i); Serial.print(" poti_left_Y: "); Serial.print(values.poti_left_Y); Serial.print(" "); Serial.println(colourizeNew);
			if (init || colourizeOld != colourizeNew) {
				setButtonColorBlue( colourizeNew );
				this->TFTscreen->rect(tB.tft_quarter_column - gBS.longEdge/2, tB.tft_middle_row - gBS.space*i - gBS.shortEdge*(i+1), gBS.longEdge, gBS.shortEdge);
			}
			colourizeNew = values->poti_right_Y >= upperBorder + i* 36.5;
			colourizeOld = this->oldState->poti_right_Y >= upperBorder + i* 36.5;
			if (init || colourizeOld != colourizeNew) {
				setButtonColorBlue( colourizeNew );
				this->TFTscreen->rect(tB.tft_last_column-tB.tft_quarter_column - gBS.longEdge/2, tB.tft_middle_row - gBS.space*i - gBS.shortEdge*(i+1), gBS.longEdge, gBS.shortEdge);
			}

			//bottom
			colourizeNew = values->poti_left_Y <= lowerBorder - i* 36.5;
			colourizeOld = this->oldState->poti_left_Y <= lowerBorder - i* 36.5;
			if (init || colourizeOld != colourizeNew) {
				setButtonColorBlue( colourizeNew );
				this->TFTscreen->rect(tB.tft_quarter_column - gBS.longEdge/2, tB.tft_middle_row + gBS.space*i + gBS.shortEdge*i, gBS.longEdge, gBS.shortEdge);
			}
			colourizeNew = values->poti_right_Y <= lowerBorder - i* 36.5;
			colourizeOld = this->oldState->poti_right_Y <= lowerBorder - i* 36.5;
			if (init || colourizeOld != colourizeNew) {
				setButtonColorBlue( colourizeNew );
				this->TFTscreen->rect(tB.tft_last_column-tB.tft_quarter_column - gBS.longEdge/2, tB.tft_middle_row + gBS.space*i + gBS.shortEdge*i, gBS.longEdge, gBS.shortEdge);
			}

			//left
			colourizeNew = values->poti_left_X <= lowerBorder - i* 36.5;
			colourizeOld = this->oldState->poti_left_X <= lowerBorder - i* 36.5;
			if (init || colourizeOld != colourizeNew) {
				setButtonColorBlue( colourizeNew );
				this->TFTscreen->rect(tB.tft_quarter_column - gBS.space*i - gBS.shortEdge*(i+1), tB.tft_middle_row - gBS.longEdge/2, gBS.shortEdge, gBS.longEdge);
			}
			colourizeNew = values->poti_right_X <= lowerBorder - i* 36.5;
			colourizeOld = this->oldState->poti_right_X <= lowerBorder - i* 36.5;
			if (init || colourizeOld != colourizeNew) {
				setButtonColorBlue( colourizeNew );
				this->TFTscreen->rect(tB.tft_last_column-tB.tft_quarter_column - gBS.space*i - gBS.shortEdge*(i+1), tB.tft_middle_row - gBS.longEdge/2, gBS.shortEdge, gBS.longEdge);
			}
			//right
			colourizeNew = values->poti_left_X >= upperBorder + i* 36.5;
			colourizeOld = this->oldState->poti_left_X >= upperBorder + i* 36.5;
			if (init || colourizeOld != colourizeNew) {
				setButtonColorBlue( colourizeNew );
				this->TFTscreen->rect(tB.tft_quarter_column + gBS.space*i + gBS.shortEdge*i, tB.tft_middle_row - gBS.longEdge/2, gBS.shortEdge, gBS.longEdge);
			}
			colourizeNew = values->poti_right_X >= upperBorder + i* 36.5;
			colourizeOld = this->oldState->poti_right_X >= upperBorder + i* 36.5;
			if (init || colourizeOld != colourizeNew) {
			  setButtonColorBlue( colourizeNew );
			  this->TFTscreen->rect(tB.tft_last_column-tB.tft_quarter_column + gBS.space*i + gBS.shortEdge*i, tB.tft_middle_row - gBS.longEdge/2, gBS.shortEdge, gBS.longEdge);
			}
		}

		//buttoms bottom
		char buttonLength = 6*gBS.shortEdge + 6*gBS.space + gBS.boxLength;
		if (init || this->oldState->btn_bottom_left != values->btn_bottom_left) {
			setButtonColorGreen(values->btn_bottom_left & !init);
			this->TFTscreen->rect(tB.tft_quarter_column - buttonLength/2, tB.tft_middle_row + buttonLength/2 + 2*gBS.space, buttonLength, gBS.middleEdge);
		}
		if (init || this->oldState->btn_bottom_right != values->btn_bottom_right) {
			setButtonColorGreen(values->btn_bottom_right & !init);
			this->TFTscreen->rect(tB.tft_last_column-tB.tft_quarter_column - buttonLength/2, tB.tft_middle_row + buttonLength/2 + 2*gBS.space, buttonLength, gBS.middleEdge);
		}

		//buttoms top
		if (init || this->oldState->btn_top_left != values->btn_top_left) {
			setButtonColorGreen(values->btn_top_left & !init);
			this->TFTscreen->rect(tB.tft_quarter_column - buttonLength/2, tB.tft_middle_row - buttonLength/2 - 2*gBS.space - gBS.middleEdge, buttonLength, gBS.middleEdge);
		}
		if (init || this->oldState->btn_top_right != values->btn_top_right) {
			setButtonColorGreen(values->btn_top_right & !init);
			this->TFTscreen->rect(tB.tft_last_column-tB.tft_quarter_column - buttonLength/2, tB.tft_middle_row - buttonLength/2 - 2*gBS.space - gBS.middleEdge, buttonLength, gBS.middleEdge);
		}

		//buttons middle
		if (init || this->oldState->btn_middle_left != values->btn_middle_left) {
			setButtonColorGreen(values->btn_middle_left & !init);
			this->TFTscreen->rect(tB.tft_quarter_column + buttonLength/2 + 2*gBS.space, tB.tft_middle_row - buttonLength/2, gBS.middleEdge, buttonLength);
		}
		if (init || this->oldState->btn_middle_right != values->btn_middle_right) {
			setButtonColorGreen(values->btn_middle_right & !init);
			this->TFTscreen->rect(tB.tft_last_column-tB.tft_quarter_column - buttonLength/2 - 2*gBS.space - gBS.middleEdge, tB.tft_middle_row - buttonLength/2, gBS.middleEdge, buttonLength);
		}

//		this->oldState->setData(values);
	}

	void refresh() {
		this->refresh(this->oldState);
	}
};
