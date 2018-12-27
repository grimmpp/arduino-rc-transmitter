#include <Arduino.h>

class RgbLed {
private:
	unsigned char lum = 255;
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;
	unsigned char pinRed = 0;
	unsigned char pinGreen = 0;
	unsigned char pinBlue = 0;

	void refresh()  {
		analogWrite(pinRed, red * lum);
		analogWrite(pinGreen, green * lum);
		analogWrite(pinBlue, blue * lum);
	}

public:
	RgbLed(unsigned char pinRed,unsigned char pinGreen, unsigned char pinBlue) {
		this->pinRed = pinRed;
		this->pinGreen = pinGreen;
		this->pinBlue = pinBlue;
		pinMode(pinRed, OUTPUT);
		pinMode(pinGreen, OUTPUT);
		pinMode(pinBlue, OUTPUT);
	}

	void setLuminosity(unsigned char lum) {
		this->lum = lum;
		refresh();
	}

	void switchOff() {
		this->red = 0;
		this->green = 0;
		this->blue = 0;
		this->refresh();
	}

	void setBlue() {
		this->red = 0;
		this->green = 0;
		this->blue = 1;
		this->refresh();
	}

	void setGreen() {
		this->red = 0;
		this->green = 1;
		this->blue = 0;
		this->refresh();
	}

	void setRed() {
		this->red = 1;
		this->green = 0;
		this->blue = 0;
		this->refresh();
	}

	bool isBlueSet() {
		return this->blue;
	}

	bool isGreenSet() {
		return this->green;
	}

	bool isRedSet() {
		return this->red;
	}
};

