#include "app/projectile.h"
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
	timeout.restart();
}

