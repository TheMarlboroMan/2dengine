#include "controller/main.h"
#include "controller/main.h"
#include "controller/controller_states.h"
#include "app/input.h"
#include "app/thing_loader.h"
#include "app/map_attribute_loader.h"
#include "app/tile_filter.h"
#include "app/thing_filter.h"
#include "app/savegame.h"
#include "app/automap_game.h" //to obtain area names...
#include "app/projectile_creator.h"
#include "app/map_transition_fade.h"
#include "app/show_text_exchange.h"
#include "app/starfield.h"
#include "app/storm.h"
#include "app/suspension.h"

#include "dfwimpl/config.h"

#include <d2d/storage/map_loader.h>
#include <d2d/video/camera_map_limit.h>
#include <d2d/video/tools.h>
#include <d2d/motion/mover.h>
#include <d2d/collision/aabb_static_checker.h>
#include <d2d/collision/ray_aabb_phase.h>
#include <d2d/collision/ray_aabb_solver.h>
#include <d2d/collision/ray_builder.h>
#include <d2d/collision/aabb_checker.h>
#include <d2d/collision/tiles_in_box.h>
#include <d2d/collision/tile_limits.h>
#include <d2d/tools/algorithm.h>

#include <dfw/id_to_path_mapper.h>

#include <tools/string_utils.h>
#include <tools/ranged_value.h>

#include <ldtools/ttf_manager.h>
#include <ldt/box.h>

#include <sstream>
#include <iostream>
#include <cmath>
#include <functional>

#ifdef IS_DEBUG_BUILD

#include <ldv/color.h>

#endif

using namespace controller;

main::main(
	app::service_provider& _sp
):
	sp{_sp},
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	shaper{_sp.get_shaper()},
	tile_impl{_sp.get_tile_impl()},
	persistence{_sp.get_persistence()},
	music_player{_sp.get_music_player()},
	sound_player{_sp.get_sound_player()},
	inventory{_sp.get_inventory()},
	game_session{_sp.get_game_session()},
	camera{ {0,0,app::logic_screen_w, app::logic_screen_h}, {0,0}},
	gd{
		camera,
		_sp.get_game_scenery_tile_draw(),
		_sp.get_game_sprite_draw(),
		_sp.get_game_sprite_fill_draw(),
		_sp.get_game_animation_sprite_finder(),
		_sp.get_ttf_manager(),
		_sp.get_video_resource_manager(),
		env,
		_sp.get_random()
	},
	particle_mod_projectile_splash{_sp.get_random()},
	particle_mod_projectile_horizontal_splash{_sp.get_random()},
	particle_mod_breaking_platform{_sp.get_random()},
	particle_mod_bonus{_sp.get_random()},
	particle_mod_smoke{_sp.get_random()},
	particle_mod_pickup{_sp.get_random()}
#ifdef IS_DEBUG_BUILD
	,
	dd{app::logic_screen_w, app::logic_screen_h}
#endif
{

#ifdef IS_DEBUG_BUILD

	setup_console(_sp);
	setup_debug_vars();
	setup_debug_trace();
	dd.set_background_color(ldv::rgba_color(128, 128, 128, 0));
	reload_values();

#endif

	setup_camera(
		app::logic_screen_w,
		app::logic_screen_h
	);

	//The renderer for all particles will be the same, so we will register
	//these in pairs and with the gd as renderer.
	//TODO: This is UGLY AF. We don't need to own SO much shit here just because
	//we want po paint a few pretty pixels!
	current_map.particle_manager.register_module(particle_mod_flame);
	current_map.particle_manager.register_renderer(gd);

	current_map.particle_manager.register_module(particle_mod_projectile_splash);
	current_map.particle_manager.register_renderer(gd);

	current_map.particle_manager.register_module(particle_mod_projectile_horizontal_splash);
	current_map.particle_manager.register_renderer(gd);

	current_map.particle_manager.register_module(particle_mod_breaking_platform);
	current_map.particle_manager.register_renderer(gd);

	current_map.particle_manager.register_module(particle_mod_bonus);
	current_map.particle_manager.register_renderer(gd);

	current_map.particle_manager.register_module(particle_mod_smoke);
	current_map.particle_manager.register_renderer(gd);

	current_map.particle_manager.register_module(particle_mod_pickup);
	current_map.particle_manager.register_renderer(gd);

	game_timeouts.add(timeout_lives_banner, 4., 4., true);
	game_timeouts.add(timeout_area_banner, 3., 0, true);
	game_timeouts.add(timeout_bonus_particles, 0.1, 0, false);
}

void main::start(
	const std::string& _map,
	int _entry_id
) {

	//Ready a simple transition...
	transition.reset(new app::map_transition_fade(
		app::map_transition_fade::colors::color_black,
		1, //from,
		1.0 //one second.
	));

	transition_exit_info.type=transition_exit::types::into_game;
	load_map(_map);
	take_player_to_entry(_entry_id, true);
}

void main::new_game(
	int _skill,
	const std::string& _slot_filename
) {

	lm::log(logger).info()<<"starting new game..."<<std::endl;
	reset_game(_skill, _slot_filename);
	start("intro_001", 1);
}

void main::load_game() {

	app::savegame_io sio{};
	lm::log(logger).info()<<"loading game from "<<game_session.savegame_file<<std::endl;

	auto save=sio.load_from_file(game_session.savegame_file);
	reset_game(save.difficulty_setting, game_session.savegame_file);

	//No need to add groups: the persistence will do it.
	persistence.reset();
	persistence.load_from_string(save.persistence_string);

	inventory.red_keys=save.red_keys;
	inventory.blue_keys=save.blue_keys;
	inventory.green_keys=save.green_keys;
	inventory.yellow_keys=save.yellow_keys;
	inventory.white_keys=save.white_keys;
	inventory.ultimate=save.ultimate;
	//TODO: Why EvEN SAVE IT THEN????
	//Treasure actually comes from the persistence layer.
	inventory.treasure=persistence.size(app::pergr_collectibles);

	game_session.skill_level=save.difficulty_setting;
	game_session.elapsed_seconds=save.elapsed_seconds;
	game_session.lives=save.lives;
	game_session.discovered_rooms=save.discovered_rooms;

	start(save.map_filename, save.entry_id);
}

void main::reset_game(
	int _skill,
	const std::string& _savegame_file
) {

	//TODO: Why should this go here in this controller?
	//I don't know, I don't remember... where is this called from? Who
	//owns these things?
	persistence.reset();

	player.reset();
	inventory.reset();
	game_session.reset(_skill, _savegame_file);
}

void main::game_over() {

	//TODO: This is only hard mode and it should remove the save xD
	throw std::runtime_error("game over is not implemented!");
}

void main::awake(
	dfw::input& /*input*/
) {

	if(current_map.in_game) {

		start_game_clock();
	}
}

void main::slumber(
	dfw::input& /*input*/
) {

	game_session.game_clock.pause();
}

void main::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	//Exiting and escaping to the menu is always possible.
	if(_input().is_exit_signal()) {

		lm::log(logger).info()<<"caught exit signal, quitting\n";
		set_leave(true);
		return;
	}

	//Transitions override regular input, including going back to the menu.
	if(transition || transition_out) {

		if(loop_transition(_lid.delta)) {

			return;
		}
	}

	//TODO: Can we check this thing with the autopilot, see if it flies??
	if(_input.is_input_down(app::input::escape)) {

		if(1!=state_size()) {

			lm::log(logger).info()<<"will go back to main menu\n";
			pop_state();
			return;
		}

		lm::log(logger).info()<<"attemping to leave main state with no stack, exiting";
		set_leave(true);
		return;
	}

	loop_scene(_input, _lid);
}

bool main::loop_transition(
	double _delta
) {

	if(transition) {

		transition->tic(_delta);
		if(!transition->is_finished()) {

			return true;
		}

		transition.reset(nullptr);

		//Load next map if need be.
		switch(transition_exit_info.type) {

			case transition_exit::types::into_new_map:
				exit_to(
					transition_exit_info.map_filename,
					transition_exit_info.next_entry_id,
					transition_exit_info.exit_origin,
					transition_exit_info.hard_exit
				);
			break;
			case transition_exit::types::into_credits:

				set_state(controller::state_credits);
			break;
			case transition_exit::types::into_game:
				//Noop.
			break;
		}
	}

	if(transition_out) {

		transition_out->tic(_delta);
		if(!transition_out->is_finished()) {

			return true;
		}

		transition_out.reset(nullptr);
	}

	return false;
}

void main::loop_scene(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	game_timeouts.tic(_lid.delta);

#ifdef IS_DEBUG_BUILD

	if(console_enabled) {

		_input().start_text_input();
		console_display->input(_input());
		return;
	}
	else {

		_input().stop_text_input();
		_input().clear_text_input();
	}

	if(_input.is_input_down(app::input::tic)) {

		console_enabled=true;
		console_display->get_output()<<"dot to repeat last command\n";
		return;
	}

	if(_input.is_input_down(app::input::reload_values)) {

		reload_values();
	}

#endif

	pli.reset();
	if(autopilot.is_enabled()) {

		autopilot.tic(_lid.delta);
		autopilot.produce(pli);
	}
	else {

		//We can only pause when NOT in autopilot.
		if(_input.is_input_down(app::input::pause)) {

			push_state(controller::state_pause);
			return;
		}

		if(_input.is_input_pressed(app::input::down)) {

			pli.y=-1;
		}
		else if(_input.is_input_pressed(app::input::up)) {

			pli.y=1;

			if(_input.is_input_down(app::input::up)) {

				pli.activate=true;
			}
		}

		if(_input.is_input_pressed(app::input::left)) {

			pli.x=-1;
		}
		else if(_input.is_input_pressed(app::input::right)) {

			pli.x=1;
		}

		if(_input.is_input_pressed(app::input::jump)) {

			pli.hold_jump=true;

			if(_input.is_input_down(app::input::jump)) {

				pli.jump=true;
			}
		}
	}

	tic(_lid.delta, pli);
}

