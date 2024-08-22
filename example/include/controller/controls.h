#pragma once

#include <app/service_provider.h>
#include <dfw/controller_interface.h>
#include <dfw/input_definitions.h>
#include "dfwimpl/config.h"
#include <string>
#include <vector>

namespace controller {

class controls:
	public dfw::controller_interface {

	public:

	                            controls(app::service_provider&);

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	private:

	void                        learn(dfw::input&, std::size_t, dfw::input_description::types, int, int);
	void                        save_and_finish(dfw::input&);

	app::service_provider&      sp;
	dfwimpl::config&            config;
	std::size_t                 index{0};
	bool                        cancel_signal{false};

	struct entry{
		int         app_id;
		std::string i8n_key,
		            config_map_key;
		dfw::input_description input;
	};

	std::vector<entry>          entries;
};

}
