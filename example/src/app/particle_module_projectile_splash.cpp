#include "app/particle_module_projectile_splash.h"

using namespace app;

void particle_module_projectile_splash::add(
	particle& _particle,
	d2d::collision::point _origin
) {

	//Lifetime will change...

	switch(added % 5) {
		case 0: _particle.max_lifetime=1.5; break;
		case 1: _particle.max_lifetime=1.1; break;
		case 2: _particle.max_lifetime=0.9; break;
		case 3: _particle.max_lifetime=1.2; break;
		case 4: _particle.max_lifetime=0.8; break;
	}

	float vx=0.;
	float vy=200.;

	switch(added % 9) {
		case 0: vy=75.; break;
		case 1: vy=55.; break;
		case 2: vy=45; break;
		case 3: vy=60.; break;
		case 4: vy=110.; break;
		case 5: vy=80.; break;
		case 6: vy=95.; break;
		case 7: vy=70.; break;
		case 8: vy=90.; break;
	}

	switch(added % 4) {
		case 0: vx=15.; break;
		case 1: vx=-11.; break;
		case 2: vx=9.; break;
		case 3: vx=12.; break;
	}
	
	_particle.pos=_origin;
	_particle.vector={vx, vy};

	++added;
}

void particle_module_projectile_splash::tic(
	particle& _particle,
	ldtools::tdelta _delta
) {
	
	//Just follow the vector and apply rudimentary fake gravity
	_particle.vector.y-=200. * _delta;

	auto& pos=_particle.pos;
	pos.x+=_particle.vector.x * _delta;
	pos.y+=_particle.vector.y * _delta;
}
