#pragma once

#include "d2d/components/particle.h"
#include <tools/number_generator.h>

using d2d::components::particle;
using d2d::components::particle_module_interface;
using ldtools::tdelta;

namespace app {

/**
 *Splash from the horizontal projectile.
 */
class particle_module_breaking_platform
	:public particle_module_interface {

	public:


	        particle_module_breaking_platform();
	void    add(particle&, d2d::collision::point);
	void    tic(particle&, tdelta);

	private:

	int     rand(int, int);
	tools::int_generator numgen;
};

}
