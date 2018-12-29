#include <Arduino.h>
#include <SoftwareSerial.h>

#include <ButtonValues.h>

#ifndef InterConnector_Def
#define InterConnector_Def

#define SS_BAUD 57600

enum Command : char {SKIP, RESET_CONTROLLER, GET_NEXT_MESSAGE, START_SIGNAL = '<'};

class InterConnector {
private:
	SoftwareSerial *softSerial;
	char readData(ButtonValues*);

public:
	InterConnector(unsigned char,unsigned char); // RX, TX

	void sendButtonValues(ButtonValues);
	bool receiveButtonValues(ButtonValues*);
	Command receiveCommand();
};

#endif
