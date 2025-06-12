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
#include "app/inventory.h"
#include "app/game_session.h"
#include "app/game_draw.h"

#include <dfw/controller_interface.h>
#include <d2d/collision/shaper.h>
#include <d2d/collision/collision_tracker.h>
#include <d2d/video/debug_display.h>
#include <d2d/components/timeouts.h>
#include <lm/logger.h>
#include <d2d/audio/music_player.h>
#include <d2d/audio/sound_player.h>
#include <memory>
#include <ldtools/time_definitions.h>

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

	void                        new_game(int, const std::string&);
	void                        start(const std::string&, int);
	void                        load_game();

	private:

	void                        loop_scene(dfw::input&, const dfw::loop_iteration_data&);
	void                        draw_scene(ldv::screen&);

	void                        tic(ldtools::tdelta, app::player_input);
	void                        tic_world(ldtools::tdelta);
	void                        tic_ground(ldtools::tdelta, app::player&, app::player_input);
	void                        tic_ladder(ldtools::tdelta, app::player&, app::player_input);
	void                        tic_air(ldtools::tdelta, app::player&, app::player_input);
	void                        tic_crouch(ldtools::tdelta, app::player&, app::player_input);
	void                        tic_defeat(ldtools::tdelta, app::player&, app::player_input);
	d2d::motion::motion_vector  ground_motion(app::player&, d2d::motion::motion_vector, ldtools::tdelta);
	int                         player_collision(app::player&, d2d::motion::motion_vector, ldtools::tdelta);
	bool                        is_in_legal_position(const d2d::collision::spatiable&, bool);
	bool                        is_in_legal_position(const d2d::collision::box&, bool);

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
	bool                        can_activate_button(const app::player&, app::button *&);
	bool                        has_key(const app::player&, const app::button&) const;

	//act on player methods..u
	void                        start_falling(app::player&);
	void                        land_on_ground(app::player&);
	void                        collide_with_wall(app::player&);
	void                        touch_ceiling(app::player&);
	void                        grab_ladder(app::player&, const app::ladder&);
	void                        defeat(app::player&);
	void                        walk_out_of_ladder(app::player&, const d2d::collision::tile&, int);
	void                        pick_up_collectible(app::player&, const app::collectible&);
	void                        discover_secret(app::player&, app::secret_cover&);
	void                        activate_button(app::player&, app::button&);
	void                        activate_touch_trigger(app::touch_trigger&);
	int                         play_sound(int);
	void                        save_game(const std::string&, int);
	void                        reset_game(int, const std::string&);
	void                        game_over();
	void                        mount_player_in_blocks(app::player&);
	void                        player_jump(app::player&);

	//world methods.
	void                        clear_transient_state();
	void                        generate_projectile(const app::projectile_generator&);
	void                        activate_tag(int, bool);
	void                        post_tic();
	void                        setup_area_banner(const std::string&);
	void                        setup_moving_blocks();
	void                        write_moving_block(app::moving_block&, int);
	void                        sync_facing_blocks();
	bool                        is_map_complete(const std::string="") const;
	void                        mark_map_as_complete();

	app::service_provider&      sp; //keep a ref, for these moment-to-moment things that don't really require us to store 100 references.
	const appenv::env&          env;
	lm::logger&                 logger;
	d2d::collision::shaper&     shaper;
	const app::tile_impl&       tile_impl;
	app::tpersistence&          persistence;
	d2d::audio::music_player&   music_player;
	d2d::audio::sound_player&   sound_player;
	app::inventory&             inventory;
	app::game_session&          game_session;

	//visual stuff.
	ldv::camera                 camera;
	app::game_draw              gd; //carries some state, so it must be a property

	//game state stuff.
	app::simulation             simulation;
	app::map                    current_map;
	app::player                 player;
	int                         last_entry_id{0};
	int                         difficulty_setting{app::skill_normal};

	struct {
		int                     active_count{0},
		                        channel_index{-1};
	}                           trap_sound;

	enum {
		timeout_lives_banner=1,
		timeout_area_banner=2
	};

	d2d::components::timeouts   game_timeouts;
	d2d::collision::collision_tracker ctracker;

	struct {
		int previous_id{0}, //previous automap area id
		    current_id{0}; //current automap area id
		void step(int _id) {

			previous_id=current_id;
			current_id=_id;
		}
		bool has_changed_area() const {return previous_id != current_id;}
	}                           area_change_info;

	/** filter for collisions with breaking platforms. */
	//TODO: Might as well mode this stuff out of here...
	struct breaking_platforms_fn{

		bool operator()(const app::breaking_platform& _block) const {

			return _block.is_solid();
		}
	};

	//TODO: Might as well mode this stuff out of here...
	struct facing_blocks_fn{

		bool operator()(const app::facing_block& _block) const {

			return _block.is_active();
		}
	};

	template<typename T>
	struct spatiable_dereferencer{

		const d2d::collision::spatiable& operator()(const T& _node) const {return _node.ent;}
	};

	//TODO: Can we have a typename for the gate ones???

#ifdef IS_DEBUG_BUILD

	void                        reload_values();
	void                        loop_debug(dfw::input&, const dfw::loop_iteration_data&);
	void                        setup_console(app::service_provider&);
	void                        draw_debug(ldv::screen&);
	console::result             execute_cmd(const std::string&, const std::vector<console::argument>&);

	bool                        debug_draw{false};
	bool                        inmortal{false};
	d2d::video::debug_display	dd;
	std::unique_ptr<console::console>   console{nullptr};
	std::unique_ptr<appconsole::console> console_display{nullptr};
	bool                        console_enabled{false};
	void                        console_display_onenter(const std::string&);

#endif
};

}