void main::load_map(
	const std::string& _map_name
) {
	std::stringstream ss;

#ifdef IS_DEBUG_BUILD

	current_map_name=_map_name;
	//This will take us to the dev files in my dev env. Obviously it will not
	//work anywhere else.
	ss<<env.build_app_path("../example/resources/maps/")<<_map_name<<".json";

#else

	ss<<env.build_app_path("resources/maps/")<<_map_name<<".json";
#endif

	const std::string map_path{ss.str()};

	lm::log(logger).info()<<"will attempt to load map "<<map_path<<"\n";

	d2d::storage::map_loader loader(map_path);

	current_map.clear();
	clear_transient_state();

	loader.load_collision_tiles(
		"logic",
		current_map.collision_tiles,
		shaper,
		tile_impl
	);

	loader.load_scenery_tiles("foreground", current_map.foreground_tiles);
	loader.load_scenery_tiles("middle", current_map.middle_tiles);
	loader.load_scenery_tiles("background", current_map.background_tiles);

	current_map.sync_tile_finder();

	//we need the maps limits, so...
	d2d::collision::tile_limits_finder tlf{};
	auto limits=tlf.find_limits(current_map.collision_tiles);
	current_map.set_limits(limits);

	app::thing_loader tl{current_map, limits, persistence, game_session.skill_level};
	loader.load_thing_layer("things", tl);

	//Setup moving stuff...
	setup_moving_blocks();

	//The loader takes references to the map data.
	app::map_attribute_loader attrl{
		current_map.background_color,
		current_map.music_id,
		current_map.background_effect,
		current_map.in_game
	};

	loader.load_properties(attrl);
	lm::log(logger).debug()<<"map musicid is "
		<<current_map.music_id
		<<", backgroundcolor is "
		<<current_map.background_color.r<<", "
		<<current_map.background_color.g<<", "
		<<current_map.background_color.b
		<<" and effect is "<<current_map.background_effect<<"\n";

	background.reset(nullptr);
	switch(current_map.background_effect) {

		case app::bg_starfield:
			background.reset(
				new app::starfield(
					camera.get_pos_box().w,
					camera.get_pos_box().h,
					inventory.treasure, //as many stars as treasure we got. That's poetic.
					sp.get_random()
				)
			);
		break;
		case app::bg_storm:
			background.reset(
				new app::storm(
					sp.get_random(),
					camera.get_pos_box().w,
					camera.get_pos_box().h,
					inventory.treasure //same, stars.
				)
			);
		break;
		case app::bg_suspension: {
			const auto& tracked_box=player.ent.get_box();
			background.reset(
				new app::suspension(
					pli,
					extract_green_key_text_nodes(),
					sp.get_random(),
					camera,
					tracked_box.origin.y,
					sp.get_ttf_manager().get("show_text_font", 8),
					inventory.treasure //same, stars.
				)
			);
			}
		break;
	}

	gd.set_background(background.get());

	//Now the music... pieces are loaded in real time so nothing to do here.
	music_player.swap(current_map.music_id, 500);

	int automap_id=-1; //A stupid default...

	//Non in-game map have no banners, no automap, no nothing. They also 
	//pause the game clock.
	if(!current_map.in_game) { 

		game_session.game_clock.pause();
	}
	else {

		start_game_clock();
		autopilot.disable(); //just in case...

		//Setup the automap.
		const auto& automap_srv=sp.get_automap();
		app::automap_game automap{automap_srv};
		automap_id=automap.map_id_from_map(_map_name);
		game_session.current_map_id=automap_id;

		lm::log(logger).debug()<<"automap id for "<<_map_name<<" = "<<automap_id<<"\n";

		//Should we show a new area banner? We need first to infer the new
		//area id... This will also make the banner show whenever a game is
		//started or loaded.
		const auto& area=automap_srv.find_area_by_map(_map_name);

		area_change_info.step(area.id);
		if(area_change_info.has_changed_area()) {

			const auto& new_area_name=sp.get_localization().get(area.localization_key);
			lm::log(logger).debug()<<"will show area banner for '"<<new_area_name<<"'\n";
			setup_area_banner(new_area_name);
		}

		//Ok, discover this map... Remember, we only discover maps with in_game.
		if(!persistence.has(app::pergr_automap, automap_id)) {

			discover_map(automap_id, area.must_draw_map);
		}
	 }

	//All activated switches should run their course now: all activated objects
	//will not save their state but will be activated when the switches do
	//their thing.
	for(const auto& button : current_map.buttons) {

		if(!button.used) {

			continue;
		}

		activate_tag(button.tag, true);
	}

	for(const auto& trigger : current_map.touch_triggers) {

		if(!trigger.used) {

			continue;
		}

		activate_tag(trigger.tag, true);
	}

	for(const auto& trigger : current_map.touch_triggers) {

		if(!trigger.used) {

			continue;
		}

		activate_tag(trigger.tag, true);
	}

	sync_facing_blocks();

	//After loading the map, tell the camera where the limits are. We use
	//removing harm tiles/solid but no camera, to allow for these to exist outside
	//the camera boundaries.
	d2d::collision::tile_limits_finder::filter_function ff=[](const d2d::collision::tile& _tile) {
		return _tile.type==app::tile_harm || _tile.type==app::tile_full_no_camera;
	};

	auto tile_limits_view=tlf.find_limits(current_map.collision_tiles, ff);

	d2d::video::camera_map_limit cml;
	cml.limit_to_collision_tiles(camera, tile_limits_view, shaper.get_tile_w(), shaper.get_tile_h(), &logger);

#ifdef IS_DEBUG_BUILD
	cml.limit_to_collision_tiles(dd.camera, tile_limits_view, shaper.get_tile_w(), shaper.get_tile_h(), &logger);
#endif

	//More stuff... if there's a boss, inject this controller so we can interact
	//with the world.
	if(current_map.boss && !current_map.boss->is_defeated()) {

		current_map.boss->set_boss_map_interface(*this);
	}
}

void main::attempt_exit(
	const app::exit& _exit
) {

	if(0!=_exit.min_rooms) {

		int visited=game_session.get_discovered_map_count();
		if(visited < _exit.min_rooms) {

			lm::log(logger).debug()<<"cannot enter, "<<visited<<"/"<<_exit.min_rooms<<" rooms visited"<<std::endl;
			play_sound(app::snd_forbidden);
			return;
		}

		if(is_map_complete(&_exit)) {

			mark_map_as_complete();
		}
	}

	//From this point on, an exit to another room will happen.
	lm::log(logger).info()<<"player is on exit to "<<_exit.map_filename<<" with entry id "<<_exit.next_entry_id<<"\n";

	//Special exits are marked as visited so they will not be loaded again
	//when re-entering this room. These count towards room completion so we must
	//also check for that.

	if(_exit.is_special()) {

		//TODO: The problem with this according to excise-engine.txt: we may crash
		//or exit, the persistence is saved but we haven't yet left this
		//room. Because the exist will disspear we will not be able to
		//finish the game anymore.

		switch(_exit.type) {

			case app::exit::types::redkey:
				persistence.add(app::pergr_events, app::perev_red_key_teleport, 1);
			break;
			case app::exit::types::bluekey:
				persistence.add(app::pergr_events, app::perev_blue_key_teleport, 1);
			break;
			case app::exit::types::greenkey:
				persistence.add(app::pergr_events, app::perev_green_key_teleport, 1);
			break;
		}

		if(is_map_complete(&_exit)) {

			mark_map_as_complete();
		}
	}

	switch(_exit.transition_type) {

		case app::exit::transition_fade_to_black:

			transition.reset(new app::map_transition_fade(
				app::map_transition_fade::colors::color_black,
				0, //to,
				0.25 //quarter of a second
			));

			transition_out.reset(new app::map_transition_fade(
				app::map_transition_fade::colors::color_black,
				1, //from,
				0.25 //quarter of a second
			));

			transition_exit_info={
				_exit.map_filename,
				_exit.next_entry_id,
				false, 
				transition_exit::types::into_new_map,
				_exit.ent.get_origin()
			};

			return;
		case app::exit::transition_fade_to_white:

			transition.reset(new app::map_transition_fade(
				app::map_transition_fade::colors::color_white,
				0, //to,
				2.0 //some seconds...
			));

			transition_out.reset(new app::map_transition_fade(
				app::map_transition_fade::colors::color_white,
				1, //from,
				2.0 //some seconds...
			));

			transition_exit_info={
				_exit.map_filename,
				_exit.next_entry_id,
				false,
				transition_exit::types::into_new_map,
				_exit.ent.get_origin()
			};

			return;

		case app::exit::transition_none:
		default:
			return exit_to(_exit);
	}
}

void main::exit_to(
	const app::exit& _exit
) {

	//Assume a soft exit, since we have an exit.
	exit_to(_exit.map_filename, _exit.next_entry_id, _exit.ent.get_origin(), false);
}

void main::exit_to(
	const std::string _map_filename, //a copy, because the original will be gone after load_map.
	int _next_entry_id,
	d2d::collision::point _prev_exit_pos,
	bool _hard_exit
) {

	player.current_ladder=nullptr;

	load_map(_map_filename);
	take_player_to_entry(_next_entry_id, _hard_exit, _prev_exit_pos);

	//game is saved at each map change xD
	if(current_map.in_game) {

		save_game(_map_filename, _next_entry_id);
	}
}

