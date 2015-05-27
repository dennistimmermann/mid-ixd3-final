#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#ifndef _LUA_EFFECTS_H
#define _LUA_EFFECTS_H

#define EFFECT_CHARGE 0x00
#define EFFECT_LOW_BATTERY 0x01
#define EFFECT_HOLD 0x02
#define EFFECT_PASS 0x03
#define EFFECT_TWINKLE 0x04
#define EFFECT_IDLE 0xFE
#define EFFECT_OFF 0xFF

#define NUM_PIXELS 24
#define EXP_SCALING 2

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
	elapsedMillis fadeTimer;
	elapsedMillis durTimer;
	bool _called;

	double rotation;
	elapsedMillis rotationTimer;

	int _step;

public:
	int charge;
	int last_charge;
	Effects();
	void run();
	void calc();
	void draw();
	void setEffect(int);
	void setInitial();
	void setFadeSpeed(double);
	bool step(int, uint);
	bool step(int);
	void gotoStep(int);
	void setPixelTarget(int, int, int, int);
	//void setRotationSpeed(double speed);
	//void setRotationAcceleration(double acceleration);
	//void setFadingSpeed(double fading);
};

#endif
