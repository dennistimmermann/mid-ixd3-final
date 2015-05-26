#include "Effects.h"
#include <Adafruit_NeoPixel.h>

Effects::Effects() {
	for(int i = 0; i < NUM_PIXELS; i++) {
		initial[i] = {0,0,0};
		target[i] = {0,0,0};
		faded[i] = {0,0,0};
		rotated[i] = {0,0,0};
		smoothed[i] = {0,0,0};
	}

	target_speed = 0.0;
	cur_effect = EFFECT_OFF;

	fade = 10000;

	strip.begin();
	strip.show();
}

void Effects::run() {
	if(cur_effect == EFFECT_CHARGE) {
		if(last_charge != charge) {
			last_charge = charge;

			for(int i = 0; i < NUM_PIXELS; i++) {
				target[i].r = 255;
			}
		}
	}
	calc();
	draw();
}

void Effects::calc() {
	double a = max(timer/fade, 0.0);
	double b = 1.0 - a;

	//fade
	for(int i = 0; i < NUM_PIXELS; i++) {
		faded[i].r = ( a * initial[i].r ) + ( b * target[i].r);
		faded[i].g = ( a * initial[i].g ) + ( b * target[i].g);
		faded[i].b = ( a * initial[i].b ) + ( b * target[i].b);
	}
}

void Effects::draw() {
	for(int i = 0; i < NUM_PIXELS; i++) {
		strip.setPixelColor(i, faded[i].r, faded[i].g, faded[i].b);
	}
	strip.show();
}
