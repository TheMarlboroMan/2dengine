#include "controller/main.h"
#include "app/input.h"
#include <d2d/storage/map_loader.h>

using namespace controller;

main::main(
	app::service_provider& _sp
):
	env{_sp.get_env()},
	logger{_sp.get_logger()},
	shaper{_sp.get_shaper()}
{ 
	//Attempt to load the starter map.
	
	const std::string map_path{env.get_app_path()+"resources/maps/map.json"};
	lm::log(logger).info()<<"will attempt to load map "<<map_path<<"\n";
	d2d::storage::map_loader loader;

	loader.load_from_file_into_map(
		map_path,
		current_map,
		shaper
	);
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
	int _fps
) {

}
