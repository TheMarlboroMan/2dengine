#pragma once

#include "entity.h"
#include <d2d/motion/definitions.h>
#include <d2d/motion/mover.h>
#include <d2d/components/timeout.h>
#include <ldtools/time_definitions.h>
#include <iostream>

namespace app {

class moving_block {

	public:

	                                        moving_block(const d2d::collision::box& , int, int, int, bool, bool);

	int                                     get_next_id() const {return next_id;}
	int                                     get_tag() const {return tag;}
	//Controls the visual aspect.
	int                                     get_type() const {return type;}
	bool                                    has_arrived() const {return states::arrived==state;}
	bool                                    is_solid() const {return solid;}

	void                                    tic(ldtools::tdelta, const d2d::motion::mover&);
	void                                    reset();
	void                                    set_target(const d2d::collision::point&, int, int, int);
	void                                    invalidate();
	void                                    activate();

	entity                                  ent;



	private:

	bool                                    is_on_or_beyond_target() const;

	enum class states {
		receiving,
		waiting,
		in_route,
		arrived,
		invalid
	}                                       state;

	int                                     tag,
	                                        first_nodeid,
	                                        next_id,
	                                        type;
	bool                                    active,
	                                        initial_active,
											solid;
	d2d::collision::point                   target,
	                                        initial_position;
	d2d::components::timeout                timeout;
};

std::ostream& operator<<(std::ostream&, const moving_block&);
}
