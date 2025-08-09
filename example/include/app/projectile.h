#pragma once

#include "entity.h"
#include <d2d/motion/definitions.h>
#include <d2d/motion/mover.h>
#include <d2d/components/timeout.h>
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
		types, 
		ldtools::tdelta
	);

	entity                  ent;

	bool                    is_moving() const {return state==states::moving;}
	bool                    is_desintegrating() const {return state==states::desintegrating;}
	bool                    is_done() const {return state==states::done;}

	void                    tic(ldtools::tdelta _delta, d2d::motion::mover);
	void                    desintegrate();
	void                    finish();

	ldtools::tdelta         get_timeout_value() const {return timeout.get();}
	types                   get_type() const {return type;}

	private:

	enum class states {
		moving,
		desintegrating,
		done
	}                       state{states::moving};
	
	types                       type;
	d2d::components::timeout    timeout;
};
}
