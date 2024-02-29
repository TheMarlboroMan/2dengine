#include "controller/main.h"
#include "app/input.h"
#include "app/thing_loader.h"
#include "app/map_attribute_loader.h"
#include "dfwimpl/config.h"
#include <d2d/storage/map_loader.h>
#include <d2d/video/camera_map_limit.h>
#include <d2d/video/tools.h>
#include <d2d/motion/mover.h>
#include <d2d/collision/phase.h>
#include <d2d/collision/checker.h>
#include <d2d/collision/tiles_in_box.h>
#include <d2d/collision/tile_filter.h>
#include <tools/string_utils.h>
#include <ldtools/ttf_manager.h>
#include <ldv/color.h>
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
	dd{app::logic_screen_w, app::logic_screen_h},
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
	},
	player{{0, 0, app::player_w, app::player_h}, {0., 0.}}
{

#ifdef IS_DEBUG_BUILD

	setup_console(_sp);

#endif

	reload_values();
	setup_timeouts();

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
	dd.set_background_color(ldv::rgba_color(128, 128, 128, 0));
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

	if(_input().is_exit_signal()) {
		set_leave(true);
		return;
	}


	app::player_input pli{};

#ifdef IS_DEBUG_BUILD

	if(console_enabled) {

		_input().start_text_input();
		console_display->input(_input());
		return;
	}

	if(_input.is_input_down(app::input::escape)) {

		set_leave(true);
		return;
	}

	if(_input.is_input_down(app::input::tic)) {

		console_enabled=true;
		return;
	}

	if(_input.is_input_down(app::input::reload_values)) {

		reload_values();
	}

	if(_input.is_input_pressed(app::input::down)) {

		pli.y=-1;
	}
	else if(_input.is_input_pressed(app::input::up)) {

		pli.y=1;
	}

	if(_input.is_input_down(app::input::jump)) {


		pli.jump=true;
	}

	if(_input.is_input_pressed(app::input::left)) {

		pli.x=-1;
	}
	else if(_input.is_input_pressed(app::input::right)) {

		pli.x=1;
	}

	tic(_lid.delta, pli);

#else

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

	if(_input.is_input_down(app::input::jump) {

		pli.jump=true;
	}

	tic(_lid.delta, pli);
#endif
}

void main::tic(
	float _delta,
	app::player_input _pli
) {

	timeouts.tic(_delta);
	scenery_tile_draw.tic(_delta);
	sprite_draw_animated.tic(_delta);

	switch(player.state) {

		case app::player::states::ground:
			tic_ground(_delta, _pli);
		break;

		case app::player::states::ladder:
			tic_ladder(_delta, _pli);
		break;

		case app::player::states::air:

			tic_air(_delta, _pli);
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
	app::player_input _pli
) {

	//Attempt to grab a ladder with up or down.
	if(0!=_pli.y && timeouts.is_ok(timeout_ladder)) {
	
		d2d::collision::checker cc;
		const auto ladders=cc.get_collisions(player.ent, current_map.ladders);

		if(ladders.size()) {

			grab_ladder(*ladders[0]);
			tic_ladder(_delta, _pli);
			return;
		}
	}

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());

	//Other thing that may happen: are we falling? Just try with a ghost box
	//a bit below the entity and do the tile check.
	{
		auto player_box_copy=player.ent.get_box();
		--player_box_copy.origin.y;
		auto contacting_tiles=adapter.find(player_box_copy, current_map.tile_finder);

		//We should ignore the passable that are now over feet level
		std::function<bool(const d2d::collision::tile*)> ignore_passable=[this](const d2d::collision::tile * _tile) -> bool {

			if(_tile->type != app::tile_half_bottom_passable
				&& _tile->type != app::tile_half_top_passable
			) {

				return true;
			}

			return _tile->get_y() < player.ent.get_origin().y;
		};

		auto valid_tiles=d2d::collision::filter_tiles(contacting_tiles, ignore_passable);
		d2d::collision::checker cc;
		if(!cc.has_collision(player_box_copy, valid_tiles)) {

			start_falling();
		}
	}

	//TODO: This would be a sequence of collisions against a STATIC world.
	//vertical and horizontal might fail.
	//horizontal phase...

	if(!_pli.x)  {

		//Player velocity is 100% tied to manual control... on the ground.
		player.velocity.x=0.0;
	}
	else {

		d2d::motion::mover mover{};
		player.velocity.x=walk_max_velocity*(double)_pli.x;

		player.facing=_pli.x > 0
			? app::faces::right
			: app::faces::left;

		//Instant acceleration.
		mover.apply_x(player.ent, player.velocity.x, _delta);

		d2d::collision::phase cph(player.ent, d2d::collision::checker::phases::horizontal);

		//This filters the map files and returns only those that would
		//collisde with the current position.
		auto current_tiles=adapter.find(player.ent, current_map.tile_finder);

		std::function<bool(const d2d::collision::tile*)> ignore_passable=[](const d2d::collision::tile * _tile) -> bool {

			return _tile->type != app::tile_half_bottom_passable
				&& _tile->type != app::tile_half_top_passable;
		};

		auto valid_tiles=d2d::collision::filter_tiles(current_tiles, ignore_passable);

		cph.detect_all(valid_tiles, d2d::collision::checker::flag_skip_passable_side_check);
		cph.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);

		if(cph.has_collision()) {

			cph.response_generic();
		}
	}

	if(_pli.jump && can_jump) {

		jump();
	}

	return;

	//TODO: Do we really need this here???
	gravity.apply_to(player.velocity, _delta);
	//vertical phase.
	{
		d2d::motion::mover mover{};
		mover.apply_y(player.ent, player.velocity.y, _delta);

		d2d::collision::phase cpv(player.ent, d2d::collision::checker::phases::vertical);

		auto current_tiles=adapter.find(player.ent, current_map.tile_finder);
		cpv.detect_all(current_tiles);
		cpv.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);
		cpv.detect_all(current_map.platform_blocks);

		if(cpv.has_collision()) {

			player.velocity.y=0.0;
			//cpv.response_generic();

			auto response=cpv.response_complex();
			response.solve(player.ent);

			//Only when colliding when the top of a box can we jump again.
			if(response.edges & d2d::collision::response::tedges::top) {

				can_jump=true;
			}
		}
	}
}

