#include "controller/main.h"
#include "app/input.h"
#include "app/thing_loader.h"
#include <d2d/storage/map_loader.h>
#include <d2d/video/camera_map_limit.h>
#include <d2d/motion/mover.h>
#include <d2d/collision/checker.h>
#include <d2d/collision/solver.h>
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

	if(pli) {

		collision_phase(pli, _lid.delta);
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

void main::collision_phase(
	app::player_input _pli,
	float _delta
) {
	d2d::motion::mover mover{};
	const double speed=170.0;

	auto collision_finder=d2d::collision::checker{};
	auto collision_solver=d2d::collision::solver{};
	bool must_recenter=false;

	if(_pli.x) {

		mover.apply_x(ent, speed*(double)_pli.x, _delta);

		collision_finder.start(ent, d2d::collision::checker::phases::horizontal);
		bool has_collision=false;
		for(const auto& tile : current_map.collision_tiles) {

			has_collision|=collision_finder.add(tile);
		}

		for(const auto& block : current_map.solid_blocks) {

			has_collision|=collision_finder.add(block);
		}

		for(const auto& block : current_map.platform_blocks) {

			has_collision|=collision_finder.add(block);
		}

		if(has_collision) {

			//TODO: Notice we have COMPLEX solutions too!
			collision_solver.horizontal(ent, collision_finder.end());
			must_recenter=true;
		}
	}

	if(_pli.y) {

		mover.apply_y(ent, speed*(double)_pli.y, _delta);

		collision_finder.start(ent, d2d::collision::checker::phases::vertical);
		bool has_collision=false;
		for(const auto& tile : current_map.collision_tiles) {

			has_collision|=collision_finder.add(tile);
		}

		for(const auto& block : current_map.solid_blocks) {

			has_collision|=collision_finder.add(block);
		}

		for(const auto& block : current_map.platform_blocks) {

			has_collision|=collision_finder.add(block);
		}

		if(has_collision) {

			//TODO: Notice we have complex solutions too!
			collision_solver.vertical(ent, collision_finder.end());
			must_recenter=true;
		}
	}

	ent.sync_boxes();

	if(must_recenter) {

		dd.center_on(ent);
	}
}
