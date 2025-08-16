#pragma once

#include "random.h"
#include "d2d/components/particle.h"

using d2d::components::particle;
using d2d::components::particle_module_interface;
using d2d::components::particle_index;
using ldtools::tdelta;

namespace app {

/**
 * Glitter that goes exploding around when the player picks up a bonus.
 */
class particle_module_pickup
	:public particle_module_interface {

	public:


	        particle_module_pickup(random&);
	void    add(particle&, d2d::collision::point, particle_index);
	void    tic(particle&, tdelta, particle_index);
	void    expire(particle&, particle_index);

	private:

	random& rand;
};

}