void main::tic_ladder(
	float _delta,
	app::player_input _pli
) {

	//While in a ladder all collisions with the world are ignored and
	//the horizontal velocity is unset.
	
	if(_pli.y) {

		d2d::motion::mover mover{};
		mover.apply_y(player.ent, ladder_max_velocity*(double)_pli.y, _delta);

		//apply the ladder movement constraints.
		current_ladder.apply(player.ent);
	}
	
	//Jump out.
	if(_pli.jump && _pli.x) {

		//There can be no ladder exit if there are collisions.
		d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
		const auto tiles_to_check=adapter.find(player.ent, current_map.tile_finder);
		d2d::collision::checker cc;
		if(!cc.has_collision(player.ent, tiles_to_check)) {

			jump_out_of_ladder(_pli.x);
		}
	}

	//Attempt to walk out.
	if(_pli.x) {

		auto player_box_copy=player.ent.get_box();
		player_box_copy.origin.y-=4;
		d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
		auto landing_candidates=adapter.find(player_box_copy, current_map.tile_finder);
		d2d::collision::checker cc;
		const auto& landing_tiles=cc.get_collisions(player_box_copy, landing_candidates);
		if(landing_tiles.size()) {

			walk_out_of_ladder(*landing_tiles[0], _pli.x);
		}
	}
}

void main::tic_air(
	float _delta,
	app::player_input _pli
) {

	//Attempt to grab a ladder with up or down.
	if(0!=_pli.y && timeouts.is_ok(timeout_ladder)) {
	
		d2d::collision::checker cc;
		const auto ladders=cc.get_collisions(player.ent, current_map.ladders);

		if(ladders.size()) {

			grab_ladder(*ladders[0]);
			tic_ladder(_delta, _pli);
			return;
		}
	}

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
	d2d::motion::mover mover{};

	//Do the horizontal phase...
	{

		//Air control is limited.
		if(_pli.x) {

			//Assume right facing...
			//TODO: almost... That was the velocity when the jump started :/.
			//TODO: Arghhh, cannot exceed 0 in negative because it fabs again :/.
			double max_velocity=player.velocity.x ? walk_max_velocity: walk_max_velocity / 2.;
			double velocity=fabs(player.velocity.x);

			velocity+=(double)_pli.x*2.0;
			if(velocity > max_velocity) {

				velocity=max_velocity;
			}

			player.velocity.x=velocity;

std::cout<<player.velocity.x<<std::endl;

			//And turn it left if need be.
			if(player.facing==app::faces::left) {

				player.velocity.x=-velocity;
			}
		}

		mover.apply_x(player.ent, player.velocity.x, _delta);

		d2d::collision::phase cph(player.ent, d2d::collision::checker::phases::horizontal);

		//This filters the map files and returns only those that would
		//collisde with the current position.
		auto current_tiles=adapter.find(player.ent, current_map.tile_finder);

		std::function<bool(const d2d::collision::tile*)> ignore_passable=[](const d2d::collision::tile * _tile) -> bool {

			return _tile->type != app::tile_half_bottom_passable
				&& _tile->type != app::tile_half_top_passable;
		};

		auto valid_tiles=d2d::collision::filter_tiles(current_tiles, ignore_passable);

		cph.detect_all(valid_tiles, d2d::collision::checker::flag_skip_passable_side_check);
		cph.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);

		if(cph.has_collision()) {

			//TODO: should shorten the x velocity.
			cph.response_generic();
		}
	}

	//TODO: Maybe letting go of jump shortens it???
	//do the vertical phase.
	{
		gravity.apply_to(player.velocity, _delta);
		
		mover.apply_y(player.ent, player.velocity.y, _delta);

		d2d::collision::phase cpv(player.ent, d2d::collision::checker::phases::vertical);

		auto current_tiles=adapter.find(player.ent, current_map.tile_finder);
			cpv.detect_all(current_tiles);
			cpv.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);
			cpv.detect_all(current_map.platform_blocks);

		if(cpv.has_collision()) {

			player.velocity.y=0.0;
			//cpv.response_generic();

			auto response=cpv.response_complex();
			response.solve(player.ent);

			//Only when colliding when the top of a box can we jump again.
			if(response.edges & d2d::collision::response::tedges::top) {

				land_on_ground();
			}

			//TODO: There should be some other interactions with edges here...
		}
	}
}

