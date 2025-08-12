#pragma once

#include "d2d/components/particle.h"

using d2d::components::particle;
using d2d::components::particle_module_interface;
using ldtools::tdelta;

namespace app {

/**
 *Just a bunch of flames in place.
 */
class particle_module_flame
	:public particle_module_interface {

	public:

	void    add(particle&, d2d::collision::point);
	void    tic(particle&, tdelta);

	private: 

	unsigned added{0};
};

}
