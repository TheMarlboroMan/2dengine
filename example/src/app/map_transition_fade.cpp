#include "app/map_transition_fade.h"
#include <ldv/screen.h>
#include <ldv/box_representation.h>

using namespace app;

map_transition_fade::map_transition_fade(
	const app::exit& _exit,
	int _color,
	int _direction,
	double _time
)
	:timer{_time, 0., false},
	original_exit{_exit},
	color{ldv::rgba8(0,0,0,255)},
	direction{0==_direction ? directions::to : directions::from}
{

	switch(_color) {
		case colors::color_black:
		default:
			color=ldv::rgba8(0,0,0,255);
		break;
		case colors::color_white:
			color=ldv::rgba8(255,255,255,255);
		break;
	}
}

void map_transition_fade::tic(
	tdelta _delta
) {

	timer.tic(_delta);
}

bool map_transition_fade::is_finished() const {

	return timer.is_finished();
}

void map_transition_fade::draw(
	ldv::screen& _screen
) const {

	//Convert timer to a linear function from 0 to 1 (or the reverse if fading
	//from a color).flip over the Y axis if needed. X is the time line.
	//y is "alpha". y=f(x) or f(-x) for the flipped version.
	float x=direction==directions::to ? timer.get() : -timer.get();
	float y=(x/timer.get_max());

	//When fading "from" we need to offset the function.. by 1, because this is 0-1.
	if(direction==directions::from) {

		y=1.f+y;
	}

	//Just draw a box over the screen.
	ldv::box_representation box(
		_screen.get_rect(),
		color
	);

	box.set_blend(ldv::representation::blends::alpha);
	box.set_alphaf(y);
	box.draw(_screen);
}