//TODO: We should separate debug draw and draw.
void main::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

	_screen.clear(current_map.background_color);
	//dd.clear(_screen);

	//TODO: Maybe some debug mode...
/*
	for(const auto& cell : current_map.collision_tiles) {

		dd.draw(_screen, cell);
	}
*/

	scenery_tile_draw.draw(_screen, dd.camera, current_map.background_tiles);

	for(const auto& block : current_map.solid_blocks) {

		dd.draw(_screen, block);
	}

	for(const auto& block : current_map.platform_blocks) {

		dd.draw(_screen, block);
	}

//TODO: Alpha is not working for these...
	for(const auto& ladder : current_map.ladders) {

//		dd.draw(_screen, ladder);
	}


//	dd.draw(_screen, ent);

	draw_player(_screen, player);
	scenery_tile_draw.draw(_screen, dd.camera, current_map.foreground_tiles);

#ifdef IS_DEBUG_BUILD
	
	if(console_enabled) {

		console_display->draw(_screen);
	}
#endif
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

void main::reload_values() {

#ifdef IS_DEBUG_BUILD

	std::cout<<current_map<<std::endl;
	
	std::stringstream ss;
	ss<<env.get_app_path()+"resources/runtime/values";
	std::cout<<"values file will be read from "<<ss.str()<<std::endl;

	std::ifstream values_file{ss.str().c_str()};

	if(!values_file.is_open()) {

		throw std::runtime_error("could not open values file!");
	}

	while(true) {

		std::string line;
		std::getline(values_file, line);

		if(values_file.eof()) {

			break;
		}

		const auto pieces=tools::explode(line, '=', 2);
		if(2 != pieces.size()) {

			continue;
		}

		const auto& name=pieces[0];
		const auto& value=pieces[1];

		if(name=="gravity_force_x") {

			gravity.force.x=std::stod(value);
		}
		else if(name=="gravity_force_y") {

			gravity.force.y=std::stod(value);
		}
		else if(name=="gravity_max_velocity") {

			gravity.max_velocity=std::stod(value);
		}
		else if(name=="jump_force") {

			jump_force=std::stod(value);
		}
		else if(name=="walk_max_velocity") {

			walk_max_velocity=std::stod(value);
		}
		else if(name=="ladder_max_velocity") {

			ladder_max_velocity=std::stod(value);
		}
		else {

			std::cout<<"bad name: "<<name<<std::endl;
		}
	}

	std::cout<<"gravity force:" <<gravity.force<<"\n"
		<<"gravity max velocity: "<<gravity.max_velocity<<"\n"
		<<"jump force: "<<jump_force
		<<std::endl;

#else

	//I guess we do nothing and run with the sweet defaults :).
	gravity.force={0.0, -300.0};
	gravity.max_velocity=-600.0;
	jump_force=120.0;

#endif
}

void main::grab_ladder(
	const app::ladder& _ladder
) {

	player.state=app::player::states::ladder;
	player.velocity.x=0.0;
	current_ladder=_ladder;
}

