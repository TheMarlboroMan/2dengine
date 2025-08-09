#pragma once

#include "d2d/collision/definitions.h"
#include <ldtools/time_definitions.h>
#include <ldv/screen.h>
#include <ldv/camera.h>
#include <map>
#include <vector>

namespace d2d { namespace components {

/**
 * the particle itself. Position and vector are expected in cartesian 
 * coordinates, but that's neither here nor there since the particle modules
 * will actually take care of drawing.
 */
struct particle {

	d2d::collision::point           pos;
	d2d::collision::motion_vector   vector;
	ldtools::tdelta                 lifetime;
	int                             type;

};

/**
 * extension endpoints to applications can create and manage their particles.
 */
class particle_module_interface {

	public:

	//Must write into the particle so that it behaves as the module desires.
	virtual void    add(particle&, d2d::collision::point)=0;

	//Must tic the given particle. Must not tic its lifetime, as the manager
	//should have already.
	virtual void    tic(particle&, ldtools::tdelta)=0;

	//Must draw the given particle.
	virtual void    draw(particle&, ldv::screen&)=0;

	//Must draw the given particle.
	virtual void    draw(particle&, ldv::screen&, const ldv::camera&)=0;

};

/**
 * manager of the particles. Contains and owns them, but does not contain the
 * modules that will actually create and move the particles around.
 */
class particle_manager {

	public:

	//!Builds the particle manager with space for N particles.
	                particle_manager(std::size_t);
	//!Adds a particle of the type N at the given point. The module registered
	//!at N is the one that will decide its velocity and lifetime! Returns
	//!false if the container is full and nothing could be added!
	bool            add(int, d2d::collision::point);
	//!Returns the size of the container.
	std::size_t     size() const {return last_free_index;}
	//!Will tic every particle.
	void            tic(ldtools::tdelta);
	//!Draws all particles to the screen.
	void            draw(ldv::screen&);
	//!Draws all particles to the screen with the given camera.
	void            draw(ldv::screen&, const ldv::camera&);
	//!Resets the manager. The container is not actually touched at all.
	void            cleanup();
	//!Registers the module unter the given id. The module MUST outlive
	//!the manager!
	void            register_module(int, particle_module_interface&);

#ifdef IS_DEBUG_BUILD
	particle&       get(std::size_t _index) {

		return particles.at(_index);
	}
#endif

	private:

	std::size_t                         last_free_index{0};
	std::vector<particle>               particles;
	//The manager DOES NOT OWN the modules.
	std::map<int, particle_module_interface*>    modules;
};

}}


