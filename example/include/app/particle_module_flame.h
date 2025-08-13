#pragma once

#include "d2d/components/particle.h"

using d2d::components::particle;
using d2d::components::particle_module_interface;
using d2d::components::particle_index;
using ldtools::tdelta;

namespace app {

/**
 *Just a bunch of flames in place.
 */
class particle_module_flame
	:public particle_module_interface {

	public:

	void    add(particle&, d2d::collision::point, particle_index);
	void    tic(particle&, tdelta, particle_index);
	void    expire(particle&, particle_index);

	private: 

	unsigned added{0};
};

}
