#pragma once

#include <d2d/motion/definitions.h>
#include <d2d/collision/definitions.h>
#include <d2d/components/timeouts.h>
#include <ostream>

namespace app {

class projectile_generator {

	public:

/**
 * indicates the direction projectiles will spawn in. The box means that
 * projectiles will spawn centered on it
 */
	struct projectile_data {
		d2d::motion::motion_vector  velocity;
		d2d::collision::box         box;
	};

							projectile_generator(
		d2d::collision::box,
		int,    //velocity
		int,	//tag
		int,    //volley count
		int,    //pause_ms
		int,    //rest_ms
		bool
	);

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
	projectile_data         get_projectile_data() const {

		return pr_data;
	}

	private:

	enum timeouts {
		timeout_volley,
		timeout_rest
	};

	enum class states {
		volley,
		rest
	}                       state;

	bool                    active;
	int                     tag;

	const int               volley_total;
	int                     volley_count{0};
	d2d::components::timeouts   timeouts;

	projectile_data         pr_data;
};

std::ostream& operator<<(std::ostream&, const projectile_generator&);
}
