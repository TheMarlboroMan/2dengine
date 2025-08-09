#include "d2d/components/particle.h"

using namespace d2d::components;

particle_manager::particle_manager(
	std::size_t _size
)
	:particles{_size, particle{ {0., 0.}, {0., 0.}, 0., 0}}
{

}

bool particle_manager::add(
	int _type,
	d2d::collision::point _at
) {

	if(particles.size()==(last_free_index-1)) {

		return false;
	}
	
	//TODO: does the type exist as a module?
	
	particles[last_free_index].type=_type;
	modules.at(_type)->add(particles[last_free_index], _at);
	++last_free_index;
	return true;
}

void particle_manager::tic(
	ldtools::tdelta _delta
) {

	for(std::size_t i=0; i < last_free_index;) {

		auto& particle=particles[i];
		particle.lifetime-=_delta;

		if(particle.lifetime <= 0.) {

			//Is it at the tail?
			if(i==last_free_index-1) {

				--last_free_index;
				continue;
			}

			std::swap(particles[i], particles[last_free_index-1]);
			--last_free_index;
			continue;
		}

		modules.at(particle.type)->tic(particle, _delta);
		++i;
	}
}

void particle_manager::draw(
	ldv::screen& _screen
) {

	for(std::size_t i=0; i < last_free_index; i++) {

		auto& particle=particles[i];
		modules.at(particle.type)->draw(particle, _screen);
	}
}

void particle_manager::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera
) {

	for(std::size_t i=0; i < last_free_index; i++) {

		auto& particle=particles[i];
		modules.at(particle.type)->draw(particle, _screen, _camera);
	}
}

void particle_manager::cleanup() {

	last_free_index=0;
}

void particle_manager::register_module(
	int _type,
	particle_module_interface& _module
) {

	modules[_type]=&_module;
}


