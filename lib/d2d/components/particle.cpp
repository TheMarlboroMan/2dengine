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
	d2d::collision::point _pos
) {

	if(particles.size()==(last_free_index-1)) {

		return false;
	}

	particles[last_free_index].type=_type;
	modules[_type]->add(particles[last_free_index], _pos);
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

		modules[particle.type]->tic(particle, _delta);
		++i;
	}
}

void particle_manager::draw(
	ldv::screen& _screen
) {

	for(std::size_t i=0; i < last_free_index; i++) {

		auto& particle=particles[i];
		renderers[particle.type]->draw(particle, _screen);
	}
}

void particle_manager::draw(
	ldv::screen& _screen,
	const ldv::camera& _camera
) {

	for(std::size_t i=0; i < last_free_index; i++) {

		auto& particle=particles[i];
		renderers[particle.type]->draw(particle, _screen, _camera);
	}
}

void particle_manager::cleanup() {

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

	cleanup();
	modules.clear();
	renderers.clear();
}


