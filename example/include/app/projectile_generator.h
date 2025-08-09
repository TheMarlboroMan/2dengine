#pragma once

#include <d2d/motion/definitions.h>
#include <d2d/collision/definitions.h>
#include <d2d/components/timeout.h>
#include <ldtools/time_definitions.h>
#include <ostream>

namespace app {

/**
 * does not actually generate the projectile, but the data that will be used
 * to create it. It is part of the game world.
 */
class projectile_generator {


	public:

	//types of projectile that this generates...
	enum class types {
		horizontal, //horizontal
		vertical, //vertical flame column
		directed,
		falling
	};

/**
 * indicates the direction projectiles will spawn in. The box means that
 * projectiles will spawn exactly on that position, with their aligment 
 * already calculated.
 */
	struct projectile_data {
		d2d::motion::motion_vector  velocity;
		d2d::collision::box         box; 
		ldtools::tdelta             desintegration_time;
		types                       type;
	};

							projectile_generator(
		d2d::collision::point,
		types,
		int,    //velocity
		int,	//tag
		int,    //volley count
		int,    //pre_ms
		int,    //pause_ms
		int,    //rest_ms
		bool,   //active
		bool    //keep active when restart
	);

	int                     get_tag() const {return tag;}

	types                   get_type() const {return type;}

	int                     get_velocity() const {return velocity;}

	d2d::collision::point   get_spawn_point() const {return spawn_point;}
/**
*Returns true if must generate a projectile.
*/
	bool                    tic(ldtools::tdelta _delta);

	void                    toggle();

	void                    reset();

	private:

/**
 * activates the generator, resets to the first volley shot, in pause state.
 */
	void                    activate();

/**
 * deactivates the generator
 */
	void                    deactivate();


	enum timeouts {
		timeout_pre=0,
		timeout_volley=1,
		timeout_rest=2
	};

	enum class states {
		pre,
		fire,
		volley_pause,
		rest
	};

	const d2d::collision::point    spawn_point;
	states                  state;
	const float             velocity;
	bool                    active,
	                        starting_active,
	                        keep_active_when_reset;
	types                   type;
	int                     tag;

	const int               volley_total;
	int                     volley_count{0};
	d2d::components::timeouts   timeouts;

	friend std::ostream& operator<<(std::ostream&, const projectile_generator&);
};

std::ostream& operator<<(std::ostream&, const projectile_generator&);
}
