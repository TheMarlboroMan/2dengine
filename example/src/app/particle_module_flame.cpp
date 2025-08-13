#include "app/particle_module_flame.h"

using namespace app;

void particle_module_flame::add(
	particle& _particle,
	d2d::collision::point _origin,
	particle_index
) {

	//The position does change a bit, not randomly mind you, but with each
	//new particle created.
	switch(added % 7) {
		case 0: _origin.x+=2; break;
		case 1: _origin.x-=2; break;
		case 2: _origin.x+=3; break;
		case 3: _origin.x-=1; break;
		case 4: _origin.x-=3; break;
		case 5: _origin.x+=1; break;
		case 6: break;
	};

	//Same as with lifetime!
	switch(added % 5) {
		case 0: _particle.max_lifetime=1.5; break;
		case 1: _particle.max_lifetime=1.1; break;
		case 2: _particle.max_lifetime=0.9; break;
		case 3: _particle.max_lifetime=1.2; break;
		case 4: _particle.max_lifetime=0.8; break;
	}

	++added;

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

