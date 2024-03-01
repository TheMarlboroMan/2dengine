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
#include <tools/string_utils.h>
#include <ldtools/ttf_manager.h>
#include <ldv/color.h>
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
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	shaper{_sp.get_shaper()},
	tile_impl{_sp.get_tile_impl()},
	sprite_draw{
		_sp.get_spritesheet_manager().at(app::ss_tiles),
		_sp.get_video_resource_manager().get_texture(app::tex_tiles)
	},
	sprite_draw_animated{
		_sp.get_spritesheet_manager().at(app::ss_tiles),
		_sp.get_video_resource_manager().get_texture(app::tex_tiles),
		_sp.get_animation_manager().at(app::animgr_tiles)
	},
	scenery_tile_draw{
		_sp.get_spritesheet_manager().at(app::ss_tiles),
		_sp.get_video_resource_manager().get_texture(app::tex_tiles),
		app::tile_w, 
		app::tile_h,
		_sp.get_animation_manager().at(app::animgr_tiles)
	}
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
			return _index==29 || _index==147;
		}
	);

	//TODO: Same, this should come from a file.
	scenery_tile_draw.set_index_to_animation_fn(
		[](int _index) -> int {

			switch(_index) {
				case 29: return 2;
				case 147: return 1;
			}

			return 0;
		}
	);

	setup_camera(
		app::logic_screen_w,
		app::logic_screen_h
	);

	//Attempt to load the starter map.
	load_map("map.json");
}

