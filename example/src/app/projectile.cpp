#include "app/projectile.h"

using namespace app;

projectile::projectile(
	d2d::collision::point _pt, 
	d2d::motion::motion_vector _velocity
):
	ent{_pt, projectile_w, projectile_h},
	velocity{_velocity}
{};

void projectile::tic(
	float _delta,
	d2d::motion::mover _mover
) {

	if(desintegrating) {

		return;
	}

	_mover.apply_x(ent, velocity.x, _delta);
}

void projectile::desintegrate() {

	//this would make the whole animation impossible to orient, we use the
	//velocity for that.
	//velocity={0.0, 0.0};
	desintegrating=true;
}

