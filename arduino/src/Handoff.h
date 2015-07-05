#ifndef _LUA_HANDOFF_H
#define _LUA_HANDOFF_H

#include <queue>
#include "Position.h"
#include "Com.h"

using namespace std;

class Handoff {
public:
	Handoff(Com *_com);
	queue <Position> handoffs;
	Com *com;
	bool doIt();
};

#endif
