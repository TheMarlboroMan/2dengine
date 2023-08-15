#include "controller/main.h"
#include "app/input.h"
#include <d2d/storage/map_loader.h>
#include <d2d/video/camera_map_limit.h>
#include <ldv/color.h>
#include <sstream>

using namespace controller;

main::main(
	app::service_provider& _sp
):
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	shaper{_sp.get_shaper()},
	tile_impl{_sp.get_tile_impl()},
	dd{480, 384}
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
	d2d::storage::map_loader loader;

	loader.load_from_file_into_map(
		map_path,
		current_map,
		shaper,
		tile_impl
	);

	//After loading the map, tell the camera where the limits are.
	d2d::video::camera_map_limit cml;
	cml.limit_to_collision_tiles(dd.camera, current_map, shaper.tile_w, shaper.tile_h, &logger);

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

	if(_input.is_input_down(app::input::escape)) {

		set_leave(true);
		return;
	}
}

void main::draw(
	ldv::screen& _screen,
	int /*_fps*/
) {
	
	//TODO: Yeah, sure. What should we be centering on???
	dd.center_on(current_map.get_collision_tiles()[0]);
	dd.clear(_screen);
	for(const auto& cell : current_map.get_collision_tiles()) {

		dd.draw(_screen, cell);
	}
}
