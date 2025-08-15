#include "app/particle_module_flame.h"

using namespace app;

void particle_module_flame::add(
	particle& _particle,
	d2d::collision::point _origin,
	particle_index
) {

	//The lifetime matches the boss timeout for firing, and a bit more.
	_particle.max_lifetime=4.0;
	_particle.pos=_origin;
	_particle.vector={0., 0.};
}

void particle_module_flame::tic(
	particle&,
	ldtools::tdelta,
	particle_index
) { }

void particle_module_flame::expire(
	particle&,
	particle_index
) {}

