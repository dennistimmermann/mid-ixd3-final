#include <queue>
#include "Handoff.h"

#define TOUCH_THRESHOLD_UPPER 1600
#define TOUCH_THRESHOLD_LOWER 1500
#define TOUCH_IDLE_TIME 7000

Handoff::Handoff(Com *_com, EffectsController * _lights) {
	com = _com;
	lights = _lights;
	// touchPads = { 0, 23, 22, 19, 18, 17, 16, 15};
	// state = {0,0,0,0,0,0,0,0};
	positiv = 0;
	counter = 0;
	wasTwo = false;

	oldState = STATE_KNOWN;
	newState = STATE_KNOWN;
}

bool Handoff::doIt() {

	Serial.println(com->position.lat);
	return true;
}

void Handoff::run() {
	check();
}

void Handoff::check() {
	positiv = 0;
	counter = 0;
	for(int i = 0; i < 8; i++) {
		int val = touchRead(touchPads[i]);
		int s = state[i];
		if(val > TOUCH_THRESHOLD_UPPER) {
			s = 1;
			positiv++;
		} else if (val < TOUCH_THRESHOLD_LOWER) {
			s = 0;
		}
		if(state[i] != s) {
			Serial.println(". counting up");
			counter++;
			state[i] = s;
		}
	}
	if(oldState == STATE_TWO && positiv <= 6) {
		//wasTwo = false;
		//userChanged();
		newState = STATE_NEW;
	} else if(positiv > 6) {
		Serial.println("... zwei personen an der kugel");
		//wasTwo = true;
		newState = STATE_TWO;
	}
	else if(positiv == 0) {
		newState = STATE_ABANDONED;
		//userLeft();
		Serial.println("... keine person an der kugel");
	} else if(counter > 4) {
		Serial.println("... kugel wurde uebergeben");
		//userChanged();
		newState = STATE_NEW;
	} else if(touchTime > TOUCH_IDLE_TIME) {
		//userChanged();
		Serial.println("... neuer Nutzer");
		newState = STATE_NEW;
		touchTime = 0;
	} else if(touchTime < TOUCH_IDLE_TIME) {
		Serial.println("... gleicher Nutzer");
		newState = STATE_KNOWN;
		touchTime = 0;
	} else {
		newState = STATE_KNOWN;
		Serial.println("wat");
	}

	if(newState != oldState) {
		Serial.println("statechange");
		Serial.println(newState);
		Serial.println(oldState);
		if(newState == STATE_NEW) {
			userChanged();
		} else if(newState == STATE_ABANDONED) {
			userLeft();
		}
	}
	oldState = newState;
}

void Handoff::userChanged() {
	Serial.println("!!! nutzerwechsel");
	lights->userChange();
	com->send();
}

void Handoff::userLeft() {
	Serial.println("! user abandoned lua sadface");
	lights->userLeft();
}
