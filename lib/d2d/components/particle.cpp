#include "d2d/components/particle.h"
#include <stdexcept>

#include <iostream>

using namespace d2d::components;

particle_manager::particle_manager(
	std::size_t _size
)
	:particles{_size, particle{}}
{

	if(0==_size) {

		throw std::runtime_error("particle_manager must have at least one particle!");
	}

	for(std::size_t i=0; i<_size; ++i) {

		particles[i].id=i;
	}
}

bool particle_manager::add(
	int _type,
	d2d::collision::point _pos
) {

	//We can fill no more particles here.
	if(last_free_index >= particles.size()) {

		return false;
	}

	particles[last_free_index].type=_type;
	particles[last_free_index].lifetime=0.;
	auto particle_id=particles[last_free_index].id;

	modules[_type]->add(particles[last_free_index], _pos, particle_id);
	if(renderers[_type]->must_subscribe(particles[last_free_index])) {

		renderers[_type]->subscribe(particles[last_free_index], particle_id);
	}
	++last_free_index;

	return true;
}

void particle_manager::tic(
	ldtools::tdelta _delta
) {

	for(std::size_t i=0; i < last_free_index;) {

		auto& particle=particles[i];
		auto type=particle.type;
		auto particle_id=particles[i].id;
		particle.lifetime+=_delta;

		if(particle.is_done()) {

			if(renderers[type]->must_subscribe(particle)) {

				renderers[type]->expire(particle, particle_id);
			}

			modules[type]->expire(particle, particle_id);

			//Is it at the tail? Free the tail...
			if(i==last_free_index-1) {

				--last_free_index;
				continue;
			}

			//Swap this one for the one at the tail...
			std::swap(particles[i], particles[last_free_index-1]);
			--last_free_index;
			continue;
		}

		modules[type]->tic(particle, _delta, particle_id);
		++i;
	}
}

void particle_manager::draw(
	ldv::screen& _screen
) const {

	for(std::size_t i=0; i < last_free_index; i++) {

		const auto& particle=particles[i];
		renderers[particle.type]->draw(particle, _screen, particle.id);
	}
}

void particle_manager::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera
) const {

	for(std::size_t i=0; i < last_free_index; i++) {

		const auto& particle=particles[i];
		renderers[particle.type]->draw(particle, _screen, _camera, particle.id);
	}
}

void particle_manager::clear() {

	last_free_index=0;
}

std::size_t particle_manager::register_module(
	particle_module_interface& _module
) {

	modules.push_back(&_module);
	return modules.size() -1;
}

std::size_t particle_manager::register_renderer(
	particle_render_interface& _module
) {

	renderers.push_back(&_module);
	return renderers.size() -1;
}

void particle_manager::reset() {

	clear();
	modules.clear();
	renderers.clear();
}