void main::load_map(
	const std::string& _map_name
) {
	std::stringstream ss;
	ss<<env.get_app_path()+"resources/maps/"<<_map_name;

	const std::string map_path{ss.str()};
	lm::log(logger).info()<<"will attempt to load map "<<map_path<<"\n";

	d2d::storage::map_loader loader(map_path);

	loader.load_collision_tiles(
		"logic",
		current_map.collision_tiles,
		shaper,
		tile_impl
	);

	loader.load_scenery_tiles("foreground", current_map.foreground_tiles);
	loader.load_scenery_tiles("background", current_map.background_tiles);

	current_map.sync_tile_finder();

	app::thing_loader tl{current_map};
	loader.load_thing_layer("things", tl);

	app::map_attribute_loader attrl{current_map.background_color};
	loader.load_properties(attrl);

	//After loading the map, tell the camera where the limits are.
	d2d::video::camera_map_limit cml;
	cml.limit_to_collision_tiles(dd.camera, current_map.collision_tiles, shaper.get_tile_w(), shaper.get_tile_h(), &logger);

	//We can also position the entity in the starting point.
	lm::log(logger).info()<<"setting starting point at "<<tl.starting_position.x<<", "<<tl.starting_position.y<<"\n";
	player.ent.set_origin({(double)tl.starting_position.x, (double)tl.starting_position.y});

//	std::cout<<current_map<<std::endl;
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
	else if(_input.is_input_down(app::input::up)) {

		pli.y=1;
	}

	if(_input.is_input_pressed(app::input::left)) {

		pli.x=-1;
	}
	else if(_input.is_input_pressed(app::input::right)) {

		pli.x=1;
	}

	if(_input.is_input_down(app::input::jump)) {

		pli.jump=true;
	}

	if(_input.is_input_pressed(app::input::jump)) {

		pli.hold_jump=true;
	}

	tic(_lid.delta, pli);
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
	}

	//aftermath
	if(player.ent.get_origin() != player.ent.get_previous_box().origin) {
	
		player.ent.sync_boxes();
		dd.center_on(player.ent);
	}
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

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());

	//Other thing that may happen: are we falling? Just try with a ghost box
	//a bit below the entity and do the tile check.
	if(is_on_air(_player)) {

		start_falling(_player);
		tic_air(_delta, _player,_pli);
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
		auto current_tiles=adapter.find(_player.ent, current_map.tile_finder, app::filter_one_way_tiles{});
		d2d::collision::phase cph(_player.ent, d2d::collision::checker::phases::horizontal);
		cph.detect_all(current_tiles, d2d::collision::checker::flag_skip_passable_side_check);
		cph.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);

		if(cph.has_collision()) {

			cph.response_generic();
		}
	}

	if(_pli.jump) {

		jump(_player);
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

	//Jump out.
	if(_pli.jump && _pli.x) {

		//There can be no ladder exit if there are collisions with tiles.
		const auto tiles_to_check=adapter.find(_player.ent, current_map.tile_finder);
		if(!cc.has_collision(_player.ent, tiles_to_check)) {

			jump_out_of_ladder(_player, _pli.x);
		}
	}

	//Attempt to walk out.
	if(_pli.x) {

		auto player_box_copy=_player.ent.get_box();
		player_box_copy.origin.y-=4;
		auto landing_candidates=adapter.find(player_box_copy, current_map.tile_finder);
		const auto& landing_tiles=cc.get_collisions(player_box_copy, landing_candidates);
		if(landing_tiles.size()) {

			walk_out_of_ladder(_player, *landing_tiles[0], _pli.x);
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
	{

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
		auto current_tiles=adapter.find(_player.ent, current_map.tile_finder, app::filter_ignore_one_way_tiles{});
		d2d::collision::phase cph(_player.ent, d2d::collision::checker::phases::horizontal);
		cph.detect_all(current_tiles, d2d::collision::checker::flag_skip_passable_side_check);
		cph.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);

		if(cph.has_collision()) {

			//TODO; A method would be nice.
			cph.response_generic();
			_player.velocity.x/=simulation.air_x_velocity_collision_reduce_factor;
		}
	}

	//do the vertical phase.
	{
		//Last chance to jump after we begin falling...
		if(_pli.jump && !_player.timeouts.is_ok(app::player::timeout_last_jump_chance) && _player.velocity.y < 0.) {

			jump(_player);
		}

		//Letting go of the jump shortens it...
		if(player.velocity.y > 0. && !_pli.hold_jump && !_player.jump_shortened) {

			_player.velocity.y/=simulation.air_y_velocity_jump_shorten_factor;;
			_player.jump_shortened=true;
		}

		simulation.gravity.apply_to(_player.velocity, _delta);
		mover.apply_y(_player.ent, _player.velocity.y, _delta);

		//Collision...
		auto current_tiles=adapter.find(_player.ent, current_map.tile_finder);
		d2d::collision::phase cpv(_player.ent, d2d::collision::checker::phases::vertical);
		cpv.detect_all(current_tiles);
		cpv.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);
		cpv.detect_all(current_map.platform_blocks);

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
	//TODO: Use a REAL camera.
	scenery_tile_draw.draw(_screen, dd.camera, current_map.background_tiles);

	//TODO:
	for(const auto& block : current_map.solid_blocks) {

		dd.draw(_screen, block);
	}

	//TODO:
	for(const auto& block : current_map.platform_blocks) {

		dd.draw(_screen, block);
	}

	draw_player(_screen, player);
	scenery_tile_draw.draw(_screen, dd.camera, current_map.foreground_tiles);
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

	switch(_player.state) {

		case app::player::states::ground: {

			int animation_index=_player.velocity.x!=0.
				? app::anim_walk
				: app::anim_idle;

			sprite_draw_animated.draw(
				_screen, 
				dd.camera, 
				d2d::video::to_screen(player.ent.get_origin()),
				animation_index,
				draw_flags
			);

			return;
		}

		case app::player::states::air:

			sprite_draw_animated.draw_frame(
				_screen,
				dd.camera,
				d2d::video::to_screen(player.ent.get_origin()),
				app::anim_jump,
				0,
				draw_flags
			);

			return;
		case app::player::states::ladder: {

			int y_mod=(int)player.ent.get_origin().y % 10;
			int frame_index=y_mod <= 4 ? 0 : 1;

			sprite_draw_animated.draw_frame(
				_screen,
				dd.camera,
				d2d::video::to_screen(player.ent.get_origin()),
				app::anim_climb,
				frame_index
			); //no flags needed

			return;
		}
	}
}

void main::grab_ladder(
	app::player& _player,
	const app::ladder& _ladder
) {

	_player.state=app::player::states::ladder;
	_player.velocity.x=0.0;
	_player.current_ladder=&_ladder;
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
	jump(_player);
}

void main::jump(
	app::player& _player
) {

	_player.velocity.y=simulation.jump_force;
	_player.state=app::player::states::air;
	_player.jump_shortened=false;
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

void main::setup_camera(
	int _screen_w,
	int _screen_h
) {

	//Let's use a buffer half the size of
	//the screen...
	unsigned w=_screen_w /2;
	unsigned h=_screen_h /2;
	int x=_screen_w /4;
	int y=_screen_h /4;

	ldv::rect margin{x, y, w, h};
	dd.set_center_margin(margin);
}

bool main::is_on_air(
	const app::player& _player
) {

	auto player_box_copy=_player.ent.get_box();
	--player_box_copy.origin.y;

	//Quicky filter out tiles not in contact...
	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	auto contacting_tiles=adapter.find(player_box_copy, current_map.tile_finder, app::filter_one_way_tiles{});

	//fine collision phase now..
	d2d::collision::checker cc;
	return !cc.has_collision(player_box_copy, contacting_tiles) 
		&& !cc.has_collision(player_box_copy, current_map.solid_blocks)
		&& !cc.has_collision(player_box_copy, current_map.platform_blocks);
}

bool main::can_grab_ladder(
	const app::player& _player,
	const app::ladder *&_ladderptr
) const {

	if(!_player.timeouts.is_ok(app::player::timeout_ladder)) {

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

#ifdef IS_DEBUG_BUILD

#include "main.debug.cpp"

#endif