void main::take_player_to_entry(
	int _id,
	bool _hard_exit,
	d2d::collision::point _prev_exit_pos
) {

	lm::log(logger).info()<<"will attempt to set player at entry "<<_id<<"\n";
	last_entry_id=_id;
	auto map_entry=find_entry_by_id(last_entry_id);

	auto find_ladders=[this, &map_entry]() {

		d2d::collision::aabb_checker cc(map_entry.ent);
		const auto ladders=cc.get_collisions(current_map.ladders);
		lm::log(logger).info()<<"found "<<ladders.size()<<" ladders in the last entry point\n";
		return ladders;
	};

	//If no exit is indicated this is a "hard transition", e.g. reloading after
	//defeat.
	if(_hard_exit) {

		lm::log(logger).info()<<"taking player to exit (hard mode)\n";

		//This is crucial, if there is an entry with a ladder we favor it and
		//place the player on it depending on how the last entry expected the
		//player to be placed. The extra tile_h is to avoid the player colliding
		//with the exit.
		auto ladders=find_ladders();
		if(ladders.size()) {

			lm::log(logger).info()<<"found ladder in entry point, it will be used\n";
			grab_ladder(player, *ladders[0]);

			if(app::entry::inner_top_edge==map_entry.position) {

				lm::log(logger).info()<<"will match the top of ladder\n";
				d2d::collision::match_top_of(player.ent, map_entry.ent);
			}
			else if(app::entry::inner_bottom_edge==map_entry.position) {

				lm::log(logger).info()<<"will match the bottom of ladder\n";
				d2d::collision::match_bottom_of(player.ent, map_entry.ent);
			}
			else {

				throw std::runtime_error("mismatch between ladder and entry, check entry position, must be top or bottom edge");
			}
		}
		else {

			lm::log(logger).info()<<"no ladder in entry point, player will stand up\n";
			player.ent.set_origin(map_entry.ent.get_origin());
			player.stand_up();
		}

		//Stop all velocity.
		player.ent.set_motion_vector({0., 0.});
	}
	else {

		lm::log(logger).info()<<"taking player to exit (soft mode)\n";
		auto& pos=player.ent.get_box();
		auto entry_pt=map_entry.ent.get_origin();
		d2d::collision::point exit_offset=pos.origin-_prev_exit_pos;

		switch(map_entry.position) {

			case app::entry::position::center_bottom:

				land_on_ground(player);
				player.ent.set_motion_vector_x(0.);
				d2d::collision::center_horizontally(player.ent, map_entry.ent);
				d2d::collision::match_bottom_of(player.ent, map_entry.ent);
			break;
			case app::entry::position::inner_top_edge:
				d2d::collision::match_top_of(player.ent, map_entry.ent);
				player.ent.set_x(entry_pt.x+exit_offset.x);
			break;

			case app::entry::position::inner_bottom_edge:
				d2d::collision::match_bottom_of(player.ent, map_entry.ent);
				player.ent.set_x(entry_pt.x+exit_offset.x);
			break;

			case app::entry::position::inner_right_edge:
				d2d::collision::match_right_of(player.ent, map_entry.ent);
				player.ent.set_y(entry_pt.y+exit_offset.y);
			break;

			case app::entry::position::inner_left_edge:
				d2d::collision::match_left_of(player.ent, map_entry.ent);
				player.ent.set_y(entry_pt.y+exit_offset.y);
			break;
		}

		//If the player was in a ladder after a soft transition we want to grab the new one.
		if(app::player::states::ladder==player.state) {

			lm::log(logger).info()<<"player was in a ladder, will try to grab a new one\n";
			auto ladders=find_ladders();
			if(!ladders.size()) {

				throw std::runtime_error("there should be a ladder at the entrypoint");
			}

			grab_ladder(player, *ladders[0]);
		}
	}

	//Commit positions so we don't get phantom collisions later!
	player.ent.commit_box();

#ifdef IS_DEBUG_BUILD
	lm::log(logger).debug()<<"new player position is "<<player.ent.get_origin()<<"\n";
	lm::log(logger).debug()<<"player box at "<<player.ent.get_box()<<"\n";
	lm::log(logger).debug()<<"player prev box at "<<player.ent.get_previous_box()<<"\n";
#endif

	//Center camera on map now..
	camera.center_on(
		d2d::video::to_screen(player.ent)
	);

#ifdef IS_DEBUG_BUILD
	dd.center_on(player.ent);
#endif
}

void main::restart_level() {

	current_map.reset();
	clear_transient_state();

	take_player_to_entry(last_entry_id, true);
}

void main::tic(
	tdelta _delta,
	app::player_input _pli
) {

#ifdef IS_DEBUG_BUILD

	if("true"==debug_session_get("log_game_seconds")) {

		auto seconds_elapsed=game_session.elapsed_seconds+game_session.game_clock.get_seconds();
		lm::log(logger).debug()<<"seconds="<<game_session.elapsed_seconds
			<<" clock="<<game_session.game_clock.get_seconds()
			<<" total="<<seconds_elapsed<<"\n";
	}

	if("true"==debug_session_get("log_tics")) {

		lm::log(logger).debug()<<"tic="<<game_tics<<"\n";
	}

	game_tics++;
#endif

	sp.get_game_scenery_tile_draw().tic(_delta);
	sp.get_game_animation_sprite_finder().tic(_delta);

	//Tic and move the world first, because the player state-aware tic methods
	//will also solve collisions and we need a "level" playing field!

	tic_world(_delta);

	//TODO:
	//Why is this outside tic_world or player_tic?
	if(current_map.moving_blocks.size() && !player.is_defeated()) {

		//Tic and correct any pushes...
		if(ctracker.tic().correct_snaps(player.ent)) {

			//Should only commit if the player was snapped in the previous tic!
			//This allows our previous position not to be in collision with
			//the box that pushed us.
			player.ent.commit_box();
		}
	}

	player.tic(_delta);

	const auto prev_face=player.facing;
	int sides=0;

	switch(player.state) {

		case app::player::states::ground:
		case app::player::states::crouch:
			sides=tic_ground(_delta, player, _pli);
		break;

		case app::player::states::ladder:
			tic_ladder(_delta, player, _pli);
		break;

		case app::player::states::air:
			sides=tic_air(_delta, player, _pli);
		break;

		case app::player::states::defeat:
			tic_defeat(_delta, player, _pli);
		break;
	}

	if(player.facing!=prev_face) {

		sync_facing_blocks();
	}

	const auto player_moved=player.ent.has_moved();
	const auto player_defeated=player.is_defeated();

	if(!player_defeated) {

		mount_player_in_blocks(player);
	}

	//Commit all movement boxes now...
	player.ent.commit_box();
	for(auto& block : current_map.moving_blocks) {

		block.ent.commit_box();
	}

	//Attempt to save a bit on camera calculations...
	if(player_moved && ! player_defeated) {

		camera.center_on(
			d2d::video::to_screen(player.ent)
		);


#ifdef IS_DEBUG_BUILD
		dd.center_on(player.ent);
#endif
	}

	//If the player can do things in the world, run them.
	if(!player_defeated) {

		post_tic(sides);
	}

	clean_expired_entities();

#ifdef IS_DEBUG_BUILD
	debug_log_trace();
#endif
}

void main::clean_expired_entities() {

	//This is more cycle intensive than just reacting to whatever thing
	//happens in the game, but I just want to have this here.

	//erase-remove idiom, get rid of projectiles we no longer want.
	current_map.projectiles.erase(
		std::remove_if(
			current_map.projectiles.begin(),
			current_map.projectiles.end(),
			[](app::projectile& _proj) {return _proj.is_done();}
		),
		current_map.projectiles.end()
	);

	bool check_map_complete=false;
	current_map.collectibles.erase(
		std::remove_if(
			current_map.collectibles.begin(),
			current_map.collectibles.end(),
			[&check_map_complete](app::collectible& _node) {
				bool result=_node.is_to_be_removed();
				check_map_complete|=result;
				return result;
			}
		),
		current_map.collectibles.end()
	);

	//Must we update the automap state because this room is now complete???
	if(check_map_complete && is_map_complete()) {

		mark_map_as_complete();
	}

	current_map.skulls.erase(
		std::remove_if(
			current_map.skulls.begin(),
			current_map.skulls.end(),
			[](app::boss_skull& _node) {return _node.is_to_be_destroyed();}
		),
		current_map.skulls.end()
	);
}

