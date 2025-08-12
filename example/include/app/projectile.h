#pragma once

#include "entity.h"
#include <d2d/motion/definitions.h>
#include <d2d/motion/mover.h>
#include <ldtools/time_definitions.h>

namespace app {

/**
 *A projectile.
 */
class projectile {

	public:

	enum class types {
		horizontal,
		vertical,
		directed,
		falling
	};

	                        projectile(
		d2d::collision::box, 
		d2d::motion::motion_vector, 
		types
	);

	entity                  ent;

	bool                    is_moving() const {return state==states::moving;}
	bool                    is_done() const {return state==states::done;}

	void                    tic(ldtools::tdelta _delta, d2d::motion::mover);
	void                    finish();

	types                   get_type() const {return type;}

	private:

	enum class states {
		moving,
		done
	}                       state{states::moving};
	
	types                       type;
};
}
