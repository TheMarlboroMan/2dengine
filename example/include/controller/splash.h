#pragma once

#include "controller_states.h"
#include "app/service_provider.h"

#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/view_composer.h>
#include <d2d/components/timeout.h>

namespace controller
{

/**
 * splash screen controller.
 */

class splash:
	public dfw::controller_interface
{
	public:

	                            splash(app::service_provider&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen& screen, int);
	virtual void                awake(dfw::input&) {}
	virtual void                slumber(dfw::input&) {}
	virtual bool                can_leave_state() const {return true;}

	private:

	//properties.
	ldtools::view_composer      layout;
	d2d::components::timeout    timer;

	void                        next();
	void                        force_leave();

	enum states {
		begin, //all black
		appear, //logo comes in
		wait, //logo stays
		dissapear, //logo goes
		leave //all black leave.
	}                           state{states::begin};
};

}