void main::post_tic(
	int _sides
) {

	//Are we crushesd?
	if(!is_in_legal_position(player.ent, true, _sides)) {

		lm::log(logger).info()<<"illegal position, assumed crushing\n";
		defeat(player);
		return;
	}

	//Are we touching a lethal tile?
	if(is_into_harm(player)) {

		defeat(player);
		return;
	}

	//Are we touching a monster?
	for(const auto& monster : current_map.linear_monsters) {

		if(d2d::collision::collides_with(player.ent, monster.ent)) {

			defeat(player);
			return;
		}
	}

	for(const auto& monster : current_map.leaping_monsters) {

		if(d2d::collision::collides_with(player.ent, monster.ent)) {

			defeat(player);
			return;
		}
	}

	if(current_map.boss && !current_map.boss->is_defeated()) {

		if(d2d::collision::collides_with(player.ent, current_map.boss->ent)) {

			defeat(player);
			return;
		}
	}

	//Or a projectile?
	for(auto& projectile : current_map.projectiles) {

		//TODO: Do we need this??
		if(!projectile.is_moving()) {

			continue;
		}

		if(d2d::collision::collides_with(player.ent, projectile.ent)) {

			//TODO: Start particles?
			projectile.finish();
			defeat(player);
			return;
		}

		for(auto& skull : current_map.skulls) {

			if(player.is_defeated()) {

				break;
			}

			if(!skull.can_be_hit()) {

				continue;
			}

			if(d2d::collision::collides_with(skull.ent, projectile.ent)) {

				create_skull_break_particles(skull);
				projectile.finish();
				skull.desintegrate();

				//and the boss must know about this...
				//Assume there is a boss, because the boss spanws the skulls.
				current_map.boss->notify_skull_destroyed(current_map.skulls.size()-1);
				return;
			}
		}
	}

	//Or a trap?
	for(const auto& trap : current_map.traps) {

		if(!trap.is_active()) {

			continue;
		}

		if(d2d::collision::collides_with(player.ent, trap.ent)) {

			defeat(player);
			return;
		}
	}

	for(const auto& trap : current_map.timed_traps) {

		if(!trap.is_harmful()) {

			continue;
		}

		if(d2d::collision::collides_with(player.ent, trap.ent)) {

			defeat(player);
			return;
		}
	}

	//Have we entered a secret cover?
	for(auto& secret_cover : current_map.secret_covers) {

		if(!secret_cover.is_hidden()) {

			continue;
		}

		if(d2d::collision::collides_with(player.ent, secret_cover.ent)) {

			discover_secret(player, secret_cover);
		}
	}

	//Have we activated a touch trigger?
	for(auto& trigger : current_map.touch_triggers) {

		//These can be used ad infinitum and are not regular triggers... we
		//could use separate objects, to be honest.
		//Negative tags serve two purposes: enter the credits or enter the ending.
		if(trigger.tag < 0 && d2d::collision::collides_with(player.ent, trigger.ent)) {

			activate_special_touch_trigger(trigger.tag);
		}

		//Any other used triggers are not even considered.
		if(trigger.used) {

			continue;
		}

		if(d2d::collision::collides_with(player.ent, trigger.ent)) {

			activate_touch_trigger(trigger);
		}
	}

	//any collectibles at hand??
	for(auto& _collectible : current_map.collectibles) {

		if(d2d::collision::collides_with(player.ent, _collectible.ent)) {

			pick_up_collectible(player, _collectible);
		}
	}

	for(auto& autopilot_node : current_map.autopilot_nodes) {

		if(autopilot_node.is_used()) {

			continue;
		}

		if(d2d::collision::collides_with(player.ent, autopilot_node.get_entity())) {

			activate_autopilot_node(autopilot_node);
		}
	}

	//Are we breaking any breakable platforms? We check if the player displaced
	//a little below is inside their boxes, only when walking or crouched.
	if(app::player::states::ground==player.state || app::player::states::crouch==player.state) {

		auto player_box_copy=player.ent.get_box();
		--player_box_copy.origin.y;

		for(auto& block : current_map.breaking_platforms) {

			//Already broken blocks can be skipped.
			if(!block.is_solid()) {

				continue;
			}

			//Blocks over the player can be skipped (think about transparent
			//blocks!, we don't want them to break when we walk into them!)
			if(!d2d::collision::is_above(player.ent, block)) {

				continue;
			}

			if(d2d::collision::collides_with(block, player_box_copy)) {

				if(block.is_ok()) {

					block.start_breaking();
					play_sound(app::snd_block_break);
					create_breaking_block_particles(block);
				}
			}
		}
	}

	//are we about to be launched somewhere?
	for(auto& block : current_map.push_triggers) {

		if(block.is_active() && d2d::collision::collides_with(player.ent, block.ent)) {

			block.activate();
			player.launch(block.velocity);
			play_sound(app::snd_launch_player);
			break; //just one launch per tic.
		}
	}

	//at the end of the tic, are we touching an exit?
	const app::exit * exitptr{nullptr};
	if(is_on_exit(player, exitptr, true)) {

		attempt_exit(*exitptr);
		return;
	}
}

void main::tic_world(
	tdelta _delta
) {

	//TODO: I guess I could move a lot of stuff to the map so that the map
	//tics????

	current_map.particle_manager.tic(_delta);

	if(nullptr!=background) {

		background->tic(_delta);
		int snd=background->get_sound();
		if(0!=snd) {

			play_sound(snd);
		}
	}

	d2d::motion::mover mover{};
	if(current_map.boss && !current_map.boss->is_defeated()) {

		current_map.boss->setup_facing(
			ldt::get_center(player.ent.get_box()).x
		);
		current_map.boss->tic(_delta);

		for(auto& skull : current_map.skulls) {

			skull.tic(_delta);
		}
	}

	for(auto& trap : current_map.timed_traps) {

		trap.tic(_delta);
	}

	for(auto& button : current_map.buttons) {

		button.tic(_delta);
	}

	for(auto& monster : current_map.linear_monsters) {

		monster.tic(_delta, mover);
	}

	for(auto& monster : current_map.leaping_monsters) {

		if(monster.is_on_air()) {

			simulation.gravity.apply_to(monster.ent, _delta);
		}

		monster.tic(_delta, mover);
	}

	for(auto& gate : current_map.gates) {

		gate.tic(_delta);
	}


	for(auto& projectile : current_map.projectiles) {

		tic_projectile(projectile, _delta);
	}

	for(auto& pg : current_map.projectile_generators) {

		if(pg.tic(_delta)) {

			generate_projectile(pg);
		}
	}

	for(auto& sc : current_map.secret_covers) {

		sc.tic(_delta);
	}

	for(auto& bp : current_map.breaking_platforms) {

		bp.tic(_delta);
	}

	for(auto& pt : current_map.push_triggers) {

		pt.tic(_delta);
	}

	for(auto& bl : current_map.moving_blocks) {

		if(bl.is_crouch_activated() && !player.is_crouched()) {

			continue;
		}

		bl.tic(_delta, mover);

		if(!bl.has_arrived()) {

			continue;
		}

		int next_id=bl.get_next_id();
		if(!next_id) {

			//Terminus station.
			bl.invalidate();
			continue;
		}

		write_moving_block(bl, next_id);
	}

	for(auto& timer : current_map.timers) {

		if(timer.tic(_delta)) {

			lm::log(logger).debug()<<"timer "<<timer.get_tag()<<" will activate target "<<timer.get_target_tag()<<"\n";
			activate_tag(timer.get_target_tag(), false);
		}
	}

	//collectibles produce particles each few tics.
	if(game_timeouts.is_finished(timeout_bonus_particles)) {

		for(const auto& c : current_map.collectibles) {

			auto type=app::prt_bonus;
			auto origin=ldt::get_center(c.ent.get_box());
			current_map.particle_manager.add(type, origin);
		}

		game_timeouts.restart(timeout_bonus_particles);
	}

	tic_repeat_sounds();
}

void main::tic_projectile(
	app::projectile& _projectile,
	tdelta _delta
) {

	d2d::motion::mover mover{};

	//Rising projectiles expire after they slow down enough...
	if(app::projectile::types::vertical==_projectile.get_type()
		&& _projectile.ent.get_motion_vector_y() <= 0.1) {

		create_projectile_end_particles(_projectile);
		_projectile.finish();
		return;
	}

	_projectile.tic(_delta, mover);

	//Outside map boundaries? dissapear at once.
	if(!current_map.is_within_boundaries(_projectile.ent)) {

		_projectile.finish();
		return;
	}

	//TODO: Do we need this?
	if(!_projectile.is_moving()) {

		return;
	}

	//We won't correct the collisions, just blow up.
	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());

	const auto tiles_to_check=adapter.find(
		_projectile.ent, 
		current_map.tile_finder,
		app::filter_tiles_projectile{}
	);

	d2d::collision::aabb_static_checker static_check(_projectile.ent, true);
	static_check.detect_all(tiles_to_check)
		.detect_all(current_map.moving_blocks, spatiable_dereferencer<app::moving_block>{})
		.detect_if(current_map.toggle_blocks, app::thing_filter_toggle_blocks{}, spatiable_dereferencer<app::toggle_block>{});

	if(static_check.has_collision()) {

		_projectile.finish();
		create_projectile_end_particles(_projectile);
		return;
	}
}

void main::generate_projectile(
	const app::projectile_generator& _generator
) {

	app::projectile_creator pc;
	play_sound(app::snd_projectile);

	switch(_generator.get_type()) {

		case app::projectile_generator::types::horizontal:
			current_map.projectiles.push_back(
				pc.create_horizontal(
					_generator.get_spawn_point(),
					_generator.get_velocity()
				)
			);
			return;
		case app::projectile_generator::types::vertical:
			current_map.projectiles.push_back(
				pc.create_vertical(
					_generator.get_spawn_point(),
					_generator.get_velocity()
				)
			);

			return;
		case app::projectile_generator::types::falling:
			current_map.projectiles.push_back(
				pc.create_falling(
					_generator.get_spawn_point(),
					_generator.get_velocity()
				)
			);
			return;
		case app::projectile_generator::types::directed:
			current_map.projectiles.push_back(
				pc.create_targeted(
					_generator.get_spawn_point(),
					_generator.get_velocity(),
					ldt::get_center(player.ent.get_box()),
					0
				)
			);

			return;
	}
}

int main::tic_ground(
	tdelta _delta,
	app::player& _player,
	app::player_input _pli
) {

	//Attempt to grab a ladder with up or down.
	const app::ladder * ladderptr{nullptr};
	if(_pli.y && can_grab_ladder(_player, ladderptr)) {

		grab_ladder(_player, *ladderptr);
		tic_ladder(_delta, _player, _pli);
		return 0;
	}

	if(_pli.activate) {

		//Attempt to enter a doorway...
		const app::exit * exitptr{nullptr};
		if(is_on_exit(_player, exitptr, false)) {

			attempt_exit(*exitptr);
			return 0;
		}

		//Attempt to activate a switch!
		app::button * btnptr{nullptr};
		if(can_activate_button(_player, btnptr)) {

			activate_button(*btnptr);
			return 0;
		}
	}

	//Other thing that may happen: are we falling? Just try with a ghost box
	//a bit below the entity and do the tile check.
	//You know what? maybe we were in a block that started moving downwards
	//and the effect has not applied yet.. So we take that into account.
	if(is_on_air(_player) && !ctracker.is_attached(_player.ent)) {

		start_falling(_player);
		return tic_air(_delta, _player,_pli);
	}

	//From this point on, there may be ground movement: all early returns
	//have been done. First, is the player riding any block?
	d2d::motion::motion_vector passive_mv{0., 0.};
	if(current_map.moving_blocks.size()) {

		passive_mv+=ctracker.attached_vector_for(_player.ent);
		player_motion(_player, passive_mv, _delta);
	}

	//Crouching stuff...
	bool is_crouched=app::player::states::crouch==_player.state;
	if(-1==_pli.y) {

		_player.crouch();
		is_crouched=true;
	}
	else if(is_crouched) {

		_player.stand_up();
		//The only case in which this can happen is when a moving platform
		//is above us. The rest of blocking stuff is static.

		if(!is_in_legal_position(player.ent, true)) {

			_player.crouch();
		}
	}

	//Active horizontal phase...
	if(!_pli.x || is_crouched)  {

		//Player velocity is 100% tied to manual control and goes 0-100.
		_player.ent.set_motion_vector_x(0.);
	}

	if(_pli.x) {

		_player.facing=_pli.x > 0
			? app::faces::right
			: app::faces::left;
	}

	//motion...
	d2d::motion::motion_vector mv{0., 0.};
	if(!is_crouched) {

		mv.x=simulation.walk_max_velocity*(double)_pli.x;
	}

	player.ent.set_motion_vector(mv);
	player_motion(_player, mv, _delta);
	int sides=player_collision(_player, passive_mv+mv, _delta);

	//Jumping... a buffered jump is as good as a jump per se so...
	_pli.jump=_pli.jump || _player.has_jump_buffered();
	if(_pli.jump && !_player.is_crouched()) {

		player_jump(_player);
	}

	//TODO: Sound when landing too???
	return sides;
}

