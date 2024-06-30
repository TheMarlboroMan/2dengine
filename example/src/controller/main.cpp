#include "controller/main.h"
#include "app/input.h"
#include "app/thing_loader.h"
#include "app/map_attribute_loader.h"
#include "app/tile_filter.h"

#include "dfwimpl/config.h"
#include <d2d/storage/map_loader.h>
#include <d2d/video/camera_map_limit.h>
#include <d2d/video/tools.h>
#include <d2d/motion/mover.h>
#include <d2d/collision/phase.h>
#include <d2d/collision/checker.h>
#include <d2d/collision/tiles_in_box.h>
#include <d2d/collision/tile_limits.h>
#include <d2d/tools/algorithm.h>
#include <tools/string_utils.h>
#include <ldtools/ttf_manager.h>
#include <ldv/color.h>
#include <ldv/box_representation.h>
#include <tools/ranged_value.h>
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
#ifdef IS_DEBUG_BUILD
	sp{_sp},
#endif
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	shaper{_sp.get_shaper()},
	tile_impl{_sp.get_tile_impl()},
	audio_resource_manager{_sp.get_audio_resource_manager()},
	music_player{_sp.get_music_player()},
	camera{ {0,0,app::logic_screen_w, app::logic_screen_h}, {0,0}},
	sprite_draw{
		_sp.get_spritesheet_manager().at(app::ss_tiles),
		_sp.get_video_resource_manager().get_texture(app::tex_tiles),
		&camera,
		true
	},
	sprite_draw_animated{
		_sp.get_animation_manager().at(app::animgr_tiles)
	},
	scenery_tile_draw{
		_sp.get_spritesheet_manager().at(app::ss_tiles),
		_sp.get_video_resource_manager().get_texture(app::tex_tiles),
		app::tile_w, 
		app::tile_h,
		_sp.get_animation_manager().at(app::animgr_tiles),
		&camera,
		true
	},
	persistence{_sp.get_persistence()}
#ifdef IS_DEBUG_BUILD
	,
	dd{app::logic_screen_w, app::logic_screen_h}
#endif
{

#ifdef IS_DEBUG_BUILD

	setup_console(_sp);
	dd.set_background_color(ldv::rgba_color(128, 128, 128, 0));
	reload_values();

#endif

	//TODO: This kind of shit could come from any file.
	scenery_tile_draw.set_is_animation_fn(
		[](int _index) -> bool {
			return _index==app::spr_water_surface || _index==app::spr_waterfall || _index==app::spr_lava_surface;
		}
	);

	//TODO: Same, this should come from a file.
	scenery_tile_draw.set_index_to_animation_fn(
		[](int _index) -> int {

			switch(_index) {
				case app::spr_water_surface: return app::anim_water_surface;
				case app::spr_waterfall: return app::anim_waterfall;
				case app::spr_lava_surface: return app::anim_lava_surface;
			}

			return 0;
		}
	);

	setup_camera(
		app::logic_screen_w,
		app::logic_screen_h
	);

	auto music_load_fn=[&](int _id) {

		auto path=_sp.get_music_id_mapper().get(_id);
		return env.build_app_path(path);
	};

	music_player.set_load_music_function(music_load_fn);
	music_player.set_unload_finished(true);
}

void main::start(
	const std::string& _map,
	int _entry_id
) {

	load_map(_map);
	take_player_to_entry(player, _entry_id, nullptr);
}

void main::set_difficulty(
	int _value
) {

	switch(_value) {

		case app::skill_easy:
		case app::skill_normal:
		case app::skill_hard:

			difficulty_setting=_value;
			return;
	}

	throw std::runtime_error("bad difficulty setting");
}

void main::awake(
	dfw::input& /*input*/
) {
}

void main::slumber(
	dfw::input& /*input*/
) {

}

void main::loop(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

#ifdef IS_DEBUG_BUILD
	loop_debug(_input, _lid);
#else
	loop_scene(_input, _lid);
#endif
}

void main::loop_scene(
	dfw::input& _input,
	const dfw::loop_iteration_data& _lid
) {

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}

	app::player_input pli{};

	if(_input.is_input_down(app::input::escape)) {

		set_leave(true);
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

	tic(_lid.delta, pli);
}

void main::load_map(
	const std::string& _map_name
) {
	std::stringstream ss;
	ss<<env.build_app_path("resources/maps/")<<_map_name<<".json";

	const std::string map_path{ss.str()};
	lm::log(logger).info()<<"will attempt to load map "<<map_path<<"\n";

	d2d::storage::map_loader loader(map_path);

	current_map.clear();

	loader.load_collision_tiles(
		"logic",
		current_map.collision_tiles,
		shaper,
		tile_impl
	);

	loader.load_scenery_tiles("foreground", current_map.foreground_tiles);
	loader.load_scenery_tiles("background", current_map.background_tiles);

	current_map.sync_tile_finder();

	//we need the maps limits, so...
	d2d::collision::tile_limits_finder tlf{};
	auto limits=tlf.find_limits(current_map.collision_tiles);
	current_map.set_limits(limits);

	app::thing_loader tl{current_map, limits, persistence, difficulty_setting};
	loader.load_thing_layer("things", tl);

	//The loader takes references to the map data.
	app::map_attribute_loader attrl{current_map.background_color, current_map.music_id};
	loader.load_properties(attrl);
	lm::log(logger).info()<<"map musicid is "
		<<current_map.music_id
		<<" and backgroundcolor is "
		<<current_map.background_color.r<<", "
		<<current_map.background_color.g<<", "
		<<current_map.background_color.b<<"\n";

	//Now the music... pieces are loaded in real time so nothing to do here.
	music_player.swap(current_map.music_id, 500);

	//All activated switches should run their course now: all activated objects
	//will not save their state but will be activated when the switches do
	//their thing.
	for(const auto& button : current_map.buttons) {

		if(!button.used) {

			continue;
		}

		activate_tag(button.tag);
	}

	for(const auto& trigger : current_map.touch_triggers) {

		if(!trigger.used) {

			continue;
		}

		activate_tag(trigger.tag);
	}

	//After loading the map, tell the camera where the limits are. We use
	//removing harm tiles/solid but no camera, to allow for these to exist outside
	//the camera boundaries.
	d2d::collision::tile_limits_finder::filter_function ff=
		[](const d2d::collision::tile& _tile) {return _tile.type==app::tile_harm || _tile.type==app::tile_full_no_camera;};

	auto tile_limits_view=tlf.find_limits(current_map.collision_tiles, ff);

	d2d::video::camera_map_limit cml;
	cml.limit_to_collision_tiles(camera, tile_limits_view, shaper.get_tile_w(), shaper.get_tile_h(), &logger);

#ifdef IS_DEBUG_BUILD
	cml.limit_to_collision_tiles(dd.camera, tile_limits_view, shaper.get_tile_w(), shaper.get_tile_h(), &logger);
#endif

//	std::cout<<current_map<<std::endl;
}

