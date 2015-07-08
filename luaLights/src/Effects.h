#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#ifndef _LUA_EFFECTS_H
#define _LUA_EFFECTS_H
#define ENERGY_DRAIN_TIME 1000

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

#define NUM_PIXELS 24
#define EXP_SCALING 2

struct color {
	int r;
	int g;
	int b;
};

struct pixelData {
	double fade;
	elapsedMillis fadeTimer;
	bool finished;
};

class Effects {
	color initial[NUM_PIXELS];
	color target[NUM_PIXELS];
	color faded[NUM_PIXELS];
	color rotated[NUM_PIXELS];
	color smoothed[NUM_PIXELS];
	pixelData meta[NUM_PIXELS];

	double target_speed;
	int cur_effect;
	double fade;
	elapsedMillis energyTimer;
	elapsedMillis durTimer;
	bool _called;
	bool ranEffect;

	double rotation;
	double rotationSpeed;
	elapsedMillis rotationTimer;
	elapsedMillis twinkleTimer;

	int _step;

public:
	uint16_t charge;
	int energy;
	color targetColor;
	color prev_color;
	int prev_effect;
	int colorIndex;
	uint16_t last_charge;
	Effects();
	void run();
	void calc();
	void draw();
	void setEffect(int);
	void setEffect(int, color);
	void setEffect(int, int, int, int);
	void setInitial();
	void setFadeSpeed(double);
	bool step(int, uint);
	bool step(int);
	void gotoStep(int);
	void setPixelTarget(int, int, int, int, double);
	void setPixelTarget(int, int, int, int);
	void userChange();
	void userLeft();
	void batteryLow();
	void flash(int, int, int);

	void setTargetColor(int r, int g, int b);
	void setEnergy(int energy);
	void setCharge(int charge);
	//void setRotationSpeed(double speed);
	//void setRotationAcceleration(double acceleration);
	//void setFadingSpeed(double fading);
};

#endif
