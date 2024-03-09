#include "app/projectile.h"
#include <iostream>

using namespace app;

projectile::projectile(
	d2d::collision::point _pt, 
	d2d::motion::motion_vector _velocity
):
	ent{_pt, projectile_w, projectile_h},
	velocity{_velocity},
	timeout{0.3f, 0.0f, true}
{};

void projectile::tic(
	float _delta,
	d2d::motion::mover _mover
) {

	timeout.tic(_delta);

	if(is_desintegrating()) {

		if(timeout.is_expired()) {

			finish();
		}

		return;
	}

	if(is_moving()) {

		_mover.apply_x(ent, velocity.x, _delta);
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

