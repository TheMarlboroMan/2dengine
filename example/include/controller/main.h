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
#include "app/player_input.h"
#include <app/player.h>

#include <dfw/controller_interface.h>
#include <d2d/collision/shaper.h>
#include <d2d/video/debug_display.h>
#include <lm/logger.h>
#include <d2d/motion/gravity.h>
#include <d2d/video/scenery_tile_draw_animated.h>
#include <d2d/video/sprite_draw.h>
#include <d2d/video/sprite_draw_animated.h>
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

	void                        draw_scene(ldv::screen&);
	void                        draw_debug(ldv::screen&);
	void                        draw_player(ldv::screen&, const app::player&);

	void                        tic(float, app::player_input);
	void                        tic_ground(float, app::player_input);
	void                        tic_ladder(float, app::player_input);
	void                        tic_air(float, app::player_input);

	void                        load_map(const std::string&);

	void                        start_falling();
	void                        land_on_ground();
	void                        touch_ceiling();
	void                        grab_ladder(const app::ladder&);
	void                        jump();
	void                        walk_out_of_ladder(const d2d::collision::tile&, int);
	void                        jump_out_of_ladder(int);

	void                        setup_camera(int, int);
/**
 * sets up static values for physics and others.
 */
	void                        reload_values();
	void                        setup_timeouts();

	const app::env&				env;
	lm::logger&					logger;
	d2d::collision::shaper&		shaper;
	const app::tile_impl&       tile_impl;

#ifdef IS_DEBUG_BUILD
	bool                        debug_draw{false};
	d2d::video::debug_display	dd;
#endif

	d2d::video::sprite_draw     sprite_draw;
	d2d::video::sprite_draw_animated sprite_draw_animated;
	d2d::video::scenery_tile_draw_animated scenery_tile_draw;

	//game vars. //TODO: should be constants in production.
	//TODO: Actually, these could be part of a large struct
	//and could be made const.
	d2d::motion::gravity        gravity;
	double                      jump_force{120.0},
	                            walk_max_velocity{170.},
	                            ladder_max_velocity{60.};

	//game state stuff.
	app::map                    current_map;
	app::player                 player;
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
