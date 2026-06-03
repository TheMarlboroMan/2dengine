#pragma once

#include "controller_states.h"
#include "app/service_provider.h"
#include <dfw/controller_interface.h>
#include <lm/logger.h>

namespace controller
{


class leave_ending:
	public dfw::controller_interface
{
	public:

	                            leave_ending(app::service_provider&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen& screen, int);
	virtual void                awake(dfw::input&);
	virtual void                slumber(dfw::input&) {}
	virtual bool                can_leave_state() const {return true;}

	private:

	//references...
	lm::logger&                 log;
};

}
