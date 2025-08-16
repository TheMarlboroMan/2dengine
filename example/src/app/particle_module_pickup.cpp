#include "app/particle_module_pickup.h"
#include "app/definitions.h"

using namespace app;

particle_module_pickup::particle_module_pickup(
	random& _rand
): rand{_rand}
{}

void particle_module_pickup::add(
	particle& _particle,
	d2d::collision::point _origin,
	particle_index
) {

	double lifetime=rand.get(30, 90);
	_particle.max_lifetime=lifetime/100.;

	double vy=rand.get(0, 80)-40;
	double vx=rand.get(0, 80)-40;

	_particle.pos=_origin;
	_particle.vector={vx, vy};
}

void particle_module_pickup::tic(
	particle& _particle,
	ldtools::tdelta _delta,
	particle_index
) {

	_particle.pos.y+=_particle.vector.y * _delta;
	_particle.pos.x+=_particle.vector.x * _delta;
}

void particle_module_pickup::expire(
	particle&,
	particle_index
) {}

