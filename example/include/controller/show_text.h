#pragma once

#include "controller_states.h"
#include "app/service_provider.h"
#include "app/env.h"
#include "app/show_text_exchange.h"

#include <ldtools/view_composer.h>
#include <ldtools/ttf_manager.h>
#include <lm/logger.h>
#include <dfw/controller_interface.h>
#include <d2d/components/timeout.h>

#include <vector>

namespace controller
{

class show_text:
	public dfw::controller_interface
{
	public:

	                            show_text(app::service_provider&);
	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen& screen, int);
	virtual void                awake(dfw::input&);
	virtual void                slumber(dfw::input&) {}
	virtual bool                can_leave_state() const {return true;}

	virtual void                request_draw(dfw::controller_view_manager&);

	private:

	void                        reload_buffer();
	void                        redraw_text();

	//references...
	lm::logger&                 log;
	app::show_text_exchange&    text_exchange;

	//properties.
	ldtools::view_composer      layout;

	enum class tstates{dictate, wait} state{tstates::dictate};

	//raw text is a large string that can be split into several others by the 
	//separator [@np]. These components are communicated through the "awake" 
	//method, by using a service shared with the main controller. These @np 
	//can be used in internationalization, to adapt different text lengths 
	//without touching the inner data structures.
	std::string                 raw_text;
	std::vector<std::string>    text_buffer;
	size_t                      current_index; //current text index.
	d2d::components::timeouts   timers;

	enum {
		time_text,
		time_blink
	};
};

}
