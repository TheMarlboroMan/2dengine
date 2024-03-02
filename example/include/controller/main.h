#pragma once

#ifdef IS_DEBUG_BUILD
#include <appconsole/console.h>
#include <console/console.h>
#endif

#include "app/service_provider.h"
#include "app/env.h"
#include "app/map.h"
#include "app/ladder.h"
#include "app/simulation.h"
#include "app/tile_impl.h"
#include "app/player_input.h"
#include "app/player.h"
#include "app/types.h"

#include <dfw/controller_interface.h>
#include <d2d/collision/shaper.h>
#include <d2d/video/debug_display.h>
#include <lm/logger.h>
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

	void                        loop_scene(dfw::input&, const dfw::loop_iteration_data&);
	void                        draw_scene(ldv::screen&);
	void                        draw_player(ldv::screen&, const app::player&);
	void                        draw_ladder(ldv::screen&, const app::ladder&);
	void                        draw_collectible(ldv::screen&, const app::collectible&);

	void                        tic(float, app::player_input);
	void                        tic_ground(float, app::player&, app::player_input);
	void                        tic_ladder(float, app::player&, app::player_input);
	void                        tic_air(float, app::player&, app::player_input);
	void                        tic_crouch(float, app::player&, app::player_input);
	void                        tic_defeat(float, app::player&, app::player_input);

	void                        setup_camera(int, int);
	void                        load_map(const std::string&);
/**
 * unloads map, loads new map, takes player to it.
 */
	void                        exit_to(app::player&, app::exit);
	void                        take_player_to_entry(app::player&, int, const app::exit* =nullptr);
	void                        restart_level();
	app::entry                  find_entry_by_id(int) const;

	//query methods...
	bool                        can_grab_ladder(const app::player&, const app::ladder *&) const;
	bool                        is_on_exit(const app::player&, const app::exit *&, bool) const;
	bool                        is_on_air(const app::player&) const;
	bool                        is_into_harm(const app::player&) const;

	//act on player methods...
	void                        start_falling(app::player&);
	void                        land_on_ground(app::player&);
	void                        collide_with_wall(app::player&);
	void                        touch_ceiling(app::player&);
	void                        grab_ladder(app::player&, const app::ladder&);
	void                        jump(app::player&);
	void                        crouch(app::player&);
	void                        stand_up(app::player&);
	void                        defeat(app::player&);
	void                        walk_out_of_ladder(app::player&, const d2d::collision::tile&, int);
	void                        jump_out_of_ladder(app::player&, int);
	void                        drop_out_of_ladder(app::player&);
	void                        pick_up_collectible(app::player&, const app::collectible&);

	const app::env&             env;
	lm::logger&                 logger;
	d2d::collision::shaper&     shaper;
	const app::tile_impl&       tile_impl;

	ldv::camera                 camera;
	d2d::video::sprite_draw     sprite_draw;
	d2d::video::sprite_draw_animated sprite_draw_animated;
	d2d::video::scenery_tile_draw_animated scenery_tile_draw;
	app::tpersistence&          persistence;

	//game state stuff.
	app::simulation             simulation;
	app::map                    current_map;
	app::player                 player;
	int                         last_entry_id{0};

#ifdef IS_DEBUG_BUILD

	void                        reload_values();
	void                        loop_debug(dfw::input&, const dfw::loop_iteration_data&);
	void                        setup_console(app::service_provider&);
	void                        draw_debug(ldv::screen&);
	console::result             execute_cmd(const std::string&, const std::vector<console::argument>&);

	bool                        debug_draw{false};
	d2d::video::debug_display	dd;
	std::unique_ptr<console::console>   console{nullptr};
	std::unique_ptr<appconsole::console> console_display{nullptr};
	bool                        console_enabled{false};
	void                        console_display_onenter(const std::string&);

#endif
};

}
