#include "EffectsController.h"
#include <Arduino.h>

const color colorTable[] = {
	{255,255,0},
	{254,153,0},
	{255,0,204},
	{204,0,255},
	{154,0,255},
	{43,43,147},
	{0,153,255},
	{0,204,255},
	{0,255,255},
	{0,255,192},
	{188,250,79},
	{96,227,255},
	{0,60,255},
	{195,142,255}
};

EffectsController::EffectsController(HardwareSerial * ser) {
	serial = ser;

	targetColor = {255,255,255};
}

void EffectsController::begin(int baud) {

	serial->begin(baud);
	// while(!serial->available()) {

	// }
	Serial.println("connected to light controller");
}

void EffectsController::setTargetColor(int r, int g, int b) {
	serial->write('s');
	serial->write(0x00);
	serial->write(r);
	serial->write(g);
	serial->write(b);
	serial->write('\n');
}

void EffectsController::setEffect(int effect) {
	// char payload[] = {'1', effect};
	// Serial.print(payload);
	// Serial.println("WAAAAAAAT");
	serial->write('s');
	serial->write(0x01);
	serial->write(effect);
	serial->write('\n');
}

void EffectsController::setEffect(int effect, color c) {
	setEffect(effect, c.r, c.g, c.b);
}

void EffectsController::setEffect(int effect, int r, int g, int b) {
	setTargetColor(r, g, b);
	setEffect(effect);
}

void EffectsController::setEnergy(int energy) {
	serial->write('s');
	serial->write(0x02);
	serial->write(energy);
	serial->write('\n');
}

void EffectsController::setCharge(int charge) {
	serial->write('s');
	serial->write(0x03);
	serial->write(charge);
	serial->write('\n');
}

void EffectsController::userChange() {
	setEnergy(100);
	color tmp = targetColor;
	do {
		targetColor = colorTable[random(0, 13)];
	} while (targetColor.r == tmp.r && targetColor.g == tmp.g && targetColor.b == targetColor.b);
	setTargetColor(targetColor.r, targetColor.g, targetColor.b);
	//setEnergy(100);
	setEffect(EFFECT_FLASH);
}

void EffectsController::userLeft() {
	setEffect(EFFECT_HOLD);
}

void EffectsController::batteryLow() {
	setEffect(EFFECT_LOW);
}

void EffectsController::flash(int r, int g, int b) {
	setTargetColor(r, g, b);
	setEffect(EFFECT_FLASH);
}