void main::walk_out_of_ladder(
	const d2d::collision::tile& _tile,
	int _x_force
) {
	d2d::collision::snap_to_top_of(player.ent, _tile);
	player.state=app::player::states::ground;
	current_ladder={0,0,0};
	timeouts.reset(timeout_ladder);

	player.facing=_x_force > 0 
		? app::faces::right
		: app::faces::left;
}

void main::jump_out_of_ladder(
	int _x_force
) {

	player.state=app::player::states::air;

	//_x_force is -1 or 1.
	double jump_out_velocity=(double)_x_force*walk_max_velocity;
	player.velocity={jump_out_velocity, jump_force};
	current_ladder={0,0,0};
	timeouts.reset(timeout_ladder);
	player.facing=_x_force > 0 
		? app::faces::right
		: app::faces::left;
}

void main::jump() {

	player.velocity.y=jump_force;
	can_jump=false;
	player.state=app::player::states::air;
}

void main::land_on_ground() {

	player.velocity.y=0.;
	can_jump=true;
	player.state=app::player::states::ground;
}

void main::start_falling() {

	can_jump=false;
	player.state=app::player::states::air;
}

void main::setup_timeouts() {

	timeouts.add(timeout_ladder, 0.5f, 0.0f);
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

#ifdef IS_DEBUG_BUILD

void main::console_display_onenter(
	const std::string& _command
) {

	console->send(_command);
}

console::result main::execute_cmd(
	const std::string& _cmd, 
	const std::vector<console::argument>& _args
) {

	if(_cmd=="exit") {

		console_enabled=false;
		return {0, "ok"};
	}

	if(_cmd=="tic") {

		tic(0.01f, app::player_input{});
		return {0, "ok"};
	}

	if(_cmd=="help") {

		std::stringstream ss;
		for(const auto& command : console->get_commands()) {

			ss<<command.name<<", ";
		}
		return {0, ss.str()};
	}

	if(_cmd=="helpcmd") {

		for(const auto& command : console->get_commands()) {

			if(command.name==_args[0].get_string()) {

				std::stringstream ss;
				ss<<command;
				return {0, ss.str()};
			}
		}

		return {1, "command not found"};
	}

	if(_cmd=="tell") {

		std::stringstream ss;
		ss<<player.ent;
		return{0, ss.str()};
	}	

	if(_cmd=="goto") {

		double x=_args[0].get_int(),
			y=_args[1].get_int();

		player.ent.set_origin({x, y});
		player.ent.sync_boxes();
		dd.center_on(player.ent);

		std::stringstream ss;
		ss<<"moved to "<<x<<","<<y;
		return {0, ss.str()};
	}

	if(_cmd=="moveby") {

		double x=_args[0].get_int(),
			y=_args[1].get_int();

		auto pt=player.ent.get_origin();
		pt+={x, y};

		player.ent.set_origin(pt);
		player.ent.sync_boxes();
		dd.center_on(player.ent);

		std::stringstream ss;
		ss<<"moved by "<<x<<","<<y;
		return {0, ss.str()};
	}

	if(_cmd=="get_collision_tiles") {

		d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
		lm::log(logger).debug()<<"will debug the collisions\n";
		adapter.set_debug_enabled(true).set_logger(&logger);

		auto current_tiles=adapter.find(player.ent, current_map.tile_finder);

		if(!current_tiles.size()) {

			return {0, "no collisions with tiles, check log for details"};
		}

		std::stringstream ss;
		ss<<"total: "<<current_tiles.size()<<" check stdout and log for details";
		for(const auto& node : current_tiles) {

			std::cout<<*node<<", ";
		}

		std::cout<<std::endl;

		return {0, ss.str()};
	}

	return {0, "..."};
}

void main::setup_console(
	app::service_provider& _sp
) {

	std::function<void(const std::string&)> display_update=[&](const std::string& _cmd) -> void {

		console_display_onenter(_cmd);
	};

	console_display.reset(
		new appconsole::console{
			{0,0,480, 100},
			_sp.get_ttf_manager().get("console_font", 14),
			display_update
		}
	);

	console.reset(new console::console(*this));
	console->connect_output(console_display->get_output());

	//TODO: it would be nice to have some helper in the console library to 
	//read these from a file, or something.
	console->map_command("exit", {});
	console->map_command("help", {});
	console->map_command("helpcmd", {{console::types::string}});
	console->map_command("tic", {});
	console->map_command("tell", {});
	console->map_command("get_collision_tiles", {});
	console->map_command("goto", {{console::types::integer}, {console::types::integer}});
	console->map_command("moveby", {{console::types::integer}, {console::types::integer}});
}
#endif
