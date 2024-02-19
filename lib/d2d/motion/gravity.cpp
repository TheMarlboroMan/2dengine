#include <d2d/motion/gravity.h>
#include <iostream>

using namespace d2d::motion;

gravity::gravity():
	force{0.0, 0.0},
	max_velocity{0.0}
{}

gravity::gravity(
	const d2d::motion::motion_vector& _force,
	double _max_velocity
):
	force{_force},
	max_velocity{_max_velocity}
{}

void gravity::apply_to(
	d2d::motion::motion_vector& _target, 
	float _delta
) {

	_target+=force*_delta;

	if(_target.y < max_velocity) {

		_target.y=max_velocity; //maximum velocity!
	}
}
