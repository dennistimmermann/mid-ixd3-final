#include "Effects.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

Adafruit_NeoPixel leds = Adafruit_NeoPixel(24, 1, NEO_GRB + NEO_KHZ800);

int scale(int in, int max, double scale) {
	return pow(in, scale)/(pow(max, scale)/max);
}

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

	fade = 5000;
	rotation = 0;

	charge = 0;
	last_charge = -1;

	_step = 0;
	_called = false;

	leds.begin();
	leds.show();
}

void Effects::run() {
	if(cur_effect == EFFECT_CHARGE) {
		if(last_charge != charge) {
			setFadeSpeed(5000);
			Serial.println("charge");

			last_charge = charge;

			for(int i = 0; i < NUM_PIXELS; i++) {
				int intensity = constrain((int)(24.0/(i+1) * (charge+8) * 4.0 - 200), 0, 255);

				target[i].r = ( 1-(charge/100.0) ) * intensity;
				target[i].g = (charge/100.0) * intensity;
				target[i].b = 0;
			}

			setInitial();

			//for(int i = 0; i < NUM_PIXELS; i++) {
			//	target[i].r = 255;
			//}
			//target[0].r = 255;
		}
	} else if(cur_effect == EFFECT_HOLD) {
		Serial.println(fadeTimer);
		if( step(0, 1000) ) {
			Serial.println("hell");
			for(int i = 0; i < NUM_PIXELS; i++) {
				target[i].r = 96;
			}
		}
		if( step(1, 500) ) {
			Serial.println("dunkel");
			for(int i = 0; i < NUM_PIXELS; i++) {
				target[i].r = 48;
			}
		}
		if( step(2, 750) ) {
			Serial.println("dunkel");
			for(int i = 0; i < NUM_PIXELS; i++) {
				target[i].r = 86;
			}
		}
		if( step(3, 1000) ) {
			Serial.println("dunkel");
			for(int i = 0; i < NUM_PIXELS; i++) {
				target[i].r = 48;
			}
		}
		if( step(4) ) {
			gotoStep(0);
		}
	} else if(cur_effect == EFFECT_TWINKLE) {

	}
	calc();
	draw();
}

void Effects::calc() {
	double a = max( (fade-fadeTimer) /fade, 0.0);
	double b = 1.0 - a;

	//Serial.println(a);

	//rotation = fmod(rotation + (rotationTimer/-100.0), NUM_PIXELS); //rotationspeed

	rotation = fmod((fmod(rotation + (rotationTimer/-10.0), NUM_PIXELS) + NUM_PIXELS), NUM_PIXELS);
	rotationTimer = 0;

	//fade
	for(int i = 0; i < NUM_PIXELS; i++) {
		faded[i].r = ( a * initial[i].r ) + ( b * target[i].r);
		faded[i].g = ( a * initial[i].g ) + ( b * target[i].g);
		faded[i].b = ( a * initial[i].b ) + ( b * target[i].b);
	}

	//rotate
	for(int i = 0; i < NUM_PIXELS; i++) {
		rotated[(int)fmod(i+rotation, NUM_PIXELS)].r = faded[i].r;
		rotated[(int)fmod(i+rotation, NUM_PIXELS)].g = faded[i].g;
		rotated[(int)fmod(i+rotation, NUM_PIXELS)].b = faded[i].b;
	}

	//smooth
	for(int i = 0; i < NUM_PIXELS; i++) {
		double perc = rotation - (int)rotation;

		smoothed[i].r = (int)( perc * rotated[i].r ) +  ( (1.0-perc) * rotated[ (i+1)%NUM_PIXELS ].r);
		smoothed[i].g = (int)( perc * rotated[i].g ) +  ( (1.0-perc) * rotated[ (i+1)%NUM_PIXELS ].g);
		smoothed[i].b = (int)( perc * rotated[i].b ) +  ( (1.0-perc) * rotated[ (i+1)%NUM_PIXELS ].b);
	}

	//Serial.println( fmod(rotation, NUM_PIXELS));
}

void Effects::setPixelTarget(int num, int r, int g, int b) {
	target[num].r = r;
	target[num].g = g;
	target[num].b = b;
}

void Effects::draw() {
	for(int i = 0; i < NUM_PIXELS; i++) {
		leds.setPixelColor(i, scale(smoothed[i].r, 255, 3), scale(smoothed[i].g, 255, 3), scale(smoothed[i].b, 255, 3));
	}
	leds.show();
}

void Effects::setEffect(int effect) {
	gotoStep(0);
	setInitial();
	cur_effect = effect;
}

void Effects::setInitial() {
	for(int i = 0; i < NUM_PIXELS; i++) {
		initial[i].r = faded[i].r;
		initial[i].g = faded[i].g;
		initial[i].b = faded[i].b;
	}
	fadeTimer = 0;
}

void Effects::setFadeSpeed(double fadespeed) {
	fade = fadespeed;
}

bool Effects::step(int num, uint dur) {
	if(num == _step) {
		if(durTimer < dur) {
			if(_called == true) {
				return false;
			} else {
				setFadeSpeed(dur);
				setInitial();
				_called = true;
				return true;
			}
		} else {
			gotoStep(_step+1);
			return false; // return false?
		}
	}
	return false;
}

bool Effects::step(int num) {
	return step(num, -1);
}

void Effects::gotoStep(int s) {
	_called = false;
	_step = s;
	durTimer = 0;
}
