#pragma once

#include "app/background_interface.h"
#include <d2d/components/timeout.h>
#include <ldv/color.h>
#include <ldv/screen.h>
#include <tools/tweener.h>
#include <tools/tweener_interpolators.h>
#include <vector>

namespace app {

/**
 * background that changes colour. With british spelling.
 */
class pulse_background
	:public background_interface {

	public:
	                        pulse_background();
	virtual                 ~pulse_background();

	//Implementation of background_interface
	void                    draw_background(ldv::screen&);
	void                    draw_foreground(ldv::screen&);
	void                    tic(tdelta);
	int                     get_sound() const {return 0;}

	private:

	void                    ready_tweeners();

	//This needs to persist between level changes and I am going to take the 
	//easy route, so there, static properties.
	static std::size_t      current_colour_index; //This is the current index for colours;
	static float            r,g,b; //components for a rbga_color.
	static double           timer_remains; //remainder of the clock when this gets destroyed
	//List of colours this will go through.
	std::vector             <ldv::rgba_color> colours;

	tools::tween::tweener<float>    tweener_r,
	                        tweener_g,
	                        tweener_b;
	tools::tween::interpolators::ease_in_out_sine interpolator;

	d2d::components::timeout counter;
};
}
