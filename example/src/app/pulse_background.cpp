#include <app/pulse_background.h>

using namespace app;

constexpr double duration{10.};

std::size_t pulse_background::current_colour_index{0};
float pulse_background::r{0.f};
float pulse_background::g{0.f};
float pulse_background::b{0.f};
double pulse_background::timer_remains{duration};

pulse_background::pulse_background()
	:tweener_r{0., 0., timer_remains},
	tweener_g{0., 0., timer_remains},
	tweener_b{0., 0., timer_remains},
	counter{duration, 0., false}
{

	counter.set(duration-timer_remains);
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

pulse_background::~pulse_background() {

	timer_remains=counter.get_max() - counter.get();
}

void pulse_background::draw_background(
	ldv::screen& _screen
) {

	_screen.clear(
		ldv::rgba_color(r, g, b, 255)
	);
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

	r=tweener_r.tic(_delta);
	g=tweener_g.tic(_delta);
	b=tweener_b.tic(_delta);
}

void pulse_background::ready_tweeners() {

	std::size_t next_index=current_colour_index+1;
	if(next_index >= colours.size()) {

		next_index=0;
	}

	const auto& next=colours[next_index];

	//Setups the tweener from the current colour
	tweener_r.reset(r, next.r, counter.get_max());
	tweener_g.reset(g, next.g, counter.get_max());
	tweener_b.reset(b, next.b, counter.get_max());
}
