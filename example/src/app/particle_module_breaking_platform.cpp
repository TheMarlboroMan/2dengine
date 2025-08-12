#include "app/particle_module_breaking_platform.h"
#include "app/definitions.h"

using namespace app;

particle_module_breaking_platform::particle_module_breaking_platform() 
	:numgen{0, 255}
{}


void particle_module_breaking_platform::add(
	particle& _particle,
	d2d::collision::point _origin
) {

	double lifetime=rand(30, 90);
	_particle.max_lifetime=lifetime/100.;

	double vy=rand(10, 40);
	double x=rand(0, app::tile_w);

	_particle.pos=_origin+x;
	_particle.vector={0., -vy};
}

void particle_module_breaking_platform::tic(
	particle& _particle,
	ldtools::tdelta _delta
) {

	auto& pos=_particle.pos;
	pos.y+=_particle.vector.y * _delta;
}

//TODO: So duplicate xD
int particle_module_breaking_platform::rand(
	int _min,
	int _max
) {

	if(_min==_max) {

		return _max;
	}
	int res=numgen() % (_max-_min);
	return res+_min;

}
