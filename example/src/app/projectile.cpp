#include "app/projectile.h"
#include <d2d/motion/accelerator.h>
#include <iostream>
#include <algorithm>

using namespace app;

projectile::projectile(
	d2d::collision::box _box, 
	d2d::motion::motion_vector _velocity,
	types _type,
	float _desintegration_ms
):
	ent{_box},
	velocity{_velocity},
	type{_type},
	timeout{_desintegration_ms / 1000.f, 0.0f, true}
{ }

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

			//falling ones move faster as they go. We could have a non-linear
			//thing, but this will do.
			case types::falling: {

				d2d::motion::accelerator ac{{0.0, 600.0}};
				ac.accelerate_y(velocity, 0.05*velocity.y);
			}
			break;
			//Flames rising...
			case types::vertical:{

				auto factor=std::max(0.4, (velocity.y * 0.02));

				d2d::motion::accelerator ac{{0.0, 0.0}};
				ac.decelerate_y(velocity, factor);
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

	//we don't set velocity to zerp because this would make the whole animation 
	//impossible to orient (velocity is used for that)
	state=states::desintegrating;
	timeout.restart();
}

