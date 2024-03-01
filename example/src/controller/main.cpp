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
			return _index==29 || _index==147 || _index==151;
		}
	);

	//TODO: Same, this should come from a file.
	scenery_tile_draw.set_index_to_animation_fn(
		[](int _index) -> int {

			switch(_index) {
				case 151: return 3;
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
	load_map("map_001.json");
	take_player_to_entry(player, 1);
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

//	std::cout<<current_map<<std::endl;
}

void main::take_player_to_entry(
	app::player& _player,
	int _id
) {

	lm::log(logger).info()<<"will attempt to set player at position "<<_id<<"\n";
	auto entry=find_entry_by_id(_id);

	//TODO: Actually, there are MORE WAYS to center a player, but this shall suffice.
	auto origin=entry.ent.get_origin();
	lm::log(logger).info()<<"setting starting point at "<<origin<<"\n";
	player.ent.set_origin({(double)origin.x, (double)origin.y});

	//Center camera on map now..
//TODO: Real camera!
#ifdef IS_DEBUG_BUILD
	dd.center_on(player.ent);
#else

#endif
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

	//aftermath
	if(player.ent.get_origin() != player.ent.get_previous_box().origin) {
	
		player.ent.sync_boxes();

		if(app::player::states::defeat != player.state) {

			dd.center_on(player.ent);
		}
	}

	//at the end of the tic, are we touching an exit?
	const app::exit * exitptr{nullptr};
	if(is_on_touch_exit(player, exitptr)) {

		load_map(exitptr->map_filename);
		take_player_to_entry(player, exitptr->next_entry_id);
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

	if(-1==_pli.y) {

		crouch(_player);
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

		//It would be absurd to walk into harm's way but...
		if(is_into_harm(_player)) {

			defeat(_player);
			return;
		}

		//Collision...
		auto current_tiles=adapter.find(_player.ent, current_map.tile_finder, app::filter_tiles_ignore_one_way_above{});
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
	cph.detect_all(current_tiles, d2d::collision::checker::flag_skip_passable_side_check);
	cph.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);

	if(cph.has_collision()) {

		//TODO; A method would be nice.
		cph.response_generic();
		collide_with_wall(_player);
	}

	//do the vertical phase.
	//Last chance to jump after we begin falling...
	if(_pli.jump && _player.timeouts.is_counting(app::player::timeout_last_jump_chance) && _player.velocity.y < 0.) {

		jump(_player);
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
	current_tiles=adapter.find(_player.ent, current_map.tile_finder);
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

void main::tic_crouch(
	float _delta,
	app::player& _player,
	app::player_input _pli
) {

	if(-1!=_pli.y) {

		stand_up(_player);
		tic_ground(_delta, _player, _pli);
		return;
	}
}

void main::tic_defeat(
	float _delta,
	app::player& _player,
	app::player_input _pli
) {

	d2d::motion::mover mover{};
	simulation.gravity.apply_to(_player.velocity, _delta);
	mover.apply(_player.ent, _player.velocity, _delta);

	if(!player.timeouts.is_counting(app::player::timeout_defeat)) {

		//Todo, actually, reload xd
		//TODO:
		set_leave(true);
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

	int animation_index=0;
	int frame_index=0;
	bool is_animation=false;

	switch(_player.state) {

		case app::player::states::ground:

			animation_index=_player.velocity.x!=0.
				? app::anim_walk
				: app::anim_idle;
			is_animation=true;
		break;
		case app::player::states::air:
			animation_index=app::anim_jump;
		break;
		case app::player::states::crouch:
			animation_index=app::anim_crouch;
		break;
		case app::player::states::defeat:
			animation_index=app::anim_defeat;
		break;
		case app::player::states::ladder:
			animation_index=app::anim_climb;
			int y_mod=(int)player.ent.get_origin().y % 10;
			frame_index=y_mod <= 4 ? 0 : 1;
			draw_flags={false, false};
		break;
	}

	if(is_animation) {

		sprite_draw_animated.draw(
			_screen, 
			dd.camera, 
			d2d::video::to_screen(player.ent.get_origin()),
			animation_index,
			draw_flags
		);

		return;
	}

	sprite_draw_animated.draw_frame(
		_screen,
		dd.camera,
		d2d::video::to_screen(player.ent.get_origin()),
		animation_index,
		frame_index,
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
	jump(_player);
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

void main::jump(
	app::player& _player
) {

	_player.velocity.y=simulation.jump_force;
	_player.state=app::player::states::air;
	_player.jump_shortened=false;
}

void main::crouch(
	app::player& _player
) {

	_player.velocity.x=0.;
	_player.state=app::player::states::crouch;
	_player.ent.get_box().h=app::player_h_crouch;
}

void main::stand_up(
	app::player& _player
) {

	_player.velocity.x=0.;
	_player.state=app::player::states::ground;
	_player.ent.get_box().h=app::player_h;
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

void main::collide_with_wall(
	app::player& _player
) {

	_player.velocity.x/=simulation.air_x_velocity_collision_reduce_factor;
}

void main::defeat(
	app::player& _player
) {

	_player.timeouts.reset(app::player::timeout_defeat);
	_player.state=app::player::states::defeat;
	_player.velocity.y=simulation.defeat_y_velocity;
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
) const {

	auto player_box_copy=_player.ent.get_box();
	--player_box_copy.origin.y;

	//Quicky filter out tiles not in contact...
	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	auto contacting_tiles=adapter.find(player_box_copy, current_map.tile_finder, app::filter_tiles_check_on_air{});

	//fine collision phase now..
	d2d::collision::checker cc;
	return !cc.has_collision(player_box_copy, contacting_tiles) 
		&& !cc.has_collision(player_box_copy, current_map.solid_blocks)
		&& !cc.has_collision(player_box_copy, current_map.platform_blocks);
}

bool main::is_on_touch_exit(
	const app::player& _player,
	const app::exit *&_exitptr
) const {

	for(const auto& exit : current_map.exits) {

		if(exit.touch && d2d::collision::collides_with(_player.ent, exit.ent)) {

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

bool main::is_into_harm(
	const app::player& _player
) const {

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	auto harm_tiles=adapter.find(_player.ent, current_map.tile_finder, app::filter_tiles_harm_only{});

	d2d::collision::checker cc;
	return cc.has_collision(_player.ent, harm_tiles);
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

		return *it;
	}

	throw std::runtime_error("could not find entry by id");
}

#ifdef IS_DEBUG_BUILD

#include "main.debug.cpp"

#endif
