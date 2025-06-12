#pragma once

#include "app/definitions.h"
#include "app/service_provider.h"
#include "app/env.h"
#include "app/automap_game.h"
#include "app/automap.h"
#include "app/types.h"
#include "app/game_session.h"

#include <dfw/controller_interface.h>
#include <lm/logger.h>
#include <ldtools/view_composer.h>
#include <ldtools/time_definitions.h>
#include <tools/i8n.h>
#include <ldv/group_representation.h>
#include <ldv/color.h>

namespace controller {

class pause:
	public dfw::controller_interface {

	public:

	                            pause(app::service_provider&);

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return timeout_passed;}

	private:

	void                        ready_map();
	bool                        ready_room(const app::map_cell&);
	void                        evaluate_timeout(ldtools::tdelta);

	const appenv::env&          env;
	lm::logger&                 logger;
	const app::inventory&       inventory;
	const tools::i8n&           localization;
	const app::tpersistence&    persistence;
	const app::game_session&    game_session;
	const app::automap&         automap;

	app::automap_game           automap_interface;
	ldtools::view_composer      view;
	ldv::group_representation   map_representation;

	ldv::rgba_color             wall_complete,
	                            wall_incomplete,
	                            regular_fill,
	                            current_fill;

	bool                        timeout_passed{false};
	ldtools::tdelta             time_elapsed{0.0};

#ifdef IS_DEBUG_BUILD
	bool                        display_all_maps{false};
#endif 
};

}
