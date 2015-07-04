#include "Effects.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

Adafruit_NeoPixel leds = Adafruit_NeoPixel(24, 1, NEO_GRB + NEO_KHZ800);

int scale(int in, int max, double scale) {
	return pow(in, scale)/(pow(max, scale)/max);
}

const uint8_t correction[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

const color colorTable[] = {
	{47,126,157},
	{145,58,223},
	{24,192,174},
	{129,228,249},
	{223,29,170}
};

Effects::Effects() {
	for(int i = 0; i < NUM_PIXELS; i++) {
		initial[i] = {0,0,0};
		target[i] = {0,0,0};
		faded[i] = {0,0,0};
		rotated[i] = {0,0,0};
		smoothed[i] = {0,0,0};
		meta[i] = {500, 0, false};
	}

	target_speed = 0.0;
	cur_effect = EFFECT_OFF;

	fade = 5000;
	rotation = 0;
	rotationSpeed = 0.0;

	charge = 0;
	energy = 100;
	last_charge = -1;

	solidR = 0;
	solidG = 0;
	solidB = 0;

	colorIndex = 0;

	_step = 0;
	_called = false;

	leds.begin();
	leds.show();
	//setInitial();
}

void Effects::run() {
	if(cur_effect == EFFECT_CHARGE) {
		if(last_charge != charge) {
			setFadeSpeed(5000);
			Serial.println("charge");
			rotationSpeed = 1;

			last_charge = charge;

			for(int i = 0; i < NUM_PIXELS; i++) {
				int intensity = constrain((int)(24.0/(i+1) * (charge+8) * 4.0 - 200), 0, 255);

				target[i].r = ( 1-(charge/100.0) ) * intensity;
				target[i].g = (charge/100.0) * intensity;
				target[i].b = 0;
				meta[i].fade = 5000;
				meta[i].finished = false;
				meta[i].fadeTimer = 0;
			}

			//setInitial();

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
	} else if(cur_effect == EFFECT_FLASH) {
		if(step(0, 500)) {
			for(int i = 0; i < NUM_PIXELS; i++) {
				//setPixelTarget(i, 255, 255, 255, 500);
				setPixelTarget(i, colorTable[colorIndex].r, colorTable[colorIndex].g, colorTable[colorIndex].b, 500);
			}
		}
		if(step(1, 1500)) {
			for(int i = 0; i < NUM_PIXELS; i++) {
				//setPixelTarget(i, 128, 128, 128, 2000);
				setPixelTarget(i, colorTable[colorIndex].r/2, colorTable[colorIndex].g/2, colorTable[colorIndex].b/2, 2000);
			}
		}
		if(step(2)) {
			setEffect(EFFECT_TWINKLE);
		}
	} else if(cur_effect == EFFECT_TWINKLE) {
		//setFadeSpeed(1000);

		if(twinkleTimer > (5000/energy) )  {
			int n = random(0, 23);
			//setPixelTarget(n, 255, 255, 255, (25000/energy) );
			setPixelTarget(n, colorTable[colorIndex].r, colorTable[colorIndex].g, colorTable[colorIndex].b, (25000/energy) );
			twinkleTimer = 0;
		}
		for(int i = 0; i < NUM_PIXELS; i++) {
			if(meta[i].finished == true) {
				setPixelTarget(i, 0, 0, 0, floor(500*log(energy)) );
			}
		}
		// if(faded[1].r >= 127) {
		// 	Serial.println("wat");
		// 	setPixelTarget(1, 0, 0, 0, 2000);
		// }
	} else if(cur_effect == EFFECT_SOLID) {
		for(int i = 0; i < NUM_PIXELS; i++) {
			setPixelTarget(i, solidR, solidG, solidB, 1);
		}
	}
	calc();
	draw();
}

void Effects::calc() {

	//Serial.println(a);

	//rotation = fmod(rotation + (rotationTimer/-100.0), NUM_PIXELS); //rotationspeed

	rotation = fmod((fmod(rotation + rotationSpeed * (rotationTimer/-100.0), NUM_PIXELS) + NUM_PIXELS), NUM_PIXELS);
	rotationTimer = 0;

	//fade
	for(int i = 0; i < NUM_PIXELS; i++) {

		double a = max( (meta[i].fade-meta[i].fadeTimer) /meta[i].fade, 0.0);
		double b = 1.0 - a;

		if(meta[i].fadeTimer > meta[i].fade) {
			meta[i].finished = true;
			//Serial.println("finished");
		}

		faded[i].r = constrain( ( a * initial[i].r ) + ( b * target[i].r), 0, 255);
		faded[i].g = constrain(( a * initial[i].g ) + ( b * target[i].g), 0, 255);
		faded[i].b = constrain(( a * initial[i].b ) + ( b * target[i].b), 0, 255);
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

		smoothed[i].r = (int) constrain(( perc * rotated[i].r ) +  ( (1.0-perc) * rotated[ (i+1)%NUM_PIXELS ].r), 0, 255);
		smoothed[i].g = (int) constrain(( perc * rotated[i].g ) +  ( (1.0-perc) * rotated[ (i+1)%NUM_PIXELS ].g), 0, 255);
		smoothed[i].b = (int) constrain(( perc * rotated[i].b ) +  ( (1.0-perc) * rotated[ (i+1)%NUM_PIXELS ].b), 0, 255);
	}

	//Serial.println( fmod(rotation, NUM_PIXELS));
}

void Effects::setPixelTarget(int num, int r, int g, int b, double fade) {
	target[num].r = constrain(r, 0, 255);
	target[num].g = constrain(g, 0, 255);
	target[num].b = constrain(b, 0, 255);

	meta[num].fadeTimer = 0;
	meta[num].fade = fade;
	meta[num].finished = false;

	initial[num].r = faded[num].r;
	initial[num].g = faded[num].g;
	initial[num].b = faded[num].b;
}

void Effects::setPixelTarget(int num, int r, int g, int b) {
	setPixelTarget(num, r, g, b, 500);
}



void Effects::draw() {
	for(int i = 0; i < NUM_PIXELS; i++) {
		int r = smoothed[i].r;
		int g = smoothed[i].g;
		int b = smoothed[i].b;

		r = map(r, 0, 255, 0, 255);
		g = map(g, 0, 255, 0, 230);
		b = map(b, 0, 255, 0, 244);

		r = correction[r];
		g = correction[g];
		b = correction[b];

		leds.setPixelColor(i, r, g, b);
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
