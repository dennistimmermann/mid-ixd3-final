#include <Arduino.h>

#ifndef _LUA_GPRS_H
#define _LUA_GPRS_H

class Gprs {
	HardwareSerial * serial;

public:
	Gprs(HardwareSerial * ser);
	void run();
	void begin(int);
	void http_post(char* data);
	void queue(char* data);
	void send();
};

#endif
