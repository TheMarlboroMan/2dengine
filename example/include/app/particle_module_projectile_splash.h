#pragma once

#include "d2d/components/particle.h"

using d2d::components::particle;
using d2d::components::particle_module_interface;
using ldtools::tdelta;

namespace app {

/**
 * A splash of a vertical projectile.
 */
class particle_module_projectile_splash
	:public particle_module_interface {

	public:

	void    add(particle&, d2d::collision::point);
	void    tic(particle&, tdelta);

	private: 

	unsigned added{0};
};

}
