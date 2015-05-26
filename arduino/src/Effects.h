#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#ifndef _LUA_EFFECTS_H
#define _LUA_EFFECTS_H

#define EFFECT_CHARGE 0x00
#define EFFECT_LOW_BATTERY 0x01
#define EFFECT_HOLD 0x02
#define EFFECT_PASS 0x03
#define EFFECT_IDLE 0xFE
#define EFFECT_OFF 0xFF

#define NUM_PIXELS 24

int charge = 50;
int last_charge = 0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, 1, NEO_GRB + NEO_KHZ800);

struct color {
	int r;
	int g;
	int b;
};

class Effects {
	color initial[NUM_PIXELS];
	color target[NUM_PIXELS];
	color faded[NUM_PIXELS];
	color rotated[NUM_PIXELS];
	color smoothed[NUM_PIXELS];

	double target_speed;
	int cur_effect;
	double fade;
	elapsedMillis timer;
public:
	Effects();
	void run();
	void calc();
	void draw();
	//void setEffect(int effect);
	//void setRotationSpeed(double speed);
	//void setRotationAcceleration(double acceleration);
	//void setFadingSpeed(double fading);
};

#endif
