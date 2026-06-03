#pragma once

#include "controller_states.h"
#include "app/service_provider.h"
#include "app/show_text_exchange.h"

#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/view_composer.h>

namespace controller
{

/**
 * this controller displays a layout with three questions (always the same)
 * and when the answer is chosen, puts it to the show_text_exchange 
 * instance.
 */

class question:
	public dfw::controller_interface
{
	public:

	                            question(app::service_provider&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen& screen, int);
	virtual void                awake(dfw::input&);
	virtual void                slumber(dfw::input&) {}
	virtual bool                can_leave_state() const {return true;}

	virtual void                request_draw(dfw::controller_view_manager&);

	private:

	void                        previous();
	void                        next();
	void                        finish();
	void                        update();

	//references...
	lm::logger&                 log;
	app::show_text_exchange&    text_exchange;

	//properties.
	ldtools::view_composer      layout;
	size_t                      current_index;
};

}
