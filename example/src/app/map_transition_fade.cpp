#include "app/map_transition_fade.h"
#include <ldv/screen.h>
#include <ldv/box_representation.h>

using namespace app;

map_transition_fade::map_transition_fade(
	const app::exit& _exit
)
	:timer{0.25, 0., false},
	original_exit{_exit}
{}

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

	//Linear conversion to 0-1
	float alpha=(timer.get() - 0.)/(timer.get_max() - 0.);

std::cout<<alpha<<std::endl;

	//Just draw a black box over this...
	ldv::box_representation box(
		_screen.get_rect(),
		ldv::rgba8(0,0,0,255)
	);

	box.set_blend(ldv::representation::blends::alpha);
	box.set_alphaf(alpha);
	box.draw(_screen);

}