void main::tic_ladder(
	tdelta _delta,
	app::player& _player,
	app::player_input _pli
) {

	//While in a ladder all collisions with the world are ignored and
	//the horizontal velocity is unset.

	if(_pli.y) {

		d2d::motion::mover mover{};
		mover.apply_y(_player.ent, simulation.ladder_max_velocity*(double)_pli.y, _delta);

		//apply the ladder movement constraints.
		player.current_ladder->apply(_player.ent);
	}

	//early exit.
	if(!_pli.jump && !_pli.x) {

		return;
	}


	//Jump out or jump down...
	if(_pli.jump && (_pli.x || -1==_pli.y)) {

		if(-1==_pli.y) {

			_player.drop_out_of_ladder();
		}
		else {

			player_jump_out_of_ladder(_player, _pli.x);
		}
	}

	//Attempt to walk out.
	if(_pli.x) {

		//There can be no exit if we are stuck in tiles.
		d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
		const auto tiles_to_check=adapter.find(_player.ent, current_map.tile_finder);

		d2d::collision::aabb_static_checker cc(_player.ent);
		if(!cc.detect_all(tiles_to_check).has_collision()) {

			auto player_box_copy=_player.ent.get_box();
			player_box_copy.origin.y-=4;

			auto landing_candidates=adapter.find(player_box_copy, current_map.tile_finder);

			d2d::collision::aabb_checker c(player_box_copy);
			const auto& landing_tiles=c.get_collisions(landing_candidates);
			if(landing_tiles.size()) {

				walk_out_of_ladder(_player, *landing_tiles[0], _pli.x);
			}
		}
	}
}

int main::tic_air(
	tdelta _delta,
	app::player& _player,
	app::player_input _pli
) {

	//TODO: Repeated shit... this and the next block are mostly the same in
	//tic_ground.
	//Attempt to grab a ladder with up or down.
	const app::ladder * ladderptr{nullptr};
	if(_pli.y && can_grab_ladder(_player, ladderptr)) {

		grab_ladder(_player, *ladderptr);
		tic_ladder(_delta, _player,_pli);
		return 0;
	}

	if(_pli.activate) {

		//Attempt to activate a switch!
		app::button * btnptr{nullptr};
		if(can_activate_button(_player, btnptr)) {

			activate_button(*btnptr);
		}
	}

	if(_pli.jump) {

		//Last chance to jump after we begin falling...
		if(_player.has_jump_last_chance()
			&& _player.ent.get_motion_vector_y() < 0.
		) {

			player_jump(_player);
		}
		//Else, attempt to buffer a jump.
		else {

			_player.buffer_jump();
		}
	}


	//Some controls apply here: Air control is limited, but one can attempt to do it.
	if(_pli.x) {

		tools::ranged_value<double> velocity{-simulation.walk_max_velocity, simulation.walk_max_velocity, player.ent.get_motion_vector_x()};

		velocity+=(double)_pli.x*simulation.air_x_velocity_acceleration_value;
		_player.ent.set_motion_vector_x(velocity);
	}
	//No horizontal input means slowing down by a tiny factor.
	else {

		auto x=_player.ent.get_motion_vector_x();
		_player.ent.set_motion_vector_x(x/simulation.air_x_velocity_reduce_factor);
	}

	//Letting go of the jump shortens it...
	if(player.ent.get_motion_vector_y() > 0. && !_pli.hold_jump && !_player.jump_shortened) {

		auto y=player.ent.get_motion_vector_y();
		_player.ent.set_motion_vector_y(y/simulation.air_y_velocity_jump_shorten_factor);
		_player.jump_shortened=true;
	}

	simulation.gravity.apply_to(_player.ent, _delta);
	player_motion(_player, _player.ent.get_motion_vector(), _delta);

	int sides=player_collision(_player, _player.ent.get_motion_vector(), _delta);

	if(0!=sides) {

		using d2d::collision::ray_aabb_solver;
		if((sides & d2d::collision::right) || (sides & d2d::collision::left)) {

			collide_with_wall(_player);
		}

		if(sides & d2d::collision::top) {

			land_on_ground(_player);
		}
		else if(sides & d2d::collision::bottom) {

			touch_ceiling(_player);
		}
	}

	return sides;
}

void main::tic_defeat(
	tdelta _delta,
	app::player& _player,
	app::player_input
) {

/**
 * no input is taken during this phase.
 */

	d2d::motion::mover mover{};
	simulation.gravity.apply_to(_player.ent, _delta);
	mover.apply(_player.ent, _delta);

	if(_player.is_defeat_timeout_done()) {

		if(game_session.is_with_lives()) {

			game_timeouts.restart(timeout_lives_banner);
		}

		restart_level();
		return;
	}
}

void main::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

#ifdef IS_DEBUG_BUILD

	debug_draw
		? draw_debug(_screen)
		: draw_scene(_screen);

	if(console_enabled) {

		console_display->draw(_screen);
	}

#else

	draw_scene(_screen);
#endif
}

void main::draw_scene(
	ldv::screen& _screen
) {

	//Draw in transition, out transition or... none.
	app::map_transition * active_transition{nullptr};
	active_transition=transition 
		? transition.get()
		: transition_out.get();

	gd.draw(
		_screen, 
		current_map, 
		player, 
		game_session.get_discovered_map_count(),
		active_transition
	);

	if(current_map.in_game && game_timeouts.is_running(timeout_area_banner)) {

		gd.draw_area_name_banner(_screen);
	}

	if(game_session.is_with_lives() && game_timeouts.is_running(timeout_lives_banner)) {

		gd.draw_lives_banner(_screen);
	}
}


void main::grab_ladder(
	app::player& _player,
	const app::ladder& _ladder
) {

	_player.stand_up();
	_player.state=app::player::states::ladder;
	_player.ent.set_motion_vector_x(0.0);
	_player.ent.set_motion_vector_y(0.0);
	_player.current_ladder=&_ladder;

	//Constrict the position already.
	_ladder.apply(_player.ent);
}

void main::walk_out_of_ladder(
	app::player& _player,
	const d2d::collision::tile& _tile,
	int _x_force
) {
	d2d::collision::snap_to_top_of(player.ent, _tile);
	_player.walk_out_of_ladder(_x_force);
}

void main::pick_up_collectible(
	app::player& /*_player,*/,
	app::collectible& _collectible
) {

	//Does not actually make the collectible dissapear :P.
	_collectible.mark_to_remove();

	persistence.add(app::pergr_collectibles, _collectible.id, 1);

	//play a jingle :D.
	play_sound(app::snd_item_pickup);

	//Spawn a lot of particles...
	for(int i=0; i<12; i++) {
		current_map.particle_manager.add(
			app::prt_pickup, 
			ldt::get_center(_collectible.ent.get_box())
		);
	}

	switch(_collectible.type) {

		case app::collectible::gold_ingot:
		case app::collectible::gem:
		case app::collectible::ruby:
		case app::collectible::diamond:

			++inventory.treasure;
		break;
		case app::collectible::yellow_key:

			inventory.yellow_keys++;
		break;
		case app::collectible::blue_key:

			inventory.blue_keys++;
		break;
		case app::collectible::red_key:

			inventory.red_keys++;
		break;
		case app::collectible::green_key:

			inventory.green_keys++;
		break;
		case app::collectible::white_key:

			inventory.white_keys++;
		break;
		case app::collectible::ultimate:

			inventory.ultimate++;
		break;
	}
}

void main::discover_secret(
	app::player&,
	app::secret_cover& _secret_cover
) {

	//Does not actually make the collectible dissapear :P.
	persistence.add(app::pergr_secret_covers, _secret_cover.id, 1);
	_secret_cover.discover();
	play_sound(app::snd_secret);
}

void main::land_on_ground(
	app::player& _player
) {

	_player.ent.set_motion_vector_y(0.);
	_player.state=app::player::states::ground;
}

void main::touch_ceiling(
	app::player& _player
) {

	_player.ent.set_motion_vector_y(0.);

	auto x=_player.ent.get_motion_vector_x();
	_player.ent.set_motion_vector_x(x/2.);
}

void main::start_falling(
	app::player& _player
) {

	_player.start_falling();
}

void main::activate_button(
	app::button& _button
) {

	_button.toggle();

	//Buttons that can be "reused" when the level reloads are not added to
	//any persistence group.
	if(_button.keep_used_when_reset) {

		int persistence_value=_button.used ? 1 : 0;
		persistence.has(app::pergr_buttons, _button.id)
			? persistence.set(app::pergr_buttons, _button.id, persistence_value)
			: persistence.add(app::pergr_buttons, _button.id, persistence_value);
	}

	//Consume the key if needed.
	switch(_button.type) {

		case app::button::types::regular: break;
		case app::button::types::yellow_keyhole:
			inventory.yellow_keys--;
		break;
		case app::button::types::blue_keyhole:
			//TODO: particles and sound!
			inventory.blue_keys--;
		break;
		case app::button::types::red_keyhole:
			//TODO: particles and sound!
			inventory.red_keys--;
		break;
		case app::button::types::green_keyhole:
			//TODO: particles and sound!
			inventory.green_keys--;
		break;
		case app::button::types::white_keyhole:
			//TODO: particles and sound!
			inventory.white_keys--;
		break;
	}

	activate_tag(_button.tag, false);
	play_sound(app::snd_switch);
}