void main::exit_to(
	app::player& _player,
	app::exit _exit
) {
/**
 * there is a reason why arguments are copied: the exit belongs to the map
 * which will get unloaded soon and would cause the original reference or
 * pointer to be lost. It has already happened to me so please, do not attemptw
 * to "fix" it.
 */

	player.current_ladder=nullptr;

	lm::log(logger).info()<<"player is on exit to "<<_exit.map_filename<<" with entry id "<<_exit.next_entry_id<<"\n";
	load_map(_exit.map_filename);
	take_player_to_entry(_player, _exit.next_entry_id, &_exit);
}

void main::take_player_to_entry(
	app::player& _player,
	int _id,
	const app::exit * _last_exit
) {

	lm::log(logger).info()<<"will attempt to set player at entry "<<_id<<"\n";
	last_entry_id=_id;
	auto map_entry=find_entry_by_id(last_entry_id);

	auto find_ladders=[this, &map_entry]() {

		d2d::collision::checker cc;
		const auto ladders=cc.get_collisions(map_entry.ent, current_map.ladders);
		lm::log(logger).info()<<"found "<<ladders.size()<<" ladders in the last entry point\n";
		return ladders;
	};

	//If no exit is indicated this is a "hard transition", e.g. reloading after
	//defeat.
	if(nullptr==_last_exit) {

		lm::log(logger).info()<<"taking player to exit (hard mode)\n";

/**
 * about ladder position:
 * when setting a ladder that must span maps we must extend the ladder up/down
 * enough so that it matches the top of the corresponding EXIT and crosses the
 * corresponding entry (or the bottom, if the ladder goes down). This is a
 * workaround on the level design.
 */
		//This is crucial, if there is an entry with a ladder we favor it and
		//place the player on it depending on how the last entry expected the
		//player to be placed. The extra tile_h is to avoid the player colliding
		//with the exit.
		auto ladders=find_ladders();
		if(ladders.size()) {

			lm::log(logger).info()<<"found ladder in entry point, it will be used\n";
			grab_ladder(_player, *ladders[0]);

			if(app::entry::inner_top_edge==map_entry.position) {

				d2d::collision::match_top_of(_player.ent, *ladders[0], app::tile_h);
			}
			else if(app::entry::inner_bottom_edge==map_entry.position) {

				d2d::collision::match_bottom_of(_player.ent, *ladders[0], app::tile_h);
			}
			else {

				throw std::runtime_error("mismatch between ladder and entry");
			}
		}
		else {

			lm::log(logger).info()<<"no ladder in entry point, player will stand up";
			_player.ent.set_origin(map_entry.ent.get_origin());
			_player.stand_up();
		}

		//Stop all velocity.
		_player.velocity={0., 0.};
	}
	else {

		lm::log(logger).info()<<"taking player to exit (soft mode)\n";
		auto& pos=player.ent.get_box();
		auto entry_pt=map_entry.ent.get_origin();
		d2d::collision::point exit_offset=pos.origin-_last_exit->ent.get_origin();

		switch(map_entry.position) {

			case app::entry::position::center_bottom:

				land_on_ground(_player);
				player.velocity.x=0.;
				d2d::collision::center_horizontally(_player.ent, map_entry.ent);
				d2d::collision::match_bottom_of(_player.ent, map_entry.ent);
			break;
			case app::entry::position::inner_top_edge:
				d2d::collision::match_top_of(_player.ent, map_entry.ent);
				_player.ent.set_x(entry_pt.x+exit_offset.x);
			break;

			case app::entry::position::inner_bottom_edge:
				d2d::collision::match_bottom_of(_player.ent, map_entry.ent);
				_player.ent.set_x(entry_pt.x+exit_offset.x);
			break;

			case app::entry::position::inner_right_edge:
				d2d::collision::match_right_of(_player.ent, map_entry.ent);
				_player.ent.set_y(entry_pt.y+exit_offset.y);
			break;

			case app::entry::position::inner_left_edge:
				d2d::collision::match_left_of(_player.ent, map_entry.ent);
				_player.ent.set_y(entry_pt.y+exit_offset.y);
			break;
		}

		//If the player was in a ladder after a soft transition we want to grab the new one.
		if(app::player::states::ladder==player.state) {

			lm::log(logger).info()<<"player was in a ladder, will try to grab a new one\n";
			auto ladders=find_ladders();
			if(!ladders.size()) {

				throw std::runtime_error("there should be a ladder at the entrypoint");
			}

			grab_ladder(_player, *ladders[0]);
		}
	}

	//Commit positions so we don't get phantom collisions later!
	_player.ent.sync_boxes();

	lm::log(logger).debug()<<"new player position is "<<_player.ent.get_origin()<<"\n";
	lm::log(logger).debug()<<"player box at "<<_player.ent.get_box()<<"\n";
	lm::log(logger).debug()<<"player prev box at "<<_player.ent.get_previous_box()<<"\n";

	//Center camera on map now..
	camera.center_on(
		d2d::video::to_screen(player.ent)
	);

#ifdef IS_DEBUG_BUILD
	dd.center_on(player.ent);
#else

#endif
}

