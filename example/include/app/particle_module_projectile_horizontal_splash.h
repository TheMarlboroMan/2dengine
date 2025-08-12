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
class particle_module_projectile_horizontal_splash
	:public particle_module_interface {

	public:


	        particle_module_projectile_horizontal_splash();
	void    add(particle&, d2d::collision::point);
	void    tic(particle&, tdelta);
	void    set_multiplier_x(double _val) {multiplier_x=_val;}

	private:

	int     rand(int, int);
	double  multiplier_x{1.};
	tools::int_generator numgen;
};

}
