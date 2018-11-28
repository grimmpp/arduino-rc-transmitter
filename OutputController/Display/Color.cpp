#include <Arduino.h>

static uint8_t Color_Brightness = 50;

class Color {
public:
	static const uint8_t defaultValue = 80;
	static const uint8_t black = 0;

	static uint16_t getRed() {
		return min(255, max(0, (defaultValue + Color_Brightness) ));
	}

	static uint16_t getGreen() {
		return min(255, max(0, (defaultValue + Color_Brightness) << 5 ));
	}

	static uint16_t getBlue() {
		return min(255, max(0, (defaultValue + Color_Brightness) << 10 ));
	}

	static uint16_t getGrey() {
		return getRed() + getGreen() + getBlue();
	}
};