void main::tic_repeat_sounds() {

	//Ok, this is easy... Count if there's any fire traps here
	//that are active or whatever.

	int active_count=0;

	for(const auto& trap : current_map.traps) {

		if(app::trap::types::fire!=trap.get_type()) {

			continue;
		}

		if(!trap.is_active()) {

			continue;
		}

		++active_count;
	}

	//TODO: No need if we already had an active count, right.
	for(const auto& trap : current_map.timed_traps) {

		if(app::timed_trap::types::fire!=trap.get_type()) {

			continue;
		}

		if(!trap.is_harmful()) {

			continue;
		}

		++active_count;
	}

	//Nothing active? was playing and must be stopped?
	if(0==active_count) {

		if(-1!=fire_trap_audio_channel_index) {

			sound_player.stop(fire_trap_audio_channel_index);
			fire_trap_audio_channel_index=-1;
		}

		return;
	}

	//Nothing playing? Please do play!
	if(-1==fire_trap_audio_channel_index) {

		fire_trap_audio_channel_index=sound_player.play_repeat(app::snd_fire);
	}
}

void main::activate_touch_trigger(
	app::touch_trigger& _trigger
) {

	if(_trigger.is_keep_used_when_reset()) {

		persistence.add(app::pergr_touch_triggers, _trigger.id, 1);
	}

	_trigger.used=true;
	activate_tag(_trigger.tag, false);
}

void main::activate_special_touch_trigger(
	int _tag
) {

	switch(_tag) {

		case -1:
			start_ending();
			return;

		case -2:
			start_credits();
			return;
	}

}

void main::start_ending() {

	//TODO: Calculate the ending we get...
	//... and traved to that map.
}

void main::start_credits() {

	//Fade to black in one second...
	transition.reset(new app::map_transition_fade(
		app::map_transition_fade::colors::color_black,
		0, //to,
		1.0 //one second.
	));

	//Go to the credits when the transition is done.
	transition_exit_info.type=transition_exit::types::into_credits;
}

void main::activate_autopilot_node(
	app::autopilot_node& _node
) {

	_node.use();

	auto val=_node.get_input();
	lm::log(logger).debug()<<"got "<<val<<" from autopilot node\n";
	if(-1==val) {

		autopilot.disable();
		return;
	}

	autopilot.receive(val, _node.get_pause());
}

void main::collide_with_wall(
	app::player& _player
) {
	auto x=_player.ent.get_motion_vector_x();

	_player.ent.set_motion_vector_x(x/simulation.air_x_velocity_collision_reduce_factor);
}

void main::defeat(
	app::player& _player
) {

#ifdef IS_DEBUG_BUILD

	//God mode :D.
	if(inmortal) {

		return;
	}
#endif

	//Avoid being defeated time and again.
	if(app::player::states::defeat==_player.state) {

		return;
	}

	if(game_session.is_with_lives()) {

		if(0==game_session.lives) {

			game_over();
			return;
		}

		--game_session.lives;
		gd.setup_lives_banner(game_session.lives);
		//TODO: save game at our current map and entry id!
		//TODO: What is the name of our map xD????
		//save_game(, last_entry_id);
	}

	play_sound(app::snd_defeat);
	_player.defeat(simulation.defeat_y_velocity);
}

void main::setup_camera(
	int _screen_w,
	int _screen_h
) {

	unsigned w=_screen_w /4;
	unsigned h=_screen_h /4;
	int x=(_screen_w /8) * 3;
	int y=(_screen_h /8) * 3;

	ldv::rect margin{x, y, w, h};

	camera.set_coordinate_system(ldv::camera::tsystem::cartesian);
	camera.set_center_margin(margin);

#ifdef IS_DEBUG_BUILD
	dd.set_center_margin(margin);
#endif
}

bool main::is_on_air(
	const app::player& _player
) const {

	auto player_box_copy=_player.ent.get_box();
	--player_box_copy.origin.y;

	//Quicky filter out tiles not in contact...
	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	auto contacting_tiles=adapter.find(
		player_box_copy,
		current_map.tile_finder,
		app::filter_tiles_check_on_air{_player.ent.get_previous_box()}
	);

	//fine collision phase now, with early exits.
	d2d::collision::aabb_static_checker cc(player_box_copy, true);
	app::thing_filter_moving_block moving_block_filter{_player.ent.get_previous_box(), _player.ent.get_motion_vector_y() > 0.};
	app::thing_filter_breaking_platorms breaking_platform_filter{_player.ent.get_previous_box(), _player.ent.get_motion_vector_y() > 0.};

	if(cc.detect_all(contacting_tiles)
		.detect_all(current_map.platform_blocks)
		.detect_if(current_map.moving_blocks, moving_block_filter, spatiable_dereferencer<app::moving_block>{})
		.detect_if(current_map.breaking_platforms, breaking_platform_filter)
		.detect_if(current_map.facing_blocks, app::thing_filter_facing_blocks{}, spatiable_dereferencer<app::facing_block>{})
		.detect_if(current_map.toggle_blocks, app::thing_filter_toggle_blocks{}, spatiable_dereferencer<app::toggle_block>{})
		.has_collision()
	) {

		return false;
	}

	return true;
}

bool main::is_on_exit(
	const app::player& _player,
	const app::exit *&_exitptr,
	bool _is_touch
) const {

	for(const auto& exit : current_map.exits) {

		if(exit.is_touch()==_is_touch && d2d::collision::collides_with(_player.ent, exit.ent)) {

			_exitptr=&exit;
			return true;
		}
	}

	return false;
}

bool main::can_grab_ladder(
	const app::player& _player,
	const app::ladder *&_ladderptr
) const {

	if(!_player.is_ladder_timeout_done()) {

		return false;
	}

	d2d::collision::aabb_checker cc(_player.ent);
	const auto ladders=cc.get_collisions(current_map.ladders);
	if(0==ladders.size()) {

		return false;
	}

	_ladderptr=ladders[0];
	return true;
}

bool main::can_activate_button(
	const app::player& _player,
	app::button *& _btnptr
) {

	for(app::button& button : current_map.buttons) {

		if(d2d::collision::collides_with(_player.ent, button.ent)) {

			if(!button.can_be_activated()) {

				continue;
			}

			if(!has_key(_player, button)) {

				play_sound(app::snd_forbidden);
				continue;
			}

			_btnptr=&button;
			return true;
		}
	}

	return false;
}

bool main::is_into_harm(
	const app::player& _player
) const {

	//Failsafe for falling out of the world scenarios.
	if(_player.ent.get_y() < -300.) {

		return true;
	}

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	auto harm_tiles=adapter.find(_player.ent, current_map.tile_finder, app::filter_tiles_harm_only{});

	d2d::collision::aabb_static_checker cc(_player.ent);
	return cc.detect_all(harm_tiles).has_collision();
}

bool main::has_key(
	const app::player& /**_player*/,
	const app::button& _button
) const {

	switch(_button.type) {

		case app::button::types::regular:

			//This kind does not need any type of key.
			return true;

		case app::button::types::yellow_keyhole:

			return inventory.yellow_keys > 0;
		case app::button::types::blue_keyhole:

			return inventory.blue_keys > 0;
		case app::button::types::red_keyhole:

			return inventory.red_keys > 0;
		case app::button::types::green_keyhole:

			return inventory.green_keys > 0;
		case app::button::types::white_keyhole:

			return inventory.white_keys > 0;
	}

	return false;
}

app::entry main::find_entry_by_id(
	int _id
) const {

	auto it=std::find_if(
		std::begin(current_map.entries),
		std::end(current_map.entries),
		[_id](const app::entry& _entry) -> bool {return _entry.id==_id;}
	);

	if(std::end(current_map.entries) != it) {

		lm::log(logger).info()<<"located map entry "<<_id<<"\n";
		return *it;
	}

	std::stringstream ss;
	ss<<"could not find entry "<<_id<<" by id";
	throw std::runtime_error(ss.str());
}

void main::activate_tag(
	int _tag,
	bool _previously_activated
) {

	//Gates can ONLY open and remain open until the end of the game.
	for(auto& gate : current_map.gates) {

		if(gate.tag!=_tag) {

			continue;
		}

		bool was_closed=gate.is_closed();

		//there is an "animation" here we might want to skip.
		_previously_activated
			? gate.open()
			: gate.activate();

		//Only make noise if the gate was closed beforehand. This comes
		//in handy if two switches activate the same gate.
		if(!_previously_activated && was_closed) {

			play_sound(app::snd_open_gate);
		}
	}

	//Toggle all traps by this tag...
	for(auto& trap : current_map.traps) {

		if(trap.get_tag()!=_tag) {

			continue;
		}

		trap.toggle();
	}

	for(auto& trap : current_map.timed_traps) {

		if(trap.get_tag()!=_tag) {

			continue;
		}

		trap.toggle();
	}

	for(auto& generator : current_map.projectile_generators) {

		if(generator.get_tag()==_tag) {

			generator.toggle();
		}
	}

	for(auto& block : current_map.moving_blocks) {

		if(block.get_tag()==_tag) {

			block.toggle();
		}
	}

	for(auto& node : current_map.text_nodes) {

		if(node.tag!=_tag) {

			continue;
		}

		//Add it to the persistence layer so it's never loaded again.
		persistence.add(app::pergr_texts, node.event_id, 1);
		auto& text_exchange=sp.get_show_text_exchange();

		text_exchange.set(node.text_index).set_colour(node.colour).clear_answers();

		if(""!=node.first_answer_index
			&& "" !=node.second_answer_index
			&& "" !=node.third_answer_index
		) {

			text_exchange.set_answers(
				node.first_answer_index,
				node.second_answer_index,
				node.third_answer_index
			);
		}

		push_state(controller::state_show_text);
	}

	for(auto& timer : current_map.timers) {

		if(timer.get_tag()==_tag) {

			timer.receive_signal();
		}
	}

	//TODO: We are NOT protected from endless loops here!!!! Can we do
	//something? I guess we can pass a third parameter and disallow more than 
	//16 recursions... Which should be more than enough. We can also detect
	//loops by having a vector of relay ids and blah blah.
	for(auto& relay : current_map.tag_relays) {

		if(relay.tag==_tag) {

			for(auto relayed_tag : relay.relays) {

				lm::log(logger).debug()<<"relay "<<relay.tag<<" will relay to "<<relayed_tag<<"\n";
				activate_tag(relayed_tag, _previously_activated);
			}
		}
	}
}

