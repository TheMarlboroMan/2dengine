#include "controller/main.h"
#include "app/input.h"
#include "app/thing_loader.h"
#include <d2d/storage/map_loader.h>
#include <d2d/video/camera_map_limit.h>
#include <d2d/motion/mover.h>
#include <d2d/collision/phase.h>
#include <d2d/collision/checker.h>
#include <d2d/collision/tiles_in_box.h>
#include <d2d/collision/tile_filter.h>
#include <tools/string_utils.h>
#include <ldv/color.h>
#include <sstream>
#include <iostream>
#include <cmath>
#include <functional>

using namespace controller;

main::main(
	app::service_provider& _sp
):
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	shaper{_sp.get_shaper()},
	tile_impl{_sp.get_tile_impl()},
	dd{480, 384},
	ent{80, -120}
{
	reload_values();

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
		current_map.collision_tiles,
		shaper,
		tile_impl
	);

	current_map.sync_tile_finder();

	app::thing_loader tl{current_map};
	loader.load_thing_layer("things", tl);

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

	if(tic_enabled) {

		if(_input.is_input_down(app::input::escape)) {

			tic_enabled=false;
			tic(_lid.delta, pli);
		}

		if(_input.is_input_down(app::input::tic)) {

			tic(_lid.delta, pli);
		}
	}
	else {

		if(_input.is_input_down(app::input::escape)) {

			set_leave(true);
			return;
		}

		if(_input.is_input_down(app::input::tic)) {

			tic_enabled=true;
		}
		else {

			tic(_lid.delta, pli);
		}
	}

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

	//Attempt to grab a ladder..
	//TODO: Grab ladder timeout, of course.

	if(1==_pli.y) {

		d2d::collision::checker cc;
		//TODO: Bad, no magic template :/.
		const auto ladders=cc.get_collisions(ent.get_box(), current_map.ladders);

		if(ladders.size()) {

			//TODO: Better a function for this changes state and sets the ladder and timeout!
			//grab_ladder();
			player_state=player_states::ladder;
			tic_ladder(_delta, _pli);
			return;
		}
	}

	gravity.apply_to(ent.velocity, _delta);

	d2d::collision::tiles_in_box adapter(shaper.get_tile_w(), shaper.get_tile_h());

	//horizontal phase...
	{
		d2d::motion::mover mover{};
		//TODO: Should be one of the values that can be loaded
		const double velocity=170.0;
		mover.apply_x(ent, velocity*(double)_pli.x, _delta);

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

			cpv.response_generic();
			ent.velocity.y=0.0;
			//TODO: Only when touching the floor!
			can_jump=true;
		}
	}
}

void main::tic_ladder(
	float _delta,
	app::player_input _pli
) {

	if(_pli.y) {

		//TODO: Move inside ladder and shit.
		//
		//TODO: apply the ladder constraint to the entity.
	}
	
	//TODO: jump out.
	if(_pli.jump && _pli.x) {

		//TODO: Better a function to leave the ladder, right?
		player_state=player_states::regular;
	}
}

void main::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {

	dd.clear(_screen);

	for(const auto& cell : current_map.collision_tiles) {

		dd.draw(_screen, cell);
	}

	for(const auto& block : current_map.solid_blocks) {

		dd.draw(_screen, block);
	}

	for(const auto& block : current_map.platform_blocks) {

		dd.draw(_screen, block);
	}

	for(const auto& ladder : current_map.ladders) {

		dd.draw(_screen, ladder);
	}

	dd.draw(_screen, ent);
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
