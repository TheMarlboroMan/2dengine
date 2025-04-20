#pragma once

#include <d2d/motion/definitions.h>
#include <d2d/collision/definitions.h>
#include <d2d/components/timeouts.h>
#include <ldtools/time_definitions.h>
#include <ostream>

namespace app {

/**
 * does not actually generate the projectile, but the data that will be used
 * to create it.
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
		bool
	);

	int                     get_tag() const {return tag;}

	types                   get_type() const {return type;}
/**
*Returns true if must generate a projectile.
*/
	bool                    tic(ldtools::tdelta _delta);

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
	bool                    active;
	types                   type;
	int                     tag;

	const int               volley_total;
	int                     volley_count{0};
	d2d::components::timeouts   timeouts;

	//TODO; Why should these be in the executable when we can have a table
	//for these kind of things? They would not be const, I guess...
	static const int        projectile_horizontal_w{7};
	static const int        projectile_horizontal_h{7};
	static const int        projectile_horizontal_desintegration_ms{300};

	static const int        projectile_vertical_w{10};
	static const int        projectile_vertical_h{10};
	static const int        projectile_vertical_desintegration_ms{530};

	static const int        projectile_round_w{6};
	static const int        projectile_round_h{6};
	static const int        projectile_round_desintegration_ms{300};

	static const int        projectile_falling_w{6};
	static const int        projectile_falling_h{6};
	static const int        projectile_falling_desintegration_ms{300};

	friend std::ostream& operator<<(std::ostream&, const projectile_generator&);
};

std::ostream& operator<<(std::ostream&, const projectile_generator&);
}
