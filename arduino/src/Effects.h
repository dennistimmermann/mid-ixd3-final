#include <Arduino.h>

#ifndef _LUA_EFFECTS_H
#define _LUA_EFFECTS_H

#define EFFECT_CHARGE 0x00
#define EFFECT_LOW_BATTERY 0x01
#define EFFECT_HOLD 0x02
#define EFFECT_PASS 0x03

class Effects {
	HardwareSerial * serial;
	TinyGPSPlus tgps;
	double lat = 0;
	double lon = 0;
	long timestamp = 0;
public:
	Effects();
	void run();
	void setEffect(int effect);
	void setRotationSpeed(double speed);
	void setRotationAcceleration(double acceleration);
	void setFadingSpeed(double fading);
};

#endif
