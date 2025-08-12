#include "app/particle_module_projectile_horizontal_splash.h"

using namespace app;

particle_module_projectile_horizontal_splash::particle_module_projectile_horizontal_splash() 
	:numgen{0, 255}
{}


void particle_module_projectile_horizontal_splash::add(
	particle& _particle,
	d2d::collision::point _origin
) {

	double lifetime=rand(30, 70);
	_particle.max_lifetime=lifetime/100.;

	double vx=rand(20, 80);
	double vy=rand(0, 60);
	_particle.pos=_origin;
	_particle.vector={vx*multiplier_x, vy-30.};
}

void particle_module_projectile_horizontal_splash::tic(
	particle& _particle,
	ldtools::tdelta _delta
) {

	//Slowly brake.
	_particle.vector-=_particle.vector * _delta;

	auto& pos=_particle.pos;
	pos.x+=_particle.vector.x * _delta;
	pos.y+=_particle.vector.y * _delta;
}

int particle_module_projectile_horizontal_splash::rand(
	int _min,
	int _max
) {

	if(_min==_max) {

		return _max;
	}
	int res=numgen() % (_max-_min);
	return res+_min;

}
