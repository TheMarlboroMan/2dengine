#pragma once

#include "random.h"
#include "d2d/components/particle.h"

using d2d::components::particle;
using d2d::components::particle_module_interface;
using d2d::components::particle_index;
using ldtools::tdelta;

namespace app {

/**
 *Splash from the horizontal projectile.
 */
class particle_module_projectile_horizontal_splash
	:public particle_module_interface {

	public:

	        particle_module_projectile_horizontal_splash(random&);
	void    add(particle&, d2d::collision::point, particle_index);
	void    tic(particle&, tdelta, particle_index);
	void    expire(particle&, particle_index);

	void    set_multiplier_x(double _val) {multiplier_x=_val;}

	private:

	random& rand;
	double  multiplier_x{1.};
};

}