/**
 * basic fire and forget sound playing.
 */
int main::play_sound(
	int _index
) {

	auto result=sound_player.play_once(_index);
	if(result < 0) {

		lm::log(logger).notice()<<"could not play sound, result was "<<result<<"\n";
	}

	return result;
}

void main::save_game(
	const std::string& _map_name,
	int _entry_id
) {

	lm::log(logger).info()<<"will save the game now...\n";

	app::savegame save{
		_map_name,
		persistence.save_to_string(),
		_entry_id,
		game_session.skill_level,
		game_session.elapsed_seconds+(int)game_session.game_clock.get_seconds(),
		game_session.get_discovered_map_count(),
		game_session.lives,
		inventory.yellow_keys,
		inventory.blue_keys,
		inventory.red_keys,
		inventory.green_keys,
		inventory.white_keys,
		inventory.ultimate,
		inventory.treasure
	};

	app::savegame_io sio{};
	sio.save_to_file(
		game_session.savegame_file,
		save
	);
}

void main::setup_area_banner(
	const std::string& _area_name
) {

	game_timeouts.restart(timeout_area_banner);
	gd.setup_area_name_banner(_area_name);
}

void main::clear_transient_state() {

	ctracker.restart();

	if(-1!=fire_trap_audio_channel_index) {

		sound_player.stop(fire_trap_audio_channel_index);
		fire_trap_audio_channel_index=-1;
	}
}

void main::player_motion(
	app::player& _player,
	d2d::motion::motion_vector _mv,
	tdelta _delta
) {

	d2d::motion::mover mover{};
	mover.apply(_player.ent, _mv, _delta);
}

int main::player_collision(
	app::player& _player,
	d2d::motion::motion_vector _mv,
	tdelta _delta
) {

	//First find out which tiles we need to check. We can easily make composite 
	//filters by chaining calls...
	struct {
		const d2d::collision::box previous_box;
		bool operator() (
			const d2d::collision::box& _box,
			const d2d::collision::tile& _tile
		) {

			app::filter_tiles_ignore_one_way_above f{previous_box};
			app::filter_remove_harm_tiles f2{};
			return f(_box, _tile) && f2(_box, _tile);
		}
	} composite_filter{_player.ent.get_previous_box()};

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	auto current_tiles=adapter.find(
		_player.ent,
		current_map.tile_finder,
		composite_filter
	);

	app::thing_filter_moving_block moving_block_filter{_player.ent.get_previous_box(), _player.ent.get_motion_vector_y() > 0.};
	app::thing_filter_breaking_platorms breaking_platform_filter{_player.ent.get_previous_box(), _player.ent.get_motion_vector_y() > 0.};
	
	//Collision...
	d2d::collision::ray_builder rb;
	auto player_ray=rb.get_previous(_player.ent, _mv)*_delta;
	d2d::collision::ray_aabb_phase cph(_player.ent, player_ray);

	cph.flags(d2d::collision::ray_aabb_phase::flag_skip_passable_side_check)
		.detect_all(current_tiles)
		.detect_if(current_map.breaking_platforms, breaking_platform_filter)
		.detect_if(current_map.facing_blocks, app::thing_filter_facing_blocks{}, spatiable_dereferencer<app::facing_block>{})
		.detect_if(current_map.toggle_blocks, app::thing_filter_toggle_blocks{}, spatiable_dereferencer<app::toggle_block>{})
		.detect_all(current_map.gates, spatiable_dereferencer<app::gate>{})
		.detect_if(current_map.moving_blocks, moving_block_filter, spatiable_dereferencer<app::moving_block>{})
		.detect_all(current_map.platform_blocks);

	//One more thing... we are riding a plataform, it may be moving down
	//and apply a downwards vector to us so we enter the platform. Detect_if
	//failed because this block is below the player.
	//We could attempt to enhance the moving block filter so it can thake
	//care of this but a) this works and b) that filter is used in other
	//contexts with non-current player positions.
	if(ctracker.is_attached(player.ent)) {

		cph.detect_one(*ctracker.get_host(player.ent));
	}

	if(!cph.has_collision()) {

		return 0;
	}

	//Now, this is something the engine cannot solve for us: the collision
	//info must be sorted but the default algorithm of "sort_and_solve" is not
	//enough. We must prioritise the X axis over the Y one to avoid glitches
	//that happen when a player has a perfect corner collision with a block at
	//its side and one directly underneath the side block: if the player moves
	//at the side and gravity pulls down we end up with two collisions at
	//the same "distance" that we cannot correctly solve.

	using d2d::collision::ray_aabb_info;
	auto collision_info=cph.get_results();

	std::sort(
		std::begin(collision_info),
		std::end(collision_info),
		//remember, returns true if _one goes before the second!
		[](const ray_aabb_info& _one, const ray_aabb_info& _other) {

			//Prioritise X collisions when intersection time is the same.
			return _one.intersection_time != _other.intersection_time
				? _one.intersection_time < _other.intersection_time
				: abs(_one.normal.x) > abs(_other.normal.x);
		}
	);

	d2d::collision::ray_aabb_solver solver{};
	auto result=solver.solve(_player.ent, collision_info);
	return result.edges;
}

bool main::is_in_legal_position(
	const d2d::collision::spatiable& _spatiable,
	bool _with_moving,
	int _edges
) {

	return is_in_legal_position(_spatiable.get_box(), _with_moving, _edges);
}

bool main::is_in_legal_position(
	const d2d::collision::box& _position,
	bool _with_moving,
	int _edges
) {

	struct {
		bool operator() (
			const d2d::collision::box& _box,
			const d2d::collision::tile& _tile
		) {
			app::filter_tiles_ignore_one_way f{};
			app::filter_remove_harm_tiles f2{};
			return f(_box, _tile) && f2(_box, _tile);
		}
	} composite_filter;

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	auto current_tiles=adapter.find(
		_position,
		current_map.tile_finder,
		composite_filter
	);

	d2d::collision::aabb_static_checker sc(_position);
	sc.detect_all(current_tiles);
if(sc.has_collision()) {

	lm::log(logger).debug()<<"with tiles\n";
}

	if(_with_moving) {

		//Am I riding a block? Am I being crushed by it because of engine 
		//problems? See, we may be riding a block that pushes us to the 
		//ceiling or wherever. Because we are riding, the collision tracker
		//does not correct it as snaps but rather produces the passive 
		//vector that is applied to the player and corrected. At the end of
		//the tic we are INTO the elevator, thus this check.
		//With lower precisions there comes a problem in whicch the player
		//enters the elevator with it goes up... Just check for any previous
		//collisions with other edges to know if there was a previous collision
		//and if we are into the platform, we are crushed.

		if(ctracker.is_attached(player.ent) && 0!=_edges) {

			sc.detect_one(*ctracker.get_host(player.ent));
		}

		//Check against other moving blocks... This is mostly to know if we
		//can stand up when under a moving block.
		app::thing_filter_moving_block moving_block_filter{_position, false};
		sc.detect_if(current_map.moving_blocks, moving_block_filter, spatiable_dereferencer<app::moving_block>{});
	}

	if(sc.has_collision()) {

		lm::log(logger).debug()<<"illegal position for box "<<_position<<" against... \n";
		for(auto& s : sc.get_results()) {

			lm::log(logger).debug()<<" >> "<<s->get_box()<<"\n";
		}
	}

	return !sc.has_collision();
}

void main::mount_player_in_blocks(
	app::player& _player
) {

	if(!current_map.moving_blocks.size()) {

		return;
	}

	//This value can be tweaked so faster boxes catch the player too! 
	//6 catches a box moving downwards at 100u/s (meaning the player cannot
	//actually detach, mind you!).
	const auto margin=2.0;

	//First we need to see if the player has detached from any block it was
	//riding.
	auto player_box_copy=_player.ent.get_box();
	player_box_copy.origin.y-=margin;
	if(ctracker.is_attached(_player.ent)) {

		auto  host=ctracker.get_host(_player.ent);
		//If we are still over the attached block, everything is fine.
		if(d2d::collision::collides_with(player_box_copy, *host)) {

			return;
		}

		//Or we can detach and see if we get attached to something later.
		lm::log(logger).debug()<<"detached from moving platform player:"<<_player.ent.get_box()<<" check_box:"<<player_box_copy<<" host:"<<host->get_box()<<std::endl;
		ctracker.detach_from_all(_player.ent);
	}

	//This takes place AFTER the player tic... if a block is under the player,
	//we can mount it. But not if we are upwards bound so that we do not snap
	//to the floor when we have still upwards momentum.
	if(_player.ent.get_motion_vector_y() > 0.) {

		return;
	}

	for(const auto& plat : current_map.moving_blocks) {

		if(d2d::collision::collides_with(plat.ent, player_box_copy)) {

			//Will not mount blocks above the real player box, this can 
			//happen with blocks that are not fully solid... Still we need to 
			//take the previous position into account: I saw the player move
			//through a box that was moving down.
			if(!d2d::collision::is_below(plat.ent, player.ent.get_previous_box())) {

				continue;
			}

			//TODO: Is this even needed??? the is_in_legal_position thingy. I 
			//am sure this is here because of some reason.
			if(is_in_legal_position(player_box_copy, false)) {

				ctracker.attach(plat.ent, _player.ent);
				//Alternatively, we could attempt to apply a vector that makes us attach to it.
				snap_to_top_of(_player.ent, plat.ent);
				land_on_ground(_player);
				lm::log(logger).debug()<<"attached to moving platform!"<<std::endl;
				break;
			}
		}
	}
}

