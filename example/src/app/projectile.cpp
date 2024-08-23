#include "app/projectile.h"
#include <iostream>

using namespace app;

projectile::projectile(
	d2d::collision::point _pt, 
	d2d::motion::motion_vector _velocity,
	types _type
):
	ent{_pt, 0, 0},
	velocity{_velocity},
	type{_type},
	timeout{0.3f, 0.0f, true}
{
	
	switch(type) {

		case types::horizontal:

			ent.set_w(projectile_horizontal_w);
			ent.set_h(projectile_horizontal_h);
		break;
		case types::round:

			ent.set_w(projectile_round_w);
			ent.set_h(projectile_round_h);
		break;
	}
}

void projectile::tic(
	float _delta,
	d2d::motion::mover _mover
) {

	timeout.tic(_delta);

	if(is_desintegrating()) {

		if(timeout.is_finished()) {

			finish();
		}

		return;
	}

	if(is_moving()) {

		_mover.apply(ent, velocity, _delta);
		return;
	}
}

void projectile::finish() {

	state=states::done;
}

void projectile::desintegrate() {

	//this would make the whole animation impossible to orient, we use the
	//velocity for that.
	//velocity={0.0, 0.0};
	state=states::desintegrating;
	timeout.resume();
}