void main::restart_level() {

	current_map.projectiles.clear();
	take_player_to_entry(player, last_entry_id, nullptr);
}

void main::tic(
	float _delta,
	app::player_input _pli
) {

	scenery_tile_draw.tic(_delta);
	sprite_draw_animated.tic(_delta);
	player.tic(_delta);

	switch(player.state) {

		case app::player::states::ground:
			tic_ground(_delta, player, _pli);
		break;

		case app::player::states::ladder:
			tic_ladder(_delta, player, _pli);
		break;

		case app::player::states::air:
			tic_air(_delta, player, _pli);
		break;

		case app::player::states::crouch:
			tic_crouch(_delta, player, _pli);
		break;

		case app::player::states::defeat:
			tic_defeat(_delta, player, _pli);
		break;
	}

	tic_world(_delta);

	//aftermath
	if(player.ent.get_origin() != player.ent.get_previous_box().origin) {
	
		player.ent.sync_boxes();

		if(app::player::states::defeat != player.state) {

			camera.center_on(
				d2d::video::to_screen(player.ent)
			);

#ifdef IS_DEBUG_BUILD
			dd.center_on(player.ent);
#endif
		}
	}

	//If the player can do things in the world, run them.
	if(!player.is_defeated()) {

		post_tic();
	}
}

void main::post_tic() {

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

	//Or a projectile?
	for(auto& projectile : current_map.projectiles) {

		if(!projectile.is_moving()) {

			continue;
		}

		if(d2d::collision::collides_with(player.ent, projectile.ent)) {

			projectile.desintegrate();
			defeat(player);
			return;
		}
	}

	//Or a trap?
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

		if(trigger.used) {

			continue;
		}

		if(d2d::collision::collides_with(player.ent, trigger.ent)) {

			activate_touch_trigger(trigger);
		}
	}

	//any collectibles at hand??
	auto it=std::find_if(
		std::begin(current_map.collectibles),
		std::end(current_map.collectibles),
		[this](const app::collectible& _collectible) -> bool {

			return d2d::collision::collides_with(player.ent, _collectible.ent);
		}
	);

	if(it!=std::end(current_map.collectibles)) {

		pick_up_collectible(player, *it);
		it=current_map.collectibles.erase(it);
		it++;
	}

	//Are we breaking any breakable platforms?
	if(app::player::states::ground==player.state || app::player::states::crouch==player.state) {

		auto player_box_copy=player.ent.get_box();
		--player_box_copy.origin.y;

		for(auto& block : current_map.breaking_platforms) {

			if(!block.is_solid()) {

				continue;
			}

			if(d2d::collision::collides_with(block, player_box_copy)) {

				block.start_breaking();
			}
		}
	}

	//at the end of the tic, are we touching an exit?
	const app::exit * exitptr{nullptr};
	if(is_on_exit(player, exitptr, true)) {

		exit_to(player, *exitptr);
		return;
	}
}

void main::tic_world(
	float _delta
) {

	d2d::motion::mover mover{};
	for(auto& trap : current_map.timed_traps) {

		trap.tic(_delta);
	}

	for(auto& monster : current_map.linear_monsters) {

		monster.tic(_delta, mover);
	}

	for(auto& monster : current_map.leaping_monsters) {

		if(monster.is_on_air()) {

			simulation.gravity.apply_to(monster.velocity, _delta);
		}

		monster.tic(_delta, mover);
	}

	for(auto& gate : current_map.gates) {

		gate.tic(_delta);
	}

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	for(auto& projectile : current_map.projectiles) {

		projectile.tic(_delta, mover);

		//Outside map boundaries? dissapear at once.
		if(!current_map.is_within_boundaries(projectile.ent)) {

			projectile.finish();
			continue;
		}

		//We won't correct the collisions, so we will check if the projectile
		//is already moving.
		if(projectile.is_moving() && adapter.has(
			projectile.ent, 
			current_map.tile_finder, 
			app::filter_tiles_projectile{}
		)) {

			projectile.desintegrate();
		}
	}

	//erase-remove idiom, get rid of projectiles we no longer want.
	current_map.projectiles.erase(
		std::remove_if(
			current_map.projectiles.begin(),
			current_map.projectiles.end(),
			[](app::projectile& _proj) {return _proj.is_done();}
		),
		current_map.projectiles.end()
	);

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
}

void main::generate_projectile(
	const app::projectile_generator& _pg
) {

	const auto spawn_data=_pg.get_projectile_data();

	auto type=app::projectile::types::horizontal;
	auto velocity=spawn_data.velocity;

	switch(_pg.get_type()) {

		case app::projectile_generator::types::linear:
			//The defaults are above...
		break;
		case app::projectile_generator::types::directed:

			type=app::projectile::types::round;
			//TODO: Should aim FOR THE CENTER!
			velocity=ldt::vector_from_points(spawn_data.box.origin, player.ent.get_origin());
			velocity.normalize();
			velocity*=fabs(spawn_data.velocity.x);
		break;
	}

	app::projectile proj{
		spawn_data.box.origin,
		velocity,
		type
	};

	d2d::collision::center_vertically(proj.ent, spawn_data.box);
	d2d::collision::center_horizontally(proj.ent, spawn_data.box);
	current_map.projectiles.push_back(proj);
}

