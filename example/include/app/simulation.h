#pragma once

#include <d2d/motion/gravity.h>

namespace app {

/**
 * a container for physics simulation values
 */
struct simulation {

	d2d::motion::gravity        gravity{{0., -500}, {-600}};
	double                      jump_force{225.0},
	                            walk_max_velocity{100.},
	                            ladder_max_velocity{60.},
	                            air_x_velocity_reduce_factor{1.02}, //speed reduction when no horizontal input on air.
	                            air_x_velocity_acceleration_value{2.0}, //acceleration when horizontal input on air.
	                            air_x_velocity_collision_reduce_factor{2.}, //speed reduction when a X collision is detected in air.
	                            air_y_velocity_jump_shorten_factor{2.}; //speed reduction when the jump input is let go of.

};
}
