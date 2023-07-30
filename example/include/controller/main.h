#pragma once

#include "app/service_provider.h"
#include "app/env.h"
#include <dfw/controller_interface.h>
#include <d2d/collision/shaper.h>
#include <d2d/world/map.h>
#include <lm/logger.h>

namespace controller {

class main:
	public dfw::controller_interface {

	public:

	                            main(app::service_provider&);

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	private:

	const app::env&				env;
	lm::logger&					logger;
	d2d::collision::shaper&		shaper;

	d2d::world::map				current_map;
};

}
