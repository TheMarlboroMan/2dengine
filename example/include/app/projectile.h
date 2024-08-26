#pragma once

#include "entity.h"
#include <d2d/motion/definitions.h>
#include <d2d/motion/mover.h>
#include <d2d/components/timeouts.h>

namespace app {

class projectile {

	public:

	enum class types {
		horizontal,
		round,
		falling
	};

	                        projectile(d2d::collision::point, d2d::motion::motion_vector, types);

	entity                  ent;
	d2d::motion::motion_vector velocity;

	bool                    is_moving() const {return state==states::moving;}
	bool                    is_desintegrating() const {return state==states::desintegrating;}
	bool                    is_done() const {return state==states::done;}

	void                    tic(float _delta, d2d::motion::mover);
	void                    desintegrate();
	void                    finish();

	float                   get_timeout_value() const {return timeout.get();}
	types                   get_type() const {return type;}

	private:

	static const int        projectile_horizontal_w{16};
	static const int        projectile_horizontal_h{9};

	static const int        projectile_round_w{6};
	static const int        projectile_round_h{6};

	static const int        projectile_falling_w{6};
	static const int        projectile_falling_h{6};

	enum class states {
		moving,
		desintegrating,
		done
	}                       state{states::moving};
	
	types                       type;
	d2d::components::timeout    timeout;
};
}
