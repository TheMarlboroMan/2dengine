#include "app/particle_module_bonus.h"
#include "app/definitions.h"

using namespace app;

particle_module_bonus::particle_module_bonus(
	random& _rand
): rand{_rand}
{}

void particle_module_bonus::add(
	particle& _particle,
	d2d::collision::point _origin,
	particle_index
) {

	double lifetime=rand.get(30, 90);
	_particle.max_lifetime=lifetime/100.;

	double vy=rand.get(30, 60);
	double x=rand.get(0, 10)-5;

	_origin.x+=x;
	_particle.pos=_origin;
	_particle.vector={0., vy};
}

void particle_module_bonus::tic(
	particle& _particle,
	ldtools::tdelta _delta,
	particle_index
) {

	_particle.pos.y+=_particle.vector.y * _delta;
}

void particle_module_bonus::expire(
	particle&,
	particle_index
) {}

