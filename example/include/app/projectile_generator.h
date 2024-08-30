#pragma once

#include <d2d/motion/definitions.h>
#include <d2d/collision/definitions.h>
#include <d2d/components/timeouts.h>
#include <ostream>

namespace app {

class projectile_generator {


	public:

	//types of projectile that this generates...
	enum class types {
		linear, //horizontal
		vertical, //vertical flame column
		directed,
		falling
	};

/**
 * indicates the direction projectiles will spawn in. The box means that
 * projectiles will spawn centered on it
 */
	struct projectile_data {
		d2d::motion::motion_vector  velocity;
		d2d::collision::point         point;
	};

							projectile_generator(
		d2d::collision::point,
		types,
		int,    //velocity
		int,	//tag
		int,    //volley count
		int,    //pause_ms
		int,    //rest_ms
		bool
	);

	int                     get_tag() const {return tag;}

	types                   get_type() const {return type;}
/**
*Returns true if must generate a projectile.
*/
	bool                    tic(float _delta);

/**
 * activates the generator, resets to the first volley shot, in pause state.
 */
	void                    activate();

/**
 * deactivates the generator
 */
	void                    deactivate();

/**
 * must return the information about the projectile it will generate
 */
	projectile_data         get_projectile_data() const;

	private:

	enum timeouts {
		timeout_volley,
		timeout_rest
	};

	enum class states {
		volley,
		rest
	}                       state;

	const d2d::collision::point    spawn_point;
	const float             velocity;
	bool                    active;
	types                   type;
	int                     tag;

	const int               volley_total;
	int                     volley_count{0};
	d2d::components::timeouts   timeouts;
};

std::ostream& operator<<(std::ostream&, const projectile_generator&);
}