void main::tic_ground(
	float _delta,
	app::player& _player,
	app::player_input _pli
) {

	//Attempt to grab a ladder with up or down.
	const app::ladder * ladderptr{nullptr};
	if(_pli.y && can_grab_ladder(_player, ladderptr)) {

		grab_ladder(_player, *ladderptr);
		tic_ladder(_delta, _player, _pli);
		return;
	}

	if(_pli.activate) {

		//Attempt to enter a doorway...
		const app::exit * exitptr{nullptr};
		if(is_on_exit(_player, exitptr, false)) {

			exit_to(_player, *exitptr);
			return;
		}

		//Attempt to activate a switch!
		app::button * btnptr{nullptr};
		if(can_activate_button(_player, btnptr)) {

			activate_button(_player, *btnptr);
			return;
		}
	}

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());

	//Other thing that may happen: are we falling? Just try with a ghost box
	//a bit below the entity and do the tile check.
	if(is_on_air(_player)) {

		start_falling(_player);
		tic_air(_delta, _player,_pli);
		return;
	}

	if(-1==_pli.y) {

		_player.crouch();
		tic_crouch(_delta, _player, _pli);
		return;
	}

	//horizontal phase...
	if(!_pli.x)  {

		//Player velocity is 100% tied to manual control and goes 0-100.
		_player.velocity.x=0.0;
	}
	else {

		//motion...
		d2d::motion::mover mover{};
		_player.velocity.x=simulation.walk_max_velocity*(double)_pli.x;

		_player.facing=_pli.x > 0
			? app::faces::right
			: app::faces::left;

		mover.apply_x(_player.ent, _player.velocity.x, _delta); //instant acceleration..

		//Collision...
		auto current_tiles=adapter.find(_player.ent, current_map.tile_finder, app::filter_tiles_ignore_one_way_above{});
		d2d::collision::phase cph(_player.ent, d2d::collision::checker::phases::horizontal);

		cph.flags(d2d::collision::checker::flag_skip_passable_side_check).detect_all(current_tiles);
		//cph.flags(d2d::collision::checker::flag_skip_passable_side_check).detect_all(current_map.platform_blocks);
		cph.detect_if(current_map.breaking_platforms, breaking_platforms_fn{});
		cph.detect_all(current_map.gates, spatiable_dereferencer<app::gate>{});

		if(cph.has_collision()) {

			cph.response_generic();
		}
	}

	if(_pli.jump) {

		_player.jump(simulation.jump_force);
	}
}

void main::tic_ladder(
	float _delta,
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
	
	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	d2d::collision::checker cc;

	//Jump out or jump down...
	if(_pli.jump && (_pli.x || -1==_pli.y)) {

		//There can be no ladder exit if there are collisions with tiles.
		const auto tiles_to_check=adapter.find(_player.ent, current_map.tile_finder);
		if(!cc.has_collision(_player.ent, tiles_to_check)) {

			if(-1==_pli.y) {

				drop_out_of_ladder(_player);
			}
			else {

				jump_out_of_ladder(_player, _pli.x);
			}
		}
	}

	//Attempt to walk out.
	if(_pli.x) {

		//TODO: These nested ifs are ugly...
		//There can be no exit if we are stuck in tiles.
		const auto tiles_to_check=adapter.find(_player.ent, current_map.tile_finder);
		if(!cc.has_collision(_player.ent, tiles_to_check)) {

			auto player_box_copy=_player.ent.get_box();
			player_box_copy.origin.y-=4;
			auto landing_candidates=adapter.find(player_box_copy, current_map.tile_finder);
			const auto& landing_tiles=cc.get_collisions(player_box_copy, landing_candidates);
			if(landing_tiles.size()) {

				walk_out_of_ladder(_player, *landing_tiles[0], _pli.x);
			}
		}
	}
}

