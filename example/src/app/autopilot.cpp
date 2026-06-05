#include "app/autopilot.h"

using namespace app;

autopilot::autopilot()
	:timer{0., 0.}
{}

void autopilot::tic(
	ldtools::tdelta _delta
) {

	if(timer.is_finished()) {

		return;
	}

	timer.tic(_delta);
}

void autopilot::receive(
	int _input,
	ldtools::tdelta _pause
) {

	if(0.!=_pause) {

		timer.target(_pause).restart();
	}

	enabled=true;
	current_produce=_input;
}

void autopilot::disable() {

	timer.target(0.);
	enabled=false;
	current_produce=0;
}

void autopilot::produce(
	player_input& _pli
) {

	if(timer.is_running()) {

		return;
	}

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
