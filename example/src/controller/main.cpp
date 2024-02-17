#include "controller/main.h"
#include "app/input.h"
#include "app/thing_loader.h"
#include <d2d/storage/map_loader.h>
#include <d2d/video/camera_map_limit.h>
#include <d2d/motion/mover.h>
#include <d2d/collision/phase_horizontal.h>
#include <d2d/collision/phase_vertical.h>
#include <d2d/collision/checker.h>
#include <ldv/color.h>
#include <sstream>
#include <iostream>

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

	app::thing_loader tl{current_map};
	loader.load_thing_layer("things", tl);

	//After loading the map, tell the camera where the limits are.
	d2d::video::camera_map_limit cml;
	cml.limit_to_collision_tiles(dd.camera, current_map.collision_tiles, shaper.tile_w, shaper.tile_h, &logger);

	//We can also position the entity in the starting point.
	lm::log(logger).info()<<"setting starting point at "<<tl.starting_position.x<<", "<<tl.starting_position.y<<"\n";
	ent.set_origin({(double)tl.starting_position.x, (double)tl.starting_position.y});

	std::cout<<current_map<<std::endl;
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

	if(_input.is_input_pressed(app::input::escape)) {

		set_leave(true);
		return;
	}

	//Ok, now the input parts...
	app::player_input pli{};
	if(_input.is_input_pressed(app::input::down)) {

		pli.y=-1;
	}
	else if(_input.is_input_pressed(app::input::up)) {

		pli.y=1;
	}

	if(_input.is_input_pressed(app::input::left)) {

		pli.x=-1;
	}
	else if(_input.is_input_pressed(app::input::right)) {

		pli.x=1;
	}


	//TODO: Actually, ply.x and y would affect the state and velocity
	//without entering collision phase. 
	bool must_recenter_view=false;
	
	if(motion_phase_horizontal(pli, _lid.delta)) {

		must_recenter_view=true;
		d2d::collision::phase_horizontal cph(ent);
		cph.detect_all(current_map.collision_tiles, d2d::collision::checker::flag_skip_passable_side_check);
		cph.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);
		//cph.detect_all(current_map.platform_blocks); All passable from the side.
		if(cph.has_collision()) {

			cph.response_generic(); //but of course, the response in this case is sooo generic xD
		}
	}

	if(motion_phase_vertical(pli, _lid.delta)) {

		d2d::collision::phase_vertical cpv(ent);
		cpv.detect_all(current_map.collision_tiles);
		cpv.detect_all(current_map.solid_blocks, d2d::collision::checker::flag_skip_passable_side_check);
		cpv.detect_all(current_map.platform_blocks);
		if(cpv.has_collision()) {

			cpv.response_generic(); //but of course, the response in this case is sooo generic xD
		}
	}

	//TODO: Would this be part of some interface???
	//Always, I guess... 
	ent.sync_boxes();
	if(must_recenter_view) {

		dd.center_on(ent);
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

	dd.draw(_screen, ent);
}

bool main::motion_phase_horizontal(
	app::player_input _pli,
	float _delta
) {

	if(!_pli.x) {

		return false;
	}

	d2d::motion::mover mover{};
	const double velocity=170.0;
	mover.apply_x(ent, velocity*(double)_pli.x, _delta);
	return true;
}

bool main::motion_phase_vertical(
	app::player_input _pli,
	float _delta
) {

	//TODO: Does gravity go here? I guess so. We can always apply... to
	//apply conditionally we would have to do some magic like "check the tile
	//below the entity and all possible objects"... And guess what? We cannot
	//do that. Our options here are:
	//- have a super nice container for the tiles that can do the x-y thingy and USE it.
	//- fuck off and iterate like crazy.
	//- always apply and use local states in collisionable entities.

	if(!_pli.y) {

		return false;
	}

	d2d::motion::mover mover{};
	const double velocity=170.0;
	mover.apply_y(ent, velocity*(double)_pli.y, _delta);
	return true;
}