void main::tic_air(
	float _delta,
	app::player& _player,
	app::player_input _pli
) {

	//Attempt to grab a ladder with up or down.
	const app::ladder * ladderptr{nullptr};
	if(_pli.y && can_grab_ladder(_player, ladderptr)) {

		grab_ladder(_player, *ladderptr);
		tic_ladder(_delta, _player,_pli);
		return;
	}

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	d2d::motion::mover mover{};

	//Do the horizontal phase...
	//Air control is limited, but one can attempt to do it.
	if(_pli.x) {

		tools::ranged_value<double> velocity{-simulation.walk_max_velocity, simulation.walk_max_velocity, player.velocity.x};

		velocity+=(double)_pli.x*simulation.air_x_velocity_acceleration_value;
		_player.velocity.x=velocity;
	}
	//No horizontal input means slowing down by a tiny factor.
	else {

		_player.velocity.x/=simulation.air_x_velocity_reduce_factor;
	}

	mover.apply_x(_player.ent, _player.velocity.x, _delta);

	//Collision...
	auto current_tiles=adapter.find(_player.ent, current_map.tile_finder, app::filter_tiles_ignore_one_way{});
	d2d::collision::phase cph(_player.ent, d2d::collision::checker::phases::horizontal);
	
	cph.flags(d2d::collision::checker::flag_skip_passable_side_check).detect_all(current_tiles);
	//cph.flags(d2d::collision::checker::flag_skip_passable_side_check).detect_all(current_map.solid_blocks);
	cph.detect_if(current_map.breaking_platforms, breaking_platforms_fn{});
	cph.detect_all(current_map.gates, spatiable_dereferencer<app::gate>{});

	if(cph.has_collision()) {

		//TODO; A method would be nice.
		cph.response_generic();
		collide_with_wall(_player);
	}

	//do the vertical phase.
	//Last chance to jump after we begin falling...
	if(_pli.jump && _player.timeouts.is_counting(app::player::timeout_last_jump_chance) && _player.velocity.y < 0.) {

		_player.jump(simulation.jump_force);
	}

	//Letting go of the jump shortens it...
	if(player.velocity.y > 0. && !_pli.hold_jump && !_player.jump_shortened) {

		_player.velocity.y/=simulation.air_y_velocity_jump_shorten_factor;;
		_player.jump_shortened=true;
	}

	simulation.gravity.apply_to(_player.velocity, _delta);
	mover.apply_y(_player.ent, _player.velocity.y, _delta);

	//TODO:
	//Actually, this is very bad xD!. We cannot walk onto a ledge... It would
	//be better to just ignore these and check at the END of the tic if the
	//player ended up in a bad position.
	//The first collision to take into account is harm tiles... 
	if(is_into_harm(_player)) {

		defeat(_player);
		return;
	}

	//Collision...
	current_tiles=adapter.find(_player.ent, current_map.tile_finder, app::filter_tiles_ignore_monster_block{});
	d2d::collision::phase cpv(_player.ent, d2d::collision::checker::phases::vertical);

	cpv.detect_all(current_tiles);

	cpv.detect_all(current_map.platform_blocks);
	cpv.detect_if(current_map.breaking_platforms, breaking_platforms_fn{});
	cpv.detect_all(current_map.gates, spatiable_dereferencer<app::gate>{});

	if(cpv.has_collision()) {

		auto response=cpv.response_complex();
		response.solve(_player.ent);

		//Only when colliding when the top of a box can we jump again.
		if(response.edges & d2d::collision::response::tedges::top) {

			land_on_ground(_player);
		}
		else if(response.edges & d2d::collision::response::tedges::bottom) {

			touch_ceiling(_player);
		}
	}
}

void main::tic_crouch(
	float _delta,
	app::player& _player,
	app::player_input _pli
) {

	//Nothing happens here... except for waiting for the player to let go
	//of the crouch input.

	if(-1!=_pli.y) {

		_player.stand_up();
		tic_ground(_delta, _player, _pli);
		return;
	}
}

