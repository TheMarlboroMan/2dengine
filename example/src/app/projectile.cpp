#include "app/projectile.h"
#include <d2d/motion/accelerator.h>
#include <iostream>
#include <algorithm>

using namespace app;

projectile::projectile(
	d2d::collision::box _box, 
	d2d::motion::motion_vector _velocity,
	types _type
):
	ent{_box},
	type{_type}
{
	ent.set_motion_vector(_velocity);
}

void projectile::tic(
	ldtools::tdelta _delta,
	d2d::motion::mover _mover
) {

	if(is_moving()) {

		_mover.apply(ent, ent.get_motion_vector(), _delta);

		switch(type) {

			//falling ones move faster as they go. We could have a non-linear
			//thing, but this will do.
			case types::falling: {

				d2d::motion::accelerator ac{{0.0, 600.0}};
				auto velocity=ent.get_motion_vector();
				ac.accelerate_y(velocity, 0.05*velocity.y);
				ent.set_motion_vector(velocity);
			}
			break;
			//Flames rising...
			case types::vertical:{

				auto velocity=ent.get_motion_vector();
				auto factor=std::max(0.4, (velocity.y * 0.02));

				d2d::motion::accelerator ac{{0.0, 0.0}};
				ac.decelerate_y(velocity, factor);
				ent.set_motion_vector(velocity);
				if(velocity.y <= 0.1) {

					//TODO: Must be something else, actually.
					//desintegrate();
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

