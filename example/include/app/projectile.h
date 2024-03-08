#pragma once

#include "entity.h"
#include <d2d/motion/definitions.h>
#include <d2d/motion/mover.h>

namespace app {

class projectile {

	public:

	                        projectile(d2d::collision::point, d2d::motion::motion_vector);

	entity                  ent;
	d2d::motion::motion_vector velocity;
	bool                    desintegrating{false};

	void                    tic(float _delta, d2d::motion::mover);
	void                    desintegrate();

	private:

	static const int        projectile_w{16};
	static const int        projectile_h{9};
};
}
