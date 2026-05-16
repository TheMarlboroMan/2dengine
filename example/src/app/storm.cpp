#include "app/storm.h"
#include "app/random.h"

#include <ldv/screen.h>
#include <ldv/color.h>

#include <iostream>

using namespace app;

storm::storm(
	random& _rng
): rng{_rng} {

	timeouts.add(timeout_wait, rng.get(5, 15), 0., false);
	timeouts.add(timeout_thunder, 0.5, 0., true);
}

void storm::tic(
	tdelta _delta
) {

	timeouts.tic(_delta);
	if(!timeouts.is_finished(timeout_wait)) {

		return;
	}

	if(timeouts.is_paused(timeout_thunder)) {

		timeouts.resume(timeout_thunder);
	}

	if(!timeouts.is_finished(timeout_thunder)) {

		return;
	}

	timeouts.pause(timeout_thunder);
	timeouts.reset(timeout_thunder);

	timeouts.target(timeout_wait, rng.get(10, 20));
	timeouts.restart(timeout_wait);
}

void storm::draw(
	ldv::screen& _screen
) {

	if(timeouts.is_paused(timeout_thunder)) {

		return;
	}

	//the max value of thunder is black, zero is white, calculate.
	int color_val=255 - ( timeouts.get(timeout_thunder) * (255 / timeouts.get_max(timeout_thunder)) );
	_screen.clear(
		ldv::rgba8(color_val, color_val, color_val, 255)
	);
}
