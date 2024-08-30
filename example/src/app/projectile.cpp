#include "app/projectile.h"
#include <iostream>
#include <algorithm>

using namespace app;

projectile::projectile(
	d2d::collision::point _pt, 
	d2d::motion::motion_vector _velocity,
	types _type
):
	ent{_pt, 0, 0},
	velocity{_velocity},
	type{_type},
	timeout{0.1f, 0.0f, true}
{
	
	switch(type) {

		case types::horizontal:

			ent.set_w(projectile_horizontal_w);
			ent.set_h(projectile_horizontal_h);
		break;
		case types::vertical:

			ent.set_w(projectile_vertical_w);
			ent.set_h(projectile_vertical_h);
		break;
		case types::directed:

			ent.set_w(projectile_round_w);
			ent.set_h(projectile_round_h);
		break;
		case types::falling:

			ent.set_w(projectile_falling_w);
			ent.set_h(projectile_falling_h);
		break;
	}
}

void projectile::tic(
	float _delta,
	d2d::motion::mover _mover
) {

	if(is_desintegrating()) {

		timeout.tic(_delta);
		if(timeout.is_finished()) {

			finish();
		}

		return;
	}

	if(is_moving()) {

		_mover.apply(ent, velocity, _delta);

		switch(type) {

			//falling ones move faster as they go.
			//TODO: I wish we could have an accelerator thingy
			case types::falling:
				velocity.y+=(velocity.y * 0.05);
			break;
			case types::vertical:{

				auto factor=std::max(0.4, (velocity.y * 0.02));

				velocity.y-=factor;
				if(velocity.y <= 0.1) {

					desintegrate();
				}
			}
			break;
			case types::directed:
			case types::horizontal:
			break;
		}
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
	//TODO: Should set its length according to type...
	timeout.restart();
}

