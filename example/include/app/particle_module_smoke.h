#pragma once

#include "random.h"
#include "d2d/components/particle.h"

using d2d::components::particle;
using d2d::components::particle_module_interface;
using d2d::components::particle_index;
using ldtools::tdelta;

namespace app {

/**
 * Smoke that fizzles out when vertical projectiles are done.
 */
class particle_module_smoke
	:public particle_module_interface {

	public:

	        particle_module_smoke(random&);
	void    add(particle&, d2d::collision::point, particle_index);
	void    tic(particle&, tdelta, particle_index);
	void    expire(particle&, particle_index);

	private:

	random& rand;
};

}
