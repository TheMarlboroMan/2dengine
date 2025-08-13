#include "app/particle_module_breaking_platform.h"
#include "app/definitions.h"

using namespace app;

particle_module_breaking_platform::particle_module_breaking_platform(
	random& _rand
): rand{_rand}
{}

void particle_module_breaking_platform::add(
	particle& _particle,
	d2d::collision::point _origin,
	particle_index
) {

	double lifetime=rand.get(30, 90);
	_particle.max_lifetime=lifetime/100.;

	double vy=rand.get(40, 80);
	double x=rand.get(0, app::tile_w);

	_origin.x+=x;
	_particle.pos=_origin;
	_particle.vector={0., -vy};
}

void particle_module_breaking_platform::tic(
	particle& _particle,
	ldtools::tdelta _delta,
	particle_index
) {

	_particle.pos.y+=_particle.vector.y * _delta;
}

void particle_module_breaking_platform::expire(
	particle&,
	particle_index
) {}