void main::tic_defeat(
	float _delta,
	app::player& _player,
	app::player_input
) {

/**
 * no input is taken during this phase.
 */

	d2d::motion::mover mover{};
	simulation.gravity.apply_to(_player.velocity, _delta);
	mover.apply(_player.ent, _player.velocity, _delta);

	if(!player.timeouts.is_counting(app::player::timeout_defeat)) {

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

	this.draw_scene(_screen);
#endif
}

void main::draw_scene(
	ldv::screen& _screen
) {

	_screen.clear(current_map.background_color);

	scenery_tile_draw.draw(_screen, current_map.background_tiles);

	for(const auto& node : current_map.buttons) {

		draw_button(_screen, node);
	}

	for(const auto& node : current_map.gates) {

		draw_gate(_screen, node);
	}

	for(const auto& block : current_map.platform_blocks) {

		draw_platform(_screen, block);
	}

	for(const auto& block : current_map.breaking_platforms) {

		draw_breaking_platform(_screen, block);
	}

	for(const auto& ladder : current_map.ladders) {

		draw_ladder(_screen, ladder);
	}

	for(const auto& collectible : current_map.collectibles) {

		draw_collectible(_screen, collectible);
	}

	for(const auto& monster : current_map.linear_monsters) {

		draw_linear_monster(_screen, monster);
	}

	for(const auto& monster : current_map.leaping_monsters) {

		draw_leaping_monster(_screen, monster);
	}

	for(const auto& trap : current_map.timed_traps) {

		if(!trap.is_active()) {

			continue;
		}

		draw_timed_trap(_screen, trap);
	}

	for(const auto& projectile : current_map.projectiles) {

		draw_projectile(_screen, projectile);
	}

	draw_player(_screen, player);
	scenery_tile_draw.draw(_screen, current_map.foreground_tiles);

	for(const auto& secret_cover : current_map.secret_covers) {

		if(secret_cover.is_discovered()) {

			continue;
		}

		draw_secret_cover(_screen, secret_cover);
	}
}

void main::draw_button(
	ldv::screen& _screen,
	const app::button& _button
) {

	int sprite_index=app::spr_key_yellow;
	switch(_button.type) {

		case app::button::types::regular:           sprite_index=app::spr_regular_button; break;
		case app::button::types::yellow_keyhole:    sprite_index=app::spr_yellow_keyhole; break;
		case app::button::types::blue_keyhole:      sprite_index=app::spr_blue_keyhole; break;
		case app::button::types::red_keyhole:       sprite_index=app::spr_red_keyhole; break;
		case app::button::types::green_keyhole:     sprite_index=app::spr_green_keyhole; break;
	}

	if(_button.used) {

		switch(_button.type) {

			case app::button::types::regular:           sprite_index=app::spr_regular_button_used; break;
			case app::button::types::yellow_keyhole:    sprite_index=app::spr_yellow_keyhole_used; break;
			case app::button::types::blue_keyhole:      sprite_index=app::spr_blue_keyhole_used; break;
			case app::button::types::red_keyhole:       sprite_index=app::spr_red_keyhole_used; break;
			case app::button::types::green_keyhole:     sprite_index=app::spr_green_keyhole_used; break;
		}
	}

	auto origin=d2d::video::to_screen(_button.ent.get_origin());

	sprite_draw.draw(
		_screen,
		origin,
		sprite_index
	);
}

void main::draw_gate(
	ldv::screen& _screen,
	const app::gate& _gate
) {

	auto origin=d2d::video::to_screen(_gate.ent.get_origin());

	int max_step=_gate.ent.get_h() / app::tile_h;
	for(int i=0; i<max_step; i++) { 

		int sprite_index=app::spr_gate;

		sprite_draw.draw(
			_screen,
			origin,
			sprite_index
		);

		origin.y+=app::tile_h;
	}
}

void main::draw_ladder(
	ldv::screen& _screen,
	const app::ladder& _ladder
) {

	auto origin=d2d::video::to_screen(_ladder.get_origin());

	int max_step=_ladder.get_h() / app::tile_h;
	for(int i=0; i<max_step; i++) { 

		int sprite_index=app::spr_ladder_yellow;
		switch(_ladder.type) {
			case app::ladder::t_ladder: sprite_index=app::spr_ladder_yellow; break;
			case app::ladder::t_chain:  sprite_index=app::spr_chain; break;
			case app::ladder::t_vine:   sprite_index=app::spr_vine; break;
		}

		sprite_draw.draw(
			_screen,
			origin,
			sprite_index
		);

		origin.y+=app::tile_h;
	}
}

void main::draw_collectible(
	ldv::screen& _screen,
	const app::collectible& _collectible
) {

	auto origin=d2d::video::to_screen(_collectible.ent.get_origin());

	int sprite_index=app::spr_gold_ingot;
	switch(_collectible.type) {

		case app::collectible::gold_ingot:  sprite_index=app::spr_gold_ingot; break;
		case app::collectible::gem:         sprite_index=app::spr_gem; break;
		case app::collectible::ruby:        sprite_index=app::spr_ruby; break;
		case app::collectible::diamond:     sprite_index=app::spr_diamond; break;
		case app::collectible::yellow_key:  sprite_index=app::spr_key_yellow; break;
		case app::collectible::blue_key:    sprite_index=app::spr_key_blue; break;
		case app::collectible::red_key:     sprite_index=app::spr_key_red; break;
		case app::collectible::green_key:   sprite_index=app::spr_key_green; break;
	}

	sprite_draw.draw(
		_screen,
		origin,
		sprite_index
	);
}

void main::draw_linear_monster(
	ldv::screen& _screen,
	const app::linear_monster& _monster
) {

	//Al sprites are facing right by default.
	d2d::video::sprite_draw::flags draw_flags{
		_monster.facing==app::faces::left,
		false
	};

	int animation_index=0;
	switch(_monster.type) {
		case app::linear_monster::types::scorpion:

			animation_index=app::anim_scorpion;
		break;
		case app::linear_monster::types::snake:

			animation_index=app::anim_snake;
		break;
		case app::linear_monster::types::bat:

			animation_index=app::anim_bat;
		break;
	}

	const auto& line=sprite_draw_animated.get(animation_index);
	draw_flags=sprite_draw_animated.flags(line, draw_flags);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_monster.ent.get_origin()),
		line.frame,
		draw_flags
	);
}

void main::draw_leaping_monster(
	ldv::screen& _screen,
	const app::leaping_monster& _monster
) {

	const auto& line=sprite_draw_animated.get(app::anim_piranha);
	auto draw_flags=sprite_draw_animated.flags(line);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_monster.ent.get_origin()),
		line.frame,
		draw_flags
	);
}

void main::draw_timed_trap(
	ldv::screen& _screen,
	const app::timed_trap& _trap
) {

	//There is always a plaque under this trap... We could fake it with a 
	//decoration object or with foreground tiles but those would be meh and
	//a nuisance in skill levels, respectively.
	
	auto origin=d2d::video::to_screen(_trap.ent.get_origin());

	//The plaque is under the trap itself and must be centered on it.
	origin.y-=app::tile_h;
	origin.x-=(app::timed_trap::fire_w / 2) + 1;

	int sprite_index=app::spr_fire_trap_plaque;

	sprite_draw.draw(
		_screen,
		origin,
		sprite_index
	);

	if(!_trap.is_harmful()) {

		return;
	}

	const auto& line=sprite_draw_animated.get(app::anim_timed_trap_fire);
	auto draw_flags=sprite_draw_animated.flags(line);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_trap.ent.get_origin()),
		line.frame,
		draw_flags
	);
}

void main::draw_breaking_platform(
	ldv::screen& _screen,
	const app::breaking_platform& _block
) {

	if(_block.is_gone()) {

		return;
	}

	if(_block.is_ok()) {

		sprite_draw.draw(
			_screen,
			d2d::video::to_screen(_block.get_origin()),
			app::spr_breaking_block
		);
		return;
	}

	int animation_index=0;
	float anim_len=0.f;

	if(_block.is_breaking()) {

		anim_len=_block.get_breaking_ms() / 1000.f;
		animation_index=app::anim_breaking_platform;
	}
	else {// if block.is_returning

		anim_len=_block.get_returning_ms() / 1000.f;
		animation_index=app::anim_breaking_platform_return;
	}

	const auto& line=sprite_draw_animated.get(
		animation_index, 
		_block.get_timer(), 
		anim_len
	);
	auto flags=sprite_draw_animated.flags(line);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_block.get_origin()),
		line.frame,
		flags
	);
}

