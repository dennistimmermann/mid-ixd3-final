#include <Arduino.h>
#include <Adafruit_FONA.h>
#include "Position.h"

#ifndef _LUA_GPRS_H
#define _LUA_GPRS_H

class Com {
	HardwareSerial * serial;
	bool charging;

public:
	Com(HardwareSerial * ser);
	elapsedMillis updatePositionTimer;
	elapsedMillis networkStatusTimer;
	Position position;

	void begin(int);
	void http_post(char* data);
	void queue(char* data);
	void send();
	void unlock();
	bool connect();
	void checkGPS();
	void run();
};

#endif
