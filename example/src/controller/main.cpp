#include "controller/main.h"
#include "app/input.h"
#include "app/player_input.h"
#include <d2d/storage/map_loader.h>
#include <d2d/video/camera_map_limit.h>
#include <d2d/motion/mover.h>
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

	//After loading the map, tell the camera where the limits are.
	d2d::video::camera_map_limit cml;
	cml.limit_to_collision_tiles(dd.camera, current_map.collision_tiles, shaper.tile_w, shaper.tile_h, &logger);

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

	if(pli) {

		//TODO: Who takes responsibility for the previous position and stuff?
		d2d::motion::mover mover{};
		const double speed=60.0;

		if(pli.x) {

			mover.apply_x(ent, speed*(double)pli.x, _lid.delta);
		}

		if(pli.y) {

			mover.apply_y(ent, speed*(double)pli.y, _lid.delta);
		}

		//TODO:
		//Sure, what about collisions now??? That would be the next step,
		//I guess.
	}
}

void main::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {
	
	dd.center_on(ent);

	dd.clear(_screen);
	for(const auto& cell : current_map.collision_tiles) {

		dd.draw(_screen, cell);
	}

	dd.draw(_screen, ent);
}
