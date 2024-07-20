#pragma once

#include "app/definitions.h"
#include "app/service_provider.h"
#include "app/env.h"
#include <dfw/controller_interface.h>
#include <lm/logger.h>

namespace controller {

class pause:
	public dfw::controller_interface {

	public:

	                            pause(app::service_provider&);

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	private:
	app::service_provider&      sp;
	const appenv::env&          env;
	lm::logger&                 logger;
	const app::inventory&       inventory;
};

}
