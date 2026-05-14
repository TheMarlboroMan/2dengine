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
#include "app/map_transition.h"
#include "app/boss_map_interface.h"
#include "app/particle_module_flame.h"
#include "app/particle_module_projectile_splash.h"
#include "app/particle_module_projectile_horizontal_splash.h"
#include "app/particle_module_breaking_platform.h"
#include "app/particle_module_bonus.h"
#include "app/particle_module_smoke.h"
#include "app/particle_module_pickup.h"

#include <dfw/controller_interface.h>
#include <d2d/collision/shaper.h>
#include <d2d/collision/collision_tracker.h>
#include <d2d/video/debug_display.h>
#include <d2d/components/timeout.h>
#include <lm/logger.h>
#include <d2d/audio/music_player.h>
#include <d2d/audio/sound_player.h>
#include <memory>
#include <ldtools/time_definitions.h>

using ldtools::tdelta;

namespace controller {

class main:
	public dfw::controller_interface,
	public app::boss_map_interface
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
	bool                        loop_transition(double);
	void                        draw_scene(ldv::screen&);

	void                        tic(tdelta, app::player_input);
	void                        tic_world(tdelta);
	void                        tic_projectile(app::projectile&, tdelta);
	void                        clean_expired_entities();
	void                        tic_ground(tdelta, app::player&, app::player_input);
	void                        tic_ladder(tdelta, app::player&, app::player_input);
	void                        tic_air(tdelta, app::player&, app::player_input);
	void                        tic_crouch(tdelta, app::player&, app::player_input);
	void                        tic_defeat(tdelta, app::player&, app::player_input);
	void                        player_motion(app::player&, d2d::motion::motion_vector, tdelta);
	int                         player_collision(app::player&, d2d::motion::motion_vector, tdelta);
	bool                        is_in_legal_position(const d2d::collision::spatiable&, bool);
	bool                        is_in_legal_position(const d2d::collision::box&, bool);

	void                        setup_camera(int, int);
	void                        load_map(const std::string&);
	void                        attempt_exit(const app::exit&);
	void                        exit_to(const app::exit&);
	void                        exit_to(const std::string, int, d2d::collision::point, bool);
	void                        take_player_to_entry(int, bool, d2d::collision::point={0.,0.});
	void                        restart_level();
	app::entry                  find_entry_by_id(int) const;

	//query methods...
	bool                        can_grab_ladder(const app::player&, const app::ladder *&) const;
	bool                        is_on_exit(const app::player&, const app::exit *&, bool) const;
	bool                        is_on_air(const app::player&) const;
	bool                        is_into_harm(const app::player&) const;
	bool                        can_activate_button(const app::player&, app::button *&);
	bool                        has_key(const app::player&, const app::button&) const;

	//act on player methods...
	void                        start_falling(app::player&);
	void                        land_on_ground(app::player&);
	void                        collide_with_wall(app::player&);
	void                        touch_ceiling(app::player&);
	void                        grab_ladder(app::player&, const app::ladder&);
	void                        defeat(app::player&);
	void                        walk_out_of_ladder(app::player&, const d2d::collision::tile&, int);
	void                        pick_up_collectible(app::player&, app::collectible&);
	void                        discover_secret(app::player&, app::secret_cover&);
	void                        activate_button(app::button&);
	void                        activate_touch_trigger(app::touch_trigger&);
	int                         play_sound(int);
	void                        save_game(const std::string&, int);
	void                        reset_game(int, const std::string&);
	void                        game_over();
	void                        mount_player_in_blocks(app::player&);
	void                        player_jump(app::player&);
	void                        player_jump_out_of_ladder(app::player&, double);

	//////
	//world methods.
	void                        clear_transient_state();
	//Entrypoint for basic projectile generation from generators.
	void                        generate_projectile(const app::projectile_generator&);
	void                        activate_tag(int, bool);
	void                        post_tic();
	void                        setup_area_banner(const std::string&);
	void                        setup_moving_blocks();
	void                        write_moving_block(app::moving_block&, int);
	void                        sync_facing_blocks();
	void                        toggle_blocks(app::player&);
	bool                        is_map_complete(const app::exit* =nullptr) const;
	void                        mark_map_as_complete();
	void                        discover_map(int, bool);

	//TODO: Move me to another entity... with all the properties too...
	void                        create_projectile_end_particles(const app::projectile&);
	void                        create_breaking_block_particles(const app::breaking_platform&);
	void                        create_skull_break_particles(const app::boss_skull&);

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
	app::game_draw              gd; //carries some state, so it must be a property.

	//components...
	//TODO: Move me somewhere else??
	app::particle_module_flame  particle_mod_flame;
	app::particle_module_projectile_splash  particle_mod_projectile_splash;
	app::particle_module_projectile_horizontal_splash  particle_mod_projectile_horizontal_splash;
	app::particle_module_breaking_platform  particle_mod_breaking_platform;
	app::particle_module_bonus  particle_mod_bonus;
	app::particle_module_smoke  particle_mod_smoke;
	app::particle_module_pickup  particle_mod_pickup;

	//game state stuff.
	app::simulation             simulation;
	app::map                    current_map;
	app::player                 player;
	int                         last_entry_id{0};
	int                         difficulty_setting{app::skill_normal};
	std::unique_ptr<app::map_transition> transition{nullptr},
	                            transition_out{nullptr};

	struct {
		int                     active_count{0},
		                        channel_index{-1};
	}                           trap_sound;

	//This will be used to store information of the next level after a transition
	//when needed.
	struct {
		std::string             map_filename;
		int                     next_entry_id{0};
		bool                    hard_exit{false},
		                        apply{false};
		d2d::collision::point   exit_origin{0., 0.};
	}                           transition_exit_info;

	enum {
		timeout_lives_banner=1,
		timeout_area_banner=2,
		timeout_bonus_particles=3
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

#ifdef IS_DEBUG_BUILD

	void                        reload_values();
	void                        reload_map_debug();
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
	std::string                 last_command,

	                            current_map_name;
#endif

	//We will also need some nice predicates and dereferencers...
	struct breaking_platforms_fn{

		bool operator()(const app::breaking_platform& _block) const {

			return _block.is_solid();
		}
	};

	//TODO: can this be a "template"???
	struct facing_blocks_fn{

		bool operator()(const app::facing_block& _block) const {

			return _block.is_active();
		}
	};

	struct toggle_blocks_fn{

		bool operator()(const app::toggle_block& _block) const {

			return _block.is_active();
		}
	};

	template<typename T>
	struct spatiable_dereferencer{

		const d2d::collision::spatiable& operator()(const T& _node) const {return _node.ent;}
	};

	//Begin implementation of boss_map_interface.
	void boss_create_targeted_projectile(d2d::collision::point, double, int=0);
	void boss_create_directed_projectile(d2d::collision::point, int, double);
	void boss_create_linear_projectile(d2d::collision::point, double);
	void boss_spawn_skull(int, double);
	void boss_spawn_particle(d2d::collision::point, int);
	void boss_defeat();
	void boss_remove();
	d2d::collision::point boss_get_target() const;
	//End implementation of boss_map_interface
};

}
