#pragma once

#include "app/definitions.h"
#include "app/service_provider.h"
#include "app/env.h"
#include "app/automap_game.h"
#include "app/types.h"

#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/view_composer.h>
#include <tools/i8n.h>
#include <ldv/group_representation.h>

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

	void                        ready_map();
	void                        ready_room(const app::map_cell&);
	void                        set_area_name();

	app::service_provider&      sp;
	const appenv::env&          env;
	lm::logger&                 logger;
	const app::inventory&       inventory;
	const tools::i8n&           localization;
	app::automap_game           automap_interface;
	const app::tpersistence     persistence;

	ldtools::view_composer      view;
	ldv::group_representation   map_representation;
};

}