void main::draw_platform(
	ldv::screen& _screen,
	const app::platform_block& _block
) {

	int index=app::spr_platform_branch;
	switch(_block.get_type()) {

		case app::platform_block::types::branch:
			index=app::spr_platform_branch;
		break;
	}

	sprite_draw.draw(
		_screen,
		d2d::video::to_screen(_block.get_origin()),
		index
	);
}

void main::draw_projectile(
	ldv::screen& _screen,
	const app::projectile& _projectile
) {

	switch(_projectile.get_type()) {

		case app::projectile::types::horizontal:
			return draw_projectile_linear(_screen, _projectile);
		case app::projectile::types::round:
			return draw_projectile_directed(_screen, _projectile);
	}
}

void main::draw_projectile_linear(
	ldv::screen& _screen,
	const app::projectile& _projectile
) {

	//Al sprites are facing right by default.
	d2d::video::sprite_draw::flags draw_flags{
		_projectile.velocity.x < 0., 
		false
	};

	if(!_projectile.is_moving()) {

		auto line=sprite_draw_animated.get(
			app::anim_projectile_end, 
			_projectile.get_timeout_value()
		);

		draw_flags=sprite_draw_animated.flags(line, draw_flags);

		sprite_draw.draw(
			_screen, 
			d2d::video::to_screen(_projectile.ent.get_origin()),
			line.frame,
			draw_flags
		);
		return;
	}

	auto line=sprite_draw_animated.get(app::anim_projectile);
	draw_flags=sprite_draw_animated.flags(line, draw_flags);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_projectile.ent.get_origin()),
		line.frame,
		draw_flags
	);
}

void main::draw_projectile_directed(
	ldv::screen& _screen,
	const app::projectile& _projectile
) {

	if(!_projectile.is_moving()) {

		auto line=sprite_draw_animated.get(
			app::anim_projectile_round_end, 
			_projectile.get_timeout_value()
		);

		sprite_draw.draw(
			_screen, 
			d2d::video::to_screen(_projectile.ent.get_origin()),
			line.frame,
			sprite_draw_animated.flags(line)
		);
		return;
	}

	auto line=sprite_draw_animated.get(app::anim_projectile_round);

	sprite_draw.draw(
		_screen, 
		d2d::video::to_screen(_projectile.ent.get_origin()),
		line.frame,
		sprite_draw_animated.flags(line)
	);
}

void main::draw_secret_cover(
	ldv::screen& _screen,
	const app::secret_cover& _secret_cover
) {

	//These are no sprites, just black rectangles...
	ldv::box_representation box{
		d2d::video::to_screen_rect(_secret_cover.ent),
		ldv::rgba_color(0,0,0,255),
		ldv::box_representation::type::fill
	};

	if(_secret_cover.is_dissapearing()) {

		int alpha=255 - (_secret_cover.get_timer() * 100);
		box.set_alpha(alpha);
	}

	box.set_blend(ldv::representation::blends::alpha);
	box.draw(_screen, camera);
}

void main::draw_player(
	ldv::screen& _screen,
	const app::player& _player
) {

	//Al sprites are facing right by default.
	d2d::video::sprite_draw::flags draw_flags{
		player.facing==app::faces::left,
		false
	};

	int animation_index=0;
	int frame_index=0;
	bool is_animation=false;

	switch(_player.state) {

		case app::player::states::ground:

			animation_index=_player.velocity.x!=0.
				? app::anim_m_walk
				: app::anim_m_idle;
			is_animation=true;
		break;
		case app::player::states::air:
			animation_index=app::anim_m_jump;
		break;
		case app::player::states::crouch:
			animation_index=app::anim_m_crouch;
		break;
		case app::player::states::defeat:
			animation_index=app::anim_m_defeat;
			is_animation=true;
		break;
		case app::player::states::ladder:
			animation_index=app::anim_m_climb;
			int y_mod=(int)player.ent.get_origin().y % 10;
			frame_index=abs(y_mod) <= 4 ? 0 : 1;
			draw_flags={false, false};
		break;
	}

	if(_player.gender==app::player::genders::female) {

		animation_index+=10; //they are exactly 10 indexes apart.
	}

	if(is_animation) {

		const auto& line=sprite_draw_animated.get(animation_index);
		draw_flags=sprite_draw_animated.flags(line, draw_flags);

		sprite_draw.draw(
			_screen, 
			d2d::video::to_screen(_player.ent.get_origin()),
			line.frame,
			draw_flags
		);

		return;
	}


	const auto& animation=sprite_draw_animated.animation(animation_index);
	const auto& line=animation.get(frame_index);
	draw_flags=sprite_draw_animated.flags(line, draw_flags);

	sprite_draw.draw(
		_screen,
		d2d::video::to_screen(player.ent.get_origin()),
		line.frame,
		draw_flags
	);
}

void main::grab_ladder(
	app::player& _player,
	const app::ladder& _ladder
) {

	_player.state=app::player::states::ladder;
	_player.velocity.x=0.0;
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
	_player.state=app::player::states::ground;
	_player.timeouts.reset(app::player::timeout_ladder);

	_player.facing=_x_force > 0 
		? app::faces::right
		: app::faces::left;

	_player.current_ladder=nullptr;
}

void main::jump_out_of_ladder(
	app::player& _player,
	int _x_force
) {

	//_x_force is -1 or 1.
	_player.velocity.x=(double)_x_force*simulation.walk_max_velocity;
	_player.facing=_x_force > 0 
		? app::faces::right
		: app::faces::left;
	_player.current_ladder=nullptr;
	_player.timeouts.reset(app::player::timeout_ladder);

	_player.jump(simulation.jump_force);
}

