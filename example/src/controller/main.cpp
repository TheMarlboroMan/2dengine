#include "controller/main.h"
#include "app/input.h"
#include "app/thing_loader.h"
#include "app/map_attribute_loader.h"
#include "dfwimpl/config.h"
#include <d2d/storage/map_loader.h>
#include <d2d/video/camera_map_limit.h>
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
	scenery_tile_draw{
		_sp.get_spritesheet_manager().at(app::ss_tiles),
		_sp.get_video_resource_manager().get_texture(app::tex_tiles),
		app::tile_w, 
		app::tile_h,
		_sp.get_animation_manager().at(app::anim_tiles)
	},
	ent{0, 0}
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
	ent.set_origin({(double)tl.starting_position.x, (double)tl.starting_position.y});

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

	switch(player_state) {

		case player_states::regular:
			tic_regular(_delta, _pli);
		break;

		case player_states::ladder:
			tic_ladder(_delta, _pli);
		break;
	}

	//aftermath
	if(ent.get_origin() != ent.get_previous_box().origin) {
	
		ent.sync_boxes();
		dd.center_on(ent);
	}
}

void main::tic_regular(
	float _delta,
	app::player_input _pli
) {

	//Attempt to grab a ladder with up or down.
	if(0!=_pli.y && timeouts.is_ok(timeout_ladder)) {
	
		d2d::collision::checker cc;
		const auto ladders=cc.get_collisions(ent, current_map.ladders);

		if(ladders.size()) {

			grab_ladder(*ladders[0]);
			tic_ladder(_delta, _pli);
			return;
		}
	}

	gravity.apply_to(ent.velocity, _delta);

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());

	//TODO: This would be a sequence of collisions against a STATIC world.
	//vertical and horizontal might fail.
	//horizontal phase...
	if(_pli.x) 
	{
		d2d::motion::mover mover{};

		//Instant acceleration.
		mover.apply_x(ent, walk_max_velocity*(double)_pli.x, _delta);


		d2d::collision::phase cph(ent, d2d::collision::checker::phases::horizontal);

		//This filters the map files and returns only those that would
		//collisde with the current position.
		auto current_tiles=adapter.find(ent, current_map.tile_finder);

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

	//vertical phase.
	//TODO: We should always check, there is always Y movement, such as gravity.
	{
		if(_pli.jump && can_jump) {

			//THis is a bad jump xD
			ent.velocity.y+=jump_force;
			can_jump=false;
		}

		d2d::motion::mover mover{};
		mover.apply_y(ent, ent.velocity.y, _delta);

		d2d::collision::phase cpv(ent, d2d::collision::checker::phases::vertical);

		auto current_tiles=adapter.find(ent, current_map.tile_finder);
		cpv.detect_all(current_tiles);
		cpv.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);
		cpv.detect_all(current_map.platform_blocks);

		if(cpv.has_collision()) {

			ent.velocity.y=0.0;
			//cpv.response_generic();

			auto response=cpv.response_complex();
			response.solve(ent);

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

	//While in a ladder all collisions with the world are ignored.
	
	if(_pli.y) {

		d2d::motion::mover mover{};
		mover.apply_y(ent, ladder_max_velocity*(double)_pli.y, _delta);

		//apply the ladder movement constraints.
		current_ladder.apply(ent);
	}
	
	//Jump out.
	if(_pli.jump && _pli.x) {

		//There can be no ladder exit if there are collisions.
		d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
		const auto tiles_to_check=adapter.find(ent, current_map.tile_finder);
		d2d::collision::checker cc;
		if(!cc.has_collision(ent, tiles_to_check)) {

			//TODO: Also jump in the direction we indicated!
			leave_ladder();
		}
	}

	//TODO: Attempt to WALK OUT.
	if(_pli.x) {

	}
}

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

	dd.draw(_screen, ent);

	scenery_tile_draw.draw(_screen, dd.camera, current_map.foreground_tiles);

#ifdef IS_DEBUG_BUILD
	
	if(console_enabled) {

		console_display->draw(_screen);
	}
#endif
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

	player_state=player_states::ladder;
	current_ladder=_ladder;
}

void main::leave_ladder() {

	player_state=player_states::regular;
	//TODO:  I wonder, will this be susceptible to collisions???
	current_ladder={0,0,0};
	timeouts.reset(timeout_ladder);
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
		ss<<ent;
		return{0, ss.str()};
	}	

	if(_cmd=="goto") {

		double x=_args[0].get_int(),
			y=_args[1].get_int();

		ent.set_origin({x, y});
		ent.sync_boxes();
		dd.center_on(ent);

		std::stringstream ss;
		ss<<"moved to "<<x<<","<<y;
		return {0, ss.str()};
	}

	if(_cmd=="moveby") {

		double x=_args[0].get_int(),
			y=_args[1].get_int();

		auto pt=ent.get_origin();
		pt+={x, y};

		ent.set_origin(pt);
		ent.sync_boxes();
		dd.center_on(ent);

		std::stringstream ss;
		ss<<"moved by "<<x<<","<<y;
		return {0, ss.str()};
	}

	if(_cmd=="get_collision_tiles") {

		d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());
		lm::log(logger).debug()<<"will debug the collisions\n";
		adapter.set_debug_enabled(true).set_logger(&logger);

		auto current_tiles=adapter.find(ent, current_map.tile_finder);

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
