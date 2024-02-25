#pragma once

#ifdef IS_DEBUG_BUILD
#include <appconsole/console.h>
#include <console/console.h>
#endif

#include "app/service_provider.h"
#include "app/env.h"
#include "app/map.h"
#include "app/ladder.h"
#include "app/timeouts.h"
#include "app/tile_impl.h"
#include <dfw/controller_interface.h>
#include <d2d/collision/shaper.h>
#include <d2d/video/debug_display.h>
#include <lm/logger.h>
#include <app/entity.h>
#include "app/player_input.h"
#include <d2d/motion/gravity.h>
#include <memory>

namespace controller {

class main:
	public dfw::controller_interface
#ifdef IS_DEBUG_BUILD
	,public console::console_environment
#endif 
	{

	public:

	                            main(app::service_provider&);

	virtual void                loop(dfw::input&, const dfw::loop_iteration_data&);
	virtual void                draw(ldv::screen&, int);
	virtual void                awake(dfw::input& /*input*/);
	virtual void                slumber(dfw::input& /*input*/);
	virtual bool                can_leave_state() const {return true;}

	private:

	void                        tic(float, app::player_input);
	void                        tic_regular(float, app::player_input);
	void                        tic_ladder(float, app::player_input);

	void                        load_map(const std::string&);

	void                        grab_ladder(const app::ladder&);
	void                        leave_ladder();

	void                        setup_camera(int, int);
/**
 * sets up static values for physics and others.
 */
	void                        reload_values();
	void                        setup_timeouts();

	enum class player_states {
		regular,
		ladder
	}                           player_state{player_states::regular};

	const app::env&				env;
	lm::logger&					logger;
	d2d::collision::shaper&		shaper;
	const app::tile_impl&       tile_impl;

	d2d::video::debug_display	dd;

	//game vars. //TODO: should be constants in production.
	d2d::motion::gravity        gravity;
	double                      jump_force{120.0},
	                            walk_max_velocity{170.},
	                            ladder_max_velocity{60.};
	bool                        can_jump{false}; //We will get it better.

	//game state stuff.
	app::map                    current_map;
	app::entity                 ent;
	app::ladder                 current_ladder{0,0,0};
	app::timeouts               timeouts;

	enum timeout_indexes {
		timeout_ladder=0
	};

#ifdef IS_DEBUG_BUILD

	void                        setup_console(app::service_provider&);
	console::result execute_cmd(const std::string&, const std::vector<console::argument>&);

	std::unique_ptr<console::console>   console{nullptr};
	std::unique_ptr<appconsole::console> console_display{nullptr};
	bool                        console_enabled{false};
	void                        console_display_onenter(const std::string&);

#endif
};

}
