
#include <EEPROM.h>

#ifndef EEPROMSTORAGE_DEF
#define EEPROMSTORAGE_DEF

struct EepromValues {
	char mode = 'A';

	bool isMenuOpen = false;
	uint8_t menuIndex = 0;
	uint8_t values[2] = {80,0};
};

class EepromStorage {
	const static uint8_t eepromAdr = 0;
public:
	static void getEepromValues(EepromValues *values) {
		EEPROM.get(eepromAdr, *values);
	}

	static void setEepromValues(EepromValues values) {
		EEPROM.put(eepromAdr, values);
	}
};

#endif EEPROMSTORAGE_DEF
