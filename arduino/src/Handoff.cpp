#include <queue>
#include "Handoff.h"



Handoff::Handoff(Com *_com) {
	com = _com;
}

bool Handoff::doIt() {

	Serial.println(com->position.lat);
	return true;
}
