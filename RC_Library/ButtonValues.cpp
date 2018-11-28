#include "Arduino.h"
#include "../RC_Library/ButtonValues.h"

	void ButtonValues::print() {
	  Serial.print("init: "); Serial.print(initialized);
	  Serial.print(" btn b r: "); Serial.print(btn_bottom_right);
	  Serial.print(" btn b l: "); Serial.print(btn_bottom_left);
	  Serial.print(" btn m r: "); Serial.print(btn_middle_right);
	  Serial.print(" btn m l: "); Serial.print(btn_middle_left);
	  Serial.print(" btn t r: "); Serial.print(btn_top_right);
	  Serial.print(" btn t l: "); Serial.print(btn_top_left);
	  Serial.print(" poti r X: "); Serial.print((int)poti_right_X);
	  Serial.print(" poti r Y: "); Serial.print((int)poti_right_Y);
	  Serial.print(" poti l X: "); Serial.print((int)poti_left_X);
	  Serial.print(" poti l Y: "); Serial.println((int)poti_left_Y);
	}
