#include <app/pulse_background.h>

using namespace app;

std::size_t pulse_background::current_colour_index{0};
ldv::rgba_color pulse_background::colour{0., 0., 0, 1.};

pulse_background::pulse_background()
	:tweener_r{0., 0., 10.},
	tweener_g{0., 0., 10.},
	tweener_b{0., 0., 10.},
	counter{10.0, 0., false}
{

	tweener_r.set_interpolator(interpolator);
	tweener_g.set_interpolator(interpolator);
	tweener_b.set_interpolator(interpolator);

	//blue: 27, 38, 50
	//green: 0 46 10
	//red: 190 38 51 //TODO: too bright?

	colours={
		ldv::rgba8(0,0,0,255), //black
		ldv::rgba8(190, 38, 51, 255), //red
		ldv::rgba8(0,0,0,255), //black
		ldv::rgba8(0, 46, 10, 255), //green
		ldv::rgba8(0,0,0,255), //black
		ldv::rgba8(27, 38, 50, 255), //blue
	};

	//setup the tweeners from the current colour to the next...
	ready_tweeners();
}

void pulse_background::draw_background(
	ldv::screen& _screen
) {

	_screen.clear(colour);
}

void pulse_background::draw_foreground(
	ldv::screen& 
) {

}

void pulse_background::tic(
	tdelta _delta
) {

	counter.tic(_delta);
	if(counter.is_finished()) {

		counter.restart();

		ready_tweeners();

		++current_colour_index;
		if(current_colour_index >= colours.size()) {

			current_colour_index=0;
		}

	}

	int r=tweener_r.tic(_delta);
	int g=tweener_g.tic(_delta);
	int b=tweener_b.tic(_delta);

	colour=ldv::rgba8(r, g, b, 255);
}

void pulse_background::ready_tweeners() {

	std::size_t next_index=current_colour_index+1;
	if(next_index >= colours.size()) {

		next_index=0;
	}

	const auto& next=colours[next_index];

	//Setups the tweener from the current colour
	tweener_r.reset(ldv::colorif(colour.r), ldv::colorif(next.r), counter.get_max());
	tweener_g.reset(ldv::colorif(colour.g), ldv::colorif(next.g), counter.get_max());
	tweener_b.reset(ldv::colorif(colour.b), ldv::colorif(next.b), counter.get_max());
}
