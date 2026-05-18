#include "app/storm.h"
#include "app/random.h"
#include "app/definitions.h"

#include <ldv/screen.h>
#include <ldv/color.h>

#include <iostream>

using namespace app;

storm::storm(
	random& _rng,
	int _w,
	int _h,
	int _star_count
): rng{_rng}, w{_w}, h{_h}, state{state_wait}, star_count{_star_count} {

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

	random_stars.reserve(star_count);
	shuffle_stars();
}

void storm::tic(
	tdelta _delta
) {

	play_sound=false;

	switch(state) {

		case state_wait: return tic_wait(_delta);
		case state_thunder: return tic_thunder(_delta);
	}
}

void storm::tic_wait(
	tdelta _delta
) {

	timeouts.tic(timeout_wait, _delta);
	if(!timeouts.is_finished(timeout_wait)) {

		return;
	}

	play_sound=true;
	state=state_thunder;
	timeouts.restart(timeout_thunder);
	shuffle_stars();
}

void storm::tic_thunder(
	tdelta _delta
) {

	timeouts.tic(timeout_thunder, _delta);

	if(!timeouts.is_finished(timeout_thunder)) {

		return;
	}

	state=state_wait;
	timeouts.target(timeout_wait, rng.get(10, 20));
	timeouts.restart(timeout_wait);
}

void storm::draw(
	ldv::screen& _screen
) {

	if(state_thunder==state) {

		//the max value of thunder is black, zero is white, calculate.
		int color_val=255 - ( timeouts.get(timeout_thunder) * (255 / timeouts.get_max(timeout_thunder)) );
		_screen.clear(
			ldv::rgba8(color_val, color_val, color_val, 255)
		);
	}

	for(auto& pt : random_stars) {

		pt.draw(_screen);
	}

	//Draw thunder stars after, in case of overlap.
	if(state_thunder==state) {

		for(auto& pt : stars) {

			pt.draw(_screen);
		}
	}
}

int storm::get_sound() const {

	return play_sound
		? snd_thunder
		: 0;
}

void storm::shuffle_stars() {

	//Palette colors for stars...
	ldv::rgba_color variants[]={
		ldv::rgba8(255,255,255,255),
		ldv::rgba8(157, 157, 157, 255),
		ldv::rgba8(27, 38, 50,255),
		ldv::rgba8(178, 220, 239, 255)
	};

	random_stars.clear();
	for(int i=0; i<star_count; i++) {

		random_stars.push_back({
			{
				rng.get(1, w-1),
				rng.get(1, h-1)
			},
			variants[rng.get(0, 3)]
		});
	}
}