void main::setup_moving_blocks() {

	if(!current_map.moving_blocks.size()) {

		return;
	}

	using namespace d2d::collision;

	struct moving_blocks_can_push_policy 
		: public collision_tracker::can_push_policy_interface {

		moving_blocks_can_push_policy(const app::moving_block& _block)
			:block{_block}
		{}

		bool operator()(const spatiable&, const spatiable&) const {

			return block.is_solid();
		}

		private:

		const app::moving_block& block;
	};

	//Setup the trackers...
	ctracker.target(player.ent);
	for(auto& block : current_map.moving_blocks) {

		ctracker.watch(block.ent, new moving_blocks_can_push_policy(block));
		write_moving_block(block, block.get_next_id());
	}
}

/**
 * given a block and an id, write the information for the block's next
 * outing. The block will move towards the _target_id waypoint!!
 */
void main::write_moving_block(
	app::moving_block& _block,
	int _target_id
) {

	//Retrieve the node by ids id...
	if(!current_map.moving_block_nodes.count(_target_id)) {

		lm::log(logger).warning()<<"missing block waypoint"<<_target_id<<"\n";
		_block.invalidate();
		return;
	}

	//Set the block target so it moves towards the waypoint. Set also the NEXT
	//waypoint for once we arrive. The means that the block will start moving
	//towards its first set waypoint at that waypoint's velocity when the map
	//loads. Whenever a new waypoint is reached we call this again with the
	//new waypoint's next id.
	const auto& waypoint=current_map.moving_block_nodes.at(_target_id);
	lm::log(logger).info()<<"moving block is set towards "<<waypoint.point<<"\n";
	_block.set_target(waypoint.point, waypoint.velocity, waypoint.wait_ms, waypoint.nextid);
}

void main::player_jump(
	app::player& _player
) {

	_player.jump(simulation.jump_force);
	play_sound(app::snd_jump);
	toggle_blocks(_player);
}

void main::player_jump_out_of_ladder(
	app::player& _player,
	double _x
) {

	//_x_force is -1 or 1.
	double velocity=_x*simulation.walk_max_velocity;
	_player.jump_out_of_ladder(velocity, simulation.jump_force);
	play_sound(app::snd_jump);
	toggle_blocks(_player);
}

void main::toggle_blocks(
	app::player& _player
) {

	for(auto& block : current_map.toggle_blocks) {

		//TODO: Particles when they dissapear???
		block.toggle();
		if(block.is_active() && d2d::collision::collides_with(block.ent, _player.ent)) {

			defeat(_player);
		}
	}
}

void main::sync_facing_blocks() {

	for(auto& block : current_map.facing_blocks) {

		block.sync(player.facing);
	}
}

/**
* we can pass around an optional TODO: SO WHAT??? Complete the docs!
*/
bool main::is_map_complete(
	const app::exit* _exit
) const {

	if(current_map.collectibles.size()) {

		lm::log(logger).debug()<<"there are "<<current_map.collectibles.size()<<" collectibles in the map still"<<std::endl;
		return false;
	}

	//check locked and special exits...
	const auto& automap_srv=sp.get_automap();
	app::automap_game automap{automap_srv};
	for(const auto& exit : current_map.exits) {

		//special exits will not be loaded once visited. Still, we check against
		//the next map name, in case we are currently exiting through this one.
		//TODO: WILL THIS MAKE SPECIAL EXITS NEVER BE COUNTED???
		if(exit.is_special() && nullptr!=_exit && exit!=*_exit) {

			lm::log(logger).debug()<<"there are special exits in the map"<<std::endl;
			return false;
		}

		//Skip regular exits, skip also if we are entering a locked exit so 
		//that we end up returning true.
		if(0==exit.min_rooms) {

			continue;
		}

		//This is a special counted door: If we are exiting through it then
		//it's ok, do not check if we already discovered what lies behind it.
		//and continue checking all other exits.
		if(nullptr!=_exit && *_exit==exit) {

			continue;
		}

		int automap_id=automap.map_id_from_map(exit.map_filename);
		if(!persistence.has(app::pergr_automap, automap_id)) {

			lm::log(logger).debug()<<"there are undiscovered exits in the map"<<std::endl;
			return false;
		}
	}

	lm::log(logger).debug()<<"map is complete"<<std::endl;

	return true;
}

//TODO: Maybe we should use the id...
void main::mark_map_as_complete() {

	persistence.set(
		app::pergr_automap,
		game_session.current_map_id,
		app::am_complete
	);
}

void main::discover_map(
	int _automap_id,
	bool _add_to_tally
) {

	lm::log(logger).debug()<<"will add id "<<_automap_id<<" to automap..."<<std::endl;
	persistence.add(
		app::pergr_automap,
		_automap_id,
		app::am_discovered
	);

	lm::log(logger).debug()<<"total discovered: "<<game_session.get_discovered_map_count()<<std::endl;

	//TODO: Sooo just make them as "non_game"? But the banner won´t show...
	//we can always separate!
	
	//If the area is not "nowhere" we can discover this new room. Nowhere
	//maps don't count for this tally.
	if(_add_to_tally) {

		++game_session.discovered_rooms;
	}

	if(is_map_complete()) {

		mark_map_as_complete();
	}
}

void main::boss_create_targeted_projectile(
	d2d::collision::point _origin,
	double _velocity,
	int _extra_angle
) {

	app::projectile_creator pc;

	current_map.projectiles.push_back(
		pc.create_targeted(
			_origin,
			_velocity,
			ldt::get_center(player.ent.get_box()),
			_extra_angle
		)
	);
}

void main::boss_create_directed_projectile(
	d2d::collision::point _origin,
	int _angle,
	double _velocity
) {

	app::projectile_creator pc;

	current_map.projectiles.push_back(
		pc.create_directed(
			_origin,
			_velocity,
			_angle
		)
	);
}

void main::boss_create_linear_projectile(
	d2d::collision::point _origin,
	double _velocity
) {

	app::projectile_creator pc;

	current_map.projectiles.push_back(
		pc.create_horizontal(
			_origin,
			_velocity
		)
	);
}

void main::boss_spawn_skull(
	int _id,
	double _timer
) {

	for(const auto& spawn : current_map.skull_spawns) {

		if(_id==spawn.id) {

			lm::log(logger).debug()<<"spawning skull at point "<<_id<<std::endl;
			current_map.skulls.push_back({spawn.point, _timer});
			for(int i=0; i<4; i++) {
				current_map.particle_manager.add(app::prt_smoke, spawn.point);
			}

			return;
		}
	}

	lm::log(logger).warning()<<"spawn skull point not found:"<<_id<<std::endl;
}

d2d::collision::point main::boss_get_target() const {

	return ldt::get_center(player.ent.get_box());
}

void main::create_projectile_end_particles(
	const app::projectile& _projectile
) {

	auto origin=ldt::get_center(_projectile.ent.get_box());
	auto type=app::prt_projectile_splash;
	int count=0;

	switch(_projectile.get_type()) {

		case app::projectile::types::falling:
		case app::projectile::types::directed:
			count=4;
			type=app::prt_projectile_splash;
		break;
		case app::projectile::types::horizontal:
			type=app::prt_projectile_horizontal_splash;
			count=6;
			particle_mod_projectile_horizontal_splash.set_multiplier_x(
				_projectile.ent.get_motion_vector_x() > 0.
					? -1
					: 1
			);
		break;
		case app::projectile::types::vertical:
			type=app::prt_smoke;
			count=6;
		break;
	}

	for(int i=0; i<count; i++) {
		current_map.particle_manager.add(type, origin);
	}
}

void main::create_breaking_block_particles(
	const app::breaking_platform& _plat
) {

	auto origin=_plat.get_origin();
	for(int i=0; i<4; i++) {
		current_map.particle_manager.add(app::prt_breaking_platform, origin);
	}
}

void main::create_skull_break_particles(
	const app::boss_skull& _skull
) {

	auto origin=_skull.ent.get_origin();
	for(int i=0; i<4; i++) {
		current_map.particle_manager.add(app::prt_smoke, origin);
	}

	for(int i=0; i<8; i++) {
		current_map.particle_manager.add(app::prt_projectile_splash, origin);
	}
}

void main::boss_spawn_particle(
	d2d::collision::point _origin,
	int _type
) {

	current_map.particle_manager.add(_type, _origin);
}

void main::boss_defeat() {

	lm::log(logger).info()<<"boss has been defeated"<<std::endl;

	//Mark the boss as defeated, never again will it show up!.
	persistence.add(app::pergr_events, app::perev_boss_defeated, 1);

	//Remove all projectiles, so the player cannot be killed...
	//TODO: Check this, maybe it still can happen...
	for(auto& projectile : current_map.projectiles) {

		projectile.finish();
		create_projectile_end_particles(projectile);
	}

	//There will be only one button in the boss map, not accesible by the
	//player. Will get activated when the boss is done and will open the
	//arena gates, allowing the player to leave.
	activate_button(current_map.buttons[0]);
}

void main::boss_remove() {

	//Remove the boss instance from the game.
	current_map.boss.reset(nullptr);
}

std::vector<std::string> main::extract_green_key_text_nodes() const {

	std::vector<std::string> result;

	for(const auto& node : current_map.text_nodes) {

		//We are only interested in special nodes with tag 999, which mean
		//that the text will be used by the background.
		if(999!=node.tag) {

			continue;
		}

		result.push_back(sp.get_localization().get(node.text_index));
	}

#ifdef IS_DEBUG_BUILD

	lm::log(logger).debug()<<"loaded "<<result.size()<<" background effect texts:\n";
	for(const auto& str : result) {

		lm::log(logger).debug()<<"text is: "<<str<<"\n";
	}

#endif

	return result;
}

#ifdef IS_DEBUG_BUILD

	#include "main.debug.cpp"

#endif

void main::start_game_clock() {

	//When the game is first started it will likely be to an intro map in which
	//the clock does not count so when we start the clock after a map load
	//we need to know if it was started or not.
	if(!game_session.game_clock.is_running()) {

		lm::log(logger).debug()<<"starting game clock\n";
		game_session.game_clock.start();
		return;
	}

	if(game_session.game_clock.is_paused()) {
 
		lm::log(logger).debug()<<"resuming game clock\n";
		game_session.game_clock.resume();
		return;
	}

}

