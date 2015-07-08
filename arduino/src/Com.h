#include <Arduino.h>
#include <Adafruit_FONA.h>
#include "Position.h"
#include "EffectsController.h"

#ifndef _LUA_GPRS_H
#define _LUA_GPRS_H

class Com {
	HardwareSerial * serial;
	EffectsController * lights;
	bool charging;

public:
	Com(HardwareSerial * ser, EffectsController * _lights);
	elapsedMillis updatePositionTimer;
	elapsedMillis networkStatusTimer;
	elapsedMillis chargeTimer;
	Position position;

	void begin(int);
	void http_post(char* data);
	void queue(char* data);
	bool send();
	void unlock();
	bool connect();
	void checkGPS();
	void run();
	bool isCharging();
};

#endif
