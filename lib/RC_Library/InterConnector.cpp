#include <Arduino.h>
#include <InterConnector.h>
#include <ButtonValues.h>

	InterConnector::InterConnector(unsigned char pinA, unsigned char pinB) {
		this->softSerial = new SoftwareSerial(pinA, pinB); // RX, TX
		this->softSerial->begin(SS_BAUD);
	}

	void InterConnector::sendButtonValues(ButtonValues value) {
		//Send start signal
		this->softSerial->write(START_SIGNAL);
		this->softSerial->write(START_SIGNAL);

		char* p= (char*)&value;
		char crc = 0xff;
		for(unsigned i=0; i<sizeof(ButtonValues); i++) {
			this->softSerial->write(*p);
			crc ^= *p;
			p++;
		}
		this->softSerial->write(crc);
	}

	Command InterConnector::receiveCommand() {
		Command cmd = SKIP;
		while (this->softSerial->available() > 0) {
			cmd = (Command) this->softSerial->read();
		}
		return cmd;
	}



	char InterConnector::readData(ButtonValues* data) {
		char* p = (char*)data;
		char crc =0xff;
		for (int i=0; i < (int)sizeof(ButtonValues); i++)
		{
			*p = this->softSerial->read();
			crc ^= *p;
			p++;
		}
		crc ^= this->softSerial->read();
		return crc == 0x00;
	}


	bool InterConnector::receiveButtonValues(ButtonValues *values) {
		boolean dataRead = false;
		int startSignalCount = 0;
		this->softSerial->write(GET_NEXT_MESSAGE);

		while (!dataRead) {
			while (this->softSerial->available() > 0 && startSignalCount < 2) {
				char startSignal = this->softSerial->read();
				if (startSignal == START_SIGNAL) {
					startSignalCount++;
				} else {
					startSignalCount = 0;
					Serial.println("UART: skip");
				}
			}

		    if (this->softSerial->available() >= (int)sizeof(ButtonValues)+1) {
		    	ButtonValues data;
		    	if (this->readData(&data)) {
//Serial.println("Valid data");
		    		*values = data;
		    		return true;
		    	} else {
		    		Serial.println("UART: Warning: no valid data");
		    	}
		    	dataRead = true;
		    }
		}

		return false;
	}

