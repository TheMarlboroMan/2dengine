#pragma once

#include "entity.h"
#include <d2d/motion/definitions.h>
#include <d2d/motion/mover.h>
#include <d2d/components/timeouts.h>

namespace app {

class projectile {

	public:

	                        projectile(d2d::collision::point, d2d::motion::motion_vector);

	entity                  ent;
	d2d::motion::motion_vector velocity;

	bool                    is_moving() const {return state==states::moving;}
	bool                    is_desintegrating() const {return state==states::desintegrating;}
	bool                    is_done() const {return state==states::done;}

	void                    tic(float _delta, d2d::motion::mover);
	void                    desintegrate();
	void                    finish();

	float                   get_timeout_value() const {return timeout.get();}

	private:

	static const int        projectile_w{16};
	static const int        projectile_h{9};

	enum class states {
		moving,
		desintegrating,
		done
	}                       state{states::moving};

	d2d::components::timeout    timeout;
};
}
