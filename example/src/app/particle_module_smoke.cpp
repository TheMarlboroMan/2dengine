#include "app/particle_module_smoke.h"
#include "app/definitions.h"

using namespace app;

particle_module_smoke::particle_module_smoke(
	random& _rand
): rand{_rand}
{}

void particle_module_smoke::add(
	particle& _particle,
	d2d::collision::point _origin,
	particle_index
) {

	double lifetime=rand.get(130, 210);
	_particle.max_lifetime=lifetime/100.;

	double vy=rand.get(0, 4);
	double vx=rand.get(0, 120)-60;

	_particle.pos=_origin;
	_particle.vector={vx, vy};
}

void particle_module_smoke::tic(
	particle& _particle,
	ldtools::tdelta _delta,
	particle_index
) {

	_particle.pos.x+=_particle.vector.x * _delta;
	_particle.pos.y+=_particle.vector.y * _delta;

	if(0.5 <= _particle.lifetime / _particle.max_lifetime) {
	
		_particle.vector.x /= 1.0001;
	}
}

void particle_module_smoke::expire(
	particle&,
	particle_index
) {}

