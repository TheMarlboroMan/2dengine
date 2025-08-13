#include "app/particle_module_projectile_horizontal_splash.h"

using namespace app;

particle_module_projectile_horizontal_splash::particle_module_projectile_horizontal_splash(
	random& _rand
):rand{_rand}
{}


void particle_module_projectile_horizontal_splash::add(
	particle& _particle,
	d2d::collision::point _origin,
	particle_index
) {

	double lifetime=rand.get(30, 70);
	_particle.max_lifetime=lifetime/100.;

	double vx=rand.get(20, 80);
	double vy=rand.get(0, 60);
	_particle.pos=_origin;
	_particle.vector={vx*multiplier_x, vy-30.};
}

void particle_module_projectile_horizontal_splash::tic(
	particle& _particle,
	ldtools::tdelta _delta,
	particle_index
) {

	//Slowly brake.
	_particle.vector-=_particle.vector * _delta;

	auto& pos=_particle.pos;
	pos.x+=_particle.vector.x * _delta;
	pos.y+=_particle.vector.y * _delta;
}

void particle_module_projectile_horizontal_splash::expire(
	particle&,
	particle_index
) {}

