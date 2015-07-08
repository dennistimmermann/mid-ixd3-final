#ifndef _LUA_HANDOFF_H
#define _LUA_HANDOFF_H

#include <queue>
#include "Position.h"
#include "Com.h"

#define STATE_ABANDONED 0x30
#define STATE_NEW 0x31
#define STATE_KNOWN 0x32
#define STATE_TWO 0x33

using namespace std;

class Handoff {
	elapsedMillis touchTime;
public:
	int touchPads[8] = { 1, 0, 23, 22, 18, 17, 16, 15};
	int state[8] = {0,0,0,0,0,0,0,0};
	Handoff(Com *_com, EffectsController *_lights);
	queue <Position> handoffs;
	Com *com;
	EffectsController *lights;
	int oldState;
	int newState;
	bool wasTwo;
	int positiv;
	int counter;
	bool doIt();
	void check();
	void run();
	void userChanged();
	void userLeft();
};

#endif
