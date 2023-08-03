#include "controller/main.h"
#include "app/input.h"
#include <d2d/storage/map_loader.h>
#include <ldv/color.h>

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
	
	const std::string map_path{env.get_app_path()+"resources/maps/map.json"};
	lm::log(logger).info()<<"will attempt to load map "<<map_path<<"\n";
	d2d::storage::map_loader loader;

	loader.load_from_file_into_map(
		map_path,
		current_map,
		shaper,
		tile_impl
	);

	dd.set_background_color(ldv::rgba_color(128, 128, 128, 0));
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

	dd.center_on(current_map.get_collision_tiles()[0]);
	dd.clear(_screen);
	for(const auto& cell : current_map.get_collision_tiles()) {

		dd.draw(_screen, cell);
	}
}
