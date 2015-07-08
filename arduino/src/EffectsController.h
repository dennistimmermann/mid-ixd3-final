#include <Arduino.h>

#ifndef _LUA_EFFECTS_H
#define _LUA_EFFECTS_H

#define EFFECT_CHARGE 0x08
#define EFFECT_LOW_BATTERY 0x01
#define EFFECT_HOLD 0x02
#define EFFECT_PASS 0x03
#define EFFECT_TWINKLE 0x04
#define EFFECT_FLASH 0x05
#define EFFECT_SOLID 0x06
#define EFFECT_LOW 0x07
#define EFFECT_LIMBO 0xFD
#define EFFECT_IDLE 0xFE
#define EFFECT_OFF 0xFF

struct color {
	int r;
	int g;
	int b;
};

class EffectsController {
	HardwareSerial * serial;
public:
	color targetColor;
	EffectsController(HardwareSerial * ser);
	void begin(int baud);
	void setTargetColor(int, int, int);
	void setEffect(int);
	void setEffect(int, color);
	void setEffect(int, int, int, int);
	void userChange();
	void userLeft();
	void batteryLow();
	void flash(int, int, int);
	void setEnergy(int energy);
	void setCharge(int charge);
	//void setRotationSpeed(double speed);
	//void setRotationAcceleration(double acceleration);
	//void setFadingSpeed(double fading);
};

#endif
