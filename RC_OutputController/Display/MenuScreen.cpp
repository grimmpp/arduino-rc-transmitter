#include <TFT.h> // Hardware-specific library

#include "./Color.cpp"
#include "../State.cpp"
#include "./IScreen.h"
#include "../MenuStateController.cpp"


class MenuScreen : public IScreen {
private:
	//uint16_t color = (Blue << 10) | (Green << 5) | Red;
//	static const uint16_t headlineColor =  (85 << 5);
	static const uint16_t arrowColor =  (80 << 5);
	static const uint16_t haedlineBGColor =  (20 << 10);
	static const uint16_t borderColor = (80 << 10);
	static const uint16_t blackColor = 0;
	static const uint16_t greyColor = (100 << 16) | (100 << 8) | 100;

	bool _init = false;

	static const uint8_t headlineHight = 15;
	static const uint8_t space = 4;
	static const uint8_t headlineY = space;
	static const uint8_t menuEntryY = headlineY + space + headlineHight + space;

	void showHeadline() {
		// show initialized
		this->clearTextArea();

		TFTscreen->stroke(haedlineBGColor);
		TFTscreen->fill(haedlineBGColor);
		TFTscreen->rect(tB.tft_first_column+1, tB.tft_first_row+1, tB.tft_last_column-tB.tft_first_column-2, tB.tft_first_row + headlineHight -2);

		TFTscreen->noFill();
		TFTscreen->stroke(Color::getGreen());
		TFTscreen->setTextSize(2);
		TFTscreen->text("Menu", tB.tft_first_column+space, tB.tft_first_row+space);
	}

	void drawMenuEntryArrows(MenuState *state) {
		uint16_t color = 0;

		// left arrow
		if (state->getSelectedMenuIndex() > 0) { color = arrowColor; }
		else { color = blackColor; }

		char offSetX = -2*space - space/2;
		char offSetY = -1;
		TFTscreen->fillTriangle(
				tB.tft_last_column-space + offSetX, tB.tft_first_row + menuEntryY + offSetY,
				tB.tft_last_column-2*space + offSetX, tB.tft_first_row + menuEntryY + space + offSetY,
				tB.tft_last_column-space + offSetX, tB.tft_first_row + menuEntryY + 2*space + offSetY,
				color);

		//right arrow
		if (state->getSelectedMenuIndex() < state->maxMenuEntryCount()-1) { color = arrowColor; }
		else { color = blackColor; }

		offSetX = -space - space/2;
		TFTscreen->fillTriangle(
				tB.tft_last_column-space + offSetX, tB.tft_first_row + menuEntryY + offSetY,
				tB.tft_last_column + offSetX, tB.tft_first_row + menuEntryY + space + offSetY,
				tB.tft_last_column-space + offSetX, tB.tft_first_row + menuEntryY + 2*space + offSetY,
				color);
	}

	void clearTextArea() {
		// clears the screen but not the headline area
		TFTscreen->stroke(blackColor);
		TFTscreen->fill(blackColor);
		TFTscreen->rect(tB.tft_first_column+1, tB.tft_first_row + headlineHight + 2* space, tB.tft_last_column-tB.tft_first_column-2, tB.tft_last_row - (tB.tft_first_row + headlineHight + 2*space)-1);
		TFTscreen->noFill();
	}

	void drawBlueBorder(bool fill) {
		TFTscreen->stroke(borderColor);
		if (fill) { TFTscreen->fill(blackColor); }
		else TFTscreen->noFill();
		TFTscreen->rect(tB.tft_first_column, tB.tft_first_row, tB.tft_last_column-tB.tft_first_column, tB.tft_last_row-tB.tft_first_row);
		TFTscreen->noFill();
	}


public:

	MenuScreen(TFT * TFTscreen, State* oldState) : IScreen(TFTscreen, oldState) { }

	void init() {
		_init = false;
		drawBlueBorder(true);
		showHeadline();
		refresh(oldState);

		_init = true;
	}


	void refresh(State * data) {
		uint8_t offSet = headlineHight+2*space;
		uint8_t lineDiff = 15;

		char *text = data->menuState->getMenuEntryName();
		char *oldText = oldState->menuState->getMenuEntryName();
		MenuIndex index = data->menuState->getSelectedMenuIndex();
		MenuIndex oldIndex = this->oldState->menuState->getSelectedMenuIndex();
//Serial.print("Menu index: ");
//Serial.print(index);
//Serial.print(", old index: ");
//Serial.println(oldIndex);

		bool indexChanged = oldIndex != index || !_init;
		indexChanged |= data->menuState->opened != oldState->menuState->opened;

		bool valueChanged = indexChanged;
		for (uint8_t i=0; i<data->menuState->maxMenuEntryCount(); i++) {
			valueChanged |= data->menuState->values[i] != oldState->menuState->values[i];
		}

/*
		if (indexChanged || valueChanged) {
for (int i=0; i<data->menuState->maxMenuEntryCount(); i++) {
	Serial.print("MenuScreen: value: i: "); Serial.print(i); Serial.print(", "); Serial.println(data->menuState->values[i]);
}
Serial.print("MenuScreen: MenuIndex size: "); Serial.println(sizeof(MenuIndex));
Serial.print("MenuScreen: comp. maxIndex: "); Serial.println(sizeof(MenuIndex)/sizeof(unsigned char));
Serial.print("MenuScreen: maxValues: "); Serial.println(data->menuState->maxValueLines());
Serial.print("MenuScreen: maxIndex: "); Serial.println(data->menuState->maxMenuEntryCount());
Serial.print("MenuScreen: oldIndex: "); Serial.print(oldIndex); Serial.print(", index: "); Serial.println(index);
Serial.print("MenuScreen: oldText: "); Serial.print(oldText); Serial.print(", text: "); Serial.println(text);
Serial.println();
		}
*/

		if (indexChanged) {
			//clearTextArea();
			drawMenuEntryArrows(data->menuState);

			TFTscreen->noFill();
			TFTscreen->setTextSize(1);

			//delete old menu entry name
			TFTscreen->stroke(blackColor);
			TFTscreen->text(oldText, tB.tft_first_column+space, tB.tft_first_row + menuEntryY);

			// show new menu entry
			TFTscreen->stroke(greyColor);
			TFTscreen->text(text, tB.tft_first_column+space, tB.tft_first_row + menuEntryY);
		}

		if (valueChanged) {
			TFTscreen->noFill();
			TFTscreen->setTextSize(1);

			TFTscreen->stroke(Color::black);
			for (uint8_t j=0; j < oldState->menuState->maxValueLines(); j++) {
				text = oldState->menuState->menuEntryValues[j];
				TFTscreen->text(text, tB.tft_first_column+space, tB.tft_first_row+4 + offSet + lineDiff*(j+1));
			}

			TFTscreen->stroke(Color::getGrey());
			for (uint8_t j=0; j < data->menuState->maxValueLines(); j++) {
				text = data->menuState->menuEntryValues[j];
				TFTscreen->text(text, tB.tft_first_column+space, tB.tft_first_row+4 + offSet + lineDiff*(j+1));
			}
		}
	}

	void refresh() {
		this->refresh(this->oldState);
	}
};
