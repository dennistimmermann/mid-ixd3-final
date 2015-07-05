#include <Arduino.h>

#ifndef _LUA_POSITION_H
#define _LUA_POSITION_H

struct Position {
	float lat;
	float lon;
	float speed;
	float heading;
	float altitude;
	long timestamp;
};

#endif
