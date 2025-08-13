#include "app/particle_module_projectile_splash.h"

using namespace app;

particle_module_projectile_splash::particle_module_projectile_splash(
	random& _rand
) :rand{_rand}
{}

void particle_module_projectile_splash::add(
	particle& _particle,
	d2d::collision::point _origin,
	particle_index
) {

	//Lifetime will change...
	double lifetime=rand.get(80, 150);
	_particle.max_lifetime=lifetime / 100.;

	double vx=rand.get(0, 30)-15.;
	double vy=rand.get(45, 110);

	_particle.pos=_origin;
	_particle.vector={vx, vy};
}

void particle_module_projectile_splash::tic(
	particle& _particle,
	ldtools::tdelta _delta,
	particle_index
) {
	
	//Just follow the vector and apply rudimentary fake gravity
	_particle.vector.y-=200. * _delta;

	auto& pos=_particle.pos;
	pos.x+=_particle.vector.x * _delta;
	pos.y+=_particle.vector.y * _delta;
}

void particle_module_projectile_splash::expire(
	particle&,
	particle_index
) {}