void main::drop_out_of_ladder(
	app::player& _player
) {

	_player.velocity.x=0.;
	_player.current_ladder=nullptr;
	_player.state=app::player::states::air;
	_player.timeouts.reset(app::player::timeout_ladder);
	//there is no last chance jump here.
}

void main::pick_up_collectible(
	app::player& _player,
	const app::collectible& _collectible
) {

	//Does not actually make the collectible dissapear :P.
	std::cout<<"got collectible with id "<<_collectible.id<<std::endl;
	persistence.add(app::pergr_collectibles, _collectible.id, 1);

	switch(_collectible.type) {

		case app::collectible::gold_ingot:
		case app::collectible::gem:
		case app::collectible::ruby:
		case app::collectible::diamond:

		break;
		case app::collectible::yellow_key:

			_player.yellow_keycount++;
			return;

		case app::collectible::blue_key:

			_player.blue_keycount++;
			return;
		case app::collectible::red_key:

			_player.red_keycount++;
			return;
		case app::collectible::green_key:

			_player.green_keycount++;
			return;
	}
}

void main::discover_secret(
	app::player&,
	app::secret_cover& _secret_cover
) {

	//Does not actually make the collectible dissapear :P.
	std::cout<<"discovered secret id "<<_secret_cover.id<<std::endl;
	persistence.add(app::pergr_secret_covers, _secret_cover.id, 1);
	_secret_cover.discover();
}

void main::land_on_ground(
	app::player& _player
) {

	_player.velocity.y=0.;
	_player.state=app::player::states::ground;
}

void main::touch_ceiling(
	app::player& _player
) {

	_player.velocity.y=0.;
	_player.velocity.x/=2.;
}

void main::start_falling(
	app::player& _player
) {

	_player.state=app::player::states::air;
	_player.timeouts.reset(app::player::timeout_last_jump_chance);
	_player.velocity.x/=2.; 
}

void main::activate_button(
	app::player& _player,
	app::button& _button
) {

	persistence.add(app::pergr_buttons, _button.id, 1);
	_button.used=true;

	//Consume the key...
	switch(_button.type) {

		case app::button::types::regular: break;
		case app::button::types::yellow_keyhole:
			_player.yellow_keycount--;
		break;
		case app::button::types::blue_keyhole: 
			_player.blue_keycount--;
		break;
		case app::button::types::red_keyhole: 
			_player.red_keycount--;
		break;
		case app::button::types::green_keyhole: 
			_player.green_keycount--;
		break;
	}

	activate_tag(_button.tag);
}

void main::activate_touch_trigger(
	app::touch_trigger& _trigger
) {

	persistence.add(app::pergr_touch_triggers, _trigger.id, 1);
	_trigger.used=true;
	activate_tag(_trigger.tag);
}

void main::collide_with_wall(
	app::player& _player
) {

	_player.velocity.x/=simulation.air_x_velocity_collision_reduce_factor;
}

void main::defeat(
	app::player& _player
) {

	//Avoid being defeated time and again.
	if(app::player::states::defeat==_player.state) {

		return; 
	}

	_player.timeouts.reset(app::player::timeout_defeat);
	_player.state=app::player::states::defeat;
	_player.velocity.y=simulation.defeat_y_velocity;
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
	auto contacting_tiles=adapter.find(player_box_copy, current_map.tile_finder, app::filter_tiles_check_on_air{});

	//fine collision phase now..
	d2d::collision::checker cc;

	if(
		 cc.has_collision(player_box_copy, contacting_tiles) 
		|| cc.has_collision(player_box_copy, current_map.platform_blocks)
	) {

		return false;
	}

	for(const auto& plat : current_map.breaking_platforms) {

		if(!plat.is_solid()) {

			continue;
		}

		if(d2d::collision::collides_with(plat, player_box_copy)) {

			return false;
		}
	}

	return true;
}

bool main::is_on_exit(
	const app::player& _player,
	const app::exit *&_exitptr,
	bool _is_touch
) const {

	for(const auto& exit : current_map.exits) {

		if(exit.touch==_is_touch && d2d::collision::collides_with(_player.ent, exit.ent)) {

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

	if(!_player.timeouts.is_expired(app::player::timeout_ladder)) {

		return false;
	}

	d2d::collision::checker cc;
	const auto ladders=cc.get_collisions(_player.ent, current_map.ladders);
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

			if(button.used) {

				continue;
			}

			if(!has_key(_player, button)) {

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

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	auto harm_tiles=adapter.find(_player.ent, current_map.tile_finder, app::filter_tiles_harm_only{});

	d2d::collision::checker cc;
	return cc.has_collision(_player.ent, harm_tiles);
}

bool main::has_key(
	const app::player& _player,
	const app::button& _button
) const {

	switch(_button.type) {

		case app::button::types::regular:

			//This kind does not need any type of key.
			return true;

		case app::button::types::yellow_keyhole:

			return _player.yellow_keycount > 0;
		case app::button::types::blue_keyhole:

			return _player.blue_keycount > 0;
		case app::button::types::red_keyhole:

			return _player.red_keycount > 0;
		case app::button::types::green_keyhole:

			return _player.green_keycount > 0;
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

	throw std::runtime_error("could not find entry by id");
}

void main::activate_tag(
	int _tag
) {

	for(auto& gate : current_map.gates) {

		if(gate.tag==_tag) {

			gate.open();
		}
	}

	//Activate all traps by this tag...
	for(auto& trap : current_map.timed_traps) {

		if(trap.get_tag()==_tag) {

			trap.activate();
		}
	}

	for(auto& generator : current_map.projectile_generators) {

		if(generator.get_tag()==_tag) {

			generator.activate();
		}
	}
}

#ifdef IS_DEBUG_BUILD

#include "main.debug.cpp"

#endif
