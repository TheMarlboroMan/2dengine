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

	//The shape of a key.
	auto blue_color=ldv::rgba8(0, 87, 132, 255);
	stars={
		{{40, 20}, blue_color},
		{{33, 23}, blue_color},
		{{35, 31}, blue_color},
		{{46, 32}, blue_color},
		{{39, 45}, blue_color},
		{{51, 74}, blue_color},
		{{50, 65}, blue_color},
		{{33, 64}, blue_color},
		{{30, 74}, blue_color},
		{{42, 77}, blue_color},
	};
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

	for(auto& pt : stars) {

		pt.draw(_screen);
	}
}
