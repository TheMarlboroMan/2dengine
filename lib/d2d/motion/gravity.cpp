#include <d2d/motion/gravity.h>
#include <d2d/collision/spatiable.h>
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
	ldtools::tdelta _delta
) {

	_target+=force*_delta;

	if(_target.y < max_velocity) {

		_target.y=max_velocity; //maximum velocity!
	}
}


void gravity::apply_to(
	d2d::collision::spatiable& _spatiable,
	ldtools::tdelta _delta
) {

	auto target=_spatiable.get_motion_vector();
	target+=force*_delta;

	if(target.y < max_velocity) {

		target.y=max_velocity; //maximum velocity!
	}

	_spatiable.set_motion_vector(target);
}
