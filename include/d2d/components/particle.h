#pragma once

#include "d2d/collision/definitions.h"
#include <ldtools/time_definitions.h>
#include <ldv/screen.h>
#include <ldv/camera.h>
#include <vector>

namespace d2d { namespace components {

/**
 * the particle itself. Position and vector are expected in cartesian 
 * coordinates, but that's neither here nor there since the manager will
 * not take care of drawing... nor will the modules.
 */
struct particle {

	d2d::collision::point           pos;
	d2d::collision::motion_vector   vector;
	ldtools::tdelta                 lifetime;
	int                             type;

};

/**
 * extension endpoints to applications can create and manage their particles.
 * Each one can setup and tic particles in a different way, say, to 
 * simulate smoke, sparks...
 */
class particle_module_interface {

	public:

	//Must write into the particle so that it behaves as the module desires.
	virtual void    add(particle&, d2d::collision::point)=0;

	//Must tic the given particle. Must not tic its lifetime, as the manager
	//should have already.
	virtual void    tic(particle&, ldtools::tdelta)=0;

};

/**
 * Interface for a class that takes care of drawing particles. Each render
 * is expected to take care of a single type of particle. 
 */
class particle_render_interface {

	public:

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
	//!at N is the one that will decide its velocity, position and lifetime! 
	//Returns false if the container is full and nothing could be added!. Makes 
	//no attempt at checking if a module was registered or not.
	bool            add(int, d2d::collision::point);
	//!Returns the size of the container.
	std::size_t     size() const {return last_free_index;}
	//!Will tic every particle using its module.
	void            tic(ldtools::tdelta);
	//!Draws all particles.
	void            draw(ldv::screen&);
	//!Draws all particles to the screen with the given camera.
	void            draw(ldv::screen&, const ldv::camera&);
	//!Resets the particles. The container is not actually touched at all.
	void            cleanup();
	//!Resets the manager completely: particles and modules.
	void            reset();
	//!Registers the module. The module MUST outlive the manager!. Returns
	//the id that was used to register it.
	std::size_t     register_module(particle_module_interface&);
	//!Same as register module, but with renderers. Makes sense to register
	//!them in the same order as modules!
	std::size_t     register_renderer(particle_render_interface&);

	//!Direct particle access...
	particle&       get(std::size_t _index) {return particles.at(_index);}

	private:

	std::size_t                         last_free_index{0};
	std::vector<particle>               particles;
	//The manager DOES NOT OWN the modules.
	std::vector<particle_module_interface*>    modules;
	std::vector<particle_render_interface*>    renderers;
};

}}


