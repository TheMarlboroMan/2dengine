#include "app/autopilot.h"

using namespace app;

void autopilot::receive(
	int _input
) {

	enabled=true;
	current_produce=_input;
}

void autopilot::disable() {

	enabled=false;
	current_produce=0;
}

void autopilot::produce(
	player_input& _pli
) {

	//This duplicates the structure in the main controller, only changing the
	//input... I guess we could refactor to prevent subtle bugs.

	if(current_produce & down) {

		_pli.y=-1;
	}
	else if(current_produce & up) {

		_pli.activate=true;

		if(current_produce & pressed) {

			_pli.y=1;
		}
	}

	if(current_produce & left) {

		_pli.x=-1;
	}
	else if(current_produce & right) {

		_pli.x=1;
	}

	if(current_produce & jump) {

		_pli.jump=true;

		if(current_produce & pressed) {

			_pli.hold_jump=true;
		}
	}
}
